#ifndef __LPSOLVE_H
#define __LPSOLVE_H

#include "config.h"
#include "gen.h"
#include "unary.h"
#ifdef HAVE_LIBGLPK
#include <glpk.h>
#endif

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif //ndef NO_NAMESPACE_GIAC

#define LP_SCORE_FACTOR 0.1667
#define LP_MIN_AWAY 0.08
#define LP_MIN_PARALLELISM 0.86
#define LP_MAX_MAGNITUDE 1e6
#define LP_CONSTR_MAXSIZE 1e5

typedef std::vector<int> ints;

enum lp_results {
    _LP_SOLVED,
    _LP_INFEASIBLE,
    _LP_UNBOUNDED,
    _LP_ERROR
};

enum lp_parsing_errors {
    _LP_ERR_SIZE = 1,
    _LP_ERR_TYPE = 2,
    _LP_ERR_DIM  = 3
};

enum lp_precision_types {
    _LP_EXACT,
    _LP_INEXACT,
    _LP_PROB_DEPENDENT
};

enum lp_relation_types {
    _LP_LEQ = -1,
    _LP_EQ  =  0,
    _LP_GEQ =  1
};

enum lp_variable_sign_types {
    _LP_VARSIGN_POS,
    _LP_VARSIGN_NEG,
    _LP_VARSIGN_POS_PART,
    _LP_VARSIGN_NEG_PART
};

struct lp_settings {
    //solver parameters
    int solver;
    int precision;
    bool presolve;
    bool maximize;
    int assumption;
    int iteration_limit;
    bool has_binary_vars;
    //branch&bound parameters
    int varselect;
    int nodeselect;
    double relative_gap_tolerance;
    int depth_limit;
    int node_limit;
    int time_limit; //in miliseconds
    int max_cuts;
    //message report parameters
    bool verbose;
    double status_report_freq;
    lp_settings();
};

struct lp_stats {
    int subproblems_examined;
    int cuts_applied;
    int max_active_nodes;
    double mip_gap;
    lp_stats();
};

class lp_range {
    gen lbound;
    gen ubound;
    void assign(const lp_range &other) { lbound=other.lb(); ubound=other.ub(); }
public:
    lp_range();
    lp_range(const lp_range &other) { assign(other); }
    ~lp_range() { }
    lp_range &operator =(const lp_range &other) { assign(other); return *this; }
    const gen &lb() const { return lbound; }
    const gen &ub() const { return ubound; }
    void set_lb(const gen &L) { lbound=L; }
    void set_ub(const gen &U) { ubound=U; }
    void tighten_lbound(const gen &l,GIAC_CONTEXT) { if (is_strictly_greater(l,lbound,contextptr)) lbound=l; }
    void tighten_ubound(const gen &u,GIAC_CONTEXT) { if (is_strictly_greater(ubound,u,contextptr)) ubound=u; }
    bool is_unrestricted_below() const { return is_inf(lbound); }
    bool is_unrestricted_above() const { return is_inf(ubound); }
};

class lp_variable {
    bool _is_integral;
    int _sign_type;
    lp_range _range;
    std::string _name;
    double pseudocost[2];
    int nbranch[2];
    void assign(const lp_variable &other);
public:
    lp_variable();
    lp_variable(const lp_variable &other) { assign(other); }
    ~lp_variable() { }
    lp_variable &operator =(const lp_variable &other) { assign(other); return *this; }
    bool is_integral() const { return _is_integral; }
    void set_integral(bool yes) { _is_integral = yes; }
    int sign_type() const { return _sign_type; }
    void set_sign_type(int type) { _sign_type=type; }
    const lp_range &range() const { return _range; }
    const std::string &name() const { return _name; }
    void set_name(const std::string &s) { _name=s; }
    void set_type(int t,GIAC_CONTEXT);
    void set_lb(const gen &L) { _range.set_lb(L); }
    void set_ub(const gen &U) { _range.set_ub(U); }
    void tighten_lbound(const gen &L,GIAC_CONTEXT) { _range.tighten_lbound(L,contextptr); }
    void tighten_ubound(const gen &U,GIAC_CONTEXT) { _range.tighten_ubound(U,contextptr); }
    void update_pseudocost(double delta,double fr,int dir);
    double score(double fr) const;
};

