/*
 * signalprocessing.cc
 *
 * (c) 2018 by Luka Marohnić
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

#ifdef HAVE_LIBSAMPLERATE
#include "samplerate.h"
#endif

using namespace std;

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

int nextpow2(int n) {
    int m=1;
    while (m<n) m*=2;
    return m;
}

bool is_sound_data(const gen &g,int &nc,int &bd,int &sr,int &len) {
    if (g.type!=_VECT)
        return false;
    vecteur &gv=*g._VECTptr;
    if (gv.size()<2 || gv.front().type!=_VECT || gv.front()._VECTptr->size()!=4)
        return false;
    vecteur &header=*gv.front()._VECTptr;
    if (!is_integer_vecteur(header))
        return false;
    int n=header[0].val,b=header[1].val,s=header[2].val,l=(8*header[3].val)/(n*b);
    if (n<1 || (b!=8 && b!=16 && b!=24) || s<1 || l<0 ||
            int(gv.size())!=n+1)
        return false;
    for (const_iterateur it=gv.begin()+1;it!=gv.end();++it) {
        if (it->type!=_VECT)
            return false;
        if (int(it->_VECTptr->size())!=l)
            return false;
    }
    nc=n; bd=b; sr=s; len=l;
    return true;
}

vecteur decode_chdata(const vecteur &data,int bd,int start,int len) {
    if (data.empty() || (bd!=8 && bd!=16))
        return vecteur(0);
    vecteur res;
    int l=len<0?data.size():len;
    res.reserve(l);
    int v;
    double denom=std::pow(2,bd-1);
    for (const_iterateur it=data.begin()+start;it!=data.begin()+start+l;++it) {
        if (!it->is_integer())
            return vecteur(0);
        v=it->val;
        if (bd==8) v-=127;
        else if (bd==16 && v>=32768) v-=65536;
        res.push_back(double(v)/denom);
    }
    return res;
}

vecteur encode_chdata(const vecteur &data,int bd,double ratio,GIAC_CONTEXT) {
    if (data.empty() || (bd!=8 && bd!=16))
        return vecteur(0);
    vecteur res;
    res.reserve(data.size());
    double v,fac=std::pow(2.0,bd-1)-1.0;
    int k;
    for (const_iterateur it=data.begin();it!=data.end();++it) {
        if (!is_real(*it,contextptr))
            return vecteur(0);
        v=std::max(-1.0,std::min(1.0,ratio*_evalf(*it,contextptr).DOUBLE_val()));
        k=std::floor(fac*v);
        if (bd==8) k+=127;
        res.push_back(k);
    }
    return res;
}

int read_channel_data(const gen &g,int &nc,vector<vecteur*> &data) {
    int ret;
    if (ckmatrix(g)) {
        matrice &chns=*g._VECTptr;
        nc=chns.size();
        ret=chns.front()._VECTptr->size();
        for (const_iterateur jt=chns.begin();jt!=chns.end();++jt) {
            data.push_back(jt->_VECTptr);
        }
    } else {
        data.push_back(g._VECTptr);
        ret=g._VECTptr->size();
    }
    return ret;
}

gen _createwav(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    int len=0,bd=16,sr=44100,nc=1;
    double ratio=1.0,norm=-1;
    vector<vecteur*> data;
    if (g.is_integer()) {
        if (g.val<1)
            return gensizeerr(contextptr);
        len=g.val;
    } else if (g.type==_VECT) {
        if (g.subtype==_SEQ__VECT) {
            vecteur &args=*g._VECTptr;
            double secs=-1,plen=-1;
            for (const_iterateur it=args.begin();it!=args.end();++it) {
                if (it->is_symb_of_sommet(at_equal)) {
                    gen &lh=it->_SYMBptr->feuille._VECTptr->front();
                    gen &rh=it->_SYMBptr->feuille._VECTptr->back();
                    if (lh==at_channels) {
                        if (!rh.is_integer() || (nc=rh.val)<1)
                            return gensizeerr(contextptr);
                    } else if (lh==at_bit_depth) {
                        if (!rh.is_integer() || (bd=rh.val)<0 || (bd!=8 && bd!=16))
                            return gensizeerr(contextptr);
                    } else if (lh==at_samplerate) {
                        if (!rh.is_integer() || (sr=rh.val)<1)
                            return gensizeerr(contextptr);
                    } else if (lh==at_size) {
                        if (!rh.is_integer() || (plen=rh.val)<1)
                            return gensizeerr(contextptr);
                    } else if (lh==at_normalize) {
                        if (!is_real(rh,contextptr))
                            return gensizeerr(contextptr);
                        norm=-_evalf(rh,contextptr).DOUBLE_val();
                    } else if (lh==at_duration) {
                        if (!is_real(rh,contextptr))
                            return gensizeerr(contextptr);
                        secs=_evalf(rh,contextptr).DOUBLE_val();
                    } else if (lh==at_channel_data) {
                        if (rh.type!=_VECT)
                            return gensizeerr(contextptr);
                        plen=read_channel_data(rh,nc,data);
                    } else return gensizeerr(contextptr);
                } else if (it->type==_VECT)
                    plen=read_channel_data(*it,nc,data);
            }
            if (secs>=0)
                len=1+std::floor(secs*sr);
            else if ((len=plen)<0)
                return gensizeerr(contextptr);
        } else {
            if (is_sound_data(g,nc,bd,sr,len)) {
                for (int i=1;i<=nc;++i) {
                    data.push_back(g._VECTptr->at(i)._VECTptr);
                }
            } else len=read_channel_data(g,nc,data);
        }
    } else return gentypeerr(contextptr);
    if (!data.empty() && int(data.size())!=nc)
        return gendimerr(contextptr);
    if (len==0) {
        for (vector<vecteur*>::const_iterator it=data.begin();it!=data.end();++it) {
            len=std::max(len,(int)(*it)->size());
        }
    }
    if (norm>=0) {
        double maxamp=0;
        for (vector<vecteur*>::const_iterator it=data.begin();it!=data.end();++it) {
            maxamp=std::max(maxamp,_evalf(_max(_abs(**it,contextptr),contextptr),contextptr).DOUBLE_val());
        }
        ratio=std::pow(10.0,-norm/20.0)/maxamp;
    }
    /* create wav data */
    vecteur header,res;
    header.resize(4);
    header[0]=nc;
    header[1]=bd;
    header[2]=sr;
    header[3]=(bd*len*nc)/8;
    res.push_back(header);
    for (vector<vecteur*>::const_iterator it=data.begin();it!=data.end();++it) {
        vecteur enc=encode_chdata(**it,bd,ratio,contextptr);
        enc.resize(len,0);
        res.push_back(enc);
    }
    return res;
}
static const char _createwav_s []="createwav";
static define_unary_function_eval (__createwav,&_createwav,_createwav_s);
define_unary_function_ptr5(at_createwav,alias_at_createwav,&__createwav,0,true)

