/*
 * lpsolve.cc
 *
 * Copyright 2021 Luka Marohnić
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "giacPCH.h"
#include "giac.h"
#include "lpsolve.h"
#include "optimization.h"
#include <ctime>
#include <set>

#ifndef DBL_MAX
#define DBL_MAX 1.79769313486e+308
#endif

using namespace std;

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif //ndef NO_NAMESPACE_GIAC

/*
 * Return the number of digits of an unsigned integer.
 */
int numdigits(unsigned i) {
    return 1+(i>0?(int)std::log10((double)i):0);
}

/*
 * Return true iff g is a (vector of) real number(s), +inf, or -inf.
 */
bool is_realcons(const gen &g,GIAC_CONTEXT) {
    if (g.type==_VECT) {
        const vecteur &v = *g._VECTptr;
        for (const_iterateur it=v.begin();it!=v.end();++it) {
            if (!is_realcons(*it,contextptr))
                return false;
        }
        return true;
    }
    return (is_inf(g) || g.type==_REAL || _evalf(g,contextptr).type==_DOUBLE_);
}

/*
 * If g is an interval, store its bounds to pair p and return true.
 * If g is not an interval, return false.
 */
bool interval2pair(const gen &g,pair<gen,gen> &p,GIAC_CONTEXT) {
    if (g.type!=_SYMB || !g.is_symb_of_sommet(at_interval))
        return false;  //g is not an interval
    const vecteur &v=*g._SYMBptr->feuille._VECTptr;
    p=make_pair(v[0],v[1]);
    return is_realcons(v,contextptr);
}

/*
 * Make a singleton vector (with 1 at position j and 0 at other positions).
 */
vecteur singleton(int n,int j) {
    vecteur v(n,0);
    v[j]=1;
    return v;
}

/*
 * Insert column c in matrix at position j.
 */
void insert_column(matrice &m,const vecteur &c,int j) {
    assert(m.size()==c.size());
    for (iterateur it=m.begin();it!=m.end();++it) {
        it->_VECTptr->insert(j>=0?it->_VECTptr->begin()+j:it->_VECTptr->end()+j,c[it-m.begin()]);
    }
}

/*
 * Append a column c to matrix m.
 */
void append_column(matrice &m,const vecteur &c) {
    assert(m.size()==c.size());
    for (iterateur it=m.begin();it!=m.end();++it) {
        it->_VECTptr->push_back(c[it-m.begin()]);
    }
}

/*
 * Remove the jth column from the matrix m.
 * If j<0, count from the last column towards the first.
 */
void remove_column(matrice &m,int j) {
    for (iterateur it=m.begin();it!=m.end();++it) {
        it->_VECTptr->erase(j+(j>=0?it->_VECTptr->begin():it->_VECTptr->end()));
    }
}

/*
 * Get the jth column from matrix m.
 * If j<0, count from the last column towards the the first.
 */
vecteur jth_column(const matrice &m,int j) {
    int n=m.front()._VECTptr->size();
    vecteur col(m.size());
    for (const_iterateur it=m.begin();it!=m.end();++it) {
        col[it-m.begin()]=it->_VECTptr->at(j>=0?j:n+j);
    }
    return col;
}

/*
 * Multiply the coefficients in v_orig by LCM of
 * denominators and then divide by their GCD.
 */
vecteur integralize(const vecteur &v_orig,GIAC_CONTEXT) {
    vecteur v(v_orig),vd;
    for (const_iterateur it=v.begin();it!=v.end();++it) {
        if (!is_zero(*it))
            vd.push_back(_denom(*it,contextptr));
    }
    if (vd.empty())
        return v;
    v=multvecteur(abs(_lcm(vd,contextptr),contextptr),v);
    return divvecteur(v,abs(_gcd(v,contextptr),contextptr));
}

void lp_variable::assign(const lp_variable &other) {
    _is_integral=other._is_integral;
    _sign_type=other._sign_type;
    _range=other._range;
    _name=other._name;
    _subs_coef=other._subs_coef;
    pseudocost[0]=other.pseudocost[0];
    pseudocost[1]=other.pseudocost[1];
    nbranch[0]=other.nbranch[0];
    nbranch[1]=other.nbranch[1];
}

/*
 * LP variable constructor. By default, it is a nonnegative variable
 * unrestricted from above.
 */
lp_variable::lp_variable() {
    _is_integral=false;
    _sign_type=_LP_VARSIGN_POS;
    _range=lp_range();
    _range.set_lb(0);
    fill_n(nbranch,2,0);
}

/*
 * Update lower (dir=0) or upper (dir=1) pseudocost.
 */
void lp_variable::update_pseudocost(double delta,double fr,int dir) {
    double sigma=pseudocost[dir]*nbranch[dir];
    sigma+=delta/(dir==0?fr:(1-fr));
    pseudocost[dir]=sigma/(++nbranch[dir]);
}

/*
 * Return score, a positive value based on pseudocost values. Variable with the
 * best (highest) score is selected for branching.
 */
double lp_variable::score(double fr) const {
    if (nbranch[0]==0 || nbranch[1]==0)
        return 0;
    double qlo=fr*pseudocost[0],qhi=(1-fr)*pseudocost[1];
    return (1-LP_SCORE_FACTOR)*std::min(qlo,qhi)+LP_SCORE_FACTOR*std::max(qlo,qhi);
}

/* Set the type of this variable. */
void lp_variable::set_type(int t,GIAC_CONTEXT) {
    switch (t) {
    case _LP_BINARYVARIABLES:
        _range.tighten_lbound(0,contextptr);
        _range.tighten_ubound(1,contextptr);
    case _LP_INTEGERVARIABLES:
        _is_integral=true;
        break;
    }
}

/*
 * Range constructor: by default, it contains no restriction.
 */
lp_range::lp_range () {
    lbound=minus_inf;
    ubound=plus_inf;
}

/*
 * Settings constructor loads some sensible defaults.
 */
lp_settings::lp_settings() {
    verbose=false;
    status_report_freq=0.2;
    solver=_LP_SIMPLEX;
    precision=_LP_PROB_DEPENDENT;
    presolve=true;
    maximize=false;
    acyclic=true;
    relative_gap_tolerance=0.0;
    has_binary_vars=false;
    varselect=-1;
    nodeselect=-1;
    depth_limit=0;
    node_limit=0;
    iteration_limit=0;
    time_limit=0;
    max_cuts=5;
}

/*
 * Stats constructor initializes the status container for the problem being
 * solved. It is used to monitor the progress and to summarize when done.
 */
lp_stats::lp_stats() {
    subproblems_examined=0;
    cuts_applied=0;
    max_active_nodes=0;
    mip_gap=-1; //negative means undefined
}

/*
 * Pivot on element with coordinates I,J.
 */
void pivot_ij(matrice &m,int I,int J,bool negate=false) {
    gen a=m[I][J];
    m[I]=divvecteur(*m[I]._VECTptr,a);
    vecteur c=jth_column(m,J),&pv=*m[I]._VECTptr;
    for (iterateur it=m.begin();it!=m.end();++it) it->_VECTptr->at(J)=0;
    pv[J]=gen(negate?-1:1)/a;
    for (int i=m.size();i-->0;)
        if (i!=I && !is_zero(c[i]))
            m[i]=subvecteur(*m[i]._VECTptr,multvecteur(c[i],pv));
}

static clock_t srbt;

/*
 * Change basis by choosing entering and leaving variables and swapping them.
 * Return true iff there is no need to change basis any further.
 */
bool lp_node::change_basis(matrice &m,const vecteur &u,vector<bool> &is_slack,ints &basis,ints &cols) {
    // ev, lv: indices of entering and leaving variables
    // ec, lr: 'entering' column and 'leaving' row in matrix m, respectively
    int ec,ev,lr,lv,nc=cols.size(),nr=basis.size(),nv=nr+nc;
    gen a,b,ratio,mincoeff=0;
    // choose a variable to enter the basis
    ev=-1;
    const vecteur &last=*m.back()._VECTptr;
    for (int j=0;j<nc;++j) {
        int k=cols[j];
        const gen &l=last[j];
        if ((use_bland && is_strictly_positive(-l,prob->ctx) &&
                (ev<0 || k+(is_slack[k]?nv:0)<ev+(is_slack[ev]?nv:0))) ||
                (!use_bland && is_strictly_greater(mincoeff,l,prob->ctx))) {
            ec=j;
            ev=k;
            mincoeff=l;
        }
    }
    if (ev<0) // the current solution is optimal
        return true;
    // choose a variable to leave the basis
    mincoeff=plus_inf;
    lv=-1;
    bool hits_ub,ub_subs;
    for (int i=0;i<nr;++i) {
        a=m[i][ec];
        b=m[i]._VECTptr->back();
        int j=basis[i];
        if (is_strictly_positive(a,prob->ctx) && is_greater(mincoeff,ratio=b/a,prob->ctx))
            hits_ub=false;
        else if (is_strictly_positive(-a,prob->ctx) && !is_inf(u[j]) &&
                    is_greater(mincoeff,ratio=(b-u[j])/a,prob->ctx))
            hits_ub=true;
        else continue;
        if (is_strictly_greater(mincoeff,ratio,prob->ctx)) {
            lv=-1;
            mincoeff=ratio;
        }
        if (lv<0 || (use_bland && j+(is_slack[j]?nv:0)<lv+(is_slack[lv]?nv:0))) {
            lv=j;
            lr=i;
            ub_subs=hits_ub;
        }
    }
    if (lv<0 && is_inf(u[ev])) { // the solution is unbounded
        optimum=minus_inf;
        return true;
    }
    if (prob->settings.acyclic)
        use_bland=is_zero(mincoeff); // Bland's rule
    if (lv<0 || is_greater(mincoeff,u[ev],prob->ctx)) {
        for (const_iterateur it=m.begin();it!=m.end();++it) {
            a=it->_VECTptr->at(ec);
            it->_VECTptr->back()-=u[ev]*a;
            it->_VECTptr->at(ec)=-a;
        }
        if (ev<nv)
            is_slack[ev]=!is_slack[ev];
        return false;
    }
    // swap variables: basic leaves, nonbasic enters
    if (ub_subs) {
        m[lr]._VECTptr->back()-=u[lv];
        if (lv<nv)
            is_slack[lv]=!is_slack[lv];
    }
    pivot_ij(m,lr,ec,ub_subs);
    basis[lr]=ev;
    cols[ec]=lv;
    return false;
}

