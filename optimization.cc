/*
 * optimization.cc
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
 *
 */

#include "giacPCH.h"
#include "giac.h"
#include "optimization.h"
#include "signalprocessing.h"
#include "graphe.h"
#include <bitset>
#include <string>
#ifdef HAVE_PARI_PARI_H
#include <pari/pari.h>
#endif

using namespace std;

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

/*
 * Disable/enable output to logptr.
 */
void suspend_logging(GIAC_CONTEXT) {
    contextptr->globalptr->_logptr_->setstate(ios_base::failbit);
}
void restore_logging(GIAC_CONTEXT) {
    contextptr->globalptr->_logptr_->clear(contextptr->globalptr->_logptr_->rdstate() & ~ios_base::failbit);
}

/*
 * Return TRUE iff g is an mapleconversion integer (corresponding to v).
 */
bool is_mcint(const gen &g,int v) {
    return g.is_integer() && g.subtype==_INT_MAPLECONVERSION && (v<0 || g.val==v);
}

/*
 * Return the index of g in v, or -1 if g is not contained in v.
 */
int indexof(const gen &g,const vecteur &v) {
    const_iterateur it=find(v.begin(),v.end(),g);
    if (it==v.end())
        return -1;
    return it-v.begin();
}

/*
 * Sort identifiers assumed to be in form x<n>, like x1,x2,...,x9,x10,x11,...
 */
vecteur sort_identifiers(const vecteur &v,GIAC_CONTEXT) {
    if (v.empty())
        return v;
    vecteur lv=*_apply(makesequence(at_size,_apply(makesequence(at_string,v),contextptr)),contextptr)._VECTptr;
    return *mtran(*_sort(mtran(makevecteur(lv,v)),contextptr)._VECTptr)[1]._VECTptr;
}

/*
 * Return 1 if g is a <(=) contraint, 2 if g is a >(=) constraint, and 0 otherwise.
 */
int which_ineq(const gen &g) {
    if (g.is_symb_of_sommet(at_inferieur_egal) || g.is_symb_of_sommet(at_inferieur_strict))
        return 1;
    if (g.is_symb_of_sommet(at_superieur_egal) || g.is_symb_of_sommet(at_superieur_strict))
        return 2;
    return 0;
}

/*
 * Find intersection res of two sorted lists a and b, return the size of res.
 */
int intersect(const vector<int> &a,const vector<int> &b,vector<int> &res) {
    res.resize(a.size()>b.size()?a.size():b.size());
    vector<int>::iterator it=set_intersection(a.begin(),a.end(),b.begin(),b.end(),res.begin());
    res.resize(it-res.begin());
    return res.size();
}

/*
 * Simplify expression g.
 */
gen simp(const gen &g,GIAC_CONTEXT) {
    if (g.type==_VECT) {
        vecteur res;
        for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
            res.push_back(simp(*it,contextptr));
        }
        return res;
    }
    gen ret=recursive_normal(g,contextptr);
    if (_evalf(g,contextptr).is_approx()) {
        suspend_logging(contextptr);
        ret=_simplify(ret,contextptr);
        restore_logging(contextptr);
    }
    return ret;
}

/*
 * Return true iff 'e' is a rational expression with respect to variables in 'vars'.
 */
bool is_rational_wrt_vars(const gen &e,const vecteur &vars) {
    for (const_iterateur it=vars.begin();it!=vars.end();++it) {
        vecteur l(rlvarx(e,*it));
        if (l.size()>1)
            return false;
    }
    return true;
}

/*
 * Return true iff x0 is feasible for constraints constr dependent on x.
 */
bool is_feasible(const vecteur &constr,const vecteur &x,const vecteur &x0,double feas_tol,GIAC_CONTEXT) {
    vecteur p=*_epsilon2zero(x0,contextptr)._VECTptr;
    for (const_iterateur it=constr.begin();it!=constr.end();++it) {
        if (it->is_symb_of_sommet(at_equal)) {
            if (!is_greater(feas_tol,_abs(subst(equal2diff(*it),x,p,false,contextptr),contextptr),contextptr))
                return false;
        } else if (which_ineq(*it)==1) {
            gen d=it->_SYMBptr->feuille._VECTptr->front()-it->_SYMBptr->feuille._VECTptr->back();
            if (!is_greater(feas_tol,max(subst(d,x,p,false,contextptr),0,contextptr),contextptr))
                return false;
        } else if (which_ineq(*it)==2) {
            gen d=it->_SYMBptr->feuille._VECTptr->front()-it->_SYMBptr->feuille._VECTptr->back();
            if (!is_greater(feas_tol,max(-subst(d,x,p,false,contextptr),0,contextptr),contextptr))
                return false;
        } else if (it->is_integer() && it->subtype==_INT_BOOLEAN) { // boolean (e.g. evaluated inequality)
            if (it->val==0)
                return false;
        } else { // an expression, assumed to be zero
            if (!is_greater(feas_tol,_abs(subst(*it,x,p,false,contextptr),contextptr),contextptr))
                return false;
        }
    }
    return true;
}

/*
 * Source: Ibrahim and Chinneck 2005 (in "Feasibility and Infeasibility in Optimization", page 70)
 */
bool constraint_consensus(const vecteur &cv,const vecteur &gconstr,const vecteur &cvars,
                          const vecteur &x,const vecteur &x_lb,const vecteur &x_ub,vecteur &x0,
                          const vector<int> &dir,vector<int> &kl,vector<int> &ku,vecteur &sl,vecteur &su,
                          double alpha,double beta,int maxiter,GIAC_CONTEXT) {
    int NINF,n=x.size(),m=cv.size();
    int iter=1;
    for (;iter<=maxiter;++iter) {
        NINF=0;
        std::fill(kl.begin(),kl.end(),0);
        std::fill(sl.begin(),sl.end(),0);
        std::fill(ku.begin(),ku.end(),0);
        std::fill(su.begin(),su.end(),0);
        for (int i=0;i<m;++i) { // for each constraint c
            vecteur nabla_c=subst(*gconstr[i]._VECTptr,x,x0,false,contextptr);
            gen viol,ec=subst(cv[i],x,x0,false,contextptr);
            viol=dir[i]==0?_abs(ec,contextptr):(dir[i]<0?max(0,ec,contextptr):max(0,-ec,contextptr));
            if (!is_zero(viol,contextptr)) {
                vecteur fv=multvecteur((dir[i]==0?_sign(-ec,contextptr):dir[i])*viol/pow(_l2norm(nabla_c,contextptr),2),nabla_c);
                gen d=_l2norm(fv,contextptr);
                if (is_strictly_greater(d,alpha,contextptr)) {
                    NINF++;
                    for (int j=0;j<n;++j) { // for each variable in c
                        if (!contains(*cvars[i]._VECTptr,x[j]))
                            continue;
                        if (is_strictly_positive(fv[j],contextptr)) {
                            ku[j]++;
                            if (is_strictly_greater(fv[j],su[j],contextptr))
                                su[j]=fv[j];
                        } else if (!is_zero(fv[j],contextptr)) {
                            kl[j]++;
                            if (is_strictly_greater(sl[j],fv[j],contextptr))
                                sl[j]=fv[j];
                        }
                    }
                }
            }
        }
        if (NINF==0)
            return true;
        vecteur t(n,0);
        for (int j=0;j<n;++j) {
            if (kl[j]==ku[j])
                t[j]=(sl[j]+su[j])/2;
            else if (ku[j]>kl[j])
                t[j]=su[j];
            else t[j]=sl[j];
        }
        if (is_greater(beta,_l2norm(t,contextptr),contextptr))
            return false; // movement tolerance exceeded
        x0=addvecteur(x0,t);
        for (int j=0;j<n;++j) {
            if (is_strictly_greater(x0[j],x_ub[j],contextptr))
                x0[j]=x_ub[j];
            else if (is_strictly_greater(x_lb[j],x0[j],contextptr))
                x0[j]=x_lb[j];
        }
    }
    return false; // max number of iterations exceeded
}

/*
 * Return an uniform random real between a and b.
 */
gen rand_uniform(const gen &a,const gen &b,GIAC_CONTEXT) {
    gen fa=_evalf(a,contextptr),fb=_evalf(b,contextptr);
    if (is_zero(b-a,contextptr))
        return fa;
    return _rand(is_greater(b,a,contextptr)?makesequence(fa,fb):makesequence(fb,fa),contextptr);
}

/*
 * Generate a normal variable with parameters mu=0 and sigma.
 * If absolut=true, return the absolute value.
 */
gen rand_normal(const gen &sigma,bool absolut,GIAC_CONTEXT) {
    assert(is_positive(sigma,contextptr));
    gen s=_evalf(sigma,contextptr);
    if (is_zero(sigma,contextptr))
        return 0;
    gen r=_randNorm(makesequence(0,s),contextptr);
    return absolut?_abs(r,contextptr):r;
}

/*
 * Find a feasible point x0 for the system constr of nonlinear constraints.
 * Return TRUE iff the generated point p is feasible with tolerance alpha.
 * The parameter beta is the movement tolerance, and maxiter is the maximum
 * available number of iterations. If these are exceeded, FALSE is returned.
 * If x0 is not initialized, it is generated by randomized standard heuristic.
 * If obj is not undef, then a feasible point with small value of obj is
 * generated by random sampling.
 */
vecteur gen_feasible_point(const gen &obj,const vecteur &constr,const vecteur &x,const vecteur &x_lb,const vecteur &x_ub,
                           int nsamp,double alpha,double beta,int maxiter,GIAC_CONTEXT) {
    assert(alpha>0 && beta>0 && maxiter>=0 && x_lb.size()==x.size() && x_ub.size()==x.size());
    int m=constr.size(),n=x.size(),nbest=5;
    vector<int> kl(n),ku(n),dir(m);
    vecteur sl(n),su(n),cv(m),cvars(m),res;
    // get constraints and compute their gradients
    matrice gconstr;
    for (const_iterateur it=constr.begin();it!=constr.end();++it) {
        int i=it-constr.begin();
        if (it->is_symb_of_sommet(at_equal))
            dir[i]=0;
        else if (which_ineq(*it)==1)
            dir[i]=-1;
        else if (which_ineq(*it)==2)
            dir[i]=1;
        else assert(false);
        cv[i]=it->_SYMBptr->feuille._VECTptr->front()-it->_SYMBptr->feuille._VECTptr->back();
        cvars[i]=_lname(cv[i],contextptr);
        gconstr.push_back(_grad(makesequence(cv[i],x),contextptr));
    }
    map<double,vecteur> cand;
    vecteur p(n);
    if (!is_undef(obj)) {
        gen ov;
        for (int pc=1;pc<=nsamp;++pc) {
            double sigma_guess=(pc*100.0)/nsamp;
            for (int j=0;j<n;++j) {
                if (!is_inf(x_lb[j]) && !is_inf(x_ub[j])) { // doubly bounded
                    double ch=rand_uniform(0,1,contextptr).DOUBLE_val();
                    gen sigma=(x_ub[j]-x_lb[j])/3.0;
                    if (ch<0.25)
                        p[j]=min(x_ub[j],x_lb[j]+rand_normal(sigma,true,contextptr),contextptr);
                    else if (ch>0.75)
                        p[j]=max(x_lb[j],x_ub[j]-rand_normal(sigma,true,contextptr),contextptr);
                    else p[j]=rand_uniform(x_lb[j],x_ub[j],contextptr);
                } else if (!is_inf(x_lb[j])) // single lower bound
                    p[j]=x_lb[j]+rand_normal(sigma_guess,true,contextptr);
                else if (!is_inf(x_ub[j])) // single upper bound
                    p[j]=x_ub[j]-rand_normal(sigma_guess,true,contextptr);
                else p[j]=rand_normal(sigma_guess,false,contextptr); // unbounded in both directions
            }
            if (constraint_consensus(cv,gconstr,cvars,x,x_lb,x_ub,p,dir,kl,ku,sl,su,alpha,beta,maxiter,contextptr)) {
                ov=_evalf(subst(obj,x,p,false,contextptr),contextptr);
                if (ov.is_approx())
                    cand[ov.to_double(contextptr)]=p;
            }
        }
        if (!cand.empty()) {
            map<double,vecteur>::const_iterator it=cand.begin();
            for (;it!=cand.end() && res.size()<nbest;++it) {
                if (!is_zero__VECT(it->second,contextptr))
                    res.push_back(it->second);
            }
            if (res.empty())
                res.push_back(vecteur(n,0.5));
            else return res;
        }
    }
    if (res.empty()) { // apply randomized standard heuristic
        gen D=rand_uniform(0.5,1.0,contextptr);
        for (int j=0;j<n;++j) {
            if (!is_inf(x_lb[j]) && !is_inf(x_ub[j])) { // doubly bounded
                gen r=(x_ub[j]-x_lb[j])/2;
                if (is_greater(D,r,contextptr))
                    D=rand_uniform(r/10,(9*r)/10,contextptr);
                p[j]=(x_lb[j]+x_ub[j])/2+D;
            } else if (!is_inf(x_lb[j])) // single lower bound
                p[j]=x_lb[j]+D;
            else if (!is_inf(x_ub[j])) // single upper bound
                p[j]=x_ub[j]-D;
            else p[j]=D; // unbounded in both directions
        }
        res.push_back(p);
    }
    if (maxiter==0)
        return true;
    /* apply constraint consensus algorithm */
    if (!constraint_consensus(cv,gconstr,cvars,x,x_lb,x_ub,*res.front()._VECTptr,dir,kl,ku,sl,su,alpha,beta,maxiter,contextptr))
        return vecteur(0);
    return res;
}

/* 
 * Simplify critical points.
 */
void cpt_simp(vecteur &cv,const vecteur &vars,const gen &f,GIAC_CONTEXT) {
  for (int j=cv.size();j-->0;) {
    gen sc=simp(_epsilon2zero(cv[j],contextptr),contextptr),img;
    if (is_undef(sc) || !is_constant_wrt_vars(sc,vars,contextptr) ||
            ((img=_evalf(im(sc,contextptr),contextptr)).is_approx() && !is_zero(img,contextptr))) {
        cv.erase(cv.begin()+j);
        continue;
    } else cv[j]=re(cv[j],contextptr);
#if 0
    if (cv[j].type==_VECT && cv[j]._VECTptr->size()==vars.size()) {
        gen val=simp(subst(f,vars,*cv[j]._VECTptr,false,contextptr),contextptr);
        if (is_inf(val) || is_undef(val) || _evalf(val,contextptr).type==_CPLX) {
            // cv[j] is not in the domain of f
            cv.erase(cv.begin()+j);
            continue;
        }
    }
#endif
    cv[j]=simp(cv[j],contextptr);
  }
}

/*
 * Compute the value of g at point x=a. It is assumed that f is continuous in x,
 * hence if f is not defined in x the limit value is returned, if computed successfully.
 * Otherwise, undef is returned. This works for functions up to 16 variables.
 */
gen eval_continuous(const gen &g,const vecteur &x,const vecteur &a,GIAC_CONTEXT) {
    assert(x.size()==a.size());
    int n=x.size(),as=array_start(contextptr);
    if (n>16)
        return eval(subst(g,x,a,false,contextptr),contextptr);
    int N=1<<n;
    map<int,vector<bitset<16> > > bsmap;
    for (int num=0;num<N;++num) {
        bitset<16> bs(num);
        bsmap[bs.count()].push_back(bs);
    }
    for (int k=0;k<=n;++k) {
        const vector<bitset<16> > &supp=bsmap[k];
        vecteur p0(k);
        for (int i=0;i<k;++i) p0[i]=as+i;
        assert((bool)_is_permu(p0,contextptr).val);
        for (vector<bitset<16> >::const_iterator it=supp.begin();it!=supp.end();++it) {
            vector<int> ind;
            for (int i=0;i<n;++i) {
                if (it->test(i)) ind.push_back(i);
            }
            vecteur x0=a;
            for (vector<int>::const_iterator jt=ind.begin();jt!=ind.end();++jt) {
                x0[*jt]=x[*jt];
            }
            gen val=subst(g,x,x0,false,contextptr);
            if (is_undef(val))
                continue;
            if (is_inf(val))
                return val;
            gen p=p0;
            gen v;
            do {
                v=val;
                for (const_iterateur jt=p._VECTptr->begin();!is_undef(v) && jt!=p._VECTptr->end();++jt) {
                    int i=ind[jt->val-as];
                    v=_limit(makesequence(v,x[i],a[i]),contextptr);
                    if (is_inf(v))
                        return v;
                }
            } while (is_undef(v) && !is_undef(p=_nextperm(p,contextptr)));
            if (!is_undef(v))
                return v;
        }
    }
    return undef;
}

/*
 * Call _solve without printing any messages.
 * If an error occurs, empty vector is returned.
 */
vecteur solve_quiet(const gen &e,const gen &x,GIAC_CONTEXT) {
    suspend_logging(contextptr);
    gen sol;
    try {
        sol=_solve(makesequence(e,x),contextptr);
        if (sol.type!=_VECT)
            return vecteur(0);
    } catch (const std::exception &e) {
        restore_logging(contextptr);
        return vecteur(0);
    }
    restore_logging(contextptr);
    return *sol._VECTptr;
}

int var_index=0;

/*
 * Create a temporary symbol "name<count>".
 */
gen temp_symb(const string &name,int count,GIAC_CONTEXT) {
    gen ret=identificateur(" "+name+(count>=0?print_INT_(count):""));
    _purge(ret,contextptr);
    return ret;
}

/*
 * Return true iff g is strictly positive.
 */
bool is_greater_than_zero(const gen &g,const vecteur &vars,GIAC_CONTEXT) {
    vecteur terms(1,g);
    if (g.is_symb_of_sommet(at_plus) && g._SYMBptr->feuille.type==_VECT)
        terms=*g._SYMBptr->feuille._VECTptr;
    bool surplus=false;
    gen rest(0);
    for (const_iterateur it=terms.begin();it!=terms.end();++it) {
        if (_lin(*it,contextptr).is_symb_of_sommet(at_exp) ||
            (_evalf(*it,contextptr).is_approx() && is_strictly_positive(*it,contextptr)))
            surplus=true;
        else rest+=*it;
    }
    if (!surplus)
        return false;
    return is_positive(rest,contextptr);
}

/*
 *Remove strictly positive factors from g.
 */
gen remove_strictly_positive_factors(const gen &g,const vecteur &vars,GIAC_CONTEXT) {
    gen f(g);
    bool is_neg=false;
    if (f.is_symb_of_sommet(at_neg)) {
        f=f._SYMBptr->feuille;
        is_neg=true;
    }
    if (f.is_symb_of_sommet(at_prod) && f._SYMBptr->feuille.type==_VECT) {
        const vecteur &fv=*f._SYMBptr->feuille._VECTptr;
        gen p(1);
        for (const_iterateur jt=fv.begin();jt!=fv.end();++jt) {
            if (is_greater_than_zero(*jt,vars,contextptr))
              continue;
            else p=*jt*p;
        }
        f=p;
    } else if (is_greater_than_zero(f,vars,contextptr))
        f=1;
    return (is_neg?-1:1)*f;
}

/*
 * Solve a system of equations.
 * This function is based on _solve but handles cases where a variable
 * is found inside trigonometric, hyperbolic or exponential functions.
 */
vecteur solve2(const vecteur &e_orig,const vecteur &vars_orig,GIAC_CONTEXT) {
    int m=e_orig.size(),n=vars_orig.size(),i=0;
    vecteur e_orig_simp=*expexpand(expand(_pow2exp(e_orig,contextptr),contextptr),contextptr)._VECTptr;
    for (iterateur it=e_orig_simp.begin();it!=e_orig_simp.end();++it) {
        if (it->is_symb_of_sommet(at_equal))
            *it=equal2diff(*it);
        gen f=(it->type==_SYMB?_factor(*it,contextptr):*it);
        gen num=remove_strictly_positive_factors(_numer(f,contextptr),vars_orig,contextptr);
        gen den=remove_strictly_positive_factors(_denom(f,contextptr),vars_orig,contextptr);
        *it=num/den;
    }
    for (;i<m;++i) {
        if (!is_rational_wrt_vars(e_orig_simp[i],vars_orig))
            break;
    }
    if (n==1 || i==m)
        return solve_quiet(e_orig_simp,vars_orig,contextptr);
    vecteur e(*halftan(_texpand(hyp2exp(e_orig_simp,contextptr),contextptr),contextptr)._VECTptr);
    vecteur lv(*exact(lvar(_evalf(lvar(e),contextptr)),contextptr)._VECTptr);
    vecteur deps(n),depvars(n,gen(0));
    vecteur vars(vars_orig);
    const_iterateur it=lv.begin();
    for (;it!=lv.end();++it) {
        i=0;
        for (;i<n;++i) {
            if (is_undef(vars[i]))
                continue;
            if (*it==(deps[i]=vars[i]) ||
                    *it==(deps[i]=exp(vars[i],contextptr)) ||
                    is_exactly_zero(simp(*it-(deps[i]=tan(vars[i]/2,contextptr)),contextptr))) {
                vars[i]=undef;
                depvars[i]=temp_symb("depvar",i,contextptr);
                break;
            }
        }
        if (i==n)
            break;
    }
    if (it!=lv.end() || contains(depvars,gen(0)))
        return solve_quiet(e_orig_simp,vars_orig,contextptr);
    vecteur e_subs=subst(e,deps,depvars,false,contextptr);
    vecteur sol=solve_quiet(e_subs,depvars,contextptr);
    vecteur ret;
    for (const_iterateur it=sol.begin();it!=sol.end();++it) {
        vecteur r(n);
        i=0;
        for (;i<n;++i) {
            gen c(it->_VECTptr->at(i));
            if (deps[i].type==_IDNT)
                r[i]=c;
            else if (deps[i].is_symb_of_sommet(at_exp) && is_strictly_positive(c,contextptr))
                r[i]=simp(ln(c,contextptr),contextptr);
            else if (deps[i].is_symb_of_sommet(at_tan))
                r[i]=simp(2*atan(c,contextptr),contextptr);
            else
                break;
        }
        if (i==n)
            ret.push_back(r);
    }
    return ret;
}

bool is_ineq_x_a(const gen &g,const gen &var,gen &a,GIAC_CONTEXT) {
    if ((g.is_symb_of_sommet(at_inferieur_egal) ||
         g.is_symb_of_sommet(at_inferieur_strict) ||
         g.is_symb_of_sommet(at_superieur_egal) ||
         g.is_symb_of_sommet(at_superieur_strict)) &&
        g._SYMBptr->feuille.type==_VECT &&
        g._SYMBptr->feuille._VECTptr->front()==var &&
        is_constant_wrt(g._SYMBptr->feuille._VECTptr->back(),var,contextptr))
    {
        a=g._SYMBptr->feuille._VECTptr->back();
        return true;
    }
    return false;
}

/*
 * Traverse the tree of symbolic expression 'e' and collect all points of
 * transition between piecewise subexpressions.
 */
void collect_transition_points(const gen &e,const gen &var,vecteur &cv,GIAC_CONTEXT) {
    if (e.type==_VECT) {
        for (const_iterateur it=e._VECTptr->begin();it!=e._VECTptr->end();++it) {
            collect_transition_points(*it,var,cv,contextptr);
        }
    }
    else if ((e.is_symb_of_sommet(at_piecewise) || e.is_symb_of_sommet(at_when)) &&
             e._SYMBptr->feuille.type==_VECT) {
        const vecteur &f=*e._SYMBptr->feuille._VECTptr;
        int sz=f.size();
        for (int i=0;i<sz/2;++i) {
            vecteur g=solve_quiet(f[2*i],var,contextptr);
            gen a,b;
            for (const_iterateur it=g.begin();it!=g.end();++it) {
                if (is_ineq_x_a(*it,var,a,contextptr))
                    cv.push_back(a);
                else if (it->is_symb_of_sommet(at_and) &&
                            it->_SYMBptr->feuille.type==_VECT &&
                            it->_SYMBptr->feuille._VECTptr->size()==2 &&
                            is_ineq_x_a(it->_SYMBptr->feuille._VECTptr->front(),var,a,contextptr) &&
                            is_ineq_x_a(it->_SYMBptr->feuille._VECTptr->back(),var,b,contextptr)) {
                    cv.push_back(a);
                    cv.push_back(b);
                }
            }
        }
    } else if (e.type==_SYMB)
        collect_transition_points(e._SYMBptr->feuille,var,cv,contextptr);
}

bool next_binary_perm(vector<bool> &perm,int to_end=0) {
    if (to_end==int(perm.size()))
        return false;
    int end=int(perm.size())-1-to_end;
    perm[end]=!perm[end];
    return perm[end]?true:next_binary_perm(perm,to_end+1);
}

vecteur make_temp_vars(const vecteur &vars,const vecteur &ineq,vecteur &bnds,bool open,GIAC_CONTEXT) {
    gen t,a,b,vmin,vmax;
    vecteur tmpvars;
    bnds.resize(vars.size(),vecteur(2,undef));
    for (const_iterateur it=vars.begin();it!=vars.end();++it) {
        vecteur as;
        vmin=vmax=undef;
        for (const_iterateur jt=ineq.begin();jt!=ineq.end();++jt) {
            if (is_linear_wrt(*jt,*it,a,b,contextptr) &&
                    is_constant_wrt_vars(a,vars,contextptr) &&
                    is_constant_wrt_vars(b,vars,contextptr))
                as.push_back(symb_inferieur_egal(*jt,0));
        }
        if (!as.empty())
            as=solve_quiet(as,*it,contextptr);
        if (as.size()==1) {
            const gen &s=as.front();
            if (s.is_symb_of_sommet(at_inferieur_egal) &&
                    s._SYMBptr->feuille._VECTptr->front()==*it)
                vmax=s._SYMBptr->feuille._VECTptr->back();
            else if (s.is_symb_of_sommet(at_superieur_egal) &&
                    s._SYMBptr->feuille._VECTptr->front()==*it)
                vmin=s._SYMBptr->feuille._VECTptr->back();
            else if (s.is_symb_of_sommet(at_and) &&
                        s._SYMBptr->feuille._VECTptr->size()==2 &&
                        s._SYMBptr->feuille._VECTptr->front().is_symb_of_sommet(at_superieur_egal) &&
                        s._SYMBptr->feuille._VECTptr->front()._SYMBptr->feuille._VECTptr->front()==*it &&
                        s._SYMBptr->feuille._VECTptr->back().is_symb_of_sommet(at_inferieur_egal) &&
                        s._SYMBptr->feuille._VECTptr->back()._SYMBptr->feuille._VECTptr->front()==*it) {
                vmin=s._SYMBptr->feuille._VECTptr->front()._SYMBptr->feuille._VECTptr->back();
                vmax=s._SYMBptr->feuille._VECTptr->back()._SYMBptr->feuille._VECTptr->back();
            }
        }
        gen v=temp_symb(it->print(contextptr),-1,contextptr);
        if (!is_undef(vmax) && !is_undef(vmin)) {
            assume_t_in_ab(v,vmin,vmax,open,open,contextptr);
            vecteur &bnd=*bnds[it-vars.begin()]._VECTptr;
            bnd.front()=vmin;
            bnd.back()=vmax;
        } else if (!is_undef(vmin)) {
            giac_assume(open?symb_superieur_strict(v,vmin):symb_superieur_egal(v,vmin),contextptr);
            bnds[it-vars.begin()]._VECTptr->front()=vmin;
        } else if (!is_undef(vmax)) {
            giac_assume(open?symb_inferieur_strict(v,vmax):symb_inferieur_egal(v,vmax),contextptr);
            bnds[it-vars.begin()]._VECTptr->back()=vmax;
        }
        tmpvars.push_back(v);
    }
    return tmpvars;
}

/*
 * Determine critical points of function f under constraints g<=0 and h=0 using KKT conditions.
 */
vecteur solve_kkt(const gen &f,const vecteur &g,const vecteur &h,const vecteur &vars_orig,const vecteur &bnds,GIAC_CONTEXT) {
    int n=vars_orig.size(),m=g.size(),l=h.size(),ineq_constr_threshold=12;
    vecteur vars(vars_orig),mug;
    matrice gr_g,gr_h;
    vars.resize(n+m+l);
    vecteur gr_f=*_grad(makesequence(f,vars_orig),contextptr)._VECTptr;
    bool enu_all=m<=ineq_constr_threshold;
    for (int i=0;i<m;++i) {
        vars[n+i]=temp_symb("mu",++var_index,contextptr);
        if (enu_all)
            giac_assume(symb_superieur_strict(vars[n+i],gen(0)),contextptr);
        else giac_assume(symb_superieur_egal(vars[n+i],gen(0)),contextptr);
        gr_g.push_back(*_grad(makesequence(g[i],vars_orig),contextptr)._VECTptr);
    }
    for (int i=0;i<l;++i) {
        vars[n+m+i]=temp_symb("lambda",++var_index,contextptr);
        gr_h.push_back(*_grad(makesequence(h[i],vars_orig),contextptr)._VECTptr);
    }
    vecteur eqv;
    bool ne;
    for (int i=0;i<n;++i) {
        gen eq(gr_f[i]),rest(0);
        for (int j=0;j<m;++j) {
            rest+=vars[n+j]*gr_g[j][i];
        }
        for (int j=0;j<l;++j) {
            rest+=vars[n+m+j]*gr_h[j][i];
        }
        eqv.push_back(eq+rest);
    }
    eqv=mergevecteur(eqv,h); // primal feasibility
    vector<bool> is_mu_zero(m,false);
    matrice cv;
    do {
        vecteur e(eqv);
        vecteur v(vars);
        for (int i=m-1;i>=0;--i) {
            if (is_mu_zero[i]) {
                e=subst(e,v[n+i],0,false,contextptr);
                v.erase(v.begin()+n+i);
            } else e.push_back(enu_all?g[i]:v[n+i]*g[i]); // complementary slackness
        }
        vecteur eq_numer=*_apply(makesequence(at_numer,e),contextptr)._VECTptr;
        vecteur eq_denom=*_apply(makesequence(at_denom,e),contextptr)._VECTptr;
        vecteur res=solve2(eq_numer,v,contextptr);
        for (const_iterateur it=res.begin();it!=res.end();++it) {
            bool ok=true;
            if (!is_approx(*it)) {
                for (const_iterateur jt=e.begin();ok && jt!=e.end();++jt) {
                    if (!is_constant_wrt_vars(eq_denom[jt-e.begin()],v,contextptr) &&
                            is_zero(simp(subst(eq_denom[jt-e.begin()],v,*(it->_VECTptr),false,contextptr),contextptr)))
                        ok=is_zero(simp(eval_continuous(*jt,v,*(it->_VECTptr),contextptr),contextptr));
                }
            }
            if (ok)
                cv.push_back(*it);
        }
    } while(enu_all && next_binary_perm(is_mu_zero));
    for (iterateur it=cv.begin();it!=cv.end();++it) {
        it->_VECTptr->resize(n);
    }
    for (const_iterateur it=vars.begin()+n;it!=vars.end();++it) {
        for (int i=cv.size();i-->0;) {
            if (contains(*_lname(cv[i],contextptr)._VECTptr,*it))
                cv.erase(cv.begin()+i);
        }
        _purge(*it,contextptr);
    }
    vars.resize(n);
    for (int i=cv.size();i-->0;) {
        for (int j=0;j<m;++j) {
            // check primal feasibility
            gen s=simp(subst(g[j],vars,cv[i],false,contextptr),contextptr);
            if (!is_zero(s,contextptr) && is_strictly_positive(s,contextptr)) {
                cv.erase(cv.begin()+i);
                break;
            }
        }
    }
    cpt_simp(cv,vars,f,contextptr);
    return *_epsilon2zero(cv,contextptr)._VECTptr;
}

/*
 * Determine critical points of an univariate function f(x). Points where it is
 * not differentiable are considered critical as well as zeros of the first
 * derivative. Also, bounds of the range of x are critical points.
 */
matrice critical_univariate(const gen &f,const gen &x,GIAC_CONTEXT) {
    gen df(derive(f,x,contextptr));
    matrice cv;
    gen z=_zeros(makesequence(df,x),contextptr);
    if (z.type==_VECT)
      cv=*z._VECTptr;
    else *logptr(contextptr) << gettext("Error") << ": " << gettext("failed to compute zeros of ") << df << "\n";
    gen den(_denom(df,contextptr));
    if (!is_constant_wrt(den,x,contextptr)) {
        z=_zeros(makesequence(den,x),contextptr);
        if (z.type==_VECT)
          cv=mergevecteur(cv,*z._VECTptr);
    }
    collect_transition_points(f,x,cv,contextptr);
    for (int i=cv.size();i-->0;) {
        if (cv[i].is_symb_of_sommet(at_and))
            cv.erase(cv.begin()+i);
        else
            cv[i]=vecteur(1,cv[i]);
    }
    return *_epsilon2zero(cv,contextptr)._VECTptr;
}

/*
 * Compute global minimum mn and global maximum mx of function f(vars) under
 * conditions g<=0 and h=0. The list of points where global minimum is achieved
 * is returned.
 */
vecteur global_extrema(const gen &f,vecteur &g,const vecteur &h,const vecteur &vars,gen &mn,gen &mx,GIAC_CONTEXT) {
    int n=vars.size();
    matrice cv,bnds;
    vecteur tmpvars=make_temp_vars(vars,g,bnds,false,contextptr);
    gen ff=subst(f,vars,tmpvars,false,contextptr);
    if (n==1) {
        cv=critical_univariate(ff,tmpvars[0],contextptr);
        for (const_iterateur it=g.begin();it!=g.end();++it) {
            gen a,b;
            if (!is_linear_wrt(*it,vars[0],a,b,contextptr) || is_zero(a)) {
              *logptr(contextptr) << gettext("Warning") << ": " << gettext("expected a linear function in ") << vars[0]
                                  << ", " << gettext("got ") << *it << "\n";
              continue;
            }
            cv.push_back(makevecteur(-b/a));
        }
    } else {
        vecteur gg=subst(g,vars,tmpvars,false,contextptr);
#if 0
        for (int i=gg.size();i-->0;) {
            gen evb=_evalb(symbolic(at_inferieur_egal,makevecteur(gg[i],0)),contextptr);
            if (evb.is_integer() && evb.subtype==_INT_BOOLEAN) {
                bool yes=(bool)evb.val;
                if (yes)
                    gg.erase(gg.begin()+i);
                else {
                    *logptr(contextptr) << "Error: the problem is infeasible\n";
                    return vecteur(0);
                }
            }
        }
#endif
        vecteur hh=subst(h,vars,tmpvars,false,contextptr);
        cv=solve_kkt(ff,gg,hh,tmpvars,bnds,contextptr);
    }
    for (const_iterateur it=tmpvars.begin();it!=tmpvars.end();++it) {
        if (!contains(vars,*it))
            _purge(*it,contextptr);
    }
    if (cv.empty())
        return vecteur(0);
    bool min_set=false,max_set=false;
    matrice min_locations;
    for (const_iterateur it=cv.begin();it!=cv.end();++it) {
        gen val=simp(eval_continuous(f,vars,*(it->_VECTptr),contextptr),contextptr);
        if (is_inf(val)) {
          *logptr(contextptr) << gettext("Error") << ": " << gettext("the function is unbounded") << "\n";
          return vecteur(0);
        }
        if (is_undef(val)) {
          *logptr(contextptr) << gettext("Error") << ": " << gettext("failed to compute function value at critical point ")
                              << (it->_VECTptr->size()==1?it->_VECTptr->front():*it) << "\n";
          continue;
        }
        if (min_set && is_zero(simp(val-mn,contextptr),contextptr)) {
            if (!contains(min_locations,*it))
                min_locations.push_back(*it);
        }
        else if (!min_set || is_strictly_greater(mn,val,contextptr)) {
            mn=val;
            min_set=true;
            min_locations=vecteur(1,*it);
        }
        if (!max_set || is_strictly_greater(val,mx,contextptr)) {
            mx=val;
            max_set=true;
        }
    }
    if (n==1) {
        for (int i=0;i<int(min_locations.size());++i) {
            min_locations[i]=min_locations[i][0];
        }
    }
    return min_locations;
}