struct lp_constraints {
    matrice lhs;
    vecteur rhs;
    ints rv;
    std::vector<double> score;
    int nrows() { return lhs.size(); }
    int ncols() { return lhs.empty()?0:lhs.front()._VECTptr->size(); }
    void append(const vecteur &lh,const gen &rh,int relation_type);
    vecteur column(int index);
    void duplicate_column(int index);
    void negate_column(int index);
    void subtract_from_rhs_column(const vecteur &v);
    void set(int index,const vecteur &lh,const gen &rh,int relation_type);
    void get(int index,vecteur &lh,gen &rh,int &relation_type);
    void get_lr(int index,vecteur &lh,gen &rh);
    void div(int index,const gen &g,GIAC_CONTEXT);
    void subtract(int index,const vecteur &v,const gen &g);
    void remove(int index);
};

class lp_node;

struct lp_problem {
    const context *ctx;
    std::pair<vecteur,gen> objective;
    double objective_norm;
    std::vector<double> obj_approx;
    std::vector<lp_variable> variables;
    vecteur variable_identifiers;
    lp_constraints constr;
    lp_constraints cuts;
    lp_settings settings;
    lp_stats stats;
    int nvars_initial;
    vecteur solution;
    gen optimum;
    lp_problem(GIAC_CONTEXT) {
        ctx=contextptr;
        settings=lp_settings();
    }
    int nc() { return constr.lhs.size(); }
    int nv() { return variables.size(); }
    void message(const char* msg,bool err=false);
    void report_status(const char* msg,int count);
    void add_identifiers_from(const gen &g);
    int get_variable_index(const identificateur &idnt);
    void set_objective(const vecteur &v,const gen &ft);
    void create_variables(int n);
    void make_problem_exact();
    void add_slack_variables();
    void tighten_variable_bounds(int i,const gen &l,const gen &u);
    void make_all_vars_bounded_below();
    bool has_integral_variables();
    bool has_approx_coefficients();
    bool lincomb_coeff(const gen &g,vecteur &varcoeffs,gen &freecoeff);
    int solve();
    vecteur output_solution();
    //GLPK routines
#ifdef HAVE_LIBGLPK
    glp_prob *glpk_initialize();
    int glpk_simplex(glp_prob *prob);
    int glpk_interior_point(glp_prob *prob);
    int glpk_branchcut(glp_prob *prob);
#endif
    int glpk_solve();
    bool glpk_load_from_file(const char *fname);
};

class lp_node {
    lp_problem *prob;
    int depth;
    std::vector<lp_range> ranges;
    gen optimum;
    vecteur solution;
    double opt_approx;
    gen infeas;
    int most_fractional;
    std::map<int,double> fractional_vars;
    ints cut_indices;
    void assign(const lp_node &other);
    gen fracpart(const gen &g) const;
public:
    lp_node() { }
    lp_node(const lp_node &other) { assign(other); }
    ~lp_node() { }
    lp_node &operator =(const lp_node &other) { assign(other); return *this; }
    lp_problem *get_prob() const { return prob; }
    void set_prob(lp_problem *p) { prob=p; }
    void resize_ranges(size_t s) { ranges.resize(s); }
    const std::vector<lp_range> &get_ranges() const { return ranges; }
    lp_range &get_range(int i) { return ranges[i]; }
    int get_depth() const { return depth; }
    void set_depth(int d) { depth=d; }
    const vecteur &get_solution() const { return solution; }
    const gen &get_optimum() const { return optimum; }
    double get_opt_approx() const { return opt_approx; }
    const gen &get_infeas() const { return infeas; }
    bool is_integer_feasible() const { return is_zero(infeas); }
    bool is_var_fractional(int index) const;
    double get_fractional_var(int index) const;
    int get_most_fractional() const { return most_fractional; }
    int get_first_fractional_var() const { return fractional_vars.begin()->first; }
    int get_last_fractional_var() const { return fractional_vars.rbegin()->first; }
    lp_node create_child();
    int solve_relaxation();
};

gen _lpsolve(const gen &args,GIAC_CONTEXT);
extern const unary_function_ptr * const  at_lpsolve;

#ifndef NO_NAMESPACE_GIAC
} //namespace giac
#endif //ndef NO_NAMESPACE_GIAC
#endif //__LPSOLVE_H