/*
 * Simplex algorithm that handles upper bounds of the variables. The solution x
 * satisfies 0<=x<=u. An initial basis must be provided.
 *
 * Basis is a vector of integers B and B[i]=j means that j-th variable is basic
 * and appears in i-th row (constraint). Basic columns are not kept in matrix,
 * which contains only the columns of nonbasic variables. A nonbasic variable
 * is assigned to the respective column with integer vector 'cols': cols[i]=j
 * means that i-th column of the matrix is associated with the j-th variable. The
 * algorithm uses the upper-bounding technique when pivoting and an adaptation
 * of Bland's rule to prevent cycling. i-th element of 'is_slack' is true iff
 * the i-th (nonbasic) variable is at its upper bound.
 *
 * If limit>0, the simplex algorithm will terminate after that many iterations.
 */
void lp_node::simplex_reduce_bounded(matrice &m,const vecteur &u,vector<bool> &is_slack,
                                     ints &basis,ints &cols,int phase,const gen &obj_ct) {
    int nr=basis.size(),nc=cols.size(),nv=nr+nc;
    int limit=prob->settings.iteration_limit;
    int &icount=prob->iteration_count;
    // iterate the simplex method
    use_bland=false;
    optimum=undef;
    char buffer[256],numbuf[8];
    double obj0=-1;
    while (true) {
        ++icount;
        if (limit>0 && icount>limit)
            break;
        if (phase>0) {
            clock_t now=clock();
            double obj=_evalf(phase==1?-m[nr][nc]:(prob->settings.maximize?-1:1)*(obj_ct-m[nr][nc]),prob->ctx).DOUBLE_val();
            if (phase==1 && obj0<obj) {
                if (is_zero(m[nr][nc]))
                    break;
                obj0=obj;
            }
            if (CLOCKS_PER_SEC/double(now-srbt)<=0.5) { // display progress info
                sprintf(numbuf,"%d",icount);
                string ns(numbuf);
                while (ns.length()<7) ns.insert(0,1,' ');
                sprintf(buffer," %s %s  obj: %g%s",phase==1?" ":"*",ns.c_str(),phase==1?100*obj/obj0:obj,phase==1?"%":"");
                prob->message(buffer);
                srbt=now;
            }
        }
        if (change_basis(m,u,is_slack,basis,cols) || (std::abs(phase)==1 && is_zero(m[nr][nc])))
            break;
    }
    if (is_undef(optimum)) {
        optimum=m[nr][nc];
        solution=vecteur(nv,0);
        for (int i=0;i<nr;++i) {
            assert(basis[i]<nv);
            solution[basis[i]]=m[i][nc];
        }
        for (int j=0;j<nv;++j) {
            if (is_slack[j])
                solution[j]=u[j]-solution[j];
        }
    }
}

/*
 * Solve the relaxed subproblem corresponding to this node.
 *
 * This function uses two-phase simplex method and applies suitable Gomory
 * mixed integer cuts generated after (each re)optimization. Weak GMI cuts are
 * discarded either because of small away or because not being parallel enough
 * to the objective. Cuts with too large coefficients (when integralized) are
 * discarded too because they slow down the computational process. Generated
 * cuts are kept in the problem structure to be used by child suboproblems
 * during the branch&bound algorithm.
 */
int lp_node::solve_relaxation() {
    int nrows=prob->constr.nrows(),ncols=prob->constr.ncols(),bs;
    matrice m;
    vecteur obj=prob->objective.first,l(ncols),u(ncols),br,row,b,lh,gmi_cut;
    gen rh,obj_ct(0),mgn,minmgn;
    ints cols(ncols),basis;
    vector<bool> is_slack(ncols,false);
    map<int,int> slack_cut;
    bool is_mip=prob->has_integral_variables();
    // determine the upper and the lower bound
    for (int j=0;j<ncols;++j) {
        const lp_variable &var=prob->variables[j];
        const lp_range &rng=ranges[j];
        l[j]=is_strictly_greater(var.lb(),rng.lb(),prob->ctx)?var.lb():rng.lb();
        u[j]=is_strictly_greater(var.ub(),rng.ub(),prob->ctx)?rng.ub():var.ub();
        if (is_strictly_greater(l[j],u[j],prob->ctx))
            return _LP_INFEASIBLE;
    }
    // populate matrix with constraint coefficients
    m=*_matrix(makesequence(nrows,ncols,0),prob->ctx)._VECTptr;
    for (int i=0;i<nrows;++i) for (int j=0;j<ncols;++j)
        m[i]._VECTptr->at(j)=prob->constr.lhs[i][j];
    b=prob->constr.rhs;
    // shift the variables according to their lower bounds such that l<=x<=u becomes 0<=x'<=u'
    for (int j=0;j<ncols;++j) {
        b=subvecteur(b,multvecteur(l[j],jth_column(m,j)));
        u[j]-=l[j];
        obj_ct+=obj[j]*l[j];
        cols[j]=j;
    }
    append_column(m,b);
    // assure that the right-hand side column has nonnegative coefficients
    for (iterateur it=m.begin();it!=m.end();++it) {
        if (!is_positive(it->_VECTptr->back(),prob->ctx)) {
            *it=multvecteur(-1,*(it->_VECTptr));
        }
    }
    // append cuts inherited from the parent node
    for (ints::const_iterator it=cut_indices.begin();it!=cut_indices.end();++it) {
        insert_column(m,vecteur(nrows,0),-1);
        slack_cut[ncols]=*it;
        prob->cuts.get_lr(*it,lh,rh);
        for (int i=0;i<prob->nv();++i) {
            rh-=lh[i]*l[i];
        }
        row=vecteur(cols.size(),0);
        for (int i=0;i<int(cols.size());++i) {
            int j=cols.at(i);
            if (j<prob->nv())
                row[i]=lh[j];
        }
        row.push_back(-1);
        row.push_back(rh);
        if (is_strictly_positive(-rh,prob->ctx)) {
            for (iterateur jt=row.begin();jt!=row.end();++jt) {
                *jt=-(*jt);
            }
        }
        m.push_back(row);
        l.push_back(0);
        u.push_back(plus_inf);
        is_slack.push_back(false);
        obj.push_back(0);
        cols.push_back(ncols++);
        ++nrows;
    }
    // optimize-add cut-reoptimize-add cut...
    // repeat until no more cuts are generated or the max_cuts limit is reached
    srbt=clock();
    while (true) {
        br=vecteur(int(cols.size())+1,0);
        bs=basis.size();
        basis.resize(nrows);
        u.resize(ncols+nrows-bs);
        for (int i=bs;i<nrows;++i) {
            br=subvecteur(br,*m[i]._VECTptr);
            basis[i]=ncols+i-bs;
            u[ncols+i-bs]=plus_inf;
        }
        m.push_back(br);
        // phase 1: minimize the sum of artificial variables
        simplex_reduce_bounded(m,u,is_slack,basis,cols,is_mip || !prob->settings.verbose?-1:1,obj_ct);
        if (!is_zero(optimum))
            return _LP_INFEASIBLE; // at least one artificial variable is basic and positive
        m.pop_back();
        for (int i=0;i<nrows;++i) {
            int j=basis[i];
            if (j<ncols)
                continue;
            // the i-th basic variable is artificial, push it out of the basis
            int k=0;
            for (;k<ncols && (is_zero(m[i][k]) || cols[k]>=ncols);++k);
            if (k==ncols)
                return _LP_ERROR;
            pivot_ij(m,i,k);
            basis[i]=cols[k];
            cols[k]=j;
        }
        // remove artificial columns from m
        for (int j=cols.size();j-->0;) {
            if (cols[j]>=ncols) {
                remove_column(m,j);
                cols.erase(cols.begin()+j);
            }
        }
        // append the bottom row to maximize -obj
        br=vecteur(ncols-nrows+1);
        for (int j=0;j<ncols-nrows;++j) {
            int k=cols[j];
            br[j]=obj[k];
            if (is_slack[k]) {
                br.back()-=br[j]*u[k];
                br[j]=-br[j];
            }
        }
        for (int i=0;i<nrows;++i) {
            int j=basis[i];
            if (is_slack[j])
                br.back()-=obj[j]*u[j];
            br=subvecteur(br,multvecteur(is_slack[j]?-obj[j]:obj[j],*m[i]._VECTptr));
        }
        m.push_back(br);
        u.resize(ncols);
        // phase 2: optimize the objective
        simplex_reduce_bounded(m,u,is_slack,basis,cols,is_mip || !prob->settings.verbose?-2:2,obj_ct);
        if (is_inf(optimum))
            return _LP_UNBOUNDED; // the solution is unbounded
        if (!is_mip || int(cut_indices.size())>=prob->settings.max_cuts)
            break;
        m.pop_back(); // remove the bottom row
        // try to generate a GMI cut
        gmi_cut.clear();
        for (int i=0;i<nrows;++i) {
            gen p(fracpart(solution[basis[i]]));
            vecteur eq(*m[i]._VECTptr);
            if (!is_zero(p) && !is_integer(eq.back())) {
                gen f0(fracpart(eq.back())),fj,sp(0),eqnorm(0);
                double away=(is_strictly_greater(f0,1-f0,prob->ctx)?1-f0:f0).to_double(prob->ctx);
                if (away<LP_MIN_AWAY)
                    continue; // too small away, discard this cut
                eq.back()=1;
                for (int k=0;k<int(cols.size());++k) {
                    int j=cols[k];
                    if (j<prob->nv() && prob->variables[j].is_integral()) {
                        fj=fracpart(eq[k]);
                        eq[k]=(is_strictly_greater(fj,f0,prob->ctx)?(fj-1)/(f0-1):fj/f0);
                    }
                    else
                        eq[k]=eq[k]/(is_strictly_positive(eq[k],prob->ctx)?f0:(f0-1));
                    if (j<prob->nv()) {
                        sp+=eq[k]*obj[j];
                        eqnorm+=pow(eq[k],2);
                    }
                }
                double dsp=sp.to_double(prob->ctx),deqnorm=eqnorm.to_double(prob->ctx);
                if (std::abs(dsp/(prob->objective_norm*std::sqrt(deqnorm)))<LP_MIN_PARALLELISM)
                    continue; // this cut is not parallel enough to the objective
                eq=integralize(eq,prob->ctx); // convert the cut into an equivalent integral representation
                mgn=_max(_abs(eq,prob->ctx),prob->ctx);
                if (mgn.to_double(prob->ctx)>LP_MAX_MAGNITUDE)
                    continue; // this cut has too large coefficients
                if (gmi_cut.empty() || is_strictly_greater(minmgn,mgn,prob->ctx)) {
                    minmgn=mgn;
                    gmi_cut=eq;
                }
            }
        }
        if (gmi_cut.empty())
            break; // no acceptable cut was generated; there's no need to reoptimize further
        // store the GMI cut
        lh=vecteur(ncols,0);
        rh=gmi_cut.back();
        for (int k=0;k<int(cols.size());++k) {
            int j=cols[k];
            if(!is_zero(lh[j]=gmi_cut[k])) {
                if (is_slack[j]) {
                    rh-=u[j]*lh[j];
                    lh[j]=-lh[j];
                }
                rh+=l[j]*lh[j];
            }
        }
        vecteur slacks(lh.begin()+prob->nv(),lh.end()),orig_lh;
        gen orig_rh;
        lh.resize(prob->nv());
        for (int k=0;k<int(slacks.size());++k) {
            int j=prob->nv()+k;
            prob->cuts.get_lr(slack_cut[j],orig_lh,orig_rh);
            rh+=slacks[k]*orig_rh;
            lh=addvecteur(lh,multvecteur(slacks[k],orig_lh));
        }
        cut_indices.push_back(prob->cuts.nrows());
        prob->cuts.append(lh,rh,_LP_GEQ);
        ++prob->stats.cuts_applied;
        // append GMI cut to the simplex tableau and reoptimize
        gmi_cut.insert(gmi_cut.end()-1,-1);
        insert_column(m,vecteur(nrows,0),-1);
        m.push_back(gmi_cut);
        slack_cut[ncols]=cut_indices.back();
        l.push_back(0);
        u.push_back(plus_inf);
        is_slack.push_back(false);
        obj.push_back(0);
        cols.push_back(ncols++);
        ++nrows;
    }
    for (int i=0;i<prob->nv();++i) {
        solution[i]+=l[i];
    }
    solution.resize(prob->nv());
    optimum=obj_ct-optimum;
    // compute some useful data for branch&bound
    opt_approx=optimum.to_double(prob->ctx);
    infeas=0;
    most_fractional=-1;
    gen p,ifs,max_ifs(0);
    for (int i=0;i<prob->nv();++i) {
        if (!prob->variables[i].is_integral())
            continue;
        p=fracpart(solution[i]);
        ifs=is_strictly_greater(p,1-p,prob->ctx)?1-p:p;
        if (is_zero(ifs))
            continue;
        fractional_vars[i]=p.to_double(prob->ctx);
        infeas+=ifs;
        if (is_strictly_greater(ifs,max_ifs,prob->ctx)) {
            most_fractional=i;
            max_ifs=ifs;
        }
    }
    return _LP_SOLVED;
}