int parse_varlist(const gen &g,vecteur &vars,vecteur &ineq,vecteur &initial) {
    vecteur varlist(g.type==_VECT?*g._VECTptr:vecteur(1,g));
    int n=0;
    for (const_iterateur it=varlist.begin();it!=varlist.end();++it) {
        if (it->is_symb_of_sommet(at_equal)) {
            vecteur &ops=*it->_SYMBptr->feuille._VECTptr;
            gen &v=ops.front(), &rh=ops.back();
            if (v.type!=_IDNT)
                return 0;
            vars.push_back(v);
            if (rh.is_symb_of_sommet(at_interval)) {
                vecteur &range=*rh._SYMBptr->feuille._VECTptr;
                if (!is_inf(range.front()))
                    ineq.push_back(range.front()-v);
                if (!is_inf(range.back()))
                    ineq.push_back(v-range.back());
            }
            else
                initial.push_back(rh);
        }
        else if (it->type!=_IDNT)
            return 0;
        else
            vars.push_back(*it);
        ++n;
    }
    return n;
}

/*
 * Function 'minimize' minimizes a multivariate continuous real function on a
 * closed and bounded region using the method of Lagrange multipliers. The
 * feasible region is specified by bounding variables or by adding one or more
 * (in)equality constraints.
 *
 * Usage
 * ^^^^^
 *     minimize(obj,[constr],vars,[opt])
 *
 * Parameters
 * ^^^^^^^^^^
 *   - obj                 : objective function to minimize
 *   - constr (optional)   : single equality or inequality constraint or
 *                           a list of constraints, if constraint is given as
 *                           expression it is assumed that it is equal to zero
 *   - vars                : single variable or a list of problem variables, where
 *                           optional bounds of a variable may be set by appending '=a..b'
 *   - location (optional) : the option keyword 'locus' or 'coordinates' or 'point'
 *
 * Objective function must be continuous in all points of the feasible region,
 * which is assumed to be closed and bounded. If one of these condinitions is
 * not met, the final result may be incorrect.
 *
 * When the fourth argument is specified, point(s) at which the objective
 * function attains its minimum value are also returned as a list of vector(s).
 * The keywords 'locus', 'coordinates' and 'point' all have the same effect.
 * For univariate problems, a vector of numbers (x values) is returned, while
 * for multivariate problems it is a vector of vectors, i.e. a matrix.
 *
 * The function attempts to obtain the critical points in exact form, if the
 * parameters of the problem are all exact. It works best for problems in which
 * the lagrangian function gradient and the constraints are rational expressions.
 * Points at which the function is not differentiable are also considered critical.
 * This function handles univariate piecewise functions.
 *
 * If no critical points were obtained, the return value is undefined.
 *
 * Examples
 * ^^^^^^^^
 * minimize(sin(x),[x=0..4])
 *    >> sin(4)
 * minimize(asin(x),x=-1..1)
 *    >> -pi/2
 * minimize(x^2+cos(x),x=0..3)
 *    >> 1
 * minimize(x^4-x^2,x=-3..3,locus)
 *    >> [-1/4,[(sqrt(2))/2,-(sqrt(2))/2]]
 * minimize(abs(x),x=-1..1)
 *    >> 0
 * minimize(x-abs(x),x=-1..1)
 *    >> -2
 * minimize(abs(exp(-x^2)-1/2),x=-4..4)
 *    >> 0
 * minimize(piecewise(x<=-2,x+6,x<=1,x^2,3/2-x/2),x=-3..2)
 *    >> 0
 * minimize(x^2-3x+y^2+3y+3,[x=2..4,y=-4..-2],point)
 *    >> -1,[[2,-2]]
 * minimize(2x^2+y^2,x+y=1,[x,y])
 *    >> 2/3
 * minimize(2x^2-y^2+6y,x^2+y^2<=16,[x,y])
 *    >> -40
 * minimize(x*y+9-x^2-y^2,x^2+y^2<=9,[x,y])
 *    >> -9/2
 * minimize(sqrt(x^2+y^2)-z,[x^2+y^2<=16,x+y+z=10],[x,y,z])
 *    >> -4*sqrt(2)-6
 * minimize(x*y*z,x^2+y^2+z^2=1,[x,y,z])
 *    >> -sqrt(3)/9
 * minimize(sin(x)+cos(x),x=0..20,coordinates)
 *    >> -sqrt(2),[5*pi/4,13*pi/4,21*pi/4]
 * minimize((1+x^2+3y+5x-4*x*y)/(1+x^2+y^2),x^2/4+y^2/3=9,[x,y])
 *    >> -2.44662490691
 * minimize(x^2-2x+y^2+1,[x+y<=0,x^2<=4],[x,y],locus)
 *    >> 1/2,[[1/2,-1/2]]
 * minimize(x^2*(y+1)-2y,[y<=2,sqrt(1+x^2)<=y],[x,y])
 *    >> -4
 * minimize(4x^2+y^2-2x-4y+1,4x^2+y^2=1,[x,y])
 *    >> -sqrt(17)+2
 * minimize(cos(x)^2+cos(y)^2,x+y=pi/4,[x,y],locus)
 *    >> (-sqrt(2)+2)/2,[[5*pi/8,-3*pi/8]]
 * minimize(x^2+y^2,x^4+y^4=2,[x,y])
 *    >> 1.41421356237
 * minimize(z*x*exp(y),z^2+x^2+exp(2y)=1,[x,y,z])
 *    >> -sqrt(3)/9
 * minimize(y,x^2-x^4-y^5=0,[x=-1..1,y=0..1])
 *    >> 0
 * minimize(x*y,x^2-x^4-y^5=0,[x=-1..1,y=0..1])
 *    >> -70*(31381059609/857500000000)^(1/10)/81
 */
gen _minimize(const gen &args,GIAC_CONTEXT) {
    if (args.type==_STRNG && args.subtype==-1) return args;
    if (args.type!=_VECT || args.subtype!=_SEQ__VECT || args._VECTptr->size()>4)
        return gentypeerr(contextptr);
    vecteur &argv=*args._VECTptr,g,h;
    bool location=false;
    int nargs=argv.size();
    if (argv.back()==at_coordonnees || argv.back()==at_lieu || argv.back()==at_point) {
        location=true;
        --nargs;
    }
    if (nargs==3) {
        vecteur constr(argv[1].type==_VECT ? *argv[1]._VECTptr : vecteur(1,argv[1]));
        for (const_iterateur it=constr.begin();it!=constr.end();++it) {
            if (it->is_symb_of_sommet(at_equal))
                h.push_back(equal2diff(*it));
            else if (it->is_symb_of_sommet(at_superieur_egal) ||
                     it->is_symb_of_sommet(at_inferieur_egal)) {
                vecteur &s=*it->_SYMBptr->feuille._VECTptr;
                g.push_back(it->is_symb_of_sommet(at_inferieur_egal)?s[0]-s[1]:s[1]-s[0]);
            } else if (it->type==_IDNT || it->type==_SYMB)
                h.push_back(*it);
            else *logptr(contextptr) << gettext("Warning") << ": " << gettext("ignoring constraint ") << *it << "\n";
        }
    }
    vecteur vars,initial;
    int n;  // number of variables
    if ((n=parse_varlist(argv[nargs-1],vars,g,initial))==0 || !initial.empty())
        return gensizeerr(contextptr);
    const gen &f=argv[0];
    gen mn,mx;
    vecteur loc;
    bool has_symb=false;
    gen simb=_lname(makevecteur(f,g,h),contextptr);
    if (simb.type==_VECT) {
        for (const_iterateur it=simb._VECTptr->begin();it!=simb._VECTptr->end();++it) {
            if (!contains(vars,*it)) {
                has_symb=true;
                break;
            }
        }
    }
    try {
        loc=global_extrema(f,g,h,vars,mn,mx,contextptr);
    } catch (const std::exception &e) {
        restore_logging(contextptr);
        loc.clear();
    }
    if (loc.empty()) {
        if (has_symb)
            return undef;
        *logptr(contextptr) << "Warning: switching to local search\n";
        gen asol=_nlpsolve(makesequence(f,mergevecteur(
          *_zip(makesequence(at_inferieur_egal,g,vecteur(g.size(),0)),contextptr)._VECTptr,
          *_zip(makesequence(at_equal,h,vecteur(h.size(),0)),contextptr)._VECTptr)),contextptr);
        if (asol.type==_VECT && asol._VECTptr->size()>1) {
            mn=asol._VECTptr->front();
            if (location) {
                loc.resize(n);
                gen pos=asol._VECTptr->at(1),v;
                if (pos.type!=_VECT || int(pos._VECTptr->size())!=n) return undef;
                for (const_iterateur it=pos._VECTptr->begin();it!=pos._VECTptr->end();++it) {
                    if (!it->is_symb_of_sommet(at_equal)) return undef;
                    int j=indexof(it->_SYMBptr->feuille._VECTptr->front(),vars);
                    if (j<0)
                        return undef;
                    loc[j]=it->_SYMBptr->feuille._VECTptr->back();
                }
            }
        } else return undef;
    } else mn=simp(mn,contextptr);
    if (location)
        return makevecteur(mn,loc);
    return mn;
}
static const char _minimize_s []="minimize";
static define_unary_function_eval (__minimize,&_minimize,_minimize_s);
define_unary_function_ptr5(at_minimize,alias_at_minimize,&__minimize,0,true)

/*
 * 'maximize' takes the same arguments as the function 'minimize', but
 * maximizes the objective function. See 'minimize' for details.
 *
 * Examples
 * ^^^^^^^^
 * maximize(cos(x),x=1..3)
 *    >> cos(1)
 * maximize(piecewise(x<=-2,x+6,x<=1,x^2,3/2-x/2),x=-3..2)
 *    >> 4
 * minimize(x-abs(x),x=-1..1)
 *    >> -2
 * maximize(x^2-3x+y^2+3y+3,[x=2..4,y=-4..-2])
 *    >> 11
 * maximize(x*y*z,x^2+2*y^2+3*z^2<=1,[x,y,z],point)
 *    >> sqrt(2)/18,[[-sqrt(3)/3,sqrt(6)/6,-1/3],[sqrt(3)/3,-sqrt(6)/6,-1/3],
 *                   [-sqrt(3)/3,-sqrt(6)/6,1/3],[sqrt(3)/3,sqrt(6)/6,1/3]]
 * maximize(x^2-x*y+2*y^2,[x=-1..0,y=-1/2..1/2],coordinates)
 *    >> 2,[[-1,1/2]]
 * maximize(x*y,[x+y^2<=2,x>=0,y>=0],[x,y],locus)
 *    >> 4*sqrt(6)/9,[[4/3,sqrt(6)/3]]
 * maximize(y^2-x^2*y,y<=x,[x=0..2,y=0..2])
 *    >> 4/27
 * maximize(2x+y,4x^2+y^2=8,[x,y])
 *    >> 4
 * maximize(x^2*(y+1)-2y,[y<=2,sqrt(1+x^2)<=y],[x,y])
 *    >> 5
 * maximize(4x^2+y^2-2x-4y+1,4x^2+y^2=1,[x,y])
 *    >> sqrt(17)+2
 * maximize(3x+2y,2x^2+3y^2<=3,[x,y])
 *    >> sqrt(70)/2
 * maximize(x*y,[2x+3y<=10,x>=0,y>=0],[x,y])
 *    >> 25/6
 * maximize(x^2+y^2+z^2,[x^2/16+y^2+z^2=1,x+y+z=0],[x,y,z])
 *    >> 8/3
 * assume(a>0);maximize(x^2*y^2*z^2,x^2+y^2+z^2=a^2,[x,y,z])
 *    >> a^6/27
 */
gen _maximize(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()<2)
        return gentypeerr(contextptr);
    vecteur gv(*g._VECTptr);
    gv[0]=-gv[0];
    gen res=_minimize(_feuille(gv,contextptr),contextptr);
    if (res.type==_VECT && res._VECTptr->size()>0) {
        res._VECTptr->front()=-res._VECTptr->front();
    }
    else if (res.type!=_VECT)
        res=-res;
    return ratnormal(res,contextptr);
}
static const char _maximize_s []="maximize";
static define_unary_function_eval (__maximize,&_maximize,_maximize_s);
define_unary_function_ptr5(at_maximize,alias_at_maximize,&__maximize,0,true)

gen _box_constraints(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()!=2 ||
            g._VECTptr->front().type!=_VECT || g._VECTptr->back().type!=_VECT ||
            g._VECTptr->front()._VECTptr->empty() || g._VECTptr->back()._VECTptr->empty())
        return gentypeerr(gettext("Expected a sequence of two nonempty lists."));
    const vecteur &x=*g._VECTptr->front()._VECTptr;
    const vecteur &b=*g._VECTptr->back()._VECTptr;
    if (!ckmatrix(b,false) || b.size()!=x.size() || b.front()._VECTptr->size()!=2)
        return gendimerr(gettext("Invalid list of bounds."));
    matrice B=mtran(b);
    gen intrv=_zip(makesequence(at_interval,B.front(),B.back()),contextptr);
    return change_subtype(_zip(makesequence(at_equal,x,intrv),contextptr),_SEQ__VECT);
}
static const char _box_constraints_s []="box_constraints";
static define_unary_function_eval (__box_constraints,&_box_constraints,_box_constraints_s);
define_unary_function_ptr5(at_box_constraints,alias_at_box_constraints,&__box_constraints,0,true)

int ipdiff::sum_ivector(const ivector &v,bool drop_last) {
    int res=0;
    for (ivector_iter it=v.begin();it!=v.end()-drop_last?1:0;++it) {
        res+=*it;
    }
    return res;
}

/*
 * IPDIFF CLASS IMPLEMENTATION
 */

ipdiff::ipdiff(const gen &f_orig,const vecteur &g_orig,const vecteur &vars_orig,GIAC_CONTEXT) {
    ctx=contextptr;
    f=f_orig;
    g=g_orig;
    vars=vars_orig;
    ord=0;
    nconstr=g.size();
    nvars=vars.size()-nconstr;
    assert(nvars>0);
    pdv[ivector(nvars,0)]=f; // make the zeroth order derivative initially available
}

void ipdiff::ipartition(int m,int n,ivectors &c,const ivector &p) {
    for (int i=0;i<n;++i) {
        if (!p.empty() && p[i]!=0)
            continue;
        ivector r;
        if (p.empty())
            r.resize(n,0);
        else r=p;
        for (int j=0;j<m;++j) {
            ++r[i];
            int s=sum_ivector(r);
            if (s==m && find(c.begin(),c.end(),r)==c.end())
                c.push_back(r);
            else if (s<m)
                ipartition(m,n,c,r);
            else break;
        }
    }
}

ipdiff::diffterms ipdiff::derive_diffterms(const diffterms &terms,ivector &sig) {
    while (!sig.empty() && sig.back()==0) {
        sig.pop_back();
    }
    if (sig.empty())
        return terms;
    int k=sig.size()-1,p;
    diffterms tv;
    ivector u(nvars+1,0);
    for (diffterms::const_iterator it=terms.begin();it!=terms.end();++it) {
        int c=it->second;
        diffterm t(it->first);
        const ivector_map &h_orig=it->first.second;
        ++t.first.at(k);
        tv[t]+=c;
        --t.first.at(k);
        ivector_map h(h_orig);
        for (ivector_map::const_iterator jt=h_orig.begin();jt!=h_orig.end();++jt) {
            ivector v=jt->first;
            if ((p=jt->second)==0)
                continue;
            if (p==1)
                h.erase(h.find(v));
            else
                --h[v];
            ++v[k];
            ++h[v];
            t.second=h;
            tv[t]+=c*p;
            --h[v];
            --v[k];
            ++h[v];
        }
        t.second=h_orig;
        for (int i=0;i<nconstr;++i) {
            ++t.first.at(nvars+i);
            u[k]=1;
            u.back()=i;
            ++t.second[u];
            tv[t]+=c;
            --t.first.at(nvars+i);
            --t.second[u];
            u[k]=0;
        }
    }
    --sig.back();
    return derive_diffterms(tv,sig);
}

const gen &ipdiff::get_pd(const pd_map &pds,const ivector &sig) const {
    try {
        return pds.at(sig);
    }
    catch (out_of_range &e) {
        return undef;
    }
}

const gen &ipdiff::differentiate(const gen &e,pd_map &pds,const ivector &sig) {
    const gen &pd=get_pd(pds,sig);
    if (!is_undef(pd))
        return pd;
    vecteur v(1,e);
    bool do_derive=false;
    assert(vars.size()<=sig.size());
    for (int i=0;i<int(vars.size());++i) {
        if (sig[i]>0) {
            v=mergevecteur(v,vecteur(sig[i],vars[i]));
            do_derive=true;
        }
    }
    if (do_derive)
        return pds[sig]=_derive(_feuille(v,ctx),ctx);
    return e;
}

void ipdiff::compute_h(const vector<diffterms> &grv,int order) {
    if (g.empty())
        return;
    ivectors hsigv;
    matrice A;
    vecteur b(g.size()*grv.size(),gen(0));
    gen t;
    int grv_sz=grv.size();
    for (int i=0;i<nconstr;++i) {
        for (int j=0;j<grv_sz;++j) {
            vecteur eq(g.size()*grv_sz,gen(0));
            const diffterms &grvj=grv[j];
            for (diffterms::const_iterator it=grvj.begin();it!=grvj.end();++it) {
                ivector sig(it->first.first),hsig;
                sig.push_back(i);
                t=gen(it->second)*differentiate(g[i],pdg,sig);
                for (ivector_map::const_iterator ht=it->first.second.begin();ht!=it->first.second.end();++ht) {
                    if (ht->second==0)
                        continue;
                    const ivector &sigh=ht->first;
                    if (sum_ivector(sigh,true)<order) {
                        gen h(get_pd(pdh,sigh));
                        assert(!is_undef(h));
                        t=t*pow(h,ht->second);
                    }
                    else {
                        assert(ht->second==1);
                        hsig=sigh;
                    }
                }
                if (hsig.empty())
                    b[grv_sz*i+j]-=t;
                else {
                    int k=0,hsigv_sz=hsigv.size();
                    for (;k<hsigv_sz;++k) {
                        if (hsigv[k]==hsig)
                            break;
                    }
                    eq[k]+=t;
                    if (k==hsigv_sz)
                        hsigv.push_back(hsig);
                }
            }
            A.push_back(*simp(eq,ctx)._VECTptr);
        }
    }
    matrice B;
    B.push_back(*simp(b,ctx)._VECTptr);
    matrice invA=*_inv(A,ctx)._VECTptr;
    vecteur sol(*mtran(mmult(invA,mtran(B))).front()._VECTptr);
    for (int i=0;i<int(sol.size());++i) {
        pdh[hsigv[i]]=simp(sol[i],ctx);
    }
}

void ipdiff::find_nearest_terms(const ivector &sig,diffterms &match,ivector &excess) {
    excess=sig;
    int i;
    for (map<ivector,diffterms>::const_iterator it=cterms.begin();it!=cterms.end();++it) {
        ivector ex(nvars,0);
        for (i=0;i<nvars;++i) {
            if ((ex[i]=sig[i]-it->first.at(i))<0)
                break;
        }
        if (i<nvars)
            continue;
        if (sum_ivector(ex)<sum_ivector(excess)) {
            excess=ex;
            match=it->second;
        }
    }
}

void ipdiff::raise_order(int order) {
    if (g.empty())
        return;
    ivectors c;
    ivector excess,init_f(nvars+nconstr,0);
    diffterm init_term;
    init_term.first=init_f;
    diffterms init_terms;
    init_terms[init_term]=1;
    vector<diffterms> grv;
    for (int k=ord+1;k<=order;++k) {
        grv.clear();
        c.clear();
        ipartition(k,nvars,c);
        for (ivectors::const_iterator it=c.begin();it!=c.end();++it) {
            diffterms terms=init_terms;
            find_nearest_terms(*it,terms,excess);
            if (sum_ivector(excess)>0) {
                terms=derive_diffterms(terms,excess);
                cterms[*it]=terms;
            }
            grv.push_back(terms);
        }
        compute_h(grv,k);
    }
    ord=order;
}

void ipdiff::compute_pd(int order,const ivector &sig) {
    gen pd;
    ivectors c;
    ipartition(order,nvars,c);
    for (ivectors::const_iterator ct=c.begin();ct!=c.end();++ct) {
        if (!sig.empty() && sig!=*ct)
            continue;
        if (g.empty()) {
            differentiate(f,pdv,sig);
            continue;
        }
        diffterms &terms=cterms[*ct];
        pd=gen(0);
        for (diffterms::const_iterator it=terms.begin();it!=terms.end();++it) {
            ivector sig(it->first.first);
            gen t(gen(it->second)*differentiate(f,pdf,sig));
            if (!is_exactly_zero(t)) {
                for (ivector_map::const_iterator jt=it->first.second.begin();jt!=it->first.second.end();++jt) {
                    if (jt->second==0)
                        continue;
                    gen h(get_pd(pdh,jt->first));
                    assert(!is_undef(h));
                    t=t*pow(h,jt->second);
                }
                pd+=t;
            }
        }
        pdv[*ct]=simp(pd,ctx);
    }
}

void ipdiff::gradient(vecteur &res) {
    if (nconstr==0)
        res=*_grad(makesequence(f,vars),ctx)._VECTptr;
    else {
        res.resize(nvars);
        ivector sig(nvars,0);
        if (ord<1) {
            raise_order(1);
            compute_pd(1);
        }
        for (int i=0;i<nvars;++i) {
            sig[i]=1;
            res[i]=derivative(sig);
            sig[i]=0;
        }
    }
}

void ipdiff::hessian(matrice &res) {
    if (nconstr==0)
        res=*_hessian(makesequence(f,vars),ctx)._VECTptr;
    else {
        res.clear();
        ivector sig(nvars,0);
        if (ord<2) {
            raise_order(2);
            compute_pd(2);
        }
        for (int i=0;i<nvars;++i) {
            vecteur r(nvars);
            ++sig[i];
            for (int j=0;j<nvars;++j) {
                ++sig[j];
                r[j]=derivative(sig);
                --sig[j];
            }
            res.push_back(r);
            --sig[i];
        }
    }
}

const gen &ipdiff::derivative(const ivector &sig) {
    if (nconstr==0)
        return differentiate(f,pdf,sig);
    int k=sum_ivector(sig); // the order of the derivative
    if (k>ord) {
        raise_order(k);
        compute_pd(k,sig);
    }
    return get_pd(pdv,sig);
}

const gen &ipdiff::derivative(const vecteur &dvars) {
    ivector sig(nvars,0);
    int j;
    for (const_iterateur it=dvars.begin();it!=dvars.end();++it) {
        if ((j=indexof(*it,vars))<0)
            return undef;
        ++sig[j];
    }
    return derivative(sig);
}

void ipdiff::partial_derivatives(int order,pd_map &pdmap) {
    if (nconstr>0 && ord<order) {
        raise_order(order);
        compute_pd(order);
    }
    ivectors c;
    ipartition(order,nvars,c);
    for (ivectors::const_iterator it=c.begin();it!=c.end();++it) {
        pdmap[*it]=derivative(*it);
    }
}

gen ipdiff::taylor_term(const vecteur &a,int k,bool shift) {
    assert(k>=0);
    if (k==0)
        return subst(f,vars,a,false,ctx);
    ivectors sigv;
    ipartition(k,nvars,sigv);
    gen term(0);
    if (nconstr>0) while (k>ord) {
        raise_order(ord+1);
        compute_pd(ord);
    }
    for (ivectors::const_iterator it=sigv.begin();it!=sigv.end();++it) {
        gen pd;
        if (g.empty()) {
            vecteur args(1,f);
            for (int i=0;i<nvars;++i) {
                for (int j=0;j<it->at(i);++j) {
                    args.push_back(vars[i]);
                }
            }
            pd=_derive(_feuille(args,ctx),ctx);
        }
        else
            pd=derivative(*it);
        pd=subst(pd,vars,a,false,ctx);
        for (int i=0;i<nvars;++i) {
            int ki=it->at(i);
            if (ki==0)
                continue;
            pd=pd*(shift?pow(vars[i]-a[i],ki):pow(vars[i],ki))/factorial(ki);
        }
        term+=pd;
    }
    return term;
}

gen ipdiff::taylor(const vecteur &a,int order) {
    assert(order>=0);
    gen T(0);
    for (int k=0;k<=order;++k) {
        T+=taylor_term(a,k);
    }
    return T;
}

/*
 * END OF IPDIFF CLASS
 */

void vars_arrangements(matrice J,ipdiff::ivectors &arrs,GIAC_CONTEXT) {
    int m=J.size(),n=J.front()._VECTptr->size();
    assert(n<=32 && m<n);
    matrice tJ(mtran(J));
    ulong N=std::pow(2,n);
    vector<ulong> sets(comb(n,m).val);
    int i=0;
    for (ulong k=1;k<N;++k) {
        bitset<32> b(k);
        if (b.count()==(size_t)m)
            sets[i++]=k;
    }
    matrice S;
    ipdiff::ivector arr(n);
    for (vector<ulong>::const_iterator it=sets.begin();it!=sets.end();++it) {
        for (i=0;i<n;++i) arr[i]=i;
        N=std::pow(2,n);
        for (i=n;i-->0;) {
            N/=2;
            if ((*it & N)!=0) {
                arr.erase(arr.begin()+i);
                arr.push_back(i);
            }
        }
        S.clear();
        for (ipdiff::ivector::const_iterator it=arr.end()-m;it!=arr.end();++it) {
            S.push_back(tJ[*it]);
        }
        if (!is_zero(_det(S,contextptr)))
            arrs.push_back(arr);
    }
}

matrice jacobian(vecteur &g,vecteur &vars,GIAC_CONTEXT) {
    matrice J;
    for (int i=0;i<int(g.size());++i) {
        gen gr=_grad(makesequence(g[i],vars),contextptr);
        if (gr.type==_VECT && gr._VECTptr->size()==vars.size())
            J.push_back(*gr._VECTptr);
        else {
            *logptr(contextptr) << gettext("Error") << ": " << gettext("failed to compute gradient of ") << g[i] << "\n";
            return vecteur(0);
        }
    }
    return J;
}

bool ck_jacobian(vecteur &g,vecteur &vars,GIAC_CONTEXT) {
    matrice J(jacobian(g,vars,contextptr));
    if (!g.empty() && J.empty())
        return false;
    int m=g.size();
    int n=vars.size()-m;
    if (_rank(J,contextptr).val<m)
        return false;
    J=mtran(J);
    J.erase(J.begin(),J.begin()+n);
    return !is_zero(_det(J,contextptr),contextptr);
}

/*
 * 'implicitdiff' differentiates function(s) defined by equation(s) or a
 * function f(x1,x2,...,xn,y1,y2,...,ym) where y1,...,ym are functions of
 * x1,x2,...,xn defined by m equality constraints.
 *
 * Usage
 * ^^^^^
 *      implicitdiff(f,constr,depvars,diffvars)
 *      implicitdiff(f,constr,vars,order_size=<posint>,[P])
 *      implicitdiff(constr,[depvars],y,diffvars)
 *
 * Parameters
 * ^^^^^^^^^^
 *      - f         : expression
 *      - constr    : (list of) equation(s)
 *      - depvars   : (list of) dependent variable(s), each of them given
 *                    either as a symbol, e.g. y, or a function, e.g. y(x,z)
 *      - diffvars  : sequence of variables w.r.t. which the differentiation
 *                    will be carried out
 *      - vars      : list all variables on which f depends such that
 *                  : dependent variables come after independent ones
 *      - P         : (list of) coordinate(s) to compute derivatives at
 *      - y         : (list of) dependent variable(s) to differentiate w.r.t.
 *                    diffvars, each of them given as a symbol
 *
 * The return value is partial derivative specified by diffvars. If
 * 'order=m' is given as the fourth argument, all partial derivatives of
 * order m will be computed and returned as vector for m=1, matrix for m=2 or
 * table for m>2. The first two cases produce gradient and hessian of f,
 * respectively. For m>2, the partial derivative
 * pd=d^m(f)/(d^k1(x1)*d^k2(x2)*...*d^kn(xn)) is saved under key [k1,k2,...kn].
 * If P is specified, pd(P) is saved.
 *
 * Examples
 * ^^^^^^^^
 * implicitdiff(x^2*y+y^2=1,y,x)
 *      >> -2*x*y/(x^2+2*y)
 * implicitdiff(R=P*V/T,P,T)
 *      >> P/T
 * implicitdiff([x^2+y=z,x+y*z=1],[y(x),z(x)],y,x)
 *      >> (-2*x*y-1)/(y+z)
 * implicitdiff([x^2+y=z,x+y*z=1],[y(x),z(x)],[y,z],x)
 *      >> [(-2*x*y-1)/(y+z),(2*x*z-1)/(y+z)]
 * implicitdiff(y=x^2/z,y,x)
 *      >> 2x/z
 * implicitdiff(y=x^2/z,y,z)
 *      >> -x^2/z^2
 * implicitdiff(y^3+x^2=1,y,x)
 *      >> -2*x/(3*y^2)
 * implicitdiff(y^3+x^2=1,y,x,x)
 *      >> (-8*x^2-6*y^3)/(9*y^5)x+3y-z,2x^2+y^2=z,[x,y,z]
 * implicitdiff(a*x^3*y-2y/z=z^2,y(x,z),x)
 *      >> -3*a*x^2*y*z/(a*x^3*z-2)
 * implicitdiff(a*x^3*y-2y/z=z^2,y(x,z),x,z)
 *      >> (12*a*x^2*y-6*a*x^2*z^3)/(a^2*x^6*z^2-4*a*x^3*z+4)
 * implicitdiff([-2x*z+y^2=1,x^2-exp(x*z)=y],[y(x),z(x)],y,x)
 *      >> 2*x/(y*exp(x*z)+1)
 * implicitdiff([-2x*z+y^2=1,x^2-exp(x*z)=y],[y(x),z(x)],[y,z],x)
 *      >> [2*x/(y*exp(x*z)+1),(2*x*y-y*z*exp(x*z)-z)/(x*y*exp(x*z)+x)]
 * implicitdiff([a*sin(u*v)+b*cos(w*x)=c,u+v+w+x=z,u*v+w*x=z],[u(x,z),v(x,z),w(x,z)],u,z)
 *      >> (a*u*x*cos(u*v)-a*u*cos(u*v)+b*u*x*sin(w*x)-b*x*sin(w*x))/
 *         (a*u*x*cos(u*v)-a*v*x*cos(u*v)+b*u*x*sin(w*x)-b*v*x*sin(w*x))
 * implicitdiff(x*y,-2x^3+15x^2*y+11y^3-24y=0,y(x),x$2)
 *      >> (162*x^5*y+1320*x^4*y^2-320*x^4-3300*x^3*y^3+800*x^3*y+968*x^2*y^4-1408*x^2*y^2+
 *          512*x^2-3630*x*y^5+5280*x*y^3-1920*x*y)/(125*x^6+825*x^4*y^2-600*x^4+
 *          1815*x^2*y^4-2640*x^2*y^2+960*x^2+1331*y^6-2904*y^4+2112*y^2-512)
 * implicitdiff((x-u)^2+(y-v)^2,[x^2/4+y^2/9=1,(u-3)^2+(v+5)^2=1],[v(u,x),y(u,x)],u,x)
 *      >> (-9*u*x-4*v*y+27*x-20*y)/(2*v*y+10*y)
 * implicitdiff(x*y*z,-2x^3+15x^2*y+11y^3-24y=0,[x,z,y],order_size=1)
 *      >> [(2*x^3*z-5*x^2*y*z+11*y^3*z-8*y*z)/(5*x^2+11*y^2-8),x*y]
 * implicitdiff(x*y*z,-2x^3+15x^2*y+11y^3-24y=0,[x,z,y],order_size=2,[1,-1,0])
 *      >> [[64/9,-2/3],[-2/3,0]]
 * pd:=implicitdiff(x*y*z,-2x^3+15x^2*y+11y^3-24y=0,[x,z,y],order_size=4,[0,z,0]);pd[4,0,0]
 *      >> -2*z
 */
gen _implicitdiff(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()<2)
        return gentypeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    const gen &f=gv[0];
    if (int(gv.size())<3)
        return gensizeerr(contextptr);
    int ci=gv[0].type!=_VECT && !gv[0].is_symb_of_sommet(at_equal)?1:0;
    vecteur freevars,depvars,diffdepvars;
    gen_map diffvars;
    // get the constraints as a list of vanishing expressions
    vecteur constr(gv[ci].type==_VECT?*gv[ci]._VECTptr:vecteur(1,gv[ci]));
    for (int i=0;i<int(constr.size());++i) {
        if (constr[i].is_symb_of_sommet(at_equal))
            constr[i]=equal2diff(constr[i]);
    }
    int m=constr.size();
    int dvi=3;
    if (ci==0) {
        if (gv[ci+1].type==_VECT)
            diffdepvars=gv[ci+2].type==_VECT?*gv[ci+2]._VECTptr:vecteur(1,gv[ci+2]);
        else
            dvi=2;
    }
    bool compute_all=false;
    int order=0;
    if (ci==1 && gv[dvi].is_symb_of_sommet(at_equal)) {
        vecteur &v=*gv[dvi]._SYMBptr->feuille._VECTptr;
        if (v.front()!=at_order || !v.back().is_integer())
            return gentypeerr(contextptr);
        order=v.back().val;
        if (order<=0)
            return gendimerr(contextptr);
        compute_all=true;
    }
    // get dependency specification
    vecteur deplist(gv[ci+1].type==_VECT?*gv[ci+1]._VECTptr:vecteur(1,gv[ci+1]));
    if (compute_all) {
        // vars must be specified as x1,x2,...,xn,y1,y2,...,ym
        int nd=deplist.size();
        if (nd<=m)
            return gensizeerr(contextptr);
        for (int i=0;i<nd;++i) {
            if (i<nd-m)
                freevars.push_back(deplist[i]);
            else
                depvars.push_back(deplist[i]);
        }
    }
    else {
        // get (in)dependent variables
        for (const_iterateur it=deplist.begin();it!=deplist.end();++it) {
            if (it->type==_IDNT)
                depvars.push_back(*it);
            else if (it->is_symb_of_sommet(at_of)) {
                vecteur fe(*it->_SYMBptr->feuille._VECTptr);
                depvars.push_back(fe.front());
                if (fe.back().type==_VECT) {
                    for (int i=0;i<int(fe.back()._VECTptr->size());++i) {
                        gen &x=fe.back()._VECTptr->at(i);
                        if (!contains(freevars,x))
                            freevars.push_back(x);
                    }
                }
                else
                    freevars.push_back(fe.back());
            }
            else
                return gentypeerr(contextptr);
        }
        // get diffvars
        for (const_iterateur it=gv.begin()+dvi;it!=gv.end();++it) {
            gen v(eval(*it,contextptr));
            gen x;
            if (v.type==_IDNT)
                diffvars[(x=v)]+=1;
            else if (v.type==_VECT && v.subtype==_SEQ__VECT)
                diffvars[(x=v._VECTptr->front())]+=v._VECTptr->size();
            else
                return gentypeerr(contextptr);
            if (!contains(freevars,x))
                freevars.push_back(x);
        }
    }
    int n=freevars.size();  // number of independent variables
    if (m!=int(depvars.size()))
        return gensizeerr(contextptr);
    vecteur vars(mergevecteur(freevars,depvars));  // list of all variables
    // check whether the conditions of implicit function theorem hold
    if (!ck_jacobian(constr,vars,contextptr))
        return gensizeerr(contextptr);
    // build partial derivative specification 'sig'
    ipdiff::ivector sig(n,0); // sig[i]=k means: derive k times with respect to ith independent variable
    ipdiff ipd(f,constr,vars,contextptr);
    if (compute_all) {
        vecteur pt(0);
        if (int(gv.size())>4) {
            pt=gv[4].type==_VECT?*gv[4]._VECTptr:vecteur(1,gv[4]);
            if (int(pt.size())!=n+m)
                return gensizeerr(contextptr);
        }
        ipdiff::pd_map pdv;
        ipd.partial_derivatives(order,pdv);
        if (order==1) {
            vecteur gr;
            ipd.gradient(gr);
            return pt.empty()?gr:simp(subst(gr,vars,pt,false,contextptr),contextptr);
        }
        else if (order==2) {
            matrice hess;
            ipd.hessian(hess);
            return pt.empty()?hess:simp(subst(hess,vars,pt,false,contextptr),contextptr);
        }
        else {
            ipdiff::ivectors c;
            ipdiff::ipartition(order,n,c);
            gen_map ret_pdv;
            for (ipdiff::ivectors::const_iterator it=c.begin();it!=c.end();++it) {
                vecteur v;
                for (int i=0;i<n;++i) {
                    v.push_back(gen(it->at(i)));
                }
                ret_pdv[v]=pt.empty()?pdv[*it]:simp(subst(pdv[*it],vars,pt,false,contextptr),contextptr);
            }
            return ret_pdv;
        }
    }
    for (gen_map::const_iterator it=diffvars.begin();it!=diffvars.end();++it) {
        int i=0;
        for (;i<n;++i) {
            if (it->first==freevars[i]) {
                sig[i]=it->second.val;
                break;
            }
        }
        assert(i<n);
    }
    // compute the partial derivative specified by 'sig'
    order=ipdiff::sum_ivector(sig);
    if (ci==1)
        return simp(ipd.derivative(sig),contextptr);
    vecteur ret;
    if (diffdepvars.empty()) {
        assert(m==1);
        diffdepvars=vecteur(1,depvars.front());
    }
    for (const_iterateur it=diffdepvars.begin();it!=diffdepvars.end();++it) {
        if (!contains(depvars,*it)) {
            // variable *it is not in depvars, so it's treated as a constant
            ret.push_back(0);
            continue;
        }
        ipdiff tmp(*it,constr,vars,contextptr);
        ret.push_back(simp(tmp.derivative(sig),contextptr));
    }
    return ret.size()==1?ret.front():ret;
}
static const char _implicitdiff_s []="implicitdiff";
static define_unary_function_eval (__implicitdiff,&_implicitdiff,_implicitdiff_s);
define_unary_function_ptr5(at_implicitdiff,alias_at_implicitdiff,&__implicitdiff,0,true)

