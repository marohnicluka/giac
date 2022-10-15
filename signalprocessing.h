/*
 * signalprocessing.h
 *
 * (c) 2018 Luka Marohnić
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
#ifndef __SIGNALPROCESSING_H
#define __SIGNALPROCESSING_H
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "first.h"
#include "gen.h"
#include "unary.h"
#include <stack>
#ifdef HAVE_LIBGSL
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_wavelet.h>
#include <gsl/gsl_wavelet2d.h>
#endif

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

enum filter_type {
    _LOWPASS_FILTER,
    _HIGHPASS_FILTER
};

enum FourierFuncType {
    _FOURIER_FUNCTYPE_UNKNOWN       =0,
    _FOURIER_FUNCTYPE_ONE           =1,
    _FOURIER_FUNCTYPE_DIRAC         =2,
    _FOURIER_FUNCTYPE_INV_MONOM     =3,
    _FOURIER_FUNCTYPE_GAUSSIAN      =4,
    _FOURIER_FUNCTYPE_LOGABSX       =5,
    _FOURIER_FUNCTYPE_BESSELJ       =6,
    _FOURIER_FUNCTYPE_SGN           =7,
    _FOURIER_FUNCTYPE_ABSX_ALPHA    =8,
    _FOURIER_FUNCTYPE_INVABSX       =9,
    _FOURIER_FUNCTYPE_GAMMA         =10,
    _FOURIER_FUNCTYPE_EXPEXP        =11,
    _FOURIER_FUNCTYPE_AIRY_AI       =12,
    _FOURIER_FUNCTYPE_EXP_HEAVISIDE =13,
    _FOURIER_FUNCTYPE_HEAVISIDE     =14,
    _FOURIER_FUNCTYPE_ATAN_OVERX    =15,
    _FOURIER_FUNCTYPE_EXPABSX       =16,
    _FOURIER_FUNCTYPE_EXPABSX_OVERX =17,
    _FOURIER_FUNCTYPE_COSECH        =18,
    _FOURIER_FUNCTYPE_SECH          =19,
    _FOURIER_FUNCTYPE_SECH_2        =20,
    _FOURIER_FUNCTYPE_CONVOLUTION   =21,
    _FOURIER_FUNCTYPE_PIECEWISE     =22,
    _FOURIER_FUNCTYPE_FUNC          =23,
    _FOURIER_FUNCTYPE_DIFF          =24,
    _FOURIER_FUNCTYPE_PARTIAL_DIFF  =25
};

enum ann_activation_function {
    _ANN_LINEAR     =1,
    _ANN_SIGMOID    =2,
    _ANN_TANH       =3
};

enum WindowFunctionType {
    _BARTLETT_HANN_WINDOW   =1,
    _BLACKMAN_HARRIS_WINDOW =2,
    _BLACKMAN_WINDOW        =3,
    _BOHMAN_WINDOW          =4,
    _COSINE_WINDOW          =5,
    _GAUSSIAN_WINDOW        =6,
    _HAMMING_WINDOW         =7,
    _HANN_POISSON_WINDOW    =8,
    _HANN_WINDOW            =9,
    _PARZEN_WINDOW          =10,
    _POISSON_WINDOW         =11,
    _RIEMANN_WINDOW         =12,
    _TRIANGLE_WINDOW        =13,
    _TUKEY_WINDOW           =14,
    _WELCH_WINDOW           =15
};

enum WaveletType {
    _DAUBECHIES_WAVELET             =1,
    _DAUBECHIES_WAVELET_CENTERED    =2,
    _HAAR_WAVELET                   =3,
    _HAAR_WAVELET_CENTERED          =4,
    _BSPLINE_WAVELET                =5,
    _BSPLINE_WAVELET_CENTERED       =6
};

gen filter(const vecteur &args,filter_type typ,GIAC_CONTEXT);
bool is_real_number(const gen &g,GIAC_CONTEXT);
gen to_real_number(const gen &g,GIAC_CONTEXT);
gen generr(const char* msg,bool translate=false);
gen generrtype(const char* msg,bool translate=false);
gen generrdim(const char* msg,bool translate=false);
gen generrarg(int i);
void print_error(const char *msg,GIAC_CONTEXT);
void print_warning(const char *msg,GIAC_CONTEXT);
bool has_rootof(const gen &g);
int dwt(void *data,int datatype,int n,int wtype,int k,int dir);

gen _createwav(const gen &g,GIAC_CONTEXT);
gen _stereo2mono(const gen &g,GIAC_CONTEXT);
gen _plotwav(const gen &g,GIAC_CONTEXT);
gen _plotspectrum(const gen &g,GIAC_CONTEXT);
gen _channels(const gen &g,GIAC_CONTEXT);
gen _bit_depth(const gen &g,GIAC_CONTEXT);
gen _samplerate(const gen &g,GIAC_CONTEXT);
gen _duration(const gen &g,GIAC_CONTEXT);
gen _channel_data(const gen &g,GIAC_CONTEXT);
gen _set_channel_data(const gen &g,GIAC_CONTEXT);
gen _cross_correlation(const gen &g,GIAC_CONTEXT);
gen _auto_correlation(const gen &g,GIAC_CONTEXT);
gen _lowpass(const gen &g,GIAC_CONTEXT);
gen _highpass(const gen &g,GIAC_CONTEXT);
gen _moving_average(const gen &g,GIAC_CONTEXT);
gen _rms(const gen &g,GIAC_CONTEXT);
gen _resample(const gen &g,GIAC_CONTEXT);
gen _convolution(const gen &g,GIAC_CONTEXT);
gen _threshold(const gen &g,GIAC_CONTEXT);
gen _bartlett_hann_window(const gen &g,GIAC_CONTEXT);
gen _blackman_harris_window(const gen &g,GIAC_CONTEXT);
gen _blackman_window(const gen &g,GIAC_CONTEXT);
gen _bohman_window(const gen &g,GIAC_CONTEXT);
gen _cosine_window(const gen &g,GIAC_CONTEXT);
gen _gaussian_window(const gen &g,GIAC_CONTEXT);
gen _hamming_window(const gen &g,GIAC_CONTEXT);
gen _hann_poisson_window(const gen &g,GIAC_CONTEXT);
gen _hann_window(const gen &g,GIAC_CONTEXT);
gen _parzen_window(const gen &g,GIAC_CONTEXT);
gen _poisson_window(const gen &g,GIAC_CONTEXT);
gen _riemann_window(const gen &g,GIAC_CONTEXT);
gen _triangle_window(const gen &g,GIAC_CONTEXT);
gen _tukey_window(const gen &g,GIAC_CONTEXT);
gen _welch_window(const gen &g,GIAC_CONTEXT);
gen _fourier(const gen &g,GIAC_CONTEXT);
gen _ifourier(const gen &g,GIAC_CONTEXT);
gen _addtable(const gen &g,GIAC_CONTEXT);
gen _rect(const gen &g,GIAC_CONTEXT);
gen _boxcar(const gen &g,GIAC_CONTEXT);
gen _tri(const gen &g,GIAC_CONTEXT);
gen _sinc(const gen &g,GIAC_CONTEXT);
gen _logistic(const gen &g,GIAC_CONTEXT);
gen _neural_network(const gen &g,GIAC_CONTEXT);
gen _train(const gen &g,GIAC_CONTEXT);
gen _readwav(const gen &g,GIAC_CONTEXT);
gen _writewav(const gen &g,GIAC_CONTEXT);
gen _soundsec(const gen &g,GIAC_CONTEXT);
gen _playsnd(const gen &g,GIAC_CONTEXT);
gen _splice(const gen &g,GIAC_CONTEXT);
gen _mixdown(const gen &g,GIAC_CONTEXT);
gen _dwt(const gen &g,GIAC_CONTEXT);
gen _idwt(const gen &g,GIAC_CONTEXT);

extern const unary_function_ptr * const at_createwav;
extern const unary_function_ptr * const at_plotwav;
extern const unary_function_ptr * const at_plotspectrum;
extern const unary_function_ptr * const at_channels;
extern const unary_function_ptr * const at_samplerate;
extern const unary_function_ptr * const at_bit_depth;
extern const unary_function_ptr * const at_duration;
extern const unary_function_ptr * const at_channel_data;
extern const unary_function_ptr * const at_set_channel_data;
extern const unary_function_ptr * const at_cross_correlation;
extern const unary_function_ptr * const at_auto_correlation;
extern const unary_function_ptr * const at_lowpass;
extern const unary_function_ptr * const at_highpass;
extern const unary_function_ptr * const at_convolution;
extern const unary_function_ptr * const at_threshold;
extern const unary_function_ptr * const at_bartlett_hann_window;
extern const unary_function_ptr * const at_blackman_harris_window;
extern const unary_function_ptr * const at_blackman_window;
extern const unary_function_ptr * const at_bohman_window;
extern const unary_function_ptr * const at_cosine_window;
extern const unary_function_ptr * const at_gaussian_window;
extern const unary_function_ptr * const at_hamming_window;
extern const unary_function_ptr * const at_hann_poisson_window;
extern const unary_function_ptr * const at_hann_window;
extern const unary_function_ptr * const at_parzen_window;
extern const unary_function_ptr * const at_poisson_window;
extern const unary_function_ptr * const at_riemann_window;
extern const unary_function_ptr * const at_triangle_window;
extern const unary_function_ptr * const at_tukey_window;
extern const unary_function_ptr * const at_welch_window;
extern const unary_function_ptr * const at_fourier;
extern const unary_function_ptr * const at_ifourier;
extern const unary_function_ptr * const at_addtable;
extern const unary_function_ptr * const at_rect;
extern const unary_function_ptr * const at_boxcar;
extern const unary_function_ptr * const at_tri;
extern const unary_function_ptr * const at_sinc;
extern const unary_function_ptr * const at_logistic;
extern const unary_function_ptr * const at_neural_network;
extern const unary_function_ptr * const at_train;
extern const unary_function_ptr * const at_readwav;
extern const unary_function_ptr * const at_writewav;
extern const unary_function_ptr * const at_soundsec;
extern const unary_function_ptr * const at_playsnd;
extern const unary_function_ptr * const at_splice;
extern const unary_function_ptr * const at_mixdown;
extern const unary_function_ptr * const at_dwt;
extern const unary_function_ptr * const at_idwt;

class audio_clip : public gen_user {
    const context *ctx;
    int _bit_depth,_sample_rate,_nc,_range,_pow2,_len,_size,_file_offset;
    std::string _filename;
    char *_data;
    void assign(const audio_clip &other,int offset=0,int len=rand_max2);
    inline double decode(int sample) const;
    inline int encode(double sample) const;
    void read(FILE *f,int offset,int len,bool read_hdr);
    static void read_header(FILE *f,int &bd,int &sr,int &nc,int &sz);
    bool write(FILE *f) const;
public:
    audio_clip(int depth,int rate,int nc,int len,GIAC_CONTEXT);
    audio_clip(const audio_clip &other,int offset=0,int len=rand_max2);
    audio_clip(const char *fname,int offset,int len,GIAC_CONTEXT);
    audio_clip(const char *fname,double t1,double t2,GIAC_CONTEXT);
    audio_clip &operator=(const audio_clip &other) { assign(other); return *this; }
    static audio_clip *from_gen(const gen &g);
    // reimplemented from gen_user
    virtual ~audio_clip() { free(_data); }
    virtual std::string print(GIAC_CONTEXT) const;
    virtual std::string texprint(GIAC_CONTEXT) const;
    virtual gen giac_constructor(GIAC_CONTEXT) const;
    virtual bool operator==(const gen &) const;
    virtual gen_user *memory_alloc() const { return new audio_clip(*this); }
    virtual gen operator()(const gen &,GIAC_CONTEXT) const;
    virtual gen operator[](const gen &);
    virtual gen operator+(const gen &) const; // concatenation of audio clips
    virtual bool is_zero() const { return false; }
    virtual bool is_one() const { return false; }
    virtual bool is_minus_one() const { return false; }
    virtual gen inv() const;
    virtual gen operator -() const { return this->inv(); }
    void load_data(const std::string &s,int offset=0);
    inline void set_sample(int c,int pos,unsigned u);
    inline unsigned get_sample(int c,int pos) const;
    void set_channel_data(int chn,const vecteur &data,int offset=0,double ratio=1.0);
    vecteur get_channel_data(int chn,int pos=0,int len=rand_max2) const;
    void set_block(const matrice &data,int offset=0);
    matrice get_block(int pos,int len) const;
    matrice get_data() const { return get_block(0,rand_max2); }
    const int &channel_count() const { return _nc; }
    const int &bit_depth() const { return _bit_depth; }
    const int &sample_rate() const { return _sample_rate; }
    const int &length() const { return _len; } // wave length in samples
    const std::string &file_name() const { return _filename; }
    double duration() const { return double(_len)/double(_sample_rate); } // duration in seconds
    int play(int pos=0,int len=rand_max2,int repeats=1) const;
    bool write_wav(const char *fname) const;
    audio_clip resample(int nsr,int quality) const;
    bool get_chunk_span(int c,int offset,int len,double &fmin,double &fmax) const;
    audio_clip reverse() const;
    audio_clip to_mono() const;
    audio_clip splice(const audio_clip &other,int pos,double ex=-1) const;
    void normalize(double dbfs);
    double peak() const;
    double peak_dbfs() const { return 20.0*std::log10(peak()); }
    double rms(int offset=0,int len=rand_max2) const;
    double similarity(const audio_clip &other,int offset=0,int other_offset=0,int len=rand_max2) const;
    void pad_zeros(int len);
    bool mix(const audio_clip &other,int offset=0,double gain=0.0,double pan=0.0,int fade_in=0,int fade_out=0);
};

class rgba_image : public gen_user {
    const context *ctx;
    std::string _filename;
    bool _tmp;
    int _d,_w,_h;
    uchar *_data;
    void assign(const rgba_image &other,int x=0,int y=0,int w=0,int h=0);
    void allocate();
    void clear();
public:
    rgba_image(int d,int w,int h,GIAC_CONTEXT);
    rgba_image(const std::vector<const gen*> &data,GIAC_CONTEXT);
    rgba_image(int d,int w,const std::string &data,GIAC_CONTEXT);
    rgba_image(const rgba_image &other,int x=0,int y=0,int w=0,int h=0);
    rgba_image &operator=(const rgba_image &other) { assign(other); return *this; }
    static rgba_image *from_gen(const gen &g);
    // reimplemented from gen_user
    virtual ~rgba_image();
    virtual std::string print(GIAC_CONTEXT) const;
    virtual std::string texprint(GIAC_CONTEXT) const;
    virtual gen giac_constructor(GIAC_CONTEXT) const;
    virtual bool operator==(const gen &) const;
    virtual gen_user *memory_alloc() const { return new rgba_image(*this); }
    virtual gen operator()(const gen &,GIAC_CONTEXT) const;
    virtual gen operator[](const gen &);
    virtual bool is_zero() const { return false; }
    virtual bool is_one() const { return false; }
    virtual bool is_minus_one() const { return false; }
    virtual gen operator -() const { return to_negative(); }
    virtual gen inv() const { return -(*this);}
    int width() const { return _w; }
    int height() const { return _h; }
    int depth() const { return _d; }
    bool has_alpha() const { return _d%2==0; }
    const std::string file_name() const { return _filename; }
    const uchar *data_array() const { return _data; }
    void set_file_name(const char *fname) { _filename=std::string(fname); _tmp=false; }
    bool is_original() const { return !_filename.empty() && !_tmp; }
    uchar get_pixel(int chn,int x,int y) const;
    void set_pixel(int chn,int x,int y,uchar v);
    int write_png(const char *fname) const;
    matrice get_channel_data(int chn,int x=0,int y=0,int w=0,int h=0) const;
    void set_channel_data(int chn,const matrice &data,int x=0,int y=0);
    rgba_image to_grayscale() const;
    rgba_image to_negative() const;
    void flatten(vecteur &res) const;
    std::string color_type_string() const;
    bool assure_on_disk();
};
extern gen (*load_image_ptr)(const char *fname,GIAC_CONTEXT);
extern gen (*resize_image_ptr)(const rgba_image &img,int w,int h,GIAC_CONTEXT);

class tempimagefilename {
    std::stack<std::string> _fnames;
public:
    tempimagefilename() { }
    ~tempimagefilename();
    const std::string &make();
    void pop() { if (!_fnames.empty()) _fnames.pop(); }
};
extern tempimagefilename temp_image_file_name;

/* Neural network class */
#ifdef HAVE_LIBGSL
class ann : public gen_user {
    std::vector<int> topology;
    std::vector<gsl_matrix*> layers;
    std::vector<gsl_matrix*> cache;
    std::vector<gsl_matrix*> deltas;
    std::vector<gsl_matrix*> weights;
    std::vector<gsl_matrix*> Deltas;
    std::vector<gsl_matrix*> moment1;
    std::vector<gsl_matrix*> moment2;
    const context *ctx;
    int block_size,iter,accum_count;
    double learning_rate,eps,momentum,beta1,beta2;
    gen x,schedule;
    vecteur y,t,fan,labels,activation,output_activation,errfunc;
    bool _is_classifier;
    gsl_vector *reg_coeff;
    gsl_matrix *eout;
    std::string title;
    void create();
    void assign(const ann &other,int bs);
    void deallocate();
    void propagate_forward(bool comp_deriv=false) const;
    void propagate_backward();
    void calc_deltas() const;
    void update_Deltas();
    void update_weights();
    void activate(int i,double &a,double *b) const;
    gen compute_errfunc(gsl_vector *t,gsl_vector *y) const;
    void compute_errfunc_diff(gsl_matrix *t,gsl_matrix *y,gsl_matrix *res) const;
    void ckinput(const matrice &input,const vecteur &output,bool ignore_output) const;
    void output2gsl_matrix(const vecteur &output,int i0,int n,gsl_matrix *m) const;
    gen gsl_vector2output(gsl_vector *output) const;
    void feed(const matrice &input,vecteur &res,const matrice &expected_output=vecteur(0)) const;
    std::string network_type() const;
    void weights2vecteur(vecteur &res) const;
public:
    ann(const std::vector<int> &topology,int bsize,GIAC_CONTEXT);
    ann(const ann &other);
    ann(const ann &other,int bs);
    ann &operator=(const ann &other);
    static ann *from_gen(const gen &g);
    // reimplemented from gen_user
    virtual ~ann() { deallocate(); }
    virtual std::string print (GIAC_CONTEXT) const;
    virtual std::string texprint (GIAC_CONTEXT) const;
    virtual gen giac_constructor (GIAC_CONTEXT) const;
    virtual bool operator ==(const gen &) const;
    virtual gen_user *memory_alloc() const { return new ann(*this); }
    virtual gen operator()(const gen &,GIAC_CONTEXT) const;
    virtual gen operator[](const gen &);
    virtual bool is_zero() const { return false; }
    virtual bool is_one() const { return false; }
    virtual bool is_minus_one() const { return false; }
    // methods
    gsl_matrix *const layer(int i) const { assert(i>=0&&i<layer_count()); return layers[i]; }
    gsl_matrix *const weight_matrix(int i) const { assert(i>=0&&i<layer_count()-1); return weights[i]; }
    gsl_matrix *const weight_updates(int i) const { assert(i>=0&&i<layer_count()-1); return Deltas[i]; }
    vecteur layout() const { return vector_int_2_vecteur(topology); }
    int layer_count() const { return topology.size(); }
    int layer_size(int i) const { assert(i>=0&&i<layer_count()); return topology[i]; }
    int input_size() const { return layer_size(0); }
    int output_size() const { return layer_size(layer_count()-1); }
    int hidden_layer_count() const { return layer_count()-2; }
    int neuron_count() const;
    const std::string &name() const { return title; }
    bool is_classifier() const { return _is_classifier; }
    bool is_default_classifier() const;
    void set_error_function(const gen &f,const vecteur &params=vecteur(0));
    void set_activation(bool out,const gen &f,const vecteur &params=vecteur(0));
    void set_regularization(const vecteur &reg) const;
    void randomize_initial_weights(const gen &f,const vecteur &fan_in_out) const;
    void set_initial_weights(const matrice &w,int i=0) const;
    void set_labels(const vecteur &lab,bool classes=false);
    void set_name(const std::string &name) { title=name; }
    void set_learning_rate(double lr,const gen &sched) { learning_rate=lr; schedule=sched; }
    void set_momentum(double mom);
    void set_adam_params(double b1,double b2) { beta1=b1; beta2=b2; }
    void restart_iteration();
    int task() const { return labels.empty()?0:(_is_classifier?1:2); }
    const gen &label(int i) const { assert(i>=0&&i<topology.back()); return labels[i]; }
    void train(const matrice &input,const vecteur &expected_output,int batch_size);
};
#endif

#ifndef NO_NAMESPACE_GIAC
} // namespace giac
#endif // ndef NO_NAMESPACE_GIAC
#endif // __SIGNALPROCESSING_H