double lp_node::get_fractional_var(int index) const {
    if (fractional_vars.find(index)!=fractional_vars.end())
        return fractional_vars.at(index);
    return 0;
}

void lp_node::assign(const lp_node &other) {
    prob=other.prob;
    depth=other.depth;
    ranges=other.ranges;
    optimum=other.optimum;
    solution=other.solution;
    opt_approx=other.opt_approx;
    infeas=other.infeas;
    most_fractional=other.most_fractional;
    fractional_vars=other.fractional_vars;
    cut_indices=other.cut_indices;
}

/*
 * Return fractional part of g, i.e. [g]=g-floor(g). It is always 0<=[g]<1.
 */
gen lp_node::fracpart(const gen &g) const {
    return g-_floor(g,prob->ctx);
}

/*
 * Create a child node with copy of ranges and cut indices and depth increased
 * by one.
 */
lp_node lp_node::create_child() {
    lp_node node(prob);
    node.depth=depth+1;
    node.ranges=this->ranges;
    node.cut_indices=this->cut_indices;
    return node;
}

/*
 * Return true iff there are integrality constraints, i.e. iff this is a
 * (mixed) integer problem.
 */
bool lp_problem::has_integral_variables() {
    for (vector<lp_variable>::const_iterator it=variables.begin();it!=variables.end();++it) {
        if (it->is_integral())
            return true;
    }
    return false;
}

/*
 * Return true iff problem has approximate (floating-point) coefficients.
 */
bool lp_problem::has_approx_coefficients() {
    if (is_approx(objective.first) ||
            objective.second.is_approx() ||
            is_approx(constr.lhs) ||
            is_approx(constr.rhs))
        return true;
    for (vector<lp_variable>::const_iterator it=variables.begin();it!=variables.end();++it) {
        if (it->lb().is_approx() || it->ub().is_approx())
            return true;
    }
    return false;
}

/*
 * Set objective function parameters.
 */
void lp_problem::set_objective(const vecteur &v,const gen &ft) {
    objective.first=v;
    objective.second=ft;
    for (const_iterateur it=v.begin();it!=v.end();++it) {
        obj_approx.push_back(abs(*it,ctx).to_double(ctx));
    }
}

/*
 * Display a message.
 */
void lp_problem::message(const char *msg,bool force_print) {
    if (force_print || settings.verbose)
        *logptr(ctx) << msg << "\n";
}

/*
 * Duplicate the jth column.
 */
void lp_constraints::duplicate_column(int index) {
    assert(index<ncols());
    vecteur col=jth_column(lhs,index);
    insert_column(lhs,col,index);
}

/*
 * Multiply the jth column by -1.
 */
void lp_constraints::negate_column(int index) {
    for (int i=0;i<nrows();++i) {
        vecteur &lh=*lhs[i]._VECTptr;
        lh[index]=-lh[index];
    }
}

/*
 * Subtract v from rhs column of constraints.
 */
void lp_constraints::subtract_from_rhs_column(const vecteur &v) {
    assert(int(v.size())==nrows());
    for (int i=0;i<nrows();++i) {
        rhs[i]-=v[i];
    }
}

/*
 * Append constraint "lh rel rh".
 */
void lp_constraints::append(const vecteur &lh,const gen &rh,int relation_type) {
    assert(nrows()==0 || int(lh.size())==ncols());
    lhs.push_back(lh);
    rhs.push_back(rh);
    rv.push_back(relation_type);
}

/*
 * Set the constraint with specified index.
 */
void lp_constraints::set(int index,const vecteur &lh,const gen &rh,int relation_type) {
    assert(index<nrows());
    lhs[index]=lh;
    rhs[index]=rh;
    rv[index]=relation_type;
}

/*
 * Get left and right side of the constraint with specified index.
 */
void lp_constraints::get_lr(int index,vecteur &lh,gen &rh) {
    assert(index<nrows());
    lh=*lhs[index]._VECTptr;
    rh=rhs[index];
}

/*
 * Get the constraint with specified index.
 */
void lp_constraints::get(int index,vecteur &lh,gen &rh,int &relation_type) {
    get_lr(index,lh,rh);
    relation_type=rv[index];
}

/*
 * Divide the constraint by g.
 */
void lp_constraints::div(int index,const gen &g,GIAC_CONTEXT) {
    assert(index<nrows() && !is_zero(g));
    lhs[index]=divvecteur(*lhs[index]._VECTptr,g);
    rhs[index]=rhs[index]/g;
    if (is_strictly_positive(-g,contextptr))
        rv[index]*=-1;
}

/*
 * Subtract vector from the constraint.
 */
void lp_constraints::subtract(int index,const vecteur &v,const gen &g) {
    assert(index<nrows());
    lhs[index]=subvecteur(*lhs[index]._VECTptr,v);
    rhs[index]-=g;
}

/*
 * Remove row with specified index.
 */
void lp_constraints::remove(int index) {
    lhs.erase(lhs.begin()+index);
    rhs.erase(rhs.begin()+index);
    rv.erase(rv.begin()+index);
}

/*
 * Delete linearly independent equality constraints, if any.
 */
int lp_constraints::remove_linearly_dependent(GIAC_CONTEXT) {
    matrice m;
    ints pos;
    std::set<int> ri,found;
    for (int i=0;i<nrows();++i) {
        if (rv[i]==_LP_EQ) {
            pos.push_back(i);
            m.push_back(mergevecteur(*lhs[i]._VECTptr,vecteur(1,rhs[i])));
        }
    }
    if (!pos.empty()) {
        matrice mf=mtran(*_rref(mtran(*_evalf(m,contextptr)._VECTptr),contextptr)._VECTptr);
        for (const_iterateur it=mf.begin();it!=mf.end();++it) {
            int i=0;
            for (;i<=ncols() && is_zero(it->_VECTptr->at(i));++i);
            if (i<=ncols() && is_one(it->_VECTptr->at(i)) && found.find(i)==found.end())
                found.insert(i);
            else ri.insert(it-mf.begin());
        }
        for (std::set<int>::const_reverse_iterator it=ri.rbegin();it!=ri.rend();++it) {
            remove(pos[*it]);
        }
    }
    return ri.size();
}

/*
 * Return the total numer of nonzeros in the constraint matrix.
 */
int lp_constraints::nonzeros() {
    int nz=0;
    for (int i=0;i<nrows();++i) {
        for (int j=0;j<ncols();++j) {
            if (!is_zero(lhs[i][j]))
                ++nz;
        }
        if (!is_zero(rhs[i]))
            ++nz;
    }
    return nz;
}

/*
 * Add identifiers from g to variable_identifiers.
 */
void lp_problem::add_identifiers_from(const gen &g) {
    vecteur vars(*_lname(g,ctx)._VECTptr);
    for (const_iterateur it=vars.begin();it!=vars.end();++it) {
        if (!contains(variable_identifiers,*it))
            variable_identifiers.push_back(*it);
    }
}

/*
 * Return variable index corresponding to the given identifier.
 */
int lp_problem::get_variable_index(const identificateur &idnt) {
    int n=variable_identifiers.size();
    for (int i=0;i<n;++i) {
        if (*variable_identifiers[i]._IDNTptr==idnt)
            return i;
    }
    return -1;
}

/*
 * Create the problem variables. Assume that they are continuous and
 * unrestricted by default.
 */
void lp_problem::create_variables(int n) {
    int m=variables.size();
    if (m>=n)
        return;
    variables.resize(n);
    for (int i=m;i<n;++i) {
        lp_variable var;
        var.set_lb(minus_inf); // default: no restrictions whatsoever
        variables[i]=var;
    }
}

/*
 * Tighten both upper and lower bound of the variable.
 */
void lp_problem::tighten_variable_bounds(int i,const gen &l,const gen &u) {
    lp_variable &var=variables[i];
    var.tighten_lbound(l,ctx);
    var.tighten_ubound(u,ctx);
}

/*
 * Output solution in form [x1=v1,x2=v2,...,xn=vn] where xk are variable
 * identifiers and vk are solution values.
 */
vecteur lp_problem::output_solution() {
    if (variable_identifiers.empty())
        return solution;
    return *_sort(_zip(makesequence(at_equal,variable_identifiers,solution),ctx),ctx)._VECTptr;
}