iterateur find_cpt(vecteur &cpts,const vecteur &cand,GIAC_CONTEXT) {
    iterateur ret;
    for (ret=cpts.begin();ret!=cpts.end();++ret) {
        if (ret->type!=_VECT || ret->_VECTptr->size()!=2)
            continue;
        if (ret->_VECTptr->front().type==_VECT) {
            const vecteur &c=*ret->_VECTptr->front()._VECTptr;
            if (c.size()!=cand.size())
                continue;
            const_iterateur it;
            for (it=c.begin();it!=c.end();++it) {
                if (!is_zero(simp(*it-cand[it-c.begin()],contextptr),contextptr))
                    break;
            }
            if (it==c.end())
                break;
        }
    }
    return ret;
}

/* classification using the bordered Hessian and Theorem 1 of David Spring (1985) */
int critical_point_class(const matrice &hess,int n,int m,GIAC_CONTEXT) {
    vecteur s;
    int i,j,k;
    for (k=1;k<=n;++k) {
        matrice M;
        for (i=0;i<2*m+k;++i) {
            const vecteur &row=*hess[i]._VECTptr;
            M.push_back(vecteur(row.begin(),row.begin()+2*m+k));
        }
        s.push_back(simp(pow(gen(-1),m)*_det(M,contextptr),contextptr));
    }
    if (contains(s,gen(0)))
        return _CPCLASS_UNDECIDED; // paranoid check, a counterexample exists for Theorem 1 of D.S.
    for (i=n-1;i>=0 && is_zero(s[i],contextptr);--i);
    if (i<0) return _CPCLASS_UNDECIDED; // the sequence is trivial
    for (j=0;j<=i && is_strictly_positive(s[j],contextptr);++j);
    if (j>i) return i==n-1?_CPCLASS_MIN:_CPCLASS_UNDECIDED;
    for (j=0;j<=i && is_strictly_positive(pow(gen(-1),j+1)*s[j],contextptr);++j);
    if (j>i) return i==n-1?_CPCLASS_MAX:_CPCLASS_UNDECIDED;
    return _CPCLASS_SADDLE;
}

vecteur find_vars(const gen &e,const vecteur &vars,GIAC_CONTEXT) {
    vecteur ret;
    for (const_iterateur it=vars.begin();it!=vars.end();++it) {
        if (!is_constant_wrt(e,*it,contextptr))
            ret.push_back(*it);
    }
    return ret;
}

bool test_parameters(const vecteur &cpt,const vecteur &vars,const vecteur &ineq,GIAC_CONTEXT) {
    vecteur csts=*_lname(cpt,contextptr)._VECTptr,rest_conds;
    gen_map conds;
    if (csts.empty())
        return true;
    for (const_iterateur it=cpt.begin();it!=cpt.end();++it) {
        vecteur fv=find_vars(*it,csts,contextptr);
        if (fv.empty()) continue;
        const gen &vr=vars[it-cpt.begin()];
        for (const_iterateur jt=ineq.begin();jt!=ineq.end();++jt) {
            if (is_constant_wrt(*jt,vr,contextptr)) continue;
            gen inq=symb_inferieur_strict(subst(*jt,vr,*it,false,contextptr),0);
            if (fv.size()==1) {
                gen &iqv=conds[fv.front()];
                if (iqv.type==_VECT)
                    iqv._VECTptr->push_back(inq);
                else iqv=vecteur(1,inq);
            } else rest_conds.push_back(inq);
        }
    }
    for (gen_map::const_iterator it=conds.begin();it!=conds.end();++it) {
        if (it->second.type!=_VECT) continue;
        vecteur inqsol=solve_quiet(it->second,it->first,contextptr);
        if (inqsol.empty())
            return false;
        *logptr(contextptr) << gettext("Warning") << ": " << gettext("assuming ");
        for (const_iterateur jt=inqsol.begin();jt!=inqsol.end();++jt) {
            *logptr(contextptr) << *jt;
            if (jt+1!=inqsol.end())
                *logptr(contextptr) << " or ";
        }
        *logptr(contextptr) << "\n";
    }
    if (!rest_conds.empty())
        *logptr(contextptr) << gettext("Warning") << ": " << gettext("assuming ") << rest_conds << "\n";
    return true;
}

void find_local_extrema(vecteur &cpts,const gen &f,const vecteur &g,const vecteur &vars,const ipdiff::ivector &arr,const vecteur &ineq,const vecteur &initial,int order_size,bool approx_hompol,GIAC_CONTEXT) {
    assert(order_size>=0);
    int nv=vars.size(),m=g.size(),n=nv-m,cls;
    vecteur bnds;
    vecteur tmpvars=make_temp_vars(vars,ineq,bnds,true,contextptr);
    if (order_size==0 && m>0) { // apply the method of Lagrange
        gen L(f);
        vecteur multipliers(m),allinitial;
        if (!initial.empty())
            allinitial=mergevecteur(vecteur(m,0),initial);
        for (int i=m;i-->0;) {
            L+=-(multipliers[i]=temp_symb("lambda",++var_index,contextptr))*g[i];
        }
        L=subst(L,vars,tmpvars,false,contextptr);
        vecteur allvars=mergevecteur(tmpvars,multipliers);
        matrice cv,bhess;
        gen tmpgr=_grad(makesequence(L,allvars),contextptr);
        if (tmpgr.type==_VECT) {
            vecteur &gr=*tmpgr._VECTptr;
            if (allinitial.empty()) {
                cv=solve2(gr,allvars,contextptr);
                cpt_simp(cv,tmpvars,subst(f,vars,tmpvars,false,contextptr),contextptr);
            } else {
                gen tmpfsol=_fsolve(makesequence(gr,allvars,allinitial),contextptr);
                if (tmpfsol.type==_VECT) {
                    vecteur &fsol=*tmpfsol._VECTptr;
                    if (!fsol.empty())
                        cv.push_back(fsol);
                }
            }
            for (iterateur it=cv.begin();it!=cv.end();++it) {
                *it=mergevecteur(vecteur(it->_VECTptr->begin()+nv,it->_VECTptr->end()),
                                 vecteur(it->_VECTptr->begin(),it->_VECTptr->begin()+nv));
            }
            allvars=mergevecteur(vecteur(allvars.begin()+nv,allvars.end()),
                                 vecteur(allvars.begin(),allvars.begin()+nv));
            if (!cv.empty()) {
                gen tmphess=_hessian(makesequence(L,allvars),contextptr);
                if (ckmatrix(tmphess))
                    bhess=*tmphess._VECTptr; // bordered Hessian
            }
            gen s;
            for (const_iterateur it=cv.begin();it!=cv.end();++it) {
                if (!test_parameters(*(it->_VECTptr),vars,ineq,contextptr))
                    continue;
                cls=_CPCLASS_UNDECIDED;
                if (!bhess.empty())
                    cls=critical_point_class(subst(bhess,allvars,*it,false,contextptr),n,m,contextptr);
                vecteur cpt=vecteur(it->_VECTptr->begin()+m,it->_VECTptr->end());
                iterateur cit=find_cpt(cpts,cpt,contextptr);
                if (cit!=cpts.end()) {
                    if (cls!=_CPCLASS_UNDECIDED)
                        cit->_VECTptr->back()=cls;
                } else cpts.push_back(makevecteur(cpt,cls));
            }
        }
    } else if (order_size>0) { // use implicit differentiation instead of Lagrange multipliers
        vecteur gr,taylor_terms,a(nv),cpt_arr(nv);
        ipdiff ipd(f,g,vars,contextptr);
        ipd.gradient(gr);
        matrice cv,hess;
        vecteur eqv=subst(mergevecteur(gr,g),vars,tmpvars,false,contextptr);
        if (initial.empty()) {
            cv=solve2(subst(eqv,vars,tmpvars,false,contextptr),tmpvars,contextptr);
            cpt_simp(cv,tmpvars,subst(f,vars,tmpvars,false,contextptr),contextptr);
        } else {
            gen tmpfsol=_fsolve(makesequence(eqv,tmpvars,initial),contextptr);
            if (tmpfsol.type==_VECT) {
                vecteur &fsol=*tmpfsol._VECTptr;
                if (!fsol.empty())
                    cv.push_back(fsol);
            }
        }
        if (!cv.empty()) {
            if (nv==1) {
                gen d;
                const gen &x=vars.front();
                for (const_iterateur it=cv.begin();it!=cv.end();++it) {
                    const gen &x0=it->_VECTptr->front();
                    cls=_CPCLASS_UNDECIDED;
                    for (int k=2;k<=order_size;++k) {
                        d=simp(subst(derive(f,x,k,contextptr),x,x0,false,contextptr),contextptr);
                        if (is_zero(d,contextptr))
                            continue;
                        if ((k%2)!=0)
                            cls=_CPCLASS_SADDLE;
                        else cls=is_strictly_positive(d,contextptr)?_CPCLASS_MIN:_CPCLASS_MAX;
                        break;
                    }
                    cpts.push_back(makevecteur(x0,cls));
                }
            } else {
                vecteur fvars(vars),ip;
                fvars.resize(n);
                gen l=temp_symb("lambda",++var_index,contextptr);
                bool approx_hp;
                gen pmin,pmax,sp(-1);
                for (int j=0;j<n;++j) {
                    sp+=pow(vars[j],2);
                    ip.push_back(sqrt(fraction(1,n),contextptr));
                }
                if (order_size>1)
                    ipd.hessian(hess);
                for (int i=0;i<nv;++i) {
                    a[i]=temp_symb("a",i,contextptr);
                }
                for (const_iterateur it=cv.begin();it!=cv.end();++it) {
                    if (!test_parameters(*(it->_VECTptr),vars,ineq,contextptr))
                        continue;
                    for (int j=0;j<nv;++j) {
                        cpt_arr[arr[j]]=it->_VECTptr->at(j);
                    }
                    iterateur ct=find_cpt(cpts,cpt_arr,contextptr);
                    if (ct==cpts.end()) {
                        cpts.push_back(makevecteur(cpt_arr,0));
                        ct=cpts.begin()+cpts.size()-1;
                    }
                    gen &cpt_class=ct->_VECTptr->back();
                    if (order_size==1 || !is_exactly_zero(cpt_class))
                        continue;
                    cls=_CPCLASS_UNDECIDED;
                    if (order_size>=2) {
                        for (int k=2;k<=order_size;++k) {
                            if (int(taylor_terms.size())<k-1)
                                taylor_terms.push_back(ipd.taylor_term(a,k,false));
                            if (is_zero(expand(taylor_terms[k-2],contextptr),contextptr))
                                break;
                            vecteur csts=*_lname(taylor_terms[k-2],contextptr)._VECTptr;
                            for (int j=csts.size();j-->0;) {
                                if (contains(vars,csts[j]) || contains(a,csts[j]))
                                    csts.erase(csts.begin()+j);
                            }
                            if (!csts.empty()) {
                                vecteur csol=solve_quiet(symb_equal(taylor_terms[k-2],0),csts,contextptr);
                                vecteur sub=subst(csol,a,*it,false,contextptr);
                                if (!is_constant_wrt_vars(sub,csts,contextptr))
                                    sub=solve_quiet(sub,csts,contextptr);
                                if (!sub.empty()) {
                                    *logptr(contextptr)
                                        << gettext("Warning") << ": " << gettext("assuming ") << csts << gettext(" not in ")
                                        << _simplify(sub,contextptr) << "\n";
                                }
                            }
                            gen p=simp(subst(taylor_terms[k-2],a,*it,false,contextptr),contextptr);
                            if (is_zero(p,contextptr))
                                continue;
                            else if (k%2)
                                cls=_CPCLASS_SADDLE;
                            else {
                                gen gL,tmp(undef);
                                if ((approx_hp=approx_hompol)) {
                                    vecteur hpvars=*_lname(p,contextptr)._VECTptr;
                                    const_iterateur hit=hpvars.begin();
                                    for (;hit!=hpvars.end() && contains(fvars,*hit);++hit);
                                    if (hit==hpvars.end()) {
                                        gL=_fMin(makesequence(p,vecteur(1,symb_equal(sp,0)),fvars,ip),
                                                 contextptr);
                                        if (gL.type==_VECT && int(gL._VECTptr->size())==n) {
                                            tmp=vecteur(1,_epsilon2zero(subst(p,fvars,*gL._VECTptr,
                                                                              false,contextptr),
                                                                        contextptr));
                                            gL=_fMax(makesequence(p,vecteur(1,symb_equal(sp,0)),
                                                                  fvars,ip),contextptr);
                                            if (gL.type==_VECT && int(gL._VECTptr->size())==n)
                                                tmp._VECTptr->push_back(_epsilon2zero(
                                                    subst(p,fvars,*gL._VECTptr,false,contextptr),
                                                    contextptr));
                                            else tmp=undef;
                                        }
                                    }
                                }
                                if (is_undef(tmp)) {
                                    approx_hp=false;
                                    fvars.push_back(l);
                                    gL=_grad(makesequence(p-l*sp,fvars),contextptr);
                                    if (gL.type==_VECT)
                                        tmp=solve_quiet(gL,fvars,contextptr);
                                }
                                if (tmp.type!=_VECT || tmp._VECTptr->empty() || (!approx_hp && !ckmatrix(tmp))) {
                                    if (k>2) break;
                                    // apply the second order derivative test
                                    matrice chess=*simp(subst(hess,vars,*it,false,contextptr),
                                                        contextptr)._VECTptr;
                                    if (is_undef(chess))
                                        break;
                                    if (_lname(chess,contextptr)._VECTptr->empty()) // no symbols here
                                        chess=*_evalf(chess,contextptr)._VECTptr;
                                    gen res=_eigenvals(chess,contextptr);
                                    if (res.type==_VECT) {
                                        vecteur eigvals=*res._VECTptr;
                                        gen e=undef;
                                        for (const_iterateur et=eigvals.begin();et!=eigvals.end();++et) {
                                            if (is_zero(*et,contextptr)) {
                                                cls=_CPCLASS_UNDECIDED;
                                                break;
                                            } else if (is_undef(e)) {
                                                e=*et;
                                                cls=is_positive(e,contextptr)?_CPCLASS_MIN:_CPCLASS_MAX;
                                            } else if (is_strictly_positive(-e*(*et),contextptr))
                                                cls=_CPCLASS_SADDLE;
                                        }
                                    }
                                    break;
                                }
                                if (approx_hp) {
                                    pmin=tmp._VECTptr->front();
                                    pmax=tmp._VECTptr->back();
                                } else {
                                    vecteur lst;
                                    const_iterateur mt=tmp._VECTptr->begin();
                                    for (;mt!=tmp._VECTptr->end();++mt) {
                                        lst.push_back(simp(subst(p,fvars,*mt->_VECTptr,
                                                                     false,contextptr),
                                                               contextptr));
                                    }
                                    pmin=_min(lst,contextptr);
                                    pmax=_max(lst,contextptr);
                                    fvars.pop_back();
                                }
                                if (is_zero(pmin,contextptr))
                                    cls=_CPCLASS_POSSIBLE_MIN;
                                else if (is_zero(pmax,contextptr))
                                    cls=_CPCLASS_POSSIBLE_MAX;
                                else if (is_strictly_positive(pmin,contextptr))
                                    cls=_CPCLASS_MIN;
                                else if (is_strictly_positive(pmax,contextptr))
                                    cls=_CPCLASS_SADDLE;
                                else
                                    cls=_CPCLASS_MAX;
                            }
                            break;
                        }
                    }
                    cpt_class=gen(cls);
                }
            }
        }
    }
    for (const_iterateur it=tmpvars.begin();it!=tmpvars.end();++it) {
        if (!contains(vars,*it))
            _purge(*it,contextptr);
    }
}

/*
 * 'extrema' attempts to find all points of strict local minima/maxima of a
 * smooth (uni/multi)variate function (subject to equality constraints).
 *
 * Usage
 * ^^^^^
 *     extrema(expr,[constr],vars,[order=n||lagrange])
 *
 * Parameters
 * ^^^^^^^^^^
 *   - expr               : differentiable expression
 *   - constr (optional)  : (list of) equality constraint(s)
 *   - vars               : (list of) problem variable(s)
 *   - order (optional)   : specify 'order_size=<nonnegative integer>' to
 *                          bound the order of the derivatives being
 *                          inspected when classifying the critical points
 *
 * The number of constraints must be less than the number of variables. When
 * there are more than one constraint/variable, they must be specified in
 * form of list.
 *
 * Variables may be specified with symbol, e.g. 'var', or by using syntax
 * 'var=a..b', which restricts the variable 'var' to the open interval (a,b),
 * where a and b are real numbers or +/-infinity. If variable list includes a
 * specification of initial point, such as, for example, [x=1,y=0,z=2], then
 * numeric solver is activated to find critical point in the vicinity of the
 * given point. In this case, the single critical point, if found, is examined.
 *
 * The function attempts to find the critical points in exact form, if the
 * parameters of the problem are all exact. It works best for problems in which
 * the gradient of lagrangian function consists of rational expressions. The
 * result may be inexact, however, if exact solutions could not be obtained.
 *
 * For classifying critical points, the bordered hessian method is used first.
 * It is only a second order test, so it may be inconclusive in some cases. In
 * these cases function looks at higher-order derivatives, up to order
 * specified by 'order' option (the extremum test). Set 'order' to 1
 * to use only the bordered hessian test or 0 to output critical points without
 * attempting to classify them. Setting 'order' to 2 or higher activates
 * checking for saddle points and inspecting higher derivatives (up to 'order')
 * to determine the nature of some or all unclassified critical points.
 * By default, 'order' equals to 5.
 *
 * The return value is a sequence with two elements: list of strict local
 * minima and list of strict local maxima. If only critical points are
 * requested (by setting 'order_size' to 0), the output consists of a single
 * list, as no classification was attempted. For univariate problems the points
 * are real numbers, while for multivariate problems they are specified as
 * lists of coordinates, so "lists of points" are in fact matrices with rows
 * corresponding to points in multivariate cases, i.e. vectors in univariate
 * cases.
 *
 * The function prints out information about saddle/inflection points and also
 * about critical points for which no decision could be made, so that the user
 * can inspect candidates for local extrema by plotting the graph, for example.
 *
 * Examples
 * ^^^^^^^^
 * extrema(-2*cos(x)-cos(x)^2,x)
 *    >> [0],[pi]
 * extrema((x^3-1)^4/(2x^3+1)^4,x=0..inf)
 *    >> [1],[]
 * extrema(x/2-2*sin(x/2),x=-12..12)
 *    >> [2*pi/3,-10*pi/3],[10*pi/3,-2*pi/3]
 * extrema(x-ln(abs(x)),x)
 *    >> [1],[]
 * assume(a>=0);extrema(x^2+a*x,x)
 *    >> [-a/2],[]
 * extrema(x^7+3x^6+3x^5+x^4+2x^2-x,x)
 *    >> [0.225847362349],[-1.53862319761]
 * extrema((x^2+x+1)/(x^4+1),x)
 *    >> [],[0.697247087784]
 * extrema(x^2+exp(-x),x)
 *    >> [LambertW(1/2)],[]
 * extrema(exp(-x)*ln(x),x)
 *    >> [],[exp(LambertW(1))]
 * extrema(tan(x)*(x^3-5x^2+1),x=-0.5)
 *    >> [-0.253519032024],[]
 * extrema(tan(x)*(x^3-5x^2+1),x=0.5)
 *    >> [],[0.272551772027]
 * extrema(exp(x^2-2x)*ln(x)*ln(1-x),x=0.5)
 *    >> [],[0.277769149124]
 * extrema(ln(2+x-sin(x)^2),x=0..2*pi)
 *    >> [],[] (needed to compute third derivative to drop critical points pi/4 and 5pi/4)
 * extrema(x^3-2x*y+3y^4,[x,y])
 *    >> [[12^(1/5)/3,(12^(1/5))^2/6]],[]
 * extrema((2x^2-y)*(y-x^2),[x,y])  //Peano surface
 *    >> [],[] (saddle point at origin)
 * extrema(5x^2+3y^2+x*z^2-z*y^2,[x,y,z])
 *    >> [],[] (possible local minimum at origin, in fact saddle)
 * extrema(3*atan(x)-2*ln(x^2+y^2+1),[x,y])
 *    >> [],[[3/4,0]]
 * extrema(x*y,x+y=1,[x,y])
 *    >> [],[[1/2,1/2]]
 * extrema(sqrt(x*y),x+y=2,[x,y])
 *    >> [],[[1,1]]
 * extrema(x*y,x^3+y^3=16,[x,y])
 *    >> [],[[2,2]]
 * extrema(x^2+y^2,x*y=1,[x=0..inf,y=0..inf])
 *    >> [[1,1]],[]
 * extrema(ln(x*y^2),2x^2+3y^2=8,[x,y])
 *    >> [],[[2*sqrt(3)/3,-4/3],[2*sqrt(3)/3,4/3]]
 * extrema(y^2+4y+2x-x^2,x+2y=2,[x,y])
 *    >> [],[[-2/3,4/3]]
 * assume(a>0);extrema(x/a^2+a*y^2,x+y=a,[x,y])
 *    >> [[(2*a^4-1)/(2*a^3),1/(2*a^3)]],[]
 * extrema(6x+3y+2z,4x^2+2y^2+z^2=70,[x,y,z])
 *    >> [[-3,-3,-4]],[[3,3,4]]
 * extrema(x*y*z,x+y+z=1,[x,y,z])
 *    >> [],[[1/3,1/3,1/3]]
 * extrema(x*y^2*z^2,x+y+z=5,[x,y,z])
 *    >> [],[[1,2,2]]
 * extrema(4y-2z,[2x-y-z=2,x^2+y^2=1],[x,y,z])
 *    >> [[2*sqrt(13)/13,-3*sqrt(13)/13,(7*sqrt(13)-26)/13]],
 *       [[-2*sqrt(13)/13,3*sqrt(13)/13,(-7*sqrt(13)-26)/13]]
 * extrema((x-3)^2+(y-1)^2+(z-1)^2,x^2+y^2+z^2=4,[x,y,z])
 *    >> [[6*sqrt(11)/11,2*sqrt(11)/11,2*sqrt(11)/11]],
 *       [[-6*sqrt(11)/11,-2*sqrt(11)/11,-2*sqrt(11)/11]]
 * extrema(x+3y-z,2x^2+y^2=z,[x,y,z])
 *    >> [],[[1/4,3/2,19/8]]
 * extrema(2x*y+2y*z+x*z,x*y*z=4,[x,y,z])
 *    >> [[2,1,2]],[]
 * extrema(x+y+z,[x^2+y^2=1,2x+z=1],[x,y,z])
 *    >> [[sqrt(2)/2,-sqrt(2)/2,-sqrt(2)+1]],[[-sqrt(2)/2,sqrt(2)/2,sqrt(2)+1]]
 * assume(a>0);extrema(x+y+z,[y^2-x^2=a,x+2z=1],[x,y,z])
 *    >> [[-sqrt(3)*sqrt(a)/3,2*sqrt(3)*sqrt(a)/3,(sqrt(3)*sqrt(a)+3)/6]],
 *       [[sqrt(3)*sqrt(a)/3,-2*sqrt(3)*sqrt(a)/3,(-sqrt(3)*sqrt(a)+3)/6]]
 * extrema((x-u)^2+(y-v)^2,[x^2/4+y^2/9=1,(u-3)^2+(v+5)^2=1],[u,v,x,y])
 *    >> [[2.35433932354,-4.23637555425,0.982084902545,-2.61340692712]],
 *       [[3.41406613851,-5.91024679428,-0.580422508346,2.87088778158]]
 * extrema(x2^6+x1^3+4x1+4x2,x1^5+x2^4+x1+x2=0,[x1,x2])
 *    >> [[-0.787457596325,0.758772338924],[-0.784754836317,-1.23363062357]],
 *       [[0.154340184382,-0.155005038065]]
 * extrema(x*y,-2x^3+15x^2*y+11y^3-24y=0,[x,y])
 *    >> [[sqrt(17)*sqrt(3*sqrt(33)+29)/17,sqrt(-15*sqrt(33)+127)*sqrt(187)/187],
 *        [-sqrt(17)*sqrt(3*sqrt(33)+29)/17,-sqrt(-15*sqrt(33)+127)*sqrt(187)/187],
 *        [sqrt(-3*sqrt(33)+29)*sqrt(17)/17,-sqrt(15*sqrt(33)+127)*sqrt(187)/187],
 *        [-sqrt(-3*sqrt(33)+29)*sqrt(17)/17,sqrt(15*sqrt(33)+127)*sqrt(187)/187]],
 *       [[1,1],[-1,-1],[0,0]]
 * extrema(x2^4-x1^4-x2^8+x1^10,[x1,x2],order=1)
 *    >> [[0,0],[0,-(1/2)^(1/4)],[0,(1/2)^(1/4)],[-(2/5)^(1/6),0],[-(2/5)^(1/6),-(1/2)^(1/4)],
 *        [-(2/5)^(1/6),(1/2)^(1/4)],[(2/5)^(1/6),0],[(2/5)^(1/6),-(1/2)^(1/4)],[(2/5)^(1/6),(1/2)^(1/4)]]
 * extrema(x2^4-x1^4-x2^8+x1^10,[x1,x2])
 *    >> [[],[]]
 * extrema(x2^6+x1^3+4x1+4x2,x1^5+x2^4+x1+x2=0,[x1,x2])
 *    >> [[-0.787457596325,0.758772338924],[-0.784754836317,-1.23363062357]],
 *       [[0.154340184382,-0.155005038065]]
 * extrema(x2^6+x1^3+2x1^2-x2^2+4x1+4x2,x1^5+x2^4+x1+x2=0,[x1,x2])
 *    >> [[-0.662879934158,-1.18571027742],[0,0]],[[0.301887394815,-0.314132376868]]
 * extrema(3x^2-2x*y+y^2-8y,[x,y])
 *    >> [[2,6]],[]
 * extrema(x^3+3x*y^2-15x-12y,[x,y])
 *    >> [[2,1]],[[-2,-1]]
 * extrema(4x*y-x^4-y^4,[x,y])
 *    >> [],[[1,1],[-1,-1]]
 * extrema(x*sin(y),[x,y])
 *    >> [],[]
 * extrema(x^4+y^4,[x,y])
 *    >> [[0,0]],[]
 * extrema(x^3*y-x*y^3,[x,y])  //dog saddle at origin
 *    >> [],[]
 * extrema(x^2+y^2+z^2,x^4+y^4+z^4=1,[x,y,z])
 *    >> [[0,0,1],[0,0,-1]],[]
 * extrema(3x+3y+8z,[x^2+z^2=1,y^2+z^2=1],[x,y,z])
 *    >> [[-3/5,-3/5,-4/5]],[[3/5,3/5,4/5]]
 * extrema(2x^2+y^2,x^4-x^2+y^2=5,[x,y])
 *    >> [[0,-sqrt(5)],[0,sqrt(5)]],
 *       [[-1/2*sqrt(6),-1/2*sqrt(17)],[-1/2*sqrt(6),1/2*sqrt(17)],
 *        [1/2*sqrt(6),-1/2*sqrt(17)],[1/2*sqrt(6),1/2*sqrt(17)]]
 * extrema((3x^4-4x^3-12x^2+18)/(12*(1+4y^2)),[x,y])
 *    >> [[2,0]],[[0,0]]
 * extrema(x-y+z,[x^2+y^2+z^2=1,x+y+2z=1],[x,y,z])
 *    >> [[(-2*sqrt(70)+7)/42,(4*sqrt(70)+7)/42,(-sqrt(70)+14)/42]],
 *       [[(2*sqrt(70)+7)/42,(-4*sqrt(70)+7)/42,(sqrt(70)+14)/42]]
 * extrema(ln(x)+2*ln(y)+3*ln(z)+4*ln(u)+5*ln(v),x+y+z+u+v=1,[x,y,z,u,v])
 *    >> [],[[1/15,2/15,1/5,4/15,1/3]]
 * extrema(x*y*z,-2x^3+15x^2*y+11y^3-24y=0,[x,y,z])
 *    >> [],[]
 * extrema(x+y-exp(x)-exp(y)-exp(x+y),[x,y])
 *    >> [],[[ln(-1/2*(-sqrt(5)+1)),ln(-1/2*(-sqrt(5)+1))]]
 * extrema(x^2*sin(y)-4*x,[x,y])    // has two saddle points
 *    >> [],[]
 * extrema((1+y*sinh(x))/(1+y^2+tanh(x)^2),[x,y])
 *    >> [],[[0,0]]
 * extrema((1+y*sinh(x))/(1+y^2+tanh(x)^2),y=x^2,[x,y])
 *    >> [[1.42217627369,2.02258535346]],[[8.69443642205e-16,7.55932246971e-31]]
 * extrema(x^2*y^2,[x^3*y-2*x^2+3x-2y^2=0],[x=0..inf,y])
 *    >> [[3/2,0]],[[0.893768095046,-0.5789326693514]]
 * extrema(alpha*x^2+beta*y^2+gamma*z^2,[a1*x+a2*y+a3*z=c,b1*x+b2*y+b3*z=d],[x,y,z])
 * assume(a>0):;extrema(sqrt(a+x+y)/(1+y+sqrt(a+x)),[x=0..inf,y=0..inf])
 *    >> [[],[]]    // saddle point: [x=(-a+1/4),y=1]
 * extrema((1+(x+y+1)^2*(19-14x+3x^2-14y+6x*y+3y^2))*(30+(2x-3y)^2*(18-32x+12x^2+48y-36x*y+27y^2)),[x,y])
 *    >> [[[0,-1],[6/5,4/5],[9/5,1/5],[-3/5,-2/5]],[[4/5,1/5]]]   // has four saddle points
 */
gen _extrema(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    vecteur constr;
    int order_size=5; // will not compute the derivatives of order higher than 'order'
    int ngv=gv.size();
    bool approx_hp=false; // use nlpsolve to determine images of homogeneous polynomials on spheres
    if (gv.back()==at_lagrange) {
        order_size=0; // use Lagrange method
        --ngv;
    } else if (gv.back().is_symb_of_sommet(at_equal)) {
        vecteur &v=*gv.back()._SYMBptr->feuille._VECTptr;
        if (v[0]==at_order && is_integer(v[1])) {
            if ((order_size=v[1].val)<1)
                return gensizeerr(gettext("Expected a positive integer."));
            --ngv;
        }
    }
    if (ngv<2 || ngv>3)
        return gensizeerr(gettext("Wrong number of input arguments."));
    // get the variables
    int nv;
    vecteur vars,ineq,initial;
    // parse variables and their ranges, if given
    if ((nv=parse_varlist(gv[ngv-1],vars,ineq,initial))==0)
        return gentypeerr(gettext("Failed to parse variables."));
    if (nv>32)
        return gendimerr(gettext("Too many variables."));
    if (!initial.empty() && int(initial.size())<nv)
        return gendimerr(gettext("Invalid initial point specification."));
    if (ngv==3) {
        // get the constraints
        if (gv[1].type==_VECT)
            constr=*gv[1]._VECTptr;
        else
            constr=vecteur(1,gv[1]);
    }
    if (order_size==0 && constr.empty())
        return gensizeerr(gettext("At least one constraint is required for Lagrange method."));
    for (iterateur it=constr.begin();it!=constr.end();++it) {
        if (it->is_symb_of_sommet(at_equal))
            *it=equal2diff(*it);
    }
    ipdiff::ivectors arrs;
    if (order_size>0 && !constr.empty()) {
        matrice J(jacobian(constr,vars,contextptr));
        if (J.empty())
            return gensizeerr(contextptr);
        if (constr.size()>=vars.size() || _rank(J,contextptr).val<int(constr.size()))
            return gendimerr(gettext("Constraint matrix is not full rank."));
        vars_arrangements(J,arrs,contextptr);
    } else {
        ipdiff::ivector arr(nv);
        for (int i=0;i<nv;++i) {
            arr[i]=i;
        }
        arrs.push_back(arr);
    }
    vecteur tmp_vars(vars.size()),cpts;
    /* iterate through all possible variable arrangements */
    bool undetected=false;
    for (ipdiff::ivectors::const_iterator ait=arrs.begin();ait!=arrs.end();++ait) {
        const ipdiff::ivector &arr=*ait;
        for (ipdiff::ivector::const_iterator it=arr.begin();it!=arr.end();++it) {
            tmp_vars[it-arr.begin()]=vars[*it];
        }
        try {
            find_local_extrema(cpts,gv[0],constr,tmp_vars,arr,ineq,initial,order_size,approx_hp,contextptr);
        } catch (const std::exception &e) {
            restore_logging(contextptr);
            undetected=true;
        }
    }
    if (undetected) {
        string msg="Warning: "+string(gettext("some critical points may have been undetected."));
        if (initial.empty())
            msg+=gettext(" Try setting an initial point for numerical approximation.");
        *logptr(contextptr) << msg << "\n";
    }
    if (order_size==1) { // return the list of critical points
        vecteur cv;
        for (const_iterateur it=cpts.begin();it!=cpts.end();++it) {
            cv.push_back(it->_VECTptr->front());
        }
        return cv;
    }
    // return sequence of minima and maxima in separate lists and report non- or possible extrema
    vecteur minv(0),maxv(0);
    for (const_iterateur it=cpts.begin();it!=cpts.end();++it) {
        gen dispt(nv==1?symb_equal(vars[0],it->_VECTptr->front()):_zip(makesequence(at_equal,vars,it->_VECTptr->front()),contextptr));
        switch(it->_VECTptr->back().val) {
        case _CPCLASS_MIN:
            minv.push_back(it->_VECTptr->front());
            break;
        case _CPCLASS_MAX:
            maxv.push_back(it->_VECTptr->front());
            break;
        case _CPCLASS_SADDLE:
            *logptr(contextptr) << dispt << ": " << gettext("saddle point") << "\n";
            break;
        case _CPCLASS_POSSIBLE_MIN:
        case _CPCLASS_POSSIBLE_MAX:
            *logptr(contextptr) << dispt << ": " << gettext("indeterminate critical point") << "\n";
            break;
        case _CPCLASS_UNDECIDED:
            *logptr(contextptr) << dispt << ": " << gettext("unclassified critical point") << "\n";
            break;
        }
    }
    return makevecteur(minv,maxv);
}
static const char _extrema_s []="extrema";
static define_unary_function_eval (__extrema,&_extrema,_extrema_s);
define_unary_function_ptr5(at_extrema,alias_at_extrema,&__extrema,0,true)

/*
 * Compute the value of expression f(x) (or |f(x)| if 'absolute' is true) for x=a.
 */
gen compf(const gen &f,identificateur &x,gen &a,bool absolute,GIAC_CONTEXT) {
    gen val(subst(f,x,a,false,contextptr));
    return _evalf(absolute?_abs(val,contextptr):val,contextptr);
}