gen _plotwav(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    vecteur opts;
    if (g.subtype==_SEQ__VECT)
        opts=vecteur(g._VECTptr->begin()+1,g._VECTptr->end());
    const gen &wav=g.subtype==_SEQ__VECT?g._VECTptr->front():g;
    int nc,bd,sr,len,slice_start=0;
    if (!is_sound_data(wav,nc,bd,sr,len))
        return gentypeerr(contextptr);
    for (const_iterateur it=opts.begin();it!=opts.end();++it) {
        if (it->is_symb_of_sommet(at_equal)) {
            gen &lh=it->_SYMBptr->feuille._VECTptr->front();
            gen &rh=it->_SYMBptr->feuille._VECTptr->back();
            if (lh==at_range) {
                if (rh.is_symb_of_sommet(at_interval)) {
                    gen a=rh._SYMBptr->feuille._VECTptr->front();
                    gen b=rh._SYMBptr->feuille._VECTptr->back();
                    if (!is_real(a,contextptr) || !is_real(b,contextptr) || is_positive(a-b,contextptr))
                        return gensizeerr(contextptr);
                    slice_start=std::max(slice_start,_floor(a*gen(sr),contextptr).val);
                    len=std::min(len-slice_start,_floor(b*gen(sr),contextptr).val-slice_start);
                } else if (rh.type==_VECT) {
                    if (rh._VECTptr->size()!=2 || !rh._VECTptr->front().is_integer() ||
                            !rh._VECTptr->back().is_integer())
                        return gensizeerr(contextptr);
                    slice_start=std::max(slice_start,rh._VECTptr->front().val);
                    len=std::min(len-slice_start,rh._VECTptr->back().val-slice_start);
                } else return gensizeerr(contextptr);
            }
        }
    }
    if (len==0)
        return vecteur(0);
    double dur=double(len)/double(sr),width=1500.0,sc=double(len)/width,x,y=nc==1?0.0:1.0;
    double fmax,fmin,s,t0=double(slice_start)/double(sr),dt=1.0/sr;
    int step=std::floor(sc),bnd;
    vecteur drawing,tvec;
    if (sc<=1.0) tvec.resize(len);
    for (int c=1;c<=nc;++c) {
        vecteur data=decode_chdata(*wav._VECTptr->at(c)._VECTptr,bd,slice_start,len),unodes,lnodes;
        if (sc<=1.0) {
            for (iterateur it=data.begin();it!=data.end();++it) {
                *it+=gen(y);
                tvec[it-data.begin()]=t0+(it-data.begin())*dt;
            }
            drawing.push_back(_listplot(_zip(makesequence(at_makevector,tvec,data),contextptr),contextptr));
        } else {
            unodes.reserve(std::floor(width));
            lnodes.reserve(std::floor(width));
            for (int i=0;i<len;i+=step) {
                fmax=fmin=0.0;
                bnd=std::min(i+step,len);
                for (int j=i;j<bnd;++j) {
                    s=_evalf(data[j],contextptr).DOUBLE_val();
                    if (s>fmax) fmax=s;
                    else if (s<fmin) fmin=s;
                }
                x=t0+dur*(i/step)/width;
                unodes.push_back(makecomplex(x,y+fmax));
                lnodes.push_back(makecomplex(x,y+fmin));
            }
            std::reverse(lnodes.begin(),lnodes.end());
            vecteur nodes=mergevecteur(unodes,lnodes);
            nodes.push_back(symbolic(at_equal,makesequence(at_display,_FILL_POLYGON)));
            drawing.push_back(_polygone(change_subtype(nodes,_SEQ__VECT),contextptr));
            drawing.push_back(_segment(makesequence(makecomplex(t0,y),makecomplex(t0+dur,y)),contextptr));
        }
        y-=2.0;
    }
    return drawing;
}
static const char _plotwav_s []="plotwav";
static define_unary_function_eval (__plotwav,&_plotwav,_plotwav_s);
define_unary_function_ptr5(at_plotwav,alias_at_plotwav,&__plotwav,0,true)