/*
 * Determine coeffcients of linear combination g of variables x in
 * variable_identifiers. varcoeffs C and freecoeff c are filled such that
 * g=C*x+c.
 */
bool lp_problem::lincomb_coeff(const gen &g,vecteur &varcoeffs,gen &freecoeff) {
    gen e(g),a;
    varcoeffs.clear();
    for (const_iterateur it=variable_identifiers.begin();it!=variable_identifiers.end();++it) {
        a=0;
        if (is_constant_wrt(e,*it,ctx) || (is_linear_wrt(e,*it,a,e,ctx) && is_realcons(a,ctx)))
            varcoeffs.push_back(a);
        else return false;
    }
    return is_realcons(freecoeff=e,ctx);
}

/*
 * Add slack variables to the problem if necessary (i.e. convert all
 * inequalities to equalities).
 */
void lp_problem::add_slack_variables() {
    ints posv;
    int nv0=constr.ncols();
    for (int i=0;i<nc();++i) {
        if (constr.rv[i]==_LP_EQ)
            continue;
        append_column(constr.lhs,multvecteur(-constr.rv[i],singleton(nc(),i)));
        constr.rv[i]=_LP_EQ;
        variables.push_back(lp_variable()); //add slack variable
        posv.push_back(i);
    }
    objective.first.resize(nv(),0);
    //determine types of slack variables
    vecteur lh;
    gen rh;
    for (int k=0;k<int(posv.size());++k) {
        int i=posv[k],j=nv0+k;
        lp_variable &var=variables[j];
        constr.get_lr(i,lh,rh);
        if (is_exact(lh) && is_exact(rh)) {
            gen den(_denom(rh,ctx));
            for (int l=0;l<nv0;++l) {
                if (is_zero(lh[l]))
                    continue;
                if (!variables[l].is_integral()) {
                    den=undef;
                    break;
                }
                if (!lh[l].is_integer())
                    den=_lcm(makesequence(den,_denom(lh[l],ctx)),ctx);
            }
            if (!is_undef(den)) {
                for (iterateur it=lh.begin();it!=lh.end();++it) {
                    *it=(*it)*den;
                }
                rh=den*rh;
                var.set_integral(true);
            }
        }
    }
}

/*
 * Make all decision variables bounded below by negating variables unrestricted
 * below and replacing variables unrestricted from both above and below with
 * the difference of two nonnegative variables. This process is reversed after
 * an optimal solution is found.
 */
void lp_problem::make_all_vars_bounded_below() {
    for (int i=nv();i-->0;) {
        lp_variable &var=variables[i];
        if (var.range().is_unrestricted_below()) {
            if (var.range().is_unrestricted_above()) {
                var.set_lb(0);
                lp_variable negvar(var);
                var.set_sign_type(_LP_VARSIGN_POS_PART);
                negvar.set_sign_type(_LP_VARSIGN_NEG_PART);
                variables.insert(variables.begin()+i,negvar);
                objective.first.insert(objective.first.begin()+i,-objective.first[i]);
                constr.duplicate_column(i);
            }
            else {
                var.set_lb(-var.ub());
                var.set_ub(plus_inf);
                var.set_sign_type(_LP_VARSIGN_NEG);
                objective.first[i]=-objective.first[i];
            }
            constr.negate_column(i);
        }
    }
}

/*
 * Force all problem parameters to be exact.
 */
void lp_problem::make_problem_exact() {
    objective.first=*exact(objective.first,ctx)._VECTptr;
    objective.second=exact(objective.second,ctx);
    constr.lhs=*exact(constr.lhs,ctx)._VECTptr;
    constr.rhs=*exact(constr.rhs,ctx)._VECTptr;
    for (vector<lp_variable>::iterator it=variables.begin();it!=variables.end();++it) {
        it->set_lb(exact(it->lb(),ctx));
        it->set_ub(exact(it->ub(),ctx));
    }
}

/*
 * Report status.
 */
void lp_problem::report_status(const char *msg,int count) {
    char buf[16];
    sprintf(buf,"%d: ",count);
    int nd=numdigits((unsigned)count);
    string str(msg);
    str.insert(0,buf);
    while (nd<8) {
        str.insert(str.begin(),(char)32);
        ++nd;
    }
    message(str.c_str());
}

/*
 * Return true iff there is a variable with lb>ub.
 */
bool lp_problem::has_infeasible_var() const {
    for (vector<lp_variable>::const_iterator it=variables.begin();it!=variables.end();++it) {
        if (is_strictly_greater(it->lb(),it->ub(),ctx))
            return true;
    }
    return false;
}

/*
 * Remove the j-th variable from the problem.
 * Use subs_coef for substitution.
 */
void lp_problem::remove_variable(int j) {
    const lp_variable &var=variables[j];
    vecteur &obj=objective.first;
    if (var.has_subs_coef(-1))
        objective.second+=obj[j]*var.get_subs_coef(-1);
    for (int k=0;k<constr.ncols();++k) {
        if (k!=j && var.has_subs_coef(k))
            obj[k]+=obj[j]*var.get_subs_coef(k);
    }
    removed_vars.push(var);
    variables.erase(variables.begin()+j);
    remove_column(constr.lhs,j);
    obj.erase(obj.begin()+j);
    removed_cols.push(j);
}

/*
 * Deduce the optimal value val of a (implied) free variable.
 * c is the respective coefficient in the objective.
 */
int lp_variable::find_opt_free(const gen &c,gen &val,GIAC_CONTEXT) const {
    if (is_zero(c)) {
        if (!is_inf(lb())) {
            val=is_integral()?_ceil(lb(),contextptr):lb();
        } else if (!is_inf(ub())) {
            val=is_integral()?_floor(ub(),contextptr):ub();
        } else val=0;
    } else if (is_positive(c,contextptr)) {
        if (!is_inf(lb())) {
            val=is_integral()?_ceil(lb(),contextptr):lb();
        } else return _LP_UNBOUNDED;
    } else {
        if (!is_inf(ub())) {
            val=is_integral()?_floor(ub(),contextptr):ub();
        } else return _LP_UNBOUNDED;
    }
    if (!is_greater(val,lb(),contextptr) || !is_greater(ub(),val,contextptr))
        return _LP_INFEASIBLE;
    return 0;
}

/*
 * Preprocess the problem.
 * Handles empty, singleton, forcing rows, and singleton columns.
 */
int lp_problem::preprocess() {
    bool changed;
    do {
        changed=false;
        for (int i=constr.nrows();i-->0;) { // detect empty and singleton rows
            const vecteur &a=*constr.lhs[i]._VECTptr;
            const gen &b=constr.rhs[i];
            int nz=0,j0=-1,r=constr.rv[i];
            for (int j=0;j<constr.ncols();++j) {
                if (!is_zero(a[j])) {
                    ++nz;
                    j0=j;
                }
            }
            if (nz==0) { // empty row detected
                if ((r==_LP_EQ && !is_zero(b)) ||
                        (r==_LP_LEQ && is_strictly_positive(-b,ctx)) ||
                        (r==_LP_GEQ && is_strictly_positive(b,ctx)))
                    return _LP_INFEASIBLE;
            } else if (nz==1) { // singleton row detected
                assert(j0>=0);
                lp_variable &var=variables[j0];
                gen val=b/a[j0];
                if (is_strictly_positive(-a[j0],ctx))
                    r*=-1;
                if (r<=0) var.tighten_ubound(val,ctx);
                if (r>=0) var.tighten_lbound(val,ctx);
            } else continue;
            constr.remove(i);
            changed=true;
        }
        if (has_infeasible_var())
            return _LP_INFEASIBLE;
        for (int i=constr.nrows();i-->0;) { // detect forcing constraints
            const vecteur &a=*constr.lhs[i]._VECTptr;
            const gen &b=constr.rhs[i];
            int r=constr.rv[i];
            gen g(0),h(0);
            for (int j=0;j<constr.ncols();++j) {
                if (is_zero(a[j]))
                    continue;
                const lp_variable &var=variables[j];
                g+=a[j]*(is_positive(a[j],ctx)?var.lb():var.ub());
                h+=a[j]*(is_positive(a[j],ctx)?var.ub():var.lb());
            }
            if (is_undef(g) || is_undef(h))
                continue;
            if ((r>=0 && is_strictly_greater(b,h,ctx)) || (r<=0 && is_strictly_greater(g,b,ctx)))
                return _LP_INFEASIBLE;
            if (r!=_LP_EQ || is_inf(g) || is_inf(h))
                continue;
            if (is_zero(g-b) || is_zero(h-b)) { // forcing constraint detected
                for (int j=0;j<constr.ncols();++j) {
                    if (is_zero(a[j]))
                        continue;
                    lp_variable &var=variables[j];
                    if (is_positive(a[j]*gen(is_zero(g-b)?1:-1),ctx))
                        var.tighten_ubound(var.lb(),ctx);
                    else var.tighten_lbound(var.ub(),ctx);
                }
                constr.remove(i);
                changed=true;
                continue;
            }
            // detect free singleton columns, tighten variable bounds
            for (int j=constr.ncols();j-->0;) {
                if (is_zero(a[j]))
                    continue;
                lp_variable &var=variables[j];
                gen l=(is_positive(a[j],ctx)?b-h:b-g)/a[j]+var.ub();
                gen u=(is_positive(a[j],ctx)?b-g:b-h)/a[j]+var.lb();
                int k=0;
                for (;k<constr.nrows() && (k==i || is_zero(constr.lhs[k][j]));++k);
                if (k==constr.nrows() && !var.is_integral() && is_greater(l,var.lb(),ctx) && is_greater(var.ub(),u,ctx)) {
                    // free column singleton detected
                    var.set_subs_coef(-1,b/a[j]);
                    for (k=0;k<constr.ncols();++k) {
                        if (k!=j && !is_zero(a[k]))
                            var.set_subs_coef(k,-a[k]/a[j]);
                    }
                    remove_variable(j);
                    constr.remove(i);
                    changed=true;
                    break;
                } else { // tighten bounds on var
                    if (var.tighten_lbound(l,ctx) || var.tighten_ubound(u,ctx))
                        changed=true;
                }
            }
        }
        if (has_infeasible_var())
            return _LP_INFEASIBLE;
        // remove fixed variables and empty columns
        for (int j=constr.ncols();j-->0;) {
            lp_variable &var=variables[j];
            gen val;
            if (is_zero__VECT(jth_column(constr.lhs,j),ctx)) { // empty column
                int res=var.find_opt_free(objective.first[j],val,ctx);
                if (res!=0)
                    return res;
                var.set_subs_coef(-1,val);
                remove_variable(j);
                changed=true;
            } else if (var.is_fixed()) { // fixed variable
                val=var.lb();
                if (var.is_integral() && !val.is_integer())
                    return _LP_INFEASIBLE;
                for (int i=0;i<constr.nrows();++i) {
                    constr.rhs[i]-=constr.lhs[i][j]*val;
                }
                var.set_subs_coef(-1,val);
                remove_variable(j);
                changed=true;
            }
        }
    } while (changed);
    return 0;
}

