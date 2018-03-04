/*
 * graphe.h
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

#ifndef __GRAPHE_H
#define __GRAPHE_H
#include <giac/config.h>
#include <giac/gen.h>
#include <giac/unary.h>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <queue>
#include <stack>

//#define _GLIBCXX_USE_CXX11_ABI 0

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

enum gt_dot_token_types {
    _GT_DOT_TOKEN_TYPE_IDENTIFIER = 1,
    _GT_DOT_TOKEN_TYPE_NUMBER = 2,
    _GT_DOT_TOKEN_TYPE_OPERATOR = 3,
    _GT_DOT_TOKEN_TYPE_STRING = 4,
    _GT_DOT_TOKEN_TYPE_DELIMITER = 5
};

enum gt_creation_options {
    _GT_OPT_DIRECTED,
    _GT_OPT_WEIGHTED,
    _GT_OPT_VERTEX_COLOR,
    _GT_OPT_VERTEX_POSITIONS,
    _GT_OPT_WEIGHTS
};

enum gt_attributes {
    _GT_ATTRIB_WEIGHT,
    _GT_ATTRIB_COLOR,
    _GT_ATTRIB_DIRECTED,
    _GT_ATTRIB_WEIGHTED,
    //add more here
    _GT_ATTRIB_USER
};

enum gt_error_codes {
    _GT_ERR_UNKNOWN = 0,
    _GT_ERR_NOT_A_GRAPH = 1,
    _GT_ERR_WEIGHTED_GRAPH_REQUIRED = 2,
    _GT_ERR_UNWEIGHTED_GRAPH_REQUIRED = 3,
    _GT_ERR_DIRECTED_GRAPH_REQUIRED = 4,
    _GT_ERR_UNDIRECTED_GRAPH_REQUIRED = 5,
    _GT_ERR_INVALID_EDGE = 6,
    _GT_ERR_INVALID_EDGE_ARC_MIX = 7,
    _GT_ERR_MATRIX_NOT_SYMMETRIC = 8,
    _GT_ERR_READING_FAILED = 9,
    _GT_ERR_EDGE_NOT_FOUND = 10,
    _GT_ERR_VERTEX_NOT_FOUND = 11,
};

class graphe {
public:
    typedef std::vector<int> ivector;
    typedef std::map<int,gen> attrib;
    typedef std::pair<int,int> ipair;
    typedef std::vector<double> rvec;
    typedef std::vector<rvec> layout;
    typedef std::map<int,std::map<int,double> > sparsemat;
    struct vertex {
        gen symbol;
        int subgraph;
        attrib attributes;
        std::map<int,attrib> neighbors;
        vertex(const gen &symb) { symbol=symb; subgraph=-1; }
    };
    typedef std::vector<vertex>::const_iterator node_iter;
    typedef std::map<int,attrib>::const_iterator neighbor_iter;
    typedef attrib::const_iterator attrib_iter;
    struct dotgraph {
        int index;
        attrib vertex_attributes;
        attrib edge_attributes;
        attrib chain_attributes;
        ivector chain;
        int pos;
        dotgraph(int i) { index=i; pos=0; chain=ivector(1,0); }
        void incr() { ++pos; if (int(chain.size())<=pos) chain.resize(pos+1,0); }
        void set(int i) { chain[pos]=i; }
        void clear_chain() { pos=0; chain.resize(1); chain.front()=0; chain_attributes.clear(); }
        bool chain_completed() { return chain.back()!=0; }
        bool chain_empty() { return pos==0 && chain.front()==0; }
    };
    class edmonds {
        std::map<int,ivector> blossoms;
        std::map<int,int> forest;
        graphe *G;
        int mate(const std::vector<ipair> &matching,int v);
        int find_root(int v);
        int root_distance(std::map<int,int>::const_iterator it);
        int root_distance(int v);
        int find_base(int v,int w);
        bool tree_path(int v,int w,ivector &path);
        std::map<int,ivector>::iterator in_blossom(int v);
        std::map<int,ivector>::iterator is_blossom_base(int v);
        void append_non_blossom_adjacents(int v, std::map<int,ivector>::const_iterator bit, ivector &lst);
        ivector adjacent(int v);
        ipair make_edge(int i,int j) { return std::make_pair(i<j?i:j,i<j?j:i); }
    public:
        edmonds(graphe *gr);
        bool find_augmenting_path(const std::vector<ipair> &matching,ivector &path);
        void find_maximum_matching(std::vector<ipair> &matching);
    };
    static const gen FAUX;
    static const gen VRAI;
private:
    std::vector<vertex> nodes;
    std::string graph_name;
    attrib attributes;
    std::vector<std::string> user_tags;
    static std::default_random_engine random_generator;
    static std::normal_distribution<double> normal_distribution;
    static std::uniform_real_distribution<double> uniform_distribution;
    bool dot_parse_attributes(std::ifstream &dotfile,attrib &attr);
    static bool insert_attribute(attrib &attr,int key,const gen &val,bool overwrite=true);
    static bool remove_attribute(attrib &attr,int key);
    static bool genmap2attrib(const gen_map &m,attrib &attr);
    static void attrib2genmap(const attrib &attr,gen_map &m);
    static attrib copy_attributes(const attrib &attr);
    void write_attrib(std::ofstream &dotfile,const attrib &attr) const;
    static void add_rvec(rvec &a,const rvec &b);
    static void subtract_rvec(rvec &a,const rvec &b);
    static void scale_rvec(rvec &p,double s);
    static void clear_rvec_coords(rvec &p);
    static double rvec_norm(const rvec &p,bool sqroot=true);
    static void copy_layout(const layout &src,layout &dest);
    static void copy_rvec(const rvec &src,rvec &dest);
    static void rand_rvec(rvec &p,double radius=1.0);
    static void rotate_layout(layout &x, double phi);
    static void translate_layout(layout &x,const rvec &dx);
    static double layout_min(const layout &x,int d);
    static void make_rvec_polar(rvec &p);
    int count_node_label_obstacles(const layout &x,int i,int quadrant) const;
    static bool sparse_matrix_element(const sparsemat &A, int i, int j, double &val);
    static void multiply_sparse_matrices(const sparsemat &A, const sparsemat &B, sparsemat &prod);
    static void transpose_sparsemat(const sparsemat &A,sparsemat &transp);
    static void print_sparsemat(const sparsemat &A);
    static void multilevel_recursion(layout &x, graphe &G, int d, double R, double K, double tol, int depth=0);
    int mdeg(const ivector &V,int i) const;
    void coarsening(graphe &G, const sparsemat &P, const ivector &V) const;
    void tomita_recurse(const ivector &R, const ivector &P_orig, const ivector &X_orig, std::vector<ivector> &cliques) const;
    void remove_isolated_node(int i);
    void make_connected_component(int i,int ci,std::map<int,int> &indices) const;
    void find_cut_vertices_dfs(int u,int &t,ivector &disc,ivector &low, std::vector<bool> &ap) const;
    void find_blocks_dfs(int v,int &i,ivector &dfi, ivector &p,std::stack<ipair> &s,
                         std::map<int,ivector> &stacked,std::vector<std::vector<ipair> > &blocks) const;
    int find_cycle_dfs(int v, std::stack<int> &s, std::vector<bool> &visited, ivector &parent) const;
public:
    graphe();
    graphe(const graphe &G);
    static gen make_idnt(const char* name,int index=-1,bool intern=true);
    static void make_default_vertex_labels(vecteur &V,int n,int n0,GIAC_CONTEXT);
    static gen boole(bool b) { return b?VRAI:FAUX; }
    static gen str2gen(const std::string &str,bool isstring=false);
    static std::string genstring2str(const gen &g);
    static std::string gen2str(const gen &g);
    bool read_gen(const vecteur &v,GIAC_CONTEXT);
    void copy(graphe &G) const;
    int tag2index(const std::string &tag);
    std::string index2tag(int index) const;
    int register_user_tag(const std::string &tag);
    void register_user_tags(const std::vector<std::string> &tags);
    gen to_gen() const;
    bool write_dot(const std::string &filename) const;
    bool read_dot(const std::string &filename);
    bool is_empty() const { return nodes.empty(); }
    matrice weight_matrix() const;
    gen weight(int i, int j) const;
    int edge_count() const;
    int node_count() const { return nodes.size(); }
    vecteur vertices() const;
    ivector adjacent_nodes(int i) const;
    vecteur edges(bool include_weights) const;
    int add_node(const gen &v);
    int add_node(const gen &v,const attrib &attr) { int i=add_node(v); nodes[i].attributes=attr; return i; }
    void add_nodes(const vecteur &v);
    bool remove_node(int i);
    bool remove_node(const gen &v);
    void remove_nodes(const vecteur &v);
    gen node(int i) const { assert(i>=0 && i<node_count()); return nodes[i].symbol; }
    int node_index(const gen &v) const;
    const attrib &node_attributes(int i) const { assert(i>=0 && i<node_count()); return nodes[i].attributes; }
    const attrib &edge_attributes(int i,int j) const;
    attrib &edge_attributes(int i,int j);
    bool add_edge(int i,int j,const gen &w=gen(1));
    bool add_edge(int i,int j,const attrib &attr);
    bool add_edge(const ipair &edge) { return add_edge(edge.first,edge.second); }
    bool add_edge(const ipair &edge,const attrib &attr) { return add_edge(edge.first,edge.second,attr); }
    ipair add_edge(const gen &v,const gen &w,const gen &weight=gen(1));
    bool remove_edge(int i,int j);
    void make_cycle(const vecteur &v);
    bool has_edge(int i, int j) const;
    bool has_edge(ipair p) const { return has_edge(p.first,p.second); }
    ipair make_edge(int i,int j) const;
    ipair make_edge(const vecteur &v) const;
    int in_degree(int index) const;
    int out_degree(int index) const;
    int degree(int index) const;
    matrice adjacency_matrix() const;
    matrice incidence_matrix() const;
    void set_graph_attribute(int key,const gen &val) { attributes[key]=val; }
    void set_graph_attributes(const attrib &attr) { attributes=copy_attributes(attr); }
    void set_node_attribute(int index, int key, const gen &val);
    void set_edge_attribute(int i, int j, int key, const gen &val);
    bool get_graph_attribute(int key, gen &val) const;
    bool get_node_attribute(int index, int key, gen &val) const;
    bool get_edge_attribute(int i, int j, int key, gen &val) const;
    void set_name(const std::string &str) { graph_name=str; }
    std::string name() const { return graph_name; }
    bool is_directed() const;
    bool is_weighted() const;
    void set_directed(bool yes) { set_graph_attribute(_GT_ATTRIB_DIRECTED,boole(yes)); }
    void make_weighted(const matrice &m);
    void make_directed();
    void strip_weights();
    void make_underlying();
    bool permute_nodes(const vecteur &sigma);
    bool relabel_nodes(const vecteur &labels);
    void make_induced_subgraph(const ivector &vi,graphe &G) const;
    ivector maximal_independent_set() const;
    void maximize_matching(std::vector<ipair> &matching);
    std::vector<ipair> find_maximal_matching() const;
    bool trail(const vecteur &v);
    void color_node(int index, int c) { set_node_attribute(index,_GT_ATTRIB_COLOR,c); }
    void color_nodes(const gen &c);
    bool parse_list_of_edges(const vecteur &v,GIAC_CONTEXT);
    bool parse_matrix(const matrice &m,bool iswei,int mode,GIAC_CONTEXT);
    void parse_trail(const vecteur &v);
    void force_directed(layout &x,double K,double R,double tol=0.001,bool ac=true);
    void multilevel_force_directed(layout &x, int d, double K, double tol=0.001);
    void coarsening_mis(const ivector &V,graphe &G,sparsemat &P) const;
    void coarsening_ec(const std::vector<ipair> &M, graphe &G, sparsemat &P) const;
    static void make_layout_unique(layout &x, double K);
    static void create_random_layout(layout &x, double K, int d);
    int node_label_best_quadrant(const layout &x,const rvec &center,int i);
    static gen rvec2gen(const rvec &p, bool pt=false);
    static rvec layout_center(const layout &x);
    static void scale_layout(layout &x,double diam);
    void tomita(std::vector<ivector> &cliques) const;
    void make_sierpinski_graph(int n, int k, bool triangle,GIAC_CONTEXT);
    void make_complete_graph(const vecteur &V);
    void make_complete_multipartite_graph(const ivector &partition_sizes,GIAC_CONTEXT);
    void make_petersen_graph(int n, int k,GIAC_CONTEXT);
    void connected_components(std::vector<ivector> &components) const;
    ivector find_cut_vertices() const;
    void find_blocks(std::vector<std::vector<ipair> > &blocks) const;
    ivector find_cycle() const;
    void collapse_edge(int i, int j);
    std::vector<ipair> incident_edges(const ivector &v);
};

#ifndef NO_NAMESPACE_GIAC
} // namespace giac
#endif // ndef NO_NAMESPACE_GIAC
#endif // __GRAPHE_H