gen _stereo2mono(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    int nc,bd,sr,len;
    if (!is_sound_data(g,nc,bd,sr,len))
        return gentypeerr(contextptr);
    if (nc==1)
        return g;
    vecteur data(len,0);
    for (int i=1;i<=nc;++i) {
        const vecteur &chan=*g._VECTptr->at(i)._VECTptr;
        data=addvecteur(data,decode_chdata(chan,bd));
    }
    data=multvecteur(fraction(1,nc),data);
    vecteur header=*g._VECTptr->front()._VECTptr,edata=encode_chdata(data,bd,1.0,contextptr);
    header[0]=1;
    header[3]=header[3]/gen(nc);
    return makevecteur(header,edata);
}
static const char _stereo2mono_s []="stereo2mono";
static define_unary_function_eval (__stereo2mono,&_stereo2mono,_stereo2mono_s);
define_unary_function_ptr5(at_stereo2mono,alias_at_stereo2mono,&__stereo2mono,0,true)

gen _plotspectrum(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    int nc,bd,sr,len,lfreq,ufreq;
    if (is_sound_data(g,nc,bd,sr,len)) {
        gen intrv=symbolic(at_interval,makesequence(0,sr/2));
        return _plotspectrum(makesequence(g,symbolic(at_equal,makesequence(at_range,intrv))),contextptr);
    } else if (g.subtype==_SEQ__VECT) {
        vecteur &gv=*g._VECTptr;
        if (gv.size()!=2 || !is_sound_data(gv.front(),nc,bd,sr,len))
            return gensizeerr(contextptr);
        vecteur data;
        if (nc>1)
            data=decode_chdata(*_stereo2mono(gv.front(),contextptr)._VECTptr->at(1)._VECTptr,bd);
        else data=decode_chdata(*gv.front()._VECTptr->at(1)._VECTptr,bd);
        len=data.size();
        if (!gv.back().is_symb_of_sommet(at_equal))
            return gensizeerr(contextptr);
        gen &lh=gv.back()._SYMBptr->feuille._VECTptr->front();
        gen &rh=gv.back()._SYMBptr->feuille._VECTptr->back();
        if (lh==at_range) {
            gen a,b;
            if (rh.type==_VECT) {
                if (rh._VECTptr->size()!=2 || !is_integer_vecteur(*rh._VECTptr))
                    return gensizeerr(contextptr);
                a=rh._VECTptr->front();
                b=rh._VECTptr->back();
            } else if (rh.is_symb_of_sommet(at_interval)) {
                a=rh._SYMBptr->feuille._VECTptr->front();
                b=rh._SYMBptr->feuille._VECTptr->back();
            } else return gensizeerr(contextptr);
            if (!is_integer(a) || !is_integer(b))
                return gensizeerr(contextptr);
            lfreq=std::max(0,a.val);
            ufreq=std::min(sr/2,b.val);
            if (lfreq>=ufreq)
                return gensizeerr(contextptr);
        }
        int n=nextpow2(len);
        data.resize(n,0);
        vecteur spec=*_fft(data,contextptr)._VECTptr;
        vecteur nodes;
        int dfreq=ufreq-lfreq,n1=((long)n*(long)lfreq)/(long)sr,n2=((long)n*(long)ufreq)/(long)sr;
        int width=std::min(dfreq,std::max(1500,dfreq/5));
        nodes.reserve(std::floor(width));
        int step=(n2-n1)/(2*width);
        if (step==0) step=1;
        double f;
        for (int i=n1;i<n2;++i) {
            f=std::max(f,_evalf(pow(_abs(spec[i],contextptr),2)/gen(n),contextptr).DOUBLE_val());
            if (i%step==0) {
                nodes.push_back(makecomplex(((long)i*(long)sr)/(long)n,f));
                f=0;
            }
        }
        nodes.push_back(ufreq+1);
        nodes.push_back(lfreq);
        nodes.push_back(symbolic(at_equal,makesequence(at_display,_FILL_POLYGON)));
        return _polygone(change_subtype(nodes,_SEQ__VECT),contextptr);
    } else return gentypeerr(contextptr);
}
static const char _plotspectrum_s []="plotspectrum";
static define_unary_function_eval (__plotspectrum,&_plotspectrum,_plotspectrum_s);
define_unary_function_ptr5(at_plotspectrum,alias_at_plotspectrum,&__plotspectrum,0,true)

gen _channels(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    int nc,bd,sr,len;
    if (!is_sound_data(g,nc,bd,sr,len))
        return gentypeerr(contextptr);
    return nc;
}
static const char _channels_s []="channels";
static define_unary_function_eval (__channels,&_channels,_channels_s);
define_unary_function_ptr5(at_channels,alias_at_channels,&__channels,0,true)