/*
 * Postprocess, affecting the solution.
 */
void lp_problem::postprocess() {
    int vs=variables.size();
    while (!removed_cols.empty()) {
        int j=removed_cols.top();
        vs++;
        lp_variable &var=removed_vars.top();
        solution.insert(solution.begin()+j,var.has_subs_coef(-1)?var.get_subs_coef(-1):0);
        for (int k=vs;k-->0;) {
            if (k!=j && var.has_subs_coef(k)) {
                solution[j]+=solution[k]*var.get_subs_coef(k);
            }
        }
        variables.insert(variables.begin()+j,var);
        removed_cols.pop();
        removed_vars.pop();
    }
}

/*
 * Solve the problem using the specified settings.
 */
int lp_problem::solve() {
    stats=lp_stats();
    char buffer[256];
    int rr=constr.remove_linearly_dependent(ctx);
    if (rr>0) {
        std::sprintf(buffer,"Removed %d linearly dependent equality constraints",rr);
        message(buffer);
    }
    std::sprintf(buffer,"Constraint matrix has %d rows, %d columns, and %d nonzeros",constr.nrows(),constr.ncols(),constr.nonzeros());
    message(buffer);
    make_problem_exact();
    if (settings.maximize) { // convert to minimization problem
        objective.first=multvecteur(-1,objective.first);
        objective.second=-objective.second;
    }
    if (settings.presolve) {
    message("Preprocessing...");
        int res=preprocess();
        if (res!=0)
            return res; // the problem is either infeasible or unbounded
        std::sprintf(buffer,"Constraint matrix has %d rows, %d columns, and %d nonzeros",constr.nrows(),constr.ncols(),constr.nonzeros());
        message(buffer);
    }
    int nvars=variables.size();
    iteration_count=0;
    if (nvars>0) {
        if (constr.nrows()==0) { // the constraint matrix is empty
            solution.resize(nvars);
            gen val;
            for (int i=0;i<nvars;++i) {
                int res=variables[i].find_opt_free(objective.first[i],val,ctx);
                if (res!=0)
                    return res;
                solution[i]=val;
                objective.second+=val*objective.first[i];
            }
            optimum=objective.second;
        } else { // use simplex method
            add_slack_variables();
            make_all_vars_bounded_below();
            objective_norm=_l2norm(objective.first,ctx).to_double(ctx);
            int result;
            optimum=undef;
            double opt_approx;
            lp_node root(this);
            root.resize_ranges(nv());
            root.set_depth(0);
            message("Optimizing...");
            if ((result=root.solve_relaxation())!=_LP_SOLVED)
                return result;
            if (root.is_integer_feasible()) {
                solution=root.get_solution();
                optimum=root.get_optimum();
            } else {
                message("Applying branch & bound method...");
                double root_optimum=root.get_optimum().to_double(ctx);
                double root_infeas=root.get_infeas().to_double(ctx);
                root.resize_ranges(nv());
                vector<lp_node> active_nodes(1,root);
                clock_t t=clock(),t0=t,now;
                int n,j,k,depth;
                double opt_lbound,fr,max_score;
                bool depth_exceeded=false,incumbent_updated,is_use_pseudocost=false;
                map<double,int> candidates;
                if (settings.nodeselect<0)
                    settings.nodeselect=_LP_BEST_LOCAL_BOUND;
                int nsel=settings.nodeselect,vsel=settings.varselect;
                while (!active_nodes.empty()) {
                    if (settings.node_limit>0 && stats.subproblems_examined>=settings.node_limit) {
                        message("Warning: node limit exceeded",true);
                        break;
                    }
                    if (settings.iteration_limit>0 && iteration_count>settings.iteration_limit) {
                        message("Warning: simplex iteration limit exceeded",true);
                        break;
                    }
                    n=active_nodes.size();
                    if (n>stats.max_active_nodes)
                        stats.max_active_nodes=n;
                    opt_lbound=DBL_MAX;
                    k=nsel==_LP_BREADTHFIRST?0:(nsel==_LP_DEPTHFIRST?int(active_nodes.size())-1:-1);
                    depth=-1;
                    for (int i=0;i<n;++i) {
                        lp_node &node=active_nodes[i];
                        if (opt_lbound>node.get_opt_approx()) {
                            opt_lbound=node.get_opt_approx();
                            if (nsel==_LP_BEST_LOCAL_BOUND || nsel==_LP_HYBRID)
                                k=i;
                        }
                    }
                    if (nsel==_LP_HYBRID && is_undef(optimum))
                        k=int(active_nodes.size())-1;
                    if (nsel==_LP_BEST_PROJECTION) {
                        double bestproj=DBL_MAX,proj,iopt=is_undef(optimum)?0:optimum.to_double(ctx);
                        for (int i=0;i<n;++i) {
                            lp_node &node=active_nodes[i];
                            proj=node.get_optimum().to_double(ctx)+
                                    (iopt-root_optimum)*node.get_infeas().to_double(ctx)/root_infeas;
                            if (proj<bestproj) {
                                bestproj=proj;
                                k=i;
                            }
                        }
                    }
                    if (k<0) {
                        message("Error: node selection strategy failed",true);
                        break;
                    }
                    j=-1;
                    if (vsel==_LP_PSEUDOCOST || vsel<0) {
                        max_score=0;
                        for (int i=0;i<nv();++i) {
                            if (!active_nodes[k].is_var_fractional(i))
                                continue;
                            lp_variable &var=variables[i];
                            double score=var.score(active_nodes[k].get_fractional_var(i));
                            if (score==0) {
                                j=-1;
                                break;
                            }
                            if (score>max_score) {
                                j=i;
                                max_score=score;
                            }
                        }
                        if (j>=0 && !is_use_pseudocost) {
                            report_status("Switched to pseudocost based branching",stats.subproblems_examined);
                            is_use_pseudocost=true;
                        }
                    }
                    if (j<0) {
                        switch (vsel) {
                        case -1:
                        case _LP_MOSTFRACTIONAL:
                        case _LP_PSEUDOCOST:
                            j=active_nodes[k].get_most_fractional();
                            break;
                        case _LP_FIRSTFRACTIONAL:
                            j=active_nodes[k].get_first_fractional_var();
                            break;
                        case _LP_LASTFRACTIONAL:
                            j=active_nodes[k].get_last_fractional_var();
                            break;
                        }
                    }
                    if (j<0) {
                        message("Error: branching variable selection strategy failed",true);
                        break;
                    }
                    if (!is_undef(optimum)) {
                        stats.mip_gap=is_zero(optimum)?-opt_lbound:(opt_approx-opt_lbound)/std::abs(opt_approx);
                        if (stats.mip_gap<=settings.relative_gap_tolerance) {
                            if (settings.relative_gap_tolerance>0)
                                message("Warning: integrality gap threshold reached",true);
                            break;
                        }
                    }
                    incumbent_updated=false;
                    for (int i=0;i<2;++i) {
                        lp_node child_node=active_nodes[k].create_child();
                        if (settings.depth_limit>0 && child_node.get_depth()>settings.depth_limit) {
                            if (!depth_exceeded) {
                                message ("Warning: depth limit exceeded",true);
                                depth_exceeded=true;
                            }
                            break;
                        }
                        lp_range &range=child_node.get_range(j);
                        if (i==(nsel==_LP_DEPTHFIRST || (nsel==_LP_HYBRID && is_undef(optimum))?1:0)) {
                            range.tighten_lbound(_ceil(active_nodes[k].get_solution()[j],ctx),ctx);
                            if (is_strictly_positive(child_node.get_range(j).lb(),ctx)) {
                                switch (variables[j].sign_type()) {
                                case _LP_VARSIGN_POS_PART:
                                    child_node.get_range(j-1).set_ub(0);
                                    break;
                                case _LP_VARSIGN_NEG_PART:
                                    child_node.get_range(j+1).set_ub(0);
                                }
                            }
                        } else range.tighten_ubound(_floor(active_nodes[k].get_solution()[j],ctx),ctx);
                        ++stats.subproblems_examined;
                        if (child_node.solve_relaxation()==_LP_SOLVED) {
                            double p=child_node.get_fractional_var(j);
                            int dir=nsel==_LP_DEPTHFIRST || (nsel==_LP_HYBRID && is_undef(optimum))?i:1-i;
                            variables[j].update_pseudocost(std::abs(child_node.get_opt_approx()-active_nodes[k].get_opt_approx()),
                                dir==0?p:1-p,dir);
                            if (!is_undef(optimum) && is_greater(child_node.get_optimum(),optimum,ctx))
                                continue;
                            if (child_node.is_integer_feasible()) { // new potential incumbent found
                                if (is_undef(optimum) || is_strictly_greater(optimum,child_node.get_optimum(),ctx)) {
                                    if (is_undef(optimum))
                                        report_status("Incumbent solution found",stats.subproblems_examined);
                                    else {
                                        std::sprintf(buffer,"Incumbent solution updated, objective value improvement: %g%%",
                                                (opt_approx-child_node.get_opt_approx())/std::abs(opt_approx)*100.0);
                                        report_status(buffer,stats.subproblems_examined);
                                    }
                                    solution=child_node.get_solution();
                                    optimum=child_node.get_optimum();
                                    opt_approx=child_node.get_opt_approx();
                                    incumbent_updated=true;
                                }
                            } else active_nodes.push_back(child_node);
                        }
                    }
                    //fathom
                    active_nodes.erase(active_nodes.begin()+k);
                    if (incumbent_updated) {
                        for (int i=active_nodes.size();i-->0;) {
                            if (is_greater(active_nodes[i].get_optimum(),optimum,ctx))
                                active_nodes.erase(active_nodes.begin()+i);
                        }
                    }
                    now=clock();
                    if (settings.time_limit>0 && 1e3*double(now-t0)/CLOCKS_PER_SEC>settings.time_limit) {
                        message("Warning: time limit exceeded",true);
                        break;
                    }
                    if (CLOCKS_PER_SEC/double(now-t)<=settings.status_report_freq) { //report status
                        std::sprintf(buffer,"%d nodes active, bound: %g",
                                     (int)active_nodes.size(),opt_lbound*(settings.maximize?-1:1));
                        string str(buffer);
                        if (stats.mip_gap>=0) {
                            std::sprintf(buffer,", gap: %g%%",stats.mip_gap*100);
                            str+=string(buffer);
                        }
                        report_status(str.c_str(),stats.subproblems_examined);
                        t=clock();
                    }
                }
                if (active_nodes.empty())
                    report_status("Tree is empty",stats.subproblems_examined);
                //show branch&bound summary
                std::sprintf(buffer,"Summary:\n * %d subproblem(s) examined\n * max. tree size: %d nodes\n * %d Gomory cut(s) applied",
                        stats.subproblems_examined,stats.max_active_nodes,stats.cuts_applied);
                message(buffer);
            }
            if (is_undef(optimum))
                return _LP_INFEASIBLE;
            for (int i=nv();i-->0;) {
                lp_variable &var=variables[i];
                switch (var.sign_type()) {
                case _LP_VARSIGN_NEG:
                    solution[i]=-solution[i];
                    break;
                case _LP_VARSIGN_NEG_PART:
                    solution[i+1]-=solution[i];
                    solution.erase(solution.begin()+i);
                    break;
                }
            }
            optimum+=objective.second;
            solution.resize(nvars);
        }
    } else {
        if (settings.presolve) {
            message("Solution found by preprocessor");
            optimum=objective.second;
        } else optimum=undef;
    }
    if (settings.presolve)
        postprocess();
    if (settings.maximize)
        optimum=-optimum;
    return _LP_SOLVED;
}

