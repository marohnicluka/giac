/*
 * signalprocessing.cc
 *
 * (c) 2018-2019 by Luka Marohnić
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
#include "signalprocessing.h"
#include "graphtheory.h"
#include "usual.h"
#include "plot.h"

#ifdef HAVE_LIBSAMPLERATE
#include "samplerate.h"
#endif
#ifdef HAVE_LIBAO
#include <ao/ao.h>
#endif
#ifndef HAVE_PNG_H
#undef HAVE_LIBPNG
#endif
#ifdef HAVE_LIBPNG
#include <png.h>
#endif

using namespace std;

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

#define _SP_BAD_SOUND_DATA gettext("Invalid sound data")
#define _SP_INVALID_RANGE gettext("Invalid range specification")
#define _SP_BAD_WINDOW gettext("Invalid window parameters")
#define MAX_TAILLE 500
#define MAX_FACTORS 10
#define MIDI_NOTE_0 8.17579891564
#define GREY_COLOR 47

double mylog2(double x) {
    return std::log(x)/M_LN2;
}

string n2hexstr(char c) {
    static const char* digits="0123456789ABCDEF";
    std::string hex(2,'0');
    for (size_t i=0,j=4;i<2;++i,j-=4)
        hex[i]=digits[(c>>j) & 0x0f];
    return hex;
}

int nextpow2(int n) {
    return 1<<int(std::ceil(mylog2(n)));
}

gen generr(const char* msg) {
    string m(msg);
    m.append(".");
    return gensizeerr(m.c_str());
}

gen generrtype(const char* msg) {
    string m(msg);
    m.append(".");
    return gentypeerr(m.c_str());
}

gen generrdim(const char* msg) {
    string m(msg);
    m.append(".");
    return gendimerr(m.c_str());
}

gen generrarg(int i) {
    return generr(string(gettext("Invalid argument")+string(" ")+print_INT_(i)).c_str());
}

void print_error(const char *msg,GIAC_CONTEXT) {
    *logptr(contextptr) << gettext("Error") << ": " << gettext(msg) << "\n";
}

void print_warning(const char *msg,GIAC_CONTEXT) {
    *logptr(contextptr) << gettext("Warning") << ": " << gettext(msg) << "\n";
}

bool is_whitespace(char c) {
    return c==32 || (c>8 && c<14);
}

string trim_left(const string &s) {
    string::const_iterator it=s.begin(),itend=s.end();
    for (;it!=itend && is_whitespace(*it);++it);
    return string(it,itend);
}

string trim_right(const string &s) {
    string::const_iterator it=s.end(),itend=s.begin();
    bool yes=true;
    while (it!=itend && (yes=is_whitespace(*(--it))));
    if (it!=itend || !yes) ++it;
    return string(itend,it);
}

bool iszero(const gen &g,const gen &t,GIAC_CONTEXT) {
    return (is_undef(t) && is_zero(_abs(g,contextptr),contextptr)) ||
        (!is_undef(t) && is_strictly_greater(t,_abs(g,contextptr),contextptr));
}

gen _trim(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    bool has_opt=g.type==_VECT && g.subtype==_SEQ__VECT,retpos=false;
    if (has_opt && g._VECTptr->size()<2)
        return gendimerr(contextptr);
    const gen &a=has_opt?g._VECTptr->front():g;
    int optstart=has_opt?g._VECTptr->size()-1:0,dir=0;
    if (has_opt) {
        if (g._VECTptr->at(optstart)==at_index) retpos=(bool)(optstart--);
        if (g._VECTptr->at(optstart)==at_left) dir=-(optstart--);
        else if (g._VECTptr->at(optstart)==at_right) dir=(optstart--);
    }
    if (a.type==_STRNG) { // trim a string
        if (optstart)
            return gensizeerr(contextptr);
        string lstr=dir<=0?trim_left(*a._STRNGptr):*a._STRNGptr;
        string rstr=dir>=0?trim_right(lstr):lstr;
        size_t d=a._STRNGptr->length()-lstr.length(),len=rstr.length();
        return retpos?(dir<0?gen(d):(dir>0?gen(len):makesequence(d,len))):string2gen(rstr,false);
    }
    gen thr;
    rgba_image *img=rgba_image::from_gen(a);
    if (img!=NULL) { // trim zero pixel margins from an image
        if (dir)
            return generr(gettext("One-sided trimming is not supported for images"));
        if (optstart>1)
            return gentoomanyargs(gettext("Too many arguments"),contextptr);
        bool inverted=false;
        if (optstart==1) {
            if (g._VECTptr->at(optstart)!=at_inv)
                return gensizeerr(contextptr);
            inverted=true;
        }
        matrice cd=*(inverted?-(*img):*img)[change_subtype(GREY_COLOR,_INT_COLOR)]._VECTptr;
        vecteur xw=*_trim(makesequence(_sum(cd,contextptr),at_index),contextptr)._VECTptr;
        vecteur yh=*_trim(makesequence(_sum(mtran(cd),contextptr),at_index),contextptr)._VECTptr;
        gen ret=makesequence(xw[0],yh[0],xw[1],yh[1]);
        return retpos?ret:(*img)(ret,contextptr);
    }
    audio_clip *clip=audio_clip::from_gen(a);
    if (clip!=NULL) { // trim an audio clip
        int n=clip->length(),sr=clip->sample_rate(),nc=clip->channel_count(),cs=sr/100;
        gen dbs(-30.0);
        if (optstart>2)
            return gentoomanyargs(gettext("Too many arguments"),contextptr);
        if (optstart==2) {
            const gen &opt=g._VECTptr->at(optstart);
            if (!is_real_number(opt,contextptr) || !is_positive(opt,contextptr))
                return generr(gettext("Expected a nonnegative real number"));
            cs=_round(sr*to_real_number(opt,contextptr),contextptr).val;
            --optstart;
        }
        if (optstart==1) {
            const gen &opt=g._VECTptr->at(optstart);
            if (!is_real_number(opt,contextptr) || !is_strictly_positive(-opt,contextptr))
                return generr(gettext("Expected a negative real number"));
            dbs=to_real_number(opt,contextptr);
        }
        int N=nextpow2(sr/20);
        gen data=_channel_data(_stereo2mono(a,contextptr),contextptr);
        if (data.type!=_VECT)
            return gensizeerr(gettext("Failed to extract audio data"));
        thr=_pow(makesequence(10.0,dbs/gen(10.0)),contextptr);
        double alpha=0.05;
        int overlap=int(alpha*(N-1)),i;
        data._VECTptr->resize(((n-overlap)/(N-overlap)+1)*(N-overlap)+overlap,0);
        int nn=data._VECTptr->size(),lb=0,ub=n;
        if (dir<=0) {
            for (i=0;i<nn-N+1;i+=N-overlap) {
                vecteur v=*_tukey_window(makesequence(vecteur(data._VECTptr->begin()+i,data._VECTptr->begin()+i+N),alpha),contextptr)._VECTptr;
                vecteur e=*_hilbert(v,contextptr)._VECTptr;
                iterateur it=e.begin(),itend=e.end();
                for (;it!=itend;++it)
                    *it=pow(re(*it,contextptr),2)+pow(im(*it,contextptr),2);
                v=*_moving_average(makesequence(e,cs),contextptr)._VECTptr;
                lb=_trim(makesequence(v,thr,at_left,at_index),contextptr).val;
                if (lb<N-cs+1) {
                    lb+=i;
                    break;
                }
            }
            if (i>=nn-N+1)
                lb=n;
            lb=std::min(lb,n);
        }
        if (dir>=0) {
            for (i=nn-N;i>=0;i-=N-overlap) {
                vecteur v=*_tukey_window(makesequence(vecteur(data._VECTptr->begin()+i,data._VECTptr->begin()+i+N),alpha),contextptr)._VECTptr;
                vecteur e=*_hilbert(v,contextptr)._VECTptr;
                iterateur it=e.begin(),itend=e.end();
                for (;it!=itend;++it)
                    *it=pow(re(*it,contextptr),2)+pow(im(*it,contextptr),2);
                e=*_moving_average(makesequence(e,cs),contextptr)._VECTptr;
                ub=_trim(makesequence(e,thr,at_right,at_index),contextptr).val;
                if (ub>0) {
                    ub+=i;
                    break;
                }
            }
            if (i<0)
                ub=0;
            ub=std::max(ub,0);
        }
        if (!retpos && ub<=lb)
            return vecteur(0);
        if (dir<0) return retpos?lb:a(lb,contextptr);
        if (dir>0) return retpos?ub:a(makesequence(0,ub),contextptr);
        gen ret=makesequence(lb,ub);
        return retpos?ret:a(ret,contextptr);
    }
    // trim a real or complex vector
    if (a.type!=_VECT)
        return gentypeerr(contextptr);
    if (optstart>1)
        return gentoomanyargs(gettext("Too many arguments"),contextptr);
    thr=optstart?g._VECTptr->at(optstart):undef;
    if (!is_undef(thr) && (!is_real_number(thr,contextptr) || !is_strictly_positive(thr,contextptr)))
        return generr(gettext("Invalid threshold specification"));
    const_iterateur it0=a._VECTptr->begin(),it1=it0,itend=a._VECTptr->end(),it2=itend;
    try {
        if (dir<=0) for (;it1!=itend && iszero(*it1,thr,contextptr);++it1);
        if (dir>=0) {
            bool yes=true;
            while (it2!=it1 && (yes=iszero(*(--it2),thr,contextptr)));
            if (it2!=it1 || !yes) ++it2;
        }
    } catch (const std::runtime_error &err) {
        *logptr(contextptr) << err.what() << "\n";
        return gensizeerr(contextptr);
    }
    if (retpos) {
        int d=it1-it0,len=it2-it1;
        return dir<0?gen(d):(dir>0?gen(len):makesequence(d,len));
    }
    return vecteur(it1,it2);
}
static const char _trim_s[]="trim";
static define_unary_function_eval (__trim,&_trim,_trim_s);
define_unary_function_ptr5(at_trim,alias_at_trim,&__trim,0,true);

bool is_real_number(const gen &g,GIAC_CONTEXT) {
    if (g.type==_INT_ || g.type==_ZINT || g.type==_FLOAT_ || g.type==_DOUBLE_ || g.type==_REAL || g.type==_FRAC)
        return true;
    return evalf_double(g,1,contextptr).type==_DOUBLE_;
}

bool is_real_vector(const vecteur &v,GIAC_CONTEXT) {
    const_iterateur it=v.begin(),itend=v.end();
    for (;it!=itend;++it) {
        if (!is_real_number(*it,contextptr))
            return false;
    }
    return true;
}

gen to_real_number(const gen &g,GIAC_CONTEXT) {
    gen ret=_evalf(g,contextptr);
    if (ret.type!=_DOUBLE_)
        return evalf_double(ret,1,contextptr);
    return ret;
}

bool is_real_interval(const gen &g,gen &a,gen &b,GIAC_CONTEXT) {
    if (!g.is_symb_of_sommet(at_interval))
        return false;
    a=g._SYMBptr->feuille._VECTptr->front();
    b=g._SYMBptr->feuille._VECTptr->back();
    if (!is_real_number(a,contextptr) || !is_real_number(b,contextptr))
        return false;
    a=to_real_number(a,contextptr);
    b=to_real_number(b,contextptr);
    return true;
}

/* Return true iff G has a boolean value. */
bool is_logical(const gen &g) {
    return g.is_symb_of_sommet(at_and) || g.is_symb_of_sommet(at_ou) ||
        g.is_symb_of_sommet(at_superieur_egal) || g.is_symb_of_sommet(at_inferieur_egal) ||
        g.is_symb_of_sommet(at_superieur_strict) || g.is_symb_of_sommet(at_inferieur_strict) ||
        g.is_symb_of_sommet(at_same) || g.is_symb_of_sommet(at_equal);
}

/* Set LB and UB to the interval G endpoints.
 * The interval endpoints may also be specified as a list. */
bool get_range(const gen &g,gen &lb,gen &ub) {
    if (g.is_symb_of_sommet(at_interval)) {
        lb=g._SYMBptr->feuille._VECTptr->front();
        ub=g._SYMBptr->feuille._VECTptr->back();
    } else if (g.type==_VECT && g._VECTptr->size()==2 &&
            is_numericv(*g._VECTptr,num_mask_withint | num_mask_withfrac)) {
        lb=g._VECTptr->front();
        ub=g._VECTptr->back();
    } else return false;
    return true;
}

/* Return 1 if a<b, -1 if a>=b, and 0 if a and b are not comparable */
int compexpr(const gen &a,const gen &b,GIAC_CONTEXT) {
    gen res=_eval(symb_inferieur_strict(a,b),contextptr);
    if (res.is_integer() && res.subtype==_INT_BOOLEAN)
        return 2*res.val-1;
    return 0;
}

void count_func_instances_recursion(const gen &g,const unary_function_ptr *f,int &cnt,bool nested) {
    if (g.type==_VECT) {
        const_iterateur it=g._VECTptr->begin(),itend=g._VECTptr->end();
        for (;it!=itend;++it) count_func_instances_recursion(*it,f,cnt,nested);
        return;
    }
    if (g.is_symb_of_sommet(f)) {
        ++cnt;
        if (!nested) return;
    }
    if (g.type==_SYMB)
        count_func_instances_recursion(g._SYMBptr->feuille,f,cnt,nested);
}

int count_func_instances(const gen &g,const unary_function_ptr *f,bool nested=false) {
    int c=0;
    count_func_instances_recursion(g,f,c,nested);
    return c;
}

bool indomain(const matrice &intervals,const gen &a,GIAC_CONTEXT) {
    if (intervals.empty())
        return false;
    assert(ckmatrix(intervals) && mcols(intervals)==2);
    const_iterateur it=intervals.begin(),itend=intervals.end();
    for (;it!=itend;++it) {
        const vecteur &i=*it->_VECTptr;
        if (is_greater(a,i.front(),contextptr) && is_greater(i.back(),a,contextptr))
            return true;
    }
    return false;
}

/* Get the assumptions on identifier G: obtain the domain, feasible interval(s) and excluded points.
 * Return true if assumptions are read successfully, else return false. */
bool get_assumptions(const gen &g,int &dom,matrice &intervals,vecteur &excluded,GIAC_CONTEXT) {
    if (g.type!=_IDNT)
        return false;
    dom=-1;
    intervals.clear();
    excluded.clear();
    gen res=_about(g,contextptr);
    if (res.type==_IDNT && res==g) {
        intervals.push_back(makevecteur(minus_inf,plus_inf));
        return true; // no assumptions
    }
    if (res.type!=_VECT || res.subtype!=_ASSUME__VECT)
        return false;
    if (res._VECTptr->size()==1) {
        if (!res._VECTptr->front().is_integer())
            return false;
        dom=res._VECTptr->front().val;
        return true;
    }
    if (res._VECTptr->size()!=3)
        return false;
    if (res._VECTptr->front().is_integer())
        dom=res._VECTptr->front().val; // 1 - real, 2 - integer, 4 - complex, 10 - rational
    intervals=*res._VECTptr->at(1)._VECTptr;
    excluded=*res._VECTptr->at(2)._VECTptr;
    return true;
}

/* Set (additional) assumptions on identifier G: must be either inequality
 * constraints or bounds in form [L,U]. */
void set_assumptions(const gen &g,const vecteur &cond,const vecteur &excluded,bool additionally,GIAC_CONTEXT) {
    if (cond.empty())
        return;
    vecteur args;
    for (const_iterateur it=cond.begin();it!=cond.end();++it) {
        if (it->type==_VECT) {
            assert(it->_VECTptr->size()==2);
            const gen &lb=it->_VECTptr->front(),&ub=it->_VECTptr->back();
            gen l=is_inf(lb)?undef:(contains(excluded,lb)?symb_superieur_strict(g,lb):symb_superieur_egal(g,lb));
            gen r=is_inf(ub)?undef:(contains(excluded,ub)?symb_inferieur_strict(g,ub):symb_inferieur_egal(g,ub));
            if (!is_undef(l) && !is_undef(r))
                args.push_back(symb_and(l,r));
            else if (!is_undef(l))
                args.push_back(l);
            else if (!is_undef(r))
                args.push_back(r);
        } else args.push_back(*it);
    }
    gen a=symbolic(at_ou,change_subtype(args,_SEQ__VECT));
    if (additionally)
        giac_additionally(a,contextptr);
    else giac_assume(a,contextptr);
}

bool is_simpler(const gen &a,const gen &b) {
    return taille(a,MAX_TAILLE)<taille(b,MAX_TAILLE);
}

/* Simplify by applying ratnormal iteratively as far as it goes. */
gen ratsimp(const gen &g,GIAC_CONTEXT) {
    gen res(g),smp;
    int ta=taille(res,MAX_TAILLE),tb;
    while (true) {
        smp=ratnormal(res,contextptr);
        if ((tb=taille(smp,MAX_TAILLE))>=ta)
            break;
        res=smp;
        ta=tb;
    }
    return res;
}

gen sign2Heaviside(const gen &g) {
    if (g.type==_VECT) {
        vecteur res;
        res.reserve(g._VECTptr->size());
        for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
            res.push_back(sign2Heaviside(*it));
        }
        return gen(res,g.subtype);
    }
    if (g.is_symb_of_sommet(at_sign))
        return 2*symbolic(at_Heaviside,g._SYMBptr->feuille)-1;
    if (g.type==_SYMB)
        return symbolic(g._SYMBptr->sommet,sign2Heaviside(g._SYMBptr->feuille));
    return g;
}

gen Heaviside2sign(const gen &g) {
    if (g.type==_VECT) {
        vecteur res;
        res.reserve(g._VECTptr->size());
        for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
            res.push_back(Heaviside2sign(*it));
        }
        return gen(res,g.subtype);
    }
    if (g.is_symb_of_sommet(at_Heaviside))
        return (symbolic(at_sign,g._SYMBptr->feuille)+1)/2;
    if (g.type==_SYMB)
        return symbolic(g._SYMBptr->sommet,Heaviside2sign(g._SYMBptr->feuille));
    return g;
}

/* Convert Re(z) and Im(z) to (z+conj(z))/2 and (z-conj(z))/2, respectively. */
gen reim2conj(const gen &g) {
    if (g.type==_VECT) {
        vecteur res;
        res.reserve(g._VECTptr->size());
        const_iterateur it=g._VECTptr->begin(),itend=g._VECTptr->end();
        for (;it!=itend;++it) res.push_back(reim2conj(*it));
        return gen(res,g.subtype);
    }
    if (g.is_symb_of_sommet(at_re) || g.is_symb_of_sommet(at_im)) {
        gen z=g._SYMBptr->feuille,s=g.is_symb_of_sommet(at_re)?1:-1;
        return (z+s*symb_conj(z))/2;
    }
    if (g.type==_SYMB)
        return symbolic(g._SYMBptr->sommet,reim2conj(g._SYMBptr->feuille));
    return g;
}

bool is_const_wrt(const gen &g,const gen &x,GIAC_CONTEXT) {
    return is_constant_wrt(sign2Heaviside(g),x,contextptr) && !contains(*_lname(g,contextptr)._VECTptr,x);
}

bool is_integer_idnt(const gen &g,GIAC_CONTEXT) {
    if (g.type!=_IDNT)
        return false;
    matrice i;
    vecteur e;
    int d;
    return get_assumptions(g,d,i,e,contextptr) && d==2;
}

/* Test whether G necessarily has an integral value. */
bool is_inZ(const gen &g_orig,GIAC_CONTEXT) {
    gen g=ratnormal(g_orig,contextptr);
    if (g.is_integer() || is_integer_idnt(g,contextptr))
        return true;
    if (g.type!=_SYMB || !is_one(_denom(g,contextptr)))
        return false;
    const gen &ar=g._SYMBptr->feuille;
    if (g.is_symb_of_sommet(at_neg))
        return is_inZ(ar,contextptr);
    gen d;
    if (g.is_symb_of_sommet(at_pow) && ar.type==_VECT && (d=ar._VECTptr->back()).is_integer() && d.val>=0)
        return is_inZ(ar._VECTptr->front(),contextptr);
    if (g.is_symb_of_sommet(at_floor) || g.is_symb_of_sommet(at_ceil) ||
            g.is_symb_of_sommet(at_sign) || g.is_symb_of_sommet(at_Heaviside) ||
            g.is_symb_of_sommet(at_legendre_symbol) || g.is_symb_of_sommet(at_jacobi_symbol))
        return true;
    if ((g.is_symb_of_sommet(at_plus) || g.is_symb_of_sommet(at_prod)) && ar.type==_VECT) {
        const_iterateur it=ar._VECTptr->begin(),itend=ar._VECTptr->end();
        for (;it!=itend;++it) {
            if (!is_inZ(*it,contextptr))
                return false;
        }
        return true;
    }
    return false;
}

/* Does the expression g contain a symbolic subexpression with sommet f?
 * set a and rest to the argument of first found f and the multiplier of f(a), respectively. */
bool contains_f(const gen &g,const unary_function_ptr *f,gen &a,gen &rest,GIAC_CONTEXT) {
    bool ret=false;
    if (g.type!=_SYMB)
        return ret;
    if (g.is_symb_of_sommet(f)) {
        rest=1;
        a=g._SYMBptr->feuille;
        return true;
    }
    if (g.is_symb_of_sommet(at_prod)) {
        rest=1;
        vecteur fac=g._SYMBptr->feuille.type==_VECT?*g._SYMBptr->feuille._VECTptr:vecteur(1,g._SYMBptr->feuille);
        const_iterateur it=fac.begin(),itend=fac.end();
        for (;it!=itend;++it) {
            if (!ret && it->is_symb_of_sommet(f)) {
                a=it->_SYMBptr->feuille;
                ret=true;
            } else rest=*it*rest;
        }
    } else if (g.is_symb_of_sommet(at_neg)) {
        if (contains_f(g._SYMBptr->feuille,f,a,rest,contextptr)) {
            rest=-rest;
            return true;
        }
        return false;
    }
    return ret;
}

/* Find linear transformations of x appearing as arguments to other functions. */
void lintx(const gen &g,const identificateur &x,vecteur &lt,GIAC_CONTEXT) {
    gen a,b;
    if (g.type==_VECT) {
        const_iterateur it=g._VECTptr->begin(),itend=g._VECTptr->end();
        for (;it!=itend;++it) lintx(*it,x,lt,contextptr);
    } else if (is_linear_wrt(g,x,a,b,contextptr) && !is_zero(a) && is_zero(im(a,contextptr)) && is_zero(im(b,contextptr)))
        lt.push_back(makevecteur(a,b));
    else if (g.type==_SYMB)
        lintx(g._SYMBptr->feuille,x,lt,contextptr);
}

/* Rewrite a linear combination of instances of f such that the number of f-applications is minimal. */
gen collect_with_func(const gen &g_orig,const unary_function_ptr *f,gen_map *m,gen *rest,GIAC_CONTEXT) {
    bool with_m=m!=NULL;
    assert(!with_m || rest!=NULL);
    gen g=expand(g_orig,contextptr);
    vecteur terms;
    if (g.is_symb_of_sommet(at_plus))
        terms=g._SYMBptr->feuille.type==_VECT?*g._SYMBptr->feuille._VECTptr:vecteur(1,g._SYMBptr->feuille);
    else terms=vecteur(1,g);
    gen ret=0,a,r;
    gen_map ft;
    if (with_m) {
        *rest=0;
        m->clear();
    }
    gen_map &fterms=with_m?*m:ft;
    const_iterateur it=terms.begin(),itend=terms.end();
    for (;it!=itend;++it) {
        if (contains_f(*it,f,a,r,contextptr))
            fterms[a]+=r;
        else if (!with_m)
            ret+=*it;
        else *rest+=*it;
    }
    gen_map::const_iterator jt=fterms.begin(),jtend=fterms.end();
    if (!with_m)
        for (;jt!=jtend;++jt) ret+=ratnormal(jt->second,contextptr)*symbolic(f,jt->first);
    return with_m?0:ret;
}

gen simplify_floor(const gen &g,GIAC_CONTEXT) {
    if (g.type==_VECT) {
        vecteur res;
        res.reserve(g._VECTptr->size());
        const_iterateur it=g._VECTptr->begin(),itend=g._VECTptr->end();
        for (;it!=itend;++it) {
            res.push_back(simplify_floor(*it,contextptr));
        }
        return gen(res,g.subtype);
    }
    if (g.type!=_SYMB)
        return g;
    const gen &args=g._SYMBptr->feuille;
    if (g.is_symb_of_sommet(at_floor)) {
        gen e=expand(ratnormal(simplify_floor(args,contextptr),contextptr),contextptr),ar=0,rest=0,num,den;
        vecteur terms=e.is_symb_of_sommet(at_plus)?*e._SYMBptr->feuille._VECTptr:vecteur(1,e);
        const_iterateur it=terms.begin(),itend=terms.end();
        for (;it!=itend;++it) {
            if (is_inZ(*it,contextptr))
                rest+=*it;
            else if (is_real_number(*it,contextptr)) {
                gen f=_floor(_abs(*it,contextptr),contextptr),s=_sign(*it,contextptr);
                rest+=f*s;
                ar+=*it-f*s;
            } else ar+=*it;
        }
        ar=ratnormal(ar,contextptr);
        if (is_inZ(ar,contextptr))
            return ar+rest;
        if (ar.type==_FRAC)
            return rest+(is_positive(ar,contextptr)?symbolic(at_floor,ar):
                         -symbolic(at_floor,(_abs(ar._FRACptr->num,contextptr)-1)/_abs(ar._FRACptr->den,contextptr))-1);
        /* handle some cases of nesting */
        if ((den=_denom(ar,contextptr)).is_integer() && den.val>0) {
            num=expand(simplify_floor(_numer(ar,contextptr),contextptr),contextptr);
            gen a,b,c(undef);
            if (num.is_symb_of_sommet(at_floor))
                return rest+simplify_floor(_floor(num._SYMBptr->feuille/den,contextptr),contextptr);
            if (num.is_symb_of_sommet(at_neg) && (a=num._SYMBptr->feuille).is_symb_of_sommet(at_ceil))
                return rest+simplify_floor(_floor(-a._SYMBptr->feuille/den,contextptr),contextptr);
            if (num.is_symb_of_sommet(at_plus) && num._SYMBptr->feuille.type==_VECT) {
                const vecteur &terms=*num._SYMBptr->feuille._VECTptr;
                const_iterateur it=terms.begin(),itend=terms.end(),jt=itend;
                a=undef;
                for (;it!=itend;++it) {
                    if (it->is_symb_of_sommet(at_floor)) {
                        if (is_undef(a) || is_simpler(a,it->_SYMBptr->feuille)) {
                            a=it->_SYMBptr->feuille;
                            jt=it;
                        }
                    } else if (it->is_symb_of_sommet(at_neg) && (b=it->_SYMBptr->feuille).is_symb_of_sommet(at_ceil)) {
                        if (is_undef(a) || is_simpler(a,-b._SYMBptr->feuille)) {
                            a=-b._SYMBptr->feuille;
                            jt=it;
                        }
                    }
                }
                if (jt!=itend) {
                    vecteur t(terms);
                    t.erase(t.begin()+(jt-terms.begin()));
                    b=_sum(t,contextptr);
                    if (is_inZ(b,contextptr))
                        return rest+simplify_floor(_floor(ratnormal((a+b)/den,contextptr),contextptr),contextptr);
                }
            }
        }
        return is_zero(rest)?_floor(ar,contextptr):rest+simplify_floor(_floor(ar,contextptr),contextptr);
    }
    if (g.is_symb_of_sommet(at_ceil))
        return simplify_floor(-symbolic(at_floor,-args),contextptr);
    return symbolic(g._SYMBptr->sommet,simplify_floor(args,contextptr));
}

bool is_periodic_wrt(const gen &g_orig,const gen &x,gen &T,GIAC_CONTEXT) {
    gen g=ratnormal(g_orig,contextptr);
    if (is_const_wrt(g,x,contextptr))
        return true;
    gen f=_lin(trig2exp(g,contextptr),contextptr);
    vecteur v;
    lvar(f,v);
    bool hasx=find(v.begin(),v.end(),x)!=v.end();
    v.clear();
    rlvarx(f,x,v);
    islesscomplexthanf_sort(v.begin(),v.end());
    int i,s=int(v.size());
    if (s<2)
        return false;
    gen a,b,r,d,U;
    bool chk=false,upd;
    for (i=1;i<s && !is_inf(T);++i) {
        upd=false;
        if (v[i].is_symb_of_sommet(at_exp) && is_linear_wrt(v[i]._SYMBptr->feuille,x,a,b,contextptr) && !is_zero(a)) {
            U=ratnormal(cst_two_pi*cst_i/a,contextptr);
            if (!is_zero(ratnormal(im(U,contextptr),contextptr)))
                return false;
            U=_abs(U,contextptr);
            upd=true;
        } else if (v[i].is_symb_of_sommet(at_floor) && is_linear_wrt(v[i]._SYMBptr->feuille,x,a,b,contextptr) &&
                !is_zero(a) && is_const_wrt(b,x,contextptr)) {
            U=ratnormal(_inv(_abs(a,contextptr),contextptr));
            upd=true;
            chk=true;
        } else if (v[i].type==_SYMB && v[i]._SYMBptr->feuille.type!=_VECT) {
            if (!is_periodic_wrt(v[i]._SYMBptr->feuille,x,T,contextptr))
                return false;
        } else return false;
        if (upd) {
            if (is_undef(T))
                T=U;
            else {
                r=ratnormal(T/U,contextptr);
                if (!is_inZ(_numer(r,contextptr),contextptr) || !is_inZ(d=_denom(r,contextptr),contextptr))
                    return false;
                T=T*d;
            }
        }
    }
    if (chk) {
        gen s=simplify_floor(g-subst(g,x,x+T,false,contextptr),contextptr);
        return is_zero(simplify(s,contextptr)) || is_zero(_trigsimplify(s,contextptr));
    }
    return !hasx;
}

/* Return a period of the given expression if it is periodic with respect
 * to the given real variable, else return +inf.
 * If the returned value is zero, it means that any positive real number is
 * a period (e.g. for constant functions). */
gen _period(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    gen e,x=identificateur("x"),T(undef);
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        e=g;
    else {
        if (g._VECTptr->size()!=2)
            return gendimerr(contextptr);
        if ((x=g._VECTptr->back()).type!=_IDNT)
            return gentypeerr(contextptr);
        e=g._VECTptr->front();
    }
    e=simplify_floor(recursive_normal(e,contextptr),contextptr);
    if (!is_periodic_wrt(e,x,T,contextptr))
        return plus_inf;
    return is_undef(T)?gen(0):T;
}
static const char _period_s[]="period";
static define_unary_function_eval (__period,&_period,_period_s);
define_unary_function_ptr5(at_period,alias_at_period,&__period,0,true);

gen _simplifyFloor(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    return simplify_floor(recursive_normal(g,contextptr),contextptr);
}
static const char _simplifyFloor_s[]="simplifyFloor";
static define_unary_function_eval (__simplifyFloor,&_simplifyFloor,_simplifyFloor_s);
define_unary_function_ptr5(at_simplifyFloor,alias_at_simplifyFloor,&__simplifyFloor,0,true);

matrice stft(const vecteur &x,int m,const unary_function_ptr *wf,GIAC_CONTEXT) {
    int n=x.size(),i,j;
    assert(n%m==0);
    int N=n/m-1; // number of DFTs
    matrice res;
    res.reserve(N);
    vecteur v(m),a(m),b(m);
    const_iterateur it;
    iterateur jt,ait,bit;
    for (i=0,ait=a.begin(),bit=b.begin();i<m;++i,++ait,++bit) {
        gen w=cst_i*exp(-i*cst_i*(cst_two_pi/(2.0*m)),contextptr);
        *ait=(1.0-w)/2.0;
        *bit=(1.0+w)/2.0;
    }
    for (i=0;i<N;++i) {
        it=x.begin()+i*m;
        jt=v.begin();
        for (j=0;j<m;++j,it+=2,++jt)
            *jt=gen(*it,*(it+1));
        vecteur u=*_fft(_eval(symbolic(wf,v),contextptr),contextptr)._VECTptr,r;
        r.reserve(m);
        for (j=0,ait=a.begin(),bit=b.begin();j<m;++j,++jt,++ait,++bit)
            r.push_back(*ait*u[j]+*bit*conj(u[(m-j)%m],contextptr));
        res.push_back(r);
    }
    return res;
}

vecteur istft(const matrice &y,GIAC_CONTEXT) {
    int N=mrows(y),m=mcols(y),n=(N+1)*m,i,j;
    vecteur x(n,0),a(m),b(m),c(m);
    const_iterateur it,ut;
    iterateur jt,ait,bit;
    for (i=0,ait=a.begin(),bit=b.begin();i<m;++i,++ait,++bit) {
        gen w=cst_i*exp(i*cst_i*(cst_two_pi/(2.0*m)),contextptr);
        *ait=(1.0+w)/2.0;
        *bit=(1.0-w)/2.0;
    }
    for (i=0,it=y.begin();i<N;++i,++it) {
        const vecteur &r=*it->_VECTptr;
        for (j=0,jt=c.begin(),ait=a.begin(),bit=b.begin();j<m;++j,++jt,++ait,++bit)
            *jt=*ait*r[j]+*bit*conj(r[(m-j)%m],contextptr);
        vecteur u=*_ifft(c,contextptr)._VECTptr;
        for (j=0,ut=u.begin(),jt=x.begin()+i*m;j<m;++j,++ut) {
            *(jt++)+=re(*ut,contextptr);
            *(jt++)+=im(*ut,contextptr);
        }
    }
    return x;
}

gen _stft(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->empty())
        return gendimerr(contextptr);
    gen wsize=128,wf=at_hamming_window;
    bool has_opts=g.subtype==_SEQ__VECT;
    if (has_opts && g._VECTptr->size()>3)
        return gentoomanyargs(gettext("Too many input arguments"));
    if (has_opts && g._VECTptr->front().type!=_VECT)
        return gentypeerr(contextptr);
    int n=g._VECTptr->size()-1,m;
    if (has_opts && n==2) {
        wf=g._VECTptr->at(n--);
        if (wf.type!=_FUNC)
            return generrtype(gettext("Expected a window function"));
    }
    if (has_opts && n==1) {
        wsize=g._VECTptr->at(n);
        if (!wsize.is_integer() || wsize.val<1)
            return generrtype(gettext("Expected a positive integer"));
        int e=std::floor(mylog2(wsize.val)+0.5);
        if ((1<<e)!=wsize.val)
            return generr(gettext("Window size must be a power of 2"));
    }
    vecteur x=has_opts?*g._VECTptr->front()._VECTptr:*g._VECTptr;
    if (x.empty())
        return gendimerr(contextptr);
    if (!is_numericv(x,num_mask_withfrac | num_mask_withint))
        return gensizeerr(contextptr);
    if (!is_zero__VECT(*im(x,contextptr)._VECTptr,contextptr)) {
        print_warning(gettext("ignoring imaginary part of input"),contextptr);
        x=*re(x,contextptr)._VECTptr;
    }
    n=x.size();
    m=wsize.val;
    if (n%m) x.resize((n/m+1)*m,0);
    return stft(x,m,wf._FUNCptr,contextptr);
}
static const char _stft_s[]="stft";
static define_unary_function_eval (__stft,&_stft,_stft_s);
define_unary_function_ptr5(at_stft,alias_at_stft,&__stft,0,true);

gen _istft(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (!ckmatrix(g))
        return gentypeerr(contextptr);
    const matrice &y=*g._VECTptr;
    if (!is_numericm(y,num_mask_withfrac | num_mask_withint))
        return gensizeerr(contextptr);
    return istft(y,contextptr);
}
static const char _istft_s[]="istft";
static define_unary_function_eval (__istft,&_istft,_istft_s);
define_unary_function_ptr5(at_istft,alias_at_istft,&__istft,0,true);

//
// AUDIO CLIP CLASS IMPLEMENTATION
//

audio_clip::audio_clip(int depth,int rate,int nc,int len,GIAC_CONTEXT) {
    if ((depth!=8 && depth!=16 && depth!=24) || len<1 || nc<1 || rate<1)
        throw std::runtime_error(gettext("Invalid sound parameters"));
    _bit_depth=depth;
    _pow2=1<<_bit_depth;
    _range=_pow2/2-1;
    _nc=nc;
    _len=len;
    _size=len*nc*depth/8;
    _sample_rate=rate;
    _data=(char*)calloc(_size,sizeof(char));
    if (_data==NULL)
        throw std::runtime_error(gettext("Failed to allocate memory for wave data"));
    ctx=contextptr;
    _filename.clear();
    _file_offset=0;
}
audio_clip::audio_clip(const audio_clip &other,int offset,int len) {
    _size=0;
    _data=NULL;
    assign(other,offset,len);
}
audio_clip::audio_clip(const char *fname,int offset,int len,GIAC_CONTEXT) {
    ctx=contextptr;
    FILE *f=fopen(fname,"r");
    if (f==NULL)
        throw std::runtime_error(gettext("Failed to open file for reading"));
    _size=0;
    try {
        read(f,offset,len,true);
        _filename=fname;
    } catch (const std::runtime_error &e) {
        fclose(f);
        if (_size>0)
            free(_data);
        throw e;
    }
    fclose(f);
}
audio_clip::audio_clip(const char *fname,double t1,double t2,GIAC_CONTEXT) {
    ctx=contextptr;
    FILE *f=fopen(fname,"r");
    if (f==NULL)
        throw std::runtime_error(gettext("Failed to open file for reading"));
    _size=0;
    int offset,len;
    try {
        read_header(f,_bit_depth,_sample_rate,_nc,_size);
        offset=(int)std::floor(t1*_sample_rate);
        len=(int)std::floor((t2-t1)*_sample_rate);
        read(f,offset,len,false);
        _filename=fname;
    } catch (const std::runtime_error &e) {
        fclose(f);
        if (_size>0)
            free(_data);
        throw e;
    }
    fclose(f);
}
audio_clip *audio_clip::from_gen(const gen &g) {
    if (g.type!=_USER)
        return NULL;
    return dynamic_cast<audio_clip*>(g._USERptr);
}
gen audio_clip::giac_constructor(GIAC_CONTEXT) const {
    if (!_filename.empty())
        return symbolic(at_readwav,makesequence(string2gen(_filename,false),_file_offset,_len));
    vecteur args;
    string data;
    data.reserve(_size*2);
    for (int i=0;i<_size;++i)
        data.append(n2hexstr(_data[i]));
    args.push_back(symb_equal(at_channels,_nc));
    args.push_back(symb_equal(at_bit_depth,_bit_depth));
    args.push_back(symb_equal(at_samplerate,_sample_rate));
    args.push_back(symb_equal(at_channel_data,string2gen(data,false)));
    return symbolic(at_createwav,gen(args,_SEQ__VECT));
}
void audio_clip::assign(const audio_clip &other,int offset,int len) {
    _bit_depth=other.bit_depth();
    _nc=other.channel_count();
    if (offset>other.length()-len)
        len=other.length()-offset;
    _len=len;
    _sample_rate=other.sample_rate();
    _pow2=other._pow2;
    _range=other._range;
    if (other._filename.size()>0) {
        _filename=other._filename;
        _file_offset=other._file_offset+offset;
    } else _filename.clear();
    ctx=other.ctx;
    int m=_nc*_bit_depth/8,sz=len*m;
    if (_size!=sz) {
        char *d=(char*)calloc(sz,sizeof(char));
        if (d!=NULL) {
            if (_data!=NULL) free(_data);
            _data=d;
            _size=sz;
        } else throw std::runtime_error(gettext("Failed to allocate memory for wave data"));
    }
    memcpy(_data,other._data+offset*m,_size);
}
gen audio_clip::operator+(const gen &g) const {
    audio_clip *other=from_gen(g);
    if (other==NULL)
        return generrtype(gettext("Expected an audio clip"));
    if (_bit_depth!=other->bit_depth())
        return generr(gettext("Audio clips must have the same bit depth"));
    if (_sample_rate!=other->sample_rate())
        return generr(gettext("Audio clips must have the same sample rate"));
    if (_nc!=other->channel_count())
        return generr(gettext("Audio clips must have the same number of channels"));
    audio_clip ret(_bit_depth,_sample_rate,_nc,_len+other->length(),ctx);
    memcpy(ret._data,this->_data,this->_size);
    memcpy(ret._data+this->_size,other->_data,other->_size);
    return ret;
}
string audio_clip::print(GIAC_CONTEXT) const {
    char buf[1024];
    sprintf(buf,gettext("a sound clip with %d samples at %d Hz (%d bit, %s)"),_len,_sample_rate,_bit_depth,(_nc==1?"mono":"stereo"));
    return buf;
}
string audio_clip::texprint(GIAC_CONTEXT) const {
    string ret="\\text{";
    ret+=this->print(contextptr);
    ret+"}";
    return ret;
}
/* Return true iff G is audio clip with the same parameters and data as this. */
bool audio_clip::operator==(const gen &g) const {
    audio_clip *other=from_gen(g);
    if (other==NULL)
        return false;
    if (_bit_depth!=other->bit_depth() || _sample_rate!=other->sample_rate() ||
            _nc!=other->channel_count() || _len!=other->length() || _size!=other->_size)
        return false;
    for (int i=0;i<_size;++i) {
        if (_data[i]!=other->_data[i])
            return false;
    }
    return true;
}
/* Copy the segment beginning at POS with length LEN (by default unlimited)
 * and return the selection as a new audio clip. */
gen audio_clip::operator()(const gen &g,GIAC_CONTEXT) const {
    int len=rand_max2,pos=0;
    gen a,b;
    if (g.is_integer())
        pos=g.val;
    else if (is_real_interval(g,a,b,contextptr)) {
        pos=(int)std::floor(0.5+_sample_rate*to_real_number(a,contextptr).to_double(contextptr));
        len=(int)std::floor(0.5+_sample_rate*to_real_number(b-a,contextptr).to_double(contextptr));
    } else if (g.type==_VECT && g.subtype==_SEQ__VECT && g._VECTptr->size()==2 &&
            g._VECTptr->front().is_integer() && g._VECTptr->back().is_integer()) {
        pos=g._VECTptr->front().val;
        len=g._VECTptr->back().val;
    } else return generr(gettext("Invalid argument"));
    if (pos<0 || pos>=_len)
        return generr(gettext("Invalid selection offset"));
    if (len<1)
        return generr(gettext("Invalid selection length"));
    return audio_clip(*this,pos,len);
}
/* Get individual samples or blocks of data on a channel.
 * Call with pair c,offset or triple c,offset,len where c
 * is the channel index (zero-based).
 * It is allowed to enter 'left' or 'right' instead of c. */
gen audio_clip::operator[](const gen &g) {
    int chn=-1,pos=0,len=rand_max2;
    gen a,b,c;
    if (g.type!=_VECT)
        c=g;
    else {
        if (g.subtype!=_SEQ__VECT)
            return gentypeerr(ctx);
        const vecteur &args=*g._VECTptr;
        if (args.size()<2 || args.size()>3)
            return gendimerr(ctx);
        c=args.front();
        if (args.size()==2 && is_real_interval(args.back(),a,b,ctx)) {
            pos=(int)std::floor(0.5+_sample_rate*a.to_double(ctx));
            len=(int)std::floor(0.5+_sample_rate*(b-a).to_double(ctx));
        } else {
            if (!is_integer_vecteur(vecteur(args.begin()+1,args.end())))
                return generrtype(gettext("Invalid range"));
            pos=args[1].val;
            if (args.size()==3)
                len=args[2].val;
        }
    }
    if (c==at_left) chn=0;
    else if (c==at_right) chn=1;
    else if (c.is_integer())
        chn=c.val;
    else return generrtype(gettext("Invalid channel specification"));
    if (chn>=_nc || chn<0)
        return generr(gettext("Invalid channel index"));
    if (pos<0 || pos>=_len)
        return generr(gettext("Invalid offset"));
    if (len<=0)
        return generr(gettext("Length should be positive"));
    if (pos>_len-len)
        len=_len-pos;
    vecteur ret;
    ret.reserve(len);
    for (int i=0;i<len;++i)
        ret.push_back(decode(get_sample(chn,pos+i)));
    return ret;
}
/* Load the wave data from the specified string and offset. */
void audio_clip::load_data(const string &s,int offset) {
    int sz=s.size()/2,len=(8*sz)/(_nc*_bit_depth),p0=offset*_nc*_bit_depth/8,j;
    for (int i=0;i<sz;++i) {
        j=p0+i;
        if (j<0) continue;
        else if (j>=_size) break;
        uchar c=static_cast<uchar>(strtol(s.substr(2*i,2).c_str(),NULL,16));
        _data[j]=static_cast<char>(c);
    }
    _filename.clear();
}
/* Set the sample at position POS of the channel C. */
void audio_clip::set_sample(int chn,int pos,unsigned u) {
    if (chn<0 || chn>=_nc || pos<0 || pos>=_len)
        return;
    int i=pos*_nc+chn;
    switch (_bit_depth) {
    case 8:
        _data[i]=u;
        break;
    case 16:
        _data[2*i]=u & 0xff;
        _data[2*i+1]=(u>>8) & 0xff;
        break;
    case 24:
        _data[3*i]=u & 0xff;
        _data[3*i+1]=(u>>8) & 0xff;
        _data[3*i+2]=(u>>16) & 0xff;
        break;
    }
}
/* Return RMS value of (a portion of) this clip. */
double audio_clip::rms(int offset,int len) const {
    int l=offset>_len-len?_len-offset:len;
    double s,tot=0;
    vector<double> chn_rms(_nc,0);
    for (int c=0;c<_nc;++c) {
        double &cr=chn_rms[c];
        for (int i=0;i<l;++i) {
            s=decode(get_sample(c,i+offset));
            cr+=s*s;
        }
        cr/=double(l);
    }
    for (int c=0;c<_nc;++c)
        tot+=chn_rms[c];
    return std::sqrt(tot);
}
/* Return the cosine similarity of chunks in this and other of
 * length LEN starting at specified offsets.
 * In case of multichannel clips, the geometric mean of channel
 * similarities is returned. */
double audio_clip::similarity(const audio_clip &other,int offset,int other_offset,int len) const {
    if (offset<0 || offset>=_len || other_offset<0 || other_offset>=other._len || len<=0)
        throw std::runtime_error(gettext("Invalid chunk position and/or length specification"));
    if (other.bit_depth()!=_bit_depth || other.sample_rate()!=_sample_rate || other.channel_count()!=_nc)
        throw std::runtime_error(gettext("Clips should have the same bit depths, sample rates and channel counts"));
    int l=std::min(len,std::min(_len-offset,other._len-other_offset));
    double p=1.0,a,b;
    for (int c=0;c<_nc;++c) {
        double pp=0.0,norm_a=0.0,norm_b=0.0;
        for (int i=0;i<l;++i) {
            a=decode(get_sample(c,i+offset));
            b=other.decode(other.get_sample(c,i+other_offset));
            pp+=a*b;
            norm_a+=a*a;
            norm_b+=b*b;
        }
        p*=pp/std::sqrt(norm_a*norm_b);
    }
    return std::pow(p,1.0/double(_nc));
}
/* Pad this audio with LEN samples of silence at the end. */
void audio_clip::pad_zeros(int len) {
    int newlen=_len+len,newsize=_size+len*_nc*_bit_depth/8;
    char *newdata=(char*)realloc(_data,newsize*sizeof(char));
    if (newdata==NULL)
        throw std::runtime_error(gettext("Failed to resize audio"));
    _data=newdata;
    for (int i=_size;i<newsize;++i) _data[i]=0;
    _size=newsize;
    _len=newlen;
}
/* Mix another audio clip in.
 * OFFSET, FADE_IN and FADE_OUT are in samples, gain is in dB. 
 * Return true if no clipping has occurred, else return false. */
bool audio_clip::mix(const audio_clip &other,int offset,double gain,double pan,int fade_in,int fade_out) {
    int onc=other.channel_count();
    if (_nc>2 || (onc>1 && onc!=_nc) || other.sample_rate()!=_sample_rate)
        throw std::runtime_error(gettext("Incompatible sound parameters"));
    double ratio=std::pow(10.0,gain/20.0),r,s1,s2,s,theta=M_PI*(pan+1.0)/4.0,pl=std::cos(theta),pr=std::sin(theta);
    int len=other.length();
    if (offset+len>_len) {
        try { pad_zeros(offset+len-_len); } catch (const std::runtime_error &e) { throw e; }
    }
    bool clipping=false;
    if (_nc==1) pl=1.0; // can't pan in mono
    for (int i=0;i<len;++i) {
        r=ratio;
        if (i<fade_in)
            r*=std::pow(std::cos(M_PI_2*(1.0-double(i)/double(fade_in))),2);
        if (i>=len-fade_out)
            r*=std::pow(std::cos(M_PI_2*(1.0-double(len-i-1)/double(fade_out))),2);
        for (int c=0;c<_nc;++c) {
            s1=decode(get_sample(c,i+offset));
            s2=(c==0?pl:pr)*r*other.decode(other.get_sample(std::min(onc-1,c),i));
            s=s1+s2;
            clipping=s>1.0 || s< -1.0;
            set_sample(c,i+offset,encode(s));
        }
    }
    return !clipping;
}
/* Splice two audio clips at position POS in this clip.
 * Use EX<0 for auto crossfade, EX=1.0 for constant gain and
 * EX=0.5 for constant power crossfade. */
audio_clip audio_clip::splice(const audio_clip &other,int pos,double ex) const {
    if (pos<0 || pos>=_len)
        throw std::runtime_error(gettext("Invalid splice position"));
    double e=ex<0?0.5+0.5*std::abs(similarity(other,pos)):ex,a,b,cf,v;
    audio_clip ret(_bit_depth,_sample_rate,_nc,pos+other._len,ctx);
    int cflen=std::min(_len-pos,other._len),s=_nc*_bit_depth/8;
    pos=_len-cflen;
    memcpy(ret._data,this->_data,pos*s);
    memcpy(ret._data+_len*s,other._data+cflen*s,(other._len-cflen)*s);
    bool clipping=false;
    // crossfade
    for (int c=0;c<_nc;++c) {
        for (int i=0;i<cflen;++i) {
            a=decode(get_sample(c,i+pos));
            b=other.decode(other.get_sample(c,i));
            if (e==0) {
                clipping=std::abs(v=a+b)>1.0;
            } else {
                cf=std::pow(double(i+1)/double(cflen),e);
                clipping=std::abs(v=a*(1.0-cf)+b*cf)>1.0;
            }
            ret.set_sample(c,i+pos,ret.encode(v));
        }
    }
    if (clipping)
        *logptr(ctx) << gettext("Warning") << ": " << gettext("clipping audio, the result will be distorted") << "\n";
    return ret;
}
/* Return this clip with channels mixed down to one. */
audio_clip audio_clip::to_mono() const {
    audio_clip ret(_bit_depth,_sample_rate,1,_len,ctx);
    for (int i=0;i<_len;++i) {
        double tot=0.0;
        for (int c=0;c<_nc;++c)
            tot+=decode(get_sample(c,i));
        ret.set_sample(0,i,ret.encode(tot/_nc));
    }
    return ret;
}
/* Reverse all channels in the clip. */
audio_clip audio_clip::reverse() const {
    audio_clip ret(_bit_depth,_sample_rate,_nc,_len,ctx);
    for (int c=0;c<_nc;++c)
        for (int i=0;i<_len;++i)
            ret.set_sample(c,i,get_sample(c,_len-i-1));
    return ret;
}
/* Find audio peak in [0,1]. */
double audio_clip::peak() const {
    double ret=0.0;
    for (int c=0;c<_nc;++c)
        for (int i=0;i<_len;++i)
            ret=std::max(ret,decode(get_sample(c,i)));
    return ret;
}
/* Normalize the audio to the specified dBFS level. */
void audio_clip::normalize(double dbfs) {
    double s=std::pow(10.0,dbfs/20.0)/peak();
    for (int c=0;c<_nc;++c)
        for (int i=0;i<_len;++i)
            set_sample(c,i,encode(s*decode(get_sample(c,i))));
}
/* Return the sample at position POS of the channel C. */
unsigned audio_clip::get_sample(int c,int pos) const {
    if (c<0 || c>=_nc || pos<0 || pos>=_len)
        return 0;
    int i=pos*_nc+c;
    unsigned v,d1,d2,d3;
    switch (_bit_depth) {
    case 8:
        v=static_cast<uchar>(_data[i]);
        break;
    case 16:
        d1=static_cast<uchar>(_data[2*i]);
        d2=static_cast<uchar>(_data[2*i+1]);
        v=d1 | (d2<<8);
        break;
    case 24:
        d1=static_cast<uchar>(_data[3*i]);
        d2=static_cast<uchar>(_data[3*i+1]);
        d3=static_cast<uchar>(_data[3*i+2]);
        v=d1 | (d2<<8) | (d3<<16);
        break;
    default:
        throw std::runtime_error(gettext("Invalid bit depth"));
    }
    return v;
}
/* Return a copy of this clip with all channels inverted. */
gen audio_clip::inv() const {
    audio_clip clip(*this);
    unsigned p=1<<_bit_depth;
    for (int c=0;c<_nc;++c)
        for (int i=0;i<_len;++i)
            clip.set_sample(c,i,p-get_sample(c,i));
    return clip;
}
/* Decode the sample, return a value in [-1,1]. */
double audio_clip::decode(int sample) const {
    int v=sample;
    if (_bit_depth==8) v-=127;
    else if (2*v>=_pow2) v-=_pow2;
    return double(2*v)/double(_pow2);
}
/* Encode a signal value SAMPLE in [-1,1]. Clipping will occur if |SAMPLE| > 1. */
int audio_clip::encode(double sample) const {
    double v;
    if (sample< -1.0 || sample>1.0)
        v=std::max(-1.0,std::min(1.0,sample));
    else v=sample;
    int k=(int)std::floor(_range*v+0.5);
    if (_bit_depth==8) k+=127;
    //else if (k<0) k+=_pow2;
    return k;
}
#ifdef RTOS_THREADX
void audio_clip::read_header(FILE *f,int &bd,int &sr,int &nc,int &sz) {
    bd=sr=nc=sz=0;
}
void audio_clip::read(FILE *f,int offset,int len) {
    throw std::runtime_error(gettext("Loading wave files is not supported"));
}
bool audio_clip::write(FILE *f) const {
    return false;
}
gen _readwav(const gen &g,GIAC_CONTEXT){
    return generr(gettext("Loading wave files is not supported"));
}
gen _writewav(const gen &g,GIAC_CONTEXT){
    return 0;
}
#else
void audio_clip::read_header(FILE *f,int &bd,int &sr,int &nc,int &sz) {
    uchar c,channels;
    unsigned int u,s,sample_rate,byte_rate,block_align=0,bits_per_sample=0,data_size=0;
    if (fread(&u,4,1,f)!=1 || u!=0x46464952 || // "RIFF"
            fread(&s,4,1,f)!=1 ||
            fread(&u,4,1,f)!=1 || u!=0x45564157 || // "WAVE"
            fread(&u,4,1,f)!=1 || u!=0x20746d66 || // "fmt "
            fread(&u,4,1,f)!=1 || u!=0x10) // 16 for PCM
        throw std::runtime_error(gettext("Failed to read wave file header"));
    c=fgetc(f);
    if (c!=1)
        throw std::runtime_error(gettext("Badly formatted wave file"));
    c=fgetc(f);
    if (c!=0)
        throw std::runtime_error(gettext("Badly formatted wave file"));
    channels=fgetc(f);
    c=fgetc(f);
    if (c!=0 || fread(&sample_rate,4,1,f)!=1 || fread(&byte_rate,4,1,f)!=1)
        throw std::runtime_error(gettext("Failed to read wave file parameters"));
    block_align=fgetc(f);
    block_align=block_align+(fgetc(f)<<8);
    bits_per_sample=fgetc(f); // 8 or 16
    bits_per_sample=bits_per_sample+(fgetc(f)<<8);
    if (fread(&u,4,1,f)!=1 || u!=0x61746164 || fread(&data_size,4,1,f)!=1)
        throw std::runtime_error(gettext("Failed to read wave data header"));
    bd=bits_per_sample;
    sr=sample_rate;
    nc=channels;
    sz=data_size;
}
/* Read audio from a wave file F.
 * Source: http://ccrma.stanford.edu/courses/422/projects/WaveFormat/ */
void audio_clip::read(FILE *f,int offset,int len,bool read_hdr) {
    unsigned u;
    int n,pos=0,nsamp,bytes_per_sample;
    if (read_hdr) {
        read_header(f,_bit_depth,_sample_rate,_nc,n);
        if (_bit_depth>24)
            throw std::runtime_error(gettext("Bit depth too high"));
    } else n=_size;
    bytes_per_sample=_bit_depth/8;
    nsamp=n/(_nc*bytes_per_sample);
    if (offset<0 || offset>=nsamp)
        throw std::runtime_error(gettext("Invalid offset"));
    if (offset>nsamp-len)
        len=nsamp-offset;
    if (len<=0)
        throw std::runtime_error(gettext("Invalid length"));
    _pow2=(1<<_bit_depth);
    _range=_pow2/2-1;
    _len=len;
    _size=_len*_nc*bytes_per_sample;
    _data=(char*)calloc(_size,sizeof(char));
    if (_data==NULL)
        throw std::runtime_error(gettext("Failed to allocate memory for wave data"));
    if (fseek(f,offset*bytes_per_sample*_nc,SEEK_CUR))
        throw std::runtime_error(gettext("Failed to read wave data"));
    for (;pos<_len&&!feof(f);++pos) {
        for (int i=0;i<_nc;++i) {
            u=0;
            if (fread(&u,bytes_per_sample,1,f)!=1)
                throw std::runtime_error(gettext("Failed to read wave data"));
            set_sample(i,pos,u);
        }
    }
    _file_offset=offset;
}
/* Write audio to file F in wave format. */
bool audio_clip::write(FILE *f) const {
    unsigned int u,byte_rate,block_align=0;
    u=0x46464952;
    if (fwrite(&u,4,1,f)!=1)
        return false;
    u=36+_size;
    if (fwrite(&u,4,1,f)!=1)
        return false;    
    u=0x45564157;
    if (fwrite(&u,4,1,f)!=1)
        return false;
    u=0x20746d66;
    if (fwrite(&u,4,1,f)!=1)
        return false;
    u=0x10;
    if (fwrite(&u,4,1,f)!=1)
        return false;
    fputc(1,f);
    fputc(0,f);
    fputc(_nc,f);
    fputc(0,f);
    block_align=_nc*_bit_depth/8;
    byte_rate=_sample_rate*block_align;
    if (fwrite(&_sample_rate,4,1,f)!=1 || fwrite(&byte_rate,4,1,f)!=1 ||
            fwrite(&block_align,2,1,f)!=1 || fwrite(&_bit_depth,2,1,f)!=1)
        return false;
    u=0x61746164; // "data"
    if (fwrite(&u,4,1,f)!=1)
        return false;
    u=_size;
    if (fwrite(&u,4,1,f)!=1)
        return false;
    // write data
    unsigned bytes_per_sample=_bit_depth/8;
    for (int i=0;i<_len;++i){
        for (int j=0;j<_nc;++j){
            u=get_sample(j,i);
            if (fwrite(&u,bytes_per_sample,1,f)!=1)
                return false;
        }
    }
    return true;
}

gen _readwav(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    const gen &fname=(g.type==_VECT && !g._VECTptr->empty())?g._VECTptr->front():g;
    if (fname.type!=_STRNG)
        return generrtype(gettext("Expected a file name"));
    int offset=0,len=rand_max2;
    double t1,t2;
    bool has_range=false;
    if (g.type==_VECT) {
        if (g.subtype!=_SEQ__VECT)
            return gentypeerr(contextptr);
        const vecteur &gv=*g._VECTptr;
        if (gv.size()<2 || gv.size()>3)
            return generrdim(gettext("Wrong number of arguments"));
        gen a,b;
        if (is_real_interval(gv[1],a,b,contextptr)) {
            t1=a.to_double(contextptr);
            t2=b.to_double(contextptr);
            has_range=true;
            if (gv.size()>2)
                return generrdim(gettext("Too many input arguments"));
        } else if (gv[1].is_integer()) {
            offset=gv[1].val;
            if (gv.size()>2) {
                if (gv[2].is_integer())
                    len=gv[2].val;
                else return generrtype(gettext("Expected a length in samples"));
            }
        } else return generrtype(gettext("Expected an offset or a range"));
    }
    try {
        if (has_range)
            return audio_clip(fname._STRNGptr->c_str(),t1,t2,contextptr);
        return audio_clip(fname._STRNGptr->c_str(),offset,len,contextptr);
    } catch (const std::runtime_error &e) {
        return generr(e.what());
    }
}

gen _writewav(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return generrdim(gettext("Wrong number of arguments"));
    if (gv.front().type!=_STRNG)
        return generrtype(gettext("Expected a file name"));
    audio_clip *clip=audio_clip::from_gen(gv.back());
    if (clip==NULL)
        return generrtype(_SP_BAD_SOUND_DATA);
    bool res=clip->write_wav(gv.front()._STRNGptr->c_str());
    return (int)res;
}
#endif

static const char _readwav_s []="readwav";
static define_unary_function_eval (__readwav,&_readwav,_readwav_s);
define_unary_function_ptr5( at_readwav,alias_at_readwav,&__readwav,0,true);

static const char _writewav_s []="writewav";
static define_unary_function_eval (__writewav,&_writewav,_writewav_s);
define_unary_function_ptr5( at_writewav,alias_at_writewav,&__writewav,0,true);

bool audio_clip::write_wav(const char *fname) const {
    FILE *f=fopen(fname,"w");
    if (f==NULL)
        return false;
    bool res=write(f);
    fclose(f);
    return res;
}

void audio_clip::set_channel_data(int chn,const vecteur &data,int offset,double ratio) {
    if (chn<0 || chn>=_nc)
        throw std::runtime_error(gettext("Invalid channel number"));
    int len=data.size(),pos;
    if (offset>=_len)
        return;
    else if (offset>_len-len)
        len=_len-offset;
    const_iterateur it=data.begin();
    gen v;
    for (pos=0;pos<len;++it,++pos) {
        v=evalf_double(*it,1,ctx);
        if (v.type!=_DOUBLE_) {
            char buf[1024];
            sprintf(buf,gettext("Failed to set channel data, %s is not a valid sample value"),it->print(ctx).c_str());
            throw std::runtime_error(buf);
        }
        set_sample(chn,offset+pos,encode(ratio*v.DOUBLE_val()));
    }
    _filename.clear();
}

void audio_clip::set_block(const matrice &data,int offset) {
    const_iterateur it=data.begin(),itend=data.end();
    int chn=0;
    for (;it!=itend;++it,++chn)
        set_channel_data(chn,*it->_VECTptr,offset);
    _filename.clear();
}

vecteur audio_clip::get_channel_data(int chn,int pos,int len) const {
    vecteur ret;
    if (pos<_len && chn>=0 && chn<_nc) {
        if (pos>_len-len)
            len=_len-pos;
        ret.reserve(len);
        for (int i=0;i<len;++i) {
            ret.push_back(decode(get_sample(chn,pos+i)));
        }
    }
    return ret;
}

matrice audio_clip::get_block(int pos,int len) const {
    matrice ret;
    ret.reserve(_nc);
    for (int chn=0;chn<_nc;++chn) {
        ret.push_back(get_channel_data(chn,pos,len));
    }
    return ret;
}

/* Play at most DUR samples starting from POS (and repeat the indicated number of times).
 * Return 0 on success, 1 on audio device error, 2 on playback error, 3 on libao missing. */
#ifdef HAVE_LIBAO
int audio_clip::play(int pos,int len,int repeats) const {
    ao_device *device=0;
    ao_sample_format format;
    memset(&format,0,sizeof(format));
    format.bits=_bit_depth;
    format.channels=_nc;
    format.rate=_sample_rate;
    format.byte_format=AO_FMT_LITTLE;
    int default_driver,block_size=_nc*_bit_depth/8,sz,offset,N=_sample_rate/20,ret=0;
    ao_initialize();
    default_driver=ao_default_driver_id();
    device=ao_open_live(default_driver,&format,NULL /* no options */);
    if (device==NULL) 
        return 1; // error opening audio device
    if (pos>_len-len)
        len=_len-pos;
    for (int r=0;r<repeats;++r) {
        offset=0;
        while (offset<len && !interrupted && !ctrl_c) {
            sz=(offset+N<=len?N:len-offset)*block_size;
            if (!ao_play(device,_data+(pos+offset)*block_size,sz)) {
                ret=2; // playback error
                break;
            }
            offset+=N;
        }
        if (interrupted || ctrl_c) {
            interrupted=ctrl_c=false;
            break;
        }
    }
    ao_close(device);
    ao_shutdown();
    return ret;
}
#else
#if !defined GIAC_GGB && (defined EMCC || defined (EMCC2))// must have EM_ASM code javascript inlined (emscripten 1.30.4 at least?)
#include <emscripten.h>
int audio_clip::play(int pos,int len,int repeats) const {
    int nbits=_bit_depth,nrate=_sample_rate,nchannels=_nc;
    unsigned data_size=pos>_len-len?_len-pos:len,b=nbits/8;
    unsigned total_size=data_size*repeats,nDataBytes=total_size*nchannels*sizeof(float);
    // copy data from v into buffer and play it
    float *ptr=(float*)malloc(nDataBytes);
    for (unsigned j=0;j<nchannels;++j) {
        for (unsigned i=0;i<total_size;++i) {
            unsigned u=get_sample(j,pos+(i%data_size));
            double ud=0;
            if (b==1)
                ud=u/128.0-1;
            else if (b==2)
                ud=u/32768.0-1;
            else if (b==3)
                ud=u/16777216.0-1;
            ptr[j*total_size+i]=ud;
        }
    }
    COUT << "Playing..." << '\n';
    EM_ASM_ARGS({
        var nchannels;
        var nDataBytes;
        var nrate;
        var ptr;
        var data_size;
        nchannels=$0;nDataBytes=$1;nrate=$2;ptr=$3;
        data_size=nDataBytes/4/nchannels;
        var audioCtx=new (window.AudioContext || window.webkitAudioContext)();
        var SoundArrayBuffer=audioCtx.createBuffer(nchannels,nDataBytes,audioCtx.sampleRate);
        var dataHeap=new Uint8Array(Module.HEAPU8.buffer,ptr,nDataBytes);
        var result=new Float32Array(dataHeap.buffer,dataHeap.byteOffset,nDataBytes/4);
        var j;
        var i;
        for (j=0;j<nchannels;j++) {
            var v=SoundArrayBuffer.getChannelData(j);
            for (i=0;i<data_size;++i)
                v[i]=result[j*data_size+i];
        }
        var source=audioCtx.createBufferSource();
        // set the buffer in the AudioBufferSourceNode
        source.buffer=SoundArrayBuffer;
        // connect the AudioBufferSourceNode to the
        // destination so we can hear the sound
        source.connect(audioCtx.destination);
        // start the source playing
        source.start();
    },nchannels,nDataBytes,nrate,ptr);
    free(ptr);
    return 0;
}
#else
int audio_clip::play(int pos,int len,int repeats) const {
    return 3; // ao is missing; no sound playback
}
#endif
#endif // HAVE_LIBAO

gen _playsnd(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    audio_clip *clip=audio_clip::from_gen((g.type==_VECT && !g._VECTptr->empty())?g._VECTptr->front():g);
    if (clip==NULL)
        return generrtype(_SP_BAD_SOUND_DATA);
    int offset=0,len=rand_max2,sr=clip->sample_rate(),repeats=1;
    if (g.type==_VECT) {
        if (g.subtype!=_SEQ__VECT)
            return gentypeerr(contextptr);
        const vecteur &gv=*g._VECTptr;
        if (gv.size()<2 || gv.size()>4)
            return generrdim(gettext("Wrong number of arguments"));
        gen a,b;
        if (is_real_interval(gv[1],a,b,contextptr)) {
            offset=(int)std::floor(0.5+sr*a.to_double(contextptr));
            len=(int)std::floor(0.5+sr*(b-a).to_double(contextptr));
            if (gv.size()>3)
                return generrdim(gettext("Too many input arguments"));
            if (gv.size()==3) {
                if (gv[2].is_integer())
                    repeats=gv[2].val;
                else return generrtype(gettext("Expected a number of repeats"));
            }
        } else if (gv[1].is_integer()) {
            offset=gv[1].val;
            if (gv.size()>2) {
                if (gv[2].is_integer())
                    len=gv[2].val;
                else return generrtype(gettext("Expected a length in samples"));
            }
            if (gv.size()>3) {
                if (gv[3].is_integer())
                    repeats=gv[3].val;
                else return generrtype(gettext("Expected a number of repeats"));
            }
        } else return generrtype(gettext("Expected an offset or a range"));
    }
    if (offset<0 || offset>=clip->length())
        return generr(gettext("Invalid offset"));
    len=std::min(len,clip->length()-offset);
    if (len<0)
        return generr(gettext("Invalid length"));
    if (len==0)
        return 1;
    int res=clip->play(offset,len,repeats);
    switch (res) {
    case 1:
        print_error("Audio device failure",contextptr);
        break;
    case 2:
        print_error("Playback failure",contextptr);
        break;
    case 3:
        print_error("libao is required for sound playback",contextptr);
        break;
    default:
        break;
    }
    return (int)(res==0);
}
static const char _playsnd_s[]="playsnd";
static define_unary_function_eval (__playsnd,&_playsnd,_playsnd_s);
define_unary_function_ptr5(at_playsnd,alias_at_playsnd,&__playsnd,0,true);

/* Splice two audio clips. */
gen _splice(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &args=*g._VECTptr;
    if (args.size()<2 || args.size()>4)
        return gendimerr(contextptr);
    audio_clip *a,*b;
    a=audio_clip::from_gen(args[0]);
    b=audio_clip::from_gen(args[1]);
    if (a==NULL || b==NULL)
        return generrtype(gettext("First two arguments should be audio clips"));
    int pos=-1,sr=a->sample_rate(),len=a->length();
    double ex=-1;
    if (args.size()>2) {
        if (!is_real_number(args[2],contextptr))
            return generrtype(gettext("Crossfade length should be a real number"));
        if (is_approx(args[2]))
            pos=len-(int)std::floor(0.5+sr*to_real_number(args[2],contextptr).to_double(contextptr));
        else if (args[2].is_integer())
            pos=len-args[2].val;
        else return gentypeerr(contextptr);
    }
    if (args.size()>3) {
        if (!is_real_number(args[3],contextptr))
            return generr(gettext("Crossfade exponent should be a real number"));
        ex=to_real_number(args[3],contextptr).to_double(contextptr);
    }
    if (pos<0)
        pos=len-(int)std::floor(0.5+sr*std::min(std::min(a->duration(),b->duration())/2.0,0.5));
    try {
        return a->splice(*b,pos,ex);
    } catch (const std::runtime_error &e) {
        return generr(e.what());
    }
}
static const char _splice_s[]="splice";
static define_unary_function_eval (__splice,&_splice,_splice_s);
define_unary_function_ptr5(at_splice,alias_at_splice,&__splice,0,true);

bool is_audio_position(const gen &g,int sr,int &offset,GIAC_CONTEXT) {
    if (g.is_integer() && g.val>=0) {
        offset=g.val;
        return true;
    }
    if (is_real_number(g,contextptr) && is_positive(g,contextptr)) {
        offset=(int)std::floor(0.5+sr*to_real_number(g,contextptr).to_double(contextptr));
        return true;
    }
    return false;
}

/* Mix audio clips down to one. */
gen _mixdown(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->empty())
        return gentypeerr(contextptr);
    const vecteur &args=*g._VECTptr;
    int s=args.size();
    if (s<=0 || s%2)
        return gendimerr(contextptr);
    int nclips=s/2,len=0,sr=0,bd=0,nc=0;
    vector<audio_clip*> clips(nclips,NULL);
    vector<int> offsets(nclips,0),cfins(nclips,0),cfouts(nclips,0);
    vector<double> gains(nclips,0.0),pans(nclips,0.0);
    for (int i=0;i<nclips;++i) {
        clips[i]=audio_clip::from_gen(args[2*i]);
        if (clips[i]==NULL)
            return generrarg(2*i+1);
        if (clips[i]->channel_count()>2)
            return generr(gettext("Only mono and stereo clips are allowed"));
        if (sr==0) sr=clips[i]->sample_rate();
        else if (sr!=clips[i]->sample_rate())
            return generr(gettext("All clips must have the same sample rate"));
        bd=std::max(bd,clips[i]->bit_depth());
        nc=std::max(nc,clips[i]->channel_count());
        if (args[2*i+1].type==_VECT && args[2*i+1]._VECTptr->size()>5)
            return gendimerr(contextptr);
        vecteur parm=args[2*i+1].type==_VECT?*args[2*i+1]._VECTptr:vecteur(1,args[2*i+1]);
        if (parm.size()>0 && !is_audio_position(parm[0],sr,offsets[i],contextptr))
            return generr(gettext("Invalid offset specification"));
        if (parm.size()>1 && (!is_real_number(parm[1],contextptr) ||
                (gains[i]=to_real_number(parm[1],contextptr).to_double(contextptr))>0))
            return generr(gettext("Invalid gain specification"));
        if (parm.size()>2 && (!is_real_number(parm[2],contextptr) ||
                (pans[i]=to_real_number(parm[2],contextptr).to_double(contextptr))< -1.0 || pans[i]>1.0))
            return generr(gettext("Invalid pan specification"));
        if (parm.size()>3 && !is_audio_position(parm[3],sr,cfins[i],contextptr))
            return generr(gettext("Invalid fade-in specification"));
        if (parm.size()>4 && !is_audio_position(parm[4],sr,cfouts[i],contextptr))
            return generr(gettext("Invalid fade-out specification"));
        if (cfins[i]+cfouts[i]>clips[i]->length())
            return generrdim(gettext("Fade-in/out is too long"));
        len=std::max(len,offsets[i]+clips[i]->length());
    }
    audio_clip ret(bd,sr,nc,len,contextptr);
    bool clipping=false,res;
    for (int i=0;i<nclips;++i) {
        res=ret.mix(*clips[i],offsets[i],gains[i],pans[i],cfins[i],cfouts[i]);
        if (!clipping && !res)
            clipping=true;
    }
    if (clipping)
        *logptr(contextptr) << gettext("Warning") << ": " << gettext("clipping audio, the result will be distorted") << "\n";
    return ret;
}
static const char _mixdown_s[]="mixdown";
static define_unary_function_eval (__mixdown,&_mixdown,_mixdown_s);
define_unary_function_ptr5(at_mixdown,alias_at_mixdown,&__mixdown,0,true);

gen discrete_wavelet_transform(const gen &g,int dir,GIAC_CONTEXT) {
#ifndef HAVE_LIBGSL
  return undef;
#else
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    vecteur *data;
    int wtype=_DAUBECHIES_WAVELET,k=4;
    bool non_standard=false,centered=false;
    if (g.subtype==_SEQ__VECT) {
        const vecteur &args=*g._VECTptr;
        if (args.empty() || args.size()>5)
            return gendimerr(contextptr);
        if (args[0].type!=_VECT)
            return gentypeerr(contextptr);
        data=args[0]._VECTptr;
        for (const_iterateur it=args.begin()+1;it!=args.end();++it) {
            if (it->type==_STRNG) {
                const string &wt=*it->_STRNGptr;
                if (wt=="daubechies")
                    wtype=_DAUBECHIES_WAVELET;
                else if (wt=="haar") {
                    wtype=_HAAR_WAVELET;
                    k=2;
                } else if (wt=="bspline") {
                    wtype=_BSPLINE_WAVELET;
                    k=103;
                } else return generr(gettext("Unsupported wavelet type"));
            } else if (it->is_integer()) {
                k=it->val;
                if ((wtype==_DAUBECHIES_WAVELET && (k%2 || k<4 || k>20)) ||
                        (wtype==_HAAR_WAVELET && k!=2) ||
                        (wtype==_BSPLINE_WAVELET && (k!=103 && k!=105 && k!=202 && k!=204 && k!=206 && k!=208
                            && k!=301 && k!=303 && k!=305 && k!=307 && k!=309)))
                    return generr(gettext("Wavelet not implemented"));
            } else if (*it==at_centre)
                centered=true;
            else if (*it==at_image)
                non_standard=true;
            else return generrarg(it-args.begin()+1);
        }
    } else data=g._VECTptr;
    int datatype=ckmatrix(*data)?(non_standard?3:2):1;
    int n=datatype==1?data->size():std::max(mcols(*data),mrows(*data));
    if (n==0)
        return vecteur(0);
    if (n & (n-1)) { // not a power of two
        n=nextpow2(n);
        *logptr(contextptr) << gettext("Warning") << ": "
                            << gettext("data size is not a power of 2, padding with zeros up to size") << " ";
        if (datatype==1)
            *logptr(contextptr) << n;
        else *logptr(contextptr) << n << "×" << n;
        *logptr(contextptr) << "\n";
    }
    void *dat;
    gsl_matrix *dd;
    double *d;
    if (datatype==1) {
        d=(double*)calloc(n,sizeof(double));
        if (d==NULL)
            return generr(gettext("Failed to allocate memory"));
        const_iterateur it=data->begin(),itend=data->end();
        for (int i=0;it!=itend;++it,++i) {
            if (!is_real_number(*it,contextptr)) {
                free(d);
                return generr(gettext("Data must be numeric"));
            }
            d[i]=to_real_number(*it,contextptr).to_double(contextptr);
        }
        dat=(void*)d;
    } else {
        dd=gsl_matrix_calloc(n,n);
        if (dd==NULL)
            return generr(gettext("Failed to allocate memory"));
        if (matrice2gsl_matrix(*data,0,0,mrows(*data),mcols(*data),false,dd,contextptr)!=GSL_SUCCESS)
            return generr(gettext("Failed to convert data to GSL matrix"));
        dat=static_cast<void*>(dd);
    }
    int res=dwt(dat,datatype,n,wtype,k,dir);
    if (res!=0)
        free(d);
    switch (res) {
        case 1: return generr(gettext("Unsupported wavelet type"));
        case 2: return generr(gettext("Failed to allocate memory"));
        case 3: return generr(gettext("DWT computation failed"));
        default: break;
    }
    if (datatype==1) {
        vecteur ret;
        ret.reserve(n);
        for (int i=0;i<n;++i)
            ret.push_back(d[i]);
        free(d);
        return ret;
    }
    matrice ret=gsl_matrix2matrice(dd);
    gsl_matrix_free(dd);
    return ret;
#endif
}

/* Discrete wavelet transform. */
gen _dwt(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
#ifndef HAVE_LIBGSL
    return generr(gettext("GSL is required for wavelet transform"));
#else
    return discrete_wavelet_transform(g,1,contextptr);
#endif
}
static const char _dwt_s[]="dwt";
static define_unary_function_eval (__dwt,&_dwt,_dwt_s);
define_unary_function_ptr5(at_dwt,alias_at_dwt,&__dwt,0,true);

/* Inverse discrete wavelet transform. */
gen _idwt(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
#ifndef HAVE_LIBGSL
    return generr(gettext("GSL is required for wavelet transform"));
#else
    return discrete_wavelet_transform(g,-1,contextptr);
#endif
}
static const char _idwt_s[]="idwt";
static define_unary_function_eval (__idwt,&_idwt,_idwt_s);
define_unary_function_ptr5(at_idwt,alias_at_idwt,&__idwt,0,true);

/* Resample this clip at NSR Hz with the specified quality using libsamplerate, returning
 * the result as a new audio clip. If the library is not available, an error is thrown. */
audio_clip audio_clip::resample(int nsr,int quality) const {
#ifndef HAVE_LIBSAMPLERATE
    throw std::runtime_error(gettext("libsamplerate is required for resampling audio"));
#else
    SRC_DATA data;
    data.src_ratio=double(nsr)/double(_sample_rate);
    int nlen=(int)std::floor(_len*data.src_ratio);
    data.input_frames=_len;
    data.output_frames=nlen;
    float *indata=new float[_len*_nc];
    data.data_out=new float[nlen*_nc];
    for (int i=0;i<_len;++i) {
        for (int j=0;j<_nc;++j) {
            indata[i*_nc+j]=decode(get_sample(j,i));
        }
    }
    data.data_in=indata;
    switch(quality) {
    case 0:
        quality=SRC_LINEAR;
        break;
    case 1:
        quality=SRC_ZERO_ORDER_HOLD;
        break;
    case 2:
        quality=SRC_SINC_FASTEST;
        break;
    case 3:
        quality=SRC_SINC_MEDIUM_QUALITY;
        break;
    case 4:
        quality=SRC_SINC_BEST_QUALITY;
        break;
    default:
        assert(false);
    }
    int res=src_simple(&data,quality,_nc);
    if (res!=0) // error
        throw std::runtime_error(src_strerror(res));
    audio_clip clip(_bit_depth,nsr,_nc,nlen,ctx);
    for (int i=0;i<nlen;++i) {
        for (int j=0;j<_nc;++j) {
            clip.set_sample(j,i,encode(data.data_out[i*_nc+j]));
        }
    }
    delete[] indata;
    delete[] data.data_out;
    return clip;
#endif
}

bool audio_clip::get_chunk_span(int c,int offset,int len,double &fu_max,double &fu_min,double &fl_max,double &fl_min) const {
    fu_max=-DBL_MAX;
    fl_min=DBL_MAX;
    fu_min=0.0;
    fl_max=0.0;
    int fu_mincount=0,fl_maxcount=0;
    if (offset<0 || offset>=_len || len<1)
        return false;
    if (offset>_len-len)
        len=_len-offset;
    for (int i=0;i<len;++i) {
        double s=decode(get_sample(c,i+offset));
        if (s>=0) {
            fu_max=std::max(fu_max,s);
            fu_min+=s; fu_mincount++;
        } else {
            fl_max+=s; fl_maxcount++;
            fl_min=std::min(fl_min,s);
        }
    }
    if (fu_mincount>0)
        fu_min/=double(fu_mincount);
    if (fl_maxcount>0)
        fl_max/=double(fl_maxcount);
    if (fu_mincount==0) {
        fu_min=fl_max;
        fl_max=fl_min;
    }
    if (fl_maxcount==0) {
        fl_max=fu_min;
        fu_min=fu_max;
    }
    if (fu_max<0) fu_max=fl_max;
    if (fl_min>0) fl_min=fu_min;
    return true;
}

// Usage: nseconds [,rate]
gen _soundsec(const gen & g,GIAC_CONTEXT){
    gen n,rate=44100;
    if (g.type==_VECT && g._VECTptr->size()==2){
        n=g._VECTptr->front();
        rate=g._VECTptr->back();
    }
    else n=g;
    n=evalf_double(n,1,contextptr);
    if (n.type!=_DOUBLE_ || n._DOUBLE_val<=0 || rate.type!=_INT_ || rate.val<1 )
        return generr(gettext("Invalid sound parameters"));
    double r=evalf_double(rate,1,contextptr)._DOUBLE_val;
    double nr=r*n._DOUBLE_val;
    if (nr>LIST_SIZE_LIMIT)
        return generr(gettext("Too many records"));
    vecteur v;
    v.reserve(int(nr));
    for (int i=0;i<nr;++i){
        v.push_back(double(i)/r);
    }
    return v;
}
static const char _soundsec_s[]="soundsec";
static define_unary_function_eval (__soundsec,&_soundsec,_soundsec_s);
define_unary_function_ptr5( at_soundsec ,alias_at_soundsec,&__soundsec,0,true);

gen _createwav(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    int len=0,bd=16,sr=44100,nc=0,offset=0;
    double ratio=1.0,norm=1,secs=-1;
    vecteur *data=NULL;
    string *strdata=NULL;
    if (g.is_integer()) {
        if (g.val<1)
            return generr(gettext("Expected a length in samples"));
        len=g.val;
    } else if (g.type==_VECT) {
        if (g.subtype==_SEQ__VECT) {
            const vecteur &args=*g._VECTptr;
            for (const_iterateur it=args.begin();it!=args.end();++it) {
                if (is_equal(*it)) {
                    const gen &lh=it->_SYMBptr->feuille._VECTptr->front();
                    const gen &rh=it->_SYMBptr->feuille._VECTptr->back();
                    if (lh==at_channels) {
                        if (!rh.is_integer() || (nc=rh.val)<1)
                            return generr(gettext("Number of channels should be a positive integer"));
                    } else if (lh==at_bit_depth) {
                        if (!rh.is_integer() || (bd=rh.val)<0 || (bd!=8 && bd!=16 && bd!=24))
                            return generr(gettext("Bit depth should be either 8, 16 or 24"));
                    } else if (lh==at_samplerate) {
                        if (!rh.is_integer() || (sr=rh.val)<1)
                            return generr(gettext("Sample rate should be a positive integer"));
                    } else if (lh==at_length) {
                        if (!rh.is_integer() || (len=rh.val)<1)
                            return generr(gettext("Length should be a positive integer"));
                    } else if (lh==at_normalize) {
                        if (!is_real_number(rh,contextptr))
                            return generr(gettext("Normalization level should be a real number"));
                        norm=-to_real_number(rh,contextptr).to_double(contextptr);
                    } else if (lh==at_duration) {
                        if (!is_real_number(rh,contextptr))
                            return generr(gettext("Duration should be a real number"));
                        secs=to_real_number(rh,contextptr).to_double(contextptr);
                    } else if (lh==at_channel_data) {
                        if (rh.type==_VECT)
                            data=rh._VECTptr;
                        else if (rh.type==_STRNG)
                            strdata=rh._STRNGptr;
                        else return generr(gettext("Channel data should be either a matrix, vector, or string"));
                    } else return generrarg(it-args.begin()+1);
                } else if (it->type==_VECT && data==NULL && strdata==NULL)
                    data=it->_VECTptr;
                else if (it->type==_STRNG && strdata==NULL && data==NULL)
                    strdata=it->_STRNGptr;
                else return generrarg(it-args.begin()+1);
            }
        } else data=g._VECTptr;
    } else return _createwav(change_subtype(vecteur(1,g),_SEQ__VECT),contextptr);
    if (secs>=0 && len==0)
        len=1+(int)std::floor(secs*sr);
    bool data_is_matrix=false;
    if (data!=NULL) {
        data_is_matrix=ckmatrix(*data);
        if (len==0)
            len=data_is_matrix?mcols(*data):int(data->size());
        if (nc==0)
            nc=data_is_matrix?mrows(*data):1;
        else if (data_is_matrix && mrows(*data)!=nc)
            return generrdim(gettext("Number of channels does not match the number of data rows"));        
    }
    if (nc==0)
        return generr(gettext("Number of channels not set"));
    if (strdata!=NULL)
        len=strdata->size()/(nc*bd/4);
    if (len==0)
        return generr(gettext("Audio length not set"));
    try {
        audio_clip clip(bd,sr,nc,len,contextptr);
        if (strdata!=NULL)
            clip.load_data(*strdata);
        else if (data!=NULL) {
            if (norm>0) {
                gen maxamp(undef);
                try {
                    maxamp=_max(data_is_matrix?_max(_apply(makesequence(at_abs,*data),contextptr),contextptr)
                                            :_abs(*data,contextptr),contextptr);
                } catch (const gen &e) {
                    *logptr(contextptr) << gettext("Failed to determine the maximum amplitude of data") << "\n";
                    return e;
                }
                if (!is_real_number(maxamp,contextptr))
                    return generr(gettext("Sample data should be numeric"));
                ratio=std::pow(10.0,-norm/20.0)/to_real_number(maxamp,contextptr).to_double(contextptr);
            }
            for (int chn=0;chn<nc;++chn)
                clip.set_channel_data(chn,*(data_is_matrix?data->at(chn)._VECTptr:data),offset,ratio);
        }
        return clip;
    } catch (const std::runtime_error &e) {
        return generr(e.what());
        
    }
}
static const char _createwav_s []="createwav";
static define_unary_function_eval (__createwav,&_createwav,_createwav_s);
define_unary_function_ptr5(at_createwav,alias_at_createwav,&__createwav,0,true)

void drawpoly(vecteur &lo,vecteur &hi,int col,vecteur &d,GIAC_CONTEXT) {
    if (!lo.empty()) {
        vecteur pts=mergevecteur(lo,*_revlist(hi,contextptr)._VECTptr);
        pts.push_back(lo.front());
        pts.push_back(symb_equal(at_display,change_subtype(col+_FILL_POLYGON,_INT_COLOR)));
        d.push_back(_polygone(change_subtype(pts,_SEQ__VECT),contextptr));
        lo.clear();
        hi.clear();
    }
}

void append_filled_polygon(const gen &A,const gen &B,const gen &C,const gen &D,int col,int p,bool f,
        vecteur &lo,vecteur &hi,vecteur &d,GIAC_CONTEXT) {
    gen y=im(makevecteur(A,B,C,D),contextptr);
    bool fill;
    gen ar0=max(y[0],y[1],contextptr)-min(y[2],y[3],contextptr);
    fill=f || (!is_zero(ar0,contextptr) && is_greater(0.95,(_abs(y[0]-y[1],contextptr)+_abs(y[2]-y[3],contextptr))/ar0,contextptr));
    if (!fill) {
        drawpoly(lo,hi,col,d,contextptr);
        gen disp=symb_equal(at_display,change_subtype(col,_INT_COLOR));
        d.push_back(_segment(makesequence(p<0?D:(p>0?A:(A+D)/2),p<0?C:(p>0?B:(B+C)/2),disp),contextptr));
    }
    hi.push_back(B);
    lo.push_back(C);
}

gen _plotwav(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    bool has_opts=g.type==_VECT && g.subtype==_SEQ__VECT,fill;
    audio_clip *clip=audio_clip::from_gen(has_opts?g._VECTptr->front():g);
    if (clip==NULL)
        return generrtype(_SP_BAD_SOUND_DATA);
    int nc=clip->channel_count(),bd=clip->bit_depth(),sr=clip->sample_rate(),len=clip->length(),offset=0;
    int dispmax_color=216,dispmin_color=227;
    gen a,b;
    double ts=-1;
    if (has_opts) for (const_iterateur it=g._VECTptr->begin()+1;it!=g._VECTptr->end();++it) {
        if (is_equal(*it)) {
            const gen &lh=it->_SYMBptr->feuille._VECTptr->front();
            const gen &rh=it->_SYMBptr->feuille._VECTptr->back();
            if (lh==at_range) {
                gen a,b;
                if (is_real_interval(rh,a,b,contextptr)) {
                    if (is_positive(a-b,contextptr))
                        return generr(_SP_INVALID_RANGE);
                    offset=std::max(offset,_round(sr*a,contextptr).val);
                    len=std::min(len-offset,_round(sr*b,contextptr).val-offset);
                } else if (rh.type==_VECT) {
                    if (rh._VECTptr->size()!=2 || !is_integer_vecteur(*rh._VECTptr))
                        return generr(_SP_INVALID_RANGE);
                    offset=std::max(offset,rh._VECTptr->front().val);
                    len=std::min(len-offset,rh._VECTptr->back().val-offset);
                } else return generr(_SP_INVALID_RANGE);
            } else if (lh==at_couleur) {
                if (rh.is_integer())
                    dispmax_color=dispmin_color=rh.val;
                else if (rh.type==_VECT) {
                    if (rh._VECTptr->size()!=2)
                        return gendimerr(contextptr);
                    if (!is_integer_vecteur(*rh._VECTptr))
                        return gentypeerr(contextptr);
                    dispmax_color=rh._VECTptr->front().val;
                    dispmin_color=rh._VECTptr->back().val;
                } else return generr(gettext("Invalid color specification"));
            } else if (lh.is_integer() && lh.subtype==_INT_PLOT && lh.val==_TSTEP) {
                if (!is_real_number(rh,contextptr) || !is_strictly_positive(rh,contextptr))
                    return generr(gettext("Invalid time step specification"));
                ts=to_real_number(rh,contextptr).DOUBLE_val();
            } else return generrarg(it-g._VECTptr->begin());
        } else return generrarg(it-g._VECTptr->begin());
    }
    if (dispmax_color<0 || dispmin_color<0 || dispmax_color>=65535 || dispmin_color>=65535)
        return generr(gettext("Invalid color value"));
    if (len==0)
        return vecteur(0);
    int width=500;
    double dur=double(len)/double(sr);
    if (ts>0)
        width=(int)std::floor(dur/ts+0.5);
    double sc=double(len)/double(width),x,y0=nc==1?0.0:1.0,y=y0;
    double fu_max,fu_min,fl_max,fl_min,s,t0=double(offset)/double(sr),dt=1.0/sr;
    vecteur drawing,tvec;
    drawing.reserve(6+(width+2)*nc);
    drawing.push_back(symb_equal(change_subtype(_AXES,_INT_PLOT),nc==1?3:4));
    drawing.push_back(symb_equal(change_subtype(_GL_X,_INT_PLOT),symb_interval(t0,t0+dur)));
    drawing.push_back(symb_equal(change_subtype(_GL_Y,_INT_PLOT),symb_interval(y0-2.0*nc+1.0,y0+1.0)));
    //drawing.push_back(symb_equal(change_subtype(_GL_X_AXIS_UNIT,_INT_PLOT),string2gen("sec",false)));
    for (int c=0;c<nc;++c) {
        if (sc<=1.0) {
            tvec.resize(len);
            vecteur data=clip->get_channel_data(c,offset,len);
            iterateur it=data.begin(),itend=data.end(),jt=tvec.begin();
            for (int i=0;it!=itend;++it,++jt,++i) {
                *it+=gen(y);
                if (c==0) *jt=t0+i*dt;
            }
            drawing.push_back(_listplot(makesequence(mtran(makevecteur(tvec,data)),
                symb_equal(at_couleur,change_subtype(dispmax_color,_INT_COLOR))),contextptr));
        } else {
            //drawing.push_back(_segment(makesequence(gen(t0,y),gen(t0+dur,y),symb_equal(at_couleur,change_subtype(dispmin_color,_INT_COLOR))),contextptr));
            vecteur cur,prev(0),up_lo,up_hi,mid_lo,mid_hi,low_lo,low_hi;
            up_lo.reserve(len); up_hi.reserve(len);
            mid_lo.reserve(len); mid_hi.reserve(len);
            low_lo.reserve(len); low_hi.reserve(len);
            double dw=dur/double(width);
            for (int i=0;i<width;++i) {
                int p=std::floor(i*sc),np=std::floor((i+1)*sc);
                clip->get_chunk_span(c,offset+p,np-p,fu_max,fu_min,fl_max,fl_min);
                x=t0+i*dw;
                cur=makevecteur(gen(x,y+fu_max),gen(x,y+fu_min),gen(x,y+fl_max),gen(x,y+fl_min));
                if (!prev.empty()) {
                    if (std::max(std::abs(fu_max),std::abs(fl_min))<1.e-2) {
                        drawpoly(up_lo,up_hi,dispmax_color,drawing,contextptr);
                        drawpoly(low_lo,low_hi,dispmax_color,drawing,contextptr);
                        drawpoly(mid_lo,mid_hi,dispmin_color,drawing,contextptr);
                        drawing.push_back(_segment(makesequence(_mean(prev,contextptr),_mean(cur,contextptr),symb_equal(at_couleur,change_subtype(dispmin_color,_INT_COLOR))),contextptr));
                    } else {
                        fill=is_strictly_greater(im(prev[0],contextptr),y,contextptr) && is_strictly_greater(y,im(prev[3],contextptr),contextptr) &&
                            is_strictly_greater(im(cur[0],contextptr),y,contextptr) && is_strictly_greater(y,im(cur[3],contextptr),contextptr);
                        append_filled_polygon(prev[1],cur[1],cur[2],prev[2],dispmin_color,0,fill,mid_lo,mid_hi,drawing,contextptr);
                        append_filled_polygon(prev[0],cur[0],cur[1],prev[1],dispmax_color,1,fill,up_lo,up_hi,drawing,contextptr);
                        append_filled_polygon(prev[2],cur[2],cur[3],prev[3],dispmax_color,-1,fill,low_lo,low_hi,drawing,contextptr);
                    }
                }
                prev=cur;
            }
            drawpoly(up_lo,up_hi,dispmax_color,drawing,contextptr);
            drawpoly(low_lo,low_hi,dispmax_color,drawing,contextptr);
            drawpoly(mid_lo,mid_hi,dispmin_color,drawing,contextptr);
        }
        if (c<nc-1)
            drawing.push_back(_segment(makesequence(gen(t0,y-1.0),gen(t0+dur,y-1.0),symb_equal(at_couleur,change_subtype(45+_DASH_LINE,_INT_COLOR))),contextptr));
        y-=2.0;
    }
    if (nc==2) {
        drawing.push_back(_legende(makesequence(gen(t0,1.95),string2gen("L",false),change_subtype(_QUADRANT4,_INT_COLOR)),contextptr));
        drawing.push_back(_legende(makesequence(gen(t0,-.05),string2gen("R",false),change_subtype(_QUADRANT4,_INT_COLOR)),contextptr));
    }
    return drawing;
}
static const char _plotwav_s []="plotwav";
static define_unary_function_eval (__plotwav,&_plotwav,_plotwav_s);
define_unary_function_ptr5(at_plotwav,alias_at_plotwav,&__plotwav,0,true)

gen _stereo2mono(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    audio_clip *clip=audio_clip::from_gen(g);
    if (clip==NULL)
        return generrtype(_SP_BAD_SOUND_DATA);
    try {
        return clip->to_mono();
    } catch (const std::runtime_error &e) {
        return generr(e.what());
    }
}
static const char _stereo2mono_s []="stereo2mono";
static define_unary_function_eval (__stereo2mono,&_stereo2mono,_stereo2mono_s);
define_unary_function_ptr5(at_stereo2mono,alias_at_stereo2mono,&__stereo2mono,0,true)

gen _plotspectrum(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    bool has_opts=g.type==_VECT && g.subtype==_SEQ__VECT,disp_default=true;
    if (has_opts && g._VECTptr->empty())
        return gentoofewargs(gettext("Too few input arguments"));
    int disp=_FILL_POLYGON,bins=512;
    vecteur fb;
    if (has_opts) for (const_iterateur it=g._VECTptr->begin()+1;it!=g._VECTptr->end();++it) {
        if (it->is_symb_of_sommet(at_equal)) {
            const gen &lh=it->_SYMBptr->feuille._VECTptr->front();
            const gen &rh=it->_SYMBptr->feuille._VECTptr->back();
            if (lh==at_couleur) {
                if (!rh.is_integer() || rh.val<0)
                    return generr(gettext("Invalid color specification"));
                disp|=rh.val;
                disp_default=false;
            } else if (lh==at_frequencies) {
                if (!rh.is_symb_of_sommet(at_interval) || !is_integer_vecteur(*rh._SYMBptr->feuille._VECTptr))
                    return generr(gettext("Invalid frequency range specification"));
                fb=*rh._SYMBptr->feuille._VECTptr;
            } else if (lh.is_integer() && lh.subtype==_INT_MAPLECONVERSION && lh.val==_KDE_BINS) {
                if (!rh.is_integer() || rh.val<2 || (1<<int(mylog2(rh.val)+0.5))!=rh.val)
                    return generr(gettext("Invalid bin count specification"));
                bins=rh.val;
            } else return generrarg(it-g._VECTptr->begin());
        } else if (it->type==_VECT) {
            if (it->_VECTptr->size()!=2 || !is_integer_vecteur(*it->_VECTptr))
                return generr(gettext("Invalid frequency range specification"));
            fb=*it->_VECTptr;
        } else return generrarg(it-g._VECTptr->begin());
    }
    audio_clip *clip=audio_clip::from_gen(has_opts?g._VECTptr->front():g);
    if (clip==NULL)
        return generr(gettext("Expected an audio clip"));
    int nc=clip->channel_count(),bd=clip->bit_depth();
    long sr=clip->sample_rate(),len=clip->length();
    int lfreq=0,ufreq=int(sr)/2;
    if (!fb.empty()) {
        lfreq=std::max(lfreq,fb.front().val);
        ufreq=std::min(ufreq,fb.back().val);
        if (lfreq>=ufreq)
            return generr(gettext("Invalid frequency range"));
    }
    if (disp_default)
        disp+=219;
    vecteur data=(nc>1?audio_clip::from_gen(_stereo2mono(*clip,contextptr)):clip)->get_channel_data(0);
    vecteur spec,nodes,drawing;
    if (len<2*bins) {
        vecteur fs=*_fft(data,contextptr)._VECTptr;
        fs.resize(fs.size()/2);
        spec=*_pointpow(makesequence(_abs(fs,contextptr),2),contextptr)._VECTptr;
    } else {
        if (len%bins) data.resize((len/bins+1)*bins,0);
        matrice fs=stft(data,bins,at_welch_window,contextptr);
        spec=*_mean(_pointpow(makesequence(_apply(makesequence(at_abs,fs),contextptr),2),contextptr),contextptr)._VECTptr;
        len=2*bins;
    }
    int n1=(len*(long)lfreq)/sr,n2=(len*(long)ufreq)/sr,dfreq=ufreq-lfreq;
    int width=std::min(dfreq,std::max(500,dfreq/5)),step=std::max(1,int(n2-n1)/int(2*width));
    nodes.reserve(width);
    drawing.reserve(width+2);
    gen f=0,d=double(sr)/double(len),fmax=0,fthr=1e-3,len2=len*len;
    for (int i=n1;i<n2;++i) {
        f+=spec[i];
        if (i%step==0) {
            nodes.push_back(makecomplex(i*d,f/len2));
            fmax=max(fmax,f,contextptr);
            f=0;
        }
    }
    drawing.push_back(symb_equal(change_subtype(_AXES,_INT_PLOT),4));
    drawing.push_back(symb_equal(change_subtype(_GL_Y,_INT_PLOT),symb_interval(0.0,1.07*fmax/len2)));
    //drawing.push_back(symb_equal(change_subtype(_GL_X_AXIS_UNIT,_INT_PLOT),string2gen("Hz",false)));
    fthr=fthr*fmax/len2;
    const_iterateur it=nodes.begin(),itend=nodes.end();
    for (;it!=itend;++it) {
        if (is_greater(im(*it,contextptr),fthr,contextptr)) {
            gen A(re(*it,contextptr)),B(re(*it+d,contextptr)),C(*it+d);
            drawing.push_back(pnt_attrib(gen(makevecteur(A,B,C,*it,A),_GROUP__VECT),vecteur(1,disp),contextptr));
        }
    }
    return drawing;
}
static const char _plotspectrum_s []="plotspectrum";
static define_unary_function_eval (__plotspectrum,&_plotspectrum,_plotspectrum_s);
define_unary_function_ptr5(at_plotspectrum,alias_at_plotspectrum,&__plotspectrum,0,true)

gen _channels(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    audio_clip *clip=audio_clip::from_gen(g);
    rgba_image *img=rgba_image::from_gen(g);
    if (clip!=NULL)
        return clip->channel_count();
    if (img!=NULL)
        return img->depth();
    return generrtype(gettext("Expected audio clip or image"));
}
static const char _channels_s []="channels";
static define_unary_function_eval (__channels,&_channels,_channels_s);
define_unary_function_ptr5(at_channels,alias_at_channels,&__channels,0,true)

gen _bit_depth(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    audio_clip *clip=audio_clip::from_gen(g);
    rgba_image *img=rgba_image::from_gen(g);
    if (clip!=NULL)
        return clip->bit_depth();
    if (img!=NULL)
        return img->bit_depth();
    return generrtype(gettext("Expected audio clip or image"));
}
static const char _bit_depth_s []="bit_depth";
static define_unary_function_eval (__bit_depth,&_bit_depth,_bit_depth_s);
define_unary_function_ptr5(at_bit_depth,alias_at_bit_depth,&__bit_depth,0,true)

gen _samplerate(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    audio_clip *clip=audio_clip::from_gen(g);
    if (clip==NULL)
        return generrtype(_SP_BAD_SOUND_DATA);
    return clip->sample_rate();
}
static const char _samplerate_s []="samplerate";
static define_unary_function_eval (__samplerate,&_samplerate,_samplerate_s);
define_unary_function_ptr5(at_samplerate,alias_at_samplerate,&__samplerate,0,true)

gen _duration(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    audio_clip *clip=audio_clip::from_gen(g);
    if (clip==NULL)
        return generrtype(_SP_BAD_SOUND_DATA);
    return double(clip->length())/double(clip->sample_rate());
}
static const char _duration_s []="duration";
static define_unary_function_eval (__duration,&_duration,_duration_s);
define_unary_function_ptr5(at_duration,alias_at_duration,&__duration,0,true)

gen _channel_data(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    bool has_opts=g.type==_VECT && g.subtype==_SEQ__VECT;
    audio_clip *clip=audio_clip::from_gen(has_opts?g._VECTptr->front():g);
    rgba_image *img=rgba_image::from_gen(has_opts?g._VECTptr->front():g);
    if (clip!=NULL) {
        int len=clip->length(),sr=clip->sample_rate(),chan=-1,slice_start=0,slice_len=len;
        bool asmatrix=false;
        if (has_opts) for (const_iterateur it=g._VECTptr->begin()+1;it!=g._VECTptr->end();++it) {
            if (it->is_integer())
                chan=it->val;
            else if (*it==at_left)
                chan=0;
            else if (*it==at_right)
                chan=1;
            else if (*it==at_matrix)
                asmatrix=true;
            else if (is_equal(*it)) {
                const gen &lh=it->_SYMBptr->feuille._VECTptr->front();
                const gen &rh=it->_SYMBptr->feuille._VECTptr->back();
                if (lh==at_range) {
                    gen a,b;
                    if (rh.type==_VECT) {
                        if (rh._VECTptr->size()!=2)
                            return generrdim(_SP_INVALID_RANGE);
                        if (!is_integer_vecteur(*rh._VECTptr))
                            return generr(_SP_INVALID_RANGE);
                        slice_start=rh._VECTptr->front().val;
                        slice_len=rh._VECTptr->back().val;
                    } else if (is_real_interval(rh,a,b,contextptr)) {
                        slice_start=std::floor(0.5+sr*a.to_double(contextptr));
                        slice_len=std::floor(0.5+sr*(b-a).to_double(contextptr));
                    } else return generr(_SP_INVALID_RANGE);
                }
            } else return generrarg(it-g._VECTptr->begin()+1);
        }
        slice_len=std::min(slice_len,len-slice_start);
        if (slice_start<0 || slice_start>=len || slice_len<=0)
            return generr(gettext("Invalid slice bounds"));
        if (chan<0) {
            vecteur ret=clip->get_block(slice_start,slice_len);
            if (ret.size()==1)
                return ret.front();
            return asmatrix?ret:change_subtype(ret,_SEQ__VECT);
        }
        if (chan>=clip->channel_count())
            return generr(gettext("Invalid channel number"));
        return clip->get_channel_data(chan,slice_start,slice_len);
    } else if (img!=NULL) {
        if (!has_opts) {
            if (img->depth()>1)
                return generr(gettext("No channel specified"));
            return (*img)[change_subtype(GREY_COLOR,_INT_COLOR)];
        }
        const vecteur &args=*g._VECTptr;
        return (*img)[change_subtype(vecteur(args.begin()+1,args.end()),_SEQ__VECT)];
    }
    return generr(gettext("Expected audio clip or image"));
}
static const char _channel_data_s []="channel_data";
static define_unary_function_eval (__channel_data,&_channel_data,_channel_data_s);
define_unary_function_ptr5(at_channel_data,alias_at_channel_data,&__channel_data,0,true)

gen _set_channel_data(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &args=*g._VECTptr;
    if (args.size()<2 || args.size()>5)
        return gendimerr(contextptr);
    audio_clip *clip=audio_clip::from_gen(args.front());
    rgba_image *img=rgba_image::from_gen(args.front());
    if (clip!=NULL) { // set channel data in an audio clip
        int nc=clip->channel_count();
        if (args[1].type!=_VECT)
            return generrtype(gettext("Invalid data specification"));
        const vecteur &data=*args[1]._VECTptr;
        bool data_is_matrix=ckmatrix(data);
        if (data_is_matrix && mrows(data)!=nc)
            return generrdim(gettext("Invalid data specification"));
        int offset=0,chn=-1;
        if (args.size()>2) { // read offset size
            if (!args[2].is_integer())
                return generrtype(gettext("Expected an integer"));
            offset=args[2].val;
            if (offset<0 || offset>=clip->length())
                return generr(gettext("Invalid offset"));
        }
        if (args.size()>3) { // read channel number
            const gen &c=args[3];
            if (c==at_left)
                chn=0;
            else if (c==at_right)
                chn=1;
            else if (c.is_integer()) {
                chn=c.val;
                if (chn<0 || chn>=nc)
                    return generrdim(gettext("Invalid channel number"));
            } else return generrtype(gettext("Invalid channel specification"));
            if (data_is_matrix)
                return generrtype(gettext("Invalid data specification"));
        }
        try {
            if (chn<0) {
                if (data_is_matrix)
                    clip->set_block(data,offset);
                else for (int i=0;i<nc;++i) clip->set_channel_data(i,data,offset);
            } else clip->set_channel_data(chn,data,offset);
        } catch (const std::runtime_error &e) {
            return generr(e.what());
        }
        return *clip;
    } else if (img!=NULL) {
        int chn=0,start=img->depth()==1?0:1;
        if (args.size()!=4+start)
            return gendimerr(contextptr);
        if (start) {
            if (args[1]==at_tran) {
                if (img->depth()%2)
                    return generr(gettext("Alpha channel not found"));
                chn=img->depth()-1;
            } else if (args[1].is_integer() && args[1].subtype==_INT_COLOR) {
                switch (args[1].val) {
                case _RED: chn=0; break;
                case _GREEN: chn=1; break;
                case _BLUE: chn=2; break;
                case GREY_COLOR:
                    if (img->depth()>2) return generr(gettext("This is not a grayscale image"));
                    chn=0;
                    break;
                default: return generr(gettext("Invalid channel specification"));
                }
            } else return generrtype(gettext("Invalid channel specification"));
        }
        if (!args[1+start].is_integer() || !args[2+start].is_integer())
            return generrtype(gettext("Invalid reference point specification"));
        int x=args[1+start].val,y=args[2+start].val;
        if (x<0 || x>=img->width() || y<0 || y>=img->height())
            return generr(gettext("Reference point outside the image"));
        if (!ckmatrix(args[3+start]))
            return generrtype(gettext("Channel data must be a matrix"));
        try {
            img->set_channel_data(chn,*args[3+start]._VECTptr,x,y);
        } catch (const std::runtime_error &e) {
            return generr(e.what());
        }
        return *img;
    }
    return generr(gettext("Expected audio clip or image"));
}
static const char _set_channel_data_s []="set_channel_data";
static define_unary_function_eval (__set_channel_data,&_set_channel_data,_set_channel_data_s);
define_unary_function_ptr5(at_set_channel_data,alias_at_set_channel_data,&__set_channel_data,0,true)

// subroutine for computing cross-correlation and convolution, (I)FFT is performed once
vecteur fft_conv(const vecteur &A,const vecteur &B,bool cnj,GIAC_CONTEXT) {
    int lenA=A.size(),lenB=B.size(),len=lenA+(cnj?0:lenB-1);
    vecteur x;
    x.reserve(len);
    const_iterateur iA=A.begin(),iAend=A.end(),iB=B.begin(),iBend=B.end();
    while (iA!=iAend || iB!=iBend) x.push_back(gen(iA!=iAend?*(iA++):0,iB!=iBend?*(iB++):0));
    x.resize(len,0);
    vecteur X=*_fft(x,contextptr)._VECTptr,Y;
    Y.reserve(len);
    X.push_back(X.front());
    const_iterateur it=X.begin();
    vecteur::reverse_iterator jt=X.rbegin();
    for (int i=0;i<len;++i,++it,++jt)
        Y.push_back(pow(cnj?_abs(*it,contextptr):*it,2)-pow(cnj?_abs(*jt,contextptr):conj(*jt,contextptr),2)
            -(cnj?2*cst_i*im(*it*(*jt),contextptr):0));
    return divvecteur(*im(_ifft(Y,contextptr),contextptr)._VECTptr,4);
}

gen _cross_correlation(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &args=*g._VECTptr;
    if (args.size()!=2 || args.front().type!=_VECT || args.back().type!=_VECT)
        return generr(gettext("Expected a sequence of two lists"));
    vecteur A=*args.front()._VECTptr,B=*args.back()._VECTptr;
    int m=A.size(),n=B.size(),N=nextpow2(std::max(n,m));
    A.resize(2*N,0);
    B.resize(2*N,0);
    vecteur cc=multvecteur(-1,fft_conv(A,B,true,contextptr));
    reverse(cc.begin(),cc.begin()+N);
    reverse(cc.begin()+N,cc.end());
    return vecteur(cc.begin()+N-m,cc.end()-N+n-1);
}
static const char _cross_correlation_s []="cross_correlation";
static define_unary_function_eval (__cross_correlation,&_cross_correlation,_cross_correlation_s);
define_unary_function_ptr5(at_cross_correlation,alias_at_cross_correlation,&__cross_correlation,0,true)

gen _auto_correlation(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG) {
        if (g.subtype==-1) return g;
        return _auto_correlation(_readwav(g,contextptr),contextptr);
    }
    return _cross_correlation(makesequence(g,g),contextptr);
}
static const char _auto_correlation_s []="auto_correlation";
static define_unary_function_eval (__auto_correlation,&_auto_correlation,_auto_correlation_s);
define_unary_function_ptr5(at_auto_correlation,alias_at_auto_correlation,&__auto_correlation,0,true)

void lowpass(vecteur &data,double cutoff,int sr) {
    double rc=0.15915494309/cutoff;
    double dt=1.0/sr;
    gen s=data.front(),alpha(dt/(rc+dt));
    iterateur it=data.begin(),itend=data.end();
    for (;it!=itend;++it) {
        s+=alpha*(*it-s);
        *it=s;
    }
}

void highpass(vecteur &data,double cutoff,int sr) {
    double rc=0.15915494309/cutoff;
    double dt=1.0/sr;
    gen alpha(rc/(rc+dt)),prevdata=data.front(),temp;
    iterateur it=data.begin(),itend=data.end();
    for (++it;it!=itend;++it) {
        temp=*it;
        *it=alpha*(*(it-1)+*it-prevdata);
        prevdata=temp;
    }
}

gen filter(const vecteur &args,filter_type typ,GIAC_CONTEXT) {
    double cutoff=to_real_number(args.at(1),contextptr).to_double(contextptr);
    if (cutoff<=0)
        return generr(gettext("Cutoff frequency should be positive"));
    audio_clip *clip=audio_clip::from_gen(args.front());
    int sr=44100;
    if (clip!=NULL) {
        sr=clip->sample_rate();
        if (cutoff<=0 || cutoff>=sr/2)
            return generr(gettext("Invalid cutoff specification"));
        gen opt(undef);
        if (args.size()>2) {
            if (!is_equal(args[2]))
                return generr(gettext("Third argument should be normalize=<real>"));
            if (args[2]._SYMBptr->feuille._VECTptr->front()==at_normalize)
                opt=args[2];
        }
        matrice data;
        for (int i=0;i<clip->channel_count();++i) {
            vecteur chdata=clip->get_channel_data(i);
            switch(typ) {
            case _LOWPASS_FILTER: lowpass(chdata,cutoff,sr); break;
            case _HIGHPASS_FILTER: highpass(chdata,cutoff,sr); break;
            }
            data.push_back(chdata);
        }
        gen argv=makesequence(symbolic(at_equal,makesequence(at_samplerate,sr)),
                              symbolic(at_equal,makesequence(at_bit_depth,clip->bit_depth())),data);
        if (!is_undef(opt)) argv._VECTptr->push_back(opt);
        return _createwav(argv,contextptr);
    } else if (args.front().type==_VECT) {
        vecteur data=*args.front()._VECTptr;
        if (args.size()>2) {
            if (!args.at(2).is_integer() || (sr=args.at(2).val)<=0)
                return generr(gettext("Invalid sample rate specification"));
        }
        if (cutoff<=0 || cutoff>=sr/2)
            return generr(gettext("Invalid cutoff specification"));
        switch(typ) {
        case _LOWPASS_FILTER: lowpass(data,cutoff,sr); break;
        case _HIGHPASS_FILTER: highpass(data,cutoff,sr); break;
        }
        return data;
    } else return generrtype(gettext("Invalid argument"));
}

gen _lowpass(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()<2 || !is_real_number(g._VECTptr->at(1),contextptr))
        return gentypeerr(contextptr);
    return filter(*g._VECTptr,_LOWPASS_FILTER,contextptr);
}
static const char _lowpass_s []="lowpass";
static define_unary_function_eval (__lowpass,&_lowpass,_lowpass_s);
define_unary_function_ptr5(at_lowpass,alias_at_lowpass,&__lowpass,0,true)

gen _highpass(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()<2 || !is_real_number(g._VECTptr->at(1),contextptr))
        return gentypeerr(contextptr);
    return filter(*g._VECTptr,_HIGHPASS_FILTER,contextptr);
}
static const char _highpass_s []="highpass";
static define_unary_function_eval (__highpass,&_highpass,_highpass_s);
define_unary_function_ptr5(at_highpass,alias_at_highpass,&__highpass,0,true)

gen _moving_average(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return generr(gettext("Expected two input arguments"));
    if (gv.front().type!=_VECT)
        return generr(gettext("First argument should be an array"));
    if (!gv.back().is_integer() || gv.back().val<=0)
        return generr(gettext("Second argument should be a positive integer"));
    const vecteur &s=*gv.front()._VECTptr;
    int n=gv.back().val,len=s.size();
    if (n>len)
        return generr(gettext("Filter length exceeds array size"));
    vecteur res;
    res.reserve(len-n+1);
    gen acc(0);
    for (int i=0;i<n;++i) acc+=s[i];
    res.push_back(acc);
    const_iterateur it=s.begin(),jt=it+n,jtend=s.end();
    for (;jt!=jtend;++jt,++it) res.push_back(acc+=(*jt)-(*it));
    return divvecteur(res,n);
}
static const char _moving_average_s []="moving_average";
static define_unary_function_eval (__moving_average,&_moving_average,_moving_average_s);
define_unary_function_ptr5(at_moving_average,alias_at_moving_average,&__moving_average,0,true)

gen _rms(const gen &g,GIAC_CONTEXT) {
	if (g.type==_STRNG && g.subtype==-1) return g;
    audio_clip *clip=audio_clip::from_gen(g);
    if (clip!=NULL)
        return clip->rms();
	if (g.type!=_VECT)
		return gentypeerr(contextptr);
    if (g.subtype==_SEQ__VECT) {
        const vecteur &args=*g._VECTptr;
        if (args.size()<2 || args.size()>3)
            return gendimerr(contextptr);
        clip=audio_clip::from_gen(args[0]);
        if (clip==NULL)
            return generrtype(gettext("Expected an audio clip"));
        if (!args[1].is_integer())
            return generrtype(gettext("Invalid start position"));
        int offset=args[1].val,len=rand_max2;
        if (args.size()==3) {
            if (!args[2].is_integer())
                return generrtype(gettext("Invalid length"));
            len=args[2].val;
        }
        if (offset<0 || offset>=clip->length())
            return generr(gettext("Invalid start position"));
        if (len<0)
            return generr(gettext("Invalid length"));
        if (len==0)
            return 0;
        return clip->rms(offset,len);
    }
	const vecteur &gv=*g._VECTptr;
	int n=gv.size();
	if (n==0)
		return generr(gettext("Input list is empty"));
	gen res(0),rp,ip;
    const_iterateur it=gv.begin(),itend=gv.end();
	for (;it!=itend;++it) {
		reim(*it,rp,ip,contextptr);
		res+=sq(rp)+sq(ip);
	}
	res=res/n;
	return sqrt(res,contextptr);
}
static const char _rms_s []="rms";
static define_unary_function_eval (__rms,&_rms,_rms_s);
define_unary_function_ptr5(at_rms,alias_at_rms,&__rms,0,true)

gen _resample(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    audio_clip *clip=NULL;
    int nsr=44100;
    int quality=2;
    if (g.type==_VECT && g.subtype==_SEQ__VECT && !g._VECTptr->empty()) {
        const vecteur &gv=*g._VECTptr;
        clip=audio_clip::from_gen(gv[0]);
        if (gv.size()<2 || !gv[1].is_integer() || (nsr=gv[1].val)<1)
            return generr(gettext("Invalid sample rate specification"));
        if (gv.size()>2) {
            if (!gv[2].is_integer() || (quality=gv[2].val)<0 || (quality>4))
                return generr(gettext("Invalid quality specification"));
        }
    } else clip=audio_clip::from_gen(g);
    if (clip==NULL)
        return generrtype(_SP_BAD_SOUND_DATA);
    try {
        return clip->resample(nsr,quality);
    } catch (const std::runtime_error &e) {
        return generr(e.what());
    }
}
static const char _resample_s []="resample";
static define_unary_function_eval (__resample,&_resample,_resample_s);
define_unary_function_ptr5(at_resample,alias_at_resample,&__resample,0,true)

static int varcount=0;

gen _convolution(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &args=*g._VECTptr;
    if (!args.empty() && args.front().type!=_VECT) {
        // convolve real functions
        gen T(0),var=identificateur("x"),tvar=identificateur(" tau"+print_INT_(++varcount));
        int n=args.size(),optstart=2;
        if (n<2)
            return gentoofewargs(gettext("Too few input arguments"));
        const gen &f1=args[0],&f2=args[1];
        if (n>2) {
            if (args[optstart].type==_IDNT)
                var=args[optstart++];
            // parse options
            for (const_iterateur it=args.begin()+optstart;it!=args.end();++it) {
                if (!is_equal(*it))
                    return generr(gettext("Expected option-value pair"));
                vecteur &s=*it->_SYMBptr->feuille._VECTptr;
                if (s.front()==at_shift)
                    T=s.back();
                else return generrarg(it-args.begin());
            }
        }
        log_output_redirect lor(contextptr);
        giac_assume(symb_superieur_egal(tvar,0),contextptr);
        gen c=_integrate(makesequence(f1*_Heaviside(var,contextptr)*
                                      subst(f2*_Heaviside(var,contextptr),var,tvar-var,false,contextptr),
                                      var,minus_inf,plus_inf),contextptr);
        _purge(tvar,contextptr);
        if (contains(_lname(c,contextptr),var))
            return generr(gettext("Failed to integrate"));
        c=subst(c,tvar,var-T,false,contextptr)*_Heaviside(var-T,contextptr);
        return c;
    }
    // convolve sequences
    if (args.size()!=2 || args.front().type!=_VECT || args.back().type!=_VECT)
        return generr(gettext("Expected a pair of lists"));
    const vecteur &A=*args.front()._VECTptr,&B=*args.back()._VECTptr;
    int mask=num_mask_withfrac | num_mask_withint;
    if (!is_numericv(A,mask) || !is_numericv(B,mask) ||
            !is_zero__VECT(*im(A,contextptr)._VECTptr,contextptr) || !is_zero__VECT(*im(B,contextptr)._VECTptr,contextptr))
        return generr(gettext("Expected real numeric input data"));
    return fft_conv(A,B,false,contextptr);
}
static const char _convolution_s []="convolution";
static define_unary_function_eval (__convolution,&_convolution,_convolution_s);
define_unary_function_ptr5(at_convolution,alias_at_convolution,&__convolution,0,true)

gen _boxcar(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()!=3)
        return gentypeerr(contextptr);
    const vecteur &args=*g._VECTptr;
    return _Heaviside(args[2]-args[0],contextptr)-_Heaviside(args[2]-args[1],contextptr);
}
static const char _boxcar_s []="boxcar";
static define_unary_function_eval (__boxcar,&_boxcar,_boxcar_s);
define_unary_function_ptr5(at_boxcar,alias_at_boxcar,&__boxcar,0,true)

gen _rect(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type==_VECT && g.subtype==_SEQ__VECT && g._VECTptr->size()==2)
      return g._VECTptr->front()*exp(g._VECTptr->back()*cst_i,contextptr);
    gen hf=fraction(1,2);
    return _boxcar(makesequence(-hf,hf,g),contextptr);
}
static const char _rect_s []="rect";
static define_unary_function_eval (__rect,&_rect,_rect_s);
define_unary_function_ptr5(at_rect,alias_at_rect,&__rect,0,true)

gen _tri(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    return (1-g)*(_Heaviside(g,contextptr)-_Heaviside(g-1,contextptr))+
           (1+g)*(_Heaviside(-g,contextptr)-_Heaviside(-g-1,contextptr));
}
static const char _tri_s []="tri";
static define_unary_function_eval (__tri,&_tri,_tri_s);
define_unary_function_ptr5(at_tri,alias_at_tri,&__tri,0,true)

gen _sinc(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (is_zero(g))
        return gen(1);
    return sin(g,contextptr)/g;
}
static const char _sinc_s []="sinc";
static define_unary_function_eval (__sinc,&_sinc,_sinc_s);
define_unary_function_ptr5(at_sinc,alias_at_sinc,&__sinc,0,true)

gen _logistic(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    gen x(g),x0(0),L(1),k(1); // standard sigmoid by default
    if (g.type==_VECT && g.subtype==_SEQ__VECT) {
        vecteur gv(*g._VECTptr);
        if (gv.empty() || gv.size()>4)
            return gendimerr(contextptr);
        x=gv.front();
        if (gv.size()>=2)
            x0=gv[1];
        if (gv.size()>=3)
            L=gv[2];
        if (gv.size()==4)
            k=gv[3];
    }
    return L/(exp(-k*(x-x0),contextptr)+1);
}
static const char _logistic_s []="logistic";
static define_unary_function_eval (__logistic,&_logistic,_logistic_s);
define_unary_function_ptr5(at_logistic,alias_at_logistic,&__logistic,0,true)

bool is_rational_wrt(const gen &e,const identificateur &x) {
    return rlvarx(e,x).size()<=1;
}

gen factorise(const gen &g,GIAC_CONTEXT) {
    return factorcollect(g,false,contextptr);
}

bool ispoly(const gen &e,const identificateur &x,int &d,GIAC_CONTEXT) {
    if (is_const_wrt(e,x,contextptr)) {
        d=0;
        return true;
    }
    if (!is_rational_wrt(e,x) || !is_const_wrt(_denom(e,contextptr),x,contextptr))
        return false;
    gen dg=_degree(makesequence(e,x),contextptr);
    return dg.is_integer() && (d=dg.val)>0;
}

gen eval_for_x_in_ab_recursion(const gen &g,const identificateur &x,const gen &a,const gen &b,bool &changed,GIAC_CONTEXT) {
    if (g.type==_VECT) {
        vecteur res;
        res.reserve(g._VECTptr->size());
        const_iterateur it=g._VECTptr->begin(),itend=g._VECTptr->end();
        for (;it!=itend;++it) res.push_back(eval_for_x_in_ab_recursion(*it,x,a,b,changed,contextptr));
        return gen(res,g.subtype);
    }
    if (g.type!=_SYMB)
        return g;
    const gen &f=g._SYMBptr->feuille;
    gen c,d;
    if ((g.is_symb_of_sommet(at_abs) || g.is_symb_of_sommet(at_Heaviside) || g.is_symb_of_sommet(at_sign))
            && is_linear_wrt(f,x,c,d,contextptr) && !is_zero(c)) {
        gen z=-d/c,s=_sign(c,contextptr);
        if (s.is_integer()) {
            bool l=is_greater(a,z,contextptr),r=is_greater(z,b,contextptr);
            if (!changed)
                changed=l||r;
            if (g.is_symb_of_sommet(at_Heaviside)) {
                if (l) return is_one(s)?1:0;
                if (r) return is_one(s)?0:1;
            } else {
                if (l) return s*(g.is_symb_of_sommet(at_abs)?f:gen(1));
                if (r) return s*(g.is_symb_of_sommet(at_abs)?-f:gen(-1));
            }
        }
    }
    return symbolic(g._SYMBptr->sommet,eval_for_x_in_ab_recursion(f,x,a,b,changed,contextptr));
}

gen eval_for_x_in_ab(const gen &g,const identificateur &x,const gen &a,const gen &b,GIAC_CONTEXT) {
    gen ret(g);
    while (true) {
        bool changed=false;
        ret=eval_for_x_in_ab_recursion(ret,x,a,b,changed,contextptr);
        if (!changed)
            break;
    }
    return ret;
}

void constlin_terms(const gen &g,const identificateur &x,gen &lt,gen &c,gen &rest,GIAC_CONTEXT) {
    gen e=expand(g,contextptr);
    vecteur terms=(e.is_symb_of_sommet(at_plus) && e._SYMBptr->feuille.type==_VECT?*e._SYMBptr->feuille._VECTptr:vecteur(1,e));
    gen a,b;
    rest=c=lt=gen(0);
    for (const_iterateur it=terms.begin();it!=terms.end();++it) {
        if (is_const_wrt(*it,x,contextptr))
            c+=*it;
        else if (is_linear_wrt(*it,x,a,b,contextptr)) {
            lt+=a;
            c+=b;
        } else rest+=*it;
    }
}

gen logabs_expand(const gen &g,const identificateur &x,GIAC_CONTEXT) {
    if (g.type==_VECT) {
        vecteur res;
        for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
            res.push_back(logabs_expand(*it,x,contextptr));
        }
        return gen(res,g.subtype);
    }
    gen a,b;
    if (g.is_symb_of_sommet(at_ln) && g._SYMBptr->feuille.is_symb_of_sommet(at_abs) &&
            is_linear_wrt(g._SYMBptr->feuille._SYMBptr->feuille,x,a,b,contextptr) &&
            !is_zero(a) && !is_one(a) && is_zero(im(a,contextptr)) && is_zero(im(b,contextptr)))
        return ln(_abs(x+b/a,contextptr),contextptr)+ln(_abs(a,contextptr),contextptr);
    if (g.type==_SYMB)
        return symbolic(g._SYMBptr->sommet,logabs_expand(g._SYMBptr->feuille,x,contextptr));
    return g;
}

bool has_rootof(const gen &g) {
    if (g.is_symb_of_sommet(at_rootof)) return true;
    if (g.type==_VECT) {
        for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
            if (has_rootof(*it)) return true;
        }
    } else if (g.type==_SYMB) return has_rootof(g._SYMBptr->feuille);
    return false;
}

bool has_partial_diff(const gen &g) {
    if (g.is_symb_of_sommet(at_of) &&
            g._SYMBptr->feuille._VECTptr->front().is_symb_of_sommet(at_derive))
        return true;
    if (g.type==_VECT) {
        for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
            if (has_partial_diff(*it)) return true;
        }
    } else if (g.type==_SYMB) return has_partial_diff(g._SYMBptr->feuille);
    return false;
}

bool is_heavisided(const gen &g,const identificateur &x,gen &rest,gen &a,gen &b,GIAC_CONTEXT) {
    if (g.is_symb_of_sommet(at_neg)) {
        if (!is_heavisided(g._SYMBptr->feuille,x,rest,a,b,contextptr))
            return false;
        rest=-rest;
        return true;
    }
    if (!g.is_symb_of_sommet(at_prod) || g._SYMBptr->feuille.type!=_VECT)
        return false;
    rest=gen(1);
    const vecteur &factors=*g._SYMBptr->feuille._VECTptr;
    bool yes=false;
    for (const_iterateur it=factors.begin();it!=factors.end();++it) {
        if (!yes && it->is_symb_of_sommet(at_Heaviside) &&
                is_linear_wrt(it->_SYMBptr->feuille,x,a,b,contextptr) && !is_zero(a))
            yes=true;
        else rest=rest*(*it);
    }
    return yes;
}

void split_operands(const vecteur &v,int sel,gen &a,gen &b,const unary_function_ptr *op) {
    const_iterateur it=v.begin(),itend=v.end();
    bitset<MAX_FACTORS> bs(sel);
    vecteur l,r;
    l.reserve(v.size());
    r.reserve(v.size());
    int i=0;
    for (;it!=itend;++it,++i) {
        if (bs[i]) l.push_back(*it);
        else r.push_back(*it);
    }
    if (l.empty()) {
        if (op==at_prod) a=1;
        if (op==at_plus) a=0;
    } else a=symbolic(op,l);
    if (r.empty()) {
        if (op==at_prod) b=1;
        if (op==at_plus) b=0;
    } else b=symbolic(op,r);
}

void allcombs(int n,vector<int> &res) {
    int N=1<<n,k;
    res.reserve(N);
    for (k=0;k<N;++k) res.push_back(k);
}

/* Reduce the number of Heavisides in g (by introducing absolute values when possible). */
gen reduce_Heaviside(const gen &g,const identificateur &x,GIAC_CONTEXT) {
    gen_map m;
    gen ret=0,rest,f,f2aminusx,a,b,d,h1,h2,r1,r2,cst,i1,i2,mu,test,retp,restp,p1,p2;
    collect_with_func(g,at_Heaviside,&m,&rest,contextptr);
    gen_map::const_iterator it=m.begin(),itend=m.end();
    vecteur factors;
    int s,s0;
    for (;it!=itend;++it) {
        if (is_linear_wrt(it->first,x,b,a,contextptr) && is_one(b)) {
            a=-a;
            f=ratnormal(it->second,contextptr);
            if (f.is_symb_of_sommet(at_neg)) {
                cst=-1;
                f=f._SYMBptr->feuille;
            } else cst=1;
            factors=f.is_symb_of_sommet(at_prod) && f._SYMBptr->feuille.type==_VECT?*f._SYMBptr->feuille._VECTptr:vecteur(1,f);
            for (int i=factors.size();i-->0;) {
                if (!contains(_lname(factors[i],contextptr),x)) {
                    cst=cst*factors[i];
                    factors.erase(factors.begin()+i);
                }
            }
            vector<int> combs,tcombs;
            if (factors.size()>MAX_FACTORS)
                combs.push_back(0);
            else if (!factors.empty())
                allcombs(factors.size(),combs);
            vector<int>::const_iterator jt=combs.begin(),kt,jtend=combs.end(),ktend;
            i1=undef;
            for (;jt!=jtend;++jt) {
                split_operands(factors,*jt,r1,r2,at_prod);
                p1=p2=0;
                if (r2.is_symb_of_sommet(at_prod))
                    r2=r2._SYMBptr->feuille.type==_VECT?r2._SYMBptr->feuille._VECTptr->front():r2._SYMBptr->feuille;
                vecteur terms;
                if (r2.is_symb_of_sommet(at_plus) && r2._SYMBptr->feuille.type==_VECT)
                    terms=r2._SYMBptr->feuille._VECTptr->size()>1 && r2._SYMBptr->feuille._VECTptr->size()<=MAX_FACTORS?
                        *r2._SYMBptr->feuille._VECTptr:vecteur(1,r2);
                else terms=vecteur(1,r2);
                tcombs.clear();
                allcombs(terms.size(),tcombs);
                gen s1,s2;
                for (kt=tcombs.begin(),ktend=tcombs.end();kt!=ktend;++kt) {
                    split_operands(terms,*kt,s1,s2,at_plus);
                    r2=expand(subst(s1,x,2*a-x,false,contextptr)+s2,contextptr);
                    p1=r1*subst(s1,x,_abs(x-a,contextptr)+a,false,contextptr);
                    p2=-r1*subst(s1,x,2*a-x,false,contextptr);
                    if (is_exactly_zero(r2)) {
                        h1=h2=0;
                    } else {
                        f2aminusx=subst(r2,x,2*a-x,false,contextptr);
                        if (is_exactly_zero(tsimplify(d=r2-f2aminusx,contextptr)))
                            continue;
                        mu=r1*r2/d;
                        h1=subst(r2,x,_abs(x-a,contextptr)+a,false,contextptr)*ratnormal(mu,contextptr);
                        h2=ratnormal(-f2aminusx*mu,contextptr);
                    }
                    test=ratnormal(ret+cst*(h1+p1)+rest+cst*(h2+p2),contextptr);
                    s=taille(test,MAX_TAILLE);
                    if (is_undef(i1) || s<s0) {
                        i1=h1;
                        i2=h2;
                        s0=s;
                        retp=p1;
                        restp=p2;
                    }
                }
            }
            if (!is_undef(i1)) {
                ret+=cst*(i1+retp);
                rest+=cst*(i2+restp);
                continue;
            }
        }
        ret+=it->second*(_abs(it->first,contextptr)+it->first)/(2*it->first);
    }
    return ret+ratnormal(rest,contextptr);
}

void find_func_linarg(const gen &g,const unary_function_ptr *f,const identificateur &x,vecteur &la,bool nested,GIAC_CONTEXT) {
    gen a,b,c;
    if (g.type==_VECT) {
        for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
            find_func_linarg(*it,f,x,la,nested,contextptr);
        }
    } else if (g.type==_SYMB) {
        const gen &e=g._SYMBptr->feuille;
        if (g.is_symb_of_sommet(f)) {
            if (is_linear_wrt(e,x,a,b,contextptr) && !is_zero(a)) {
                if (find(la.begin(),la.end(),-b/a)==la.end())
                    la.push_back(-b/a);
            } else if (nested)
                find_func_linarg(e,f,x,la,nested,contextptr);
        } else find_func_linarg(e,f,x,la,nested,contextptr);
    }
}

bool has_integral(const gen &g) {
    if (g.type==_VECT) {
        for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
            if (has_integral(*it))
                return true;
        }
        return false;
    }
    if (g.type!=_SYMB)
        return false;
    if (g.is_symb_of_sommet(at_integrate) || g.is_symb_of_sommet(at_int) ||
            g.is_symb_of_sommet(at_Int))
        return true;
    return has_integral(g._SYMBptr->feuille);
}

bool is_same(const gen &g,const gen &h,GIAC_CONTEXT) {
    return is_zero(ratnormal(g-h,contextptr));
}

static vecteur fourier_table;
static vecteur laplace_table;

gen _addtable(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &args=*g._VECTptr;
    if (args.size()!=5)
        return generrdim(gettext("Wrong number of input arguments"));
    int ti=0;
    if (args[0]==at_fourier) ti=1;
    if (args[0]==at_laplace) ti=2;
    if (!args[1].is_symb_of_sommet(at_of) || !args[2].is_symb_of_sommet(at_of) ||
            args[1]._SYMBptr->feuille._VECTptr->front().type!=_IDNT ||
            args[2]._SYMBptr->feuille._VECTptr->front().type!=_IDNT ||
            args[3].type!=_IDNT || args[4].type!=_IDNT)
        return generrtype(gettext("Invalid input argument"));
    const identificateur &f=*args[1]._SYMBptr->feuille._VECTptr->front()._IDNTptr;
    const identificateur &F=*args[2]._SYMBptr->feuille._VECTptr->front()._IDNTptr;
    const identificateur &t=*args[3]._IDNTptr,&s=*args[4]._IDNTptr;
    const vecteur fvars=gen2vecteur(args[1]._SYMBptr->feuille._VECTptr->back());
    const vecteur Fvars=gen2vecteur(args[2]._SYMBptr->feuille._VECTptr->back());
    if (fvars.size()!=Fvars.size())
        return gendimerr(contextptr);
    int k=-1;
    for (int i=fvars.size();i-->0;) {
        if (fvars[i].type!=_IDNT || Fvars[i].type!=_IDNT)
            return generr(gettext("Expected an identifier"));
        const identificateur &x1=*fvars[i]._IDNTptr,&x2=*Fvars[i]._IDNTptr;
        if (x1==x2) continue;
        if (k>=0 || !(x1==t) || !(x2==s))
            return gensizeerr(contextptr);
        k=i;
    }
    if (k<0)
        return gensizeerr(contextptr);
    if (ti<1 || ti>2) return gen(0);
    vecteur &tbl=(ti==1?fourier_table:laplace_table);
    for (const_iterateur it=tbl.begin();it!=tbl.end();++it) {
        const vecteur &v=*(it->_VECTptr);
        if (v[0]==f || v[0]==F || v[1]==f || v[1]==F) return gen(0);
    }
    tbl.push_back(makevecteur(f,F,fvars.size(),k));
    return gen(1);
}
static const char _addtable_s []="addtable";
static define_unary_function_eval (__addtable,&_addtable,_addtable_s);
define_unary_function_ptr5(at_addtable,alias_at_addtable,&__addtable,0,true)

vecteur fourier_table_find(const identificateur &f,int sz) {
    const_iterateur it=fourier_table.begin(),itend=fourier_table.end();
    for (;it!=itend;++it) {
        const vecteur &r=*(it->_VECTptr);
        if (r[2].val!=sz) continue;
        if (r[0]==f) return makevecteur(1,r[1],r[3]);
        if (r[1]==f) return makevecteur(2*cst_pi,r[0],r[3]);
    }
    return vecteur(0);
}

bool is_func(const gen &g,identificateur &f,vecteur &vars) {
    if (!g.is_symb_of_sommet(at_of) || g._SYMBptr->feuille._VECTptr->front().type!=_IDNT)
        return false;
    f=*g._SYMBptr->feuille._VECTptr->front()._IDNTptr;
    vars=gen2vecteur(g._SYMBptr->feuille._VECTptr->back());
    return true;
}

bool is_diff(const gen &g,const identificateur &x,gen &f,int &d) {
    if (!g.is_symb_of_sommet(at_derive) || g._SYMBptr->feuille.type!=_VECT)
        return false;
    const vecteur &args=*g._SYMBptr->feuille._VECTptr;
    int n;
    if (args.size()==2 && args.back()==x) d=1;
    else if (args.size()==3 && args[1]==x && args[2].is_integer() && (n=args[2].val)>0) d=n;
    else return false;
    f=args.front();
    return true;
}

bool is_partialdiff(const gen &g,identificateur &f,vecteur &vars,vecteur &deg,GIAC_CONTEXT) {
    if (!g.is_symb_of_sommet(at_of)) return false;
    gen df=g._SYMBptr->feuille._VECTptr->front();
    const gen &v=g._SYMBptr->feuille._VECTptr->back();
    if (v.type!=_VECT) return false;
    vars=*v._VECTptr;
    int n,sz=vars.size();
    deg=vecteur(sz,0);
    while (df.is_symb_of_sommet(at_derive) && df._SYMBptr->feuille.type==_VECT) {
        const vecteur &args=*df._SYMBptr->feuille._VECTptr;
        if (args.size()!=2 || !args.back().is_integer() || (n=args.back().val)<0 || n>=sz)
            return false;
        deg[n]+=gen(1);
        df=args.front();
    }
    if (df.type!=_IDNT || is_zero(deg,contextptr)) return false;
    f=*df._IDNTptr;
    return true;
}

/* return true iff g = a*(x+b)^2+c */
bool is_poly2(const gen &g,const identificateur &x,gen &a,gen &b,gen &c,GIAC_CONTEXT) {
    int st;
    if(!ispoly(g,x,st,contextptr) || st!=2)
        return false;
    gen cf=_canonical_form(makesequence(g,x),contextptr),e,s(undef),d;
    if (cf.is_symb_of_sommet(at_plus) && cf._SYMBptr->feuille.type==_VECT &&
            cf._SYMBptr->feuille._VECTptr->size()==2) {
        e=cf._SYMBptr->feuille._VECTptr->front();
        c=cf._SYMBptr->feuille._VECTptr->back();
    } else {
        e=cf;
        c=0;
    }
    a=gen(1);
    if (e.is_symb_of_sommet(at_neg)) {
        a=gen(-1);
        e=e._SYMBptr->feuille;
    }
    if (e.is_symb_of_sommet(at_prod) && e._SYMBptr->feuille.type==_VECT) {
        const vecteur &factors=*e._SYMBptr->feuille._VECTptr;
        for (const_iterateur it=factors.begin();it!=factors.end();++it) {
            if (is_const_wrt(*it,x,contextptr))
                a=a*(*it);
            else if (is_undef(s))
                s=*it;
            else return false;
        }
    } else s=e;
    if (s.is_symb_of_sommet(at_pow) && s._SYMBptr->feuille.type==_VECT &&
            s._SYMBptr->feuille._VECTptr->size()==2 && is_one(s._SYMBptr->feuille._VECTptr->back()-1)) {
        gen l=s._SYMBptr->feuille._VECTptr->front();
        if (!is_linear_wrt(l,x,d,b,contextptr) || !is_one(d))
            return false;
    } else return false;
    return true;
}

/* return true iff g = a*(b*x+c)^n */
bool is_monomial(const gen &g,const identificateur &x,int &n,gen &a,gen &b,gen &c,GIAC_CONTEXT) {
    if (!ispoly(g,x,n,contextptr))
        return false;
    gen gf=factorise(makesequence(g,x),contextptr),s(undef);
    if (gf.is_symb_of_sommet(at_neg)) {
        gf=g._SYMBptr->feuille;
        a=gen(-1);
    } else a=gen(1);
    if (gf.is_symb_of_sommet(at_prod) && gf._SYMBptr->feuille.type==_VECT) {
        const vecteur &factors=*gf._SYMBptr->feuille._VECTptr;
        for (const_iterateur it=factors.begin();it!=factors.end();++it) {
            if (is_const_wrt(*it,x,contextptr))
                a=a*(*it);
            else if (is_undef(s))
                s=*it;
            else return false;
        }
    } else s=gf;
    return !has_inf_or_undef(s) && ((n==1 && is_linear_wrt(s,x,b,c,contextptr)) ||
        (n>1 && s.is_symb_of_sommet(at_pow) && s._SYMBptr->feuille.type==_VECT &&
           is_linear_wrt(s._SYMBptr->feuille._VECTptr->front(),x,b,c,contextptr)));
}

/* Rewrite g_orig in a way that the products of exp and step functions
 * can be transformed by Fourier transform. */
gen collect_expHeaviside(const gen &g_orig,GIAC_CONTEXT) {
    gen g=_lin(g_orig,contextptr),r,hr,ret(0),ef;
    gen_map em,hm;
    collect_with_func(g,at_exp,&em,&r,contextptr);
    gen_map::const_iterator it=em.begin(),itend=em.end();
    for (;it!=itend;++it) {
        ef=exp(it->first,contextptr);
        hm.clear();
        hr=0;
        collect_with_func(it->second,at_Heaviside,&hm,&hr,contextptr);
        if (hm.size()==1 && is_minus_one(ratnormal(hm.begin()->second/hr,contextptr)))
            ret-=ef*hm.begin()->second*_Heaviside(-hm.begin()->first,contextptr);
        else ret+=ef*it->second;
    }
    return r+ret;
}

/* Simplify Heaviside(ax^2+bx+c) to a linear combination of Heavisides with linear arguments. */
gen simplify_quadratic_Heaviside(const gen &g,const identificateur &x,GIAC_CONTEXT) {
    if (g.type==_VECT) {
        vecteur res;
        res.reserve(g._VECTptr->size());
        const_iterateur it=g._VECTptr->begin(),itend=g._VECTptr->end();
        for (;it!=itend;++it) {
            res.push_back(simplify_quadratic_Heaviside(*it,x,contextptr));
        }
        return gen(res,g.subtype);
    }
    gen a,b,c;
    if (g.is_symb_of_sommet(at_Heaviside)) {
        if (is_quadratic_wrt(g._SYMBptr->feuille,x,a,b,c,contextptr) && !is_zero(a)) {
            gen d=b*b-4*a*c;
            if (is_positive(-d,contextptr))
                return _Heaviside(a,contextptr);
            if (is_strictly_positive(d,contextptr)) {
                gen x1=(-b-sqrt(d,contextptr))/(2*a),x2=(-b+sqrt(d,contextptr))/(2*a);
                if (is_positive(a,contextptr))
                    return _Heaviside(x1-x,contextptr)+_Heaviside(x-x2,contextptr);
                if (is_positive(-a,contextptr))
                    return _boxcar(makesequence(x1,x2,x),contextptr);
            }
        }
        return g;
    }
    if (g.type==_SYMB)
        return symbolic(g._SYMBptr->sommet,simplify_quadratic_Heaviside(g._SYMBptr->feuille,x,contextptr));
    return g;
}

gen simplify_linarg(const gen &g,const identificateur &x,GIAC_CONTEXT) {
    if (g.type==_VECT) {
        vecteur res;
        res.reserve(g._VECTptr->size());
        const_iterateur it=g._VECTptr->begin(),itend=g._VECTptr->end();
        for (;it!=itend;++it) {
            res.push_back(simplify_linarg(*it,x,contextptr));
        }
        return gen(res,g.subtype);
    }
    gen a,b;
    if ((g.is_symb_of_sommet(at_Heaviside) || g.is_symb_of_sommet(at_sign)) &&
            is_linear_wrt(g._SYMBptr->feuille,x,a,b,contextptr) && !is_zero(a)) {
        b=symbolic(g._SYMBptr->sommet,g._SYMBptr->feuille/a);
        return g.is_symb_of_sommet(at_sign)?_sign(a,contextptr)*b:plus_one_half+_sign(a,contextptr)*(b+minus_one_half);
    }
    if (g.is_symb_of_sommet(at_Dirac) && g._SYMBptr->feuille.type!=_VECT &&
            is_linear_wrt(g._SYMBptr->feuille,x,a,b,contextptr) && !is_zero(a))
        return symbolic(at_Dirac,x+b/a)/_abs(a,contextptr);
    if (g.is_symb_of_sommet(at_Dirac) && g._SYMBptr->feuille.type==_VECT && g._SYMBptr->feuille._VECTptr->size()==2 &&
            g._SYMBptr->feuille._VECTptr->front().is_symb_of_sommet(at_neg) && g._SYMBptr->feuille._VECTptr->back().is_integer())
        return pow(gen(-1),g._SYMBptr->feuille._VECTptr->back().val)*
            symbolic(at_Dirac,makesequence(g._SYMBptr->feuille._VECTptr->front()._SYMBptr->feuille,g._SYMBptr->feuille._VECTptr->back()));
    if (g.type==_SYMB)
        return symbolic(g._SYMBptr->sommet,simplify_linarg(g._SYMBptr->feuille,x,contextptr));
    return g;
}

/* Linearize expressions with step functions wrt the variable x.
 * This rewrites products and powers of step functions as linear combinations of step functions. */
gen lin_Heaviside(const gen &g_orig,const identificateur &x,GIAC_CONTEXT) {
    gen ret(1),a,b,g(g_orig);
    if (g.type==_VECT) {
        vecteur res;
        res.reserve(g._VECTptr->size());
        const_iterateur it=g._VECTptr->begin(),itend=g._VECTptr->end();
        for (;it!=itend;++it) res.push_back(lin_Heaviside(*it,x,contextptr));
        return gen(res,g.subtype);
    }
    if (g.is_symb_of_sommet(at_plus) && g._SYMBptr->feuille.type==_VECT)
        return symbolic(at_plus,lin_Heaviside(g._SYMBptr->feuille,x,contextptr));
    if (g.is_symb_of_sommet(at_neg)) {
        ret=gen(-1);
        g=g._SYMBptr->feuille;
    }
    vecteur fac,L;
    if (g.is_symb_of_sommet(at_prod) && g._SYMBptr->feuille.type==_VECT)
        fac=*g._SYMBptr->feuille._VECTptr;
    else fac=vecteur(1,g);
    const_iterateur it=fac.begin(),itend=fac.end();
    bool recurse=false;
    for (;it!=itend;++it) {
        if (it->is_symb_of_sommet(at_pow) && is_strictly_positive(it->_SYMBptr->feuille._VECTptr->back(),contextptr) &&
                it->_SYMBptr->feuille._VECTptr->front().is_symb_of_sommet(at_Heaviside)) {
            ret=ret*it->_SYMBptr->feuille._VECTptr->front();
            recurse=true;
        } else if (L.size()<2 && it->is_symb_of_sommet(at_Heaviside) && is_linear_wrt(it->_SYMBptr->feuille,x,a,b,contextptr) && is_one(a))
            L.push_back(-b);
        else ret=*it*ret;
    }
    if (L.size()<2) {
        ret=ret*(L.size()==1?_Heaviside(x-L.front(),contextptr):gen(1));
        return recurse?lin_Heaviside(ret,x,contextptr):ret;
    }
    return lin_Heaviside(ret*_Heaviside(x-_max(gen(L,_SEQ__VECT),contextptr),contextptr),x,contextptr);
}

gen linH(const gen &g,const identificateur &x,GIAC_CONTEXT) {
    return lin_Heaviside(expand(simplify_linarg(g,x,contextptr),contextptr),x,contextptr);
}

gen factor_abs_arg(const gen &g,GIAC_CONTEXT) {
    if (g.type==_VECT) {
        vecteur res;
        res.reserve(g._VECTptr->size());
        const_iterateur it=g._VECTptr->begin(),itend=g._VECTptr->end();
        for (;it!=itend;++it) res.push_back(factor_abs_arg(*it,contextptr));
        return gen(res,g.subtype);
    }
    if (g.is_symb_of_sommet(at_abs))
        return _abs(_factor(factor_abs_arg(g._SYMBptr->feuille,contextptr),contextptr),contextptr);
    if (g.type==_SYMB)
        return symbolic(g._SYMBptr->sommet,factor_abs_arg(g._SYMBptr->feuille,contextptr));
    return g;
}

gen abs2Heaviside(const gen &g_orig,const identificateur &x,GIAC_CONTEXT) {
    vecteur la;
    gen g=factor_abs_arg(g_orig,contextptr);
    find_func_linarg(g,at_abs,x,la,true,contextptr);
    if (la.empty() || !is_numericv(*_evalf(la,contextptr)._VECTptr))
        return g_orig;
    la=*_sort(la,contextptr)._VECTptr;
    la.push_back(plus_inf);
    gen ret=0,last=minus_inf;
    const_iterateur it=la.begin(),itend=la.end();
    for (;it!=itend;++it) {
        ret+=eval_for_x_in_ab(g,x,last,*it,contextptr)*_boxcar(makesequence(last,*it,x),contextptr);
        last=*it;
    }
    return abs2Heaviside(ret,x,contextptr);
}

gen cond2Heaviside(const gen &g,const identificateur &x,GIAC_CONTEXT) {
    if ((g.is_symb_of_sommet(at_and) || g.is_symb_of_sommet(at_et)) && g._SYMBptr->feuille.type==_VECT) {
        gen res(1);
        const vecteur &args=*g._SYMBptr->feuille._VECTptr;
        for (const_iterateur it=args.begin();it!=args.end();++it) {
            res=res*cond2Heaviside(*it,x,contextptr);
        }
        return res;
    }
    if (g.is_symb_of_sommet(at_ou) && g._SYMBptr->feuille.type==_VECT) {
        gen res(1);
        const vecteur &args=*g._SYMBptr->feuille._VECTptr;
        for (const_iterateur it=args.begin();it!=args.end();++it) {
            res=res*(1-cond2Heaviside(*it,x,contextptr));
        }
        return 1-res;
    }
    if (g.is_symb_of_sommet(at_inferieur_egal) || g.is_symb_of_sommet(at_inferieur_strict)) {
        if (g._SYMBptr->feuille._VECTptr->front()==x &&
                is_const_wrt(g._SYMBptr->feuille._VECTptr->back(),x,contextptr))
            return _Heaviside(g._SYMBptr->feuille._VECTptr->back()-x,contextptr);
        if (g._SYMBptr->feuille._VECTptr->back()==x &&
                is_const_wrt(g._SYMBptr->feuille._VECTptr->front(),x,contextptr))
            return _Heaviside(x-g._SYMBptr->feuille._VECTptr->front(),contextptr);
    }
    if (g.is_symb_of_sommet(at_superieur_egal) || g.is_symb_of_sommet(at_superieur_strict)) {
        if (g._SYMBptr->feuille._VECTptr->front()==x &&
                is_const_wrt(g._SYMBptr->feuille._VECTptr->back(),x,contextptr))
            return _Heaviside(x-g._SYMBptr->feuille._VECTptr->back(),contextptr);
        if (g._SYMBptr->feuille._VECTptr->back()==x &&
                is_const_wrt(g._SYMBptr->feuille._VECTptr->front(),x,contextptr))
            return _Heaviside(g._SYMBptr->feuille._VECTptr->front()-x,contextptr);
    }
    return undef;
}

bool has_Dirac(const gen &g,const identificateur &x,gen &a,gen &b,int &k,gen &rest,GIAC_CONTEXT) {
    gen gk,fac;
    rest=gen(1);
    vecteur factors;
    if (g.is_symb_of_sommet(at_neg)) {
        bool res=has_Dirac(g._SYMBptr->feuille,x,a,b,k,rest,contextptr);
        rest=-rest;
        return res;
    }
    if (g.is_symb_of_sommet(at_prod) && g._SYMBptr->feuille.type==_VECT)
        factors=*g._SYMBptr->feuille._VECTptr;
    else factors=vecteur(1,g);
    const_iterateur it=factors.begin(),itend=factors.end();
    for (;it!=itend;++it) {
        if (it->is_symb_of_sommet(at_Dirac)) {
            const gen &arg=(it->_SYMBptr->feuille.type==_VECT?it->_SYMBptr->feuille._VECTptr->front()
                                                             :it->_SYMBptr->feuille);
            if (is_linear_wrt(arg,x,a,b,contextptr) && !is_zero(a) && is_zero(im(a,contextptr)) && is_zero(im(b,contextptr))) {
                gk=it->_SYMBptr->feuille.type==_VECT?it->_SYMBptr->feuille._VECTptr->back():gen(0);
                if (!gk.is_integer() || (k=gk.val)<0)
                    return false;
                rest=ratnormal(rest*symbolic(at_prod,vecteur(it+1,itend)),contextptr);
                return !contains(rest,at_Dirac);
            }
        }
        fac=*it;
        //if (fac.is_symb_of_sommet(at_abs))
        //    fac=fac._SYMBptr->feuille*(2*_Heaviside(fac,contextptr)-1);
        rest=rest*fac;
    }
    return false;
}

gen expand_Dirac(const gen &g,const identificateur &x,GIAC_CONTEXT) {
    if (g.type==_VECT) {
        vecteur res;
        res.reserve(g._VECTptr->size());
        const_iterateur it=g._VECTptr->begin(),itend=g._VECTptr->end();
        for (;it!=itend;++it) res.push_back(expand_Dirac(*it,x,contextptr));
        return gen(res,g.subtype);
    }
    if (g.is_symb_of_sommet(at_Dirac)) {
        const gen &arg=g._SYMBptr->feuille;
        if (is_constant_wrt(arg,x,contextptr))
            return g;
        if (arg.type!=_VECT && is_rational_wrt(arg,x)) {
            gen dz=_zeros(makesequence(_denom(arg,contextptr),x),contextptr);
            if (dz.type==_VECT && dz._VECTptr->empty()) {
                gen z=_zeros(makesequence(_numer(arg,contextptr),x),contextptr);
                if (z.type==_VECT) {
                    if (z._VECTptr->empty())
                        return 0;
                    gen h=_derive(makesequence(arg,x),contextptr),res(0);
                    const_iterateur it=z._VECTptr->begin(),itend=z._VECTptr->end();
                    for (;it!=itend;++it) {
                        gen hz=ratnormal(subst(h,x,*it,false,contextptr),contextptr);
                        if (is_exactly_zero(hz))
                            break;
                        res+=_Dirac(x-*it,contextptr)/_abs(hz,contextptr);
                    }
                    if (it==itend)
                        return res;
                }
            }
        }
    } else if (g.type==_SYMB)
        return symbolic(g._SYMBptr->sommet,expand_Dirac(g._SYMBptr->feuille,x,contextptr));
    return g;
}

gen simp_dirac_rat(const gen &g,const identificateur &x,const gen &z,int &n,GIAC_CONTEXT) {
    gen a,b,r,res;
    int k;
    if (is_monomial(g,x,k,r,a,b,contextptr) && is_exactly_zero(ratnormal(z+b/a,contextptr))) {
        if (k>n)
            return 0;
        res=(k%2?-1:1)*_comb(makesequence(n,k),contextptr)*_factorial(k,contextptr)*r;
        n-=k;
    } else if (is_monomial(_inv(g,contextptr),x,k,r,a,b,contextptr) && is_exactly_zero(ratnormal(z+b/a,contextptr))) {
        res=_inv((k%2?-1:1)*_comb(makesequence(n+k,k),contextptr)*_factorial(k,contextptr)*r,contextptr);
        n+=k;
    } else return g;
    return res;
}

gen simplify_Dirac(const gen &g_orig,const identificateur &x,const unary_function_ptr *simpfunc,GIAC_CONTEXT) {
    gen g=expand(g_orig,contextptr),r,ret(0),rest(0),zero_rest,z,s,rn,v,a,b,c,te;
    vecteur terms;
    if (g.is_symb_of_sommet(at_plus) && g._SYMBptr->feuille.type==_VECT)
        terms=*g._SYMBptr->feuille._VECTptr;
    else terms=vecteur(1,g);
    gen_map dterms;
    int k,n;
    for (const_iterateur it=terms.begin();it!=terms.end();++it) {
        if (has_Dirac(*it,x,a,b,n,r,contextptr))
            dterms[makevecteur(-b/a,n)]+=r/(_abs(a,contextptr)*pow(a,n));
        else rest+=*it;
    }
    int dom;
    matrice intervals;
    vecteur excluded;
    if (!get_assumptions(x,dom,intervals,excluded,contextptr)) {
        dom=1; // assume real
        intervals=vecteur(1,makevecteur(minus_inf,plus_inf));
    } else if (dom==4) { // complex
        *logptr(contextptr) << gettext("Warning") << ": " << gettext("assuming") << " " << x << " " << gettext("is real") << "\n";
        giac_additionally(makesequence(x,1),contextptr);
        dom=1;
    }
    for (gen_map::const_iterator it=dterms.begin();it!=dterms.end();++it) {
        const vecteur &d=*it->first._VECTptr;
        const gen &z=d.front(),&r=it->second;
        n=d.back().val;
        if (contains(excluded,z) || !indomain(intervals,z,contextptr) || (dom==2 && !is_inZ(z,contextptr)) ||
                (dom==10 && (!is_inZ(_numer(z,contextptr),contextptr) || !is_inZ(_denom(z,contextptr),contextptr))))
            continue;
        for (int p=0;p<2;++p) {
            gen rp=p==0?re(r,contextptr):im(r,contextptr);
            try {
                te=_taylor(makesequence(rp,symb_equal(x,z),std::max(1,n),change_subtype(_POLY1__VECT,_INT_MAPLECONVERSION)),contextptr);
            } catch (const gen &err) {
                *logptr(contextptr) << err << "\n";
                ret+=rp*_Dirac(n==0?x-z:makesequence(x-z,n),contextptr);
                continue;
            }
            vecteur tt=te.is_symb_of_sommet(at_plus) && te._SYMBptr->feuille.type==_VECT?
                *te._SYMBptr->feuille._VECTptr:vecteur(1,te);
            for (const_iterateur jt=tt.begin();jt!=tt.end();++jt) {
                k=n;
                if (!is_exactly_zero(c=simp_dirac_rat(*jt,x,z,k,contextptr)))
                    ret+=(p==0?1:cst_i)*c*_Dirac(k==0?x-z:makesequence(x-z,k),contextptr);
            }
        }
    }
    collect_with_func(ret,at_Dirac,&dterms,&zero_rest,contextptr);
    ret=0;
    for (gen_map::const_iterator it=dterms.begin();it!=dterms.end();++it) {
        ret+=(simpfunc==NULL?it->second:_eval(symbolic(simpfunc,it->second),contextptr))*_Dirac(it->first,contextptr);
    }
    rest+=zero_rest;
    return ret+(simpfunc==NULL?rest:_eval(symbolic(simpfunc,rest),contextptr));
}

gen simpfactor(const gen &g,GIAC_CONTEXT) {
    //return factorise(_tcollect(ratnormal(_texpand(g,contextptr),contextptr),contextptr),contextptr);
    return ratnormal(g,contextptr);
}

gen simpcollect(const gen &g,const unary_function_ptr *f,GIAC_CONTEXT) {
    gen_map m;
    gen rest,ret=0;
    collect_with_func(g,f,&m,&rest,contextptr);
    gen_map::const_iterator mt=m.begin(),mtend=m.end();
    for (;mt!=mtend;++mt) {
        ret+=symbolic(f,mt->first)*simpfactor(mt->second,contextptr);
    }
    return ret+simpfactor(rest,contextptr);
}

gen _simplifyDirac(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    bool has_vars=g.type==_VECT && g.subtype==_SEQ__VECT;
    if (has_vars && g._VECTptr->size()!=2)
        return gendimerr(contextptr);
    const gen &e=has_vars?g._VECTptr->front():g;
    if (count_func_instances(e,at_Dirac)==0)
        return e;
    gen x=identificateur("x"),ret;
    if (has_vars)
        x=g._VECTptr->back();
    if (x.type!=_IDNT)
        return gentypeerr(contextptr);
    try {
        ret=simplify_Dirac(expand_Dirac(e,*x._IDNTptr,contextptr),*x._IDNTptr,at_simplify,contextptr);
    } catch (const gen &err) {
        *logptr(contextptr) << e << "\n";
        return e;
    }
    return ret;
}
static const char _simplifyDirac_s []="simplifyDirac";
static define_unary_function_eval (__simplifyDirac,&_simplifyDirac,_simplifyDirac_s);
define_unary_function_ptr5(at_simplifyDirac,alias_at_simplifyDirac,&__simplifyDirac,0,true)

gen _sign2Heaviside(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    return sign2Heaviside(g);
}
static const char _sign2Heaviside_s []="sign2Heaviside";
static define_unary_function_eval (__sign2Heaviside,&_sign2Heaviside,_sign2Heaviside_s);
define_unary_function_ptr5(at_sign2Heaviside,alias_at_sign2Heaviside,&__sign2Heaviside,0,true)

gen _Heaviside2sign(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    return Heaviside2sign(g);
}
static const char _Heaviside2sign_s []="Heaviside2sign";
static define_unary_function_eval (__Heaviside2sign,&_Heaviside2sign,_Heaviside2sign_s);
define_unary_function_ptr5(at_Heaviside2sign,alias_at_Heaviside2sign,&__Heaviside2sign,0,true)

gen _linstep(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    bool has_var=g.type==_VECT && g.subtype==_SEQ__VECT;
    gen x=identificateur("x");
    if (has_var) {
        if (g._VECTptr->size()!=2)
            return gendimerr(contextptr);
        x=g._VECTptr->back();
    }
    if (x.type!=_IDNT)
        return gentypeerr(contextptr);
    const gen &f=has_var?g._VECTptr->front():g;
    gen res=simplify_quadratic_Heaviside(abs2Heaviside(sign2Heaviside(
        flatten_piecewise(f,contextptr)),*x._IDNTptr,contextptr),*x._IDNTptr,contextptr);
    res=linH(res,*x._IDNTptr,contextptr);
    return simpcollect(res,at_Heaviside,contextptr);
}
static const char _linstep_s []="linstep";
static define_unary_function_eval (__linstep,&_linstep,_linstep_s);
define_unary_function_ptr5(at_linstep,alias_at_linstep,&__linstep,0,true)

gen _linabs(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    bool has_var=g.type==_VECT && g.subtype==_SEQ__VECT;
    gen x=identificateur("x");
    if (has_var) {
        if (g._VECTptr->size()!=2)
            return gendimerr(contextptr);
        x=g._VECTptr->back();
    }
    if (x.type!=_IDNT)
        return gentypeerr(contextptr);
    const gen &f=has_var?g._VECTptr->front():g;
    gen res=simplify_quadratic_Heaviside(abs2Heaviside(sign2Heaviside(
        flatten_piecewise(f,contextptr)),*x._IDNTptr,contextptr),*x._IDNTptr,contextptr);
    res=reduce_Heaviside(linH(res,*x._IDNTptr,contextptr),*x._IDNTptr,contextptr);
    gen res0(res);
    int s,t0=taille(res,MAX_TAILLE),t=t0;
    while (true) {
        gen smp=recursive_normal(res,contextptr);
        if ((s=taille(smp,MAX_TAILLE))==t)
            return collect_with_func(s<t0?res:res0,at_abs,NULL,NULL,contextptr);
        res=smp;
        t=s;
    }
    return res0; // unreachable
}
static const char _linabs_s []="linabs";
static define_unary_function_eval (__linabs,&_linabs,_linabs_s);
define_unary_function_ptr5(at_linabs,alias_at_linabs,&__linabs,0,true)

/* Returns an equivalent univariate piecewise expression depending on the variable x. */
gen to_piecewise(const gen &g_orig,const identificateur &x,GIAC_CONTEXT) {
    gen g=linH(simplify_quadratic_Heaviside(abs2Heaviside(sign2Heaviside(g_orig),x,contextptr),x,contextptr),x,contextptr);
    g=collect_with_func(g,at_Heaviside,NULL,NULL,contextptr);
    vecteur z,p;
    find_func_linarg(g,at_Heaviside,x,z,false,contextptr);
    if (!is_numericv(*_evalf(z,contextptr)._VECTptr))
        return g;
    z=*_sort(z,contextptr)._VECTptr;
    int n=z.size();
    z.push_back(plus_inf);
    p.reserve(n+1);
    gen last=minus_inf,h;
    const_iterateur it=z.begin(),itend=z.end();
    for (;it!=itend;++it) {
        h=ratnormal(eval_for_x_in_ab(g,x,last,*it,contextptr),contextptr);
        if (contains(h,at_Heaviside))
            return g;
        p.push_back(_lin(h,contextptr));
        last=*it;
    }
    vecteur args;
    args.reserve(2*n+1);
    for (int i=0;i<n;++i) {
        args.push_back(symb_inferieur_strict(x,z[i]));
        args.push_back(p[i]);
    }
    args.push_back(p.back());
    return args.size()>1?symbolic(at_piecewise,gen(args,_SEQ__VECT)):args.front();
}

gen flatten_piecewise_recursion(const gen &g,GIAC_CONTEXT) {
    if (g.type==_VECT) {
        vecteur res;
        res.reserve(g._VECTptr->size());
        const_iterateur it=g._VECTptr->begin(),itend=g._VECTptr->end();
        for (;it!=itend;++it) res.push_back(flatten_piecewise_recursion(*it,contextptr));
        return gen(res,g.subtype);
    }
    if (g.is_symb_of_sommet(at_piecewise)) {
        if (g._SYMBptr->feuille.type!=_VECT)
            return g._SYMBptr->feuille;
        const vecteur &args=*g._SYMBptr->feuille._VECTptr;
        int n=args.size()-1,i=0;
        gen ret(0),last(1),d;
        for (;i<n;i+=2) {
            const gen &ci=args[i],&eci=args[i+1];
            if (ci.is_symb_of_sommet(at_superieur_egal) || ci.is_symb_of_sommet(at_superieur_strict)) {
                ret+=last*_Heaviside(d=ci._SYMBptr->feuille._VECTptr->front()-ci._SYMBptr->feuille._VECTptr->back(),contextptr)*eci;
                last=last*_Heaviside(-d,contextptr);
            } else if (ci.is_symb_of_sommet(at_inferieur_egal) || ci.is_symb_of_sommet(at_inferieur_strict)) {
                ret+=last*_Heaviside(d=ci._SYMBptr->feuille._VECTptr->back()-ci._SYMBptr->feuille._VECTptr->front(),contextptr)*eci;
                last=last*_Heaviside(-d,contextptr);
            } else return g;
        }
        return ret+(n%2?0:last*args.back());
    }
    if (g.is_symb_of_sommet(at_when)) {
        if (g._SYMBptr->feuille.type!=_VECT || g._SYMBptr->feuille._VECTptr->size()!=3)
            return g;
        const gen &c=g._SYMBptr->feuille._VECTptr->front();
        gen d;
        if (c.is_symb_of_sommet(at_superieur_egal) || c.is_symb_of_sommet(at_superieur_strict)) {
            d=c._SYMBptr->feuille._VECTptr->front()-c._SYMBptr->feuille._VECTptr->back();
            return _Heaviside(d,contextptr)*g._SYMBptr->feuille._VECTptr->at(1)+_Heaviside(-d,contextptr)*g._SYMBptr->feuille._VECTptr->at(2);
        }
        if (c.is_symb_of_sommet(at_inferieur_egal) || c.is_symb_of_sommet(at_inferieur_strict)) {
            d=c._SYMBptr->feuille._VECTptr->back()-c._SYMBptr->feuille._VECTptr->front();
            return _Heaviside(d,contextptr)*g._SYMBptr->feuille._VECTptr->at(1)+_Heaviside(-d,contextptr)*g._SYMBptr->feuille._VECTptr->at(2);
        }
        return g;
    }
    if (g.type==_SYMB)
        return symbolic(g._SYMBptr->sommet,flatten_piecewise_recursion(g._SYMBptr->feuille,contextptr));
    return g;
}

gen flatten_piecewise(const gen &g,GIAC_CONTEXT) {
    gen ret(g);
    while (contains(ret,at_piecewise) || contains(ret,at_when)) {
        ret=flatten_piecewise_recursion(ret,contextptr);
    }
    return ret;
}

/* return true iff g = a*|x+b| */
bool is_linabs(const gen &g_orig,const identificateur &x,gen &a,gen &b,GIAC_CONTEXT) {
    gen g=factorise(g_orig,contextptr),A(0),B;
    a=gen(1);
    if (g.is_symb_of_sommet(at_neg)) {
        a=gen(-1);
        g=g._SYMBptr->feuille;
    }
    vecteur factors;
    if (g.is_symb_of_sommet(at_prod) && g._SYMBptr->feuille.type==_VECT)
        factors=*g._SYMBptr->feuille._VECTptr;
    else factors=vecteur(1,g);
    for (const_iterateur it=factors.begin();it!=factors.end();++it) {
        if (is_const_wrt(*it,x,contextptr))
            a=a*(*it);
        else if (is_zero(A) && it->is_symb_of_sommet(at_abs) &&
                    is_linear_wrt(it->_SYMBptr->feuille,x,A,B,contextptr) && !is_zero(A)) {
            a=a*_abs(A,contextptr);
            b=B/A;
        } else return false;
    }
    return true;
}

iterateur find_lin(vecteur &lv,const gen &key,GIAC_CONTEXT) {
    iterateur it=lv.begin();
    for (;it!=lv.end();++it) {
        if (is_same(key,it->_VECTptr->front(),contextptr))
            break;
    }
    return it;
}

void extract_linpow(const gen &g,const identificateur &x,
                    const gen &e,gen &c,vecteur &lv,gen &rest,GIAC_CONTEXT) {
    gen a,b;
    if (is_const_wrt(g,x,contextptr))
        c=c*_pow(makesequence(g,e),contextptr);
    else if (g.is_symb_of_sommet(at_neg)) {
        c=c*_pow(makesequence(-1,e),contextptr);
        extract_linpow(g._SYMBptr->feuille,x,e,c,lv,rest,contextptr);
    } else if (g.is_symb_of_sommet(at_prod) && g._SYMBptr->feuille.type==_VECT) {
        const_iterateur it=g._SYMBptr->feuille._VECTptr->begin();
        for (;it!=g._SYMBptr->feuille._VECTptr->end();++it) {
            extract_linpow(*it,x,e,c,lv,rest,contextptr);
        }
    } else if (g.is_symb_of_sommet(at_pow)) {
        const vecteur &v=*g._SYMBptr->feuille._VECTptr;
        extract_linpow(v.front(),x,e*v.back(),c,lv,rest,contextptr);
    } else if (g.is_symb_of_sommet(at_sqrt)) {
        extract_linpow(g._SYMBptr->feuille,x,e/2,c,lv,rest,contextptr);
    } else if (g.is_symb_of_sommet(at_abs) && is_linear_wrt(g._SYMBptr->feuille,x,a,b,contextptr)) {
        c=c*_pow(makesequence(_abs(a,contextptr),e),contextptr);
        iterateur it=find_lin(lv,b/a,contextptr);
        if (it==lv.end())
            lv.push_back(makevecteur(b/a,0,e));
        else (*(it->_VECTptr))[2]+=e;
    } else if (is_linear_wrt(g,x,a,b,contextptr)) {
        c=c*_pow(makesequence(a,e),contextptr);
        iterateur it=find_lin(lv,b/a,contextptr);
        if (it==lv.end())
            lv.push_back(makevecteur(b/a,e,0));
        else (*(it->_VECTptr))[1]+=e;
    } else rest=rest*_pow(makesequence(g,e),contextptr);
}

bool is_linpow(const gen &g,const identificateur &x,gen &a,gen &b,gen &c,bool &s,GIAC_CONTEXT) {
    gen c1(1),c2(1),r1(1),r2(1),h=simplify_linarg(g,x,contextptr);
    vecteur lv1,lv2;
    gen num=_numer(h,contextptr),den=_denom(h,contextptr);
    if (!is_const_wrt(num,x,contextptr)) num=factorise(num,contextptr);
    if (!is_const_wrt(den,x,contextptr)) den=factorise(den,contextptr);
    extract_linpow(num,x,1,c1,lv1,r1,contextptr);
    extract_linpow(den,x,1,c2,lv2,r2,contextptr);
    if (lv1.size()>1 || lv2.size()>1)
        return false;
    if (!lv2.empty()) {
        const vecteur &v=*lv2.front()._VECTptr;
        iterateur it=find_lin(lv1,v.front(),contextptr);
        if (it==lv1.end())
            lv1.push_back(makevecteur(v[0],-v[1],-v[2]));
        else {
            vecteur &w=*(it->_VECTptr);
            w[1]-=v[1];
            w[2]-=v[2];
        }
    }
    if (lv1.size()!=1)
        return false;
    b=ratnormal(lv1.front()._VECTptr->front(),contextptr);
    c=c1/c2;
    gen rest=r1/r2,a1=lv1.front()._VECTptr->at(1),a2=lv1.front()._VECTptr->at(2),A,B;
    if (!is_zero(im(a1,contextptr)) || !is_zero(im(a2,contextptr)) || !a1.is_integer())
        return false;
    a=a1+a2;
    if (is_strictly_greater(a,-1,contextptr)) {
        if (is_zero(_even(a1,contextptr)))
            rest=rest*_sign(x+b,contextptr);
    } else {
        if (!a2.is_integer())
            return false;
        if (is_zero(_even(a2,contextptr)))
            rest=rest*_sign(x+b,contextptr);
    }
    rest=ratnormal(rest,contextptr);
    if (is_const_wrt(rest,x,contextptr)) {
        c=c*rest;
        s=false;
    } else if (rest.is_symb_of_sommet(at_sign) &&
               is_linear_wrt(rest._SYMBptr->feuille,x,A,B,contextptr) && is_one(A) &&
               is_same(B/A,b,contextptr)) {
        s=true;          
    } else return false;
    return true;
}

bool is_expexp(const gen &g_orig,const identificateur &x,gen &a,gen &b,GIAC_CONTEXT) {
    gen g=expand(g_orig,contextptr);
    if (!g.is_symb_of_sommet(at_plus) || g._SYMBptr->feuille.type!=_VECT)
        return false;
    const vecteur &v=*g._SYMBptr->feuille._VECTptr;
    gen t1(0),t2(undef),a1,b1,a2,b2;
    for (const_iterateur it=v.begin();it!=v.end();++it) {
        if (is_undef(t2) && it->is_symb_of_sommet(at_neg) &&
                 it->_SYMBptr->feuille.is_symb_of_sommet(at_exp) &&
                 is_linear_wrt(it->_SYMBptr->feuille._SYMBptr->feuille,x,a2,b2,contextptr))
            t2=*it;
        else t1+=*it;
    }
    if (!is_linear_wrt(t1,x,a1,b1,contextptr) || !is_zero(b1) || !is_same(a1,a2,contextptr))
        return false;
    a=a1;
    b=b2;
    return true;
}

int is_expsum(const gen &g_orig,const identificateur &x,gen &a,gen &b,gen &c,bool &sq,GIAC_CONTEXT) {
    gen g=_lin(expand(g_orig,contextptr),contextptr);
    c=gen(1);
    if (g.is_symb_of_sommet(at_neg)) {
        c=gen(-1);
        g=g._SYMBptr->feuille;
    }
    vecteur factors;
    vecteur A(2),B(2),S(3);
    gen rpart,ipart,p;
    int i=0;
    sq=false;
    if (g.is_symb_of_sommet(at_prod) && g._SYMBptr->feuille.type==_VECT)
        factors=*g._SYMBptr->feuille._VECTptr;
    else factors=vecteur(1,g);
    for (const_iterateur it=factors.begin();it!=factors.end();++it) {
        if (is_const_wrt(*it,x,contextptr))
            c=c*(*it);
        else if (it->is_symb_of_sommet(at_plus) && it->_SYMBptr->feuille.type==_VECT &&
                 it->_SYMBptr->feuille._VECTptr->size()>=2) {
            rpart=re(*it,contextptr);
            ipart=im(*it,contextptr);
            if (is_zero(rpart)) {
                p=ipart;
                c=c*cst_i;
            } else if (is_zero(ipart))
                p=rpart;
            else return 0;
            if (!p.is_symb_of_sommet(at_plus) || p._SYMBptr->feuille.type!=_VECT ||
                    p._SYMBptr->feuille._VECTptr->size()<2)
                return 0;
            const vecteur &terms=*(p._SYMBptr->feuille._VECTptr);
            for (const_iterateur jt=terms.begin();jt!=terms.end();++jt) {
                if (i<2 && jt->is_symb_of_sommet(at_exp) &&
                        is_linear_wrt(jt->_SYMBptr->feuille,x,A[i],B[i],contextptr)) {
                    S[i]=gen(1);
                    ++i;
                } else if (i<2 && jt->is_symb_of_sommet(at_neg) &&
                        jt->_SYMBptr->feuille.is_symb_of_sommet(at_exp) &&
                        is_linear_wrt(jt->_SYMBptr->feuille._SYMBptr->feuille,x,A[i],B[i],contextptr)) {
                    S[i]=gen(-1);
                    ++i;
                } else if (!sq && is_one(_abs(S[2]=*jt/2,contextptr)) && terms.size()==3)
                    sq=true;
                else return 0;
            }
        } else return 0;
    }
    if (i<2 || !is_zero(ratnormal(A[0]+A[1],contextptr)) || !is_zero(ratnormal(B[0]+B[1],contextptr)))
        return 0;
    if (sq) {
        if (!is_one(S[0]) || !is_one(S[1]))
            return false;
        a=A[0]/2;
        b=B[0]/2;
        return 1+(1+S[2]).val/2;
    }
    c=c*S[0];
    a=A[0];
    b=B[0];
    return 1+int(is_one(S[0]*S[1]));
}

bool is_integral(const gen &g,identificateur &x,gen &f,gen &a,gen &b,GIAC_CONTEXT) {
    if ((!g.is_symb_of_sommet(at_Int) && !g.is_symb_of_sommet(at_int) &&
            !g.is_symb_of_sommet(at_integrate)) || g._SYMBptr->feuille.type!=_VECT)
        return false;
    const vecteur &args=*g._SYMBptr->feuille._VECTptr;
    if (args.size()!=2 && args.size()!=4)
        return false;
    f=args.front();
    if (args.size()==2) {
        if (!is_equal(args.back()) ||
                args.back()._SYMBptr->feuille._VECTptr->front().type!=_IDNT ||
                !args.back()._SYMBptr->feuille._VECTptr->back().is_symb_of_sommet(at_interval))
            return false;
        x=*args.back()._SYMBptr->feuille._VECTptr->front()._IDNTptr;
        a=args.back()._SYMBptr->feuille._VECTptr->back()._SYMBptr->feuille._VECTptr->front();
        b=args.back()._SYMBptr->feuille._VECTptr->back()._SYMBptr->feuille._VECTptr->back();
        return true;
    }
    if (args[1].type!=_IDNT)
        return false;
    x=*args[1]._IDNTptr;
    a=args[2];
    b=args[3];
    return true;
}

bool is_convolution(const gen &e,const identificateur &x,gen &f,gen &g,GIAC_CONTEXT) {
    gen a,b,h;
    identificateur t;
    if (!is_integral(e,t,h,a,b,contextptr))
        return false;
    if (!is_inf(a))
        h=h*_Heaviside(t-a,contextptr);
    if (!is_inf(b))
        h=h*_Heaviside(b-t,contextptr);
    vecteur factors;
    if (h.is_symb_of_sommet(at_prod) && h._SYMBptr->feuille.type==_VECT)
        factors=*h._SYMBptr->feuille._VECTptr;
    else factors=vecteur(1,h);
    f=g=gen(1);
    for (const_iterateur it=factors.begin();it!=factors.end();++it) {
        if (is_const_wrt(*it,x,contextptr))
            f=f*(*it);
        else g=g*(*it);
    }
    f=subst(f,t,x,false,contextptr);
    g=recursive_normal(subst(g,x,x+t,false,contextptr),contextptr);
    return is_const_wrt(g,t,contextptr);
}

vecteur analyze_terms(const gen &g,const identificateur &x,GIAC_CONTEXT) {
    vecteur terms,ret;
    if (g.is_symb_of_sommet(at_plus)) {
        if (g._SYMBptr->feuille.type==_VECT)
            terms=*g._SYMBptr->feuille._VECTptr;
        else terms=vecteur(1,g._SYMBptr->feuille);
    } else terms=vecteur(1,g);
    for (const_iterateur it=terms.begin();it!=terms.end();++it) {
        vecteur factors;
        gen cnst(1),rest(1),exprest(0),sh(0),rt=ratnormal(*it,contextptr);
        int deg=0,st;
        if (!is_const_wrt(rt,x,contextptr))
            rt=factorise(rt,contextptr);
        if (rt.is_symb_of_sommet(at_neg)) {
            cnst=gen(-1);
            rt=rt._SYMBptr->feuille;
        }
        if (rt.is_symb_of_sommet(at_prod) && rt._SYMBptr->feuille.type==_VECT)
            factors=*rt._SYMBptr->feuille._VECTptr;
        else factors=vecteur(1,rt);
        for (int i=factors.size();i-->0;) {
            gen &fac=factors[i];
            if (fac.is_symb_of_sommet(at_inv)) {
                if (fac._SYMBptr->feuille.is_symb_of_sommet(at_prod) &&
                    fac._SYMBptr->feuille._SYMBptr->feuille.type==_VECT) {
                    const vecteur &d=*fac._SYMBptr->feuille._SYMBptr->feuille._VECTptr;
                    for (const_iterateur jt=d.begin();jt!=d.end();++jt) {
                        factors.push_back(symbolic(at_inv,*jt));
                    }
                    factors.erase(factors.begin()+i);
                }
            }
        }
        for (const_iterateur jt=factors.begin();jt!=factors.end();++jt) {
            if (jt->is_symb_of_sommet(at_exp)) {
                gen lt1,lt2,c1,c2,rest1,rest2;
                constlin_terms(re(jt->_SYMBptr->feuille,contextptr),x,lt1,c1,rest1,contextptr);
                constlin_terms(im(jt->_SYMBptr->feuille,contextptr),x,lt2,c2,rest2,contextptr);
                cnst=cnst*exp(c1+cst_i*c2,contextptr);
                sh+=lt2;
                exprest+=rest1+x*lt1+cst_i*rest2;
            } else if (is_const_wrt(*jt,x,contextptr)) {
                cnst=cnst*(*jt);
            } else if (ispoly(*jt,x,st,contextptr)) {
                deg+=st;
                cnst=cnst*_lcoeff(makesequence(*jt,x),contextptr);
            } else rest=rest*(*jt);
        }
        rest=rest*exp(exprest,contextptr);
        cnst=recursive_normal(_lin(cnst,contextptr),contextptr);
        ret.push_back(makevecteur(cnst,sh,deg,rest));
    }
    return ret;
}

vecteur fourier_terms(const gen &g_orig,const identificateur &x,bool do_simp,GIAC_CONTEXT) {
    gen g=_lin(_trig2exp(_hyp2exp(_pow2exp(g_orig,contextptr),contextptr),contextptr),contextptr);
    if (do_simp) {
        g=ratnormal(logabs_expand(lnexpand(g,contextptr),x,contextptr),contextptr);
        if (contains(g,at_Dirac))
            g=simplify_Dirac(g,x,NULL,contextptr);
    }
    g=expand(g,contextptr);
    vecteur terms=analyze_terms(g,x,contextptr);
    // decompose terms into sums of partial fractions
    for (int i=terms.size();i-->0;) {
        assert(terms[i].type==_VECT);
        vecteur &v=*terms[i]._VECTptr;
        if (!is_rational_wrt(v[3],x) || is_const_wrt(_denom(v[3],contextptr),x,contextptr) || !v[2].is_integer())
            continue;
        v[3]=v[3]*pow(x,v[2].val);
        v[2]=0;
        vecteur rterms=analyze_terms(_cpartfrac(makesequence(v[3],x),contextptr),x,contextptr);
        for (const_iterateur it=rterms.begin();it!=rterms.end();++it) {
            assert(it->type==_VECT);
            const vecteur &w=*(it->_VECTptr);
            terms.push_back(makevecteur(v[0]*w[0],v[1],v[2]+w[2],expand(w[3],contextptr)));
        }
        terms.erase(terms.begin()+i);
    }
    // collect terms to reduce complexity
    for (int i=0;i<int(terms.size());++i) {
        vecteur &v=*terms[i]._VECTptr;
        for (int j=terms.size();j-->i+1;) {
            const vecteur &w=*terms[j]._VECTptr;
            if (is_same(v[3],w[3],contextptr) && is_same(v[1],w[1],contextptr) && is_same(v[2],w[2],contextptr)) {
                v[0]+=w[0];
                terms.erase(terms.begin()+j);
            }
        }
    }
    return terms;
}

int fourier_func_type(const gen &g,const identificateur &x,vecteur &params,GIAC_CONTEXT) {
    params.clear();
    if (is_const_wrt(g,x,contextptr)) {
        params.push_back(g);
        return _FOURIER_FUNCTYPE_ONE;
    }
    gen a,b,c,d;
    vecteur tbl,vars,deg;
    identificateur fcn;
    bool sgn,sq;
    int dg,hyp,k,n;
    if (is_rational_wrt(g,x)) {
        gen num=_numer(g,contextptr),den=_denom(g,contextptr);
        if(!is_const_wrt(num,x,contextptr))
            return _FOURIER_FUNCTYPE_UNKNOWN;
        if (is_linear_wrt(den,x,a,b,contextptr)) {
            params=makevecteur(num/a,b/a,1);
        } else if (is_monomial(den,x,n,a,b,c,contextptr)) {
            //assert(!is_zero(a) && !is_zero(b));
            params=makevecteur(num/(a*pow(b,n)),c/b,n);
        } else return _FOURIER_FUNCTYPE_UNKNOWN;
        return _FOURIER_FUNCTYPE_INV_MONOM;
    }
    if (g.is_symb_of_sommet(at_sign) && is_linear_wrt(g._SYMBptr->feuille,x,a,b,contextptr) &&
            is_zero(im(a,contextptr)) && is_zero(im(b,contextptr))) {
        params=makevecteur(a,b);
        return _FOURIER_FUNCTYPE_SGN;
    }
    if (g.is_symb_of_sommet(at_Dirac) && ((g._SYMBptr->feuille.type==_VECT &&
            is_linear_wrt(g._SYMBptr->feuille._VECTptr->front(),x,a,b,contextptr)) ||
            is_linear_wrt(g._SYMBptr->feuille,x,a,b,contextptr)) &&
            is_zero(im(a,contextptr)) && is_zero(im(b,contextptr))) {
        params=makevecteur(a,b/a,
            g._SYMBptr->feuille.type==_VECT?g._SYMBptr->feuille._VECTptr->back():gen(0));
        return _FOURIER_FUNCTYPE_DIRAC;
    }
    if (has_Dirac(g,x,a,b,k,c,contextptr)) {
        params=makevecteur(a,b,k,c);
        return _FOURIER_FUNCTYPE_DIRAC_F;
    }
    if (g.is_symb_of_sommet(at_BesselJ) && g._SYMBptr->feuille.type==_VECT &&
            g._SYMBptr->feuille._VECTptr->size()==2 &&
            (c=g._SYMBptr->feuille._VECTptr->front()).is_integer() && is_positive(c,contextptr) &&
            is_linear_wrt(g._SYMBptr->feuille._VECTptr->back(),x,a,b,contextptr) &&
            is_zero(im(a,contextptr)) && is_zero(im(b,contextptr))) {
        params=makevecteur(a,b,c);
        return _FOURIER_FUNCTYPE_BESSELJ;
    }
    if (g.is_symb_of_sommet(at_ln) && g._SYMBptr->feuille.is_symb_of_sommet(at_abs) &&
            is_linear_wrt(g._SYMBptr->feuille._SYMBptr->feuille,x,a,b,contextptr) &&
            is_zero(im(a,contextptr)) && is_zero(im(b,contextptr))) {
        params.push_back(b/a);
        return _FOURIER_FUNCTYPE_LOGABSX;
    }
    if (g.is_symb_of_sommet(at_inv) && g._SYMBptr->feuille.is_symb_of_sommet(at_abs) &&
            is_linear_wrt(g._SYMBptr->feuille._SYMBptr->feuille,x,a,b,contextptr) &&
            is_zero(im(a,contextptr)) && is_zero(im(b,contextptr))) {
        params=makevecteur(symbolic(at_inv,_abs(a,contextptr)),b/a);
        return _FOURIER_FUNCTYPE_INVABSX;
    }
    if (is_linpow(g,x,a,b,c,sgn,contextptr) && is_zero(im(b,contextptr))) {
        if (!sgn && is_greater(-1,a,contextptr)) {
            params=makevecteur(c,b,-a);
            return _FOURIER_FUNCTYPE_INV_MONOM;
        }
        params=makevecteur(a,b,c,sgn?gen(1):gen(0));
        return _FOURIER_FUNCTYPE_ABSX_ALPHA;
    }
    if (g.is_symb_of_sommet(at_exp) &&
            is_poly2(g._SYMBptr->feuille,x,a,b,c,contextptr) &&
            is_zero(im(b,contextptr)) && is_positive(re(-a,contextptr),contextptr)) {
        params=makevecteur(exp(c,contextptr),b,-a);
        return _FOURIER_FUNCTYPE_GAUSSIAN;
    }
    if (g.is_symb_of_sommet(at_Gamma) && is_one(re(g._SYMBptr->feuille,contextptr)) &&
            is_linear_wrt(im(g._SYMBptr->feuille,contextptr),x,a,b,contextptr)) {
        params=makevecteur(a,b/a);
        return _FOURIER_FUNCTYPE_GAMMA;
    }
    if (g.is_symb_of_sommet(at_prod) && g._SYMBptr->feuille.type==_VECT &&
            g._SYMBptr->feuille._VECTptr->size()==2 &&
            ((g._SYMBptr->feuille._VECTptr->front().is_symb_of_sommet(at_exp) &&
              is_linear_wrt(g._SYMBptr->feuille._VECTptr->front()._SYMBptr->feuille,x,a,d,contextptr) &&
              g._SYMBptr->feuille._VECTptr->back().is_symb_of_sommet(at_Heaviside) &&
              is_linear_wrt(g._SYMBptr->feuille._VECTptr->back()._SYMBptr->feuille,x,b,c,contextptr)) ||
             (g._SYMBptr->feuille._VECTptr->back().is_symb_of_sommet(at_exp) &&
              is_linear_wrt(g._SYMBptr->feuille._VECTptr->back()._SYMBptr->feuille,x,a,d,contextptr) &&
              g._SYMBptr->feuille._VECTptr->front().is_symb_of_sommet(at_Heaviside) &&
              is_linear_wrt(g._SYMBptr->feuille._VECTptr->front()._SYMBptr->feuille,x,b,c,contextptr)))
              && is_zero(im(b,contextptr)) && is_zero(im(c,contextptr)) && !is_zero(b) && is_zero(d) &&
              is_strictly_positive(-a/b,contextptr)) {
        params=makevecteur(a,b,c);
        return _FOURIER_FUNCTYPE_EXP_HEAVISIDE;
    }
    if (g.is_symb_of_sommet(at_Heaviside) && is_linear_wrt(g._SYMBptr->feuille,x,a,b,contextptr) &&
            is_zero(im(a,contextptr)) && is_zero(im(b,contextptr))) {
        params=makevecteur(a,b/a);
        return _FOURIER_FUNCTYPE_HEAVISIDE;
    }
    if (g.is_symb_of_sommet(at_exp) && is_linabs(g._SYMBptr->feuille,x,a,b,contextptr) &&
            is_strictly_positive(-a,contextptr)) {
        params=makevecteur(-a,b);
        return _FOURIER_FUNCTYPE_EXPABSX;
    }
    if (g.is_symb_of_sommet(at_exp) && is_expexp(g._SYMBptr->feuille,x,a,b,contextptr) &&
            is_zero(im(a,contextptr)) && is_zero(im(b,contextptr))) {
        params=makevecteur(a,b);
        return _FOURIER_FUNCTYPE_EXPEXP;
    }
    if (g.is_symb_of_sommet(at_atan) && is_linear_wrt(g._SYMBptr->feuille,x,a,b,contextptr) &&
            is_zero(im(a,contextptr)) && is_zero(im(b,contextptr))) {
        params=makevecteur(a,b);
        return _FOURIER_FUNCTYPE_ATAN;
    }
    if (g.is_symb_of_sommet(at_Airy_Ai) && is_linear_wrt(g._SYMBptr->feuille,x,a,b,contextptr) &&
            is_zero(im(a,contextptr)) && is_zero(im(b,contextptr))) {
        params=makevecteur(a,b/a);
        return _FOURIER_FUNCTYPE_AIRY_AI;
    }
    if ((hyp=is_expsum(_inv(g,contextptr),x,a,b,c,sq,contextptr))!=0) {
        params=makevecteur(c*gen(sq?4:2),a,b/a);
        if (hyp==1 && !sq) return _FOURIER_FUNCTYPE_COSECH;
        if (hyp==2) return sq?_FOURIER_FUNCTYPE_SECH_2:_FOURIER_FUNCTYPE_SECH;
        return _FOURIER_FUNCTYPE_UNKNOWN;
    }
    if (is_convolution(g,x,a,b,contextptr)) {
        params=makevecteur(a,b);
        return _FOURIER_FUNCTYPE_CONVOLUTION;
    }
    if (is_diff(g,x,a,dg)) {
        params=makevecteur(a,dg);
        return _FOURIER_FUNCTYPE_DIFF;
    }
    if (is_partialdiff(g,fcn,vars,deg,contextptr) &&
            !(tbl=fourier_table_find(fcn,vars.size())).empty() &&
            is_linear_wrt(vars[tbl[2].val],x,a,b,contextptr)){
        params=mergevecteur(mergevecteur(tbl,makevecteur(a,b)),mergevecteur(vars,deg));
        return _FOURIER_FUNCTYPE_PARTIAL_DIFF;
    }
    if (is_func(g,fcn,vars) && !(tbl=fourier_table_find(fcn,vars.size())).empty() &&
            is_linear_wrt(vars[tbl[2].val],x,a,b,contextptr)) {        
        params=mergevecteur(tbl,mergevecteur(makevecteur(a,b),vars));
        return _FOURIER_FUNCTYPE_FUNC;
    }
    if (g.is_symb_of_sommet(at_Fourier) && g._SYMBptr->feuille.type==_VECT && g._SYMBptr->feuille._VECTptr->size()==3 &&
            g._SYMBptr->feuille._VECTptr->back()==x && ((a=g._SYMBptr->feuille._VECTptr->at(1)).type==_IDNT || a.type==_SYMB)) {
        params=makevecteur(g._SYMBptr->feuille._VECTptr->front(),a);
        return _FOURIER_FUNCTYPE_FOURIER;
    }
    if (g.is_symb_of_sommet(at_Hilbert) && g._SYMBptr->feuille.type==_VECT &&
            g._SYMBptr->feuille._VECTptr->size()==2 && g._SYMBptr->feuille._VECTptr->back()==x) {
        params=vecteur(1,g._SYMBptr->feuille._VECTptr->front());
        return _FOURIER_FUNCTYPE_HILBERT;
    }
    // some functions with denominator equal to ax+b
    gen gnorm=ratnormal(g,contextptr);
    if (is_linear_wrt(_denom(gnorm,contextptr),x,c,d,contextptr) && !is_zero(c)) {
        gen h=_numer(gnorm,contextptr);
        if (h.is_symb_of_sommet(at_atan) && is_linear_wrt(h._SYMBptr->feuille,x,a,b,contextptr) &&
                !is_zero(a) && is_same(b*c,a*d,contextptr)) {
            params=makevecteur(a,c,b/a);
            return _FOURIER_FUNCTYPE_ATAN_OVERX;
        }
        if (h.is_symb_of_sommet(at_exp) && is_linabs(h._SYMBptr->feuille,x,a,b,contextptr) &&
                is_same(b*c,d,contextptr) && is_strictly_positive(-a,contextptr)) {
            params=makevecteur(-a,b,c);
            return _FOURIER_FUNCTYPE_EXPABSX_OVERX;
        }
    }
    return _FOURIER_FUNCTYPE_UNKNOWN;
}

gen derive_FT(const gen &g,const identificateur &x,int n,GIAC_CONTEXT) {
    gen d=g;
    for (int i=0;i<n;++i) {
        d=_derive(makesequence(simplify_linarg(sign2Heaviside(d),x,contextptr),x),contextptr);
    }
    return ratnormal(_lin(d,contextptr),contextptr);
}

gen fourier(const gen &f_orig,const identificateur &x,const identificateur &s,
            bool try_diff,bool try_int,bool use_lintx,bool verbose,GIAC_CONTEXT) {
    vecteur terms=fourier_terms(f_orig,x,false,contextptr),p;
    gen ret(0),intgr(0),t,t_ab,a,b,c,n;
    gen rest(0);
    vecteur vars,degr;
    int ind,sz;
    string warn;
    if (verbose) *logptr(contextptr) << "Transforming " << terms.size() << " original term(s)...\n";
    for (int ti=0;ti<int(terms.size());++ti) {
        const gen &term=terms[ti];
        assert(term.type==_VECT);
        const vecteur &v=*term._VECTptr;
        gen cnst=v[0],sh=v[1],d=v[2],f=v[3];
        assert(d.is_integer() && is_positive(d,contextptr));
        if (verbose) *logptr(contextptr) << " * Transforming original term " << exp(cst_i*x*sh,contextptr)*pow(x,d.val)*f
                                         << " with constant factor " << cnst << "\n";
        switch (fourier_func_type(f,x,p,contextptr)) {
        case _FOURIER_FUNCTYPE_ABSX_ALPHA:
            t=p[2]*exp(cst_i*p[1]*s,contextptr);
            if (is_greater(-1,p[0],contextptr)) {
                assert(p[0].is_integer() && is_one(p[3]));
                n=-p[0]-1;
                t=-2*t*_pow(makesequence(-cst_i*s,n),contextptr)*
                    (ln(_abs(s,contextptr),contextptr)+cst_euler_gamma)/_factorial(n,contextptr);
            } else {
                t=-2*t*_Gamma(p[0]+1,contextptr)/
                    _pow(makesequence(_abs(s,contextptr),p[0]+1),contextptr);
                if (is_one(p[3]))
                    t=t*cst_i*_sign(s,contextptr)*cos(cst_pi*p[0]/2,contextptr);
                else t=t*sin(cst_pi*p[0]/2,contextptr);
            }
            break;
        case _FOURIER_FUNCTYPE_LOGABSX:
            t=-exp(cst_i*p[0]*s,contextptr)*cst_pi
                *(_inv(_abs(s,contextptr),contextptr)+2*cst_euler_gamma*_Dirac(s,contextptr));
            break;
        case _FOURIER_FUNCTYPE_INVABSX:
            t=-2*p[0]*exp(cst_i*p[1]*s,contextptr)*(ln(_abs(s,contextptr),contextptr)+cst_euler_gamma);
            break;
        case _FOURIER_FUNCTYPE_EXPABSX:
            t=2*p[0]*exp(cst_i*p[1]*s,contextptr)/(pow(p[0],2)+pow(s,2));
            break;
        case _FOURIER_FUNCTYPE_EXPABSX_OVERX:
            t=-2*cst_i*atan(s/p[0],contextptr)*exp(cst_i*p[1]*s,contextptr)/p[2];
            break;
        default:
            rest+=cnst*exp(cst_i*sh*x,contextptr)*pow(x,d.val)*f;
            continue;
        }
        t=cnst*subst(pow(cst_i,d.val)*derive_FT(t,s,d.val,contextptr),s,s-sh,false,contextptr);
        ret+=exp2pow(_lin(recursive_normal(t,contextptr),contextptr),contextptr);
    }
    bool do_simp=!has_integral(rest) && !has_partial_diff(rest);
    if (do_simp) {
        rest=collect_expHeaviside(linH(abs2Heaviside(sign2Heaviside(
                flatten_piecewise(rest,contextptr)),x,contextptr),x,contextptr),contextptr);
    }
    if (!is_zero(rest)) {
        terms=fourier_terms(exp2pow(rest,contextptr),x,do_simp,contextptr);
        rest=0;
        if (verbose) *logptr(contextptr) << "Transforming " << terms.size() << " term(s)...\n";
        for (int ti=0;ti<int(terms.size());++ti) {
            const gen &term=terms[ti];
            assert(term.type==_VECT);
            const vecteur &v=*term._VECTptr;
            gen cnst=v[0],sh=v[1],d=v[2],f=v[3];
            assert(d.is_integer() && is_positive(d,contextptr));
            if (verbose) *logptr(contextptr) << " * Transforming term " << exp(cst_i*x*sh,contextptr)*pow(x,d.val)*f
                                            << " with constant factor " << cnst << "\n";
            switch (fourier_func_type(f,x,p,contextptr)) {
            case _FOURIER_FUNCTYPE_FUNC:
                if (verbose) *logptr(contextptr) << "   type: function\n";
                a=p[3];
                b=p[4];
                c=p[0];
                vars=vecteur(p.begin()+5,p.end());
                vars[p[2].val]=(is_one(c)?s:-s)/a;
                if (vars.size()==1) vars=makevecteur(p[1],vars.front());
                else vars=makevecteur(p[1],gen(vars,_SEQ__VECT));
                t=c*symbolic(at_of,vars)*exp(cst_i*s*b/a,contextptr)/_abs(a,contextptr);
                break;
            case _FOURIER_FUNCTYPE_DIFF:
                if (verbose) *logptr(contextptr) << "   type: derivative\n";
                t=_pow(makesequence(cst_i*s,p[1]),contextptr)*fourier(p[0],x,s,false,try_int,use_lintx,verbose,contextptr);
                break;
            case _FOURIER_FUNCTYPE_PARTIAL_DIFF:
                if (verbose) *logptr(contextptr) << "   type: partial derivative\n";
                a=p[3];
                b=p[4];
                c=p[0];
                vars=vecteur(p.begin()+5,p.end()-(p.size()-5)/2);
                vars[p[2].val]=(is_one(c)?s:-s)/a;
                vars=makevecteur(p[1],gen(vars,_SEQ__VECT));
                degr=vecteur(p.begin()+5+(p.size()-5)/2,p.end());
                t=c*symbolic(at_of,vars)*exp(cst_i*s*b/a,contextptr)/_abs(a,contextptr);
                t=t*_pow(makesequence(cst_i*s/a,degr[p[2].val]),contextptr);
                for (int i=0;i<int(degr.size());++i) {
                    if (i==p[2].val) continue;
                    t=_derive(makesequence(t,vars[i],degr[i]),contextptr);
                }
                break;
            case _FOURIER_FUNCTYPE_ONE:
                if (verbose) *logptr(contextptr) << "   type: constant\n";
                t=2*p[0]*cst_pi*_Dirac(s,contextptr);
                break;
            case _FOURIER_FUNCTYPE_DIRAC:
                if (verbose) *logptr(contextptr) << "   type: Dirac delta distribution\n";
                t=_pow(makesequence(cst_i*s/p[0],p[2]),contextptr)*exp(cst_i*p[1]*s,contextptr)/_abs(p[0],contextptr);
                break;
            case _FOURIER_FUNCTYPE_DIRAC_F:
                t=subst(p[3],x,-p[1]/p[0],false,contextptr)*exp(cst_i*s*p[1]/p[0],contextptr)/_abs(p[0],contextptr);
                if (has_inf_or_undef(t) && !is_undef(_eval(t,contextptr)))
                    return t;
                try {
                    if (has_inf_or_undef(t))
                        t=_limit(makesequence(_derive(makesequence(subst(p[3],x,(x-p[1])/p[0],false,contextptr)
                            *exp(-cst_i*s*(x-p[1])/p[0],contextptr),x,p[2]),contextptr),x,0),contextptr)/_abs(p[0],contextptr);
                } catch (const gen &err) {
                    return undef;
                }
                break;
            case _FOURIER_FUNCTYPE_HEAVISIDE:
                if (verbose) *logptr(contextptr) << "   type: Heaviside function\n";
                t=exp(cst_i*s*p[1],contextptr)*
                (_sign(p[0],contextptr)/(cst_i*s)+cst_pi*_Dirac(s,contextptr));
                break;
            case _FOURIER_FUNCTYPE_EXP_HEAVISIDE:
                t=exp(p[2]*(cst_i*s-p[0])/p[1],contextptr)*_sign(p[1],contextptr)/(cst_i*s-p[0]);
                break;
            case _FOURIER_FUNCTYPE_BESSELJ:
                t=2*_pow(makesequence(-cst_i,p[2]),contextptr)*exp(cst_i*p[1]*s/p[0],contextptr)*
                _tchebyshev1(makesequence(p[2],s/p[0]),contextptr)*_rect(s/(2*p[0]),contextptr)/
                (_abs(p[0],contextptr)*sqrt(1-pow(s/p[0],2),contextptr));
                break;
            case _FOURIER_FUNCTYPE_GAUSSIAN:
                if (is_strictly_positive(re(p[2],contextptr),contextptr))
                    t=p[0]*exp(cst_i*p[1]*s,contextptr)*sqrt(cst_pi/p[2],contextptr)*exp(-pow(s,2)/(4*p[2]),contextptr);
                else {
                    b=im(-p[2],contextptr);
                    a=_sign(b,contextptr);
                    b=_abs(b,contextptr);
                    assert(!is_zero(b));
                    t=p[0]*sqrt(cst_pi/b,contextptr)*exp(-a*cst_i*(pow(s,2)/(4*b)-cst_pi/4),contextptr);
                }
                break;
            case _FOURIER_FUNCTYPE_GAMMA:
                t=2*cst_pi*exp(cst_i*p[1]*s-s/p[0]-exp(-s/p[0],contextptr),contextptr)/
                _abs(p[0],contextptr);
                break;
            case _FOURIER_FUNCTYPE_EXPEXP:
                t=exp(cst_i*p[1]*s/p[0]-p[1],contextptr)*
                _Gamma(cst_i*s/p[0]+1,contextptr)/_abs(p[0],contextptr);
                break;
            case _FOURIER_FUNCTYPE_AIRY_AI:
                t=exp(cst_i*(p[1]*s+pow(s/p[0],3)/3),contextptr)/_abs(p[0],contextptr);
                break;
            case _FOURIER_FUNCTYPE_ATAN:
                t=-cst_i*cst_pi*_sign(p[0],contextptr)*exp(cst_i*s*p[1]/p[0]-_abs(s/p[0],contextptr),contextptr)/s;
                break;
            case _FOURIER_FUNCTYPE_ATAN_OVERX:
                t=cst_pi*_sign(p[0],contextptr)*exp(cst_i*p[2]*s,contextptr)*
                _upper_incomplete_gamma(makesequence(0,_abs(s/p[0],contextptr)),contextptr)/p[1];
                break;
            case _FOURIER_FUNCTYPE_COSECH:
                t=-cst_i*cst_pi*exp(cst_i*s*p[2],contextptr)*tanh(cst_pi*s/(2*p[1]),contextptr)/
                (_abs(p[1],contextptr)*p[0]);
                break;
            case _FOURIER_FUNCTYPE_SECH:
                t=cst_pi*exp(cst_i*p[2]*s,contextptr)/
                (p[0]*cosh(cst_pi*s/(2*p[1]),contextptr)*_abs(p[1],contextptr));
                break;
            case _FOURIER_FUNCTYPE_SECH_2:
                t=s*cst_pi*exp(cst_i*p[2]*s,contextptr)/
                (p[0]*p[1]*sinh(cst_pi*s/(2*p[1]),contextptr)*_abs(p[1],contextptr));
                break;
            case _FOURIER_FUNCTYPE_CONVOLUTION:
                if (verbose) *logptr(contextptr) << "   type: convolution\n";
                t=fourier(p[0],x,s,try_diff,try_int,use_lintx,verbose,contextptr)
                    *fourier(p[1],x,s,try_diff,try_int,use_lintx,verbose,contextptr);
                break;
            case _FOURIER_FUNCTYPE_FOURIER:
                if (verbose) *logptr(contextptr) << "   type: Fourier transform\n";
                t=2*cst_pi*subst(p[0],p[1],-s,false,contextptr);
                break;
            case _FOURIER_FUNCTYPE_HILBERT:
                if (verbose) *logptr(contextptr) << "   type: Hilbert transform\n";
                t=-cst_i*_sign(s,contextptr)*fourier(p[0],x,s,try_diff,try_int,use_lintx,verbose,contextptr);
                break;
            default:
                rest+=cnst*exp(cst_i*sh*x,contextptr)*pow(x,d.val)*f;
                continue;
            }
            t=cnst*subst(pow(cst_i,d.val)*derive_FT(t,s,d.val,contextptr),s,s-sh,false,contextptr);
            ret+=_exp2pow(_lin(recursive_normal(t,contextptr),contextptr),contextptr);
        }
    }
    if (!is_zero(rest)) {
        terms=fourier_terms(Heaviside2sign(reduce_Heaviside(simplify_linarg(rest,x,contextptr),x,contextptr)),x,do_simp,contextptr);
        for (const_iterateur it=terms.begin();it!=terms.end();++it) {
            assert(it->type==_VECT);
            const vecteur &v=*(it->_VECTptr);
            gen cnst=v[0],sh=v[1],d=v[2],f=v[3];
            assert(d.is_integer() && is_positive(d,contextptr));
            if (verbose) *logptr(contextptr) << " * Transforming REST term " << exp(cst_i*x*sh,contextptr)*pow(x,d.val)*f
                                             << " with constant factor " << cnst << "\n";
            switch (fourier_func_type(f,x,p,contextptr)) {
            case _FOURIER_FUNCTYPE_LOGABSX:
                t=-exp(cst_i*p[0]*s,contextptr)*cst_pi
                    *(_inv(_abs(s,contextptr),contextptr)+2*cst_euler_gamma*_Dirac(s,contextptr));
                break;
            case _FOURIER_FUNCTYPE_INVABSX:
                t=-2*p[0]*exp(cst_i*p[1]*s,contextptr)*(ln(_abs(s,contextptr),contextptr)+cst_euler_gamma);
                break;
            case _FOURIER_FUNCTYPE_EXPABSX:
                t=2*p[0]*exp(cst_i*p[1]*s,contextptr)/(pow(p[0],2)+pow(s,2));
                break;
            case _FOURIER_FUNCTYPE_EXPABSX_OVERX:
                t=-2*cst_i*atan(s/p[0],contextptr)*exp(cst_i*p[1]*s,contextptr)/p[2];
                break;
            case _FOURIER_FUNCTYPE_SGN:
                t=-2*cst_i*exp(cst_i*p[1]*s/p[0],contextptr)*_sign(p[0],contextptr)/s;
                break;
            case _FOURIER_FUNCTYPE_INV_MONOM:
                n=p[2]-1;
                if (is_zero(im(p[1],contextptr))) {
                    t=_pow(makesequence(-cst_i*s,n),contextptr)*
                      exp(cst_i*p[1]*s,contextptr)*_sign(s,contextptr);
                } else {
                    gen sgn=_sign(im(p[1],contextptr),contextptr);
                    t=2*sgn*_pow(makesequence(-cst_i*s,n),contextptr)*
                      exp(cst_i*p[1]*s,contextptr)*_Heaviside(sgn*s,contextptr);
                }
                t=-t*p[0]*cst_pi*cst_i/_factorial(n,contextptr);
                break;
            default:
                if (verbose) *logptr(contextptr) << "   Failed to find the expression in the table\n";
                if (try_diff) {
                    // check whether we can transform the derivative
                    gen df=derive_FT(f,x,1,contextptr);
                    gen F=fourier(df,x,s,false,try_int,false,verbose,contextptr),chk,fchk;
                    if (!is_undef(F) && !is_inf(F) && !contains(F,at_Fourier) && !has_integral(F)) {
                        if (verbose) *logptr(contextptr) << "   Transformed the derivative\n";
                        t=-cst_i*F/s;
                        chk=f-subst(fourier(t,s,x,false,try_int,false,verbose,contextptr),x,-x,false,contextptr)/cst_two_pi;
                        if (!is_undef(chk) && !is_inf(chk) && !contains(chk,at_Fourier)) {
                            chk=simplify(chk,contextptr);
                            if (verbose) *logptr(contextptr) << "chk: " << chk << "\n";
                            fchk=fourier(chk,x,s,false,try_int,false,verbose,contextptr);
                            if (!is_undef(fchk) && !is_inf(fchk) && !contains(fchk,at_Fourier)) {
                                t+=fchk;
                                break;
                            }
                        }
                    }
                }
                gen fr;
                string intwarn;
                if (try_int) {
                    if (verbose) *logptr(contextptr) << "   Trying symbolic integration\n";
                    log_output_redirect lor(contextptr);
                    fr=_integrate(makesequence(exp(-cst_i*x*(s-sh),contextptr)*pow(x,d.val)*f,x,minus_inf,plus_inf),contextptr);
                    intwarn=lor.get_buffer_string();
                }
                if (!try_int || is_undef(fr) || is_inf(fr) || has_integral(fr)) {
                    if (use_lintx) {
                        vecteur lt;
                        lintx(_lin(f,contextptr),x,lt,contextptr);
                        if (!lt.empty()) {
                            const_iterateur it=lt.begin(),itend=lt.end();
                            for (;it!=itend;++it) {
                                const vecteur &ab=*it->_VECTptr;
                                if (is_one(ab.front()) && is_zero(ab.back()))
                                    continue;
                                gen F=fourier(recursive_normal(subst(f,x,(x-ab.back())/ab.front(),false,contextptr),contextptr),
                                    x,s,try_diff,try_int,false,verbose,contextptr);
                                if (!is_undef(F) && !is_inf(F) && !contains(F,at_Fourier) && !has_integral(F)) {
                                    t=subst(F,s,s/ab.front(),false,contextptr)*exp(cst_i*s*ab.back()/ab.front(),contextptr)
                                        /_abs(ab.front(),contextptr);
                                    break;
                                }
                            }
                            if (it!=itend)
                                break;
                        }
                    }
                    fr=symbolic(at_Fourier,makesequence(pow(x,d.val)*exp(cst_i*x*sh,contextptr)*f,x,s));
                } else if (try_int && verbose)
                    warn+=intwarn;
                intgr+=cnst*fr;
                continue;
            }
            t=cnst*subst(pow(cst_i,d.val)*derive_FT(t,s,d.val,contextptr),s,s-sh,false,contextptr);
            ret+=exp2pow(_lin(recursive_normal(t,contextptr),contextptr),contextptr);
        }
    }
    if (!warn.empty())
        *logptr(contextptr) << warn;
    log_output_redirect lor(contextptr);
    if (contains(ret,at_Dirac))
        ret=simplify_Dirac(ret,s,NULL,contextptr);
    ret=simplify_linarg(ret,s,contextptr);
    gen rcn=reduce_Heaviside(ret,s,contextptr);
    if (is_simpler(rcn,ret)) ret=rcn;
    ret=exp2pow(ret,contextptr);
    if (is_zero(ratnormal(im(ret,contextptr),contextptr)))
        ret=re(ret,contextptr);
    rcn=recursive_normal(ret,contextptr);
    if (is_rational_wrt(rcn,s)) {
        ret=simplify(rcn,contextptr);
    } else if (is_simpler(rcn,ret)) ret=rcn;
    rcn=ratnormal(_exp2trig(ret,contextptr),contextptr);
    if (is_simpler(rcn,ret)) ret=rcn;
    if (contains(ret,at_Heaviside)) {
        rcn=ratnormal(_exp2trig(Heaviside2sign(ret),contextptr),contextptr);
        if (is_simpler(rcn,ret))
            ret=collect_with_func(rcn,at_sign,NULL,NULL,contextptr);
        else ret=collect_with_func(ratnormal(sign2Heaviside(ret),contextptr),at_Heaviside,NULL,NULL,contextptr);
    }
    rcn=factorise(ret,contextptr);
    if (is_simpler(rcn,ret)) ret=rcn;
    intgr=ratnormal(intgr,contextptr);
    return ret+intgr;
}

bool parse_fourier_args(const vecteur &args,gen &f_orig,identificateur &var,gen &tvar,bool &try_int,bool &try_diff,GIAC_CONTEXT) {
    f_orig=args.front();
    int optstart=1;
    if (args.size()>=2 && args[1].type==_IDNT)
        var=*args[optstart++]._IDNTptr;
    if (optstart==2 && args.size()>=3 && (args[2].type==_IDNT || (args[2].type==_SYMB && !args[2].is_symb_of_sommet(at_equal))))
        tvar=args[optstart++];
    // parse options
    for (const_iterateur it=args.begin()+optstart;it!=args.end();++it) {
        if (it->is_symb_of_sommet(at_equal)) {
            const gen &lh=it->_SYMBptr->feuille._VECTptr->front();
            const gen &rh=it->_SYMBptr->feuille._VECTptr->back();
            if (!rh.is_integer() || rh.subtype!=_INT_BOOLEAN)
                return false;
            if (lh==at_integrate || lh==at_int || lh==at_Int)
                try_int=(bool)rh.val;
            else if (lh==at_derive || lh==at_deriver)
                try_diff=(bool)rh.val;
            else return false;
        } else return false;
    }
    if (tvar.type==_IDNT && *tvar._IDNTptr==var) {
        *logptr(contextptr) << gettext("Original and transform variables must be different") << "\n";
        return false;
    }
    return true;
}

gen _fourier(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    gen var(undef),f_orig;
    identificateur x("t"),s(" fourier_s"),om("omega");
    bool try_int=false,try_diff=false,verbose=false;
    if (g.type==_VECT) {
        if (g.subtype!=_SEQ__VECT)
            return gentypeerr(contextptr);
        if (!parse_fourier_args(*g._VECTptr,f_orig,x,var,try_int,try_diff,contextptr))
            return gensizeerr(contextptr);
    } else f_orig=g;
    if (_eval(x,contextptr).type!=_IDNT || _eval(om,contextptr).type!=_IDNT)
        return generr(gettext("Default variable not available"));
    gen ret=fourier(f_orig,x,s,try_diff,try_int,true,verbose,contextptr);
    if (has_inf_or_undef(ret))
        return generr(gettext("Fourier transform does not exist"));
    if (!is_undef(var))
        return subst(ret,s,var,false,contextptr);
    return subst(ret,s,om,false,contextptr);
}
static const char _fourier_s []="fourier";
static define_unary_function_eval (__fourier,&_fourier,_fourier_s);
define_unary_function_ptr5(at_fourier,alias_at_fourier,&__fourier,0,true)

gen _ifourier(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    gen var(undef),f_orig;
    identificateur s("omega"),x(" fourier_x"),t("t");
    bool try_int=false,try_diff=false,verbose=false;
    if (g.type==_VECT) {
        if (g.subtype!=_SEQ__VECT)
            return gentypeerr(contextptr);
        if (!parse_fourier_args(*g._VECTptr,f_orig,s,var,try_int,try_diff,contextptr))
            return gensizeerr(contextptr);
    } else f_orig=g;
    if (_eval(s,contextptr).type!=_IDNT || _eval(t,contextptr).type!=_IDNT)
        return generr(gettext("Default variable not available"));
    gen ret=fourier(f_orig,s,x,try_diff,try_int,true,verbose,contextptr);
    if (has_inf_or_undef(ret))
        return generr(gettext("Inverse Fourier transform does not exist"));
    if (!is_undef(var))
        return ratnormal(subst(ret,x,-var,false,contextptr)/cst_two_pi,contextptr);
    return ratnormal(subst(ret,x,-t,false,contextptr)/cst_two_pi,contextptr);
}
static const char _ifourier_s []="ifourier";
static define_unary_function_eval (__ifourier,&_ifourier,_ifourier_s);
define_unary_function_ptr5(at_ifourier,alias_at_ifourier,&__ifourier,0,true)

// inert form of fourier(f,x,a*s+b) with auto simplification of a*s+b to s
gen _Fourier(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type==_VECT && g.subtype==_SEQ__VECT && g._VECTptr->size()==3) {
        const gen &f=g._VECTptr->at(0),&x=g._VECTptr->at(1),&s=g._VECTptr->at(2);
        vecteur svar=*_lname(s,contextptr)._VECTptr;
        gen a,b,o;
        if (x.type==_IDNT && svar.size()==1 && is_linear_wrt(s,o=svar.front(),a,b,contextptr) && !is_zero(a))
            return symbolic(at_Fourier,makesequence(exp(-cst_i*x*b/a,contextptr)*subst(f,x,x/a,false,contextptr),x,o));
    }
    return symbolic(at_Fourier,g);
}
static const char _Fourier_s []="Fourier";
static define_unary_function_eval (__Fourier,&_Fourier,_Fourier_s);
define_unary_function_ptr5(at_Fourier,alias_at_Fourier,&__Fourier,0,true)

/* Find the continuous Hilbert transform. */
gen hilbtrans(const gen &args,GIAC_CONTEXT) {
    gen x=identificateur("x"),o=identificateur(" hilbert_omega"),f;
    if (args.type==_VECT) {
        if (args.subtype!=_SEQ__VECT)
            return gentypeerr(contextptr);
        if (args._VECTptr->size()!=2)
            return gendimerr(contextptr);
        f=args._VECTptr->front();
        x=args._VECTptr->back();
    } else f=args;
    if (x.type!=_IDNT || f.type==_VECT)
        return gentypeerr(contextptr);
    if (_eval(x,contextptr).type!=_IDNT)
        return generr(gettext("Default variable not available"));
    if (is_const_wrt(f,x,contextptr))
        return 0;
    if (f.type!=_IDNT && f.type!=_SYMB)
        return gentypeerr(contextptr);
    _purge(o,contextptr);
    try {
        gen F=_fourier(makesequence(f,x,o),contextptr);
        if (has_inf_or_undef(F))
            return undef;
        gen G=_ifourier(makesequence(-cst_i*_sign(o,contextptr)*F,o,x),contextptr);
        if (has_inf_or_undef(G))
            return undef;
        if (contains(G,at_Fourier))
            return _Hilbert(makesequence(f,x),contextptr);
        return G;
    } catch (const gen &err) {
        *logptr(contextptr) << err << "\n";
    }
    return undef;
}

gen _Hilbert(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()!=2 || g._VECTptr->back().type!=_IDNT)
        return gentypeerr(contextptr);
    return symbolic(at_Hilbert,g);
}
static const char _Hilbert_s []="Hilbert";
static define_unary_function_eval (__Hilbert,&_Hilbert,_Hilbert_s);
define_unary_function_ptr5(at_Hilbert,alias_at_Hilbert,&__Hilbert,0,true)

/* Return analytic signal from a real data sequence X. */
gen dht(const vecteur &x,GIAC_CONTEXT) {
    if (!is_zero__VECT(*im(x,contextptr)._VECTptr,contextptr))
        *logptr(contextptr) << gettext("Warning") << ": " << gettext("hilbert ignores the imaginary part of input") << "\n";
    vecteur y=*re(x,contextptr)._VECTptr,u,v;
    int N=y.size();
    vector<pair<double,double> > sct;
    try {
        u=*_fft(y,contextptr)._VECTptr;
    } catch (const std::runtime_error &err) {
        *logptr(contextptr) << err.what() << "\n";
        return gensizeerr(contextptr);
    }
    const_iterateur it=u.begin();
    v.reserve(N);
    for (int k=0;k<=N/2;++it,++k) v.push_back(((k==0||k==(N+1)/2)?1:2)*(*it));
    v.resize(N,0);
    return _ifft(v,contextptr);
} 

/* Replace every exp(-ax) for a>0 with identifier xa and return pairs (a,xa) in m. */
gen replace_exp_with_idnt(const gen &g,const gen &x,gen_map &m,GIAC_CONTEXT) {
    if (g.type==_VECT) {
        vecteur res;
        res.reserve(g._VECTptr->size());
        const_iterateur it=g._VECTptr->begin(),itend=g._VECTptr->end();
        for (;it!=itend;++it) res.push_back(replace_exp_with_idnt(*it,x,m,contextptr));
        return gen(res,g.subtype);
    }
    gen a,b;
    int s;
    if (g.is_symb_of_sommet(at_exp) && is_linear_wrt(g._SYMBptr->feuille,x,a,b,contextptr) &&
            !is_zero(a) && is_exactly_zero(b) && (s=compexpr(0,a,contextptr))!=0) {
        gen_map::const_iterator it=m.find(s*a);
        gen v;
        if (it==m.end()) {
            v=identificateur(" var"+print_INT_(m.size()));
            //_purge(v,contextptr);
            m.insert(make_pair(s*a,v));
        } else v=it->second;
        assert(v.type==_IDNT);
        return s>0?_inv(v,contextptr):v;
    }
    if (g.type==_SYMB)
        return symbolic(g._SYMBptr->sommet,replace_exp_with_idnt(g._SYMBptr->feuille,x,m,contextptr));
    return g;
}

/* Return true if there exists integer n and polynomial q such that
 * p*q=(1-x^n) holds and n is minimal, or false if such n does not exist. */
bool pq_1minusxn(const gen &p,const gen &x,int &n,gen &q,GIAC_CONTEXT) {
    gen res=_roots(makesequence(p,x),contextptr);
    if (!ckmatrix(res) || mcols(*res._VECTptr)!=2)
        return false;
    const matrice &r=*res._VECTptr;
    int nr=mrows(r);
    vecteur ncand;
    ncand.reserve(nr);
    for (int k=0;k<nr;++k) {
        if (!is_one(r[k][1]) || !is_one(simplify(_abs(r[k][0],contextptr),contextptr)))
            return false;
        int mode=get_mode_set_radian(contextptr);
        gen phi=ratnormal(arg(r[k][0],contextptr)/cst_pi,contextptr),a,b;
        angle_mode(mode,contextptr);
        if (phi.is_integer()) {
            a=phi.val;
            b=1;
        } else if (phi.type!=_FRAC || !(a=phi._FRACptr->num).is_integer() || !(b=phi._FRACptr->den).is_integer())
            return false;
        ncand.push_back(is_one(_even(a,contextptr))?b:2*b);
    }
    gen N=_lcm(ncand,contextptr);
    if (!N.is_integer())
        return false;
    n=N.val;
    gen pol=1-pow(x,n),qr=_quorem(makesequence(pol,p,x),contextptr);
    if (qr.type!=_VECT || qr._VECTptr->size()!=2 || !is_exactly_zero(qr._VECTptr->back()))
        return false;
    q=qr._VECTptr->front();
    return true;
}

/* Split g into terms f1(x)/(1-exp(-a1*x)) + f2(x)/(1-exp(-a2*x)) + ... + fn(x)/(1-exp(-an*x)) + fn+1(x),
 * where fk(x) = sum(exp(bkj*x)*Rkj(x),j=1..nk), where Rkj are rational for k=1,...,n+1. */
void expand_lt(const gen &g_orig,const gen &x,vecteur &f,vecteur &a,GIAC_CONTEXT) {
    gen_map m;
    gen g=replace_exp_with_idnt(recursive_normal(exp2pow(simplify(expexpand(g_orig,contextptr),
        contextptr),contextptr),contextptr),x,m,contextptr);
    gen_map::const_iterator it=m.begin(),itend=m.end();
    vecteur fac;
    f.reserve(m.size()+1);
    a.reserve(m.size());
    for (;it!=itend;++it) {
        const gen &y=it->second,&b=it->first;
        if (y.type!=_IDNT || !is_rational_wrt(g,*y._IDNTptr))
            continue;
        if (fac.empty()) {
            gen facg=_factors(_denom(g,contextptr),contextptr);
            if (facg.type!=_VECT)
                continue;
            fac=*facg._VECTptr;
        }
        gen d=1,p=1,q;
        for (const_iterateur jt=fac.begin();jt!=fac.end();++jt) {
            if (is_const_wrt(*jt,y,contextptr) || *jt==y)
                d=d*_pow(makesequence(*jt,*(jt+1)),contextptr);
            else p=p*_pow(makesequence(*jt,*(jt+1)),contextptr);
            ++jt;
        }
        int n;
        if (is_one(p) || !pq_1minusxn(p,y,n,q,contextptr))
            continue;
        gen qr=_quorem(makesequence(_numer(g,contextptr)*q,1-pow(y,n),y),contextptr);
        if (qr.type!=_VECT || qr._VECTptr->size()!=2)
            continue;
        f.push_back(ratnormal(qr._VECTptr->back()/d,contextptr));
        a.push_back(n*b);
        g=ratnormal(qr._VECTptr->front()/d,contextptr);
        fac.clear();
    }
    f.push_back(g);
    for (it=m.begin();it!=itend;++it)
        f=subst(f,it->second,exp(-x*it->first,contextptr),false,contextptr);
}

bool laplace_periodic(const gen &g_orig,const gen &x,const gen &s,gen &t,GIAC_CONTEXT) {
    gen g=simplify_floor(recursive_normal(g_orig,contextptr),contextptr),T(undef);
    if (!is_periodic_wrt(g,x,T,contextptr) || is_undef(T) || !is_strictly_positive(T,contextptr)) {
        if (contains(g_orig,at_floor))
            t=undef;
        return false;
    }
    if (!contains(g_orig,at_floor))
        return false;
    t=_int(makesequence(g*exp(-s*x,contextptr),x,0,T),contextptr)/(1-exp(-s*T,contextptr));
    return true;
}

bool ilaplace2(const gen &g,const gen &s,const gen &x,gen &orig,GIAC_CONTEXT) {
    if (s.type!=_IDNT || x.type!=_IDNT)
        return false;
    vecteur f,a;
    gen F,h,n=identificateur(" n"),k=identificateur(" k");
    //_purge(makesequence(n,k),contextptr);
    sto(gen(makevecteur(change_subtype(2,1)),_ASSUME__VECT),n,contextptr);
    orig=0;
    expand_lt(g,s,f,a,contextptr);
    if (a.empty())
        return false;
    int na=a.size(),i=0;
    for (;i<=na;++i) {
        F=_ilaplace(makesequence(f[i],s,x),contextptr);
        if (i==na) {
            orig+=F;
            break;
        }
        if (contains(F,at_ilaplace))
            return false;
        const gen &T=a[i];
        h=ratnormal(eval_for_x_in_ab(F,*x._IDNTptr,minus_inf,T,contextptr),contextptr);
        orig+=_periodic(makesequence(h,*x._IDNTptr,0,T),contextptr);
        h=ratnormal(eval_for_x_in_ab(F,*x._IDNTptr,T,plus_inf,contextptr),contextptr);
        if (!is_zero(h)) {
            h=_sum(makesequence(subst(h,x,x-T*(k-1),false,contextptr),k,1,n),contextptr);
            orig+=subst(h,n,_floor(x/T,contextptr),false,contextptr)*_Heaviside(x-T,contextptr);
        }
    }
    return true;
}

#ifdef HAVE_LIBGSL
/* Empirical Mode Decomposition (to max IMAX imf components + residual).
 * Adapted from Matlab code written by Ivan Magrin-Chagnolleau <ivan@ieee.org>.
 * IMAX is the maximum number of IMFs to be generated.
 * EMIN is the mminimal number of local extrema that an IMF must have.
 * VTOL is the variance threshold for stopping the sifting process (usually 0.1). */
void emd(const vecteur &data,matrice &imf,vecteur &residue,int imax,int emin,double vtol,GIAC_CONTEXT) {
    int n=data.size(),i;
    gsl_vector *x,*h,*envu,*envl;
    double *maxes,*mins,*maxesy,*minsy,var;
    maxes=new double[n+2];
    maxesy=new double[n+2];
    mins=new double[n+2];
    minsy=new double[n+2];
    maxes[0]=mins[0]=0;
    h=gsl_vector_alloc(n);
    envu=gsl_vector_alloc(n);
    envl=gsl_vector_alloc(n);
    x=vecteur2gsl_vector(data,contextptr);
    gsl_interp_accel *acc;
    gsl_spline *sp;
    while (true) {
        var=1;
        gsl_vector_memcpy(h,x);
        int iterc=0,ne;
        bool isres=false,notimf=true;
        while (var>vtol || notimf) {
            // find indices of extremal samples
            int maxc=1,minc=1,zc=0;
            maxesy[0]=minsy[0]=gsl_vector_get(h,0);
            for (i=1;i<n-2;++i) {
                double h1=gsl_vector_get(h,i-1),h2=gsl_vector_get(h,i),
                       h3=gsl_vector_get(h,i+1),h4=gsl_vector_get(h,i+2);
                if (h1==0 || h1*h2<0)
                    ++zc;
                double d=h3-h2,dl=h2-h1,dr=h4-h3;
                if ((d==0 && dl<0 && dr>0) || (dl<0 && d>0)) {
                    mins[minc]=i; minsy[minc++]=gsl_vector_get(h,i);
                } else if ((d==0 && dl>0 && dr<0) || (dl>0 && d<0)) {
                    maxes[maxc]=i; maxesy[maxc++]=gsl_vector_get(h,i);
                }
            }
            mins[minc]=maxes[maxc]=n-1;
            minsy[minc++]=maxesy[maxc++]=gsl_vector_get(h,n-1);
            ne=maxc+minc-4;
            if (ne<emin) { // residue
                isres=true;
                break;
            }
            notimf=std::abs(zc-ne)>1;
            // compute the upper envelope
            acc=gsl_interp_accel_alloc();
            sp=gsl_spline_alloc(maxc>2?gsl_interp_cspline:gsl_interp_linear,maxc);
            gsl_spline_init(sp,maxes,maxesy,maxc);
            for (i=0;i<n;++i) gsl_vector_set(envu,i,gsl_spline_eval(sp,i,acc));
            gsl_spline_free(sp);
            gsl_interp_accel_free(acc);
            // compute the lower envelope
            acc=gsl_interp_accel_alloc();
            sp=gsl_spline_alloc(minc>2?gsl_interp_cspline:gsl_interp_linear,minc);
            gsl_spline_init(sp,mins,minsy,minc);
            for (i=0;i<n;++i) gsl_vector_set(envl,i,gsl_spline_eval(sp,i,acc));
            gsl_spline_free(sp);
            gsl_interp_accel_free(acc);
            gsl_vector_scale(envu,0.5);
            gsl_vector_scale(envl,0.5);
            gsl_vector_add(envl,envu); // compute the mean of envelopes, store it in envl
            var=0;
            for (i=0;i<n;++i) {
                var+=std::pow(gsl_vector_get(envl,i),2)/(std::pow(gsl_vector_get(h,i),2)+1e-7);
            }
            var/=n;
            gsl_vector_sub(h,envl); // subtract mean from h
        }
        if (isres) {
            residue=gsl_vector2vecteur(h);
            break;
        }
        imf.push_back(gsl_vector2vecteur(h));
        gsl_vector_sub(x,h);
        if (imax==(int)imf.size()) {
            residue=gsl_vector2vecteur(x);
            break;
        }
    }
    delete[] maxes;
    delete[] mins;
    delete[] maxesy;
    delete[] minsy;
    gsl_vector_free(x);
    gsl_vector_free(h);
    gsl_vector_free(envu);
    gsl_vector_free(envl);
}
#endif

gen _emd(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
#ifdef HAVE_LIBGSL
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->empty())
        return gendimerr(contextptr);
    double var_tol=0.1;
    int imf_limit=RAND_MAX,min_extrema=2;
    bool out_residue=true;
    if (g.subtype==_SEQ__VECT) { // has options
        const vecteur &args=*g._VECTptr;
        if (args.front().type!=_VECT)
            return gentypeerr(contextptr);
        const_iterateur it=args.begin()+1,itend=args.end();
        for (;it!=itend;++it) {
            if (it->is_symb_of_sommet(at_equal)) {
                const gen &p=it->_SYMBptr->feuille._VECTptr->front();
                const gen &v=it->_SYMBptr->feuille._VECTptr->back();
                if (p==at_threshold) {
                    gen dv=v.evalf_double(1,contextptr);
                    if (dv.type!=_DOUBLE_ || is_positive(-dv,contextptr))
                        return gensizeerr(contextptr);
                    var_tol=dv.DOUBLE_val();
                } else if (p==at_limit) {
                    if (!v.is_integer())
                        return gentypeerr(contextptr);
                    imf_limit=v.val;
                    if (imf_limit<1)
                        return gensizeerr(contextptr);
                } else if (p==at_residue) {
                    if (!v.is_integer() || v.subtype!=_INT_BOOLEAN)
                        return gentypeerr(contextptr);
                    out_residue=(bool)v.val;
                } else if (p==at_extrema) {
                    if (!v.is_integer())
                        return gentypeerr(contextptr);
                    min_extrema=v.val;
                    if (min_extrema<2)
                        return gensizeerr(contextptr);
                } else return generrarg(it-args.begin());
            } else return generrarg(it-args.begin());
        }
    }
    const vecteur &data=g.subtype==_SEQ__VECT?*g._VECTptr->front()._VECTptr:*g._VECTptr;
    if (!is_real_vector(data,contextptr))
        return gensizeerr("Data must be numeric");
    matrice imf;
    vecteur residue;
    try {
        emd(data,imf,residue,imf_limit,min_extrema,var_tol,contextptr);
    } catch (const gen &err) {
        return err;
    } catch (const std::runtime_error &re) {
        return gensizeerr(re.what());
    }
    if (out_residue)
        return makesequence(imf,residue);
    return imf;
#else
    return generr(gettext("GSL is required for empirical mode decomposition"));
#endif
}
static const char _emd_s []="emd";
static define_unary_function_eval (__emd,&_emd,_emd_s);
define_unary_function_ptr5(at_emd,alias_at_emd,&__emd,0,true)

void draw_imf_plot(const vecteur &imf,const string &lab,int &m,int mode,vecteur &drawing,const gen &disp,GIAC_CONTEXT) {
    gen a=evalf_double(_max(_abs(imf,contextptr),contextptr),1,contextptr),y=1.15*m;
    int len=imf.size();
    drawing.push_back(_legende(makesequence(gen(0,mode!=0?y-0.95:y),string2gen(lab,false),
        change_subtype(mode!=0?_QUADRANT2:_QUADRANT3,_INT_COLOR)),contextptr));
    if (mode==1 && a.type==_DOUBLE_)
        drawing.push_back(_legende(makesequence(gen(0,y),string2gen(print_DOUBLE_(1.11*a.DOUBLE_val(),3),false),
            symb_equal(at_couleur,change_subtype(_QUADRANT3,_INT_COLOR))),contextptr));
    drawing.push_back(_listplot(makesequence(addvecteur(divvecteur(imf,a/0.45),vecteur(len,y-0.5)),disp),contextptr));
    drawing.push_back(_rectangle(makesequence(gen(0,y-1),gen(len,y-1),1.0/len),contextptr));
    --m;
}

gen _imfplot(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    bool hasopt=g.subtype==_SEQ__VECT,show_range=true;
    if (hasopt && g._VECTptr->size()<2)
        return gendimerr(contextptr);
    if (hasopt && g._VECTptr->front().type!=_VECT)
        return gentypeerr(contextptr);
    const vecteur &imfs=hasopt?*g._VECTptr->front()._VECTptr:*g._VECTptr;
    int mask=num_mask_withfrac | num_mask_withint;
    if (!ckmatrix(imfs) || !is_numericm(imfs,mask))
        return generr(gettext("Expected a matrix of real numbers"));
    int n=imfs.size(),len=mcols(imfs),m=n;
    const vecteur *res=NULL,*orig=NULL;
    gen ev,rev,disp=symb_equal(at_couleur,change_subtype(_BLUE,_INT_COLOR)); // default IMF color
    set<int> ind;
    if (hasopt) {
        if (g._VECTptr->size()==2 && g._VECTptr->back().is_integer()) {
            m=g._VECTptr->back().val;
            if (m<1)
                return generr(gettext("Expected a positive integer"));
        } else for (const_iterateur it=g._VECTptr->begin()+1;it!=g._VECTptr->end();++it) {
            if (!it->is_symb_of_sommet(at_equal))
                return gensizeerr("Invalid optional argument");
            const gen &p=it->_SYMBptr->feuille._VECTptr->front();
            const gen &v=it->_SYMBptr->feuille._VECTptr->back();
            if (p==at_residue) {
                rev=_evalf(v,contextptr);
                if (rev.type!=_VECT || (int)rev._VECTptr->size()!=len || !is_numericv(*(res=rev._VECTptr),mask))
                    return generr(gettext("Invalid residue specification"));
            } else if (p==at_max) {
                if (!v.is_integer() || v.subtype!=_INT_BOOLEAN)
                    return gensizeerr("Expected a boolean value");
                show_range=(bool)v.val;
            } else if (p==at_count) {
                if (!v.is_integer() || (m=v.val)<1)
                    return generr(gettext("Expected a positive integer"));
            } else if (p==at_input) {
                ev=_evalf(v,contextptr);
                if (ev.type!=_VECT || (int)ev._VECTptr->size()!=len || !is_numericv(*(orig=ev._VECTptr),mask))
                    return generr(gettext("Invalid original signal specification"));
            } else if (p==at_display || p==at_couleur) {
                disp=*it;
            } else if (p==at_index) {
                vecteur iv(gen2vecteur(v));
                for (const_iterateur jt=iv.begin();jt!=iv.end();++jt) {
                    if (jt->is_integer()) {
                        if (jt->val<0 || jt->val>=n)
                            return generrdim(gettext("IMF index out of range"));
                        ind.insert(jt->val);
                    } else if (jt->is_symb_of_sommet(at_interval)) {
                        const gen &l=jt->_SYMBptr->feuille._VECTptr->front();
                        const gen &u=jt->_SYMBptr->feuille._VECTptr->back();
                        if (!l.is_integer() || !u.is_integer())
                            return generr(gettext("Expected a range of integers"));
                        int il=l.val,iu=u.val;
                        if (il<0 || il>=n || iu<il || iu>=n)
                            return generrdim(gettext("Invalid IMF index range"));
                        for (int i=il;i<=iu;++i)
                            ind.insert(i);
                    } else return generr(gettext("Invalid IMF index specification"));
                }
            } else return generr(gettext("Unknown option"));
        }
    }
    if (ind.empty()) for (int i=0;i<m;++i) ind.insert(i);
    m=ind.size();
    vecteur drawing=vecteur(1,symb_equal(change_subtype(_AXES,_INT_PLOT),0));
    string str="IMF ",sstr=gettext("Signal"),rstr=gettext("Residue");
    if (orig!=NULL)
        draw_imf_plot(*orig,sstr,m,0,drawing,disp,contextptr);
    for (set<int>::const_iterator it=ind.begin();it!=ind.end();++it) {
        const vecteur &imf=*imfs[*it]._VECTptr;
        string lab=str+print_INT_(*it);
        draw_imf_plot(imf,lab,m,show_range?1:0,drawing,disp,contextptr);
    }
    if (res!=NULL)
        draw_imf_plot(*res,rstr,m,show_range?-1:0,drawing,disp,contextptr);
    return drawing;
}
static const char _imfplot_s []="imfplot";
static define_unary_function_eval (__imfplot,&_imfplot,_imfplot_s);
define_unary_function_ptr5(at_imfplot,alias_at_imfplot,&__imfplot,0,true)
static const char _plotimf_s []="plotimf";
static define_unary_function_eval (__plotimf,&_imfplot,_plotimf_s);
define_unary_function_ptr5(at_plotimf,alias_at_plotimf,&__plotimf,0,true)

/* Fill RES with instantaneous frequency values for analytic signal AS.
 * If SR>0 (sample rate), then the values are expressed in Hertz,
 * otherwise in radians per sample. */
void inst_freq(const vecteur &as,vecteur &res,int sr,GIAC_CONTEXT) {
    res.reserve(as.size());
    res.push_back(0);
    const_iterateur it=as.begin()+1,itend=as.end();
    int mode = get_mode_set_radian(contextptr);
    gen a=sr<=0?1.0:M_1_PI*0.5*sr;
    for (;it!=itend;++it) {
        res.push_back(max(0,a*arg(conj(*(it-1),contextptr)*(*it),contextptr),contextptr));
    }
    angle_mode(mode,contextptr);
}

/* Fill RES with instantaneous amplitude values for analytic signal AS. */
void inst_amp(const vecteur &as,vecteur &res,GIAC_CONTEXT) {
    res.reserve(as.size());
    const_iterateur it=as.begin(),itend=as.end();
    for (;it!=itend;++it) {
        res.push_back(_abs(*it,contextptr));
    }
}

/* Fill the table HS with Hilbert spectrum data for signal with
 *   EMD decomposition IMF (without the residual part).
 * If SR>0 (sample rate), then the frequency data will be in Hertz,
 +   otherwise in radians per sample.
 * FMIN and FMAX, if defined, specify the frequency band of interest.
 *   Other frequencies will be discarded. If undef, FMIN is the
 *   smallest frequency and FMAX is the largest frequency value
 *   occurring in IMF.
 * FBINS specifies the frequency resolution, i.e. the size of
 *   the subdivision of interval [FMIN,FMAX].
 * LOGF=true means that log frequency axis is used with integers
 *   corresponding to MIDI note names (middle C = 60). */
void hht(const matrice &imf,map<pair<int,int>,double> &hs,int sr,gen &fmin,gen &fmax,int fbins,bool logf,GIAC_CONTEXT) {
    if (imf.empty() || fbins<1)
        return;
    matrice f,a;
    f.reserve(imf.size());
    a.reserve(imf.size());
    const_iterateur it=imf.begin(),itend=imf.end();
    int rc=0,n=mcols(imf),i,j,h;
    for (;it!=itend;++it) {
        if (it->type!=_VECT)
            continue;
        vecteur fi,ai,as=*_hilbert(*it->_VECTptr,contextptr)._VECTptr;
        inst_freq(as,fi,sr,contextptr);
        f.push_back(fi);
        inst_amp(as,ai,contextptr);
        a.push_back(ai);
        ++rc;
    }
    if (rc==0)
        return;
    if (is_undef(fmin))
        fmin=_min(_min(f,contextptr),contextptr);
    if (is_undef(fmax))
        fmax=_max(_max(f,contextptr),contextptr);
    if (logf)
        fmin=_evalf(max(MIDI_NOTE_0,fmin,contextptr),contextptr);
    gen r=logf?ln(fmax/fmin,contextptr):fmax-fmin;
    if (!is_strictly_positive(r,contextptr))
        return;
    for (j=0;j<n;++j) {
        for (i=0;i<rc;++i) {
            h=_floor((fbins-1)*(logf?ln(f[i][j]/fmin,contextptr):f[i][j]-fmin)/r,contextptr).val;
            if (h>=0 && h<fbins)
                hs[make_pair(h,j)]+=evalf_double(a[i][j],1,contextptr)._DOUBLE_val;
        }
    }
}

void arrayplot(const matrice &c,const gen &x1,const gen &x2,const gen &y1,const gen &y2,int pal,
        bool with_colormap,bool with_image,bool with_transparency,vecteur &drawing,GIAC_CONTEXT) {
    int s1=mrows(c),s2=mcols(c),i,j,col,t,R,G,B;
    gen w=(x2-x1)/gen(21.0),h=y2-y1,ts=(x2-x1)/gen(s2),fs=(y2-y1)/gen(s1),r=fs/ts,d,hi,minc=plus_inf,maxc=minus_inf;
    const_iterateur it=c.begin(),itend=c.end(),jt,jtend;
    for (;it!=itend;++it) for (jt=it->_VECTptr->begin(),jtend=it->_VECTptr->end();jt!=jtend;++jt) {
        minc=min(minc,*jt,contextptr);
        maxc=max(maxc,*jt,contextptr);
    }
    gen dm=maxc-minc,fr;
    if (is_zero(dm,contextptr))
        return;
    rgba_image *img=with_image?new rgba_image(with_transparency?4:3,s2,s1,contextptr):NULL;
    bool ok=true;
    for (i=0;i<s1;++i) {
        for (j=0;j<s2;++j) {
            fr=evalf_double((c[i][j]-minc)/dm,1,contextptr);
            if (fr.type!=_DOUBLE_)
                continue;
            if (!colormap_color_rgb(pal,fr.DOUBLE_val(),col,R,G,B,contextptr))
                ok=false;
            if (img!=NULL) {
                t=_floor(256*tanh(5*fr,contextptr),contextptr).val;
                img->set_pixel(0,j,s1-i-1,R);
                img->set_pixel(1,j,s1-i-1,G);
                img->set_pixel(2,j,s1-i-1,B);
                if (with_transparency)
                    img->set_pixel(3,j,s1-i-1,t);
            } else {
                gen A(x1+j*ts,y1+i*fs),B(x1+(j+1)*ts,y1+i*fs),C(x1+(j+1)*ts,y1+(i+1)*fs),D(x1+j*ts,y1+(i+1)*fs);
                drawing.push_back(pnt_attrib(gen(makevecteur(A,B,C,D,A),_GROUP__VECT),vecteur(1,_FILL_POLYGON+col),contextptr));
            }
        }
    }
    if (!ok) print_warning(gettext("failed to convert RGB color"),contextptr);
    if (img!=NULL) {
        if (!img->assure_on_disk()) {
            delete img;
            return arrayplot(c,x1,x2,y1,y2,pal,with_colormap,false,with_transparency,drawing,contextptr);
        }
        drawing.push_back(symbolic(at_rectangle,makesequence(gen(x1,y1),gen(x2,y1),h/(x2-x1),
                            symb_equal(change_subtype(_GL_TEXTURE,_INT_PLOT),string2gen(img->file_name(),false)))));
        delete img;
    }
    if (with_colormap) {
        minc=evalf_double(minc,1,contextptr);
        maxc=evalf_double(maxc,1,contextptr);
        int cb=50; // the number of color bins
        for (i=0;i<cb;++i) {
            d=symb_equal(at_display,change_subtype(_FILL_POLYGON+colormap_color(pal,double(i)/cb),_INT_COLOR));
            hi=y1+(i*h)/cb;
            drawing.push_back(_rectangle(makesequence(gen(x2+w,hi),gen(x2+2*w,hi),h/(cb*w),d),contextptr));
        }
        drawing.push_back(_rectangle(makesequence(gen(x2+w,y1),gen(x2+2*w,y1),h/w),contextptr));
        drawing.insert(drawing.begin(),symb_equal(change_subtype(_GL_X,_INT_PLOT),
                                       symb_interval(_evalf(-w,contextptr),_evalf(x2+3*w,contextptr))));
#if 0
        if (minc.type==_DOUBLE_)
            drawing.push_back(_legende(makesequence(gen(x2+2*w,y1),string2gen(print_DOUBLE_(minc.DOUBLE_val(),4),false)),contextptr));
        if (maxc.type==_DOUBLE_)
            drawing.push_back(_legende(makesequence(gen(x2+2*w,y2),string2gen(print_DOUBLE_(maxc.DOUBLE_val(),4),false),change_subtype(_QUADRANT4,_INT_COLOR)),contextptr));
#endif
    }
}

/* Return a visualization of the Hilbert spectrum HS.
 * S1 and S2 are numbers of columns and rows, respectively, of the spectrum HS.
 * FRAME is [tmin,tmax,fmin,fmax], specifying the location and size of drawing.
 * RAINBOW=true uses the rainbow colors (low amplitudes are violet, high are yellow),
 *              otherwise shades of grey are used (only 24 of them available in FLTK). */
void plot_hilbert_spectrum(const map<pair<int,int>,double> &hs,int s1,int s2,const vecteur &frame,
        int sr,int pal,bool with_image,bool cmap,bool transp,vecteur &drawing,GIAC_CONTEXT) {
    assert(frame.size()==4 && is_numericv(frame,num_mask_withfrac | num_mask_withint));
    gen t0=frame[0],t1=frame[1],f0=frame[2],f1=frame[3],ts=(t1-t0)/gen(s2);
    map<pair<int,int>,double>::const_iterator it=hs.begin(),itend=hs.end();
    matrice c=*_matrix(makesequence(s1,s2,0),contextptr)._VECTptr;
    for (;it!=itend;++it) {
        int i=it->first.first,j=it->first.second;
        double tpos=sr<=0?j:(double)j/(double)sr;
        if (!is_greater(tpos,t0,contextptr) || is_greater(tpos,t1,contextptr))
            continue;
        j=_floor((tpos-t0)/ts,contextptr).val;
        c[i]._VECTptr->at(j)=max(c[i][j],it->second,contextptr);
    }
    arrayplot(c,t0,t1,f0,f1,pal,cmap,with_image,transp,drawing,contextptr);
}

gen _hilbert(const gen &g,GIAC_CONTEXT){
    if (g.type==_STRNG &&g.subtype==-1) return g;
    int n,p;
    if (g.type==_INT_)
        n=p=g.val;
    else if (g.type==_VECT && g.subtype==_SEQ__VECT && g._VECTptr->size()==2 &&
             g._VECTptr->front().type==_INT_ && g._VECTptr->back().type==_INT_) {
        n=g._VECTptr->front().val;
        p=g._VECTptr->back().val; 
    } else if (g.type==_VECT && g.subtype!=_SEQ__VECT && !g._VECTptr->empty() &&
            is_numericv(*g._VECTptr,num_mask_withfrac | num_mask_withint))
        return dht(*g._VECTptr,contextptr);
    else try {
        return hilbtrans(g,contextptr);
    } catch (const std::runtime_error &err) {
        //*logptr(contextptr) << err.what() << "\n";
        return undef;
    }
    if (n<=0 || p<=0)
        return gensizeerr(contextptr);
    vecteur c;
    for (int k=0;k<n;k++) {
        vecteur l(p);
        for (int j=0;j<p;j++) {
            l[j]=rdiv(1,k+j+1,contextptr);
        }
        c.push_back(l);
    } 
    return gen(c,_MATRIX__VECT);
}
static const char _hilbert_s[]="hilbert";
static define_unary_function_eval (__hilbert,&_hilbert,_hilbert_s);
define_unary_function_ptr5(at_hilbert,alias_at_hilbert,&__hilbert,0,true);

gen _instfreq(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type==_SYMB) {
        if (_eval(identificateur("x"),contextptr).type!=_IDNT)
            return generr(gettext("Default variable not available"));
        return _instfreq(makesequence(g,identificateur("x")),contextptr);
    }
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    if (g.subtype==_SEQ__VECT) {
        if (g._VECTptr->size()!=2)
            return generrdim(gettext("Expected two arguments"));
        const gen &u=g._VECTptr->front(),&x=g._VECTptr->back();
        if (u.type!=_SYMB || x.type!=_IDNT)
            return gentypeerr(contextptr);
        gen N=u/_abs(u,contextptr);
        gen res=im(conj(N,contextptr)*_derive(makesequence(N,x),contextptr),contextptr);
        return ratsimp(res,contextptr);
    }
    int n;
    if ((n=g._VECTptr->size())<2)
        return vecteur(0);
    const_iterateur it=g._VECTptr->begin(),jt=it+1,jtend=g._VECTptr->end();
    vecteur omega;
    omega.reserve(n-1);
    for (;jt!=jtend;++it,++jt) omega.push_back(arg(*jt*conj(*it,contextptr),contextptr));
    return omega;
}
static const char _instfreq_s[]="instfreq";
static define_unary_function_eval (__instfreq,&_instfreq,_instfreq_s);
define_unary_function_ptr5(at_instfreq,alias_at_instfreq,&__instfreq,0,true);

gen _instphase(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type==_SYMB) {
        if (_eval(identificateur("x"),contextptr).type!=_IDNT)
            return generr(gettext("Default variable not available"));
        return _instphase(makesequence(g,identificateur("x")),contextptr);
    }
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    if (g.subtype==_SEQ__VECT) {
        if (g._VECTptr->size()!=2)
            return generrdim(gettext("Expected two arguments"));
        const gen &u=g._VECTptr->front(),&x=g._VECTptr->back();
        if (u.type!=_SYMB || x.type!=_IDNT)
            return gentypeerr(contextptr);
        gen N=u/_abs(u,contextptr);
        gen omega=_instfreq(g,contextptr);
        gen ph0=subst(arg(N,contextptr),x,0,false,contextptr),tau=identificateur(" tau");
        log_output_redirect lor(contextptr);
        gen res=ph0+_int(makesequence(subst(omega,x,tau,false,contextptr),tau,0,x),contextptr);
        return ratsimp(res,contextptr);
    }
    int n;
    if ((n=g._VECTptr->size())<1)
        return vecteur(0);
    const_iterateur it=g._VECTptr->begin(),jt=it+1,jtend=g._VECTptr->end();
    vecteur phase;
    phase.reserve(n);
    phase.push_back(arg(*it,contextptr));
    if (n>1) for (;jt!=jtend;++it,++jt) phase.push_back(phase.back()+arg(*jt*conj(*it,contextptr),contextptr));
    return phase;
}
static const char _instphase_s[]="instphase";
static define_unary_function_eval (__instphase,&_instphase,_instphase_s);
define_unary_function_ptr5(at_instphase,alias_at_instphase,&__instphase,0,true);

/* Converts MIDI note scpecification S to frequency F in Hz.
 * Example: midi2freq("F#5",f)
 * If the specification S is invalid, the return value is false, otherwise it is true. */
bool midi2freq(const string &s,gen &f) {
    string notes("C#D#EF#G#A#B");
    size_t name,pos=1;
    if (s.size()<2 || s.at(0)=='#' || (name=notes.find(toupper(s.at(0))))==string::npos)
        return false;
    if (s.at(1)=='#') {
        name++;
        pos++;
    }
    if (pos==s.size())
        return false;
    int oct=0;
    if (s.at(pos)!='0' && (oct=atoi(s.substr(pos).c_str()))==0)
        return false;
    int midi=(int)name+12*(oct+1);
    f=MIDI_NOTE_0*std::pow(2.0,(double)midi/12.0);
    return true;
}

/* Returns the Hilbert-Huang transform of a given signal. */
gen _hht(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    bool has_opt=g.subtype==_SEQ__VECT;
    gen fmin(undef),fmax(undef),tmin(0),tmax(undef);
    int n,sr=0,fbins=0,tbins=0,pal=7<<2,otype=1; // otype = 0: plot, 1: image, 2: matrix
    bool logf=false,semitones=false,cmap=true,transp=true;
    if (has_opt) {
        const vecteur &args=*g._VECTptr;
        if (args.empty())
            return gendimerr(contextptr);
        if (args.front().type!=_VECT)
            return gentypeerr(contextptr);
        const_iterateur it=args.begin()+1,itend=args.end();
        for (;it!=itend;++it) {
            if (*it==at_ln) {
                logf=true;
                continue;
            }
            if (!it->is_symb_of_sommet(at_equal))
                return generr(gettext("Unknown optional argument"));
            const gen &p=it->_SYMBptr->feuille._VECTptr->front();
            const gen &v=it->_SYMBptr->feuille._VECTptr->back();
            if (p==at_frequencies) {
                if (v.type==_VECT && v._VECTptr->size()==2 && v._VECTptr->front().type==_STRNG && v._VECTptr->back().type==_STRNG) {
                    if (!midi2freq(*v._VECTptr->front()._STRNGptr,fmin) || !midi2freq(*v._VECTptr->back()._STRNGptr,fmax))
                        return generr(gettext("Invalid MIDI note range specification"));
                    semitones=true;
                } else if (!get_range(v,fmin,fmax))
                    return generrtype(gettext("Invalid frequency range specification"));
            } else if (p==at_range) {
                if (!get_range(v,tmin,tmax))
                    return generrtype(gettext("Invalid time range specification"));
            } else if (p==at_samplerate) {
                if (!v.is_integer() || v.val<1)
                    return generr(gettext("Sample rate must be a positive integer"));
                sr=v.val;
            } else if (p==at_output) {
                if (v==at_plot)
                    otype=0;
                else if (v==at_image)
                    otype=1; // the default
                else if (v==at_matrix)
                    otype=2;
                else return generr(gettext("Unknown output type"));
            } else if (p.is_integer() && p.subtype==_INT_MAPLECONVERSION) {
                if (p.val==_KDE_BINS) {
                    if (!p.is_integer() || p.val<1)
                        return generr(gettext("Number of frequency bins must be a positive integer"));
                    fbins=p.val;
                }
                else return generr(gettext("Unknown optional argument"));
            } else if (p==at_ln) {
                if (!v.is_integer() || v.subtype!=_INT_BOOLEAN)
                    return generrtype(gettext("Expected a boolean value"));
                logf=(bool)v.val;
            } else if (p==at_legende) {
                if (v.is_integer() && v.subtype==_INT_BOOLEAN)
                    cmap=(bool)v.val;
                else return generr(gettext("Expected a boolean value"));
            } else if (p==at_tran) {
                if (v.is_integer() && v.subtype==_INT_BOOLEAN)
                    transp=(bool)v.val;
                else return generr(gettext("Expected a boolean value"));
            } else if (p==at_couleur) {
                if (!v.is_integer() || (v.subtype!=_INT_BOOLEAN && !is_colormap_index(v.val)))
                    return generrtype(gettext("Invalid colormap specification"));
                pal=v.subtype==_INT_BOOLEAN?((v.val==0?1:7)<<2):v.val;
            } else return generr(gettext("Unknown optional argument"));
        }
    }
    if (logf && sr<=0)
        return generr(gettext("Log-frequency requires sample rate"));
    if (fbins<=0) fbins=otype==1?200:100;
    if (tbins<=0) tbins=otype==1?450:220;
#if 0
    if (logf && is_undef(fmin) && is_undef(fmax)) {
        // set frequency range to match the one of a clavichord
        fmin=8*MIDI_NOTE_0;
        fmax=128*MIDI_NOTE_0;
    }
#endif
    const vecteur &data=*(has_opt?g._VECTptr->front():g)._VECTptr;
    matrice imf;
    map<pair<int,int>,double> hs;
    vecteur residue,frame,drawing;
    bool data_is_imf=ckmatrix(data);
    n=data_is_imf?mcols(data):data.size();
    if (!data_is_imf) {
#ifdef HAVE_LIBGSL
        emd(data,imf,residue,RAND_MAX,2,0.1,contextptr);
        if (imf.empty())
            return generr(gettext("No IMF found"));
#else
        return generr(gettext("GSL is required for empirical mode decomposition"));
#endif
    }
    hht(data_is_imf?data:imf,hs,sr,fmin,fmax,fbins,logf,contextptr);
    gen flabel=string2gen(logf?gettext("semitones"):(sr>0?gettext("frequency [Hz]"):gettext("ω [radians/sample]")),false);
    gen tlabel=string2gen(sr>0?gettext("time [seconds]"):gettext("offset [samples]"),false);
    if (otype==2) {
        matrice retm=*_matrix(makesequence(fbins,n,0),contextptr)._VECTptr;
        map<pair<int,int>,double>::const_iterator it=hs.begin(),itend=hs.end();
        for (;it!=itend;++it) retm[it->first.first]._VECTptr->at(it->first.second)=it->second;
        return retm;
    }
    if (is_undef(tmax))
        tmax=sr<=0?n:(double)n/(double)sr;
    if (logf) {
        fmin=12*ln(fmin/MIDI_NOTE_0,contextptr)/gen(M_LN2);
        fmax=12*ln(fmax/MIDI_NOTE_0,contextptr)/gen(M_LN2);
    }
    frame=*_evalf(makevecteur(tmin,tmax,fmin,fmax),contextptr)._VECTptr;
    drawing.push_back(symb_equal(change_subtype(_LABELS,_INT_PLOT),makevecteur(tlabel,flabel)));
    drawing.push_back(symb_equal(change_subtype(_AXES,_INT_PLOT),3));
    plot_hilbert_spectrum(hs,fbins,tbins,frame,sr,pal,otype,cmap,transp,drawing,contextptr);
    return gen(drawing,_SEQ__VECT);
}
static const char _hht_s []="hht";
static define_unary_function_eval (__hht,&_hht,_hht_s);
define_unary_function_ptr5(at_hht,alias_at_hht,&__hht,0,true)


gen apply_window_function(int wf,const gen &arg,GIAC_CONTEXT) {
    if (arg.type==_STRNG && arg.subtype==-1) return arg;
    int start=0,len=0;
    vecteur *data,res;
    double alpha;
    bool has_parm=true;
    if (arg.type!=_VECT)
        return gentypeerr(contextptr);
    switch (wf) {
    case _BLACKMAN_WINDOW: alpha=0.16; break;
    case _COSINE_WINDOW: case _HANN_POISSON_WINDOW: case _POISSON_WINDOW: alpha=1.0; break;
    case _GAUSSIAN_WINDOW: alpha=0.1; break;
    case _TRIANGLE_WINDOW: alpha=0; break;
    case _TUKEY_WINDOW: alpha=0.5; break;
    case _HANN_WINDOW: alpha=2.0; break;
    default: has_parm=false;
    }
    if (arg.subtype==_SEQ__VECT) {
        const vecteur &args=*arg._VECTptr;
        if (args.empty())
            return gendimerr(contextptr);
        if (args.front().type!=_VECT)
            return gentypeerr(contextptr);
        data=args.front()._VECTptr;
        const gen *parm=NULL,*offset=NULL,*ws=NULL;
        if (has_parm && args.size()==2)
            parm=&args[1];
        if (args.size()>2) {
            offset=&args[1];
            ws=&args[2];
        }
        if (has_parm && args.size()==4)
            parm=&args[3];
        if (parm) {
            if (!is_real_number(*parm,contextptr))
                return gentypeerr(contextptr);
            switch (wf) {
            case _GAUSSIAN_WINDOW:
                if (is_strictly_greater(*parm,0.5,contextptr))
                    return generr(gettext("Parameter value should be less than 0.5"));
                break;
            case _TRIANGLE_WINDOW:
                if (!is_one(*parm) && !is_minus_one(*parm) && !is_exactly_zero(*parm))
                    return generr(gettext("Parameter value should be -1, 0, or 1"));
                break;
            case _TUKEY_WINDOW:
                if (is_strictly_positive(-*parm,contextptr) || is_strictly_greater(*parm,1,contextptr))
                    return generr(gettext("Parameter value should be in [0,1]"));
                break;
            }
            alpha=to_real_number(*parm,contextptr).to_double(contextptr);
        }
        if (offset) {
            if (!offset->is_integer())
                return gentypeerr(contextptr);
            start=offset->val;
        }
        if (ws) {
            if (!ws->is_integer())
                return gentypeerr(contextptr);
            len=ws->val;
            if (len<=0)
                return gensizeerr(contextptr);
        }
    } else data=arg._VECTptr;
    int data_size=data->size();
    if (start<0 || start>=data_size)
        return generr(gettext("Invalid window start position"));
    if (len==0)
        len=data_size-start;
    if (len<2)
        return generr(gettext("Window size should be at least 2"));
    if (len>data_size-start)
        return generr(gettext("Window size is too large"));
    res.clear();
    res.reserve(len);
    int j=0;
    double s,d=double(len-1),a0=0.62,a1=0.48,a2=0.38,b0=0.35875,b1=0.48829,b2=0.14128,b3=0.01168,xk;
    double hamming_alpha=0.54,hamming_beta=0.46,tukey_beta=alpha*d/2.0;
    const_iterateur it=data->begin()+start;
    for (;j<len;++j,++it) {
        xk=std::abs(2*j/d-1);
        switch (wf) {
        case _BARTLETT_HANN_WINDOW:
            s=a0-a1*std::abs(j/d-0.5)-a2*std::cos(2*j*M_PI/d);
            break;
        case _BLACKMAN_HARRIS_WINDOW:
            s=b0-b1*std::cos(2*j*M_PI/d)+b2*std::cos(4*j*M_PI/d)-b3*std::cos(6*j*M_PI/d);
            break;
        case _BLACKMAN_WINDOW:
            s=(1-alpha)/2.0-0.5*std::cos(2*j*M_PI/d)+alpha/2.0*std::cos(4*j*M_PI/d);
            break;
        case _BOHMAN_WINDOW:
            s=(1-xk)*std::cos(M_PI*xk)+std::sin(M_PI*xk)/M_PI;
            break;
        case _HANN_WINDOW:
        case _COSINE_WINDOW:
            s=std::pow(std::sin(j*M_PI/d),alpha);
            break;
        case _GAUSSIAN_WINDOW:
            s=std::exp(-0.5*std::pow((j-d/2.0)/(alpha*d/2.0),2));
            break;
        case _HAMMING_WINDOW:
            s=hamming_alpha-hamming_beta*std::cos(2*j*M_PI/d);
            break;
        case _HANN_POISSON_WINDOW:
            s=0.5*(1-std::cos(2*j*M_PI/d))*std::exp(-alpha*std::abs(d-2*j)/d);
            break;
        case _PARZEN_WINDOW:
            s=1+std::abs(2*(len-1)-4*j)<=len?1-6*xk*xk*(1-xk):2*std::pow(1-xk,3);
            break;
        case _POISSON_WINDOW:
            s=std::exp(-alpha*xk);
            break;
        case _RIEMANN_WINDOW:
            s=2*j+1==len?1:std::sin(M_PI*xk)/(M_PI*xk);
            break;
        case _TRIANGLE_WINDOW:
            s=1-std::abs(2.0*(j-d/2)/(len+alpha));
            break;
        case _TUKEY_WINDOW:
            s=j<tukey_beta?0.5*(1+std::cos(M_PI*(j/tukey_beta-1)))
                :(j>=tukey_beta && j<=d*(1-alpha/2.0)?1:0.5*(1+std::cos(M_PI*(j/tukey_beta-2.0/alpha+1))));
            break;
        case _WELCH_WINDOW:
            s=1-std::pow(2.0*(j-d/2)/d,2);
            break;
        default:
            assert(false); // unknown window function!
        }
        res.push_back(s*(*it));
    }
    return res;
}

bool nivelate(vecteur &data,int k,const gen &b,const gen &val,const unary_function_ptr *comp,GIAC_CONTEXT) {
    gen r;
    if (has_i(data[k]) && !is_zero((r=_abs(data[k],contextptr)))) {
        if (_eval(symbolic(comp,makesequence(r,b)),contextptr).val!=0) {
            data[k]=val*data[k]/r;
            return true;
        }
    } else if (_eval(symbolic(comp,makesequence(data[k],b)),contextptr).val!=0) {
        data[k]=val;
        return true;
    }
    return false;
}

gen _threshold(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &args=*g._VECTptr;
    if (int(args.size())<2)
        return gendimerr(contextptr);
    if (args.front().type!=_VECT)
        return gentypeerr(contextptr);
    const vecteur &data=*args.front()._VECTptr;
    if (ckmatrix(data)) {
        matrice rmat;
        rmat.reserve(data.size());
        const_iterateur it=data.begin(),itend=data.end();
        vecteur a(args.begin()+1,args.end());
        a.insert(a.begin(),0);
        for (;it!=itend;++it) {
            a.front()=*it;
            rmat.push_back(_threshold(change_subtype(a,_SEQ__VECT),contextptr));
        }
        return rmat;
    }
    gen bnd=args.at(1);
    int n=data.size();
    vecteur output=data;
    if (bnd.type==_VECT) {
        if (int(bnd._VECTptr->size())!=2)
            return generr(gettext("Expected a list with two elements"));
        gen lb=bnd._VECTptr->front(),ub=bnd._VECTptr->back(),lval,uval;
        if (is_equal(lb)) {
            lval=_rhs(lb,contextptr);
            lb=_lhs(lb,contextptr);
        } else lval=lb;
        if (is_equal(ub)) {
            uval=_rhs(ub,contextptr);
            ub=_lhs(ub,contextptr);
        } else uval=ub;
        for (int k=0;k<n;++k) {
            if (!nivelate(output,k,lb,lval,at_inferieur_strict,contextptr))
                nivelate(output,k,ub,uval,at_superieur_strict,contextptr);
        }
    } else {
        gen val;
        if (is_equal(bnd)) {
            val=_rhs(bnd,contextptr);
            bnd=_lhs(bnd,contextptr);
        } else val=bnd;
        if (!is_real_number(bnd,contextptr))
            return generrtype(gettext("Expected a real constant"));
        gen comp=at_inferieur_strict,isabs;
        bool absolute=false;
        for (const_iterateur it=args.begin()+2;it!=args.end();++it) {
            if (*it==at_superieur_strict || *it==at_superieur_egal || *it==at_inferieur_egal)
                comp=*it;
            isabs=gen(1);
            if (*it==at_abs || (is_equal(*it) &&
                                it->_SYMBptr->feuille._VECTptr->front()==at_abs &&
                                (isabs=it->_SYMBptr->feuille._VECTptr->back()).type==_INT_ &&
                                isabs.subtype==_INT_BOOLEAN)) {
                if (has_i(data) || !is_strictly_positive(bnd,contextptr))
                    return gensizeerr(contextptr);
                absolute=(bool)isabs.val;
            }
        }
        for (int k=0;k<n;++k) {
            if (absolute) {
                if (_eval(symbolic(comp._FUNCptr,makesequence(_abs(data[k],contextptr),bnd)),contextptr).val!=0)
                    output[k]=is_positive(data[k],contextptr)?val:-val;
            } else nivelate(output,k,bnd,val,comp._FUNCptr,contextptr);
        }
    }
    return _eval(output,contextptr);
}
static const char _threshold_s []="threshold";
static define_unary_function_eval (__threshold,&_threshold,_threshold_s);
define_unary_function_ptr5(at_threshold,alias_at_threshold,&__threshold,0,true)

gen _bartlett_hann_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_BARTLETT_HANN_WINDOW,g,contextptr);
}
static const char _bartlett_hann_window_s []="bartlett_hann_window";
static define_unary_function_eval (__bartlett_hann_window,&_bartlett_hann_window,_bartlett_hann_window_s);
define_unary_function_ptr5(at_bartlett_hann_window,alias_at_bartlett_hann_window,&__bartlett_hann_window,0,true)

gen _blackman_harris_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_BLACKMAN_HARRIS_WINDOW,g,contextptr);
}
static const char _blackman_harris_window_s []="blackman_harris_window";
static define_unary_function_eval (__blackman_harris_window,&_blackman_harris_window,_blackman_harris_window_s);
define_unary_function_ptr5(at_blackman_harris_window,alias_at_blackman_harris_window,&__blackman_harris_window,0,true)

gen _blackman_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_BLACKMAN_WINDOW,g,contextptr);
}
static const char _blackman_window_s []="blackman_window";
static define_unary_function_eval (__blackman_window,&_blackman_window,_blackman_window_s);
define_unary_function_ptr5(at_blackman_window,alias_at_blackman_window,&__blackman_window,0,true)

gen _bohman_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_BOHMAN_WINDOW,g,contextptr);
}
static const char _bohman_window_s []="bohman_window";
static define_unary_function_eval (__bohman_window,&_bohman_window,_bohman_window_s);
define_unary_function_ptr5(at_bohman_window,alias_at_bohman_window,&__bohman_window,0,true)

gen _cosine_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_COSINE_WINDOW,g,contextptr);
}
static const char _cosine_window_s []="cosine_window";
static define_unary_function_eval (__cosine_window,&_cosine_window,_cosine_window_s);
define_unary_function_ptr5(at_cosine_window,alias_at_cosine_window,&__cosine_window,0,true)

gen _gaussian_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_GAUSSIAN_WINDOW,g,contextptr);
}
static const char _gaussian_window_s []="gaussian_window";
static define_unary_function_eval (__gaussian_window,&_gaussian_window,_gaussian_window_s);
define_unary_function_ptr5(at_gaussian_window,alias_at_gaussian_window,&__gaussian_window,0,true)

gen _hamming_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_HAMMING_WINDOW,g,contextptr);
}
static const char _hamming_window_s []="hamming_window";
static define_unary_function_eval (__hamming_window,&_hamming_window,_hamming_window_s);
define_unary_function_ptr5(at_hamming_window,alias_at_hamming_window,&__hamming_window,0,true)

gen _hann_poisson_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_HANN_POISSON_WINDOW,g,contextptr);
}
static const char _hann_poisson_window_s []="hann_poisson_window";
static define_unary_function_eval (__hann_poisson_window,&_hann_poisson_window,_hann_poisson_window_s);
define_unary_function_ptr5(at_hann_poisson_window,alias_at_hann_poisson_window,&__hann_poisson_window,0,true)

gen _hann_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_HANN_WINDOW,g,contextptr);
}
static const char _hann_window_s []="hann_window";
static define_unary_function_eval (__hann_window,&_hann_window,_hann_window_s);
define_unary_function_ptr5(at_hann_window,alias_at_hann_window,&__hann_window,0,true)

gen _parzen_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_PARZEN_WINDOW,g,contextptr);
}
static const char _parzen_window_s []="parzen_window";
static define_unary_function_eval (__parzen_window,&_parzen_window,_parzen_window_s);
define_unary_function_ptr5(at_parzen_window,alias_at_parzen_window,&__parzen_window,0,true)

gen _poisson_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_POISSON_WINDOW,g,contextptr);
}
static const char _poisson_window_s []="poisson_window";
static define_unary_function_eval (__poisson_window,&_poisson_window,_poisson_window_s);
define_unary_function_ptr5(at_poisson_window,alias_at_poisson_window,&__poisson_window,0,true)

gen _riemann_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_RIEMANN_WINDOW,g,contextptr);
}
static const char _riemann_window_s []="riemann_window";
static define_unary_function_eval (__riemann_window,&_riemann_window,_riemann_window_s);
define_unary_function_ptr5(at_riemann_window,alias_at_riemann_window,&__riemann_window,0,true)

gen _triangle_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_TRIANGLE_WINDOW,g,contextptr);
}
static const char _triangle_window_s []="triangle_window";
static define_unary_function_eval (__triangle_window,&_triangle_window,_triangle_window_s);
define_unary_function_ptr5(at_triangle_window,alias_at_triangle_window,&__triangle_window,0,true)

gen _tukey_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_TUKEY_WINDOW,g,contextptr);
}
static const char _tukey_window_s []="tukey_window";
static define_unary_function_eval (__tukey_window,&_tukey_window,_tukey_window_s);
define_unary_function_ptr5(at_tukey_window,alias_at_tukey_window,&__tukey_window,0,true)

gen _welch_window(const gen &g,GIAC_CONTEXT) {
    return apply_window_function(_WELCH_WINDOW,g,contextptr);
}
static const char _welch_window_s []="welch_window";
static define_unary_function_eval (__welch_window,&_welch_window,_welch_window_s);
define_unary_function_ptr5(at_welch_window,alias_at_welch_window,&__welch_window,0,true)

/*
 * ANN CLASS IMPLEMENTATION (Artificial Neural Networks)
 * A simple implementation of feedforward neural network (multilayer perceptron) using GSL
 */
#ifdef HAVE_LIBGSL
/* Constructors. */
ann::ann(const vector<int> &topology,int bsize,GIAC_CONTEXT) {
    if (topology.size()<3)
        throw std::runtime_error(gettext("At least three layers are required"));
    ctx=contextptr;
    // block_size: the number of input vectors that can be passed through the network at once
    block_size=bsize;
    // no momentum by default
    momentum=0;
    // default Adam parameters from the paper
    beta1=0.9; beta2=0.999;
    // default initial learning rate and schedule multiplier
    learning_rate=1e-3; schedule=1.0;
    // iteration counter
    iter=accum_count=0;
    // topology
    this->topology=topology;
    // epsilon: used for preventing division by zero (default value from Adam paper)
    eps=1e-8; // epsilon(contextptr);
    // this network is a regression model by default
    _is_classifier=false;
    // construct the network
    create();
    // no regularization by default
    gsl_vector_set_zero(reg_coeff);
    // create temporary symbolic variables
    x=identificateur(" nn_x");
    //_purge(x,ctx);
    y.reserve(topology.back());
    t.reserve(topology.back());
    for (int i=0;i<topology.back();++i) {
        y.push_back(identificateur(" y"+print_INT_(i+1)));
        t.push_back(identificateur(" t"+print_INT_(i+1)));
        //_purge(makesequence(y.back(),t.back()),ctx);
    }
    fan=makevecteur(identificateur(" fan_in"),identificateur(" fan_out"));
}
ann::ann(const ann &other) {
    assign(other,other.block_size);
}
ann::ann(const ann &other,int bs) {
    assign(other,bs);
}
/* Assign OTHER to this network. */
ann &ann::operator=(const ann &other) {
    deallocate();
    layers.clear();
    cache.clear();
    deltas.clear();
    weights.clear();
    Deltas.clear();
    moment1.clear();
    moment2.clear();
    assign(other,other.block_size);
    return *this;
}
void ann::assign(const ann &other,int bs) {
    ctx=other.ctx;
    topology=other.topology;
    learning_rate=other.learning_rate;
    block_size=bs;
    eps=other.eps;
    momentum=other.momentum;
    beta1=other.beta1;
    beta2=other.beta2;
    iter=other.iter;
    accum_count=other.accum_count;
    schedule=other.schedule;
    y=other.y;
    t=other.t;
    fan=other.fan;
    errfunc=other.errfunc;
    activation=other.activation;
    output_activation=other.output_activation;
    labels=other.labels;
    _is_classifier=other._is_classifier;
    title=other.title;
    create();
    gsl_vector_memcpy(this->reg_coeff,other.reg_coeff);
    int lc=layer_count(),i;
    for (i=0;i<lc;++i) {
        gsl_matrix_memcpy(layers[i],other.layer(i));
        if (i>0) {
            gsl_matrix_memcpy(weights[i-1],other.weight_matrix(i-1));
            gsl_matrix_memcpy(Deltas[i-1],other.weight_updates(i-1));
            gsl_matrix_memcpy(moment1[i-1],other.moment1[i-1]);
            gsl_matrix_memcpy(moment2[i-1],other.moment2[i-1]);
        }
    }
}
/* Create the network parameters and allocate the required space. */
void ann::create() {
    reg_coeff=gsl_vector_alloc(layer_count()-1);
    eout=eout=gsl_matrix_alloc(output_size(),block_size);
    if (reg_coeff==NULL || eout==NULL)
        throw std::bad_alloc();
    int lc=layer_count(),ti,tip,j,sz;
    for (int i=0;i<lc;++i) {
        ti=topology[i];
        sz=ti+(i==lc-1?0:1);
        if (ti<1)
            throw std::runtime_error(gettext("Invalid number of neurons in a layer"));
        layers.push_back(gsl_matrix_alloc(sz,block_size));
        cache.push_back(i==0?NULL:gsl_matrix_alloc(sz,block_size));
        deltas.push_back(i==0?NULL:gsl_matrix_alloc(sz,block_size));
        if (layers.back()==NULL || (i>0 && (deltas.back()==NULL || cache.back()==NULL)))
            throw std::bad_alloc();
        if (i!=lc-1) { // initialize bias neurons and constant cache
            for (int b=0;b<block_size;++b) {
                gsl_matrix_set(layers.back(),ti,b,1.0);
                if (i>0) gsl_matrix_set(cache.back(),ti,b,1.0);
            }
        }
        if (i>0) { // initialize all weights (including bias) to zero
            tip=topology[i-1];
            weights.push_back(gsl_matrix_calloc(tip+1,sz));
            Deltas.push_back(gsl_matrix_calloc(tip+1,sz));
            moment1.push_back(gsl_matrix_calloc(tip+1,sz));
            moment2.push_back(gsl_matrix_calloc(tip+1,sz));
            if (weights.back()==NULL || Deltas.back()==NULL || moment1.back()==NULL || moment2.back()==NULL)
                throw std::bad_alloc();
            gsl_matrix_set(weights.back(),tip,ti,1.0);
        }
    }
}
/* Free all GSL array data in the network. */
void ann::deallocate() {
    if (reg_coeff!=NULL) gsl_vector_free(reg_coeff);
    if (eout!=NULL) gsl_matrix_free(eout);
    vector<gsl_matrix*>::const_iterator jt,jtend;
    for (jt=layers.begin(),jtend=layers.end();jt!=jtend;++jt)
        if (*jt!=NULL) gsl_matrix_free(*jt);
    for (jt=cache.begin(),jtend=cache.end();jt!=jtend;++jt)
        if (*jt!=NULL) gsl_matrix_free(*jt);
    for (jt=deltas.begin(),jtend=deltas.end();jt!=jtend;++jt)
        if (*jt!=NULL) gsl_matrix_free(*jt);
    for (jt=weights.begin(),jtend=weights.end();jt!=jtend;++jt)
        if (*jt!=NULL) gsl_matrix_free(*jt);
    for (jt=Deltas.begin(),jtend=Deltas.end();jt!=jtend;++jt)
        if (*jt!=NULL) gsl_matrix_free(*jt);
    for (jt=moment1.begin(),jtend=moment1.end();jt!=jtend;++jt)
        if (*jt!=NULL) gsl_matrix_free(*jt);
    for (jt=moment2.begin(),jtend=moment2.end();jt!=jtend;++jt)
        if (*jt!=NULL) gsl_matrix_free(*jt);    
}
/* Set momentum mom (for 0<=mom<1 use classical momentum and for mom=1 use Adam). */
void ann::set_momentum(double mom) {
    if (mom<0)
        throw std::runtime_error(gettext("Momentum should be nonnegative"));
    momentum=std::min(1.0,mom);
}
/* Set activation function for LAYER. If LAYER=-1, it is set for all hidden neurons.
 * e.g. set_activation(-1,tanh)
 * or set_activation(2,logistic,[0,2,1.5]) <- assuming layer count of three
 * or set_activation(-1,piecewise(x< -pi/2, -1, x<pi/2, sin(x), 1)). */
void ann::set_activation(bool out,const gen &f,const vecteur &params) {
    gen res,dif;
    vecteur s,act;
    int pf=0;
    if (f==at_id)
        pf=_ANN_LINEAR;
    else if (f==at_logistic)
        pf==_ANN_SIGMOID;
    else if (f==at_tanh)
        pf=_ANN_TANH;
    else if (f.is_integer() && f.subtype==_INT_MAPLECONVERSION)
        pf=f.val;
    if (pf>0) {
        act.push_back(change_subtype(pf,_INT_MAPLECONVERSION));
        if (!is_numericv(params,num_mask_withfrac|num_mask_withint))
            throw std::runtime_error(gettext("Activation function parameters should be numeric"));
        act.push_back(_evalf(params,ctx));
    } else { // custom activation function
        switch (f.type) {
        case _FUNC:
        case _SYMB:
            if (f.type==_FUNC || f.is_symb_of_sommet(at_program)) {
                if (f.type==_SYMB && f._SYMBptr->feuille._VECTptr->front()._VECTptr->size()!=params.size()+1)
                    throw std::runtime_error(gettext("Wrong number of arguments to function"));
                res=f(gen(mergevecteur(vecteur(1,x),params),_SEQ__VECT),ctx);
                if (_lname(res,ctx)._VECTptr->size()>1)
                    throw std::runtime_error(gettext("Function contains uninitialized parameters"));
                break;
            } else {
                if (!params.empty())
                    throw std::runtime_error(gettext("No parameters can be passed alongside a symbolic expression"));
                s=*_lname(f,ctx)._VECTptr;
                if (s.size()!=1)
                    throw std::runtime_error(gettext("Expected a univariate symbolic expression"));
                res=subst(f,s.front(),x,false,ctx);
            }
            break;
        default:
            throw std::runtime_error(gettext("Invalid activation function type"));
        }
        dif=_unapply(makesequence(_derive(makesequence(res,x),ctx),x),ctx);
        act=makevecteur(_unapply(makesequence(res,x),ctx),dif);
    }
    if (out) output_activation=act;
    else activation=act;
}
/* Set the error function.
 * e.g. set_error_function(distance2) or, if you have a function f(x,y,a,b,...) where a,b,... are
 * parameters, use: set_error_function(f,[A,B,...]) where A,B,... are the parameter values (fixed). */
void ann::set_error_function(const gen &f,const vecteur &params) {
    if (f.is_integer() && f.subtype==_INT_MAPLECONVERSION) {
        if (!is_numericv(params,num_mask_withfrac|num_mask_withint))
            throw std::runtime_error(gettext("Error function parameters should be numeric"));
        errfunc=makevecteur(f,_evalf(params,ctx));
    } else {
        if (f.type!=_FUNC && !f.is_symb_of_sommet(at_program))
            throw std::runtime_error("Invalid error function type");
        if (f.type==_SYMB && f._SYMBptr->feuille._VECTptr->front()._VECTptr->size()!=params.size()+1)
            throw std::runtime_error(gettext("Wrong number of arguments to function"));
        vecteur vars=mergevecteur(t,y);
        gen res=f(gen(mergevecteur(makevecteur(t,y),params),_SEQ__VECT),ctx);
        if ((int)_lname(res,ctx)._VECTptr->size()>2*output_size())
            throw std::runtime_error(gettext("Function contains uninitialized parameters"));
        errfunc=makevecteur(_unapply(gen(mergevecteur(vecteur(1,res),vars),_SEQ__VECT),ctx),
                            _unapply(gen(mergevecteur(vecteur(1,_grad(makesequence(res,y),ctx)),vars),_SEQ__VECT),ctx));
    }
}
/* Set labels for output neurons. */
void ann::set_labels(const vecteur &lab,bool classes) {
    labels=lab;
    if (classes && output_size()==1) {
        if (lab.size()!=2)
            throw std::runtime_error(gettext("Expected 2 labels for binary classification"));
    } else {
        if ((int)lab.size()!=output_size())
            throw std::runtime_error(gettext("Number of labels does not match the output size"));
    }
    _is_classifier=classes;
}
/* Set L2-regularization (weight decay) parameters. */
void ann::set_regularization(const vecteur &reg) const {
    if (reg.size()!=reg_coeff->size)
        throw std::runtime_error(gettext("Invalid number of regularization parameters"));
    if (vecteur2gsl_vector(reg,reg_coeff,ctx)!=GSL_SUCCESS)
        throw std::runtime_error(gettext("Failed to set regularization parameters"));
}
/* Return the network type as a string. */
string ann::network_type() const {
    switch (task()) {
        case 1: return gettext("a classifier");
        case 2: return gettext("a multi-label classifier");
        default: break;
    }
    return gettext("a neural network");
}
/* Apply activation function or its derivative to A. */
void ann::activate(int i,double &a,double *b) const {
    bool ones=_is_classifier && i+1==layer_count();
    const vecteur &f=(i+1==layer_count()?output_activation:activation);
    double e,v;
    if (f.front().is_integer() && f.front().subtype==_INT_MAPLECONVERSION) {
        const vecteur &params=*f.back()._VECTptr;
        bool nop=params.empty();
        switch (f.front().val) {
        case _ANN_LINEAR:
            if (b!=NULL) *b=1.0;
            break;
        case _ANN_SIGMOID:
            e=a>0?std::exp(-a):std::exp(a);
            v=(a>0?1.0:e)/(1.0+e);
            a=v;
            if (b!=NULL) *b=ones?1.0:v*(1-v);
            break;
        case _ANN_TANH:
            v=std::tanh(a);
            a=v;
            if (b!=NULL) *b=ones?1.0:1.0-v*v;
            break;
        case _ANN_RELU: // leaky ReLU if parameter is set
            v=nop?0.0:params.front().to_double(ctx);
            if (b!=NULL) *b=ones||a>0?1.0:v;
            if (a<0) a*=v;
            break;
        default: // not reachable
            a=giac::nan();
            if (b!=NULL) *b=giac::nan();
            break;
        }
    } else {
        if (b!=NULL) *b=ones?1.0:f[1](a,ctx).to_double(ctx);
        a=f[0](a,ctx).to_double(ctx);
    }
}
/* Propagate a block of samples forward. */
void ann::propagate_forward(bool comp_deriv) const {
    int i,j,k,ti,len,lc=layer_count(),os=output_size(),s1;
    gsl_vector_view mc;
    double s,*o,m;
    bool softmax=is_default_classifier() && os>1;
    for (i=1;i<lc;++i) {
        s1=layers[i]->size1;
        gsl_blas_dgemm(CblasTrans,CblasNoTrans,1.0,weights[i-1],layers[i-1],0.0,layers[i]);
        if (softmax && i+1==lc) {
            for (k=0;k<block_size;++k) {
                mc=gsl_matrix_column(layers[i],k);
                m=gsl_vector_max(&mc.vector);
                s=0;
                for (j=0;j<os;++j) {
                    o=gsl_vector_ptr(&mc.vector,j);
                    s+=(*o=std::exp(*o-m));
                }
                gsl_vector_scale(&mc.vector,1.0/s);
            }
            if (comp_deriv) gsl_matrix_set_all(cache.back(),1.0);
        } else {
            len=(s1-(i+1==lc?0:1))*block_size;
            for (j=0;j<len;++j)
                activate(i,layers[i]->data[j],comp_deriv?&cache[i]->data[j]:NULL);
        }
    }
} 
/* Return E(t,y). t can be changed! */
gen ann::compute_errfunc(gsl_vector *t,gsl_vector *y) const {
    if (errfunc.size()!=2)
        throw std::runtime_error(gettext("Error function not defined"));
    if (errfunc.front().is_integer() && errfunc.front().subtype==_INT_MAPLECONVERSION) {
        const vecteur &params=*errfunc.back()._VECTptr;
        int i,n=y->size,ef=errfunc.front().val;
        double res,ti,yi;
        switch (ef) {
        case _ANN_HALF_MSE:
            gsl_vector_sub(t,y);
            gsl_blas_ddot(t,t,&res);
            return res/(2.0*n);
        case _ANN_CROSS_ENTROPY:
        case _ANN_LOG_LOSS:
            res=0;
            for (i=0;i<n;++i) {
                ti=gsl_vector_get(t,i);
                yi=gsl_vector_get(y,i);
                if (ti>0.0) res-=ti*std::log(std::max(yi,eps));
                if (ef==_ANN_LOG_LOSS)
                    if (ti<1.0) res-=(1.0-ti)*std::log(std::max(1.0-yi,eps));
            }
            return res;
        default:
            break;
        }
        return undef; // not reachable
    }
    vecteur args=mergevecteur(gsl_vector2vecteur(t),gsl_vector2vecteur(y));
    return errfunc.front()(gen(args,_SEQ__VECT),ctx);
}
/* Compute dE(t,y)/dy and store it to res. */
void ann::compute_errfunc_diff(gsl_matrix *t,gsl_matrix *y,gsl_matrix *res) const {
    if (errfunc.size()!=2)
        throw std::runtime_error(gettext("Error function not defined"));
    int n=y->size1,m=y->size2;
    if (errfunc.front().is_integer() && errfunc.front().subtype==_INT_MAPLECONVERSION) {
        const vecteur &params=*errfunc.back()._VECTptr;
        int i,len=m*n,ef=errfunc.front().val;
        double ti,yi;
        switch (ef) {
        case _ANN_HALF_MSE:
            gsl_matrix_memcpy(res,y);
            gsl_matrix_sub(res,t);
            break;
        case _ANN_CROSS_ENTROPY:
        case _ANN_LOG_LOSS:
            for (i=0;i<len;++i) {
                ti=t->data[i];
                yi=y->data[i];
                if (ti>0.0) res->data[i]=-ti/std::max(yi,eps);
                if (ef==_ANN_LOG_LOSS)
                    if (ti<1.0) res->data[i]-=(1.0-ti)/std::max(1.0-yi,eps);
            }
            break;
        default:
            assert(false); // not reachable
        }
    } else {
        matrice args=mtran(mergevecteur(gsl_matrix2matrice(t),gsl_matrix2matrice(y))),val;
        val.reserve(m);
        const_iterateur it=args.begin(),itend=args.end();
        for (;it!=itend;++it)
            val.push_back(errfunc.back()(gen(*it,_SEQ__VECT),ctx));
        if (matrice2gsl_matrix(val,0,0,0,0,true,res,ctx)!=GSL_SUCCESS)
            throw std::runtime_error(gettext("Failed to compute the error function derivative"));
    }
}
/* Return true iff this network is a classifier with usual error function. */
bool ann::is_default_classifier() const {
    if (!_is_classifier)
        return false;
    const gen &e=errfunc.front();
    if (!e.is_integer() || e.subtype!=_INT_MAPLECONVERSION)
        return false;
    return e.val==(output_size()==1?_ANN_LOG_LOSS:_ANN_CROSS_ENTROPY);
}
/* Calculate the errors made by neurons. */
void ann::calc_deltas() const {
    int i,j,len,lc=layer_count();
    if (is_default_classifier()) {
        gsl_matrix_memcpy(deltas.back(),layers.back());
        gsl_matrix_sub(deltas.back(),eout);
    } else compute_errfunc_diff(eout,layers.back(),deltas.back());
    for (i=lc-2;i>=0;--i) {
        gsl_matrix_mul_elements(deltas[i+1],cache[i+1]);
        if (i>0)
            gsl_blas_dgemm(CblasNoTrans,CblasNoTrans,1.0,weights[i],deltas[i+1],0.0,deltas[i]);
    }
}
/* Accumulate weight deltas. */
void ann::update_Deltas() {
    gsl_matrix_view dm,lm;
    int lc=layer_count(),i;
    for (i=0;i<lc-1;++i)
        gsl_blas_dgemm(CblasNoTrans,CblasTrans,1.0,layers[i],deltas[i+1],1.0,Deltas[i]);
}
/* Update weights. */
void ann::update_weights() {
    if (accum_count==0)
        return;
    ++iter;
    double eta=_evalf(schedule.is_symb_of_sommet(at_program)?schedule(iter,ctx):schedule,ctx).to_double(ctx);
    double lambda,e=std::sqrt(1.0-std::pow(beta2,iter)),c=eps*e,lr=eta*learning_rate,alpha=lr*e/(1.0-std::pow(beta1,iter)),l;
    int i,lc=layer_count();
    size_t j,len,n1,n2,bias_start;
    bool adam=(momentum==1);
    for (i=0;i<lc-1;++i) {
        n1=weights[i]->size1;
        n2=weights[i]->size2;
        len=n1*n2;
        bias_start=n2*(n1-1);
        lambda=gsl_vector_get(reg_coeff,i);
        l=1.0-eta*lambda;
        for (j=0;j<len;++j) {
            if (i<lc-2 && (j+1)%n2==0)
                continue;
            double &d=Deltas[i]->data[j],&w=weights[i]->data[j],&fm=moment1[i]->data[j],&sm=moment2[i]->data[j];
            d/=(double)accum_count;
            if (lambda>0 && j<bias_start) {
                d+=w*lambda;
                w*=l;
            }
            if (adam) {
                fm=beta1*fm+(1.0-beta1)*d;
                sm=beta2*sm+(1.0-beta2)*d*d;
                w-=alpha*fm/(std::sqrt(sm)+c);
            } else w-=(fm=momentum*fm+lr*d);
        }
        gsl_matrix_set_zero(Deltas[i]);
    }
    accum_count=0;
}
/* Backward propagation of the expected output. */
void ann::propagate_backward() {
    calc_deltas();
    update_Deltas();
}
/* Initialize iterative learning. Set ITER to 0 and zero-out the moments. */
void ann::restart_iteration() {
    iter=0;
    int lc=layer_count()-1,i=0;
    for (;i<lc-1;++i) {
        gsl_matrix_set_zero(moment1[i]);
        gsl_matrix_set_zero(moment2[i]);
    }
}
/* Convert output to GSL matrix. */
void ann::output2gsl_matrix(const vecteur &output,int i0,int n,gsl_matrix *m) const {
    int i;
    gsl_matrix_set_zero(m);
    const_iterateur it,jt;
    bool ck;
    switch (this->task()) {
    case 0:
        if (ckmatrix(output))
            assert(matrice2gsl_matrix(output,i0,0,n,0,true,m,ctx)==GSL_SUCCESS);
        else {
            gsl_vector_view m1=gsl_matrix_subrow(m,0,0,n);
            assert(vecteur2gsl_vector(output.begin()+i0,output.begin()+i0+n,&m1.vector,ctx)==GSL_SUCCESS);
        }
        break;
    case 1:
        for (i=0,jt=output.begin()+i0;i<n;++i,++jt) {
            it=std::find(labels.begin(),labels.end(),*jt);
            if (it==labels.end())
                throw std::runtime_error(gettext("Output label not found"));
            if (output_size()==1)
                gsl_matrix_set(m,0,i,it-labels.begin());
            else gsl_matrix_set(m,it-labels.begin(),i,1.0);
        }
        break;
    case 2:
        for (i=0,jt=output.begin()+i0;i<n;++i,++jt) {
            const vecteur &samp=*it->_VECTptr;
            for (it=labels.begin();it!=labels.end();++it) {
                if (contains(samp,*it))
                    gsl_matrix_set(m,it-labels.begin(),i,1.0);
            }
        }
        break;
    }
}
/* Check input-output consistency. */
void ann::ckinput(const matrice &input,const vecteur &output,bool ignore_output) const {
    if (mcols(input)!=input_size())
        throw std::runtime_error(gettext("Invalid input size"));
    if (!is_numericm(input,num_mask_withfrac|num_mask_withint))
        throw std::runtime_error(gettext("Input should be numeric"));
    if (ignore_output) return;
    if (input.size()!=output.size())
        throw std::runtime_error(gettext("Number of samples does not match the number of outputs"));
    if (ckmatrix(output) && mcols(output)!=output_size())
        throw std::runtime_error(gettext("Invalid output dimension"));
    switch (this->task()) {
    case 0:
        if (!is_numericv(output,num_mask_withfrac|num_mask_withint))
            throw std::runtime_error(gettext("Output should be numeric"));
        if (!ckmatrix(output) && output_size()>1)
            throw std::runtime_error(gettext("Invalid output dimension"));
        break;
    case 1:
        break;
    case 2:
        if (!ckmatrix(output))
            throw std::runtime_error(gettext("Output should be a vector of labels"));
        break;
    default:
        throw std::runtime_error(gettext("Invalid network task"));
    }
}
/* Compute the gradient update for the given input and expected output.
 * After batch_size samples are passed forward (or all samples for size 0), update the weights.
 * Setting batch_size to a negative number disables updating the weights. */
void ann::train(const matrice &input,const vecteur &expected_output,int batch_size) {
    int nsamp=input.size(),i,j=0,jprev=0,sz=0,ep;
    if (batch_size>0 && batch_size>nsamp)
        throw std::runtime_error(gettext("Batch size too large"));
    ckinput(input,expected_output,false);
    bool update=false;
    gsl_matrix_view ov;
    while (j<nsamp) {
        j=std::min(j+block_size,nsamp);
        sz+=j-jprev;
        if ((batch_size>0 && sz>=batch_size) || (batch_size>=0 && j==nsamp)) {
            if (batch_size>0 && sz>batch_size)
                j-=sz-batch_size;
            update=true;
            sz=0;
        }
        if (j-jprev<block_size)
            gsl_matrix_set_zero(layers.front());
        assert(matrice2gsl_matrix(input,jprev,0,j-jprev,0,true,layers.front(),ctx)==GSL_SUCCESS);
        propagate_forward(true);
        output2gsl_matrix(expected_output,jprev,j-jprev,eout);
        propagate_backward();
        accum_count+=j-jprev;
        if (update) {
            update_weights();
            update=false;
        }
        jprev=j;
    }
}
/* Convert the output to gen. */
gen ann::gsl_vector2output(gsl_vector *output) const {
    vecteur sel;
    size_t i;
    const_iterateur it;
    switch (this->task()) {
    case 0: // regression
        return gsl_vector2vecteur(output);
    case 1: // classifier
        if (output_size()==1)
            return labels[gsl_vector_get(output,0)>=0.5?1:0];
        return labels[gsl_vector_max_index(output)];
    case 2: // labeller
        sel.reserve(labels.size());
        for (i=0,it=labels.begin();i<output->size;++i,++it) {
            if (gsl_vector_get(output,i)>=0.5)
                sel.push_back(*it);
        }
        return sel;
    default:
        break;
    }
    throw std::runtime_error(gettext("Invalid network task"));
}
/* Feed the input through the network.
 * If expected_output is NULL, return the output in res.
 * If expected_output is not NULL, compute error function values for the obtained
 * and expected output and store these values in res. */
void ann::feed(const matrice &input,vecteur &res,const vecteur &expected_output) const {
    int nsamp=input.size(),i,j=0,jprev=0;
    res.clear();
    if (nsamp==0) return;
    bool comp=!expected_output.empty();
    ckinput(input,expected_output,!comp);
    res.reserve(nsamp);
    gsl_vector_view ov,eov;
    while (j<nsamp) {
        j=std::min(j+block_size,nsamp);
        assert(matrice2gsl_matrix(input,jprev,0,j-jprev,0,true,layers.front(),ctx)==GSL_SUCCESS);
        if (j-jprev<block_size)
            gsl_matrix_set_zero(eout);
        propagate_forward(false);
        if (comp) output2gsl_matrix(expected_output,jprev,j-jprev,eout);
        for (i=0;i<j-jprev;++i) {
            ov=gsl_matrix_column(layers.back(),i);
            if (comp) {
                eov=gsl_matrix_column(eout,i);
                res.push_back(compute_errfunc(&eov.vector,&ov.vector));
            } else res.push_back(gsl_vector2output(&ov.vector));
        }
        jprev=j;
    }
}
/* Feed the argument forward, return the result. */
gen ann::operator()(const gen &g,GIAC_CONTEXT) const {
    if (g.type!=_VECT)
        return generrtype(gettext("Input should be a vector or a matrix"));
    vecteur res;
    try {
        if (g.subtype==_SEQ__VECT) {
            if (g._VECTptr->size()!=2)
                return generrdim(gettext("Expected 2 arguments"));
            const gen &inp=g._VECTptr->front(),&out=g._VECTptr->back();
            if (inp.type!=_VECT)
                return generrtype(gettext("Input should be a vector or a matrix"));
            if (ckmatrix(inp)) {
                if (out.type!=_VECT)
                    return generrtype(gettext("Expected output should be a vector"));
                feed(*inp._VECTptr,res,*out._VECTptr);
            } else feed(vecteur(1,inp),res,vecteur(1,out));
        } else if (ckmatrix(g))
            feed(*g._VECTptr,res);
        else feed(vecteur(1,g),res);
    } catch (const std::runtime_error &e) {
        *logptr(contextptr) << e.what() << "\n";
        return undef;
    }
    if (ckmatrix(res) && output_size()==1)
        res=*mtran(res).front()._VECTptr;
    return res.size()==1?res.front():res;
}
/* Convert weight matrices to a vector of giac matrices. */
void ann::weights2vecteur(vecteur &res) const {
    int i,lc=layer_count();
    res.clear();
    res.reserve(lc-1);
    gsl_matrix_view mv;
    for (i=0;i<lc-1;++i) {
        mv=gsl_matrix_submatrix(weights[i],0,0,weights[i]->size1,weights[i]->size2-(i+2==lc?0:1));
        res.push_back(gsl_matrix2matrice(&mv.matrix));
    }
}
/* Return the given hyperparameter value or the i-th layer (after feed-forward). */
gen ann::operator[](const gen &g) {
    int i,j,lc=layer_count();
    vecteur ret;
    gsl_matrix_view mv;
    if (!g.is_integer())
        return gentypeerr(ctx);
    i=g.val;
    if (g.subtype==_INT_MAPLECONVERSION) {
        switch (i) {
        case _ANN_LEARNING_RATE:
            if (schedule.is_symb_of_sommet(at_program))
                ret=makevecteur(string2gen("Scheduled",false),learning_rate,schedule);
            else return learning_rate;
            break;
        case _ANN_BLOCK_SIZE:
            return block_size;
        case _ANN_TOPOLOGY:
            ret=vector_int_2_vecteur(topology);
            break;
        case _GT_WEIGHTS:
            weights2vecteur(ret);
            break;
        case _ANN_WEIGHT_DECAY:
            ret=gsl_vector2vecteur(reg_coeff);
            break;
        case _ANN_MOMENTUM:
            if (momentum==1)
                ret=makevecteur(string2gen("Adam",false),beta1,beta2);
            else return momentum;
            break;
        default:
            return generr(gettext("Unknown hyperparameter"));
        }
    } else if (g.subtype==_INT_PLOT) {
        switch (i) {
        case _LABELS:
            ret=labels;
            break;
        case _TITLE:
            return string2gen(title,false);
        default:
            return generr(gettext("Unknown hyperparameter"));
        }
    } else {
        if (i<0 || i>=lc)
            return generrdim(gettext("Layer index out of range"));
        mv=gsl_matrix_submatrix(layers[i],0,0,layers[i]->size1,layers[i]->size2-(i+1==lc?0:1));
        ret=gsl_matrix2matrice(&mv.matrix,true);
    }
    return ret;
}
/* Initialize weights using the initializer f (a random variable). Bias weights are initialized to zero. */
void ann::randomize_initial_weights(const gen &f,const vecteur &fan_in_out) const {
    gen r;
    if (fan_in_out.empty())
        r=f;
    else if (fan_in_out.size()==1)
        r=subst(f,fan_in_out.front(),fan[0],false,ctx);
    else if (fan_in_out.size()==2)
        r=subst(f,fan_in_out,fan,false,ctx);
    else throw std::runtime_error(gettext("Invalid weights initializer specification"));
    int i=0;
    for (;i<layer_count()-1;++i) {
        gen fan_in=topology[i],fan_out=topology[i+1];
        gen args=makesequence(fan_in,fan_out,subst(r,fan,makevecteur(fan_in,fan_out),false,ctx));
        set_initial_weights(*(is_real_number(args._VECTptr->back(),ctx)?_matrix(args,ctx):_ranm(args,ctx))._VECTptr,i);
    }
}
/* Initialize weigths between i-th and (i+1)-th layer directly from a matrix. */
void ann::set_initial_weights(const matrice &w,int i) const {
    if (i<0||i+1>=layer_count())
        throw std::runtime_error(gettext("Invalid layer specification"));
    if (mrows(w)<topology[i] || mrows(w)>topology[i]+1 || mcols(w)!=topology[i+1])
        throw std::runtime_error(gettext("Invalid weight matrix dimensions"));
    if (matrice2gsl_matrix(w,0,0,0,0,false,weights[i],ctx)!=GSL_SUCCESS)
        throw std::runtime_error(gettext("Failed to initialize weights"));
}
/* Return the total number of neurons (including bias neurons). */
int ann::neuron_count() const {
    int s=0;
    for (vector<int>::const_iterator it=topology.begin();it!=topology.end();++it)
        s+=*it;
    return s+layer_count()-1;
}
/* Return an one-line description of this network. */
string ann::print (GIAC_CONTEXT) const {
    stringstream ss;
    if (title.size()>0)
        ss << title << ": ";
    ss << network_type() << " " << gettext("with input of size") << " " << topology.front();
    switch (task()) {
    case 1:
        ss << " " << gettext("and") << " ";
        if (output_size()==1)
            ss << 2;
        else ss << output_size();
        ss << " " << gettext("classes");
        break;
    case 2:
        ss << " " << gettext("and") << " " << output_size() << " " << gettext("labels");
        break;
    default:
        ss << " " << gettext("and") << " " << gettext("output of size") << " " << output_size();
        break;
    }
    return ss.str();
}
/* Return the LaTeX code containing the one-line description of this network. */
string ann::texprint(GIAC_CONTEXT) const {
    string ret="\\text{";
    ret+=print(contextptr);
    ret+="}";
    return ret;
}
/* Return the pointer to the neural network contained in G, or NULL if G is not a neural network. */
ann *ann::from_gen(const gen &g) {
    if (g.type!=_USER)
        return NULL;
    return dynamic_cast<ann*>(g._USERptr);
}
/* Return true iff the other network has the same parameters as this network. */
bool ann::operator ==(const gen &g) const {
    ann *other=from_gen(g);
    if (other==NULL)
        return false;
    if (this->topology!=other->topology || this->labels!=other->labels || this->activation!=other->activation ||
            this->output_activation!=other->output_activation || this->_is_classifier!=other->_is_classifier ||
            this->errfunc!=other->errfunc || this->learning_rate!=other->learning_rate || this->schedule!=other->schedule ||
            this->block_size!=other->block_size || this->momentum!=other->momentum ||
            (momentum==1 && (this->beta1!=other->beta1 || this->beta2!=other->beta2)))
        return false;
    for(int i=0;i<topology.size()-1;++i) {
        if (gsl_vector_get(this->reg_coeff,i)!=gsl_vector_get(other->reg_coeff,i))
            return false;
    }
    return true;
}
/* Implement the giac constructor. */
gen ann::giac_constructor(GIAC_CONTEXT) const {
    vecteur args;
    // topology
    args.push_back(vector_int_2_vecteur(topology));
    // hidden activation
    if (activation.front().is_integer() && activation.front().subtype==_INT_MAPLECONVERSION) {
        vecteur act;
        switch (activation.front().val) {
            case _ANN_LINEAR: act.push_back(at_id); break;
            case _ANN_SIGMOID: act.push_back(at_logistic); break;
            case _ANN_TANH: act.push_back(at_tanh); break;
            default: act.push_back(activation.front()); break;
        }
        act=mergevecteur(act,*activation.back()._VECTptr);
        if (!act.empty())
            args.push_back(symb_equal(change_subtype(_FUNC,_INT_TYPE),act.size()>1?act:act.front()));
    } else args.push_back(symb_equal(change_subtype(_FUNC,_INT_TYPE),activation.front()));
    // output activation
    if (!_is_classifier && output_activation.front().is_integer() && output_activation.front().subtype==_INT_MAPLECONVERSION) {
        vecteur act;
        switch (output_activation.front().val) {
            case _ANN_LINEAR: act.push_back(at_id); break;
            default: break;
        }
        act=mergevecteur(act,*output_activation.back()._VECTptr);
        if (!act.empty())
            args.push_back(symb_equal(at_output,act.size()>1?act:act.front()));
    } else args.push_back(symb_equal(at_output,output_activation.front()));
    // error function
    if (errfunc.front().is_integer() && errfunc.front().subtype==_INT_MAPLECONVERSION) {
        vecteur err(1,errfunc.front());
        err=mergevecteur(err,*errfunc.back()._VECTptr);
        args.push_back(symb_equal(at_erf,err.size()>1?err:err.front()));
    } else args.push_back(symb_equal(at_erf,errfunc.front()));
    // learning rate
    args.push_back(symb_equal(change_subtype(_ANN_LEARNING_RATE,_INT_MAPLECONVERSION),
                              schedule.is_symb_of_sommet(at_program)?makevecteur(learning_rate,schedule):gen(learning_rate)));
    // batch size
    args.push_back(symb_equal(change_subtype(_ANN_BLOCK_SIZE,_INT_MAPLECONVERSION),block_size));
    // weight decay parameters
    args.push_back(symb_equal(change_subtype(_ANN_WEIGHT_DECAY,_INT_MAPLECONVERSION),gsl_vector2vecteur(reg_coeff)));
    // momentum
    args.push_back(symb_equal(change_subtype(_ANN_MOMENTUM,_INT_MAPLECONVERSION),momentum==1?makevecteur(beta1,beta2):momentum));
    // title
    if (title.size()>0)
        args.push_back(symb_equal(change_subtype(_TITLE,_INT_PLOT),string2gen(title,false)));
    // labels
    if (!labels.empty())
        args.push_back(symb_equal(_is_classifier?at_classes:change_subtype(_LABELS,_INT_PLOT),labels));
    // weights
    vecteur w;
    weights2vecteur(w);
    args.push_back(symb_equal(change_subtype(_GT_WEIGHTS,_INT_MAPLECONVERSION),w));
    // finalize
    return symbolic(at_neural_network,change_subtype(args,_SEQ__VECT));
}
/*
 * END OF ANN CLASS IMPLEMENTATION
 */

/* Compute wavelet transform of DATA with N samples, changing DATA in place.
 * DIR is 1 for forward transform and -1 for inverse transform. */
int dwt(void *data,int datatype,int n,int wtype,int k,int dir) {
    const gsl_wavelet_type *wt;
    switch (wtype) {
    case _DAUBECHIES_WAVELET:
        wt=gsl_wavelet_daubechies;
        break;
    case _DAUBECHIES_WAVELET_CENTERED:
        wt=gsl_wavelet_daubechies_centered;
        break;
    case _HAAR_WAVELET:
        wt=gsl_wavelet_haar;
        break;
    case _HAAR_WAVELET_CENTERED:
        wt=gsl_wavelet_haar_centered;
        break;
    case _BSPLINE_WAVELET:
        wt=gsl_wavelet_bspline;
        break;
    case _BSPLINE_WAVELET_CENTERED:
        wt=gsl_wavelet_bspline_centered;
        break;
    default:
        return 1; // unsupported wavelet type
    }
    gsl_wavelet *w=gsl_wavelet_alloc(wt,k);
    if (w==NULL)
        return 2; // memory allocation failed
    gsl_wavelet_workspace *work=gsl_wavelet_workspace_alloc(n);
    if (work==NULL) {
        gsl_wavelet_free(w);
        return 2;
    }
    bool ret=0;
    int res;
    switch (datatype) {
    case 1: // one-dimensional transform
        res=(dir>0)?gsl_wavelet_transform_forward(w,(double*)data,1,n,work)
                   :gsl_wavelet_transform_inverse(w,(double*)data,1,n,work);
        break;
    case 2: // two-dimensional transform
        res=(dir>0)?gsl_wavelet2d_transform_matrix_forward(w,static_cast<gsl_matrix*>(data),work)
                   :gsl_wavelet2d_transform_matrix_inverse(w,static_cast<gsl_matrix*>(data),work);
        break;
    case 3: // "non-standard" two-dimensional transform
        res=(dir>0)?gsl_wavelet2d_nstransform_matrix_forward(w,static_cast<gsl_matrix*>(data),work)
                   :gsl_wavelet2d_nstransform_matrix_inverse(w,static_cast<gsl_matrix*>(data),work);
        break;
    default:
        assert(false);
    }
    if (res!=GSL_SUCCESS)
        ret=3; // DWT computation failed
    gsl_wavelet_free(w);
    gsl_wavelet_workspace_free(work);
    return ret;
}
#else
bool dwt(double *data,int n,int wtype,int k,int dir) {
    return false;
}
#endif // HAVE_LIBGSL

gen _neural_network(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
#ifdef HAVE_LIBGSL
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    vecteur topology,act,out_act,err,lab,winit,reg;
    vector<matrice*> init_weights;
    gen rate(0.001),schedule(1),mom(0.0),fan_in=identificateur(" fan_in"),fan_out=identificateur(" fan_out");
    string name;
    bool cls=false,set_rate=false;
    int i,bs=1;
    ann *src=NULL;
    if (g.subtype==_SEQ__VECT) {
        const vecteur &gv=*g._VECTptr;
        if (gv.empty())
            return generrdim(gettext("Sequence of arguments is empty"));
        if ((src=ann::from_gen(gv.front()))==NULL) {
            if (gv.front().type!=_VECT)
                return generrtype(gettext("Invalid topology specification"));
            topology=*gv.front()._VECTptr;
        }
        // parse options
        for (const_iterateur it=gv.begin()+1;it!=gv.end();++it) {
            if (it->is_symb_of_sommet(at_equal)) {
                const gen &prop=it->_SYMBptr->feuille._VECTptr->front();
                const gen &val=it->_SYMBptr->feuille._VECTptr->back();
                if (prop==at_output) {
                    out_act=val.type==_VECT?*val._VECTptr:vecteur(1,val);
                    if (out_act.empty())
                        return generr(gettext("Invalid output activation function specification"));
                    vecteur params(out_act.begin()+1,out_act.end());
                    out_act=makevecteur(out_act.front(),params);
                } else if (prop==at_classes) {
                    if (val.type!=_VECT)
                        return generrtype(gettext("Expected a list of classes"));
                    lab=*val._VECTptr;
                    cls=true;
                } else if (prop==at_erf) {
                    err=val.type==_VECT?*val._VECTptr:vecteur(1,val);
                    if (err.empty())
                        return generr(gettext("Invalid error function specification"));
                    vecteur params(err.begin()+1,err.end());
                    err=makevecteur(err.front(),params);
                } else if (prop.is_integer() && prop.subtype==_INT_MAPLECONVERSION) {
                    switch (prop.val) {
                    case _GT_WEIGHTS:
                        if (val.type==_VECT) {
                            if (!val._VECTptr->empty()) {
                                if (ckmatrix(val._VECTptr->front())) {
                                    if (val._VECTptr->size()!=topology.size()-1)
                                        return generrdim(gettext("Invalid number of initial weight matrices"));
                                    init_weights.reserve(val._VECTptr->size());
                                    for (const_iterateur wt=val._VECTptr->begin();wt!=val._VECTptr->end();++wt) {
                                        if (wt!=val._VECTptr->begin() && !ckmatrix(*wt))
                                            return generrtype(gettext("Expected an initial weight matrix"));
                                        init_weights.push_back(wt->_VECTptr);
                                    }
                                } else winit=makevecteur(val._VECTptr->front(),vecteur(val._VECTptr->begin()+1,val._VECTptr->end()));
                            }
                        } else winit=makevecteur(val,vecteur(0));
                        break;
                    case _ANN_LEARNING_RATE:
                        if (val.type==_VECT) {
                            if (val._VECTptr->size()!=2)
                                return generr(gettext("Invalid scheduled learning rate specification"));
                            if (!val._VECTptr->back().is_symb_of_sommet(at_program) ||
                                    val._VECTptr->back()._SYMBptr->feuille._VECTptr->front()._VECTptr->size()!=1)
                                return generr(gettext("Schedule multiplier should be an univariate function"));
                            rate=val._VECTptr->front();
                            schedule=val._VECTptr->back();
                        } else rate=val;
                        if (!is_real_number(rate,contextptr) || !is_strictly_positive(rate=to_real_number(rate,contextptr),contextptr))
                            return generr(gettext("(Initial) learning rate should be a positive real number"));
                        set_rate=true;
                        break;
                    case _ANN_BLOCK_SIZE:
                        if (!val.is_integer() || (bs=val.val)<1)
                            return generr(gettext("Block size should be a positive integer"));
                        break;
                    case _ANN_MOMENTUM:
                        if (val.is_integer() && val.subtype==_INT_PLOT && val.val==_ADAPTIVE) {
                            mom=1;
                        } else if (val.type==_VECT) {
                            if (val._VECTptr->size()!=2 || !is_numericv(*val._VECTptr,num_mask_withfrac) ||
                                    is_positive(-_min(val,contextptr),contextptr) || is_greater(_max(val,contextptr),1,contextptr))
                                return generr(gettext("Invalid specification of adaptive momentum parameters"));
                            mom=_evalf(val,contextptr);
                        } else if (is_real_number(val,contextptr)) {
                            mom=to_real_number(val,contextptr);
                            if (!is_positive(mom,contextptr) || is_greater(mom,1,contextptr))
                                return generr(gettext("Momentum parameter should be a real number in [0,1)"));
                        }
                        break;
                    case _ANN_WEIGHT_DECAY:
                        reg=val.type==_VECT?*val._VECTptr:vecteur(1,val);
                        if (!is_numericv(reg,num_mask_withfrac|num_mask_withint) || is_strictly_positive(-_min(reg,contextptr),contextptr))
                            return generr(gettext("Invalid weight decay parameter(s) specification"));
                        break;
                    default:
                        return generr(gettext("Invalid property specification"));
                    }
                } else if (prop.is_integer() && prop.subtype==_INT_TYPE && prop.val==_FUNC) {
                    act=val.type==_VECT?*val._VECTptr:vecteur(1,val);
                    if (act.empty())
                        return generr(gettext("Invalid activation function specification"));
                    vecteur params(act.begin()+1,act.end());
                    act=makevecteur(act.front(),params);
                } else if (prop.is_integer() && prop.subtype==_INT_PLOT) {
                    if (prop.val==_LABELS) {
                        if (val.type!=_VECT)
                            return generrtype(gettext("Expected a list of labels"));
                        lab=*val._VECTptr;
                    } else if (prop.val==_TITLE) {
                        if (val.type!=_STRNG)
                            return generrtype(gettext("Title should be a string"));
                        name=*val._STRNGptr;
                    } else return generr(gettext("Invalid property specification"));
                } else return generr(gettext("Invalid property specification"));
            } else return generrtype(gettext("Invalid optional argument"));
        }
    } else topology=*g._VECTptr;
    vector<int> t;
    if (src==NULL) {
        if (topology.size()<2)
            return generrdim(gettext("Too few layers in the network"));
        if (!is_integer_vecteur(topology))
            return generrtype(gettext("Invalid layer size type"));
        if (is_strictly_greater(1,_min(topology,contextptr),contextptr))
            return gensizeerr("Invalid layer size, should be at least 1");
        if (topology.size()==2) { // guess the size of the hidden layer: (input+output)*2/3
            gen hsize=_round(2*_sum(topology,contextptr)/3,contextptr);
            *logptr(contextptr) << gettext("No hidden layers specified, creating 1 hidden layer of size") << " " << hsize << "\n";
            topology.insert(topology.begin()+1,hsize);
        }
        vecteur2vector_int(topology,0,t);
    }
    if (reg.size()==1) {
        gen r=reg.front();
        reg=vecteur((src==NULL?topology.size():src->layer_count())-1,r);
    }
    gen glorot=inv(sqrt(fan_in+fan_out,contextptr),contextptr),lecun=inv(sqrt(fan_in,contextptr),contextptr);
    vecteur fan=makevecteur(fan_in,fan_out);
    vecteur glorot_uniform=makevecteur(symbolic(at_uniformd,makesequence(minus_sqrt6*glorot,plus_sqrt6*glorot)),fan);
    vecteur glorot_normal=makevecteur(symbolic(at_normald,makesequence(0,plus_sqrt2*glorot)),fan);
    vecteur he_uniform=makevecteur(symbolic(at_uniformd,makesequence(minus_sqrt6*lecun,plus_sqrt6*lecun)),fan);
    vecteur he_normal=makevecteur(symbolic(at_normald,makesequence(0,plus_sqrt2*lecun)),fan);
    vecteur lecun_uniform=makevecteur(symbolic(at_uniformd,makesequence(minus_sqrt3*lecun,plus_sqrt3*lecun)),fan);
    vecteur lecun_normal=makevecteur(symbolic(at_normald,makesequence(0,lecun)),fan);
    if (src==NULL && winit.empty() && init_weights.empty()) // set default initialization
        winit=makevecteur(symbolic(at_uniformd,makesequence(-lecun,lecun)),makevecteur(fan_in,fan_out));
    try {
        log_output_redirect lor(contextptr);
        ann network=src==NULL?ann(t,bs,contextptr):ann(*src,bs);
        if (src==NULL || set_rate)
            network.set_learning_rate(rate.to_double(contextptr),schedule);
        if (mom.type==_VECT) {
            network.set_momentum(1);
            network.set_adam_params(mom._VECTptr->front().to_double(contextptr),mom._VECTptr->back().to_double(contextptr));
        } else if (src==NULL) network.set_momentum(mom.to_double(contextptr));
        if (name.length()>0)
            network.set_name(name);
        if (!reg.empty())
            network.set_regularization(reg);
        // set labels
        if (!lab.empty())
            network.set_labels(lab,cls);
        int task=network.task();
        if (task<0)
            return generr(gettext("Bad output labels"));
        // set activation function for hidden layers
        if (!act.empty())
            network.set_activation(false,act.front(),*act.back()._VECTptr);
        else if (src==NULL) switch (task) {
        case 0:
            network.set_activation(false,change_subtype(_ANN_RELU,_INT_MAPLECONVERSION));
            break;
        case 1:
        case 2:
            network.set_activation(false,at_tanh);
            break;
        default: assert(false);
        }
        // set activation function for the output layer
        if (!out_act.empty()) {
            if (cls)
                *logptr(contextptr) << gettext("Warning") << ": "
                                    << gettext("'output' option is ignored in classifier networks") << "\n";
            network.set_activation(true,out_act.front(),*out_act.back()._VECTptr);
        } else if (src==NULL) switch (task) {
        case 0:
            network.set_activation(true,at_id);
            break;
        case 1:
            if (network.output_size()==1)
                network.set_activation(true,at_logistic);
            else ; // softmax will be used
            break;
        case 2:
            network.set_activation(true,at_logistic);
            break;
        default: assert(false);
        }
        // set error function
        if (!err.empty())
            network.set_error_function(err.front(),*err.back()._VECTptr);
        else if (src==NULL) switch (task) {
        case 0:
            network.set_error_function(change_subtype(_ANN_HALF_MSE,_INT_MAPLECONVERSION));
            break;
        case 1:
            if (network.output_size()>1)
                network.set_error_function(change_subtype(_ANN_CROSS_ENTROPY,_INT_MAPLECONVERSION));
            else network.set_error_function(change_subtype(_ANN_LOG_LOSS,_INT_MAPLECONVERSION));
            break;
        case 2:
            network.set_error_function(change_subtype(_ANN_LOG_LOSS,_INT_MAPLECONVERSION));
            break;
        default: assert(false);
        }
        // initialize weights
        if (!init_weights.empty()) {
            i=0;
            for (vector<matrice*>::const_iterator it=init_weights.begin();it!=init_weights.end();++it,++i)
                network.set_initial_weights(*(*it),i);
        } else if (!winit.empty()) {
            if (winit.front().type==_STRNG) {
                if (!winit.back()._VECTptr->empty())
                    *logptr(contextptr) << gettext("Warning") << ": " << gettext("ignoring parameters for weight initializer") << "\n";
                const string &str=*winit.front()._STRNGptr;
                if (str=="glorot-uniform")
                    winit=glorot_uniform;
                else if (str=="he-uniform")
                    winit=he_uniform;
                else if (str=="lecun-uniform")
                    winit=lecun_uniform;
                else if (str=="glorot-normal")
                    winit=glorot_normal;
                else if (str=="he-normal")
                    winit=he_normal;
                else if (str=="lecun-normal")
                    winit=lecun_normal;
                else return generr(gettext("Unknown weight initializer"));
            }
            network.randomize_initial_weights(winit.front(),*winit.back()._VECTptr);
        }
        return network;
    } catch (const std::runtime_error &e) {
        return generr(e.what());
    }
#else
    return generr(gettext("GNU Scientific Library is required for neural networks"));
#endif // HAVE_LIBGSL
}
static const char _neural_network_s []="neural_network";
static define_unary_function_eval (__neural_network,&_neural_network,_neural_network_s);
define_unary_function_ptr5(at_neural_network,alias_at_neural_network,&__neural_network,0,true)

gen _train(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
#ifdef HAVE_LIBGSL
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    const vecteur &gv=*g._VECTptr;
    if (gv.size()<3)
        return generrdim(gettext("Expected 3 or more arguments"));
    ann *net=ann::from_gen(gv.front());
    if (net==NULL)
        return generrtype(gettext("First argument should be a neural network"));
    const gen &inp=gv[1],&out=gv[2];
    if (!ckmatrix(inp) || out.type!=_VECT)
        return generrtype(gettext("Input and output should be a matrix and a vector"));
    int batch_size=0;
    if (gv.size()>3 && (!gv[3].is_integer() || (batch_size=gv[3].val)>(int)inp._VECTptr->size()))
        return generr(gettext("Invalid batch size specification"));
    try {
        net->train(*inp._VECTptr,*out._VECTptr,batch_size);
    } catch (const std::runtime_error &e) {
        return generr(e.what());
    }
    return *net;
#else
    return generr(gettext("GNU Scientific Library is required for neural networks"));
#endif // HAVE_LIBGSL
}
static const char _train_s []="train";
static define_unary_function_eval (__train,&_train,_train_s);
define_unary_function_ptr5(at_train,alias_at_train,&__train,0,true)

//
// RGBA IMAGE CLASS IMPLEMENTATION
//
static int cached_rgba_image_counter=0;
string make_image_name() {
    return ":/"+print_INT_(++cached_rgba_image_counter);
}
gen (*load_image_ptr)(const char *fname,GIAC_CONTEXT)=0;
gen (*resize_image_ptr)(const rgba_image &img,int w,int h,GIAC_CONTEXT)=0;
void (*cache_rgba_image_ptr)(const rgba_image &img)=0;
void (*uncache_rgba_image_ptr)(const rgba_image &img)=0;
#if 0
tempimagefilename::~tempimagefilename() {
    while (!_fnames.empty()) {
        remove(_fnames.top().c_str());
        _fnames.pop();
    }
}
const string &tempimagefilename::make() {
    _fnames.push(temp_file_name("casimage","png"));
    return _fnames.top();
}
tempimagefilename temp_image_file_name;
string make_unique_image_filename() {
    string ret=temp_image_file_name.make();
    temp_image_file_name.pop();
    return ret;
}
#endif
rgba_image::rgba_image(int d,int w,int h,GIAC_CONTEXT) {
    ctx=contextptr;
    _d=d;
    _w=w;
    _h=h;
    allocate();
    _filename=make_image_name();
    _tmp=true;
}
rgba_image::rgba_image(const std::vector<const gen*> &data,GIAC_CONTEXT) {
    ctx=contextptr;
    const matrice* chndata[4];
    int i,j,k;
    _d=data.size();
    if (_d<1 || _d>4)
        throw std::runtime_error(gettext("Invalid number of channels"));
    _w=_h=0;
    matrice alpha;
    for (int i=0;i<_d;++i) {
        if (data[i]==NULL) {
            alpha=*_matrix(makesequence(_h,_w,255),contextptr)._VECTptr;
            chndata[i]=&alpha;
        } else {
            if (!ckmatrix(*data[i]))
                throw std::runtime_error(gettext("Channel data should be a matrix"));
            const matrice &c=*data[i]->_VECTptr;
            if (_w && _h && (_w!=mcols(c) || _h!=mrows(c)))
                throw std::runtime_error(gettext("Incompatible dimensions of channel data matrix"));
            else if (_w==0 && _h==0) {
                _w=mcols(c);
                _h=mrows(c);
            }
            if (!is_integer_matrice(c))
                throw std::runtime_error(gettext("Data values should be integers"));
            chndata[i]=&c;
        }
    }
    allocate();
    _filename=make_image_name();
    _tmp=true;
    const_iterateur it,jt;
    for (i=0;i<_d;++i)
        for (k=0,it=chndata[i]->begin();k<_h;++k,++it)
            for (j=0,jt=it->_VECTptr->begin();j<_w;++j,++jt)
                set_pixel(i,j,k,static_cast<uchar>(jt->to_int()));
}
rgba_image::rgba_image(int d,int w,const string &data,GIAC_CONTEXT) {
    ctx=contextptr;
    int sz=data.size()/2,j=0;
    _d=d;
    _w=w;
    _h=sz/(_w*_d);
    if (_d<1 || _d>4 || _w<=0 || _h<=0)
        throw std::runtime_error(gettext("Invalid image parameters"));
    allocate();
    _filename=make_image_name();
    _tmp=true;
    for (int i=0;i<sz;++i)
        _data[j++]=static_cast<uchar>(strtol(data.substr(2*i,2).c_str(),NULL,16));
}
void rgba_image::set_file_name(const char *fname) {
    _filename=std::string(fname);
    _tmp=false;
}
/* Return the color type description. */
string rgba_image::color_type_string() const {
    switch (_d) {
        case 0: case 1: return gettext("grayscale");
        case 2: return gettext("grayscale with alpha channel");
        case 3: return gettext("RGB");
        case 4: return gettext("RGBA");
        default: break;
    }
    return "";
}
bool rgba_image::assure_on_disk() {
    if (is_original())
        return true;
#if 1
    if (cache_rgba_image_ptr) {
        if (_filename.empty()) {
            _filename=make_image_name();
            _tmp=true;
        } else (*uncache_rgba_image_ptr)(*this);
        (*cache_rgba_image_ptr)(*this);
        return true;
    }
#else
    if (_filename.empty()) {
        _filename=make_unique_image_filename();
        active_rgba_filenames[_filename]++;
        _tmp=true;
    } else if (ckfileexists(_filename.c_str()))
        return true;
    if (write_png(_filename.c_str())==0)
        return true;
#endif
    return false;
}
string rgba_image::print(GIAC_CONTEXT) const {
    stringstream ss;
    ss << gettext("an image of size") << " " << _w << "×" << _h << " (";
    string chd=color_type_string();
    if (chd.empty())
        ss << _d << " " << gettext("channels");
    else ss << chd;
    ss << ")";
    return ss.str();
}
rgba_image::rgba_image(const rgba_image &other,int x,int y,int w,int h) {
    _data=NULL;
    assign(other,x,y,w,h);
}
void rgba_image::clear() {
    if (_data!=NULL)
        free(_data);
}
void rgba_image::assign(const rgba_image &other,int x,int y,int w,int h) {
    clear();
    ctx=other.ctx;
    _filename=other._filename;
    _tmp=other._tmp;
    _d=other._d;
    _w=w==0?other._w:std::min(other._w-x,w);
    _h=h==0?other._h:std::min(other._h-y,h);
    allocate();
    if (x || y || w || h) {
        _filename.clear();
        for (int i=0;i<_w;++i)
            for (int j=0;j<_h;++j)
                for (int k=0;k<_d;++k)
                    set_pixel(k,i,j,other.get_pixel(k,i+x,j+y));
    } else memcpy(_data,other._data,_w*_d*_h*sizeof(uchar));
}
string rgba_image::texprint(GIAC_CONTEXT) const {
    return string("\\text{")+print(contextptr)+"}";
}
rgba_image *rgba_image::from_gen(const gen &g) {
    if (g.type!=_USER)
        return NULL;
    return dynamic_cast<rgba_image*>(g._USERptr);
}
bool rgba_image::operator==(const gen &g) const {
    rgba_image *img=rgba_image::from_gen(g);
    if (img==NULL || _d!=img->_d || _w!=img->_w || _h!=img->_h)
        return false;
    int sz=_w*_d*_h;
    for (int i=0;i<sz;++i) {
        if (_data[i]!=img->_data[i])
            return false;
    }
    return true;
}
void rgba_image::allocate() {
    _data=(uchar*)calloc(_w*_h*_d,sizeof(uchar));
    if (_data==NULL)
        throw std::runtime_error(gettext("Failed to allocate memory for image"));
}
rgba_image::~rgba_image() {
    clear();
}
uchar rgba_image::get_pixel(int chn,int x,int y) const {
    if (x<0 || x>=_w || y<=0 || y>=_h || chn>=_d)
        return 0;
    if (chn>=0)
        return _data[y*_w*_d+x*_d+chn];
    unsigned int val=0;
    int imax=(_d%2)?_d:_d-1;
    for (int i=0;i<imax;++i)
        val+=_data[y*_w*_d+x*_d+i];
    return (uchar)(val/imax);
}
void rgba_image::set_pixel(int chn,int x,int y,uchar v) {
    if (x>=0 && x<_w && y>=0 && y<_h && chn>=0 && chn<_d)
        _data[y*_w*_d+x*_d+chn]=v;
}
/* crop or resize the image, return a modified copy */
gen rgba_image::operator()(const gen &g,GIAC_CONTEXT) const {
    int x=0,y=0,w,h,task=0;
    if (g.is_integer() && g.subtype==_INT_COLOR && g.val==GREY_COLOR)
        return to_grayscale();
    if (evalf_double(g,1,contextptr).type==_DOUBLE_) {
        double s=evalf_double(g,1,contextptr).to_double(contextptr);
        if (s<=0)
            return generr(gettext("Expected a positive real number"));
        w=(int)std::floor(s*_w+0.5);
        h=(int)std::floor(s*_h+0.5);
        task=1;
    } else {
        if (g.type!=_VECT || g.subtype!=_SEQ__VECT || !is_integer_vecteur(*g._VECTptr))
            return gentypeerr(contextptr);
        const vecteur &args=*g._VECTptr;
        switch (args.size()) {
        case 2: // resize
            w=args.front().val;
            h=args.back().val;
            task=1;
            break;
        case 4: // crop
            x=args[0].val;
            y=args[1].val;
            w=args[2].val;
            h=args[3].val;
            task=2;
            break;
        default:
            return gendimerr(contextptr);
        }
    }
    if (w<=0 || h<=0)
        return gendimerr(contextptr);
    if (x>=_w || y>=_h || x<0 || y<0)
        return gensizeerr(contextptr);
    assert(task>0);
    if (task==1) {
        if (resize_image_ptr)
            return (*resize_image_ptr)(*this,w,h,ctx);
        return generr(gettext("FLTK is required for resizing the image"));
    } else if (task==2) {
        w=std::min(w,_w-x);
        h=std::min(h,_h-y);
        try {
            return rgba_image(*this,x,y,w,h);
        } catch (const std::runtime_error &e) {
            return gensizeerr(e.what());
        }
    }
    return undef; // unreachable
}
gen rgba_image::operator[](const gen &g) {
    if (g.type!=_VECT)
        return (*this)[gen(vecteur(1,g),_SEQ__VECT)];
    if (g.subtype!=_SEQ__VECT)
        return gentypeerr(ctx);
    const vecteur &args=*g._VECTptr;
    if (args.empty())
        return gendimerr(ctx);
    int x,y,w=0,h=0,task=0,chn=0,start=1;
    if (args.front()==at_tran) {
        if (_d==1 || _d==3)
            return gensizeerr(ctx);
        chn=_d-1;
    } else if (args.front().is_integer() && args.front().subtype==_INT_COLOR) {
        switch (args.front().val) {
        case _RED: chn=0; break;
        case _GREEN: chn=1; break;
        case _BLUE: chn=2; break;
        case GREY_COLOR: chn=-1; break; // gray
        default: return gensizeerr(ctx);
        }
    } else start=0;
    if (chn>=_d)
        return generr(gettext("Invalid channel number"));
    if (chn<0 && _d<=2)
        chn=0;
    if (args.size()==1)
        return get_channel_data(chn);
    if (args.size()<2+start || args.size()>4+start)
        return gendimerr(ctx);
    if (!is_integer_vecteur(vecteur(g._VECTptr->begin()+start,g._VECTptr->end())))
        return gentypeerr(ctx);
    x=args[start].val;
    y=args[1+start].val;
    switch (args.size()-start) {
    case 2:
        task=1;
        break;
    case 4:
        w=args[2+start].val;
        h=args[3+start].val;
        task=2;
        break;
    default:
        return gendimerr(ctx);
    }
    if (x<0 || y<0 || x>=_w || y>=_h)
        return gensizeerr(ctx);
    if (task==1) {
        return get_pixel(chn,x,y);
    } else if (task==2) {
        w=std::min(w,_w-x);
        h=std::min(h,_h-y);
        if (w<=0 || h<=0)
            return gendimerr(ctx);
        return get_channel_data(chn,x,y,w,h);
    }
    return undef; // unreachable
}
matrice rgba_image::get_channel_data(int chn,int x,int y,int w,int h) const {
    int width=w==0?_w-x:std::min(w,_w-x),height=h==0?_h-y:std::min(h,_h-y);
    if (width<=0 || height<=0)
        throw std::runtime_error(gettext("Invalid slice dimensions"));
    matrice ret=*_matrix(makesequence(height,width,0),ctx)._VECTptr;
    iterateur it,jt;
    int i,j;
    uchar v;
    for (i=0,it=ret.begin();i<height;++i,++it) {
        for (j=0,jt=it->_VECTptr->begin();j<width;++j,++jt) {
            v=get_pixel(chn,j+x,i+y);
            if (v)
                *jt=v;
        }
    }
    return ret;
}
void rgba_image::set_channel_data(int chn,const matrice &data,int x,int y) {
    const_iterateur it=data.begin(),itend=data.end(),jt,jtend;
    int i,j;
    gen v;
    for (i=0;it!=itend;++it,++i) {
        for (j=0,jt=it->_VECTptr->begin(),jtend=it->_VECTptr->end();jt!=jtend;++j,++jt) {
            if (!jt->is_integer())
                throw std::runtime_error(gettext("Invalid pixel data"));
            set_pixel(chn,j+x,i+y,static_cast<uchar>(jt->val));
        }
    }
    _filename.clear();
}
#ifdef HAVE_LIBPNG
int rgba_image::write_png(const char *fname) const {
    png_bytep *rows=new png_bytep[_h];
    for (int i=0;i<_h;++i)
        rows[i]=_data+i*_w*_d;
    png_structp png_ptr;
    png_infop info_ptr;
    int retval=0;
    FILE *fp=fopen(fname,"wb");
    if (!fp) {
        retval=2; goto done; // could not open file for writing
    }
    if (!(png_ptr=png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL, NULL))) {
        retval=3; goto done; // could not create png write struct
    }
    if (!(info_ptr=png_create_info_struct(png_ptr)) || setjmp(png_jmpbuf(png_ptr))) {
        retval=4; goto done; // could not create png info struct
    }
    png_init_io(png_ptr,fp);
    int colortype;
    switch (_d) {
    case 1: colortype=PNG_COLOR_TYPE_GRAY; break;
    case 2: colortype=PNG_COLOR_TYPE_GA; break;
    case 3: colortype=PNG_COLOR_TYPE_RGB; break;
    case 4: colortype=PNG_COLOR_TYPE_RGBA; break;
    default: retval=5; goto done; // could not determine color type (should be unreachable)
    }
    png_set_IHDR(png_ptr,info_ptr,_w,_h,8,colortype, 
                PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_BASE,PNG_FILTER_TYPE_BASE);
    png_write_info(png_ptr,info_ptr);
    png_write_image(png_ptr,rows);
    png_write_end(png_ptr,NULL);
    fclose(fp);
done:
    delete[] rows;
    return retval;
}
#else
int rgba_image::write_png(const char *fname) const {
    return 1; // no png export
}
#endif // LIBPNG
gen rgba_image::giac_constructor(GIAC_CONTEXT) const {
    if (is_original())
        return symbolic(at_image,string2gen(_filename,false));
    string data;
    int sz=_w*_h*_d;
    data.reserve(sz*2);
    for (int i=0;i<sz;++i)
        data.append(n2hexstr(_data[i]));
    return symbolic(at_image,makesequence(_d,_w,string2gen(data,false)));
}
rgba_image rgba_image::to_grayscale() const {
    bool ha=has_alpha();
    rgba_image ret(ha?2:1,_w,_h,ctx);
    for (int i=0;i<_w;++i) {
        for (int j=0;j<_h;++j) {
            ret.set_pixel(0,i,j,get_pixel(-1,i,j));
            if (ha) ret.set_pixel(1,i,j,get_pixel(_d-1,i,j));
        }
    }
    return ret;
}
rgba_image rgba_image::blend(const rgba_image &other,double t) const {
    const rgba_image *img1=this,*img2=&other,*tmp;
    int w,h,i,j;
    gen resized1,resized2;
    if (img1->depth()>img2->depth()) {
        tmp=img1; img1=img2; img2=tmp;
        t=1.-t;
    }
    if (img1->width()!=img2->width() || img1->height()!=img2->height()) {
        // resize each image to an image with geometric mean aspect ratio and average area
        if (!resize_image_ptr)
            throw std::runtime_error(gettext("FLTK is required for image resizing"));
        double w1=img1->width(),h1=img1->height(),w2=img2->width(),h2=img2->height();
        double p1=w1*h1,p2=w2*h2,ar1=w1/h1,ar2=w2/h2,ar=std::pow(ar1,1.-t)*std::pow(ar2,t),p=(1.-t)*p1+t*p2;
        w=(int)std::floor(std::sqrt(ar*p)+.5);
        h=(int)std::floor(std::sqrt(p/ar)+.5);
        resized1=(*resize_image_ptr)(*img1,w,h,ctx);
        resized2=(*resize_image_ptr)(*img2,w,h,ctx);
        img1=from_gen(resized1); img2=from_gen(resized2);
        if (img1==NULL || img2==NULL)
            throw std::runtime_error(gettext("Failed to resize images"));
    } else {
        w=width();
        h=height();
    }
    int d1=img1->depth(),d2=img2->depth(),d; 
    if (d1==d2) d=d1;
    else if (d1%2) d=d2;
    else d=4;
    rgba_image ret(d,w,h,ctx);
    uchar r1,g1,b1,a1,r2,g2,b2,a2,r,g,b,a;
    for (i=0;i<w;++i) {
        for (j=0;j<h;++j) {
            r1=img1->get_pixel(0,i,j);
            g1=d1<3?r1:img1->get_pixel(1,i,j);
            b1=d1<3?r1:img1->get_pixel(2,i,j);
            a1=d1%2?255:img1->get_pixel(d1-1,i,j);
            r2=img2->get_pixel(0,i,j);
            g2=d2<3?r2:img2->get_pixel(1,i,j);
            b2=d2<3?r2:img2->get_pixel(2,i,j);
            a2=d2%2?255:img2->get_pixel(d2-1,i,j);
            giac::blend(r1,g1,b1,r2,g2,b2,t,r,g,b);
            if (d<3) r=(r+g+b)/3;
            ret.set_pixel(0,i,j,r);
            if (d>2) {
                ret.set_pixel(1,i,j,g);
                ret.set_pixel(2,i,j,b);
            }
            if (d%2==0)
                ret.set_pixel(d-1,i,j,static_cast<uchar>((int)std::floor(a1*(1.-t)+a2*t+.5)));
        }
    }
    return ret;
}
rgba_image rgba_image::blend(int color,double t) const {
    uchar r,g,b,ri,gi,bi,rr,gg,bb;
    if (!index2rgb(color,r,g,b))
        throw std::runtime_error(gettext("Not a valid FLTK color"));
    int w=width(),h=height(),d=depth(),i,j;
    rgba_image ret(d%2?3:4,w,h,ctx);
    for (i=0;i<w;++i) {
        for (j=0;j<h;++j) {
            ri=get_pixel(0,i,j);
            gi=d<3?ri:get_pixel(1,i,j);
            bi=d<3?ri:get_pixel(2,i,j);
            giac::blend(r,g,b,ri,gi,bi,t,rr,gg,bb);
            ret.set_pixel(0,i,j,rr);
            ret.set_pixel(1,i,j,gg);
            ret.set_pixel(2,i,j,bb);
            if (d%2==0)
                ret.set_pixel(3,i,j,get_pixel(3,i,j));
        }
    }
    return ret;
}
void rgba_image::flatten(vecteur &res) const {
    res.clear();
    int sz=_w*_h*_d;
    res.reserve(sz);
    for (int i=0;i<sz;++i)
        res.push_back(_data[i]);
}
rgba_image rgba_image::to_negative() const {
    rgba_image ret(_d,_w,_h,ctx);
    int ac=has_alpha()?_d:0,sz=_w*_h*_d;
    for (int i=0;i<sz;++i)
        ret._data[i]=(ac>0 && (i+1)%ac==0)?_data[i]:255-_data[i];
    return ret;
}

#ifndef NO_NAMESPACE_GIAC
}
#endif // ndef NO_NAMESPACE_GIAC