gen _bit_depth(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    int nc,bd,sr,len;
    if (!is_sound_data(g,nc,bd,sr,len))
        return gentypeerr(contextptr);
    return bd;
}
static const char _bit_depth_s []="bit_depth";
static define_unary_function_eval (__bit_depth,&_bit_depth,_bit_depth_s);
define_unary_function_ptr5(at_bit_depth,alias_at_bit_depth,&__bit_depth,0,true)

gen _samplerate(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    int nc,bd,sr,len;
    if (!is_sound_data(g,nc,bd,sr,len))
        return gentypeerr(contextptr);
    return sr;
}
static const char _samplerate_s []="samplerate";
static define_unary_function_eval (__samplerate,&_samplerate,_samplerate_s);
define_unary_function_ptr5(at_samplerate,alias_at_samplerate,&__samplerate,0,true)

gen _duration(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    int nc,bd,sr,len;
    if (!is_sound_data(g,nc,bd,sr,len))
        return gentypeerr(contextptr);
    return _evalf(fraction(len,sr),contextptr);
}
static const char _duration_s []="duration";
static define_unary_function_eval (__duration,&_duration,_duration_s);
define_unary_function_ptr5(at_duration,alias_at_duration,&__duration,0,true)

gen _channel_data(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    vecteur opts;
    if (g.subtype==_SEQ__VECT) {
        if (g._VECTptr->front().type!=_VECT)
            return gentypeerr(contextptr);
        opts=vecteur(g._VECTptr->begin()+1,g._VECTptr->end());
    }
    const gen &data=g.subtype==_SEQ__VECT?g._VECTptr->front():g;
    int nc,bd,sr,len;
    if (!is_sound_data(data,nc,bd,sr,len))
        return gentypeerr(contextptr);
    int chan=0,slice_start=0,slice_len=len;
    bool asmatrix=false;
    for (const_iterateur it=opts.begin();it!=opts.end();++it) {
        if (it->is_integer()) {
            if (it->val<1 || it->val>nc)
                return gensizeerr(contextptr);
            chan=it->val;
        } else if (*it==at_matrix)
            asmatrix=true;
        else if (it->is_symb_of_sommet(at_equal)) {
            gen &lh=it->_SYMBptr->feuille._VECTptr->front();
            gen &rh=it->_SYMBptr->feuille._VECTptr->back();
            if (lh==at_range) {
                if (rh.type==_VECT) {
                    if (rh._VECTptr->size()!=2)
                        return gendimerr(contextptr);
                    if (!is_integer_vecteur(*rh._VECTptr))
                        return gensizeerr(contextptr);
                    int start=rh._VECTptr->front().val,stop=rh._VECTptr->back().val;
                    slice_start=start-1; slice_len=stop-start+1;
                } else if (rh.is_symb_of_sommet(at_interval)) {
                    gen &a=rh._SYMBptr->feuille._VECTptr->front();
                    gen &b=rh._SYMBptr->feuille._VECTptr->back();
                    if (!a.is_integer() || !b.is_integer()) {
                        if (!is_real(a,contextptr) || !is_real(b,contextptr))
                            return gensizeerr(contextptr);
                        slice_start=std::floor(sr*_evalf(a,contextptr).DOUBLE_val());
                        slice_len=std::floor(sr*_evalf(b-a,contextptr).DOUBLE_val());
                    } else {
                        slice_start=a.val-1;
                        slice_len=(b-a).val+1;
                    }
                } else return gensizeerr(contextptr);
            }
        }
    }
    if (slice_start<0 || slice_start>=len || slice_len<0 || slice_len>len || slice_start+slice_len>len)
        return gensizeerr(contextptr);
    if (chan==0) {
        vecteur ret;
        for (const_iterateur it=data._VECTptr->begin()+1;it!=data._VECTptr->end();++it) {
            ret.push_back(decode_chdata(*it->_VECTptr,bd,slice_start,slice_len));
        }
        if (ret.size()==1)
            return ret.front();
        return asmatrix?ret:change_subtype(ret,_SEQ__VECT);
    }
    return decode_chdata(*data._VECTptr->at(chan)._VECTptr,bd,slice_start,slice_len);
}
static const char _channel_data_s []="channel_data";
static define_unary_function_eval (__channel_data,&_channel_data,_channel_data_s);
define_unary_function_ptr5(at_channel_data,alias_at_channel_data,&__channel_data,0,true)

gen _cross_correlation(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &args=*g._VECTptr;
    if (args.size()!=2 || args.front().type!=_VECT || args.back().type!=_VECT)
        return gensizeerr(contextptr);
    vecteur A=*args.front()._VECTptr,B=*args.back()._VECTptr;
    int m=A.size(),n=B.size(),N=nextpow2(std::max(n,m));
    A.resize(2*N,0);
    B.resize(2*N,0);
    vecteur a=*_fft(A,contextptr)._VECTptr,b=*_fft(B,contextptr)._VECTptr;
    vecteur cc_ffted=*_pointprod(makesequence(a,conj(b,contextptr)),contextptr)._VECTptr;
    vecteur cc=*_apply(makesequence(at_real,_ifft(cc_ffted,contextptr)),contextptr)._VECTptr;
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
    for (iterateur it=data.begin();it!=data.end();++it) {
        s+=alpha*(*it-s);
        *it=s;
    }
}