#ifdef HAVE_LIBGLPK

/*
 * Create GLPK problem from constr.
 */
glp_prob *lp_problem::glpk_initialize() {
    glp_prob *glp=glp_create_prob();
    glp_add_rows(glp,nc());
    glp_add_cols(glp,nv());
    glp_set_obj_dir(glp,settings.maximize?GLP_MAX:GLP_MIN);
    const vecteur &obj=objective.first;
    int obj_sz=obj.size();
    for (int i=0;i<=nv();++i) {
        glp_set_obj_coef(glp,i,(i==0?objective.second:(i<=obj_sz?obj[i-1]:gen(0))).to_double(ctx));
        if (i>0) {
            lp_variable &var=variables[i-1];
            glp_set_col_kind(glp,i,var.is_integral()?GLP_IV:GLP_CV);
            int bound_type=GLP_FR;
            if (!var.range().is_unrestricted_below() && var.range().is_unrestricted_above())
                bound_type=GLP_LO;
            else if (var.range().is_unrestricted_below() && !var.range().is_unrestricted_above())
                bound_type=GLP_UP;
            else if (!var.range().is_unrestricted_below() && !var.range().is_unrestricted_above())
                bound_type=GLP_DB;
            else if (is_zero(var.ub()-var.lb()))
                bound_type=GLP_FX;
            double lo=var.range().is_unrestricted_below()?0:var.lb().to_double(ctx);
            double hi=var.range().is_unrestricted_above()?0:var.ub().to_double(ctx);
            if (var.is_integral()) {
                lo=ceil(lo);
                hi=floor(hi);
                if (bound_type==GLP_DB && lo==hi)
                    bound_type=GLP_FX;
            }
            if (lo==0 && hi==1 && var.is_integral())
                glp_set_col_kind(glp,i,GLP_BV);
            else glp_set_col_bnds(glp,i,bound_type,lo,hi);
        }
    }
    int n=constr.nrows()*constr.ncols();
    int *ia=new int[n+1],*ja=new int[n+1]; int k=0;
    double *ar=new double[n+1];
    gen a;
    for (int i=0;i<constr.nrows();++i) {
        for (int j=0;j<constr.ncols();++j) {
            if (is_zero(a=constr.lhs[i][j]))
                continue;
            ++k;
            ia[k]=i+1;
            ja[k]=j+1;
            ar[k]=a.to_double(ctx);
        }
        double rh=constr.rhs[i].to_double(ctx);
        switch (constr.rv[i]) {
        case _LP_EQ:
            glp_set_row_bnds(glp,i+1,GLP_FX,rh,0.0);
            break;
        case _LP_LEQ:
            glp_set_row_bnds(glp,i+1,GLP_UP,0.0,rh);
            break;
        case _LP_GEQ:
            glp_set_row_bnds(glp,i+1,GLP_LO,rh,0.0);
            break;
        }
    }
    glp_load_matrix(glp,k,ia,ja,ar);
    delete [] ia;
    delete [] ja;
    delete [] ar;
    return glp;
}

/*
 * Solve LP problem using GLPK implementation of simplex method.
 */
int lp_problem::glpk_simplex(glp_prob *prob) {
    glp_smcp parm;
    glp_init_smcp(&parm);
    parm.msg_lev=settings.verbose?GLP_MSG_ON:GLP_MSG_ERR;
    if (settings.iteration_limit>0)
        parm.it_lim=settings.iteration_limit;
    if (settings.time_limit>0)
        parm.tm_lim=settings.time_limit;
    parm.presolve=settings.presolve?GLP_ON:GLP_OFF;
    return glp_simplex(prob,&parm);
}

/*
 * Solve LP problem using GLPK implementation of interior point method.
 */
int lp_problem::glpk_interior_point(glp_prob *prob) {
    glp_iptcp parm;
    glp_init_iptcp(&parm);
    parm.msg_lev=settings.verbose?GLP_MSG_ON:GLP_MSG_ERR;
    return glp_interior(prob,&parm);
}

void lp_problem::glpk_callback(glp_tree *tree,void *info) {
    if (interrupted || ctrl_c) {
        interrupted=ctrl_c=false;
        glp_ios_terminate(tree);
    }
    lp_problem *prob=static_cast<lp_problem*>(info);
    switch (glp_ios_reason(tree)) {
    case GLP_ISELECT:
        if (prob->use_blb)
            glp_ios_select_node(tree,glp_ios_best_node(tree));
        break;
    case GLP_IBINGO:
        if (prob->settings.nodeselect==_LP_HYBRID && !prob->use_blb) {
            prob->message("Switching to best-local-bound node selection");
            prob->use_blb=true;
        }
        break;
    default:
        break;
    }
}

/*
 * Solve MIP problem using GLPK implementation of branch and cut method.
 */
int lp_problem::glpk_branchcut(glp_prob *prob) {
    glp_iocp parm;
    glp_init_iocp(&parm);
    parm.msg_lev=settings.verbose?GLP_MSG_ON:GLP_MSG_ERR;
    if (settings.time_limit>0)
        parm.tm_lim=settings.time_limit;
    parm.out_frq=(int)(1000.0/settings.status_report_freq);
    parm.mip_gap=settings.relative_gap_tolerance;
    parm.gmi_cuts=settings.max_cuts>0?GLP_ON:GLP_OFF;
    parm.mir_cuts=settings.max_cuts>0?GLP_ON:GLP_OFF;
    parm.clq_cuts=settings.has_binary_vars?GLP_ON:GLP_OFF;
    parm.cov_cuts=settings.has_binary_vars?GLP_ON:GLP_OFF;
    parm.presolve=settings.presolve?GLP_ON:GLP_OFF;
    parm.cb_func=&glpk_callback;
    parm.cb_info=static_cast<void*>(this);
    switch (settings.varselect) {
    case _LP_FIRSTFRACTIONAL:
        parm.br_tech=GLP_BR_FFV;
        break;
    case _LP_LASTFRACTIONAL:
        parm.br_tech=GLP_BR_LFV;
        break;
    case _LP_MOSTFRACTIONAL:
        parm.br_tech=GLP_BR_MFV;
        break;
    case _LP_PSEUDOCOST:
        parm.br_tech=GLP_BR_PCH;
        break;
    default:
        parm.br_tech=GLP_BR_DTH;
    }
    switch (settings.nodeselect) {
    case _LP_HYBRID:
    case _LP_DEPTHFIRST:
        parm.bt_tech=GLP_BT_DFS;
        break;
    case _LP_BREADTHFIRST:
        parm.bt_tech=GLP_BT_BFS;
        break;
    case _LP_BEST_PROJECTION:
        parm.bt_tech=GLP_BT_BPH;
        break;
    case _LP_BEST_LOCAL_BOUND:
    default:
        parm.bt_tech=GLP_BT_BLB;
    }
    use_blb=false;
    return glp_intopt(prob,&parm);
}

int lp_problem::term_hook(void *info,const char *s) {
    *static_cast<std::ostream*>(info) << s;
    return true;
}

#endif

/*
 * Solve the problem using the GLPK library.
 */
int lp_problem::glpk_solve() {
#ifndef HAVE_LIBGLPK
    message("Warning: solving in floating-point arithmetic requires GLPK library",true);
    return solve();
#else
    /* redirect GLPK output to logptr */
    int term_old;
    if (settings.verbose)
        glp_term_hook(term_hook,static_cast<void*>(logptr(ctx)));
    else term_old=glp_term_out(GLP_OFF);
    glp_prob *prob=glpk_initialize();
    int result=0,solution_status;
    bool is_mip=has_integral_variables();
    switch (settings.solver) {
    case _LP_SIMPLEX:
        if (is_mip) {
            result=glpk_branchcut(prob);
            if (result==GLP_EMIPGAP || result==GLP_ETMLIM || result==GLP_ESTOP)
                result=0;
            if (result==0)
                solution_status=glp_mip_status(prob);
        } else {
            result=glpk_simplex(prob);
            if (result==GLP_ETMLIM || result==GLP_EITLIM)
                result=0;
            if (result==0)
                solution_status=glp_get_status(prob);
        }
        break;
    case _LP_INTERIOR_POINT:
        if ((result=glpk_interior_point(prob))==0)
            solution_status=glp_ipt_status(prob);
        break;
    }
    if (result==0) { //solving process finished successfully
        switch (solution_status) {
        case GLP_OPT:
            solution_status=_LP_SOLVED;
            break;
        case GLP_FEAS:
            message("Warning: the solution is not necessarily optimal",true);
            solution_status=_LP_SOLVED;
            break;
        case GLP_INFEAS:
            message("Warning: the solution is infeasible",true);
            solution_status=_LP_SOLVED;
            break;
        case GLP_NOFEAS:
            solution_status=_LP_INFEASIBLE;
            break;
        case GLP_UNBND:
            solution_status=_LP_UNBOUNDED;
            break;
        case GLP_UNDEF:
            solution_status=_LP_ERROR;
            break;
        }
        if (solution_status==_LP_SOLVED) { //get solution and optimum
            solution.resize(nv());
            switch (settings.solver) {
            case _LP_SIMPLEX:
                if (is_mip) {
                    optimum=glp_mip_obj_val(prob);
                    for (int i=0;i<nv();++i) {
                        solution[i]=glp_mip_col_val(prob,i+1);
                        if (variables[i].is_integral())
                            solution[i]=_round(solution[i],ctx);
                    }
                }
                else {
                    optimum=glp_get_obj_val(prob);
                    for (int i=0;i<nv();++i) {
                        solution[i]=glp_get_col_prim(prob,i+1);
                    }
                }
                break;
            case _LP_INTERIOR_POINT:
                optimum=glp_ipt_obj_val(prob);
                for (int i=0;i<nv();++i) {
                    solution[i]=glp_ipt_col_prim(prob,i+1);
                }
                break;
            }
        }
    } else switch (result) { // GLPK returned an error
    case GLP_EBADB:   message("Error (GLPK): invalid basis",true); break;
    case GLP_ESING:   message("Error (GLPK): singular matrix",true); break;
    case GLP_ECOND:   message("Error (GLPK): ill-conditioned matrix",true); break;
    case GLP_EBOUND:  message("Error (GLPK): invalid bounds",true); break;
    case GLP_EFAIL:   message("Error (GLPK): solver failed",true); break;
    case GLP_EOBJLL:  message("Error (GLPK): objective lower limit reached",true); break;
    case GLP_EOBJUL:  message("Error (GLPK): objective upper limit reached",true); break;
    case GLP_ENOPFS:  message("Error (GLPK): no primal feasible solution",true); break;
    case GLP_ENODFS:  message("Error (GLPK): no dual feasible solution",true); break;
    case GLP_EROOT:   message("Error (GLPK): root LP optimum not provided",true); break;
    case GLP_ENOFEAS: message("Error (GLPK): no primal/dual feasible solution",true); break;
    case GLP_ENOCVG:  message("Error (GLPK): no convergence",true); break;
    case GLP_EINSTAB: message("Error (GLPK): numerical instability",true); break;
    case GLP_EDATA:   message("Error (GLPK): invalid data",true); break;
    case GLP_ERANGE:  message("Error (GLPK): result out of range",true); break;
    default:          message("Error (GLPK): unknown error",true); break;
    }
    glp_delete_prob(prob);
    if (settings.verbose)
        glp_term_hook(NULL,NULL);
    else glp_term_out(term_old);
    if (result==0)
        return solution_status;
    return _LP_ERROR;
#endif
}