/*
 * find zero of expression f(x) for x in [a,b] using Brent solver
 */
gen find_zero(const gen &f,identificateur &x,gen &a,gen &b,GIAC_CONTEXT) {
    gen I(symb_interval(a,b));
    gen var(symb_equal(x,I));
    gen tmpsol=_fsolve(makesequence(f,var,_BRENT_SOLVER),contextptr);
    if (tmpsol.type==_VECT) {
        vecteur &sol=*tmpsol._VECTptr;
        return sol.empty()?(a+b)/2:sol[0];
    } else return (a+b)/2;
}

/*
 * Find point of maximum/minimum of unimodal expression f(x) in [a,b] using the
 * golden-section search.
 */
gen find_peak(const gen &f,identificateur &x,gen &a_orig,gen &b_orig,GIAC_CONTEXT) {
    gen a(a_orig),b(b_orig);
    gen c(b-(b-a)/GOLDEN_RATIO),d(a+(b-a)/GOLDEN_RATIO);
    while (is_strictly_greater(_abs(c-d,contextptr),epsilon(contextptr),contextptr)) {
        gen fc(compf(f,x,c,true,contextptr)),fd(compf(f,x,d,true,contextptr));
        if (is_strictly_greater(fc,fd,contextptr))
            b=d;
        else
            a=c;
        c=b-(b-a)/GOLDEN_RATIO;
        d=a+(b-a)/GOLDEN_RATIO;
    }
    return (a+b)/2;
}

/*
 * Compute n Chebyshev nodes in [a,b].
 */
vecteur chebyshev_nodes(gen &a,gen &b,int n,GIAC_CONTEXT) {
    vecteur nodes(1,a);
    for (int i=1;i<=n;++i) {
        nodes.push_back(_evalf((a+b)/2+(b-a)*symbolic(at_cos,((2*i-1)*cst_pi/(2*n)))/2,contextptr));
    }
    nodes.push_back(b);
    return *_sort(nodes,contextptr)._VECTptr;
}

/*
 * Implementation of Remez method for minimax polynomial approximation of a
 * continuous bounded function, which is not necessary differentiable in all
 * points of (a,b).
 *
 * Source: http://homepages.rpi.edu/~tasisa/remez.pdf
 *
 * Usage
 * ^^^^^
 *      minimax(expr,var=a..b,n,[opts])
 *
 * Parameters
 * ^^^^^^^^^^
 *      - expr            : expression to be approximated on [a,b]
 *      - var             : variable
 *      - a,b             : bounds of the function domain
 *      - n               : degree of the minimax approximation polynomial
 *      - opts (optional) : sequence of options
 *
 * This function uses 'compf', 'find_zero' and 'find_peak'. It does not use
 * derivatives to determine points of local extrema of error function, but
 * instead implements the golden search algorithm to find these points in the
 * exchange phase of Remez method.
 *
 * The returned polynomial may have degree lower than n, because the latter is
 * decremented during execution of the algorithm if there is no unique solution
 * for coefficients of a nth degree polynomial. After each decrement, the
 * algorithm is restarted. If the degree of resulting polynomial is m<n, it
 * means that polynomial of degree between m and n cannot be obtained by using
 * this implementation.
 *
 * In 'opts' one may specify 'limit=<posint>' which limits the number of
 * iterations. By default, it is unlimited.
 *
 * Be aware that, in some cases, the result with high n may be unsatisfying,
 * producing larger error than the polynomials for smaller n. This happens
 * because of the rounding errors. Nevertheless, a good approximation of an
 * "almost" smooth function can usually be obtained with n less than 30. Highly
 * oscillating functions containing sharp cusps and spikes will probably be
 * approximated poorly.
 *
 * Examples
 * ^^^^^^^^
 * minimax(x*exp(-x),x=0..10,24)
 * minimax(x*sin(x),x=0..10,25)
 * minimax(ln(2+x-sin(x)^2),x=0..2*pi,20)
 * minimax(cos(x^2-x+1),x=-2..2,40)
 * minimax(atan(x),x=-5..5,25)
 * minimax(tanh(sin(9x)),x=-1/2..1/2,40)
 * minimax(abs(x),x=-1..1,20)
 * minimax(abs(x)*sqrt(abs(x)),x=-2..2,15)
 * minimax(min(1/cosh(3*sin(10x)),sin(9x)),x=-0.3..0.4,25)
 * minimax(when(x==0,0,exp(-1/x^2)),x=-1..1,30)
 */
gen _minimax(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    if (gv.size()<3)
        return gensizeerr(contextptr);
    if (!gv[1].is_symb_of_sommet(at_equal) || !is_integer(gv[2]))
        return gentypeerr(contextptr);
    // detect parameters
    vecteur s(*gv[1]._SYMBptr->feuille._VECTptr);
    if (s[0].type!=_IDNT || !s[1].is_symb_of_sommet(at_interval))
        return gentypeerr((contextptr));
    identificateur x(*s[0]._IDNTptr);
    s=*s[1]._SYMBptr->feuille._VECTptr;
    gen a(_evalf(s[0],contextptr)),b(_evalf(s[1],contextptr));
    if (!is_strictly_greater(b,a,contextptr))
        return gentypeerr(contextptr);
    const gen &f=gv[0];
    int n=gv[2].val;
    gen threshold(1.02);  // threshold for stopping criterion
    // detect options
    int limit=0;
    //bool poly=true;
    for (const_iterateur it=gv.begin()+3;it!=gv.end();++it) {
        if (it->is_symb_of_sommet(at_equal)) {
            vecteur &p=*it->_SYMBptr->feuille._VECTptr;
            if (p[0]==at_limit) {
                if (!is_integer(p[1]) || !is_strictly_positive(p[1],contextptr))
                    return gentypeerr(contextptr);
                limit=p[1].val;
            }
        } else if (is_integer(*it)) {
            switch (it->val) {
//          case _FRAC:
//              poly=false;
//              break;
            }
        }
    }
    // create Chebyshev nodes to start with
    vecteur nodes(chebyshev_nodes(a,b,n,contextptr)),sol;
    gen p,best_p,best_emax,emax,emin,tmpsol;
    int iteration_count=0;
    while (true) { // iterate the algorithm
        iteration_count++;
        if (n<1 || (limit>0 && iteration_count>limit))
            break;
        // compute polynomial p
        matrice m;
        vecteur fv;
        for (int i=0;i<n+2;++i) {
            fv.push_back(_evalf(subst(f,x,nodes[i],false,contextptr),contextptr));
            vecteur r;
            for (int j=0;j<n+1;++j) {
                r.push_back(j==0?gen(1):pow(nodes[i],j));
            }
            r.push_back(pow(gen(-1),i));
            m.push_back(r);
        }
        tmpsol=_linsolve(makesequence(_epsilon2zero(m,contextptr),fv),contextptr);
        if (tmpsol.type==_VECT)
            sol=*tmpsol._VECTptr;
        else sol.clear();
        if (sol.empty() || !_lname(sol,contextptr)._VECTptr->empty()) {
            // Solution is not unique, it contains a symbol.
            // Decrease n and start over.
            nodes=chebyshev_nodes(a,b,--n,contextptr);
            continue;
        }
        p=gen(0);
        for (int i=0;i<n+1;++i) {
            p+=sol[i]*pow(x,i);
        }
        // compute the error function and its zeros
        gen e(f-p);
        vecteur zv(1,a);
        for (int i=0;i<n+1;++i) {
            zv.push_back(find_zero(e,x,nodes[i],nodes[i+1],contextptr));
        }
        zv.push_back(b);
        // remez exchange:
        // determine points of local extrema of error function e
        vecteur ev(n+2,0);
        for (int i=0;i<n+2;++i) {
            if (i>0 && i<n+1) {
                nodes[i]=find_peak(e,x,zv[i],zv[i+1],contextptr);
                ev[i]=compf(e,x,nodes[i],true,contextptr);
                continue;
            }
            gen e1(compf(e,x,zv[i],true,contextptr)),e2(compf(e,x,zv[i+1],true,contextptr));
            if (is_greater(e1,e2,contextptr)) {
                nodes[i]=zv[i];
                ev[i]=e1;
            } else {
                nodes[i]=zv[i+1];
                ev[i]=e2;
            }
        }
        // compute minimal and maximal absolute error
        emin=_min(ev,contextptr);
        emax=_max(ev,contextptr);
        if (is_exactly_zero(best_emax) || is_strictly_greater(best_emax,emax,contextptr)) {
            best_p=p;
            best_emax=emax;
        }
        // emin >= E is required to continue, also check
        // if the threshold is reached, i.e. the difference
        // between emax and emin is at least fifty times
        // smaller than emax
        if (is_strictly_greater(sol.back(),emin,contextptr) ||
                is_greater(threshold*emin,emax,contextptr)) {
            break;
        }
    }
    *logptr(contextptr) << gettext("max. absolute error: ") << best_emax << "\n";
    return best_p;
}
static const char _minimax_s []="minimax";
static define_unary_function_eval (__minimax,&_minimax,_minimax_s);
define_unary_function_ptr5(at_minimax,alias_at_minimax,&__minimax,0,true)

/*
 * TPROB CLASS IMPLEMENTATION
 */

/* The constructor.
 * - s: supply vector
 * - d: demand vector
 * - m: a symbol representing infinite cost
 */
tprob::tprob(const vecteur &s,const vecteur &d,const gen &m,GIAC_CONTEXT) {
    eps=epsilon(contextptr)/(s.size()*d.size()+1);
    ctx=contextptr;
    supply=s;
    demand=d;
    M=m;
}

/*
 * North-West-Corner method giving the initial feasible solution to the
 * transportatiom problem with given supply and demand vectors. It handles
 * degeneracy cases (assignment problems, for example, always have degenerate
 * solutions).
 */
void tprob::north_west_corner(matrice &feas) {
    int m=supply.size(),n=demand.size();
    feas=*_matrix(makesequence(m,n,0),ctx)._VECTptr;
    int i=0,j=0;
    while (i<m && j<n) {
        const gen &s=supply[i],&d=demand[j];
        gen u,v;
        for (int k=0;k<i;++k) {
            v+=_epsilon2zero(feas[k][j],ctx);
        }
        for (int k=0;k<j;++k) {
            u+=_epsilon2zero(feas[i][k],ctx);
        }
        gen a=min(s-u,d-v,ctx);
        feas[i]._VECTptr->at(j)=a;
        int k=i+j;
        if (u+a==s)
            ++i;
        if (v+a==d)
            ++j;
        if (i<m && j<n && i+j-k==2) // avoid degeneracy
            feas[i-1]._VECTptr->at(j)=eps;
    }
}

/*
 * Stepping stone path method for determining a closed path "jumping" from one
 * positive element of X to another in the same row or column.
 */
tprob::ipairs tprob::stepping_stone_path(ipairs &path_orig,const matrice &X) {
    ipairs path(path_orig);
    int I=path.back().first,J=path.back().second;
    int m=X.size(),n=X.front()._VECTptr->size();
    if (path.size()>1 && path.front().second==J)
        return path;
    bool hrz=path.size()%2==1;
    for (int i=0;i<(hrz?n:m);++i) {
        int cnt=0;
        for (ipairs::const_iterator it=path.begin();it!=path.end();++it) {
            if ((hrz && it->second==i) || (!hrz && it->first==i))
                ++cnt;
        }
        if (cnt<2 && !is_exactly_zero(X[hrz?I:i][hrz?i:J])) {
            path.push_back(make_pair(hrz?I:i,hrz?i:J));
            ipairs fullpath(stepping_stone_path(path,X));
            if (!fullpath.empty())
                return fullpath;
            path.pop_back();
        }
    }
    return ipairs(0);
}

/*
 * Implementation of the MODI (modified ditribution) method.
 * It handles forbidden routes and degenerate solutions.
 * - P_orig: the cost matrix
 * - X: the solution matrix (must be initialized to a feasible solution)
 * After the function returns, X will contain an optimal solution.
 */
void tprob::modi(const matrice &P_orig,matrice &X) {
    matrice P(P_orig);
    int m=X.size(),n=X.front()._VECTptr->size();
    vecteur u(m),v(n);
    if (M.type==_IDNT)
        P=subst(P,M,_inv(eps,ctx).DOUBLE_val(),false,ctx);
    for (int i=0;i<m;++i)
        u[i]=i==0?gen(0):temp_symb("u",++var_index,ctx);
    for (int j=0;j<n;++j)
        v[j]=temp_symb("v",++var_index,ctx);
    vecteur vars(mergevecteur(vecteur(u.begin()+1,u.end()),v));
    while (true) {
        vecteur eqv;
        for (int i=0;i<m;++i) {
            for (int j=0;j<n;++j) {
                if (!is_exactly_zero(X[i][j]))
                    eqv.push_back(u[i]+v[j]-P[i][j]);
            }
        }
        vecteur sol(*_linsolve(makesequence(eqv,vars),ctx)._VECTptr);
        vecteur U(1,0),V(sol.begin()+m-1,sol.end());
        U=mergevecteur(U,vecteur(sol.begin(),sol.begin()+m-1));
        gen cmin(0);
        bool optimal=true;
        int I,J;
        for (int i=0;i<m;++i) {
            for (int j=0;j<n;++j) {
                if (is_exactly_zero(X[i][j])) {
                    gen c(P[i][j]-U[i]-V[j]);
                    if (is_strictly_greater(cmin,c,ctx)) {
                        cmin=c;
                        optimal=false;
                        I=i;
                        J=j;
                    }
                }
            }
        }
        if (optimal)
            break;
        ipairs path;
        path.push_back(make_pair(I,J));
        path=stepping_stone_path(path,X);
        gen d(X[path.at(1).first][path.at(1).second]);
        for (ipairs::const_iterator it=path.begin()+3;it<path.end();it+=2) {
            d=min(d,X[it->first][it->second],ctx);
        }
        for (int i=0;i<int(path.size());++i) {
            gen &Xij=X[path.at(i).first]._VECTptr->at(path.at(i).second);
            gen x(Xij+(i%2?-d:d));
            bool has_zero=false;
            for (ipairs::const_iterator it=path.begin();it!=path.end();++it) {
                if (is_exactly_zero(X[it->first][it->second])) {
                    has_zero=true;
                    break;
                }
            }
            if ((!is_exactly_zero(x) && is_strictly_greater(fraction(1,2),x,ctx)) ||
                    (is_exactly_zero(x) && has_zero))
                x=eps;
            Xij=x;
        }
    }
    X=*exact(_epsilon2zero(X,ctx),ctx)._VECTptr;
}

void tprob::solve(const matrice &cost_matrix,matrice &sol) {
    north_west_corner(sol);
    modi(cost_matrix,sol);
}

/*
 * END OF TPROB CLASS
 */

/*
 * Function 'tpsolve' solves a transportation problem using MODI method.
 *
 * Usage
 * ^^^^^
 *      tpsolve(supply,demand,cost_matrix)
 *
 * Parameters
 * ^^^^^^^^^^
 *      - supply      : source capacity (vector of m positive integers)
 *      - demand      : destination demand (vector of n positive integers)
 *      - cost_matrix : real matrix C=[c_ij] of type mXn where c_ij is cost of
 *                      transporting an unit from ith source to jth destination
 *                      (a nonnegative number)
 *
 * Supply and demand vectors should contain only positive integers. Cost matrix
 * must be consisted of nonnegative real numbers, which do not have to be
 * integers. There is a possibility of adding a certain symbol to cost matrix,
 * usually M, to indicate the "infinite cost", effectively forbidding the
 * transportation on a certain route. The notation of the symbol may be chosen
 * arbitrarily, but must be used consistently within a single problem.
 *
 * The return value is a sequence of total (minimal) cost and matrix X=[x_ij]
 * of type mXn where x_ij is equal to number of units which have to be shipped
 * from ith source to jth destination, for all i=1,2,..,m and j=1,2,..,n.
 *
 * This function uses 'north_west_corner' to determine initial feasible
 * solution and then applies MODI method to optimize it (function 'modi', which
 * uses 'stepping_stone_path'). Also, it is capable of handling degeneracy of
 * the initial solution and during iterations of MODI method.
 *
 * If the given problem is not balanced, i.e. if supply exceeds demand or vice
 * versa, dummy supply/demand points will be automatically added to the
 * problem, augmenting the cost matrix with zeros. Resulting matrix will not
 * contain dummy point.
 *
 * Examples
 * ^^^^^^^^
 * Balanced transportation problem:
 *  tpsolve([12,17,11],[10,10,10,10],[[500,750,300,450],[650,800,400,600],[400,700,500,550]])
 *      >> 20200,[[0,0,2,10],[0,9,8,0],[10,1,0,0]]
 * Non-balanced transportation problem:
 *  tpsolve([7,10,8,8,9,6],[9,6,12,8,10],[[36,40,32,43,29],[28,27,29,40,38],[34,35,41,29,31],[41,42,35,27,36],[25,28,40,34,38],[31,30,43,38,40]])
 *      >> [[0,0,2,0,5],[0,0,10,0,0],[0,0,0,0,5],[0,0,0,8,0],[9,0,0,0,0],[0,6,0,0,0]]
 * Transportation problem with forbidden routes:
 *  tpsolve([95,70,165,165],[195,150,30,45,75],[[15,M,45,M,0],[12,40,M,M,0],[0,15,25,25,0],[M,0,M,12,0]])
 *      >> [[20,0,0,0,75],[70,0,0,0,0],[105,0,30,30,0],[0,150,0,15,0]]
 * Assignment problem:
 *  tpsolve([1,1,1,1],[1,1,1,1],[[10,12,9,11],[5,10,7,8],[12,14,13,11],[8,15,11,9]])
 *      >> [[0,0,1,0],[1,0,0,0],[0,1,0,0],[0,0,0,1]]
 */
gen _tpsolve(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    if (gv.size()<3)
        return gensizeerr(contextptr);
    if (gv[0].type!=_VECT || gv[1].type!=_VECT ||
            gv[2].type!=_VECT || !ckmatrix(*gv[2]._VECTptr))
        return gentypeerr(contextptr);
    vecteur supply(*gv[0]._VECTptr),demand(*gv[1]._VECTptr);
    if (!is_integer_vecteur(supply,true) || !is_integer_vecteur(demand,true))
        return gentypeerr(gettext("Supply and demand quantites must be integers."));
    matrice P(*gv[2]._VECTptr);
    if (is_strictly_greater(0,_min(_min(P,contextptr),contextptr),contextptr))
        return gensizeerr(gettext("All costs must be non-negative."));
    vecteur sy(*_lname(P,contextptr)._VECTptr);
    if (sy.size()>1)
        return gensizeerr(gettext("At most one symbol is allowed in the cost matrix."));
    int m=supply.size(),n=demand.size();
    if (m!=int(P.size()) || n!=int(P.front()._VECTptr->size()))
        return gendimerr(gettext("Cost matrix dimensions do not match supply and demand."));
    gen M(sy.size()==1 && sy[0].type==_IDNT?sy[0]:0);
    gen ts(_sum(supply,contextptr)),td(_sum(demand,contextptr));
    if (ts!=td) {
        if (is_greater(ts,td,contextptr)) {
            demand.push_back(ts-td);
            P=mtran(P);
            P.push_back(vecteur(m,0));
            P=mtran(P);
        } else {
            supply.push_back(td-ts);
            P.push_back(vecteur(n,0));
        }
    }
    matrice X;
    tprob tp(supply,demand,M,contextptr);
    tp.solve(P,X);
    if (is_strictly_greater(ts,td,contextptr)) {
        X=mtran(X);
        X.pop_back();
        X=mtran(X);
    } else if (is_strictly_greater(td,ts,contextptr))
        X.pop_back();
    gen cost(0);
    for (int i=0;i<m;++i) {
        for (int j=0;j<n;++j) {
            cost+=P[i][j]*X[i][j];
        }
    }
    if (contains(*_lname(cost,contextptr)._VECTptr,M))
        return gensizeerr(gettext("The problem has no feasible solution."));
    return makesequence(cost,X);
}
static const char _tpsolve_s []="tpsolve";
static define_unary_function_eval (__tpsolve,&_tpsolve,_tpsolve_s);
define_unary_function_ptr5(at_tpsolve,alias_at_tpsolve,&__tpsolve,0,true)

gen compute_invdiff(int n,int k,vecteur &xv,vecteur &yv,map<tprob::ipair,gen> &invdiff,GIAC_CONTEXT) {
    tprob::ipair I=make_pair(n,k);
    assert(n<=k);
    gen res(invdiff[I]);
    if (!is_zero(res,contextptr))
        return res;
    if (n==0)
        return invdiff[I]=yv[k];
    if (n==1)
        return invdiff[I]=(xv[k]-xv[0])/(yv[k]-yv[0]);
    gen d1(compute_invdiff(n-1,n-1,xv,yv,invdiff,contextptr));
    gen d2(compute_invdiff(n-1,k,xv,yv,invdiff,contextptr));
    return invdiff[I]=(xv[k]-xv[n-1])/(d2-d1);
}

gen thiele(int k,vecteur &xv,vecteur &yv,identificateur &var,map<tprob::ipair,gen> &invdiff,GIAC_CONTEXT) {
    if (k==int(xv.size()))
        return gen(0);
    gen phi(compute_invdiff(k,k,xv,yv,invdiff,contextptr));
    return (var-xv[k-1])/(phi+thiele(k+1,xv,yv,var,invdiff,contextptr));
}

/*
 * 'thiele' computes rational interpolation for the given list of points using
 * Thiele's method with continued fractions.
 *
 * Source: http://www.astro.ufl.edu/~kallrath/files/pade.pdf (see page 19)
 *
 * Usage
 * ^^^^^
 *      thiele(data,[v])
 * or   thiele(data_x,data_y,[v])
 *
 * Parameters
 * ^^^^^^^^^^
 *      - data      : list of points [[x1,y1],[x2,y2],...,[xn,yn]]
 *      - v         : identifier or a symbolic expression
 *      - data_x    : list of x coordinates [x1,x2,...,xn]
 *      - data_y    : list of y coordinates [y1,y2,...,yn]
 *
 * The return value is an expression R(v), where R is rational interpolant of
 * the given set of points.
 *
 * Note that the interpolant may have singularities in
 * [min(data_x),max(data_x)].
 *
 * Example
 * ^^^^^^^
 * Function f(x)=(1-x^4)*exp(1-x^3) is sampled on interval [-1,2] in 13
 * equidistant points:
 *
 * data_x:=[-1,-0.75,-0.5,-0.25,0,0.25,0.5,0.75,1,1.25,1.5,1.75,2],
 * data_y:=[0.0,2.83341735599,2.88770329586,2.75030303645,2.71828182846,
 *          2.66568510781,2.24894558809,1.21863761951,0.0,-0.555711613283,
 *         -0.377871362418,-0.107135851128,-0.0136782294833]
 *
 * To obtain rational function passing through these points, input:
 *      thiele(data_x,data_y,x)
 * Output:
 *      (-1.55286115659*x^6+5.87298387514*x^5-5.4439152812*x^4+1.68655817708*x^3
 *       -2.40784868317*x^2-7.55954205222*x+9.40462512097)/(x^6-1.24295718965*x^5
 *       -1.33526268624*x^4+4.03629272425*x^3-0.885419321*x^2-2.77913222418*x+3.45976823393)
 */
gen _thiele(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    vecteur xv,yv;
    gen x0=identificateur("x");
    int opt_pos=1;
    if (g.subtype==_SEQ__VECT) {
        const vecteur &gv=*g._VECTptr;
        if (gv[0].type!=_VECT)
            return gentypeerr(contextptr);
        if (ckmatrix(gv[0],false)) {
            matrice m(mtran(*gv[0]._VECTptr));
            if (m.size()!=2)
                return gensizeerr(contextptr);
            xv=*m[0]._VECTptr;
            yv=*m[1]._VECTptr;
        } else {
            if (gv.size()<2)
                return gensizeerr(contextptr);
            if (gv[1].type!=_VECT)
                return gentypeerr(contextptr);
            if (gv[0]._VECTptr->size()!=gv[1]._VECTptr->size())
                return gensizeerr(contextptr);
            xv=*gv[0]._VECTptr;
            yv=*gv[1]._VECTptr;
            opt_pos=2;
        }
        if (gv.size()-opt_pos>=1)
            x0=gv[opt_pos];
    } else {
        if (!ckmatrix(*g._VECTptr,false))
            return gentypeerr(contextptr);
        matrice m(mtran(*g._VECTptr));
        if (m.size()!=2)
            return gensizeerr(contextptr);
        xv=*m[0]._VECTptr;
        yv=*m[1]._VECTptr;
    }
    if (xv.size()<2)
        return gensizeerr(gettext("At least two sample points are required."));
    gen x=temp_symb("x",-1,contextptr);
    map<tprob::ipair,gen> invdiff;
    gen rat(yv[0]+thiele(1,xv,yv,*x._IDNTptr,invdiff,contextptr));
    // detect singularities
    gen den(_denom(rat,contextptr));
    matrice sing;
    if (*_lname(den,contextptr)._VECTptr==vecteur(1,x)) {
        for (int i=0;i<int(xv.size())-1;++i) {
            gen y1(_evalf(subst(den,x,xv[i],false,contextptr),contextptr));
            gen y2(_evalf(subst(den,x,xv[i+1],false,contextptr),contextptr));
            if (is_positive(-y1*y2,contextptr))
                sing.push_back(makevecteur(xv[i],xv[i+1]));
        }
    }
    if (!sing.empty()) {
        *logptr(contextptr) << gettext("Warning") << ": " << gettext("interpolant has singularities in ");
        for (int i=0;i<int(sing.size());++i) {
            *logptr(contextptr) << "(" << sing[i][0] << "," << sing[i][1] << ")";
            if (i<int(sing.size())-1)
                *logptr(contextptr) << (i<int(sing.size())-2?", ":" and ");
        }
        *logptr(contextptr) << "\n";
    }
    if (x0.type==_VECT) {
        vecteur res;
        res.reserve(x0._VECTptr->size());
        for (const_iterateur it=x0._VECTptr->begin();it!=x0._VECTptr->end();++it) {
            res.push_back(simp(subst(rat,x,*it,false,contextptr),contextptr));
        }
        return res;
    }
    return simp(subst(rat,x,x0,false,contextptr),contextptr);
}
static const char _thiele_s []="thiele";
static define_unary_function_eval (__thiele,&_thiele,_thiele_s);
define_unary_function_ptr5(at_thiele,alias_at_thiele,&__thiele,0,true)

void add_identifiers(const gen &source,vecteur &dest,GIAC_CONTEXT) {
    vecteur v(*_lname(source,contextptr)._VECTptr);
    for (const_iterateur it=v.begin();it!=v.end();++it) {
        if (!contains(dest,*it))
            dest.push_back(*it);
    }
    dest=sort_identifiers(dest,contextptr);
}

bool eliminate_equalities(const vector<nlp_lineq> &leqv,const vecteur &vars,vecteur &constr,vecteur &lsol,vecteur &leq_vars,GIAC_CONTEXT) {
    int n=leqv.size();
    graphe G(contextptr,false);
    G.add_nodes(n);
    vector<int> tmp;
    for (int i=0;i<n;++i) {
        const nlp_lineq &nl1=leqv[i];
        for (int j=i+1;j<n;++j) {
            const nlp_lineq &nl2=leqv[j];
            if (intersect(nl1.lvars,nl2.nlvars,tmp)==0 && intersect(nl1.nlvars,nl2.lvars,tmp)==0)
                G.add_edge(i,j);
        }
    }
    G.find_maximal_cliques();
    const graphe::ivectors &mc=G.maximal_cliques();
    if (mc.empty())
        return false;
    vector<pair<pair<int,int>,int> > mca(mc.size());
    for (graphe::ivectors_iter it=mc.begin();it!=mc.end();++it) {
        pair<pair<int,int>,int> &e=mca[it-mc.begin()];
        vector<int> isect;
        for (graphe::ivector_iter jt=it->begin();jt!=it->end();++jt) {
            const nlp_lineq &lineq=leqv[*jt];
            if (jt==it->begin())
                isect=lineq.lvars;
            else {
                intersect(isect,lineq.lvars,tmp);
                isect=tmp;
            }
        }
        e.first.first=it->size();
        e.first.second=isect.size();
        e.second=it-mc.begin();
    }
    sort(mca.begin(),mca.end());
    const graphe::ivector &c=mc[mca.rbegin()->second];
    leq_vars.clear();
    vecteur eqv;
    vector<int> rc;
    for (graphe::ivector_iter it=c.begin();it!=c.end();++it) {
        const nlp_lineq &lineq=leqv[*it];
        eqv.push_back(lineq.e);
        rc.push_back(lineq.index);
        for (vector<int>::const_iterator jt=lineq.lvars.begin();jt!=lineq.lvars.end();++jt) {
            const gen &v=vars[*jt];
            if (!contains(leq_vars,v))
                leq_vars.push_back(v);
        }
    }
    lsol=*_solve(makesequence(eqv,leq_vars),contextptr)._VECTptr;
    if (!lsol.empty()) {
        lsol=*lsol.front()._VECTptr;
        sort(rc.begin(),rc.end());
        for (vector<int>::const_reverse_iterator it=rc.rbegin();it!=rc.rend();++it) {
            constr.erase(constr.begin()+*it);
        }
    }
    return true;
}

bool nlp_subs_fxvars(const nlp_fxvars &fxvars,gen &obj,vecteur &constr,GIAC_CONTEXT) {
    obj=subst(obj,fxvars.names,fxvars.values,false,contextptr);
    for (int i=constr.size();i-->0;) {
        gen &c=constr[i];
        assert(c.type==_SYMB);
        gen d=c._SYMBptr->feuille._VECTptr->front()-c._SYMBptr->feuille._VECTptr->back();
        d=subst(d,fxvars.names,fxvars.values,false,contextptr);
        if (is_zero(d,contextptr))
            constr.erase(constr.begin()+i);
        else {
            if (_evalf(d,contextptr).is_approx() && c.is_symb_of_sommet(at_equal))
                return false;
            c=_eval(symbolic(c._SYMBptr->sommet,makesequence(d,0)),contextptr);
            if (c.is_integer() && c.subtype==_INT_BOOLEAN) {
                if (c.val==0)
                    return false;
                constr.erase(constr.begin()+i);
            }
        }
    }
    return true;
}

/*
 * Optimize obj subject to constr w.r.t. vars with bounds lbv and ubv using COBYLA.
 * Return true iff the obtained solution is feasible.
 */
bool nlp_optimize(const gen &obj_orig,const vecteur &constr_orig,const vecteur &vars_orig,
        const vecteur &lbv_orig,const vecteur &ubv_orig,const vecteur &initp_orig,
        gen &optval,vecteur &sol,double eps,double feas_tol,int nsamp,int maxiter,bool presolve,GIAC_CONTEXT) {
    gen obj(obj_orig);
    vecteur constr=constr_orig,vars=vars_orig,lbv=lbv_orig,ubv=ubv_orig,initp=initp_orig;
    /* presolve to reduce the number of variables */
    stack<nlp_fxvars> fx_subs;
    if (presolve) {
        int pass=0,nvars=vars.size();
        nlp_fxvars fxvars;
        fxvars.n_oldvars=vars.size();
        for (int j=vars.size();j-->0;) { // remove fixed variables
            if (is_zero(ubv[j]-lbv[j],contextptr)) {
                fxvars.names.push_back(vars[j]);
                fxvars.values.push_back((lbv[j]+ubv[j])/2);
                fxvars.indices.push_back(j);
                vars.erase(vars.begin()+j);
                lbv.erase(lbv.begin()+j);
                ubv.erase(ubv.begin()+j);
                if (!initp.empty())
                    initp.erase(initp.begin()+j);
            } else if (is_strictly_greater(lbv[j],ubv[j],contextptr))
                return false; // infeasible
        }
        if (!fxvars.names.empty()) {
            if (!nlp_subs_fxvars(fxvars,obj,constr,contextptr))
                return false;
            fx_subs.push(fxvars);            
        }
        bool changed;
        do { // substitute variables
            changed=false;
            fxvars.indices.clear();
            fxvars.names.clear();
            fxvars.values.clear();
            fxvars.n_oldvars=vars.size();
            vector<nlp_lineq> leqv;
            for (const_iterateur ct=constr.begin();ct!=constr.end();++ct) {
                if (!ct->is_symb_of_sommet(at_equal))
                    continue;
                gen d=exact(ct->_SYMBptr->feuille._VECTptr->front()-ct->_SYMBptr->feuille._VECTptr->back(),contextptr),a,b;
                vecteur lin;
                for (const_iterateur it=vars.begin();it!=vars.end();++it) {
                    if (is_linear_wrt(d,*it,a,b,contextptr) && !is_exactly_zero(a) && is_constant_wrt_vars(a,vars,contextptr))
                        lin.push_back(*it);
                }
                if (lin.empty())
                    continue;
                vecteur v=*_lname(d,contextptr)._VECTptr;
                for (const_iterateur it=lin.begin();it!=lin.end();++it) {
                    v.erase(find(v.begin(),v.end(),*it));
                }
                nlp_lineq lineq;
                lineq.e=d;
                lineq.index=ct-constr.begin();
                for (const_iterateur it=lin.begin();it!=lin.end();++it) {
                    lineq.lvars.push_back(indexof(*it,vars));
                }
                for (const_iterateur it=v.begin();it!=v.end();++it) {
                    lineq.nlvars.push_back(indexof(*it,vars));
                }
                sort(lineq.lvars.begin(),lineq.lvars.end());
                sort(lineq.nlvars.begin(),lineq.nlvars.end());
                leqv.push_back(lineq);
            }
            if (leqv.empty())
                continue;
            vecteur lsol,leq_vars;
            if (!eliminate_equalities(leqv,vars,constr,lsol,leq_vars,contextptr))
                continue;
            if (!lsol.empty()) {
                for (const_iterateur it=lsol.begin();it!=lsol.end();++it) {
                    int i,j=it-lsol.begin();
                    if (is_constant_wrt(*it,leq_vars[j],contextptr)) {
                        fxvars.names.push_back(leq_vars[j]);
                        fxvars.values.push_back(*it);
                        fxvars.indices.push_back(i=indexof(leq_vars[j],vars));
                        if (!is_inf(lbv[i]))
                            constr.push_back(symb_superieur_egal(*it,lbv[i]));
                        if (!is_inf(ubv[i]))
                            constr.push_back(symb_inferieur_egal(*it,ubv[i]));
                    }
                }
                for (int j=vars.size();j-->0;) {
                    if (find(fxvars.indices.begin(),fxvars.indices.end(),j)!=fxvars.indices.end()) {
                        vars.erase(vars.begin()+j);
                        lbv.erase(lbv.begin()+j);
                        ubv.erase(ubv.begin()+j);
                        if (!initp.empty())
                            initp.erase(initp.begin()+j);
                    }
                }
            } else return false;
            if (changed=!fxvars.names.empty()) {
                if (!nlp_subs_fxvars(fxvars,obj,constr,contextptr))
                    return false;
                fx_subs.push(fxvars);
            }
        } while (changed && (++pass>0));
    }
    /* optimize */
    if (!vars.empty()) { // solve the problem using COBYLA
        vecteur initpv;
        if (initp.empty()) { // generate feasible points for multistart
            initpv=gen_feasible_point(obj,constr,vars,lbv,ubv,nsamp,feas_tol,epsilon(contextptr),100,contextptr);
            if (initpv.empty()) {
                initp.resize(vars.size(),0.5);
                initpv.push_back(initp);
            }
        } else initpv.push_back(initp);
        for (int i=vars.size();i-->0;) { // convert variable bounds to inequality constraints
            if (!is_inf(lbv[i]))
                constr.push_back(symb_superieur_egal(vars[i],lbv[i]));
            if (!is_inf(ubv[i]))
                constr.push_back(symb_inferieur_egal(vars[i],ubv[i]));
        }
        gen cobyla_res,cobyla_obj;
        vecteur cobyla_sol;
        optval=plus_inf;
        for (const_iterateur it=initpv.begin();it!=initpv.end();++it) {
            initp=*(it->_VECTptr);
            try {
                if (!is_feasible(constr,vars,initp,feas_tol,contextptr)) {
                    gen tmpinitp=_fMin(makesequence(0,constr,vars,initp,feas_tol,maxiter),contextptr);
                    if (tmpinitp.type==_VECT && !tmpinitp._VECTptr->empty())
                        initp=tmpinitp._VECTptr->front().type==_STRNG?*tmpinitp._VECTptr->at(1)._VECTptr:*tmpinitp._VECTptr;
                }
                if (!is_feasible(constr,vars,initp,feas_tol,contextptr))
                    continue;
                cobyla_res=_fMin(makesequence(obj,constr,vars,initp,eps,maxiter),contextptr);
                if (cobyla_res.type!=_VECT || cobyla_res._VECTptr->empty())
                    gensizeerr(gettext("Failed to optimize, COBYLA returned ")+cobyla_res.print(contextptr));
                cobyla_sol=*cobyla_res._VECTptr;
                if (cobyla_sol.front().type==_STRNG) {
                    cobyla_obj=cobyla_sol.front();
                    cobyla_sol=*cobyla_sol.back()._VECTptr;
                } else cobyla_obj=subst(obj,vars,cobyla_sol,false,contextptr);
                if ((cobyla_obj.type==_STRNG && is_inf(optval)) || is_strictly_greater(optval,cobyla_obj,contextptr)) {
                    optval=cobyla_obj;
                    sol=cobyla_sol;
                }
            } catch (std::runtime_error &err) { // an error raised in COBYLA or user interruption
                *logptr(contextptr) << err.what() << "\n";
                break;
            }
        }
        if (is_inf(optval))
            return false; // failed to find feasible region
    } else { // all variables are fixed in preprocessing
        optval=obj;
        sol=vecteur(0);
    }
    /* postprocess */
    vecteur csol;
    while (!fx_subs.empty()) {
        nlp_fxvars fx=fx_subs.top();
        if (!vars.empty())
            fx.values=subst(fx.values,vars,sol,false,contextptr);
        csol=vecteur(fx.n_oldvars,undef);
        vecteur tmp_vars=vars;
        vars=vecteur(fx.n_oldvars,undef);
        for (int i=fx.indices.size();i-->0;) {
            csol[fx.indices[i]]=fx.values[i];
            vars[fx.indices[i]]=fx.names[i];
        }
        int i=0;
        for (const_iterateur it=sol.begin();it!=sol.end();++it) {
            int j=it-sol.begin();
            while (!is_undef(csol[i])) ++i;
            csol[i]=*it;
            vars[i]=tmp_vars[j];
        }
        fx_subs.pop();
        sol=csol;
    }
    return true;
}