void highpass(vecteur &data,double cutoff,int sr) {
    double rc=0.15915494309/cutoff;
    double dt=1.0/sr;
    gen alpha(rc/(rc+dt)),prevdata=data.front(),temp;
    for (iterateur it=data.begin()+1;it!=data.end();++it) {
        temp=*it;
        *it=alpha*(*(it-1)+*it-prevdata);
        prevdata=temp;
    }
}

gen filter(const vecteur &args,filter_type typ,GIAC_CONTEXT) {
    double cutoff=_evalf(args.at(1),contextptr).DOUBLE_val();
    int nc,bd,sr,len;
    if (is_sound_data(*args.front()._VECTptr,nc,bd,sr,len)) {
        if (cutoff<=0 || cutoff>=sr/2)
            return gensizeerr(contextptr);
        gen opt(undef);
        if (args.size()>2) {
            if (!args[2].is_symb_of_sommet(at_equal))
                return gensizeerr(contextptr);
            if (args[2]._SYMBptr->feuille._VECTptr->front()==at_normalize)
                opt=args[2];
        }
        matrice data;
        for (int i=1;i<=nc;++i) {
            vecteur chdata=decode_chdata(*args.front()._VECTptr->at(i)._VECTptr,bd);
            switch(typ) {
            case _LOWPASS_FILTER: lowpass(chdata,cutoff,sr); break;
            case _HIGHPASS_FILTER: highpass(chdata,cutoff,sr); break;
            }
            data.push_back(chdata);
        }
        gen argv=makesequence(symbolic(at_equal,makesequence(at_samplerate,sr)),
                              symbolic(at_equal,makesequence(at_bit_depth,bd)),data);
        if (!is_undef(opt)) argv._VECTptr->push_back(opt);
        return _createwav(argv,contextptr);
    } else {
        vecteur data=*args.front()._VECTptr;
        sr=44100;
        if (args.size()>2) {
            if (!is_integer(args.at(2)) || (sr=args.at(2).val)<=0)
                return gentypeerr(contextptr);
        }
        if (cutoff<=0 || cutoff>=sr/2)
            return gensizeerr(contextptr);
        switch(typ) {
        case _LOWPASS_FILTER: lowpass(data,cutoff,sr); break;
        case _HIGHPASS_FILTER: highpass(data,cutoff,sr); break;
        }
        return data;
    }
}

gen _lowpass(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()<2 ||
            g._VECTptr->front().type!=_VECT || !is_real(g._VECTptr->at(1),contextptr))
        return gentypeerr(contextptr);
    return filter(*g._VECTptr,_LOWPASS_FILTER,contextptr);
}
static const char _lowpass_s []="lowpass";
static define_unary_function_eval (__lowpass,&_lowpass,_lowpass_s);
define_unary_function_ptr5(at_lowpass,alias_at_lowpass,&__lowpass,0,true)

gen _highpass(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()<2 ||
            g._VECTptr->front().type!=_VECT || !is_real(g._VECTptr->at(1),contextptr))
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
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr("Wrong number of input arguments");
    if (gv.front().type!=_VECT)
        return gensizeerr("First argument must be an array");
    if (!gv.back().is_integer() || gv.back().val<=0)
        return gensizeerr("Second argument must be a positive integer");
    vecteur &s=*gv.front()._VECTptr;
    int n=gv.back().val,len=s.size();
    if (n>len)
        return gensizeerr("Filter length exceeds array size");
    vecteur res(len-n+1);
    gen acc(0);
    for (int i=0;i<n;++i) acc+=s[i];
    res[0]=acc;
    for (int i=n;i<len;++i) {
        acc-=s[i-n];
        acc+=s[i];
        res[i-n+1]=acc;
    }
    return multvecteur(fraction(1,n),res);
}
static const char _moving_average_s []="moving_average";
static define_unary_function_eval (__moving_average,&_moving_average,_moving_average_s);
define_unary_function_ptr5(at_moving_average,alias_at_moving_average,&__moving_average,0,true)