/*
 * Load problem from file in LP or (possibly gzipped) MPS format. File types
 * are distinguished by examining the filename extension. If it is .gz or .mps,
 * it is assumed that the file is in (gzipped) MPS format. If it is .lp, it is
 * assumed that file is in LP format.
 */
bool lp_problem::glpk_load_from_file(const char *fname) {
#ifndef HAVE_LIBGLPK
    message("Error: loading CPLEX and MPS files requires GLPK library",true);
    return false;
#endif
#ifdef HAVE_LIBGLPK
    glp_prob *prob=glp_create_prob();
    bool print_messages=true;
    int result,term_old;
    string str(fname);
    string ext(str.substr(str.find_last_of(".") + 1));
    if (print_messages)
        glp_term_hook(term_hook,static_cast<void*>(logptr(ctx)));
    else term_old=glp_term_out(GLP_OFF);
    if (ext=="mps" || ext=="gz") // MPS format (modern)
        result=glp_read_mps(prob,GLP_MPS_FILE,NULL,fname);
    else if (ext=="lp") // CPLEX LP format
        result=glp_read_lp(prob,NULL,fname);
    else {
        message("Error: file format not supported",true);
        result=1;
    }
    if (result==0) { // successfully loaded
        char buffer[256];
        int obj_dir=glp_get_obj_dir(prob),len,t,k;
        int nr=glp_get_num_rows(prob),n=glp_get_num_cols(prob);
        if (nr*n>LP_CONSTR_MAXSIZE) {
            message("Error: constraint matrix too large",true);
            return false;
        }
        int *ind=new int[1+n];
        double *val=new double[1+n];
        create_variables(n);
        constr.lhs=*_matrix(makesequence(nr,nv()),ctx)._VECTptr;
        constr.rhs=vecteur(nr);
        constr.rv=ints(nr);
        variable_identifiers=vecteur(nv());
        settings.maximize=(obj_dir==GLP_MAX);
        objective.second=glp_get_obj_coef(prob,0);
        objective.first=vecteur(nv());
        for (int j=0;j<nv();++j) {
            objective.first[j]=glp_get_obj_coef(prob,j+1);
            lp_variable &var=variables[j];
            t=glp_get_col_type(prob,j+1);
            k=glp_get_col_kind(prob,j+1);
            var.set_name(string(glp_get_col_name(prob,j+1)));
            variable_identifiers[j]=identificateur(var.name());
            if (t!=GLP_UP && t!=GLP_FR)
                var.set_lb(glp_get_col_lb(prob,j+1));
            if (t!=GLP_LO && t!=GLP_FR)
                var.set_ub(glp_get_col_ub(prob,j+1));
            if (k!=GLP_CV)
                var.set_integral(true);
            if (k==GLP_BV) {
                var.tighten_lbound(0,ctx);
                var.tighten_ubound(1,ctx);
            }
        }
        for (int i=nr;i>0;--i) {
            if ((len=glp_get_mat_row(prob,i,ind,val))>0) {
                vecteur &row=*constr.lhs[i-1]._VECTptr;
                for (int j=1;j<=len;++j) {
                    row[ind[j]-1]=val[j];
                }
            }
            t=glp_get_row_type(prob,i);
            if (t==GLP_FR) {
                sprintf(buffer,"Warning: row bounds not set, discarding constraint %d",i);
                message(buffer,true);
                constr.remove(i-1);
                continue;
            }
            if (t==GLP_FX) {
                constr.rhs[i-1]=glp_get_row_lb(prob,i);
                constr.rv[i-1]=_LP_EQ;
            }
            else {
                if (t==GLP_LO || t==GLP_DB) {
                    constr.rhs[i-1]=glp_get_row_lb(prob,i);
                    constr.rv[i-1]=_LP_GEQ;
                }
                if (t==GLP_DB)
                    constr.append(*constr.lhs[i-1]._VECTptr,glp_get_row_ub(prob,i),_LP_LEQ);
                if (t==GLP_UP || t==GLP_DB) {
                    constr.rhs[i-1]=glp_get_row_ub(prob,i);
                    constr.rv[i-1]=_LP_LEQ;
                }
            }
        }
        delete [] ind;
        delete [] val;
    }
    glp_delete_prob(prob);
    if (print_messages)
        glp_term_hook(NULL,NULL);
    else glp_term_out(term_old);
    return result==0;
#endif
}

bool lp_problem::assign_variable_types(const gen &g,int t) {
    pair<gen,gen> range;
    int i,i0=array_start(ctx);
    if (g.type==_VECT) {
        for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
            if (!assign_variable_types(*it,t))
                return false;
        }
    } else if ((g.type==_IDNT && (i=get_variable_index(*g._IDNTptr))>=0) ||
             (g.is_integer() && (i=g.to_int()-i0)>=0))
        variables[i].set_type(t,ctx);
    else if (interval2pair(g,range,ctx) &&
             range.first.is_integer() && range.second.is_integer()) {
        for (i=range.first.to_int();i<=range.second.to_int();++i) {
            variables[i-i0].set_type(t,ctx);
        }
    } else return false;
    return true;
}

bool parse_limit(const gen &g,int &lim,GIAC_CONTEXT) {
    if (is_positive(g,contextptr)) {
        if (g.is_integer())
            lim=g.to_int();
        else if (is_inf(g))
            lim=0;
        else return false;
    } else return false;
    return true;
}

bool parse_options_and_bounds(const_iterateur &it,const_iterateur &itend,lp_problem &prob) {
    for (;it!=itend;++it) {
        if (*it==at_maximize)
            prob.settings.maximize=true;
        else if (*it==at_minimize)
            prob.settings.maximize=false;
        else if (it->is_integer() && it->subtype==_INT_MAPLECONVERSION) {
            switch(it->val) {
            case _LP_MAXIMIZE:
                prob.settings.maximize=true;
                break;
            case _LP_VERBOSE:
                prob.settings.verbose=true;
                break;
            case _LP_PRESOLVE:
                prob.settings.presolve=true;
                break;
            }
        } else if (it->is_symb_of_sommet(at_equal)) {
            //parse the argument in form "option=value"
            pair<gen,gen> bounds;
            gen a,lh(_lhs(*it,prob.ctx)),rh(_rhs(*it,prob.ctx));
            if (lh.type==_IDNT && interval2pair(rh,bounds,prob.ctx)) {
                int vi=prob.get_variable_index(*lh._IDNTptr);
                if (vi>=0)
                    prob.tighten_variable_bounds(vi,bounds.first,bounds.second);
            } else if (lh==at_maximize && rh.is_integer() && rh.subtype==_INT_BOOLEAN)
                prob.settings.maximize=(bool)rh.to_int();
            else if (lh==at_assume && rh.is_integer())
                prob.settings.assumption=rh.to_int();
            else if (lh.is_integer() && lh.subtype==_INT_MAPLECONVERSION) {
                switch(lh.to_int()) {
                case _LP_INTEGERVARIABLES:
                case _LP_BINARYVARIABLES:
                    if (!prob.assign_variable_types(rh,lh.to_int()))
                        return false;
                    break;
                case _LP_DEPTHLIMIT:
                    if (!parse_limit(rh,prob.settings.depth_limit,prob.ctx))
                        return false;
                    break;
                case _LP_NODE_LIMIT:
                    if (!parse_limit(rh,prob.settings.node_limit,prob.ctx))
                        return false;
                    break;
                case _LP_ITERATION_LIMIT:
                    if (!parse_limit(rh,prob.settings.iteration_limit,prob.ctx))
                        return false;
                    break;
                case _LP_MAX_CUTS:
                    if (!parse_limit(rh,prob.settings.max_cuts,prob.ctx))
                        return false;
                    break;
                case _LP_TIME_LIMIT:
                    if (!parse_limit(rh,prob.settings.time_limit,prob.ctx))
                        return false;
                    break;
                case _LP_NODESELECT:
                    if (rh.is_integer() && rh.subtype==_INT_MAPLECONVERSION)
                        prob.settings.nodeselect=rh.to_int();
                    else return false;
                    if (prob.settings.nodeselect<_LP_DEPTHFIRST || prob.settings.nodeselect>_LP_HYBRID)
                        return false;
                    break;
                case _LP_VARSELECT:
                    if (rh.is_integer() && rh.subtype==_INT_MAPLECONVERSION)
                        prob.settings.varselect=rh.to_int();
                    else return false;
                    if (prob.settings.varselect<_LP_FIRSTFRACTIONAL || prob.settings.varselect>_LP_PSEUDOCOST)
                        return false;
                    break;
                case _LP_METHOD:
                    if (rh==at_exact)
                        prob.settings.precision=_LP_EXACT;
                    else if (rh==at_float)
                        prob.settings.precision=_LP_INEXACT;
                    else if (rh.is_integer() && rh.subtype==_INT_MAPLECONVERSION) {
                        if (rh.to_int()==_LP_INTERIOR_POINT) {
                            prob.settings.precision=_LP_INEXACT;
                            prob.settings.solver=_LP_INTERIOR_POINT;
                        } else if (rh.to_int()==_LP_SIMPLEX) {
                            prob.settings.precision=_LP_PROB_DEPENDENT;
                            prob.settings.solver=_LP_SIMPLEX;
                        } else return false;
                    } else return false;
                    break;
                case _LP_GAP_TOLERANCE:
                    a=_evalf(rh,prob.ctx);
                    if (a.type==_DOUBLE_ && is_positive(a,prob.ctx))
                        prob.settings.relative_gap_tolerance=a.DOUBLE_val();
                    else return false;
                    break;
                case _LP_MAXIMIZE:
                    if (rh.is_integer() && rh.subtype==_INT_BOOLEAN)
                        prob.settings.maximize=(bool)rh.to_int();
                    else return false;
                    break;
                case _LP_VERBOSE:
                    if (rh.is_integer() && rh.subtype==_INT_BOOLEAN)
                        prob.settings.verbose=(bool)rh.to_int();
                    else return false;
                    break;
                case _LP_ASSUME:
                    if (rh.is_integer())
                        prob.settings.assumption=rh.to_int();
                    else return false;
                    break;
                case _LP_PRESOLVE:
                    if (rh.is_integer() && rh.subtype==_INT_BOOLEAN)
                        prob.settings.presolve=(bool)rh.to_int();
                    else return false;
                    break;
                case _GT_ACYCLIC:
                    if (rh.is_integer() && rh.subtype==_INT_BOOLEAN)
                        prob.settings.acyclic=(bool)rh.to_int();
                    else return false;
                    break;
                default:
                    break;
                }
            } else return false;
        } else return false;
    }
    return true;
}