bool nlp_node::find_branch_var(const vecteur &sol,const set<int> &iv_ind,double tol,GIAC_CONTEXT) {
    set<int>::const_iterator it=iv_ind.begin();
    for (;it!=iv_ind.end() && _abs(sol[*it]-_round(sol[*it],contextptr),contextptr).DOUBLE_val()<tol;++it);
    if (it==iv_ind.end()) // solution is integer feasible
        return false;
    branch_var=*it;
    frac_val=sol[*it];
    return true;
}


/*
 * 'nlpsolve' computes an optimum of a nonlinear objective function, subject to
 * nonlinear equality and inequality constraints, using the COBYLA algorithm.
 *
 * Syntax
 * ^^^^^^
 *      nlpsolve(objective,[constr],[bd],[opts])
 *
 * - objective: objective expression
 * - constr: list of equality/inequality constraints
 * - bd: sequence of arguments of type x=a..b or [x,y...]=a..b, where x,y... are problem variables and a<=b are reals
 * - opts: a sequence of one or more of the following:
 *       assume=nlp_nonnegative,nlp_binary,nlp_integer|integer,nlp_nonnegint|nonnegint (2 or more options in a list)
 *       maximize[=true] or nlp_maximize[=true]
 *       nlp_initialpoint=[x1=a,x2=b,...]
 *       nlp_precision=real
 *       nlp_iterationlimit=intg>0
 *       nlp_integervariables=list
 *       nlp_binaryvariables=list
 *       nlp_presolve=true|false
 *       nlp_samples=intg>=0
 *
 * If initial point is not given, it will be automatically generated. The given point does not need to be feasible.
 * If nlp_samples>0 then the feasible region is randomly sampled and the best point is used as the initial one.
 * Note that choosing a good initial point is needed for obtaining a correct solution in some cases.
 *
 * Examples
 * ^^^^^^^^
 * (problems taken from:
 *      www.ai7.uni-bayreuth.de/test_problem_coll.pdf
 *      https://www.maplesoft.com/support/help/maple/view.aspx?path=Optimization%2FNLPSolve)
 *
 * nlpsolve(
 *  (x1-10)^3+(x2-20)^3,
 *  [(x1-5)^2+(x2-5)^2>=100,(x2-5)^2+(x1-6)^2<=82.81],
 *  nlp_initialpoint=[x1=20.1,x2=5.84]) // problem 19, using initial point
 * nlpsolve(sin(x1+x2)+(x1-x2)^2-1.5x1+2.5x2+1,x1=-1.5..4,x2=-3..3) // problem 5
 * nlpsolve(ln(1+x1^2)-x2,[(1+x1^2)^2+x2^2=4]) // problem 7
 * nlpsolve(
 *  x1,[x2>=exp(x1),x3>=exp(x2)],maximize=true,
 *  x1=0..100,x2=0..100,x3=0..10,nlp_initialpoint=[x1=1,x2=1.05,x3=2.9]) // problem 34 (modified)
 * nlpsolve(-x1*x2*x3,[72-x1-2x2-2x3>=0],x1=0..20,x2=0..11,x3=0..42) // problem 36
 * nlpsolve(2-1/120*x1*x2*x3*x4*x5,[x1<=1,x2<=2,x3<=3,x4<=4,x5<=5],assume=nlp_nonnegative) // problem 45
 * nlpsolve(sin(x)/x,x=1..30) // Maple computes wrong result for this example, at least on their web page
 * nlpsolve(x^3+2x*y-2y^2,x=-10..10,y=-10..10,nlp_initialpoint=[x=3,y=4],maximize) // Maple example
 * nlpsolve(w^3*(v-w)^2+(w-x-1)^2+(x-y-2)^2+(y-z-3)^2,[w+x+y+z<=5,3z+2v=3],assume=nlp_nonnegative) // Maple example
 * nlpsolve(sin(x)*Psi(x),x=1..20,nlp_initialpoint=[x=16]) // Maple example, needs an initial point
 */
gen _nlpsolve(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type==_SYMB) // unconstrained minimization without any options and bounds
        return _nlpsolve(makesequence(g,vecteur(0)),contextptr);
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()<2)
        return gentypeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    vecteur constr,vars,initp;
    gen obj=gv.front();
    add_identifiers(obj,vars,contextptr);
    bool maximize=false,has_initp=false,presolve=true,minlp=false,nonneg=false;
    int maxiter=1e5,nsamp=500,wc=0;
    double eps=epsilon(contextptr),int_tol=1e-6,feas_tol=1e-6;
    const_iterateur it=gv.begin();
    /* get the constraints, if any */
    if (gv[1].type==_VECT) {
        constr=*gv[1]._VECTptr;
        add_identifiers(constr,vars,contextptr);
        ++it;
    }
    if (vars.empty()) {
        if (!is_feasible(constr,vars,vecteur(0),feas_tol,contextptr))
            return vecteur(0);
        return change_subtype(makevecteur(obj,vecteur(0)),_LIST__VECT);
    }
    vecteur lbv(vars.size(),minus_inf),ubv(vars.size(),plus_inf),intvars,binvars;
    /* parse options and bounds */
    while (++it!=gv.end()) {
        bool invalid=false;
        if (*it==at_maximize || is_mcint(*it,_NLP_MAXIMIZE)) {
            maximize=true;
        } else if (it->is_symb_of_sommet(at_equal)) {
            const gen &lh=it->_SYMBptr->feuille._VECTptr->front();
            const gen &rh=it->_SYMBptr->feuille._VECTptr->back();
            if (lh==at_assume && rh.is_integer() && (rh.val==_ZINT || rh.subtype==_INT_MAPLECONVERSION)) {
                // parse assumptions
                switch (rh.val) {
                case _NONNEGINT: case _NLP_NONNEGINT:
                    intvars=vars;
                case _NLP_NONNEGATIVE:
                    nonneg=true;
                    break;
                case _NLP_INTEGER: case _ZINT:
                    intvars=vars;
                    break;
                case _NLP_BINARY:
                    binvars=vars;
                    break;
                default:
                    invalid=true;
                    break;
                }
            } else if (lh==at_maximize && rh.is_integer() && rh.subtype==_INT_BOOLEAN) {
                maximize=(bool)rh.val;
            } else if (lh.is_integer() && lh.val==_NLP_INITIALPOINT && rh.type==_VECT) {
                initp.resize(vars.size(),undef);
                vecteur &pnt=*rh._VECTptr;
                const_iterateur jt=pnt.begin();
                for (;jt!=pnt.end();++jt) {
                    gen var;
                    if (jt->is_symb_of_sommet(at_equal) && contains(vars,var=jt->_SYMBptr->feuille._VECTptr->front()))
                        initp.at(indexof(var,vars))=jt->_SYMBptr->feuille._VECTptr->back();
                    else break;
                }
                if (jt!=pnt.end() || contains(initp,undef) || !_lname(_evalf(initp,contextptr),contextptr)._VECTptr->empty())
                    return gensizeerr(gettext("Invalid initial point specification."));
                if (is_zero__VECT(initp,contextptr))
                    return gensizeerr(gettext("Cannot use zero vector as an initial point."));
            } else if (is_mcint(lh,_NLP_ITERATIONLIMIT) && rh.is_integer() && rh.val>0) {
                maxiter=rh.val;
            } else if (is_mcint(lh,_NLP_SAMPLES) && rh.is_integer() && rh.val>=0) {
                nsamp=rh.val;
            } else if (is_mcint(lh,_NLP_PRESOLVE) && rh.is_integer() && rh.subtype==_INT_BOOLEAN) {
                presolve=(bool)rh.val;
            } else if (is_mcint(lh,_NLP_MAXIMIZE) && rh.is_integer() && rh.subtype==_INT_BOOLEAN) {
                maximize=(bool)rh.val;
            } else if (is_mcint(lh,_NLP_PRECISION) && _evalf(rh,contextptr).is_approx() && is_strictly_positive(rh,contextptr)) {
                eps=rh.to_double(contextptr);
            } else if (is_mcint(lh,_NLP_FEAS_TOL) && _evalf(rh,contextptr).is_approx() && is_strictly_positive(rh,contextptr)) {
                feas_tol=rh.to_double(contextptr);
            } else if (is_mcint(lh,_NLP_INT_TOL) && _evalf(rh,contextptr).is_approx() && is_strictly_positive(rh,contextptr)) {
                int_tol=rh.to_double(contextptr);
            } else if (is_mcint(lh,_NLP_INTEGERVARIABLES) && rh.type==_VECT) {
                intvars=*rh._VECTptr;
            } else if (is_mcint(lh,_NLP_BINARYVARIABLES) && rh.type==_VECT) {
                binvars=*rh._VECTptr;
            } else if (rh.is_symb_of_sommet(at_interval)) {
                vecteur vs=lh.type==_VECT?*lh._VECTptr:vecteur(1,lh);
                const gen &lb=rh._SYMBptr->feuille._VECTptr->front();
                const gen &ub=rh._SYMBptr->feuille._VECTptr->back();
                if (is_strictly_greater(lb,ub,contextptr))
                    return gensizeerr(gettext("Infeasible bounds on variable(s) ")+_feuille(vs,contextptr).print(contextptr)+string("."));
                for (const_iterateur vt=vs.begin();vt!=vs.end();++vt) {
                    int j=indexof(*vt,vars);
                    if (j<0)
                        return gensizeerr(gettext("Expected a variable."));
                    lbv[j]=max(lb,lbv[j],contextptr);
                    ubv[j]=min(ub,ubv[j],contextptr);
                }
            } else invalid=true;
        } else invalid=true;
        if (invalid)
            return gensizeerr(gettext("Invalid argument ")+print_INT_(it-gv.begin())+string("."));
    }
    if (maximize)
        obj=-obj;
    if (nonneg) for (iterateur it=lbv.begin();it!=lbv.end();++it) {
        *it=max(0,*it,contextptr);
    }
    // handle integral variables
    intvars=mergevecteur(intvars,binvars);
    minlp=!intvars.empty();
    set<int> intvars_indices;
    for (const_iterateur it=intvars.begin();it!=intvars.end();++it) {
        int j=indexof(*it,vars);
        if (j<0) // specified variable not found among problem variables
            return gensizeerr(gettext("Invalid specification of integer variables."));
        intvars_indices.insert(j);
        if (contains(binvars,*it)) { // tighten bounds on binary variable
            lbv[j]=max(lbv[j],0,contextptr);
            ubv[j]=min(ubv[j],1,contextptr);
        }
        // make bounds integral
        lbv[j]=_ceil(lbv[j],contextptr);
        ubv[j]=_floor(ubv[j],contextptr);
    }
    /* parse constraints, detect infeasible/redundant ones,
     * convert trivial inequalities to variable bounds,
     * convert expr=a..b to expr>=a and expr<=b */
    for (int i=constr.size();i-->0;) {
        gen &c=constr[i],a,b;
        if (c.is_integer() && c.subtype==_INT_BOOLEAN) {
            if (c.val) {
                constr.erase(constr.begin()+i);
                continue;
            } else {
                *logptr(contextptr) << gettext("Error") << ": " << gettext("constraint ") << i
                    << gettext(" evaluated to FALSE, consider changing inequality to equality (e.g. use x^2=0 instead of x^2<=0)\n");
                return gensizeerr(gettext("Infeasible constraint ")+print_INT_(i));
            }
        }
        if (!c.is_symb_of_sommet(at_equal) && which_ineq(c)==0)
            return gensizeerr(gettext("Invalid constraint ")+print_INT_(i));
        if (c.is_symb_of_sommet(at_equal) && c._SYMBptr->feuille._VECTptr->back().is_symb_of_sommet(at_interval)) {
            gen a=c._SYMBptr->feuille._VECTptr->back()._SYMBptr->feuille._VECTptr->front();
            gen b=c._SYMBptr->feuille._VECTptr->back()._SYMBptr->feuille._VECTptr->back();
            gen lh=c._SYMBptr->feuille._VECTptr->front();
            c=symb_inferieur_egal(lh,b);
            constr.insert(constr.begin()+i,symb_superieur_egal(lh,a));
            continue;
        }
        if (!c.is_symb_of_sommet(at_equal)) {
            int dir=which_ineq(c)==1?-1:1;
            gen d=c._SYMBptr->feuille._VECTptr->front()-c._SYMBptr->feuille._VECTptr->back();
            for (const_iterateur it=vars.begin();it!=vars.end();++it) {
                int j=it-vars.begin();
                if (is_linear_wrt(d,*it,a,b,contextptr) && !is_zero(a,contextptr) &&
                        _evalf(a,contextptr).is_approx() && _evalf(b,contextptr).is_approx()) {
                    if (!is_positive(a,contextptr))
                        dir=-dir;
                    if (dir>0)
                        lbv[j]=max(lbv[j],-b/a,contextptr);
                    else ubv[j]=min(ubv[j],-b/a,contextptr);
                    constr.erase(constr.begin()+i);
                    break;
                }
            }
        }
    }
    obj=_evalf(obj,contextptr);
    constr=*_evalf(constr,contextptr)._VECTptr;
    lbv=*_evalf(lbv,contextptr)._VECTptr;
    ubv=*_evalf(ubv,contextptr)._VECTptr;
    initp=*_evalf(initp,contextptr)._VECTptr;
    gen optval,optimum=plus_inf;
    vecteur sol,optsol;
    vector<nlp_node> active_nodes;
    if (!nlp_optimize(obj,constr,vars,lbv,ubv,initp,optval,sol,eps,feas_tol,nsamp,maxiter,presolve,contextptr))
        return vecteur(0); // problem is not feasible
    nlp_node root;
    if (minlp && root.find_branch_var(sol,intvars_indices,int_tol,contextptr)) { // apply branch & bound
        root.lbv=lbv;
        root.ubv=ubv;
        root.opt=optval;
        root.depth=0;
        active_nodes.push_back(root);
        while (!active_nodes.empty()) {
            int k=-1;
            gen best_opt=plus_inf;
            for (vector<nlp_node>::const_iterator it=active_nodes.begin();it!=active_nodes.end();++it) {
                if (is_strictly_greater(best_opt,it->opt,contextptr)) {
                    best_opt=it->opt;
                    k=it-active_nodes.begin();
                }
            }
            nlp_node lb,ub,&p=active_nodes[k];
            int j=p.branch_var;
            lb.lbv=ub.lbv=p.lbv;
            lb.ubv=ub.ubv=p.ubv;
            lb.depth=ub.depth=p.depth+1;
            lb.lbv[j]=max(lb.lbv[j],_ceil(p.frac_val,contextptr),contextptr);
            ub.ubv[j]=min(ub.ubv[j],_floor(p.frac_val,contextptr),contextptr);
            bool optimum_updated=false;
            // solve up-branch relaxation
            if (nlp_optimize(obj,constr,vars,lb.lbv,lb.ubv,initp,lb.opt,sol,eps,feas_tol,nsamp,maxiter,presolve,contextptr)) {
                if (!_evalf(lb.opt,contextptr).is_approx()) {
                    *logptr(contextptr) << gettext("Warning") << ": " << gettext("unable to minimize at given precision") << "\n";
                    lb.opt=subst(obj,vars,sol,false,contextptr);
                }
                if (is_strictly_greater(optimum,lb.opt,contextptr)) {
                    if (lb.find_branch_var(sol,intvars_indices,int_tol,contextptr)) {
                        active_nodes.push_back(lb);
                    } else { // new incumbent found
                        optimum=lb.opt;
                        optsol=sol;
                        optimum_updated=true;
                    }
                }
            }
            // solve down-branch relaxation
            if (nlp_optimize(obj,constr,vars,ub.lbv,ub.ubv,initp,ub.opt,sol,eps,feas_tol,nsamp,maxiter,presolve,contextptr)) {
                if (!_evalf(ub.opt,contextptr).is_approx()) {
                    *logptr(contextptr) << gettext("Warning") << ": " << gettext("unable to minimize at given precision") << "\n";
                    ub.opt=subst(obj,vars,sol,false,contextptr);
                }
                if (is_strictly_greater(optimum,ub.opt,contextptr)) {
                    if (ub.find_branch_var(sol,intvars_indices,int_tol,contextptr)) {
                        active_nodes.push_back(ub);
                    } else { // new incumbent found
                        optimum=ub.opt;
                        optsol=sol;
                        optimum_updated=true;
                    }
                }
            }
            active_nodes.erase(active_nodes.begin()+k); // delete parent node
            if (optimum_updated) { // prune nodes
                for (int i=active_nodes.size();i-->0;) {
                    if (is_greater(active_nodes[i].opt,optimum,contextptr))
                        active_nodes.erase(active_nodes.begin()+i);
                }
            }
        }
        if (is_inf(optimum)) // no integer-feasible solution found
            return vecteur(0);
    } else {
        optsol=sol;
        optimum=optval;
    }
    if (maximize)
        optimum=-optimum;
#if 0
    for (set<int>::const_iterator it=intvars_indices.begin();it!=intvars_indices.end();++it) {
        optsol[*it]=_round(optsol[*it],contextptr);
    }
#endif
    /* return the optimal solution */
    return gen(makevecteur(_epsilon2zero(optimum,contextptr),
        _zip(makesequence(at_equal,vars,_epsilon2zero(optsol,contextptr)),contextptr)),_LIST__VECT);
}
static const char _nlpsolve_s []="nlpsolve";
static define_unary_function_eval (__nlpsolve,&_nlpsolve,_nlpsolve_s);
define_unary_function_ptr5(at_nlpsolve,alias_at_nlpsolve,&__nlpsolve,0,true)

/*
 * Returns the trigonometric polynomial in variable x passing through points
 * with ordinate componets in 'data' and the abscissa components equally spaced between
 * a and b (the first being equal a and the last being equal to b).
 */
gen triginterp(const vecteur &data,const gen &a,const gen &b,const identificateur &x,GIAC_CONTEXT) {
    int n=data.size();
    if (n<2)
        return gensizeerr(contextptr);
    int N=(n%2)==0?n/2:(n-1)/2;
    gen T=(b-a)*fraction(n,n-1),twopi=2*_IDNT_pi(),X;
    matrice cos_coeff=*_matrix(makesequence(N,n,0),contextptr)._VECTptr;
    matrice sin_coeff=*_matrix(makesequence(N,n,0),contextptr)._VECTptr;
    for (int k=0;k<n;++k) {
        X=twopi*(a/T+fraction(k,n));
        for (int j=1;j<=N;++j) {
            cos_coeff[j-1]._VECTptr->at(k)=cos(j*X,contextptr);
            sin_coeff[j-1]._VECTptr->at(k)=sin(j*X,contextptr);
        }
    }
    gen tp=_mean(data,contextptr);
    for (int j=0;j<N;++j) {
        gen c=fraction(((n%2)==0 && j==N-1)?1:2,n);
        gen ak=_evalc(trig2exp(scalarproduct(data,*cos_coeff[j]._VECTptr,contextptr),contextptr),contextptr);
        gen bk=_evalc(trig2exp(scalarproduct(data,*sin_coeff[j]._VECTptr,contextptr),contextptr),contextptr);
        tp+=simp(c*ak,contextptr)*cos(simp((j+1)*twopi/T,contextptr)*x,contextptr);
        tp+=simp(c*bk,contextptr)*sin(simp((j+1)*twopi/T,contextptr)*x,contextptr);
    }
    return tp;
}

gen _triginterp(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &args=*g._VECTptr;
    if (args.size()<2)
        return gensizeerr(contextptr);
    if (args.front().type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &data=*args.front()._VECTptr;
    gen x,ab,a,b,&vararg=args.at(1);
    if (vararg.is_symb_of_sommet(at_equal) &&
               (x=_lhs(vararg,contextptr)).type==_IDNT &&
               (ab=_rhs(vararg,contextptr)).is_symb_of_sommet(at_interval)) {
        a=_lhs(ab,contextptr);
        b=_rhs(ab,contextptr);
    } else if (args.size()==4 && (x=args.back()).type==_IDNT) {
        a=args.at(1);
        b=args.at(2);
    } else return gensizeerr(contextptr);
    gen tp=triginterp(data,a,b,*x._IDNTptr,contextptr);
    if (is_approx(data) || is_approx(a) || is_approx(b))
        tp=_evalf(tp,contextptr);
    return tp;
}
static const char _triginterp_s []="triginterp";
static define_unary_function_eval (__triginterp,&_triginterp,_triginterp_s);
define_unary_function_ptr5(at_triginterp,alias_at_triginterp,&__triginterp,0,true)

/* Compute a rational interpolation of the given points using
 * the method of Floater and Hormann.
 * Usage: ratinterp(Mtrx(M) || Vect(X),[Vect(Y)],[Var(x) || Vect(P)],[d])
 * - If P is given, then the interpolant is evaluated at these points
 * - d is a nonnegative integer not larger than |X| (default: d=|X|)
 */
gen _ratinterp(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    vecteur X,Y;
    gen x=temp_symb("x",-1,contextptr),x0=identificateur("x");
    int d=-1,opts_at=2;
    /* parse input arguments */
    if (g.subtype==_SEQ__VECT) {
        const vecteur &gv=*g._VECTptr;
        if (gv.front().type!=_VECT)
            return gentypeerr(contextptr);
        if (ckmatrix(*gv.front()._VECTptr,false)) {
            const matrice &M=*gv.front()._VECTptr;
            if (M.empty() || M.front()._VECTptr->size()!=2)
                return gensizeerr(contextptr);
            matrice tM=mtran(M);
            X=*tM.front()._VECTptr;
            Y=*tM.back()._VECTptr;
            opts_at=1;
        } else {
            if (gv.size()<2)
                return gensizeerr(contextptr);
            if (gv[1].type!=_VECT)
                return gentypeerr(contextptr);
            X=*gv[0]._VECTptr;
            Y=*gv[1]._VECTptr;
            if (X.empty() || X.size()!=Y.size())
                return gensizeerr(contextptr);
        }
        if (gv.size()-opts_at>=1)
            x0=gv[opts_at];
        if (gv.size()-opts_at>=2) {
            const gen &arg=gv[opts_at+1];
            if (!arg.is_integer() || arg.val<0 || arg.val>=int(X.size()))
                return gensizeerr(contextptr);
            d=arg.val;
        }
    } else {
        if (!ckmatrix(*g._VECTptr,false))
            return gentypeerr(contextptr);
        const matrice &M=*g._VECTptr;
        if (M.empty() || M.front()._VECTptr->size()!=2)
            return gensizeerr(contextptr);
        matrice tM=mtran(M);
        X=*tM.front()._VECTptr;
        Y=*tM.back()._VECTptr;
    }
    int n=X.size()-1;
    if (n<1)
        return gensizeerr(gettext("At least two sample points are required."));
    if (d<0)
        d=std::min(3,n/2);
    /* compute the interpolant corresponding to d in barycentric form */
    vecteur w(n+1,0);
    vecteur D(n+1);
    for (int k=0;k<=n;++k) {
        for (int i=0;i<=n-d;++i) {
            if (k-d<=i && i<=k) {
                gen p(i%2?-1:1);
                for (int j=i;j<=i+d;++j) {
                    if (j!=k)
                        p=p*_inv(X[k]-X[j],contextptr);
                }
                w[k]+=p;
            }
        }
        D[k]=w[k]/(x-X[k]);
    }
    gen N=scalarproduct(D,Y,contextptr),r=N/_sum(D,contextptr);
    if (x0.type==_VECT) {
        vecteur res;
        res.reserve(x0._VECTptr->size());
        for (const_iterateur it=x0._VECTptr->begin();it!=x0._VECTptr->end();++it) {
            res.push_back(simp(subst(r,x,*it,false,contextptr),contextptr));
        }
        return res;
    }
    return simp(subst(r,x,x0,false,contextptr),contextptr);
}
static const char _ratinterp_s []="ratinterp";
static define_unary_function_eval (__ratinterp,&_ratinterp,_ratinterp_s);
define_unary_function_ptr5(at_ratinterp,alias_at_ratinterp,&__ratinterp,0,true)

/* select a good bandwidth for kernel density estimation using a direct plug-in method (DPI),
 * Gaussian kernel is assumed */
double select_bandwidth_dpi(const vector<double> &data,double sd) {
    int n=data.size();
    double g6=1.23044723*sd,s=0,t,t2;
    for (vector<double>::const_iterator it=data.begin();it!=data.end();++it) {
        for (vector<double>::const_iterator jt=it+1;jt!=data.end();++jt) {
            t=(*it-*jt)/g6;
            t2=t*t;
            s+=(2*t2*(t2*(t2-15)+45)-30)*std::exp(-t2/2);
        }
    }
    s-=15*n;
    double g4=g6*std::pow(-(6.0*n)/s,1/7.0);
    s=0;
    for (vector<double>::const_iterator it=data.begin();it!=data.end();++it) {
        for (vector<double>::const_iterator jt=it+1;jt!=data.end();++jt) {
            t=(*it-*jt)/g4;
            t2=t*t;
            s+=(2*t2*(t2-6)+6)*std::exp(-t2/2);
        }
    }
    s+=3*n;
    return std::pow(double(n)/(M_SQRT2*s),0.2)*g4;
}

gen fft_sum(const vecteur &c,const vecteur &k,int M,GIAC_CONTEXT) {
    return _scalar_product(makesequence(c,_mid(makesequence(_convolution(makesequence(c,k),contextptr),M,M),contextptr)),contextptr);
}

/* faster bandwidth DPI selector using binned data and FFT */
double select_bandwidth_dpi_bins(int n,const vecteur &c,double d,double sd,GIAC_CONTEXT) {
    int M=c.size();
    vecteur k(2*M+1);
    double g6=1.23044723*sd,s=0,t,t2;
    for (int i=0;i<=2*M;++i) {
        t=d*double(i-M)/g6;
        t2=t*t;
        k[i]=gen((2*t2*(t2*(t2-15)+45)-30)*std::exp(-t2/2));
    }
    s=_evalf(fft_sum(c,k,M,contextptr),contextptr).DOUBLE_val();
    double g4=g6*std::pow(-(6.0*n)/s,1/7.0);
    for (int i=0;i<=2*M;++i) {
        t=d*double(i-M)/g4;
        t2=t*t;
        k[i]=gen((2*t2*(t2-6)+6)*std::exp(-t2/2));
    }
    s=_evalf(fft_sum(c,k,M,contextptr),contextptr).DOUBLE_val();
    return std::pow(double(n)/(M_SQRT2*s),0.2)*g4;
}

/* kernel density estimation with Gaussian kernel */
gen kernel_density(const vector<double> &data,double bw,double sd,int bins,double a,double b,int interp,const gen &x,GIAC_CONTEXT) {
    int n=data.size();
    double SQRT_2PI=std::sqrt(2.0*M_PI);
    if (bins<=0) { // return density as a sum of exponential functions, usable for up to few hundred samples
        if (bw<=0)
            bw=select_bandwidth_dpi(data,sd);
        double fac=bw*n*SQRT_2PI;
        gen res(0),h(2.0*bw*bw);
        for (vector<double>::const_iterator it=data.begin();it!=data.end();++it) {
            res+=exp(-pow(x-gen(*it),2)/h,contextptr);
        }
        return res/gen(fac);
    }
    /* FFT method, constructs an approximation on [a,b] with the specified number of bins.
     * If interp>0, interpolation of order interp is performed and the density is returned piecewise. */
    assert(b>a && bins>0);
    double d=(b-a)/(bins-1);
    vecteur c(bins,0);
    int index;
    for (vector<double>::const_iterator it=data.begin();it!=data.end();++it) {
        index=(int)((*it-a)/d+0.5);
        if (index>=0 && index<bins) c[index]+=1;
    }
    if (bw<=0) { // select bandwidth
        if (n<=1000)
            bw=select_bandwidth_dpi(data,sd);
        else bw=select_bandwidth_dpi_bins(n,c,d,sd,contextptr);
        *logptr(contextptr) << gettext("selected bandwidth: ") << bw << "\n";
    }
    int L=std::min(bins-1,(int)std::floor(1+4*bw/d));
    vecteur k(2*L+1);
    for (int i=0;i<=2*L;++i) {
        k[i]=gen(1.0/(n*bw*SQRT_2PI)*std::exp(-std::pow(d*double(i-L)/bw,2)/2.0));
    }
    gen res=_mid(makesequence(_convolution(makesequence(c,k),contextptr),L,bins),contextptr);
    if (interp>0) { // interpolate the obtained points
        int pos0=0;
        if (x.type!=_IDNT) {
            double xd=_evalf(x,contextptr).DOUBLE_val();
            if (xd<a || xd>=b || (pos0=std::floor((xd-a)/d))>bins-2)
                return 0;
            if (interp==1) {
                const gen &y1=res._VECTptr->at(pos0),&y2=res._VECTptr->at(pos0+1),x1=a+pos0*d;
                return y1+(x-x1)*(y2-y1)/gen(d);
            }
        }
        vecteur pos(bins);
        for (int i=0;i<bins;++i) pos[i]=a+d*i;
        gen X=x.type==_IDNT?x:temp_symb("X",-1,contextptr);
        vecteur p=*_spline(makesequence(pos,res,X,interp),contextptr)._VECTptr;
        vecteur args(0);
        if (x.type==_IDNT)
            args.reserve(2*bins+1);
        for (int i=0;i<bins;++i) {
            if (x.type==_IDNT) {
                args.push_back(i+1<bins?symb_inferieur_strict(X,pos[i]):symb_inferieur_egal(X,pos[i]));
                args.push_back(i==0?0:p[i-1]);
            } else if (i==pos0) res=simp(subst(p[i],X,x,false,contextptr),contextptr);
            if (i+1<bins && interp>1 && !solve_quiet(p[i],symb_equal(X,symb_interval(pos[i],pos[i+1])),contextptr).empty())
                *logptr(contextptr) << gettext("Warning") << ": " << gettext("interpolated density has negative values in") << " ["
                                    << pos[i] << "," << pos[i+1] << "]\n";
        }
        if (x.type!=_IDNT) return res;
        args.push_back(0);
        res=symbolic(at_piecewise,change_subtype(args,_SEQ__VECT));
        return res;
    }
    return res;
}

bool parse_interval(const gen &feu,double &a,double &b,GIAC_CONTEXT) {
    const vecteur &v=*feu._VECTptr;
    gen l=v.front(),r=v.back();
    if ((l=_evalf(l,contextptr)).type!=_DOUBLE_ || (r=_evalf(r,contextptr)).type!=_DOUBLE_ ||
            !is_strictly_greater(r,l,contextptr))
        return false;
    a=l.DOUBLE_val(); b=r.DOUBLE_val();
    return true;
}

gen _kernel_density(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    gen x=identificateur("x");
    double a=0,b=0,bw=0,sd,d,sx=0,sxsq=0;
    int bins=100,interp=1,method=_KDE_METHOD_LIST,bw_method=_KDE_BW_METHOD_DPI;
    if (g.subtype==_SEQ__VECT) {
        // parse options
        for (const_iterateur it=g._VECTptr->begin()+1;it!=g._VECTptr->end();++it) {
            if (it->is_symb_of_sommet(at_equal)) {
                const gen &opt=it->_SYMBptr->feuille._VECTptr->front();
                const gen &v=it->_SYMBptr->feuille._VECTptr->back();
                if (opt==_KDE_BANDWIDTH) {
                    if (v==at_select)
                        bw_method=_KDE_BW_METHOD_DPI;
                    else if (v==at_gauss || v==at_normal || v==at_normald)
                        bw_method=_KDE_BW_METHOD_ROT;
                    else {
                        gen ev=_evalf(v,contextptr);
                        if (ev.type!=_DOUBLE_ || !is_strictly_positive(ev,contextptr))
                            return gensizeerr(contextptr);
                        bw=ev.DOUBLE_val();
                    }
                } else if (opt==_KDE_BINS) {
                    if (!v.is_integer() || !is_strictly_positive(v,contextptr))
                        return gensizeerr(contextptr);
                    bins=v.val;
                } else if (opt==at_range) {
                    if (v.type==_VECT) {
                        if (v._VECTptr->size()!=2 || !parse_interval(v,a,b,contextptr))
                            return gensizeerr(contextptr);
                    } else if (!v.is_symb_of_sommet(at_interval) ||
                               !parse_interval(v._SYMBptr->feuille,a,b,contextptr))
                        return gensizeerr(contextptr);
                } else if (opt==at_output || opt==at_Output) {
                    if (v==at_exact)
                        method=_KDE_METHOD_EXACT;
                    else if (v==at_piecewise)
                        method=_KDE_METHOD_PIECEWISE;
                    else if (v==_MAPLE_LIST)
                        method=_KDE_METHOD_LIST;
                    else return gensizeerr(contextptr);
                } else if (opt==at_interp) {
                    if (!v.is_integer() || (interp=v.val)<1)
                        return gensizeerr(contextptr);
                } else if (opt==at_spline) {
                    if (!v.is_integer() || (interp=v.val)<1)
                        return gensizeerr(contextptr);
                    method=_KDE_METHOD_PIECEWISE;
                } else if (opt.type==_IDNT) {
                    x=opt;
                    if (!v.is_symb_of_sommet(at_interval) || !parse_interval(v._SYMBptr->feuille,a,b,contextptr))
                        return gensizeerr(contextptr);
                } else if (opt==at_eval) x=v;
                else return gensizeerr(contextptr);
            } else if (it->type==_IDNT) x=*it;
            else if (it->is_symb_of_sommet(at_interval)) {
                if (!parse_interval(it->_SYMBptr->feuille,a,b,contextptr))
                    return gensizeerr(contextptr);
            } else if (*it==at_exact)
                method=_KDE_METHOD_EXACT;
            else if (*it==at_piecewise)
                method=_KDE_METHOD_PIECEWISE;
            else return gensizeerr(contextptr);
        }
    }
    if (x.type!=_IDNT && (_evalf(x,contextptr).type!=_DOUBLE_ || method==_KDE_METHOD_LIST))
        return gensizeerr(contextptr);
    vecteur &data=g.subtype==_SEQ__VECT?*g._VECTptr->front()._VECTptr:*g._VECTptr;
    int n=data.size();
    if (n<2)
        return gensizeerr(contextptr);
    vector<double> ddata(n);
    gen e;
    for (const_iterateur it=data.begin();it!=data.end();++it) {
        if ((e=_evalf(*it,contextptr)).type!=_DOUBLE_)
            return gensizeerr(contextptr);
        d=ddata[it-data.begin()]=e.DOUBLE_val();
        sx+=d;
        sxsq+=d*d;
    }
    sd=std::sqrt(1/double(n-1)*(sxsq-1/double(n)*sx*sx));
    if (bw_method==_KDE_BW_METHOD_ROT) { // Silverman's rule of thumb
        double iqr=_evalf(_quartile3(data,contextptr)-_quartile1(data,contextptr),contextptr).DOUBLE_val();
        bw=1.06*std::min(sd,iqr/1.34)*std::pow(double(data.size()),-0.2);
        *logptr(contextptr) << gettext("selected bandwidth: ") << bw << "\n";
    }
    if (bins>0 && a==0 && b==0) {
        a=_evalf(_min(data,contextptr),contextptr).DOUBLE_val()-3*bw;
        b=_evalf(_max(data,contextptr),contextptr).DOUBLE_val()+3*bw;
    }
    if (method==_KDE_METHOD_EXACT)
        bins=0;
    else if (method==_KDE_METHOD_LIST) {
        if (bins<1)
            return gensizeerr(contextptr);
        interp=0;
    } else if (method==_KDE_METHOD_PIECEWISE) {
        if (bins<1 || interp<1)
            return gensizeerr(contextptr);
    }
    return kernel_density(ddata,bw,sd,bins,a,b,interp,x,contextptr);
}
static const char _kernel_density_s []="kernel_density";
static define_unary_function_eval (__kernel_density,&_kernel_density,_kernel_density_s);
define_unary_function_ptr5(at_kernel_density,alias_at_kernel_density,&__kernel_density,0,true)

static const char _kde_s []="kde";
static define_unary_function_eval (__kde,&_kernel_density,_kde_s);
define_unary_function_ptr5(at_kde,alias_at_kde,&__kde,0,true)

/* maximum likelihood estimation for Weibull distribution */
gen weibull_mle(const vecteur &S,const gen &k0,const gen &eps,GIAC_CONTEXT) {
    gen s0(0),s1(0),s2(0),L(0),t,l,n(S.size());
    for (const_iterateur it=S.begin();it!=S.end();++it) {
        L+=(l=ln(*it,contextptr));
        t=exp(k0*l,contextptr);
        s0+=t; s1+=t*l; s2+=t*sq(l);
    }
    L=L/n;
    gen ik0=_inv(k0,contextptr),k=k0-(s1-s0*(ik0+L))/(s2+s0*sq(ik0)-s1*(ik0+L));
    if (!is_strictly_positive(k,contextptr))
        return undef;
    if (is_greater(_abs(k-k0,contextptr),eps,contextptr))
        return weibull_mle(S,k,eps,contextptr);
    gen lambda=exp(_inv(k,contextptr)*ln(s0/n,contextptr),contextptr);
    return symbolic(at_weibulld,makesequence(k,lambda));
}

/* maximum likelihood estimation for Cauchy distribution */
gen cauchy_mle(const vecteur &S,const gen &x0_init,const gen &gama_init,const gen &eps,GIAC_CONTEXT) {
    matrice m=*_matrix(makesequence(2,2,0),contextptr)._VECTptr;
    gen n(S.size()),d,dsq,den,densq,gsq=sq(gama_init),F(0),G(-n/2);
    gen &Fx0=m.front()._VECTptr->front(),&Fgama=m.front()._VECTptr->back(),
            &Gx0=m.back()._VECTptr->front(),&Ggama=m.back()._VECTptr->back();
    for (const_iterateur it=S.begin();it!=S.end();++it) {
        d=*it-x0_init; dsq=sq(d); den=gsq+dsq; densq=sq(den);
        F+=d/den; G+=gsq/den;
        Fx0+=(dsq-gsq)/densq; Fgama+=d/densq; Ggama+=dsq/densq;
    }
    Gx0=2*gsq*Fgama;
    Fgama=-2*gama_init*Fgama;
    Ggama=2*gama_init*Ggama;
    gen tmpdelta=_linsolve(makesequence(m,makevecteur(-F,-G)),contextptr);
    if (tmpdelta.type==_VECT) {
        vecteur &delta=*tmpdelta._VECTptr;
        if (delta.empty())
            return undef;
        gen x0=x0_init+delta.front(),gama=gama_init+delta.back();
        if (is_greater(_l2norm(delta,contextptr),eps,contextptr))
            return cauchy_mle(S,x0,gama,eps,contextptr);
        return symbolic(at_cauchyd,makesequence(x0,gama));
    }
    return undef;
}

/* fit distribution of the given type to the given data using the method of maximum likelihood */
gen _fitdistr(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->front().type!=_VECT)
        return gensizeerr(contextptr);
    vecteur &S=*g._VECTptr->front()._VECTptr;
    int n=S.size(); // number of samples
    if (n<2)
        return gensizeerr(contextptr);
    const gen &dist=g._VECTptr->back();
    gen N(n);
    /* compute sample mean and variance */
    gen mean(0),var(0),ev;
    for (const_iterateur it=S.begin();it!=S.end();++it) {
        ev=_evalf(*it,contextptr);
        if (ev.type!=_DOUBLE_)
            return gensizeerr(contextptr);
        mean+=ev;
    }
    mean=mean/(N-1);
    for (const_iterateur it=S.begin();it!=S.end();++it) {
        var+=sq(*it-mean);
    }
    var=var/(N-1);
    gen sdev=sqrt(var,contextptr);
    /* fit the parameters of the specified distribution by the method of maximum likelihood */
    if (dist==at_normal || dist==at_normald || dist==at_NORMALD) {
        return symbolic(at_normald,makesequence(mean,sdev));
    } else if (dist==at_poisson || dist==at_POISSON) {
        for (const_iterateur it=S.begin();it!=S.end();++it) {
            if (!it->is_integer() || it->val<0) return gensizeerr(contextptr);
        }
        return symbolic(at_poisson,mean);
    } else if (dist==at_exp || dist==at_EXP || dist==at_exponential || dist==at_exponentiald) {
        for (const_iterateur it=S.begin();it!=S.end();++it) {
            if (!is_strictly_positive(*it,contextptr)) return gensizeerr(contextptr);
        }
        return symbolic(at_exponentiald,_inv(mean,contextptr));
    } else if (dist==at_geometric) {
        for (const_iterateur it=S.begin();it!=S.end();++it) {
            if (!it->is_integer() || it->val<1) return gensizeerr(contextptr);
        }
        return symbolic(at_geometric,_inv(mean,contextptr));
    } else if (dist==at_gammad || dist==at_Gamma) {
        gen slog(0);
        for (const_iterateur it=S.begin();it!=S.end();++it) {
            if (!is_strictly_positive(*it,contextptr)) return gensizeerr(contextptr);
            slog+=ln(*it,contextptr);
        }
        gen a_init=sq(mean)/var,aidn=temp_symb("a",-1,contextptr);
        gen e=ln(aidn,contextptr)-Psi(aidn,contextptr)-ln(mean,contextptr)+slog/N;
        gen a=_solve(makesequence(e,symb_equal(aidn,a_init),_NEWTON_SOLVER),contextptr);
        return symbolic(at_gammad,makesequence(a,a/mean));
    } else if (dist==at_betad || dist==at_Beta) {
        gen slog(0),s1log(0),aidn=temp_symb("a",-1,contextptr),bidn=temp_symb("b",-1,contextptr);
        for (const_iterateur it=S.begin();it!=S.end();++it) {
            if (!is_greater(*it,0,contextptr) || is_strictly_greater(*it,1,contextptr))
                return gensizeerr(contextptr);
            slog+=ln(*it,contextptr); s1log+=ln(1-*it,contextptr);
        }
        gen fac=(var+sq(mean)-mean)/var,a_init=-mean*fac,b_init=(mean-1)*fac;
        gen e1=Psi(aidn,contextptr)-Psi(aidn+bidn,contextptr)-slog/N;
        gen e2=Psi(bidn,contextptr)-Psi(aidn+bidn,contextptr)-s1log/N;
        gen tmpsol=_fsolve(makesequence(makevecteur(e1,e2),makevecteur(aidn,bidn),makevecteur(a_init,b_init),_NEWTONJ_SOLVER),contextptr);
        if (tmpsol.type!=_VECT || tmpsol._VECTptr->size()!=2)
            return gensizeerr(gettext("Unable to compute parameters for Beta distribution."));
        vecteur &sol=*tmpsol._VECTptr;
        return symbolic(at_betad,change_subtype(sol,_SEQ__VECT));
    } else if (dist==at_cauchy || dist==at_cauchyd) {
        gen x0_init=_median(S,contextptr);
        gen gama_init=(_quartile3(S,contextptr)-_quartile1(S,contextptr))/2;
        return cauchy_mle(S,x0_init,gama_init,1e-5,contextptr);
    } else if (dist==at_weibull || dist==at_weibulld) {
        if (is_zero(var)) return gensizeerr(contextptr);
        gen kidn=temp_symb("k",-1,contextptr),slog(0);
        for (const_iterateur it=S.begin();it!=S.end();++it) {
            if (!is_positive(*it,contextptr)) return gensizeerr(contextptr);
            slog+=ln(*it,contextptr);
        }
        gen e=_Gamma(1+gen(2)/kidn,contextptr)/_Gamma(1+gen(1)/kidn,contextptr)-var/sq(mean)-1;
        gen k_init=_fsolve(makesequence(e,symb_equal(kidn,max(1,_inv(var,contextptr),contextptr)),_NEWTON_SOLVER),contextptr);
        return weibull_mle(S,k_init,1e-5,contextptr);
    }
    return gensizeerr(contextptr); // the distribution is not recognized
}
static const char _fitdistr_s []="fitdistr";
static define_unary_function_eval (__fitdistr,&_fitdistr,_fitdistr_s);
define_unary_function_ptr5(at_fitdistr,alias_at_fitdistr,&__fitdistr,0,true)