gen _resample(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
#ifndef HAVE_LIBSAMPLERATE
    *logptr(contextptr) << "Error: libsamplerate is required for resampling audio" << endl;
    return vecteur(0);
#else
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    int nc,bd,sr,len;
    const gen &snd=g.subtype==_SEQ__VECT?g._VECTptr->front():g;
    if (!is_sound_data(snd,nc,bd,sr,len))
        return gentypeerr(contextptr);
    int nsr=44100;
    int quality=2;
    if (g.subtype==_SEQ__VECT) {
        vecteur &gv=*g._VECTptr;
        if (gv.size()<2 || !gv[1].is_integer() || (nsr=gv[1].val)<1)
            return gensizeerr(contextptr);
        if (gv.size()>2) {
            if (!gv[2].is_integer() || (quality=gv[2].val)<0 || (quality>4))
                return gensizeerr(contextptr);
        }
    }
    matrice chdata;
    for (int i=1;i<=nc;++i) {
        vecteur ch=decode_chdata(*snd._VECTptr->at(i)._VECTptr,bd);
        chdata.push_back(ch);
    }
    int nlen=((long)len*(long)nsr)/(long)sr;
    SRC_DATA data;
    data.input_frames=len;
    data.output_frames=nlen;
    float *indata=new float[len*nc];
    data.data_out=new float[nlen*nc];
    data.src_ratio=double(nsr)/double(sr);
    for (int i=0;i<len;++i) {
        for (int j=0;j<nc;++j) {
            indata[i*nc+j]=_evalf(chdata[j][i],contextptr).DOUBLE_val();
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
    src_simple(&data,quality,nc);
    matrice output=*_matrix(makesequence(nc,nlen,0),contextptr)._VECTptr;
    for (int i=0;i<nlen;++i) {
        for (int j=0;j<nc;++j) {
            output[j]._VECTptr->at(i)=data.data_out[i*nc+j];
        }
    }
    delete[] indata;
    delete[] data.data_out;
    vecteur header=*snd._VECTptr->front()._VECTptr;
    header[2]=nsr;
    header[3]=(bd*nc*nlen)/8;
    vecteur ret;
    ret.push_back(header);
    for (const_iterateur it=output.begin();it!=output.end();++it) {
        ret.push_back(encode_chdata(*it->_VECTptr,bd,1.0,contextptr));
    }
    return ret;
#endif
}
static const char _resample_s []="resample";
static define_unary_function_eval (__resample,&_resample,_resample_s);
define_unary_function_ptr5(at_resample,alias_at_resample,&__resample,0,true)

gen _convolution(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &args=*g._VECTptr;
    if (args.size()!=2 || args.front().type!=_VECT || args.back().type!=_VECT)
        return gensizeerr(contextptr);
    vecteur A=*args.front()._VECTptr,B=*args.back()._VECTptr;
    int lenA=A.size(),lenB=B.size(),len=2*nextpow2(std::max(lenA,lenB));
    A.resize(len-1,0);
    B.resize(len-1,0);
    vecteur a=*_fft(A,contextptr)._VECTptr,b=*_fft(B,contextptr)._VECTptr;
    vecteur cv=*_apply(makesequence(at_real,_ifft(_pointprod(makesequence(a,b),
                                                             contextptr),
                                                  contextptr)),
                       contextptr)._VECTptr;
    cv.resize(lenA+lenB-1);
    return cv;
}
static const char _convolution_s []="convolution";
static define_unary_function_eval (__convolution,&_convolution,_convolution_s);
define_unary_function_ptr5(at_convolution,alias_at_convolution,&__convolution,0,true)

vecteur apply_window_function(const gen &expr,const identificateur &k,const vecteur &data,int start,int len,GIAC_CONTEXT) {
    vecteur output(len);
    for (int j=0;j<len;++j) {
        output[j]=_evalf(subst(expr,k,gen((double)j),false,contextptr),contextptr)*data[start+j];
    }
    return output;
}

bool nivelate(vecteur &data,int k,const gen &b,const gen &val,const unary_function_ptr *comp,GIAC_CONTEXT) {
    gen r;
    if (has_i(data[k]) && !is_zero((r=_abs(data[k],contextptr)))) {
        if (_eval(symbolic(comp,makevecteur(r,b)),contextptr).val!=0) {
            data[k]=val*data[k]/r;
            return true;
        }
    }
    else {
        if (_eval(symbolic(comp,makevecteur(data[k],b)),contextptr).val!=0) {
            data[k]=val;
            return true;
        }
    }
    return false;
}

gen _threshold(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &args=*g._VECTptr;
    if (int(args.size())<2)
        return gensizeerr(contextptr);
    if (args.front().type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &data=*args.front()._VECTptr;
    gen &bnd=args.at(1);
    int n=data.size();
    vecteur output=data;
    if (bnd.type==_VECT) {
        if (int(bnd._VECTptr->size())!=2)
            return gensizeerr(contextptr);
        gen lb=bnd._VECTptr->front(),ub=bnd._VECTptr->back(),lval,uval;
        if (lb.is_symb_of_sommet(at_equal)) {
            lval=_rhs(lb,contextptr);
            lb=_lhs(lb,contextptr);
        } else lval=lb;
        if (ub.is_symb_of_sommet(at_equal)) {
            uval=_rhs(ub,contextptr);
            ub=_lhs(ub,contextptr);
        } else uval=ub;
        for (int k=0;k<n;++k) {
            if (!nivelate(output,k,lb,lval,at_inferieur_strict,contextptr))
                nivelate(output,k,ub,uval,at_superieur_strict,contextptr);
        }
    } else {
        gen val;
        if (bnd.is_symb_of_sommet(at_equal)) {
            val=_rhs(bnd,contextptr);
            bnd=_lhs(bnd,contextptr);
        } else val=bnd;
        if (!is_real(bnd,contextptr))
            return gentypeerr(contextptr);
        gen comp=at_inferieur_strict,isabs;
        bool absolute=false;
        for (const_iterateur it=args.begin()+2;it!=args.end();++it) {
            if (*it==at_superieur_strict || *it==at_superieur_egal || *it==at_inferieur_egal)
                comp=*it;
            isabs=gen(1);
            if (*it==at_abs || (it->is_symb_of_sommet(at_equal) &&
                                it->_SYMBptr->feuille._VECTptr->front()==at_abs &&
                                (isabs=it->_SYMBptr->feuille._VECTptr->back()).type==_INT_ &&
                                isabs.subtype==_INT_BOOLEAN)) {
                if (has_i(data) || !is_strictly_positive(bnd,contextptr))
                    return gentypeerr(contextptr);
                absolute=(bool)isabs.val;
            }
        }
        for (int k=0;k<n;++k) {
            if (absolute) {
                if (_eval(symbolic(comp._FUNCptr,makevecteur(_abs(data[k],contextptr),bnd)),contextptr).val!=0)
                    output[k]=is_positive(data[k],contextptr)?val:-val;
            } else nivelate(output,k,bnd,val,comp._FUNCptr,contextptr);
        }
    }
    return _eval(output,contextptr);
}
static const char _threshold_s []="threshold";
static define_unary_function_eval (__threshold,&_threshold,_threshold_s);
define_unary_function_ptr5(at_threshold,alias_at_threshold,&__threshold,0,true)

bool parse_window_parameters(const gen &g,vecteur &data,int &start,int &len,double *alpha,GIAC_CONTEXT) {
    start=0;
    if (g.type==_VECT && g.subtype!=_SEQ__VECT) {
        data=*g._VECTptr;
        len=data.size();
        return true;
    }
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()>3 || g._VECTptr->front().type!=_VECT)
        return false;
    vecteur &args=*g._VECTptr;
    data=*args.front()._VECTptr;
    len=data.size();
    bool has_alpha;
    if (is_real(args.at(1),contextptr)) {
        has_alpha=true;
        if (!alpha)
            return false;
        *alpha=_evalf(args.at(1),contextptr).DOUBLE_val();
    } else if (args.size()>2) return false;
    if (args.back().is_symb_of_sommet(at_interval)) {
        gen lh=_lhs(args.back(),contextptr),rh=_rhs(args.back(),contextptr);
        if (!lh.is_integer() || !rh.is_integer() || lh.val<0 || rh.val>=len)
            return false;
        start=lh.val;
        len=rh.val-start+1;
    } else if (args.size()>2 || !has_alpha) return false;
    return true;
}

gen _bartlett_hann_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,0,contextptr))
        return gentypeerr(contextptr);
    double a=0.62,b=0.48,c=0.38;
    gen expr=a-b*_abs(k/(N-1)-fraction(1,2),contextptr)-c*cos(2*k*_IDNT_pi()/(N-1),contextptr);
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _bartlett_hann_window_s []="bartlett_hann_window";
static define_unary_function_eval (__bartlett_hann_window,&_bartlett_hann_window,_bartlett_hann_window_s);
define_unary_function_ptr5(at_bartlett_hann_window,alias_at_bartlett_hann_window,&__bartlett_hann_window,0,true)