int parse_constraints(bool is_matrix_form,const_iterateur &it,const_iterateur &itend,lp_problem &prob) {
    vecteur cv;
    if (is_matrix_form) {
        if (it->type!=_VECT)
            return _LP_ERR_TYPE;
        cv=*it->_VECTptr;
        int n=cv.size();
        if ((n%2)!=0)
            return _LP_ERR_SIZE;
        if (n>0) {
            //constraints are given in form [A,b,Aeq,beq] such that
            //A.x<=b and/or Aeq.x=beq (Aeq and beq are optional)
            if (cv[0].type!=_VECT || cv[1].type!=_VECT)
                return _LP_ERR_TYPE;
            vecteur &A=*cv[0]._VECTptr,&b=*cv[1]._VECTptr;
            int len;
            if ((len=A.size())!=int(b.size()))
                return _LP_ERR_DIM;
            for (int i=0;i<len;++i) {
                prob.constr.append(*A[i]._VECTptr,b[i],_LP_LEQ);
            }
            if (n>2) { //there are equality constraints
                if (cv[2].type!=_VECT || cv[3].type!=_VECT)
                    return _LP_ERR_TYPE;
                vecteur &Aeq=*cv[2]._VECTptr,&beq=*cv[3]._VECTptr;
                if ((len=Aeq.size())!=int(beq.size()))
                    return _LP_ERR_DIM;
                for (int i=0;i<len;++i) {
                    prob.constr.append(*Aeq[i]._VECTptr,beq[i],_LP_EQ);
                }
            }
            if (!ckmatrix(prob.constr.lhs))
                return _LP_ERR_DIM;
            if (prob.nv()==0)
                prob.create_variables(prob.constr.ncols());
            else if (prob.nv()!=prob.constr.ncols())
                return _LP_ERR_DIM;
        }
        //if the third arg is [bl,bu] then assume bl<=x<=bu
        ++it;
        if (it!=itend && it->type==_VECT) {
            matrice &bounds=*it->_VECTptr;
            if (!ckmatrix(bounds) || int(bounds.size())!=2 ||
                    int(bounds.front()._VECTptr->size())<prob.nv())
                return _LP_ERR_DIM;
            if (prob.nv()==0)
                prob.create_variables(bounds.front()._VECTptr->size());
            for (int i=0;i<prob.nv();++i) {
                prob.tighten_variable_bounds(i,bounds[0][i],bounds[1][i]);
            }
            ++it;
        }
        if (prob.nv()==0)
            return _LP_ERR_SIZE;
        if (prob.objective.first.empty())
            prob.set_objective(vecteur(prob.nv(),0),0);
    } else { //the problem is given in symbolic form
        if (it->type==_VECT) {
            cv=*it->_VECTptr;
            prob.add_identifiers_from(cv);
            ++it;
        }
        prob.create_variables(prob.variable_identifiers.size());
        vecteur sides,c;
        pair<gen,gen> bounds;
        gen r;
        int rel;
        for (const_iterateur jt=cv.begin();jt!=cv.end();++jt) {
            if (jt->is_symb_of_sommet(at_equal))
                rel=_LP_EQ;
            else if (jt->is_symb_of_sommet(at_inferieur_egal))
                rel=_LP_LEQ;
            else if (jt->is_symb_of_sommet(at_superieur_egal))
                rel=_LP_GEQ;
            else
                return _LP_ERR_TYPE;
            sides=*jt->_SYMBptr->feuille._VECTptr;
            if (jt->is_symb_of_sommet(at_equal) && interval2pair(sides.back(),bounds,prob.ctx)) {
                if (!prob.lincomb_coeff(sides.front(),c,r))
                    return _LP_ERR_TYPE;
                prob.constr.append(c,bounds.first-r,_LP_GEQ);
                prob.constr.append(c,bounds.second-r,_LP_LEQ);
                continue;
            }
            if (!prob.lincomb_coeff(sides.front()-sides.back(),c,r))
                return _LP_ERR_TYPE;
            prob.constr.append(c,-r,rel);
        }
    }
    if (!parse_options_and_bounds(it,itend,prob))
        return _LP_ERR_TYPE;
    return 0;
}

gen _lpsolve(const gen &args,GIAC_CONTEXT) {
    gen g(args);
    if (g.type==_STRNG)
        g=makesequence(g);
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gensizeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    vecteur obj;
    const_iterateur it=gv.begin(),itend=gv.end();
    lp_problem prob(contextptr); //create LP problem with default settings
    bool is_matrix_form=false;
    if (it->type==_STRNG) { //problem is given in file
        int len=_size(*it,contextptr).to_int();
        string fname(it->_STRNGptr->begin(),it->_STRNGptr->begin()+len);
        if (!prob.glpk_load_from_file(fname.c_str()))
            return undef;
        ++it;
        if (it->type==_VECT)
            return gentypeerr(contextptr);
    } else { //problem is entered manually
        is_matrix_form=it->type==_VECT;
        if (is_matrix_form) {
            obj=*it->_VECTptr;
            if (!obj.empty()) {
                prob.set_objective(obj,0);
                prob.create_variables(obj.size());
            }
        } else {
            prob.add_identifiers_from(*it);
            gen obj_ct;
            if (!prob.lincomb_coeff(*it,obj,obj_ct))
                return gentypeerr(contextptr);
            prob.set_objective(obj,obj_ct);
        }
        ++it;
    }
    switch (parse_constraints(is_matrix_form,it,itend,prob)) {
    case _LP_ERR_SIZE:
        return gensizeerr(contextptr);
    case _LP_ERR_TYPE:
        return gentypeerr(contextptr);
    case _LP_ERR_DIM:
        return gendimerr(contextptr);
    default:
        if (prob.nc()==0) {
            for (int i=0;i<prob.nv();++i) {
                lp_variable &var=prob.variables[i];
                vecteur row(prob.nv(),0);
                if (!is_inf(var.lb())) {
                    row[i]=1;
                    prob.constr.append(row,var.lb(),_LP_GEQ);
                    break;
                }
                else if (!is_inf(var.ub())) {
                    row[i]=1;
                    prob.constr.append(row,var.ub(),_LP_LEQ);
                    break;
                }
            }
            if (prob.nc()==0) {
                prob.message("Error: no constraints detected",true);
                return gensizeerr(contextptr);
            }
        }
        break;
    }
    for (int i=0;i<prob.nv();++i) {
        lp_variable &var=prob.variables[i];
        switch (prob.settings.assumption) {
        case _ZINT:
        case _LP_INTEGER:
            var.set_type(_LP_INTEGERVARIABLES,contextptr);
            break;
        case _LP_BINARY:
            var.set_type(_LP_BINARYVARIABLES,contextptr);
            break;
        case _NONNEGINT:
        case _LP_NONNEGINT:
            var.set_type(_LP_INTEGERVARIABLES,contextptr);
        case _LP_NONNEGATIVE:
            var.tighten_lbound(0,contextptr);
            break;
        }
    }
    bool is_solver_exact;
    switch (prob.settings.precision) {
    case _LP_EXACT:
        is_solver_exact=true;
        break;
    case _LP_INEXACT:
        is_solver_exact=false;
        break;
    case _LP_PROB_DEPENDENT:
        is_solver_exact=!prob.has_approx_coefficients();
        break;
    }
    vector<lp_variable>::const_iterator vt=prob.variables.begin();
    for (;vt!=prob.variables.end();++vt) {
        if (vt->is_integral() && is_zero(vt->lb()) && is_one(vt->ub())) {
            prob.settings.has_binary_vars=true;
            break;
        }
    }
    //solve the problem
    switch (is_solver_exact?prob.solve():prob.glpk_solve()) {
    case _LP_INFEASIBLE:
        prob.message("Error: problem has no feasible solutions",true);
        return vecteur(0);
    case _LP_UNBOUNDED:
        prob.message("Error: problem has unbounded solution",true);
        return makevecteur(prob.settings.maximize?plus_inf:minus_inf,vecteur(0));
    case _LP_ERROR:
        return undef;
    default: //_LP_SOLVED
        return gen(makevecteur(_eval(prob.optimum,contextptr),_eval(prob.output_solution(),contextptr)),_LIST__VECT);
    }
}
static const char _lpsolve_s[]="lpsolve";
static define_unary_function_eval (__lpsolve,&_lpsolve,_lpsolve_s);
define_unary_function_ptr5(at_lpsolve,alias_at_lpsolve,&__lpsolve,0,true)

#ifndef NO_NAMESPACE_GIAC
}
#endif //ndef NO_NAMESPACE_GIAC