/* evaluates the function f(x,y,y') in the specified point and returns the result as a floating point number. */
double eval_func(const gen &f,const vecteur &vars,const gen &x,const gen &y,const gen &dy,bool &errflag,GIAC_CONTEXT) {
    gen e=_evalf(subst(f,vars,makevecteur(x,y,dy),false,contextptr),contextptr);
    if (e.type!=_DOUBLE_) {
        errflag=false;
        return 0;
    }
    return e.DOUBLE_val();
}

/* approximate the solution of the following boundary-value problem:
 * y''=f(x,y,y'), a<=x<=b, y(a)=alpha, y(b)=beta.
 * The solution is stored in the lists X, w1 and w2 such that
 * X=[x0=a,x1,x2,..,xN=b] and w1[k]=y(xk), w2[k]=y'(xk) for k=0,1,...,N.
 * Return value: 0 on success, 1 if the maximum number of iterations M is exceeded and 2 on computation failure.
 */
int shooting(const gen &f,const gen &x_idn,const gen &y_idn,const gen &dy_idn,const gen &TK_orig,
             const gen &x1,const gen &x2,const gen &y1,const gen &y2,
             int N,double tol,int M,vecteur &X,vecteur &Y,vecteur &dY,GIAC_CONTEXT) {
    gen dfy=derive(f,y_idn,contextptr),dfdy=derive(f,dy_idn,contextptr);
    vecteur vars=makevecteur(x_idn,y_idn,dy_idn);
    double a=x1.DOUBLE_val(),b=x2.DOUBLE_val(),alpha=y1.DOUBLE_val(),beta=y2.DOUBLE_val();
    double h=(b-a)/N,x,k11,k12,k21,k22,k31,k32,k41,k42,dk11,dk12,dk21,dk22,dk31,dk32,dk41,dk42,u1,u2,fv1,fv2,w1i,w2i;
    double TK=is_undef(TK_orig)?(beta-alpha)/(b-a):TK_orig.DOUBLE_val();
    vector<double> w1(N+1),w2(N+1);
    int k=1;
    dfy=simp(dfy,contextptr);
    dfdy=simp(dfdy,contextptr);
    bool ef=true;
    while (k<=M) {
        w1[0]=alpha;
        w2[0]=TK;
        u1=0;
        u2=1;
        for (int i=0;i<N;++i) {
            x=a+i*h;
            w1i=w1[i];
            w2i=w2[i];
            k11=h*w2i;
            k12=h*eval_func(f,vars,x,w1i,w2i,ef,contextptr);
            k21=h*(w2i+k12/2);
            k22=h*eval_func(f,vars,x+h/2,w1i+k11/2,w2i+k12/2,ef,contextptr);
            k31=h*(w2i+k22/2);
            k32=h*eval_func(f,vars,x+h/2,w1i+k21/2,w2i+k22/2,ef,contextptr);
            k41=h*(w2i+k32);
            k42=h*eval_func(f,vars,x+h,w1i+k31,w2i+k32,ef,contextptr);
            w1[i+1]=w1i+(k11+2*k21+2*k31+k41)/6;
            w2[i+1]=w2i+(k12+2*k22+2*k32+k42)/6;
            dk11=h*u2;
            dk12=h*(eval_func(dfy,vars,x,w1i,w2i,ef,contextptr)*u1+
                    eval_func(dfdy,vars,x,w1i,w2i,ef,contextptr)*u2);
            dk21=h*(u2+dk12/2);
            fv1=eval_func(dfy,vars,x+h/2,w1i,w2i,ef,contextptr);
            fv2=eval_func(dfdy,vars,x+h/2,w1i,w2i,ef,contextptr);
            dk22=h*(fv1*(u1+dk11/2)+fv2*(u2+dk12/2));
            dk31=h*(u2+dk22/2);
            dk32=h*(fv1*(u1+dk21/2)+fv2*(u2+dk22/2));
            dk41=h*(u2+dk32);
            dk42=h*(eval_func(dfy,vars,x+h,w1i,w2i,ef,contextptr)*(u1+dk31)+
                    eval_func(dfdy,vars,x+h,w1i,w2i,ef,contextptr)*(u2+dk32));
            u1+=(dk11+2*dk21+2*dk31+dk41)/6;
            u2+=(dk12+2*dk22+2*dk32+dk42)/6;
            if (!ef) return 2;
        }
        if (std::abs(w1[N]-beta)<=tol) {
            X.resize(N+1);
            Y.resize(N+1);
            dY.resize(N+1);
            for (int i=0;i<=N;++i) {
                X[i]=a+i*h;
                Y[i]=w1[i];
                dY[i]=w2[i];
            }
            return 0; // success
        }
        TK-=(w1[N]-beta)/u1;
        ++k;
    }
    return 1; // max number of iterations is exceeded
}

/* the finite-difference method as an slower but more stable alternative to shooting method */
int finitediff(const gen &f,const gen &x_idn,const gen &y_idn,const gen &dy_idn,const gen &x1,const gen &x2,
               const gen &y1,const gen &y2,int N,double tol,int M,vecteur &X,vecteur &Y,GIAC_CONTEXT) {
    gen dfy=derive(f,y_idn,contextptr),dfdy=derive(f,dy_idn,contextptr);
    vecteur vars=makevecteur(x_idn,y_idn,dy_idn);
    double a=x1.DOUBLE_val(),b=x2.DOUBLE_val(),alpha=y1.DOUBLE_val(),beta=y2.DOUBLE_val();
    double h=(b-a)/(N+1),fac=(beta-alpha)/(b-a)*h,x,t;
    vector<double> W(N+2,alpha),A(N+1),B(N+1),C(N+1),D(N+1),U(N+1),L(N+1),Z(N+1);
    vecteur V(N+1);
    W[N+1]=beta;
    for (int i=1;i<=N;++i) W[i]+=i*fac;
    int k=1;
    bool ef=true;
    dfy=simp(dfy,contextptr);
    dfdy=simp(dfdy,contextptr);
    while (k<=M) {
        x=a+h;
        t=(W[2]-alpha)/(2*h);
        A[1]=2+h*h*eval_func(dfy,vars,x,W[1],t,ef,contextptr);
        B[1]=h*eval_func(dfdy,vars,x,W[1],t,ef,contextptr)/2-1;
        D[1]=W[2]+alpha-2*W[1]-h*h*eval_func(f,vars,x,W[1],t,ef,contextptr);
        for (int i=2;i<N;++i) {
            x=a+i*h;
            t=(W[i+1]-W[i-1])/(2*h);
            A[i]=2+h*h*eval_func(dfy,vars,x,W[i],t,ef,contextptr);
            B[i]=h*eval_func(dfdy,vars,x,W[i],t,ef,contextptr)/2-1;
            C[i]=-h*eval_func(dfdy,vars,x,W[i],t,ef,contextptr)/2-1;
            D[i]=W[i+1]+W[i-1]-2*W[i]-h*h*eval_func(f,vars,x,W[i],t,ef,contextptr);
        }
        x=b-h;
        t=(beta-W[N-1])/(2*h);
        A[N]=2+h*h*eval_func(dfy,vars,x,W[N],t,ef,contextptr);
        C[N]=-h*eval_func(dfdy,vars,x,W[N],t,ef,contextptr)/2-1;
        D[N]=W[N-1]+beta-2*W[N]-h*h*eval_func(f,vars,x,W[N],t,ef,contextptr);
        if (!ef) return 2;
        L[1]=A[1];
        U[1]=B[1]/A[1];
        Z[1]=D[1]/L[1];
        for (int i=2;i<N;++i) {
            L[i]=A[i]-C[i]*U[i-1];
            U[i]=B[i]/L[i];
            Z[i]=(D[i]-C[i]*Z[i-1])/L[i];
        }
        L[N]=A[N]-C[N]*U[N-1];
        Z[N]=(D[N]-C[N]*Z[N-1])/L[N];
        V[N]=Z[N];
        W[N]+=Z[N];
        for (int i=N;i-->1;) {
            V[i]=gen(Z[i])-gen(U[i])*V[i+1];
            W[i]+=Z[i]-U[i]*V[i+1].DOUBLE_val();
        }
        if (is_greater(tol,_l2norm(V,contextptr),contextptr)) {
            X.resize(N+2);
            Y.resize(N+2);
            for (int i=0;i<=N+1;++i) {
                X[i]=a+i*h;
                Y[i]=W[i];
            }
            return 0; // success
        }
        ++k;
    }
    return 1; // maximum number of iterations is exceeded
}

gen _bvpsolve(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    gen tk(undef);
    int maxiter=RAND_MAX;
    if (gv.size()<3 || gv[1].type!=_VECT || gv[2].type!=_VECT ||
            gv[1]._VECTptr->size()!=2 || (gv[2]._VECTptr->size()!=2 && gv[2]._VECTptr->size()!=3))
        return gensizeerr(contextptr);
    const gen &f=gv.front(),&t=gv[1]._VECTptr->front(),&y=gv[1]._VECTptr->back(),
            y1=_evalf(gv[2]._VECTptr->at(0),contextptr),y2=_evalf(gv[2]._VECTptr->at(1),contextptr);
    if (gv[2]._VECTptr->size()==3 && (tk=_evalf(gv[2]._VECTptr->at(2),contextptr)).type!=_DOUBLE_)
        return gensizeerr(contextptr);
    if (y.type!=_IDNT || !t.is_symb_of_sommet(at_equal) ||
            t._SYMBptr->feuille._VECTptr->front().type!=_IDNT ||
            !t._SYMBptr->feuille._VECTptr->back().is_symb_of_sommet(at_interval))
        return gensizeerr(contextptr);
    const gen &x=t._SYMBptr->feuille._VECTptr->front();
    const vecteur &rng=*t._SYMBptr->feuille._VECTptr->back()._SYMBptr->feuille._VECTptr;
    gen x1=_evalf(rng.front(),contextptr),x2=_evalf(rng.back(),contextptr);
    if (x1.type!=_DOUBLE_ || x2.type!=_DOUBLE_ || y1.type!=_DOUBLE_ || y2.type!=_DOUBLE_ ||
            !is_strictly_greater(x2,x1,contextptr))
        return gensizeerr(contextptr);
    int N=100;
    /* parse options */
    int output_type=_BVP_LIST;
    for (const_iterateur it=gv.begin()+3;it!=gv.end();++it) {
        if (it->is_symb_of_sommet(at_equal)) {
            const gen &lh=it->_SYMBptr->feuille._VECTptr->front();
            const gen &rh=it->_SYMBptr->feuille._VECTptr->back();
            if (lh==at_output || lh==at_Output) {
                if (rh==_MAPLE_LIST)
                    output_type=_BVP_LIST;
                else if (rh==at_derive)
                    output_type=_BVP_DIFF;
                else if (rh==at_piecewise)
                    output_type=_BVP_PIECEWISE;
                else if (rh==at_spline)
                    output_type=_BVP_SPLINE;
                else return gensizeerr(contextptr);
            } else if (lh==at_limit) {
                if (!rh.is_integer() || (maxiter=rh.val)<1)
                    return gensizeerr(contextptr);
            } else return gensizeerr(contextptr);
        } else if (it->is_integer()) {
            if ((N=it->val)<2)
                return gensizeerr(contextptr);
        } else return gensizeerr(contextptr);
    }
    gen dy=temp_symb("dy",-1,contextptr);
    gen F=subst(f,derive(symb_of(y,x),x,contextptr),dy,false,contextptr);
    F=subst(F,symb_of(y,x),y,false,contextptr);
    F=subst(F,symbolic(at_derive,y),dy,false,contextptr);
    vecteur X,Y,dY;
    double tol=_epsilon(change_subtype(vecteur(0),_SEQ__VECT),contextptr).to_double(contextptr);
    int ec=shooting(F,x,y,dy,tk,x1,x2,y1,y2,N,tol,maxiter,X,Y,dY,contextptr);
    if (ec==1) {
        *logptr(contextptr) << gettext("Error") << ": " << gettext("maximum number of iterations exceeded") << "\n";
        return undef;
    }
    if (ec==2) {
        *logptr(contextptr) << gettext("Error") << ": " << gettext("the shooting method failed to converge");
        if (is_undef(tk))
            *logptr(contextptr) << ", " << gettext("try to set an initial guess for ") << y << "'(" << x1 << ")";
        *logptr(contextptr) << "\n";
        if (N>=3 && (output_type==_BVP_LIST || output_type==_BVP_PIECEWISE)) {
            *logptr(contextptr) << gettext("Trying the finite-difference method instead") << "\n";
            ec=finitediff(F,x,y,dy,x1,x2,y1,y2,N-1,tol,maxiter,X,Y,contextptr);
            if (ec==2) {
                *logptr(contextptr) << gettext("Error") << ": " << gettext("failed to converge") << "\n";
                return undef;
            }
            if (ec==1) {
                *logptr(contextptr) << gettext("Error") << ": " << gettext("maximum number of iterations exceeded") << "\n";
                return undef;
            }
        } else return undef;
    }
    vecteur res,coeff;
    matrice m=*_matrix(makesequence(4,4,0),contextptr)._VECTptr;
    m[0]._VECTptr->at(3)=m[1]._VECTptr->at(3)=m[2]._VECTptr->at(2)=m[3]._VECTptr->at(2)=1;
    switch (output_type) {
    case _BVP_LIST:
        res.reserve(N+1);
        for (int i=0;i<=N;++i)
            res.push_back(makevecteur(X[i],Y[i]));
        break;
    case _BVP_DIFF:
        res.reserve(N+1);
        for (int i=0;i<=N;++i)
            res.push_back(makevecteur(X[i],Y[i],dY[i]));
        break;
    case _BVP_PIECEWISE:
    case _BVP_SPLINE:
        res.reserve(2*(N+1)+1);
        for (int i=0;i<=N;++i) {
            res.push_back(i<N?symb_inferieur_strict(x,X[i]):symb_inferieur_egal(x,X[i]));
            if (i==0)
                res.push_back(0);
            else if (output_type==_BVP_PIECEWISE)
                res.push_back(Y[i-1]+(x-X[i-1])*(Y[i]-Y[i-1])/(X[i]-X[i-1]));
            else {
                m[0]._VECTptr->at(0)=pow(X[i-1],3);
                m[0]._VECTptr->at(1)=pow(X[i-1],2);
                m[0]._VECTptr->at(2)=X[i-1];
                m[1]._VECTptr->at(0)=pow(X[i],3);
                m[1]._VECTptr->at(1)=pow(X[i],2);
                m[1]._VECTptr->at(2)=X[i];
                m[2]._VECTptr->at(0)=3*m[0][1];
                m[2]._VECTptr->at(1)=2*X[i-1];
                m[3]._VECTptr->at(0)=3*m[1][1];
                m[3]._VECTptr->at(1)=2*X[i];
                coeff=*_linsolve(makesequence(m,makevecteur(Y[i-1],Y[i],dY[i-1],dY[i])),contextptr)._VECTptr;
                res.push_back(pow(x,3)*coeff[0]+pow(x,2)*coeff[1]+x*coeff[2]+coeff[3]);
            }
        }
        res.push_back(0);
        return symbolic(at_piecewise,change_subtype(res,_SEQ__VECT));
    default:
        break;
    }
    return res;
}
static const char _bvpsolve_s []="bvpsolve";
static define_unary_function_eval (__bvpsolve,&_bvpsolve,_bvpsolve_s);
define_unary_function_ptr5(at_bvpsolve,alias_at_bvpsolve,&__bvpsolve,_QUOTE_ARGUMENTS,true)

gen strip_constants(const gen &g,GIAC_CONTEXT) {
    if (g.is_symb_of_sommet(at_neg))
        return g._SYMBptr->feuille;
    if (g.is_symb_of_sommet(at_prod) && g._SYMBptr->feuille.type==_VECT) {
        const vecteur &feu=*g._SYMBptr->feuille._VECTptr;
        gen ret(1);
        for (const_iterateur it=feu.begin();it!=feu.end();++it) {
            if (_evalf(*it,contextptr).is_approx())
                continue;
            ret=ret*strip_constants(*it,contextptr);
        }
        return ret;
    }
    if (g.is_symb_of_sommet(at_inv))
        return _inv(strip_constants(g._SYMBptr->feuille,contextptr),contextptr);
    return g;
}

/* return the expression for conjugate points */
gen _conjugate_equation(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    if (gv.size()!=5)
        return gensizeerr(contextptr);
    const gen &y0=gv[0],&parm=gv[1],&pvals=gv[2],&t=gv[3],&a=gv[4];
    if (y0.type!=_SYMB || t.type!=_IDNT || parm.type!=_VECT || parm._VECTptr->size()!=2 ||
            pvals.type!=_VECT || pvals._VECTptr->size()!=2 ||
            _evalf(a,contextptr).type!=_DOUBLE_)
        return gensizeerr(contextptr);
    const gen &alpha=parm._VECTptr->front(),&beta=parm._VECTptr->back();
    if (alpha.type!=_IDNT || beta.type!=_IDNT)
        return gensizeerr(contextptr);
    gen y1=derive(y0,alpha,contextptr),y2=derive(y0,beta,contextptr);
    gen ret=_collect(simp(subst(y1*subst(y2,t,a,false,contextptr)-y2*subst(y1,t,a,false,contextptr),
                                     parm,pvals,false,contextptr),contextptr),contextptr);
    return strip_constants(ret,contextptr);
}
static const char _conjugate_equation_s []="conjugate_equation";
static define_unary_function_eval (__conjugate_equation,&_conjugate_equation,_conjugate_equation_s);
define_unary_function_ptr5(at_conjugate_equation,alias_at_conjugate_equation,&__conjugate_equation,0,true)

static int cnst_count=0;

/* return the (list of) Euler-Lagrange equation(s) for functional L(u,du,t) */
gen _euler_lagrange(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    gen L,t=identificateur("x");
    vecteur u=makevecteur(identificateur("y"));
    if (g.type!=_VECT) {
        L=g;
        if (L.type==_IDNT)
            L=eval(L,contextptr);
        if (!contains(lidnt(L),t))
            t=t__IDNT_e;
    } else {
        if (g.subtype!=_SEQ__VECT)
            return gensizeerr(contextptr);
        const vecteur &gv=*g._VECTptr;
        L=gv.front();
        if (gv.size()>1) {
            if (gv.size()==2 && gv[1].is_symb_of_sommet(at_of)) {
                u.front()=gv[1]._SYMBptr->feuille._VECTptr->front();
                t=gv[1]._SYMBptr->feuille._VECTptr->back();
            } else if (gv.size()==2 && gv[1].type==_VECT && !gv[1]._VECTptr->empty()) {
                u.clear();
                t=undef;
                for (const_iterateur it=gv[1]._VECTptr->begin();it!=gv[1]._VECTptr->end();++it) {
                    if (!it->is_symb_of_sommet(at_of))
                        return gensizeerr(contextptr);
                    u.push_back(it->_SYMBptr->feuille._VECTptr->front());
                    if (is_undef(t))
                        t=it->_SYMBptr->feuille._VECTptr->back();
                    else if (t!=it->_SYMBptr->feuille._VECTptr->back())
                        return gensizeerr(contextptr);
                }
            } else t=gv[1];
            if (t.type!=_IDNT)
                return gensizeerr(contextptr);
        }
        if (gv.size()>2) {
            if (gv[2].type==_IDNT)
                u.front()=gv[2];
            else if (gv[2].type==_VECT)
                u=*gv[2]._VECTptr;
            else return gensizeerr(contextptr);
        }
    }
    L=idnteval(L,contextptr);
    int n=u.size();
    vecteur du(n),Du(n),Dut(n),DU(n),D2U(n),d2u(n),ut(n);
    for (int i=0;i<n;++i) {
        if (u[i].type!=_IDNT)
            return gensizeerr(contextptr);
        ut[i]=symb_of(u[i],t);
        du[i]=temp_symb("du",i,contextptr);
        d2u[i]=temp_symb("d2u",i,contextptr);
        Du[i]=symbolic(at_derive,u[i]);
        Dut[i]=symb_of(Du[i],t);
        DU[i]=derive(ut[i],t,contextptr);
        D2U[i]=derive(ut[i],t,2,contextptr);
    }
    L=subst(L,Dut,du,false,contextptr);
    L=subst(L,Du,du,false,contextptr);
    L=subst(L,DU,du,false,contextptr);
    L=subst(L,ut,u,false,contextptr);
    vecteur ret;
    if (n==1 && !depend(L,*t._IDNTptr)) {
        ret.push_back(symb_equal(simp(du[0]*derive(L,du[0],contextptr)-L,contextptr),
                identificateur("K_"+print_INT_(cnst_count++))));
    }
    for (int i=0;i<n;++i) {
        if (!depend(L,*u[i]._IDNTptr)) {
            ret.push_back(symb_equal(simp(derive(L,du[i],contextptr),contextptr),
                                     identificateur("K_"+print_INT_(cnst_count++))));
        } else {
            gen eq=derive(L,u[i],contextptr);
            eq-=derive(subst(derive(L,du[i],contextptr),makevecteur(du[i],u[i]),
                             makevecteur(DU[i],ut[i]),false,contextptr),t,contextptr);
            eq=subst(eq,makevecteur(DU[i],D2U[i]),makevecteur(du[i],d2u[i]),false,contextptr);
            eq=symb_equal(simp(subst(eq,ut[i],u[i],false,contextptr),contextptr),0);
            vecteur sol;
            if (depend(eq,*d2u[i]._IDNTptr) && (sol=solve_quiet(eq,d2u[i],contextptr)).empty())
                eq=symb_equal(d2u[i],simp(sol.front(),contextptr));
            ret.push_back(eq);
        }
    }
    ret=subst(ret,ut,u,false,contextptr);
    gen tmprs=radsimp(ret,contextptr);
    if (tmprs.type==_VECT && tmprs._VECTptr->size()==ret.size())
        ret=*tmprs._VECTptr;
    ret=subst(subst(ret,u,ut,false,contextptr),mergevecteur(du,d2u),
                    mergevecteur(DU,D2U),false,contextptr);
    return ret.size()==1?ret.front():ret;
}
static const char _euler_lagrange_s []="euler_lagrange";
static define_unary_function_eval_quoted (__euler_lagrange,&_euler_lagrange,_euler_lagrange_s);
define_unary_function_ptr5(at_euler_lagrange,alias_at_euler_lagrange,&__euler_lagrange,_QUOTE_ARGUMENTS,true)

gen parse_functional(const gen &L,const gen &t,const gen &y,const gen &dy,GIAC_CONTEXT) {
    assert(t.type==_IDNT && y.type==_IDNT && dy.type==_IDNT);
    gen ret=subst(L,symb_of(symbolic(at_derive,y),t),dy,false,contextptr);
    ret=subst(ret,symbolic(at_derive,y),dy,false,contextptr);
    ret=subst(ret,derive(symb_of(y,t),t,contextptr),dy,false,contextptr);
    ret=subst(ret,symb_of(y,t),y,false,contextptr);
    return ret;
}

/* return the Jacobi equation(s) for the functional L(y,y',a<=t<=b) and stationary function Y,
 * use h as unknown with h(a)=0 */
gen _jacobi_equation(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    gen L,t,y,Y,h,a;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gensizeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    L=gv.front();
    if (gv.size()==5) {
        if (!gv[1].is_symb_of_sommet(at_of))
            return gensizeerr(contextptr);
        y=gv[1]._SYMBptr->feuille._VECTptr->front();
        t=gv[1]._SYMBptr->feuille._VECTptr->back();
        Y=gv[2]; h=gv[3]; a=gv[4];
    } else if (gv.size()==6) {
        t=gv[1]; y=gv[2]; Y=gv[3]; h=gv[4]; a=gv[5];
    } else return gensizeerr(contextptr);
    if (t.type!=_IDNT || h.type!=_IDNT || y.type!=_IDNT || _evalf(a,contextptr).type!=_DOUBLE_)
        return gensizeerr(contextptr);
    L=idnteval(L,contextptr);
    gen dy=temp_symb("dy",-1,contextptr);
    L=parse_functional(L,t,y,dy,contextptr);
    gen dY=derive(Y,t,contextptr);
    gen Ldydy=subst(simp(derive(L,dy,2,contextptr),contextptr),
                    makevecteur(y,dy),makevecteur(Y,dY),false,contextptr);
    gen Lyy=subst(simp(derive(L,y,2,contextptr),contextptr),
                  makevecteur(y,dy),makevecteur(Y,dY),false,contextptr);
    gen Lydy=subst(simp(derive(derive(L,y,contextptr),dy,contextptr),contextptr),
                   makevecteur(y,dy),makevecteur(Y,dY),false,contextptr);
    gen d=simp(Lyy-derive(Lydy,t,contextptr),contextptr);
    if (is_zero(Ldydy))
        return change_subtype(is_zero(d),_INT_BOOLEAN);
    gen jeq=simp(derive(Ldydy,t,contextptr),contextptr)*derive(symb_of(h,t),t,contextptr);
    jeq+=Ldydy*derive(symb_of(h,t),t,2,contextptr)-d*symb_of(h,t);
    jeq=symb_equal(jeq,0);
    gen sol=_deSolve(makesequence(makevecteur(jeq,symb_equal(symb_of(h,a),0)),makevecteur(t,h)),contextptr);
    if (sol.type==_VECT && sol._VECTptr->size()==1 && sol._VECTptr->front().type==_STRNG)
        return jeq;
    return makesequence(jeq,sol.is_symb_of_sommet(at_prod) || sol.is_symb_of_sommet(at_neg)?simp(sol,contextptr):sol);
}
static const char _jacobi_equation_s []="jacobi_equation";
static define_unary_function_eval_quoted (__jacobi_equation,&_jacobi_equation,_jacobi_equation_s);
define_unary_function_ptr5(at_jacobi_equation,alias_at_jacobi_equation,&__jacobi_equation,_QUOTE_ARGUMENTS,true)