gen _blackman_harris_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,0,contextptr))
        return gentypeerr(contextptr);
    gen a(0.35875),b(0.48829),c(0.14128),d(0.01168);
    gen K=k*_IDNT_pi()/(N-1),expr=a-b*cos(2*K,contextptr)+c*cos(4*K,contextptr)-d*cos(6*K,contextptr);
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _blackman_harris_window_s []="blackman_harris_window";
static define_unary_function_eval (__blackman_harris_window,&_blackman_harris_window,_blackman_harris_window_s);
define_unary_function_ptr5(at_blackman_harris_window,alias_at_blackman_harris_window,&__blackman_harris_window,0,true)

gen _blackman_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    double alpha=0.16;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,&alpha,contextptr) || alpha<=0)
        return gentypeerr(contextptr);
    gen K=k*_IDNT_pi()/(N-1),expr=(1-alpha)/2-cos(2*K,contextptr)/2+alpha*cos(4*K,contextptr)/2;
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _blackman_window_s []="blackman_window";
static define_unary_function_eval (__blackman_window,&_blackman_window,_blackman_window_s);
define_unary_function_ptr5(at_blackman_window,alias_at_blackman_window,&__blackman_window,0,true)

gen _bohman_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,0,contextptr))
        return gentypeerr(contextptr);
    gen K=_abs(2*k/(N-1)-1,contextptr),expr=(1-K)*cos(_IDNT_pi()*K,contextptr)+sin(_IDNT_pi()*K,contextptr)/_IDNT_pi();
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _bohman_window_s []="bohman_window";
static define_unary_function_eval (__bohman_window,&_bohman_window,_bohman_window_s);
define_unary_function_ptr5(at_bohman_window,alias_at_bohman_window,&__bohman_window,0,true)

gen _cosine_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    double alpha=1.0;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,&alpha,contextptr) || alpha<=0)
        return gentypeerr(contextptr);
    gen expr=exp(alpha*ln(sin(k*_IDNT_pi()/(N-1),contextptr),contextptr),contextptr);
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _cosine_window_s []="cosine_window";
static define_unary_function_eval (__cosine_window,&_cosine_window,_cosine_window_s);
define_unary_function_ptr5(at_cosine_window,alias_at_cosine_window,&__cosine_window,0,true)

gen _gaussian_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    double alpha=0.1;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,&alpha,contextptr) || alpha<=0 || alpha>0.5)
        return gentypeerr(contextptr);
    gen c=(N-1)/2.0,expr=exp(-pow((k-c)/(alpha*c),2)/2,contextptr);
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _gaussian_window_s []="gaussian_window";
static define_unary_function_eval (__gaussian_window,&_gaussian_window,_gaussian_window_s);
define_unary_function_ptr5(at_gaussian_window,alias_at_gaussian_window,&__gaussian_window,0,true)