gen makevars(const gen &e,const gen &t,const vecteur &depvars,const vecteur &diffvars,GIAC_CONTEXT) {
    if (e.is_symb_of_sommet(at_of) && e._SYMBptr->feuille._VECTptr->back()==t) {
        const gen &u=e._SYMBptr->feuille._VECTptr->front();
        for (const_iterateur it=depvars.begin();it!=depvars.end();++it) {
            if (*it==u) return u;
            if (u==symbolic(at_derive,*it)) return diffvars[it-depvars.begin()];
        }
    } else if (e.is_symb_of_sommet(at_derive)) {
        const gen &feu=e._SYMBptr->feuille;
        if (feu.type!=_VECT || (feu._VECTptr->size()==2 && feu._VECTptr->at(1)==t)) {
            gen f=makevars(feu.type==_VECT?feu._VECTptr->front():feu,t,depvars,diffvars,contextptr);
            for (const_iterateur it=depvars.begin();it!=depvars.end();++it) {
                if (*it==f) return diffvars[it-depvars.begin()];
            }
        }
    } else if (e.type==_SYMB) {
        const gen &feu=e._SYMBptr->feuille;
        if (feu.type==_VECT) {
            vecteur nf;
            nf.reserve(feu._VECTptr->size());
            for (const_iterateur it=feu._VECTptr->begin();it!=feu._VECTptr->end();++it) {
                nf.push_back(makevars(*it,t,depvars,diffvars,contextptr));
            }
            return symbolic(e._SYMBptr->sommet,change_subtype(nf,_SEQ__VECT));
        }
        return symbolic(e._SYMBptr->sommet,makevars(feu,t,depvars,diffvars,contextptr));
    }
    return _eval(e,contextptr);
}

gen apply_sign(const gen &g,const gen &simp,GIAC_CONTEXT) {
    gen e=_sign(g,contextptr);
    if (!e.is_symb_of_sommet(at_sign))
        return e;
    gen arg=_apply(makesequence(simp,vecteur(1,e._SYMBptr->feuille)),contextptr)._VECTptr->front();
    arg=_factor(arg,contextptr);
    if (is_zero(simp(arg-g,contextptr)))
        return e;
    return apply_sign(arg,simp,contextptr);
}

/* return the sign of the expression, simplified */
gen determine_sign(const gen &e_orig,const gen &simp,GIAC_CONTEXT) {
    gen e=_apply(makesequence(simp,vecteur(1,e_orig)),contextptr)._VECTptr->front();
    if (e.type==_SYMB)
        return apply_sign(_factor(e,contextptr),simp,contextptr);
    return _sign(e,contextptr);
}

gen strip_sign(const gen &g) {
    if (g.is_symb_of_sommet(at_neg))
        return -strip_sign(g._SYMBptr->feuille);
    if (g.is_symb_of_sommet(at_sign))
        return g._SYMBptr->feuille;
    if (g.is_symb_of_sommet(at_prod) && g._SYMBptr->feuille.type==_VECT) {
        gen ret(1);
        const vecteur &v=*g._SYMBptr->feuille._VECTptr;
        for (const_iterateur it=v.begin();it!=v.end();++it)
            ret=ret*strip_sign(*it);
        return ret;
    }
    return g;
}

/* return the condition(s) under which the given function is convex */
gen _convex(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    vecteur vars;
    gen f=gv.front(),t(undef),TRUE=change_subtype(1,_INT_BOOLEAN),FALSE=change_subtype(0,_INT_BOOLEAN);
    if (gv.size()<2)
        return gensizeerr(contextptr);
    f=idnteval(f,contextptr);
    if (gv[1].type==_VECT)
        vars=*gv[1]._VECTptr;
    else vars.push_back(gv[1]);
    if (vars.empty())
        return gensizeerr(contextptr);
    gen simp_func=at_simplify;
    if (gv.size()>2) {
        gen lh,rh;
        if (!gv[2].is_symb_of_sommet(at_equal) || (lh=gv[2]._SYMBptr->feuille._VECTptr->front())!=at_simplify ||
                !(rh=gv[2]._SYMBptr->feuille._VECTptr->back()).is_integer() || rh.subtype!=_INT_BOOLEAN)
            return gensizeerr(contextptr);
        if (rh.val==0)
            simp_func=at_ratnormal;
    }
    vecteur fvars,depvars;
    for (iterateur it=vars.begin();it!=vars.end();++it) {
        if (it->is_symb_of_sommet(at_of)) {
            const gen &u=it->_SYMBptr->feuille._VECTptr->front();
            const gen &v=it->_SYMBptr->feuille._VECTptr->back();
            if (v.type!=_IDNT || u.type!=_IDNT)
                return gensizeerr(contextptr);
            if (is_undef(t))
                t=v;
            else if (t!=v)
                return gensizeerr(contextptr);
            if (!contains(depvars,u))
                depvars.push_back(u);
        } else if (it->type==_IDNT) {
            if (!contains(fvars,*it))
                    fvars.push_back(*it);
        } else return gensizeerr(contextptr);
    }
    vecteur diffvars,diffs;
    int cnt=0;
    for (const_iterateur it=depvars.begin();it!=depvars.end();++it) {
        diffvars.push_back(temp_symb("tmp",++cnt,contextptr));
        diffs.push_back(derive(symb_of(*it,t),t,contextptr));
    }
    gen F=is_undef(t)?_eval(f,contextptr):makevars(f,t,depvars,diffvars,contextptr);
    vecteur allvars=mergevecteur(fvars,mergevecteur(depvars,diffvars));
    if (allvars.size()==1) { // univariate function
        gen e=determine_sign(derive(F,allvars.front(),2,contextptr),simp_func,contextptr);
        if (is_one(e))
            return TRUE;
        if (is_minus_one(e))
            return FALSE;
        e=_apply(makesequence(simp_func,vecteur(1,strip_sign(e))),contextptr)._VECTptr->front();
        return eval(e.is_symb_of_sommet(at_neg)?symb_inferieur_egal(e._SYMBptr->feuille,0):symb_superieur_egal(e,0),contextptr);
    }
    // multivariate case, compute the eigenvalues of the Hessian
    int n=allvars.size(),N=1<<n;
    matrice H=*_hessian(makesequence(F,allvars),contextptr)._VECTptr;
    vecteur cond;
    cond.reserve(N);
    for (int i=1;i<N;++i) {
        matrice m_tmp,m;
        for (int j=0;j<n;++j) {
            if (((int)std::pow(2,j) & i)==0) continue;
            m_tmp.push_back(H[j]);
        }
        m_tmp=mtran(m_tmp);
        for (int j=0;j<n;++j) {
            if (((int)std::pow(2,j) & i)==0) continue;
            m.push_back(m_tmp[j]);
        }
        cond.push_back(determine_sign(_det(m,contextptr),simp_func,contextptr));
    }
    vecteur res;
    for (const_iterateur it=cond.begin();it!=cond.end();++it) {
        if (is_minus_one(*it))
            return FALSE;
        if (is_one(*it) || is_zero(*it))
            continue;
        res.push_back(strip_sign(*it));
    }
    if (res.empty())
        return TRUE;
    for (int i=0;i<int(res.size());++i) {
        for (int j=res.size();j-->i+1;) {
            if (is_zero(simp(res[j]-res[i],contextptr)))
                res.erase(res.begin()+j);
        }
    }
    for (int i=res.size();i-->0;) {
        if (res[i].is_integer() && res[i].subtype==_INT_BOOLEAN) {
            if (res[i].val==1)
                res.erase(res.begin()+i);
            else if (res[i].val==0)
                return FALSE;
        }
        gen &r=res[i];
        r=_apply(makesequence(simp_func,vecteur(1,r)),contextptr)._VECTptr->front();
        r=symb_superieur_egal(subst(r,diffvars,diffs,false,contextptr),0);
    }
    if (res.empty())
        return TRUE;
    gen simb=_lname(res,contextptr);
    if (simb.type==_VECT && simb._VECTptr->size()==1) {
        vecteur s=solve_quiet(res,simb._VECTptr->front(),contextptr);
        if (!s.empty())
            res=s;
    }
    return eval(res,contextptr);
}
static const char _convex_s []="convex";
static define_unary_function_eval_quoted (__convex,&_convex,_convex_s);
define_unary_function_ptr5(at_convex,alias_at_convex,&__convex,_QUOTE_ARGUMENTS,true)

gen _numdiff(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    if (gv.size()<3)
        return gensizeerr(gettext("Too few arguments."));
    if (gv[0].type!=_VECT || gv[1].type!=_VECT)
        return gentypeerr(contextptr);
    if (gv[0]._VECTptr->size()!=gv[1]._VECTptr->size() || gv[0]._VECTptr->empty())
        return gensizeerr(gettext("Badly sized X and Y."));
    // avoid numerical instabilities by using exact arithmetic
    vecteur X=*exact(gv[0],contextptr)._VECTptr;
    vecteur Y=*exact(gv[1],contextptr)._VECTptr;
    gen x0=exact(gv[2],contextptr);
    int M=1,N=X.size()-1;
    vecteur rest(1,M);
    if (gv.size()>=4) {
        rest=vecteur(gv.begin()+3,gv.end());
        for (const_iterateur it=rest.begin();it!=rest.end();++it) {
            if (!it->is_integer() || it->val<0)
                return gentypeerr(contextptr);
        }
        M=_max(rest,contextptr).val;
    }
    vecteur res(rest.size());
    // Fornberg's algorithm
    vecteur del((N+1)*(N+1)*(M+1),0);
    del[0]=1;
    gen c1(1),c2,c3;
    for (int n=1;n<=N;++n) {
        c2=1;
        int m_max=n<M?n:M;
        for (int ni=0;ni<n;++ni) {
            c3=X[n]-X[ni];
            c2=c2*c3;
            for (int m=0;m<=m_max;++m) {
                del[(n*(N+1)+ni)*(M+1)+m]=((X[n]-x0)*del[((n-1)*(N+1)+ni)*(M+1)+m]-
                m*del[((n-1)*(N+1)+ni)*(M+1)+(m==0?m:m-1)])/c3;
            }
        }
        for (int m=0;m<=m_max;++m) {
            del[(n*(N+1)+n)*(M+1)+m]=c1/c2*(m*del[((n-1)*(N+1)+n-1)*(M+1)+(m==0?m:m-1)]-
            (X[n-1]-x0)*del[((n-1)*(N+1)+n-1)*(M+1)+m]);
        }
        c1=c2;
    }
    // compute approximations of the derivatives
    for (const_iterateur it=rest.begin();it!=rest.end();++it) {
        gen d(0);
        for (int ni=0;ni<=N;++ni) {
            d+=del[(N*(N+1)+ni)*(M+1)+it->val]*Y[ni];
        }
        res[it-rest.begin()]=d;
    }
    gen result=res.size()==1?res.front():res;
    if (g.is_approx() && _lname(result,contextptr)._VECTptr->empty())
        result=_evalf(result,contextptr);
    return result;
}
static const char _numdiff_s []="numdiff";
static define_unary_function_eval (__numdiff,&_numdiff,_numdiff_s);
define_unary_function_ptr5(at_numdiff,alias_at_numdiff,&__numdiff,0,true)

bool parse_equations(const gen &eq,vecteur &eqv) {
    if (eq.type==_VECT) {
        for (const_iterateur it=eq._VECTptr->begin();it!=eq._VECTptr->end();++it) {
            if (!parse_equations(*it,eqv))
                return false;
        }
    } else {
        gen e;
        if (eq.is_symb_of_sommet(at_equal)) {
            const gen &lhs=eq._SYMBptr->feuille._VECTptr->front();
            const gen &rhs=eq._SYMBptr->feuille._VECTptr->back();
            e=lhs-rhs;
        } else e=eq;
        if (e.type!=_SYMB)
            return false;
        eqv.push_back(e);
    }
    return true;
}

static int integer_placeholder_count=-1;
static int positive_integer_placeholder_count=-1;

gen make_integer_placeholder(const string &base,GIAC_CONTEXT) {
    if (integer_placeholder_count<0)
        integer_placeholder_count=array_start(contextptr);
    gen ph=identificateur(string("_")+base+print_INT_(integer_placeholder_count++));
    return ph;
}

gen make_positive_integer_placeholder(const string &base,GIAC_CONTEXT) {
    if (positive_integer_placeholder_count<0)
        positive_integer_placeholder_count=array_start(contextptr);
    gen ph=identificateur(string("_")+base+print_INT_(positive_integer_placeholder_count++));
    return ph;
}

/* return true iff a is divisible by b */
bool is_divisible(const gen &a,const gen &b,GIAC_CONTEXT) {
    assert(a.is_integer() && b.is_integer());
    return is_zero(_irem(makesequence(a,b),contextptr));
}

/* if g is a homogeneous polynomial in vars of degree d>0, return d, else return 0 */
int is_homogeneous_poly(const gen &g_orig,const vecteur &vars,gen_map *coef,GIAC_CONTEXT) {
    gen g=expand(g_orig,contextptr);
    if (is_exactly_zero(g))
        return 0;
    vecteur cf(1,g),deg(1,vecteur(0));
    for (const_iterateur it=vars.begin();it!=vars.end();++it) {
        vecteur cf_next,deg_next;
        for (const_iterateur jt=cf.begin();jt!=cf.end();++jt) {
            vecteur d=*deg[jt-cf.begin()]._VECTptr;
            vecteur cfx=*_coeff(makesequence(*jt,*it),contextptr)._VECTptr;
            for (const_iterateur kt=cfx.begin();kt!=cfx.end();++kt) {
                if (is_exactly_zero(*kt))
                    continue;
                cf_next.push_back(*kt);
                d.push_back(cfx.end()-kt-1);
                deg_next.push_back(d);
                d.pop_back();
            }
        }
        cf=cf_next;
        deg=deg_next;
    }
    if (!is_constant_wrt_vars(cf,vars,contextptr))
        return 0;
    assert(!deg.empty() && ckmatrix(deg,false));
    vecteur sum_deg=*_sum(mtran(deg),contextptr)._VECTptr;
    assert(is_integer_vecteur(sum_deg,true));
    gen d=sum_deg.front();
    for (const_iterateur it=sum_deg.begin()+1;it!=sum_deg.end();++it) {
        if (!is_exactly_zero(*it-d))
            return 0;
    }
    if (coef!=NULL) {
        int n=cf.size();
        for (int i=0;i<n;++i) {
            (*coef)[deg[i]]=cf[i];
        }
    }
    return d.val;
}

/* return the root of the square part of g */
gen square_part(const gen &g,GIAC_CONTEXT) {
    assert(g.is_integer());
    vecteur f=*_ifactors(g,contextptr)._VECTptr;
    int n=f.size()/2;
    gen ret(1);
    for (int i=0;i<n;++i) {
        gen p=f[2*i],r=f[2*i+1];
        assert(r.is_integer());
        for (int j=0;j<r.val/2;++j)
            ret=ret*p;
    }
    return ret;
}

/* return true iff g = a*x^2 + b*x*y + c*y^2 + d*x + e*y + f */
bool is_binary_quadratic_wrt(const gen &g,const gen &x,const gen &y,
                             gen &a,gen &b,gen &c,gen &d,gen &e,gen &f,GIAC_CONTEXT) {
    gen A,B,C,D,E,F;
    if (!is_quadratic_wrt(g,x,A,B,C,contextptr)) {
        A=0;
        if (!is_linear_wrt(g,x,B,C,contextptr) || is_exactly_zero(B))
            return false;
    } else if (!is_constant_wrt(A,y,contextptr))
        return false;
    a=A;
    if (!is_exactly_zero(B)) {
        if (!is_linear_wrt(B,y,D,E,contextptr))
            return false;
        b=D;
        d=E;
    } else b=d=0;
    if (!is_exactly_zero(C)) {
        if (is_constant_wrt(C,y,contextptr)) {
            c=e=0;
            f=C;
        } else if (is_linear_wrt(C,y,D,E,contextptr)) {
            c=0;
            e=D;
            f=E;
        } else if (is_quadratic_wrt(C,y,D,E,F,contextptr)) {
            c=D;
            e=E;
            f=F;
        } else return false;
    } else c=e=f=0;
    return true;
}

/* convert rationals in coef to integers by multiplying them with
 * lcm of the denominators and subsequently dividing by gcd */
bool integralize(const vecteur &coef,vecteur &coef_int,GIAC_CONTEXT) {
    if (coef.empty())
        return true;
    for (const_iterateur it=coef.begin()+1;it!=coef.end();++it) {
        if (!is_rational(*it))
            return false;
    }
    coef_int=multvecteur(_lcm(_apply(makesequence(at_denom,coef),contextptr),contextptr),coef);
    assert(is_integer_vecteur(coef_int,true));
    coef_int=divvecteur(coef_int,_gcd(coef_int,contextptr));
    return is_integer_vecteur(coef_int,true);
}

/* Rosser's algorithm for an integer solution of a1 x1 + a2 x2 + ... + an xn = b,
 * return true iff it exists and in that case write it to sol
 * (source: Esmaeili et al. 2001) */
bool rosser(const vecteur &a_orig,const gen &b,vecteur &sol,GIAC_CONTEXT) {
    int n=a_orig.size();
    vecteur a=a_orig,ind(n);
    for (int i=0;i<n;++i) {
        if (!is_positive(a[i],contextptr)) {
            a[i]=-a[i];
            ind[i]=-(i+1);
        } else ind[i]=i+1;
    }
    matrice z=mtran(*_revlist(_sort(_zip(makesequence(a,ind),contextptr),contextptr),contextptr)._VECTptr);
    a=*z.front()._VECTptr;
    vector<int> iv=vecteur_2_vector_int(*z.back()._VECTptr);
    matrice C=*_idn(n,contextptr)._VECTptr;
    C=mtran(mergevecteur(matrice(1,a),C));
    while (!is_zero(C[1][0])) {
        gen f=_floor(fraction(C[0][0],C[1][0]),contextptr);
        C[0]=subvecteur(*C[0]._VECTptr,multvecteur(f,*C[1]._VECTptr));
        for (int i=0;i<n-1;++i) {
            if (!is_greater(C[i][0],C[i+1][0],contextptr)) {
                gen tmp=C[i+1];
                C[i+1]=C[i];
                C[i]=tmp;
            } else break;
        }
    }
    vecteur u(C[0]._VECTptr->begin()+1,C[0]._VECTptr->end());
    gen q=b/C[0][0];
    if (!q.is_integer())
        return false;
    vecteur x=multvecteur(q,u);
    sol.resize(n);
    for (int i=0;i<n;++i) {
        int j=iv[i];
        if (j<0)
            sol[-j-1]=-x[i];
        else sol[j-1]=x[i];
    }
    return true;
}

/* solve system of linear diophantine equations AX=b with integer coefficients
 * using the method of Esmaeili et al. (Numer. Math 90: 101-115, 2001),
 * return true iff there exists a solution (q is arbitrary integer vector) */
bool ABS_diophantine(const matrice &A,const vecteur &b,const vecteur &q,vecteur &sol,GIAC_CONTEXT) {
    assert(!A.empty());
    int m=A.size(),n=A.front()._VECTptr->size();
    assert(m<=n);
    vecteur x(n,0),z(n);
    matrice H=*_idn(n,contextptr)._VECTptr;
    for (int i=0;i<m;++i) {
        const vecteur &a=*A[i]._VECTptr;
        gen tau=scalarproduct(a,x,contextptr)-b[i];
        vecteur s=multmatvecteur(H,a);
        if (is_zero__VECT(s,contextptr)) {
            if (is_zero(tau))
                continue;
            return false;
        }
        gen delta=_gcd(s,contextptr),alpha;
        if (!(alpha=tau/delta).is_integer())
            return false;
        assert(rosser(s,delta,z,contextptr));
        vecteur p=multmatvecteur(mtran(H),z);
        x=subvecteur(x,multvecteur(alpha,p));
        H=subvecteur(H,divvecteur(mmult(mtran(matrice(1,s)),matrice(1,multvecteurmat(z,H))),delta));
    }
    H=mtran(H);
    sol=addvecteur(x,multmatvecteur(H,q));
    return true;
}

/* return true iff list of equations eqv is equivalent to linear system A*x=b */
bool is_linear_system(const vecteur &eqv,const vecteur &x,matrice &A,vecteur &b,GIAC_CONTEXT) {
    A.clear();
    b.clear();
    int n=x.size(),m=eqv.size(),i,j;
    A.reserve(m);
    b.resize(m);
    gen p,q;
    vecteur c;
    for (const_iterateur it=eqv.begin();it!=eqv.end();++it) {
        i=it-eqv.begin();
        vecteur eq(n);
        gen e=*it;
        for (const_iterateur jt=x.begin();jt!=x.end();++jt) {
            j=jt-x.begin();
            if (is_constant_wrt(e,*jt,contextptr))
                eq[j]=0;
            else if (is_linear_wrt(e,*jt,p,q,contextptr) && is_rational(p)) {
                eq[j]=p;
                e=q;
            } else return false;
        }
        if (!is_rational(e))
            return false;
        eq.push_back(-e);
        assert(integralize(eq,c,contextptr));
        b[i]=c.back();
        c.pop_back();
        A.push_back(c);
    }
    return true;
}

/* return +-1 if g is Pell equation x^2-d*y^2=+-1
 * for positive integer d, otherwise return 0 */
int is_pell_equation(const gen &g,const gen &x,const gen &y,gen &d,GIAC_CONTEXT) {
    gen a,b,c,f;
    if (!is_quadratic_wrt(g,x,a,b,c,contextptr) || !is_one(a) || !is_exactly_zero(b))
        return 0;
    if (!is_quadratic_wrt(c,y,a,b,f,contextptr) || !is_exactly_zero(b) || !is_one(_abs(f,contextptr)))
        return 0;
    d=-a;
    if (!d.is_integer() || !is_strictly_positive(d,contextptr) || is_perfect_square(d))
        return 0;
    return is_positive(f,contextptr)?-1:1;
}

/* solve Pell equation x^2-d*y^2=+-1, using the algorithm of Alexandre Junod (2015) */
vecteur solve_pell_equation(const gen &d,bool rhs_positive,int nsols,GIAC_CONTEXT) {
    if (is_perfect_square(d))
        return vecteur(0);
    int maxiter=32767,i=-1;
    gen a0,b0,c0,a1,b1,c1,a,b,c,q;
    /* initialize */
    a0=0; b0=1; c0=d;
    a=1; b=0; c=1;
    vecteur sols;
    while (++i<maxiter) {
        if (i>0) {
            /* found a solution */
            if (rhs_positive) {
                if (is_divisible(2*a,c,contextptr))
                    sols.push_back(makevecteur(2*a*a/c+gen((i+1)%2!=0?-1:1),2*a*b/c));
            } else if (i%2!=0 && is_one(c))
                sols.push_back(makevecteur(a,b));
            if (nsols==int(sols.size()))
                break;
        }
        a1=a; b1=b; c1=c;
        q=_floor((sqrt(d-c0*c,contextptr)+sqrt(d,contextptr))/c,contextptr);
        a=q*a+a0;
        b=q*b+b0;
        c=2*q*sqrt(d-c0*c,contextptr)+c0-q*q*c;
        a0=a1; b0=b1; c0=c1;
    }
    if (i>=maxiter)
        *logptr(contextptr) << gettext("Warning") << ": " << gettext("maximum number of iterations exceeded") << "\n";
    return sols;
}

bool is_negative_pell_insoluble(const gen &d,const gen &u1,GIAC_CONTEXT) {
    if (is_one(_is_prime(d,contextptr)))
        return !is_divisible(d-1,4,contextptr);
    if (is_divisible(d-1,4,contextptr) || is_divisible(d-2,4,contextptr))
        return !is_divisible(u1+1,2*d,contextptr);
    return false;
}

gen pell_fundamental(const gen &d,bool rhs_positive,GIAC_CONTEXT) {
    if (!rhs_positive) {
        vecteur fs=solve_pell_equation(d,true,1,contextptr);
        if (fs.empty())
            return undef;
        if (is_negative_pell_insoluble(d,fs.front(),contextptr))
            return vecteur(0);
    }
    vecteur sols=solve_pell_equation(d,rhs_positive,1,contextptr);
    if (sols.empty())
        return undef;
    return sols.front();
}

gen pell_general(const gen &d,bool rhs_positive,const gen &ph,bool output_components,GIAC_CONTEXT) {
    gen fund=pell_fundamental(d,rhs_positive,contextptr);
    if (is_undef(fund) || (fund.type==_VECT && fund._VECTptr->empty()))
        return fund;
    gen n=is_undef(ph)?make_positive_integer_placeholder("N",contextptr):ph;
    const gen &u1=fund._VECTptr->front(),&v1=fund._VECTptr->back();
    gen sqr_d=sqrt(d,contextptr),f1,f2;
    if (rhs_positive) {
        f1=pow(u1+v1*sqr_d,n,contextptr); f2=pow(u1-v1*sqr_d,n,contextptr);
    } else {
        f1=pow(u1+v1*sqr_d,2*n+1,contextptr); f2=pow(u1-v1*sqr_d,2*n+1,contextptr);
    }
    if (output_components)
        return makevecteur(f1,f2,sqr_d);
    return makevecteur((f1+f2)/2,(f1-f2)/(2*sqr_d));
}

vecteur tengely_zeros(const gen &F,const gen &u,const gen &t,const gen &x,const gen &p,GIAC_CONTEXT) {
    gen s1=_zeros(makesequence(F-_pow(makesequence(u-t,p),contextptr),x),contextptr);
    gen s2=_zeros(makesequence(F-_pow(makesequence(u+t,p),contextptr),x),contextptr);
    vecteur s=mergevecteur(s1.type==_VECT?*s1._VECTptr:vecteur(0),s2.type==_VECT?*s2._VECTptr:vecteur(0));
    s.push_back(0);
    return s;
}

vecteur tengely_T_set(const gen &t,const gen &D,GIAC_CONTEXT) {
    vecteur dv=*_divis(D,contextptr)._VECTptr,res;
    for (const_iterateur it=dv.begin();it!=dv.end();++it) {
        const gen &d=*it;
        gen lb=-2*d*t,ub=2*d*t;
        gen n=lb.is_integer()?lb+1:_ceil(lb,contextptr);
        while (is_strictly_greater(ub,n,contextptr)) {
            if (!contains(res,n/d))
                res.push_back(n/d);
            n=n+1;
        }
    }
    return res;
}

/* factorize g over Q and return the list of factors with multiplicities */
vecteur factorize_over_Q(const gen &g,GIAC_CONTEXT) {
    bool old=contextptr->globalptr->_withsqrt_;
    contextptr->globalptr->_withsqrt_=false;
    vecteur fv=*_factors(g,contextptr)._VECTptr;
    assert(!fv.empty() && fv.size()%2==0);
    contextptr->globalptr->_withsqrt_=old;
    return fv;
}

/* return true if (multivariate) polynomial g with
 * rational coefficients is irreducible in Q */
bool is_irreducible_in_Q(const gen &g,GIAC_CONTEXT) {
    vecteur fv=factorize_over_Q(g,contextptr);
    int cnt=0;
    for (const_iterateur it=fv.begin();it!=fv.end();it=it+2) {
        if (!it->is_integer())
            ++cnt;
    }
    return cnt==1;
}

bool is_tengely(const gen &g,const gen &x,const gen &y,gen &F,gen &G,GIAC_CONTEXT) {
    vecteur xcf=*_coeff(makesequence(expand(g,contextptr),x),contextptr)._VECTptr;
    if (xcf.size()<2)
        return false;
    gen rest=xcf.back();
    xcf.pop_back();
    vecteur ycf=*_coeff(makesequence(expand(rest,contextptr),y),contextptr)._VECTptr;
    if (ycf.size()<2)
        return false;
    vecteur v=mergevecteur(xcf,ycf),vi;
    if (!is_constant_wrt_vars(v,makevecteur(x,y),contextptr) || !integralize(v,vi,contextptr))
        return false;
    xcf=vecteur(vi.begin(),vi.begin()+xcf.size());
    ycf=vecteur(vi.begin()+xcf.size(),vi.end());
    xcf.push_back(0);
    if (!is_one(-xcf.front()*ycf.front()))
        return false;
    F=_r2e(makesequence(xcf,x),contextptr);
    G=_r2e(makesequence(ycf,y),contextptr);
    if (is_one(_lcoeff(makesequence(F,x),contextptr)))
        G=-G;
    else F=-F;
    if (!is_irreducible_in_Q(F-G,contextptr))
        return false;
    gen n=xcf.size()-1,m=ycf.size()-1;
    return is_greater(m,n,contextptr) && !is_one(_gcd(makesequence(n,m),contextptr));
}

/* solve Diophantine equation in form F(x)=G(y) with gcd(deg(G),deg(F))>1,
 * where F and G are monic polynomials such that F(X)-G(Y) is irreducible
 * in Q[X,Y] and also deg(G)>=deg(F), using Tengely's algorithm */
vecteur solve_tengely(const gen &F,const gen &G,const gen &x,const gen &y,GIAC_CONTEXT) {
    gen n=_degree(makesequence(F,x),contextptr);
    gen m=_degree(makesequence(G,y),contextptr);
    gen d=_gcd(makesequence(n,m),contextptr);
    assert(d.is_integer() && d.val>1);
    gen p=_ifactors(d,contextptr)._VECTptr->front();
    gen pol=change_subtype(gen(_POLY1__VECT),_INT_MAPLECONVERSION);
    gen Fp=_pow(makesequence(F,_inv(p,contextptr)),contextptr);
    gen Gp=_pow(makesequence(G,_inv(p,contextptr)),contextptr);
    gen u=_series(makesequence(Fp,symb_equal(x,plus_inf),n/p,pol),contextptr);
    gen v=_series(makesequence(Gp,symb_equal(y,plus_inf),m/p,pol),contextptr);
    u=_ratnormal(u,contextptr);
    v=_ratnormal(v,contextptr);
    vecteur cu=*_coeff(makesequence(u,x),contextptr)._VECTptr;
    vecteur cv=*_coeff(makesequence(v,y),contextptr)._VECTptr;
    gen c0=cu.back()-cv.back();
    cu.pop_back();
    cv.pop_back();
    vecteur cf=mergevecteur(cu,cv);
    cf.push_back(c0);
    gen D=_lcm(_apply(makesequence(at_denom,cf),contextptr),contextptr);
    gen xlo,xhi,ylo,yhi,xl,xh,yl,yh,t=_inv(2*D,contextptr),numeq,last_numeq(undef);
    while (true) {
        vecteur xs=tengely_zeros(F,u,t,x,p,contextptr);
        vecteur ys=tengely_zeros(G,v,t,y,p,contextptr);
        xlo=_min(xs,contextptr);
        xhi=_max(xs,contextptr);
        ylo=_min(ys,contextptr);
        yhi=_max(ys,contextptr);
        numeq=_floor(xhi-xlo,contextptr)+_floor(yhi-ylo,contextptr)+(p==2?8*D*t:4*D*t+1);
        if (!is_undef(last_numeq) && !is_strictly_greater(last_numeq,numeq,contextptr))
            break;
        last_numeq=numeq;
        xl=_ceil(xlo,contextptr); if (!xl.is_integer()) xl=_ceil(_evalf(xlo,contextptr),contextptr);
        xh=_floor(xhi,contextptr); if (!xh.is_integer()) xh=_ceil(_evalf(xhi,contextptr),contextptr);
        yl=_ceil(ylo,contextptr); if (!yl.is_integer()) yl=_ceil(_evalf(ylo,contextptr),contextptr);
        yh=_floor(yhi,contextptr); if (!yh.is_integer()) yh=_ceil(_evalf(yhi,contextptr),contextptr);
        t=2*t;
    }
    vecteur sols;
    gen k,sol,soly,s,res;
    for (k=yl;is_greater(yh,k,contextptr);k=k+1) {
        sol=_rationalroot(subst(G,y,k,false,contextptr)-F,contextptr);
        if (sol.type==_VECT) for (const_iterateur it=sol._VECTptr->begin();it!=sol._VECTptr->end();++it) {
            if (it->is_integer() && !contains(sols,s=makevecteur(*it,k)))
                sols.push_back(s);
        }
    }
    for (k=xl;is_greater(xh,k,contextptr);k=k+1) {
        sol=_rationalroot(subst(F,x,k,false,contextptr)-G,contextptr);
        if (sol.type==_VECT) for (const_iterateur it=sol._VECTptr->begin();it!=sol._VECTptr->end();++it) {
            if (it->is_integer() && !contains(sols,s=makevecteur(k,*it)))
                sols.push_back(s);
        }
    }
    vecteur T=tengely_T_set(t,D,contextptr);
    for (int i=0;i<2;++i) {
        for (const_iterateur it=T.begin();it!=T.end();++it) {
            res=_resultant(makesequence(F-G,u+(i==0?-v:v)-*it,y),contextptr);
            sol=_rationalroot(res,contextptr);
            if (sol.type==_VECT) for (const_iterateur jt=sol._VECTptr->begin();jt!=sol._VECTptr->end();++jt) {
                if (jt->is_integer()) {
                    soly=_rationalroot(subst(F,x,*jt,false,contextptr)-G,contextptr);
                    if (soly.type==_VECT) for (const_iterateur kt=soly._VECTptr->begin();kt!=soly._VECTptr->end();++kt) {
                        if (kt->is_integer() && !contains(sols,s=makevecteur(*jt,*kt)))
                            sols.push_back(s);
                    }
                }
            }
        }
        if (is_strictly_greater(p,2,contextptr))
            break;
    }
    return sols;
}

/* return true iff g is a polynomial with integer coefficients w.r.t. vars */
bool is_int_poly(const gen &g,const vecteur &vars,GIAC_CONTEXT) {
    if (vars.empty() || is_constant_wrt_vars(g,vars,contextptr))
        return g.is_integer();
    if (!is_rational_wrt_vars(g,vars) || !is_one(_denom(g,contextptr)))
        return false;
    vecteur cf=*_coeff(makesequence(expand(g,contextptr),vars.front()),contextptr)._VECTptr;
    vecteur rest_vars(vars.begin()+1,vars.end());
    for (const_iterateur it=cf.begin();it!=cf.end();++it) {
        if (it->is_integer())
            continue;
        if (!is_int_poly(*it,rest_vars,contextptr))
            return false;
    }
    return true;
}

/* return true iff g is a polynomial in Q[x] with either:
 * - at least two distinct irreducible factors (over Q),
 * - exactly one irreducible factor with degree > 2,
 * - two (conjugate) complex roots. */
bool is_thue_polynomial(const gen &g,const gen &x,GIAC_CONTEXT) {
    vecteur cf=*_coeff(makesequence(expand(g,contextptr),x),contextptr)._VECTptr;
    for (const_iterateur it=cf.begin();it!=cf.end();++it) {
        if (!is_rational(*it))
            return false;
    }
    vecteur fv=factorize_over_Q(g,contextptr);
    int cnt=0;
    for (const_iterateur it=fv.begin();it!=fv.end();it+=2) {
        if (!it->is_integer())
            ++cnt;
    }
    if (cnt>=2 || (cnt==1 && is_greater(_degree(makesequence(g,x),contextptr),3,contextptr)))
        return true;
    gen z=_cZeros(makesequence(g,x),contextptr);
    if (z.type==_VECT) for (const_iterateur it=z._VECTptr->begin();it!=z._VECTptr->end();++it) {
        if (!is_exactly_zero(im(*it,contextptr)))
            return true;
    }
    return false;
}


/* return true iff g=P(X,Y)-a=0 is a Thue equation in x and y with finitely many solutions,
 * provides univariate polynomial P(x,1) and the free term a (nonzero integer) */
bool is_thue_equation(const gen &g,const gen &x,const gen &y,gen &P,gen &a,GIAC_CONTEXT) {
    vecteur vars=makevecteur(x,y);
    a=-_ratnormal(subst(g,vars,vecteur(vars.size(),0),false,contextptr),contextptr);
    if (!is_rational(a) || is_homogeneous_poly(g+a,vars,NULL,contextptr)==0)
        return false;
    P=subst(g+a,y,1,false,contextptr);
    return is_thue_polynomial(P,x,contextptr);
}

/* return true iff x^2+A=0 (mod B) is soluble */
bool is_soluble(const gen &A,const gen &B,GIAC_CONTEXT) {
    gen x(0);
    for (;is_strictly_greater(B,x,contextptr);x=x+1) {
        if (is_divisible(x*x+A,B,contextptr))
            return true;
    }
    return false;
}

/* return true iff a, b, and c do not have the same sign,
 * ax^2+by^2+cz^2=0 (mod 8) has a non-trivial solution (modulo 8) and
 * x^2+bc=0 (mod a), x^2+ac=0 (mod b), and x^2+ab=0 (mod c) are also soluble,
 * where a, b, and c are square-free and pairwise coprime
 * (this is equivalent to ax^2+by^2+cz^2=0 being locally soluble everywhere) */
bool is_locally_soluble_everywhere(const gen &a,const gen &b,const gen &c,GIAC_CONTEXT) {
    if (is_strictly_positive(a*b, contextptr) &&
        is_strictly_positive(a*c, contextptr) &&
        is_strictly_positive(b*c, contextptr))
        return false;
    bool is_soluble_in_Q2=false;
    for (int x=0;!is_soluble_in_Q2 && x<8;++x) {
        for (int y=0;!is_soluble_in_Q2 && y<8;++y) {
            for (int z=0;z<8;++z) {
                gen X(x),Y(y),Z(z);
                if (is_divisible(a*X*X+b*Y*Y+c*Z*Z,8,contextptr)) {
                    is_soluble_in_Q2=true;
                    break;
                }
            }
        }
    }
    if (!is_soluble_in_Q2)
        return false;
    return is_soluble(b*c,abs(a,contextptr),contextptr) &&
            is_soluble(a*c,abs(b,contextptr),contextptr) &&
            is_soluble(a*b,abs(c,contextptr),contextptr);
}

/* use Lagrange's method of descent to determine a solution of w^2=Ax^2+By^2 which is
 * locally soluble everywhere, return [x0,y0,w0] */
vecteur descent(const gen &A,const gen &B,GIAC_CONTEXT) {
    if (is_strictly_greater(abs(A,contextptr),abs(B,contextptr),contextptr)) {
        vecteur sol=descent(B,A,contextptr);
        return makevecteur(sol[1],sol[0],sol[2]);
    }
    if (is_one(A))
        return makevecteur(1,0,1);
    if (is_one(B))
        return makevecteur(0,1,1);
    gen ub=abs(B,contextptr)/gen(2),r(0);
    for (;is_greater(ub,r,contextptr);r=r+1) {
        if (is_divisible(r*r-A,B,contextptr))
            break;
    }
    assert(is_greater(ub,r,contextptr));
    gen Q=_ratnormal((r*r-A)/B,contextptr);
    gen d=square_part(Q,contextptr),d2=d*d;
    vecteur sol=descent(A,_ratnormal(Q/d2,contextptr),contextptr);
    return makevecteur((r*sol[0]-sol[2])/(r*r-A),sol[1]/(B*d),(-A*sol[0]+r*sol[2])/(r*r-A));
}

/* convert a ternary quadratic form into a*x^2+b*y^2+c*z^2 where a, b and c are square-free and pairwise coprime */
bool ternary_quadratic_normal_form(const gen &g_orig,const vecteur &vars,vecteur &P,vecteur &q,GIAC_CONTEXT) {
    /* diagonalize the form */
    gen g=_gauss(makesequence(g_orig,vars,0),contextptr);
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()!=3 ||
        g._VECTptr->at(1).type!=_VECT || g._VECTptr->at(2).type!=_VECT)
        return false;
    vecteur cf=*g._VECTptr->at(1)._VECTptr;
    P=*g._VECTptr->at(2)._VECTptr;
    if (!integralize(cf,q,contextptr))
        return false;
    /* make the coefficients square-free */
    for (iterateur it=q.begin();it!=q.end();++it) {
        gen d=square_part(*it,contextptr),&p=P[it-q.begin()];
        p=multvecteur(d,*p._VECTptr);
        *it=*it/(d*d);
    }
    /* make the coefficients pairwise coprime */
    for (int c=0;c<3;++c) {
        int i=c,j=(c+1)%3,k=(c+2)%3;
        gen d=_gcd(makesequence(q[i],q[j]),contextptr);
        q[i]=q[i]/d;
        q[j]=q[j]/d;
        q[k]=q[k]*d;
        P[k]=divvecteur(*P[k]._VECTptr,d);
    }
    return true;
}

/* return the general solution of Q(x,y,z)=0, where Q is a ternary quadratic form */
gen solve_ternary_quadratic_form(const gen &Q,const vecteur &vars,const vecteur &fvars,bool all_sols,GIAC_CONTEXT) {
    assert(fvars.size()>=2);
    const gen &p=fvars[0],&q=fvars[1],&Z3=fvars[2];
    /* normalize the quadratic form Q */
    vecteur R,P;
    if (!ternary_quadratic_normal_form(Q,vars,P,R,contextptr)) {
        *logptr(contextptr) << gettext("Error") << ": " << gettext("failed to normalize quadratic form") << "\n";
        return undef;
    }
    const gen &a=R[0],&b=R[1],&c=R[2];
    /* check local solubility */
    if (!is_locally_soluble_everywhere(a,b,c,contextptr))
        return vecteur(0);
    /* find a particular integral solution */
    vecteur s0=descent(-a*c,-b*c,contextptr),sol;
    s0[2]=s0[2]/c; //get z from w
    s0=*_linsolve(makesequence(P,s0),contextptr)._VECTptr;
    if (!integralize(s0,sol,contextptr)) {
        *logptr(contextptr) << gettext("Error") << ": " << gettext("particular solution is not rational") << "\n";
        return undef;
    }
    if (all_sols) {
        /* find the general solution */
        gen r=temp_symb("r",-1,contextptr),A,B;
        gen pol=_ratnormal(subst(Q,vars,makevecteur(r*sol[0],r*sol[1]+p,r*sol[2]+q),false,contextptr),contextptr);
        assert(is_linear_wrt(pol,r,A,B,contextptr));
        vecteur cr(5),ci;
        cr[0]=-_coeff(makesequence(A,p,1),contextptr);
        cr[1]=-_coeff(makesequence(A,q,1),contextptr);
        cr[2]=_coeff(makesequence(B,p,2),contextptr);
        cr[3]=_coeff(makesequence(B,makevecteur(p,q),makevecteur(1,1)),contextptr);
        cr[4]=_coeff(makesequence(B,q,2),contextptr);
        assert(integralize(cr,ci,contextptr));
        vecteur gsol(3);
        gen p2=pow(p,2,contextptr),q2=pow(q,2,contextptr);
        gsol[0]=sol[0]*ci[2]*p2+sol[0]*ci[3]*p*q+sol[0]*ci[4]*q2;
        gsol[1]=(ci[2]*sol[1]+ci[0])*p2+(ci[3]*sol[1]+ci[1])*p*q+ci[4]*sol[1]*q2;
        gsol[2]=ci[2]*sol[2]*p2+(ci[3]*sol[2]+ci[0])*p*q+(ci[4]*sol[2]+ci[1])*q2;
        return divvecteur(multvecteur(Z3,gsol),_Gcd(change_subtype(gsol,_SEQ__VECT),contextptr));
    }
    return sol;
}

/* remove solutions of x^2+bxy+cy^2+f=0 associated with the other ones
 * by using the criterion of Andreescu (2015, p. 56) */
void purge_nonfundamental_solutions(vecteur &sols,const gen &a,const gen &b,const gen &c,const gen &f,GIAC_CONTEXT) {
    gen D=b*b-4*a*c,N=-4*a*f;
    for (int i=0;i<int(sols.size());++i) {
        const gen &x1=sols[i]._VECTptr->front(),&y1=sols[i]._VECTptr->back();
        gen u1=2*a*x1+b*y1,v1=y1;
        for (int j=sols.size();j-->i+1;) {
            const gen &x2=sols[j]._VECTptr->front(),&y2=sols[j]._VECTptr->back();
            gen u2=2*a*x2+b*y2,v2=y2;
            if (is_divisible(u1*u2-D*v1*v2,N,contextptr) && is_divisible(v1*u2-u1*v2,N,contextptr)) {
                if (is_positive(v1,contextptr) && (!is_positive(v2,contextptr) || is_greater(v2,v1,contextptr)))
                    sols.erase(sols.begin()+j);
                else {
                    sols.erase(sols.begin()+i);
                    --i;
                    break;
                }
            }
        }
    }
}

/* find continued fraction expansion of a quadratic irrationality g, double the period if odd-length */
vecteur contfrac_expansion(const gen &g,GIAC_CONTEXT) {
    assert(!is_rational(g));
    int k=0;
    vecteur ex,period;
    while (true) {
        ex=*_dfc(makesequence(g,pow(10,++k,contextptr)),contextptr)._VECTptr;
        if (ex.back().type==_VECT) {
            period=*ex.back()._VECTptr;
            ex.pop_back();
            break;
        } // note that the loop will go forever if g is not a quadratic irrationality!
    }
    if (period.size()%2==0)
        return ex;
    return mergevecteur(ex,period);
}

/* find solutions to ax^2+bxy+cy^2+f=0 where 4f^2<b^2-4ac and b^2-4ac>0 is not a perfect square */
vecteur solve_contfrac(const gen &a,const gen &b,const gen &c,const gen &f,bool only_first,GIAC_CONTEXT) {
    gen d=b*b-4*a*c,sqr_d=sqrt(d,contextptr);
    assert(is_strictly_positive(d,contextptr) && !sqr_d.is_integer() && is_strictly_greater(d,4*f*f,contextptr));
    vecteur t=makevecteur((-b+sqr_d)/(2*a),(-b-sqr_d)/(2*a)),sols;
    for (const_iterateur it=t.begin();it!=t.end();++it) {
        vecteur ex=contfrac_expansion(*it,contextptr),p,q;
        int n=ex.size();
        p.resize(n+2,0); q.resize(n+2,0);
        p[1]=q[0]=1;
        for (int k=0;k<n;++k) {
            gen x=ex[k]*p[k+1]+p[k],y=ex[k]*q[k+1]+q[k],g=gcd(x,y);
            x=x/g; y=y/g;
            if (is_zero(a*x*x+b*x*y+c*y*y+f)) {
                if (only_first)
                    return makevecteur(x,y);
                sols.push_back(makevecteur(x,y));
            }
            p[k+2]=x;
            q[k+2]=y;
        }
    }
    return sols;
}

/* find solutions to ax^2+bxy+cy^2+f=0 where b^2-4ac>0 is not a perfect square, 4f^2>=b^2-4ac and gcd(a,b,f)=1 */
vecteur solve_abf_coprime(const gen &a,const gen &b,const gen &c,const gen &f,GIAC_CONTEXT) {
    vecteur sols;
    for (gen s=0;is_strictly_greater(_abs(f,contextptr),s,contextptr);s=s+1) {
        if (is_divisible(a*s*s+b*s+c,f,contextptr)) {
            vecteur sol=solve_contfrac((a*s*s+b*s+c)/f,-2*a*s-b,a*f,1,false,contextptr);
            for (const_iterateur it=sol.begin();it!=sol.end();++it) {
                const vecteur yz=*(it->_VECTptr);
                gen x=s*yz[0]-f*yz[1],y=yz[0];
                sols.push_back(makevecteur(x,y));
            }
        }
    }
    return mergevecteur(sols,multvecteur(-1,sols));
}

/* find solutions to ax^2+bxy+cy^2+f=0 where b^2-4ac>0 is not a perfect square */
vecteur solve_homogeneous_binary_quadratic(const vecteur &cf,GIAC_CONTEXT) {
    gen a=cf[0],b=cf[1],c=cf[2],f=cf[3];
    gen d=b*b-4*a*c,g=_gcd(makesequence(a,b,c),contextptr);
    if (!is_divisible(f,g,contextptr))
        return vecteur(0); // no solutions
    a=a/g; b=b/g; c=c/g; f=f/g;
    if (is_strictly_greater(d,4*f*f,contextptr))
        return solve_contfrac(a,b,c,f,false,contextptr);
    vecteur dv=*_idivis(f,contextptr)._VECTptr,sols,sol;
    for (const_iterateur it=dv.begin();it!=dv.end();++it) {
        if (!is_perfect_square(*it))
            continue;
        gen F=f/(*it),i,m;
        if (is_strictly_greater(d,4*F*F,contextptr)) {
            sol=solve_contfrac(a,b,c,F,false,contextptr);
        } else if (is_one(_gcd(makesequence(a,b,F),contextptr))) {
            sol=solve_abf_coprime(a,b,c,F,contextptr);
        } else if (is_one(_gcd(makesequence(b,c,F),contextptr))) {
            sol=solve_abf_coprime(c,b,a,F,contextptr);
            sol=*_apply(makesequence(at_revlist,sol),contextptr)._VECTptr;
        } else {
            bool found=false;
            for (i=0;!found && is_strictly_greater(_abs(F,contextptr),i,contextptr);i=i+1) {
                for (m=0;is_greater(i+1,m,contextptr);m=m+1) {
                    if (is_one(gcd(i,m)) && is_one(gcd(a*i*i+b*i*m+c*m*m,F))) {
                        found=true;
                        break;
                    }
                }
            }
            if (found) {
                vecteur nj=*_iabcuv(makesequence(i,-m,1),contextptr)._VECTptr;
                const gen &n=nj.front(),&j=nj.back();
                sol=solve_abf_coprime(a*i*i+b*i*m+c*m*m,2*a*i*j+b*i*n+b*j*m+2*c*m*n,a*j*j+b*j*n+c*n*n,F,contextptr);
                for (iterateur jt=sol.begin();jt!=sol.end();++jt) {
                    gen X=jt->_VECTptr->front(),Y=jt->_VECTptr->back();
                    *jt=makevecteur(i*X+j*Y,m*X+n*Y);
                }
            }
        }
        if (!sol.empty())
            sols=mergevecteur(sols,multvecteur(sqrt(*it,contextptr),sol));
    }
    return sols;
}

vecteur solve_linrec(const vecteur &r1,const vecteur &r2,const gen &x1,const gen &y1,const gen &n,GIAC_CONTEXT) {
    gen u=temp_symb("u",-1,contextptr),v=temp_symb("v",-1,contextptr);
    gen un=symb_of(u,n),vn=symb_of(v,n),unp=symb_of(u,n+1),vnp=symb_of(v,n+1),u1=symb_of(u,0),v1=symb_of(v,0);
    gen rec1=symb_equal(unp,r1[0]*un+r1[1]*vn+r1[2]),rec2=symb_equal(vnp,r2[0]*un+r2[1]*vn+r2[2]);
    gen icond1=symb_equal(u1,x1),icond2=symb_equal(v1,y1);
    gen args=makesequence(makevecteur(rec1,rec2),makevecteur(un,vn),makevecteur(icond1,icond2));
    return *_rsolve(args,contextptr)._VECTptr->front()._VECTptr;
}

/* solve general binary quadratic equation using the methods of Dario Alejandro Alpern at
 * https://www.alpertron.com.ar/METHODS.HTM */
gen solve_binary_quadratic(const vecteur &cf,gen &ph,bool set_ph,bool &alt_sols,bool fund_only,GIAC_CONTEXT) {
    gen a=cf[0],b=cf[1],c=cf[2],d=cf[3],e=cf[4],f=cf[5];
    if (is_zero(a) && is_zero(b) && is_zero(c)) {
        /* linear case */
        gen g=_gcd(makesequence(d,e),contextptr);
        if (!is_divisible(f,g,contextptr))
            return vecteur(0); // no solutions
        d=d/g; e=e/g; f=f/g;
        gen uv=_iabcuv(makesequence(d,e,-f),contextptr);
        assert(uv.type==_VECT && uv._VECTptr->size()==2);
        gen u=uv._VECTptr->front(),v=uv._VECTptr->back();
        gen t=is_undef(ph)?make_integer_placeholder("Z",contextptr):ph;
        if (is_undef(ph) && set_ph)
            ph=t;
        return makevecteur(u+e*t,v-d*t);
    }
    vecteur sol;
    if (is_zero(a) && is_zero(c) && !is_zero(b)) {
        /* simple hyperbolic case */
        if (is_zero(d*e-b*f)) {
            bool c1=is_divisible(e,b,contextptr),c2=is_divisible(d,b,contextptr);
            if (!c1 && !c2)
                return vecteur(0); // no solutions
            gen t=is_undef(ph)?make_integer_placeholder("Z",contextptr):ph;
            if (c1) sol.push_back(makevecteur(-e/b,t));
            if (c2) sol.push_back(makevecteur(t,-d/b));
        } else {
            vecteur dv=*_idivis(d*e-b*f,contextptr)._VECTptr;
            dv=mergevecteur(dv,multvecteur(-1,dv)); // also include negative divisors
            for (const_iterateur it=dv.begin();it!=dv.end();++it) {
                const gen &D=*it;
                gen x=(D-e)/b,y=((d*e-b*f)/D-d)/b;
                if (x.is_integer() && y.is_integer())
                    sol.push_back(makevecteur(x,y));
            }
        }
        return sol;
    }
    gen dsc=b*b-4*a*c;
    if (!is_positive(dsc,contextptr)) {
        /* elliptical case */
        gen A=dsc,B=2*(b*e-2*c*d),C=e*e-4*c*f;
        dsc=B*B-4*A*C;
        if (!is_positive(dsc,contextptr))
            return vecteur(0); // no solutions
        if (!is_positive(A,contextptr)) {
            A=-A; B=-B; C=-C;
        }
        gen r1=(-B-sqrt(dsc,contextptr))/(2*A),r2=(-B+sqrt(dsc,contextptr))/(2*A);
        assert(is_greater(r2,r1,contextptr));
        A=c;
        for (gen x=_ceil(r1,contextptr);is_greater(r2,x,contextptr);x=x+1) {
            B=e+b*x; C=a*x*x+d*x+f;
            if (!is_positive(dsc=B*B-4*A*C,contextptr))
                continue;
            gen y1=(-B-sqrt(dsc,contextptr))/(2*A),y2=(-B+sqrt(dsc,contextptr))/(2*A);
            if (y1.is_integer())
                sol.push_back(makevecteur(x,y1));
            if (!is_zero(dsc) && y2.is_integer())
                sol.push_back(makevecteur(x,y2));
        }
        return sol;
    }
    if (is_zero(dsc)) {
        /* parabolic case */
        gen sgn(is_positive(a*b,contextptr)?1:-1);
        gen g=(is_positive(a,contextptr)?1:-1)*_gcd(makesequence(a,c),contextptr);
        a=a/g; b=b/g; c=c/g;
        gen sqr_a=sqrt(a,contextptr),sqr_c=sgn*sqrt(c,contextptr);
        assert(sqr_a.is_integer() && sqr_c.is_integer());
        gen r=sqr_c*d-sqr_a*e;
        if (is_zero(r)) {
            dsc=d*d-4*a*f*g;
            if (!is_positive(dsc,contextptr))
                return vecteur(0); // no solutions
            gen u1=(-d+sqrt(dsc,contextptr))/(2*sqr_a*g),u2=(-d-sqrt(dsc,contextptr))/(2*sqr_a*g);
            if (u1.is_integer()) {
                gen s=solve_binary_quadratic(makevecteur(0,0,0,sqr_a,sqr_c,-u1),ph,true,alt_sols,false,contextptr);
                if (s.type==_VECT && !s._VECTptr->empty())
                    sol.push_back(s);
            }
            if (!is_zero(dsc) && u2.is_integer()) {
                gen s=solve_binary_quadratic(makevecteur(0,0,0,sqr_a,sqr_c,-u2),ph,false,alt_sols,false,contextptr);
                if (s.type==_VECT && !s._VECTptr->empty())
                    sol.push_back(s);
            }
        } else {
            gen t(undef);
            for (gen u(0);is_greater(_abs(r,contextptr),u,contextptr);u=u+1) {
                gen numx=sqr_c*g*u*u+e*u+sqr_c*f,numy=sqr_a*g*u*u+d*u+sqr_a*f;
                if (is_divisible(numx,r,contextptr) && is_divisible(numy,r,contextptr)) {
                    if (is_undef(t))
                        t=is_undef(ph)?make_integer_placeholder("Z",contextptr):ph;
                    gen x=-sqr_c*g*r*pow(t,2,contextptr)-t*(e+2*sqr_c*g*u)-numx/r;
                    gen y=sqr_a*g*r*pow(t,2,contextptr)+t*(d+2*sqr_a*g*u)+numy/r,contextptr;
                    sol.push_back(makevecteur(x,y));
                }
            }
        }
        return sol;
    }
    if (is_strictly_positive(dsc,contextptr)) {
        /* tedious hyperbolic case */
        if (is_zero(d) && is_zero(e)) { // binary quadratic form
            if (!is_perfect_square(dsc)) {
                vecteur psols=solve_homogeneous_binary_quadratic(makevecteur(a,b,c,f),contextptr);
                if (!psols.empty()) {
                    purge_nonfundamental_solutions(psols,a,b,c,f,contextptr);
                    gen n;
                    if (!fund_only)
                        n=is_undef(ph)?make_integer_placeholder("Z",contextptr):ph;
                    for (const_iterateur it=psols.begin();it!=psols.end();++it) {
                        gen x=it->_VECTptr->front(),y=it->_VECTptr->back();
                        if (!fund_only) {
                            /* find other solutions by solving a system of linear recurrences */
                            vecteur rs=solve_contfrac(1,b,a*c,-1,true,contextptr);
                            const gen &r=rs.front(),&s=rs.back();
                            vecteur rsol=solve_linrec(makevecteur(r,-c*s,0),makevecteur(a*s,r+b*s,0),x,y,n,contextptr);
                            x=rsol.front(); y=rsol.back();
                        }
                        sol.push_back(makevecteur(x,y));
                    }
                }
            } else { // simple factorization
                if (is_zero(a)) {
                    gen s=solve_binary_quadratic(makevecteur(c,b,a,f),ph,false,alt_sols,false,contextptr);
                    if (s.type==_VECT)
                        sol=*_apply(makesequence(at_revlist,s),contextptr)._VECTptr;
                } else {
                    gen k=sqrt(dsc,contextptr);
                    if (is_zero(f)) {
                        gen s=solve_binary_quadratic(makevecteur(0,0,0,2*a,b+k,0),ph,true,alt_sols,false,contextptr);
                        if (s.type==_VECT && !s._VECTptr->empty())
                            sol.push_back(s);
                        s=solve_binary_quadratic(makevecteur(0,0,0,2*a,b-k,0),ph,false,alt_sols,false,contextptr);
                        if (s.type==_VECT && !s._VECTptr->empty())
                            sol.push_back(s);
                    } else {
                        vecteur u=*_idivis(-4*a*f,contextptr)._VECTptr;
                        u=mergevecteur(u,multvecteur(-1,u)); // also include negative divisors
                        for (const_iterateur it=u.begin();it!=u.end();++it) {
                            gen y=(*it+4*a*f/(*it))/(2*k);
                            gen x=(*it-(b+k)*y)/(2*a);
                            if (x.is_integer() && y.is_integer())
                                sol.push_back(makevecteur(x,y));
                        }
                    }
                }
            }
        } else { // the non-homogeneous case
            gen G=2*a*e-b*d,s,g;
            g=_gcd(makesequence(dsc,G),contextptr);
            gen F=4*a*(-f*dsc-a*e*e+b*d*e-c*d*d)/g;
            s=solve_binary_quadratic(makevecteur(-dsc/g,0,g,0,0,F),ph,false,alt_sols,true,contextptr);
            if (s.type==_VECT) {
                for (const_iterateur it=s._VECTptr->begin();it!=s._VECTptr->end();++it) {
                    const gen &x1=it->_VECTptr->front(),&y1=it->_VECTptr->back();
                    gen y=-(g*y1-G)/dsc;
                    gen x=(x1-b*y-d)/(2*a);
                    vecteur Z=*_lname(makevecteur(x,y),contextptr)._VECTptr;
                    if (Z.empty()) {
                        if (x.is_integer() && y.is_integer())
                            sol.push_back(makevecteur(x,y));
                    } else {
                        assert(Z.size()==1); // there should be exactly one global variable
                        const gen &t=Z.front();
                        gen A,B,C,D;
                        assert(is_linear_wrt(x,t,A,B,contextptr) && is_linear_wrt(y,t,C,D,contextptr));
                        if (!B.is_integer() || !D.is_integer())
                            continue;
                        gen pA=_numer(A,contextptr),pC=_numer(C,contextptr);
                        gen qA=_denom(A,contextptr),qC=_denom(C,contextptr);
                        A=pA/_gcd(makesequence(pA,qA),contextptr);
                        C=pC/_gcd(makesequence(pC,qC),contextptr);
                        sol.push_back(makevecteur(A*t+B,C*t+D));
                    }
                }
                if (!fund_only && !is_perfect_square(dsc) && !sol.empty()) {
                    /* find other solutions by solving a system of linear recurrences */
                    gen n=is_undef(ph)?make_integer_placeholder("Z",contextptr):ph;
                    for (iterateur it=sol.begin();it!=sol.end();++it) {
                        gen x=it->_VECTptr->front(),y=it->_VECTptr->back();
                        vecteur rsv=solve_contfrac(1,b,a*c,-1,false,contextptr);
                        gen P,Q,R,S,K,L,r,s;
                        for (const_iterateur jt=rsv.begin();jt!=rsv.end();++jt) {
                            r=jt->_VECTptr->front(); s=jt->_VECTptr->back();
                            P=r,Q=-c*s; R=a*s; S=r+b*s;
                            K=-(c*d*(P+S-2)+e*(b-b*r-2*a*c*s))/dsc;
                            L=d*s-(d*(b-b*r-2*a*c*s)+a*e*(P+S-2))/dsc;
                            if (K.is_integer() && L.is_integer())
                                break;
                        }
                        if (!K.is_integer() || !L.is_integer()) {
                            r=rsv.front()._VECTptr->front(); s=rsv.front()._VECTptr->back();
                            P=r*r-a*c*s*s; Q=-c*s*(2*r+b*s); K=-c*d*s*s-e*r*s;
                            R=a*s*(2*r+b*s); S=r*r+2*b*r*s+(b*b-a*c)*s*s; L=d*s*(r+b*s)-a*e*s*s;
                        }
                        *it=solve_linrec(makevecteur(P,Q,K),makevecteur(R,S,L),x,y,n,contextptr);
                    }
                }
            }
        }
        return sol;
    }
    return undef; // unable to solve the equation
}

/* Diophantine equation solver, which can solve:
 *  - (systems of) linear equation(s) (Esmaeili et al., 2001)
 *  - Pell equations (Junod, 2015)
 *  - Thue equations (using PARI) - only in MinGW!
 *  - homogeneous equations Q(x,y,z)=0, where Q is ternary quadratic form (Smart, 1998)
 *  - general quadratic equations in two variables (Alpern)
 *  - certain polynomial equations of the type f(x)=g(y) (Tengely, 2003)
 */
gen _isolve(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    gen eq;
    vecteur vars,fvars,opts,eqv;
    if (g.type==_VECT && g.subtype==_SEQ__VECT) {
        const vecteur &gv=*g._VECTptr;
        eq=gv.front();
        opts=vecteur(gv.begin()+1,gv.end());
    } else eq=g;
    if (!parse_equations(eq,eqv))
        return gentypeerr(contextptr);
    vars=sort_identifiers(*_lname(eqv,contextptr)._VECTptr,contextptr);
    gen sol(undef);
    matrice A;
    vecteur Fr;
    bool find_alternate_sols=false,output_sequences=true;
    int n=vars.size(),m=eqv.size();
    /* parse options */
    for (const_iterateur it=opts.begin();it!=opts.end();++it) {
        if (it->is_symb_of_sommet(at_equal)) {
            const gen &ls=it->_SYMBptr->feuille._VECTptr->front();
            const gen &rs=it->_SYMBptr->feuille._VECTptr->back();
            if (ls==at_seq && rs.is_integer() && rs.subtype==_INT_BOOLEAN)
                output_sequences=(bool)rs.val;
        } else if (it->type==_IDNT) {
            fvars.push_back(*it);
        } else if (it->type==_VECT) {
            for (const_iterateur jt=it->_VECTptr->begin();jt!=it->_VECTptr->end();++jt) {
                if (jt->type==_IDNT)
                    fvars.push_back(*jt);
                else return gentypeerr(contextptr);
            }
        }
    }
    /* solve equation(s) */
    if (n==1) {
        /* univariate case */
        sol=solve_quiet(eqv,vars.front(),contextptr);
        for (int i=sol._VECTptr->size();i-->0;) {
            if (!sol._VECTptr->at(i).is_integer())
                sol._VECTptr->erase(sol._VECTptr->begin()+i);
        }
    } else if (is_linear_system(eqv,vars,A,Fr,contextptr) && (n>2 || m>1)) {
        /* linear equation with at least three indeterminates or a system of linear equations */
        gen rk=_rank(A,contextptr);
        if (!rk.is_integer() || rk.val<m)
            return gensizeerr(gettext("System matrix is not full rank."));
        vecteur tmp(n);
        for (int i=0;i<n;++i) {
            tmp[i]=temp_symb("tmp_",i,contextptr);
        }
        sol=vecteur(0);
        if (ABS_diophantine(A,Fr,tmp,*sol._VECTptr,contextptr)) {
            vecteur tmp_vars=*_lname(sol,contextptr)._VECTptr;
            for (const_iterateur it=tmp_vars.begin();it!=tmp_vars.end();++it) {
                int i=it-tmp_vars.begin();
                sol=subst(sol,*it,i<int(fvars.size())?fvars[i]:make_integer_placeholder("Z",contextptr),
                          false,contextptr);
            }
        }
    } else if (n==2 && m==1) {
        /* single equation with two indeterminates */
        gen &x=vars.front(),&y=vars.back(),&eq=eqv.front(),a,b,c,d,e,f;
        vecteur t(6);
        int pd1=0,pd2=0;
        bool y_first=false;
        if ((pd1=is_pell_equation(eq,x,y,d,contextptr))!=0 ||
                (pd2=is_pell_equation(eq,y,x,d,contextptr))!=0) {
            /* Pell equation */
            if (output_sequences) {
                gen ph=fvars.empty()?undef:fvars.front();
                sol=pell_general(d,pd1+pd2>0,ph,false,contextptr);
            } else sol=pell_fundamental(d,pd1+pd2>0,contextptr);
            if (pd1==0)
                sol=_apply(makesequence(at_revlist,sol),contextptr);
        } else if (is_binary_quadratic_wrt(eq,x,y,a,b,c,d,e,f,contextptr) &&
                   integralize(makevecteur(a,b,c,d,e,f),t,contextptr)) {
            gen ph(fvars.empty()?undef:fvars.front());
            sol=solve_binary_quadratic(t,ph,false,find_alternate_sols,!output_sequences,contextptr);
        } else if (is_thue_equation(eq,x,y,t[0],t[1],contextptr) ||
                   (y_first=is_thue_equation(eq,y,x,t[0],t[1],contextptr))) {
            /* Thue equation */
            vecteur Pcf=*_coeff(makesequence(t[0],y_first?y:x),contextptr)._VECTptr,cf_int;
            Pcf.push_back(t[1]);
            assert(integralize(Pcf,cf_int,contextptr));
            if (is_zero(cf_int.back())) {
                cf_int.pop_back();
                gen P=_r2e(makesequence(cf_int,x),contextptr);
                gen rsol=_rationalroot(P,contextptr);
                gen ph=fvars.empty()?undef:fvars.front();
                sol=vecteur(0);
                if (rsol.type==_VECT) for (const_iterateur it=rsol._VECTptr->begin();it!=rsol._VECTptr->end();++it) {
                    if (is_zero(*it))
                        continue;
                    if (it->is_integer()) {
                        a=*it; b=1;
                    } else {
                        a=_numer(*it,contextptr);
                        b=_denom(*it,contextptr);
                    }
                    if (is_undef(ph))
                        ph=make_integer_placeholder("Z",contextptr);
                    sol._VECTptr->push_back(makevecteur(a*ph,b*ph));
                }
            } else {
#ifdef __MINGW_H
#ifdef HAVE_LIBPARI
                GEN alpha=gen2GEN(cf_int.back(),vecteur(0),contextptr);
                cf_int.pop_back();
                gen R=expand(_r2e(makesequence(cf_int,x),contextptr),contextptr);
                GEN P=gen2GEN(R,vecteur(1,x),contextptr);
                GEN tnf=thueinit(P,1,DEFAULTPREC);
                sol=GEN2gen(thue(tnf,alpha,NULL),vecteur(0));
#else
                *logptr(contextptr) << "Warning: PARI library is required for solving Thue equations\n";
#endif
#else
                *logptr(contextptr) << gettext("Error") << ": " << gettext("failed to solve Thue equation") << "\n";
#endif
            }
            if (sol.type==_VECT && y_first)
                sol=_apply(makesequence(at_revlist,sol),contextptr);
            find_alternate_sols=true;
        } else if (is_tengely(eq,x,y,t[0],t[1],contextptr) || (y_first=is_tengely(eq,y,x,t[0],t[1],contextptr))) {
            /* polynomial equation of type f(x)=g(y) */
            sol=solve_tengely(t[0],t[1],y_first?y:x,y_first?x:y,contextptr);
            if (y_first)
                sol=_apply(makesequence(at_revlist,sol),contextptr);
        }
        if (find_alternate_sols && sol.type==_VECT) for (int i=sol._VECTptr->size();i-->0;) {
            const vecteur &s=*sol._VECTptr->at(i)._VECTptr;
            matrice alt_s;
            alt_s.push_back(makevecteur(s[1],s[0]));
            alt_s.push_back(makevecteur(-s[1],s[0]));
            alt_s.push_back(makevecteur(s[1],-s[0]));
            alt_s.push_back(makevecteur(-s[1],-s[0]));
            alt_s.push_back(makevecteur(-s[0],s[1]));
            alt_s.push_back(makevecteur(s[0],-s[1]));
            alt_s.push_back(makevecteur(-s[0],-s[1]));
            for (const_iterateur it=alt_s.begin();it!=alt_s.end();++it) {
                if (is_zero(_ratnormal(subst(eq,vars,*it,false,contextptr),contextptr)) && !contains(sol,*it))
                    sol._VECTptr->push_back(*it);
            }
        }
    } else if (n==3 && m==1) {
        /* three variables, single equation */
        const gen &eq=eqv.front();
        if (is_homogeneous_poly(eq,vars,NULL,contextptr)==2) {
            /* ternary quadratic form */
            while (fvars.size()<3) {
                fvars.push_back(make_integer_placeholder("Z",contextptr));
            }
            sol=solve_ternary_quadratic_form(eq,vars,fvars,output_sequences,contextptr);
        }
    }
    if (is_undef(sol)) {
        /* check whether a system of polynomial equations with integer coefficients is given, try solving */
        bool is_int_sys=true;
        for (const_iterateur it=eqv.begin();it!=eqv.end();++it) {
            if (!is_int_poly(*it,vars,contextptr)) {
                is_int_sys=false;
                break;
            }
        }
        if (is_int_sys) {
            sol=solve_quiet(eqv,vars,contextptr);
            if (sol.type==_VECT) {
                for (int i=sol._VECTptr->size();i-->0;) {
                    const vecteur &isol=*(sol._VECTptr->at(i))._VECTptr;
                    for (const_iterateur it=isol.begin();it!=isol.end();++it) {
                        if (it->is_integer())
                            continue;
                        if (!is_int_poly(*it,vars,contextptr)) {
                            sol._VECTptr->erase(sol._VECTptr->begin()+i);
                            break;
                        }
                    }
                }
            }
            vecteur sph=*_lname(sol,contextptr)._VECTptr;
            if (!sph.empty()) {
                for (const_iterateur it=sph.begin();it!=sph.end();++it) {
                    int i=it-sph.begin();
                    gen ph=i<int(fvars.size())?fvars[i]:make_integer_placeholder("Z",contextptr);
                    sol=subst(sol,*it,ph,false,contextptr);
                }
            }
        } else return gentypeerr(gettext("Failed to solve equation over integers."));
    }
    if (n>1 && sol.type==_VECT) {
        /* format the output */
        if (ckmatrix(*sol._VECTptr,false) && int(sol._VECTptr->front()._VECTptr->size())==n) {
            for (iterateur it=sol._VECTptr->begin();it!=sol._VECTptr->end();++it) {
                *it=_zip(makesequence(at_equal,vars,*it),contextptr);
            }
        } else if (!sol._VECTptr->empty())
            sol=_zip(makesequence(at_equal,vars,sol),contextptr);
        sol=change_subtype(sol,_LIST__VECT);
    }
    return sol;
}
static const char _isolve_s []="isolve";
static define_unary_function_eval (__isolve,&_isolve,_isolve_s);
define_unary_function_ptr5(at_isolve,alias_at_isolve,&__isolve,0,true)


#ifndef NO_NAMESPACE_GIAC
}
#endif // ndef NO_NAMESPACE_GIAC