gen _hamming_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,0,contextptr))
        return gentypeerr(contextptr);
    gen a(0.54),b(0.46),expr=a-b*cos(2*_IDNT_pi()*k/(N-1),contextptr);
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _hamming_window_s []="hamming_window";
static define_unary_function_eval (__hamming_window,&_hamming_window,_hamming_window_s);
define_unary_function_ptr5(at_hamming_window,alias_at_hamming_window,&__hamming_window,0,true)

gen _hann_poisson_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    double alpha=1;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,&alpha,contextptr))
        return gentypeerr(contextptr);
    gen K=2*_IDNT_pi()*k/(N-1);
    gen expr=(1-cos(K,contextptr))*exp(-alpha*_abs(N-1-2*k,contextptr)/(N-1),contextptr)/2;
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _hann_poisson_window_s []="hann_poisson_window";
static define_unary_function_eval (__hann_poisson_window,&_hann_poisson_window,_hann_poisson_window_s);
define_unary_function_ptr5(at_hann_poisson_window,alias_at_hann_poisson_window,&__hann_poisson_window,0,true)

gen _hann_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,0,contextptr))
        return gentypeerr(contextptr);
    gen expr=pow(sin(_IDNT_pi()*k/(N-1),contextptr),2);
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _hann_window_s []="hann_window";
static define_unary_function_eval (__hann_window,&_hann_window,_hann_window_s);
define_unary_function_ptr5(at_hann_window,alias_at_hann_window,&__hann_window,0,true)

gen _parzen_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,0,contextptr))
        return gentypeerr(contextptr);
    gen K=1-2*k/(N-1),cond=symb_inferieur_egal(symbolic(at_abs,(N-1)/2.0-k),(N-1)/4.0);
    gen f1=1-6*pow(K,2)*(1-_abs(K,contextptr)),f2=2*pow(1-_abs(K,contextptr),3);
    gen expr=symbolic(at_when,makevecteur(cond,f1,f2));
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _parzen_window_s []="parzen_window";
static define_unary_function_eval (__parzen_window,&_parzen_window,_parzen_window_s);
define_unary_function_ptr5(at_parzen_window,alias_at_parzen_window,&__parzen_window,0,true)

gen _poisson_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    double alpha=1.0;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,&alpha,contextptr))
        return gentypeerr(contextptr);
    gen expr=exp(-alpha*_abs(2*k/(N-1)-1,contextptr),contextptr);
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _poisson_window_s []="poisson_window";
static define_unary_function_eval (__poisson_window,&_poisson_window,_poisson_window_s);
define_unary_function_ptr5(at_poisson_window,alias_at_poisson_window,&__poisson_window,0,true)

gen _riemann_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,0,contextptr))
        return gentypeerr(contextptr);
    gen K=(2*k/(N-1)-1)*_IDNT_pi(),cond=symbolic(at_same,makevecteur(k,(N-1)/2.0));
    gen expr=symbolic(at_when,makevecteur(cond,1,sin(K,contextptr)/K));
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _riemann_window_s []="riemann_window";
static define_unary_function_eval (__riemann_window,&_riemann_window,_riemann_window_s);
define_unary_function_ptr5(at_riemann_window,alias_at_riemann_window,&__riemann_window,0,true)

gen _triangle_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    double L=0;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,&L,contextptr) || (L!=1 && L!=-1 && L!=0))
        return gentypeerr(contextptr);
    gen expr=1-_abs((2*k-N+1)/(N+L),contextptr);
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _triangle_window_s []="triangle_window";
static define_unary_function_eval (__triangle_window,&_triangle_window,_triangle_window_s);
define_unary_function_ptr5(at_triangle_window,alias_at_triangle_window,&__triangle_window,0,true)

gen _tukey_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    double alpha=0.5;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,&alpha,contextptr) || alpha<0 || alpha>1)
        return gentypeerr(contextptr);
    double p=alpha*(N-1)/2.0,q=1-alpha/2;
    gen cond1=symb_inferieur_strict(k,p),cond2=symb_inferieur_egal(k,q*(N-1));
    gen f1=(1+cos(_IDNT_pi()*(k/p-1),contextptr))/2,f2=(1+cos(_IDNT_pi()*(k/p+1-2/alpha),contextptr))/2;
    gen expr=symbolic(at_piecewise,makevecteur(cond1,f1,cond2,1,f2));
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _tukey_window_s []="tukey_window";
static define_unary_function_eval (__tukey_window,&_tukey_window,_tukey_window_s);
define_unary_function_ptr5(at_tukey_window,alias_at_tukey_window,&__tukey_window,0,true)

gen _welch_window(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    vecteur data;
    int start,N;
    identificateur k(" k");
    if (!parse_window_parameters(g,data,start,N,0,contextptr))
        return gentypeerr(contextptr);
    double p=(N-1)/2.0;
    gen expr=1-pow(1-k/p,2);
    return apply_window_function(expr,k,data,start,N,contextptr);
}
static const char _welch_window_s []="welch_window";
static define_unary_function_eval (__welch_window,&_welch_window,_welch_window_s);
define_unary_function_ptr5(at_welch_window,alias_at_welch_window,&__welch_window,0,true)

#ifndef NO_NAMESPACE_GIAC
}
#endif // ndef NO_NAMESPACE_GIAC
