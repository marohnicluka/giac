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
#ifndef DBL_MAX
#define DBL_MAX 1.79769313486e+308
#endif

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

enum gt_dot_token_type {
    _GT_DOT_TOKEN_TYPE_IDENTIFIER = 1,
    _GT_DOT_TOKEN_TYPE_NUMBER = 2,
    _GT_DOT_TOKEN_TYPE_OPERATOR = 3,
    _GT_DOT_TOKEN_TYPE_STRING = 4,
    _GT_DOT_TOKEN_TYPE_DELIMITER = 5
};

enum gt_creation_option {
    _GT_OPT_DIRECTED,
    _GT_OPT_WEIGHTED,
    _GT_OPT_VERTEX_COLOR,
    _GT_OPT_VERTEX_POSITIONS,
    _GT_OPT_WEIGHTS
};

enum gt_attribute {
    _GT_ATTRIB_WEIGHT,
    _GT_ATTRIB_COLOR,
    _GT_ATTRIB_DIRECTED,
    _GT_ATTRIB_WEIGHTED,
    _GT_ATTRIB_POSITION,
    //add more here
    _GT_ATTRIB_USER
};

enum gt_layout_style {
    _GT_STYLE_SPRING,
    _GT_STYLE_PLANAR,
    _GT_STYLE_3D,
    _GT_STYLE_CIRCLE
};

enum gt_error_code {
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
    typedef std::vector<ivector> ivectors;
    typedef std::map<int,gen> attrib;
    typedef std::pair<int,int> ipair;
    typedef std::vector<ipair> ipairs;
    typedef std::pair<double,double> dpair;
    typedef std::vector<dpair> dpairs;
    typedef std::vector<double> point;
    typedef std::vector<point> layout;
    typedef std::map<int,std::map<int,double> > sparsemat;
    class vertex {
        gen m_symbol;
        int m_subgraph;
        attrib m_attributes;
        std::map<int,attrib> m_neighbors;
    public:
        vertex();
        vertex(const vertex &v);
        vertex& operator =(const vertex &other);
        const gen &symbol() const { return m_symbol; }
        void set_symbol(const gen &s) { m_symbol=s; }
        int subgraph() const { return m_subgraph; }
        void set_subgraph(int s) { m_subgraph=s; }
        const attrib &attributes() const{ return m_attributes; }
        attrib &attributes() { return m_attributes; }
        void set_attribute(int key,const gen &val) { m_attributes[key]=val; }
        void set_attributes(const attrib &attr) { copy_attributes(attr,m_attributes); }
        const std::map<int,attrib> &neighbors() const { return m_neighbors; }
        void add_neighbor(int i,const attrib &attr=attrib()) { copy_attributes(attr,m_neighbors[i]); }
        attrib &neighbor_attributes(int i);
        const attrib &neighbor_attributes(int i) const;
        bool has_neighbor(int i) const { return m_neighbors.find(i)!=m_neighbors.end(); }
        void remove_neighbor(int i) { m_neighbors.erase(m_neighbors.find(i)); }
        void clear_neighbors() { m_neighbors.clear(); }
    };
    class dotgraph {
        int m_index;
        attrib vertex_attr;
        attrib edge_attr;
        attrib chain_attr;
        ivector m_chain;
        int pos;
    public:
        dotgraph();
        dotgraph(const dotgraph &other);
        dotgraph(int i);
        dotgraph& operator =(const dotgraph &other);
        int index() const { return m_index; }
        void set_index(int i) { m_chain[pos]=i; }
        const attrib &vertex_attributes() const { return vertex_attr; }
        const attrib &edge_attributes() const { return edge_attr; }
        const attrib &chain_attributes() const { return chain_attr; }
        attrib &vertex_attributes() { return vertex_attr; }
        attrib &edge_attributes() { return edge_attr; }
        attrib &chain_attributes() { return chain_attr; }
        const ivector &chain() const { return m_chain; }
        ivector &chain() { return m_chain; }
        int position() const { return pos; }
        void incr() { ++pos; if (int(m_chain.size())<=pos) m_chain.resize(pos+1,0); }
        void clear_chain() { pos=0; m_chain.resize(1); m_chain.front()=0; chain_attr.clear(); }
        bool chain_completed() { return m_chain.back()!=0; }
        bool chain_empty() { return pos==0 && m_chain.front()==0; }
    };
    class edmonds { // implementation of the blossom algorithm
        graphe *G;
        std::map<int,ivector> blossoms;
        std::map<int,int> forest;
        int mate(const ipairs &matching,int v);
        int find_root(int k);
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
        bool find_augmenting_path(const ipairs &matching,ivector &path);
        void find_maximum_matching(ipairs &matching);
    };
    class treeshaper {
        graphe *G;
        ivector ancestor;
        std::map<int,ivector> level;
        std::vector<double> modifier;
        std::vector<double> prelim;
        std::vector<bool> visited;
        double hsep;
        double vsep;
        double t;
        void layout_level(const ivector &V);
        void walk(layout &x,int v,int lev,double modsum);
        int max_depth(int node,int depth,int incumbent_depth);
        int best_apex();
        void make_all_unvisited() { std::fill(visited.begin(),visited.end(),false); }
        void sort_nodes_by_levels(int v,int lev);
    public:
        treeshaper(graphe *g);
        void make_layout(layout &x,double sep,int apex=0);
        double seconds_elapsed() { return t; }
    };
    class rectangle {
        double m_x;
        double m_y;
        double m_width;
        double m_height;
    public:
        rectangle();
        rectangle(double X,double Y,double W,double H);
        rectangle(const rectangle &rect);
        rectangle& operator =(const rectangle &other);
        double x() const { return m_x; }
        double y() const { return m_y; }
        double width() const { return m_width; }
        double height() const { return m_height; }
    };
    typedef std::vector<vertex>::const_iterator node_iter;
    typedef std::map<int,attrib>::const_iterator neighbor_iter;
    typedef attrib::const_iterator attrib_iter;
    typedef std::vector<point>::const_iterator layout_iter;
    typedef ivector::const_iterator ivector_iter;
    typedef ivectors::const_iterator ivectors_iter;
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
    static void copy_attributes(const attrib &src,attrib &dest);
    void write_attrib(std::ofstream &dotfile,const attrib &attr) const;
    static void add_point(point &a,const point &b);
    static void subtract_point(point &a,const point &b);
    static void scale_point(point &p,double s);
    static double point_vecprod2d(const point &v,const point &w);
    static void clear_point_coords(point &p);
    static double point_displacement(const point &p,bool sqroot=true);
    static void copy_point(const point &src,point &dest);
    static void rand_point(point &p,double radius=1.0);
    static void point_reflection(const point &src,const point &axis,point &dest);
    static bool points_coincide(const point &p,const point &q,double tol);
    static void copy_layout(const layout &src,layout &dest);
    static void rotate_layout(layout &x, double phi);
    static void translate_layout(layout &x,const point &dx);
    static double layout_min(const layout &x,int d);
    static void point2polar(point &p);
    static bool sparse_matrix_element(const sparsemat &A, int i, int j, double &val);
    static void multiply_sparse_matrices(const sparsemat &A, const sparsemat &B, sparsemat &prod);
    static void transpose_sparsemat(const sparsemat &A,sparsemat &transp);
    static void multilevel_recursion(layout &x, graphe &G, int d, double R, double K, double tol, int depth=0);
    int mdeg(const ivector &V,int i) const;
    void coarsening(graphe &G, const sparsemat &P, const ivector &V) const;
    void tomita_recurse(const ivector &R, const ivector &P_orig, const ivector &X_orig, ivectors &cliques) const;
    void remove_isolated_node(int i);
    void make_connected_component(int i,int ci,std::map<int,int> &indices) const;
    void find_cut_vertices_dfs(int u,int &t,ivector &disc,ivector &low,std::vector<bool> &ap) const;
    void find_blocks_dfs(int v,int u,int &t,ivector &dfi,ivector &p,ipairs &stck,std::vector<ipairs> &blocks) const;
    int find_cycle_dfs(int v,std::stack<int> &path,std::vector<bool> &visited,ivector &parent) const;
    bool find_path_dfs(int dest, int v, std::stack<int> &path, std::vector<bool> &visited) const;
    void get_edges(ipairs &E) const;
    void bridge_contact_vertices(const graphe &bridge,const std::vector<bool> &embedding,ivector &cp) const;
    static void split_faces(const ivector &face,ivectors &faces);
    static void sort_rectangles(std::vector<rectangle> &rectangles);
    static bool pack_rectangles(const std::vector<rectangle> &rectangles,dpairs &embedding,double ew,double eh);
    static dpairs pack_rectangles_neatly(const std::vector<rectangle> &rectangles);
    static bool edge_crossing(const point &p,const point &r,const point &q,const point &s, point &crossing);
    static double ccw(const point &p1, const point &p2, const point &p3);
    static double polyarea(const layout &v);
    void promote_edge_crossings(layout &x);
    double purchase(const layout &x, int v, int w, int orig_node_count, const point &axis,
                    const ipairs &E, std::vector<double> &sc, double tol) const;
    point axis_of_symmetry(layout &x) const;
    static void bisector(int v,int w,const layout &x,point &bsec);
    static double squared_distance(const point &p,const point &line);
    void force_directed(layout &x,double K,double R,double tol=0.001,bool ac=true);
    void multilevel_force_directed(layout &x,int d,double K,double tol=0.001);
    void make_regular_polygon(layout &x,const ivector &face,double R=1.0);
    void coarsening_mis(const ivector &V,graphe &G,sparsemat &P) const;
    void coarsening_ec(const ipairs &M, graphe &G, sparsemat &P) const;
    int node_label_best_quadrant(const layout &x,const point &center,int i) const;
    static void append_segment(vecteur &v,const point &p,const point &q,int color,int width,bool arrow=false);
    static void append_vertex(vecteur &v,const point &p,int color,int width);
    static void append_label(vecteur &v,const point &p,const gen &label,int quadrant);
    bool planar_embedding_block(ivectors &faces) const;
    int planar_embedding_connected(ivectors &faces,ipairs &temp_edges);
    void plestenjak_layout(layout &x,ivectors &faces,int f,double tol=0.01);
    bool planar_layout(layout &x,double K);
    static void find_block_neighbors(int i,ivectors &blocks);
    static int common_element(const ivector &v1,const ivector &v2,int offset=0);
    void embed_children_blocks(int i,ivectors &block_tree,std::vector<ivectors> &blocks_faces,ipairs &temp_edges);
    int handle_2deg_nodes(ivectors &faces,int f0,ipairs &temp_edges);
    int handle_concave_faces(ivectors &faces,int f0,ipairs &temp_edges);
    int subdivide_faces(ivectors &faces,int f0);
    void triangulate(ivectors &faces,int i,int v,std::vector<bool> &visited,ipairs &temp_edges);

public:
    graphe();
    graphe(const graphe &G);
    static gen make_idnt(const char* name,int index=-1,bool intern=true);
    static void make_default_vertex_labels(vecteur &labels,int n,int n0,GIAC_CONTEXT);
    static gen boole(bool b) { return b?VRAI:FAUX; }
    static gen str2gen(const std::string &str,bool isstring=false);
    static std::string genstring2str(const gen &g);
    static std::string gen2str(const gen &g);
    bool read_gen(const vecteur &v,GIAC_CONTEXT);
    void copy(graphe &G) const;
    void join(const graphe &G);
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
    void translate_indices_to(const graphe &G,const ivector &indices,ivector &dest) const;
    void translate_indices_from(const graphe &G,const ivector &indices,ivector &dest) const;
    vecteur edges(bool include_weights) const;
    int add_node(const gen &v);
    int add_node(const gen &v,const attrib &attr) { int i=add_node(v); nodes[i].set_attributes(attr); return i; }
    void add_nodes(const vecteur &v);
    bool remove_node(int i);
    bool remove_node(const gen &v);
    void remove_nodes(const vecteur &v);
    const gen &node(int i) const { assert(i>=0 && i<node_count()); return nodes[i].symbol(); }
    vecteur get_nodes(const ivector &v) const;
    int node_index(const gen &v) const;
    const attrib &node_attributes(int i) const { assert(i>=0 && i<node_count()); return nodes[i].attributes(); }
    const attrib &edge_attributes(int i,int j) const;
    attrib &edge_attributes(int i,int j);
    void add_edge(int i,int j,const gen &w=gen(1));
    void add_edge(int i,int j,const attrib &attr);
    void add_edge(const ipair &edge) { add_edge(edge.first,edge.second); }
    void add_edge(const ipair &edge,const attrib &attr) { add_edge(edge.first,edge.second,attr); }
    ipair add_edge(const gen &v,const gen &w,const gen &weight=gen(1));
    bool remove_edge(int i,int j);
    bool remove_edge(const ipair &p) { return remove_edge(p.first,p.second); }
    void make_cycle(const vecteur &v);
    bool has_edge(int i,int j) const;
    bool has_edge(ipair p) const { return has_edge(p.first,p.second); }
    ipair make_edge(int i,int j) const;
    ipair make_edge(const vecteur &v) const;
    bool is_adjacent(int i,int j) const;
    int in_degree(int index) const;
    int out_degree(int index) const;
    int degree(int index) const;
    matrice adjacency_matrix() const;
    matrice incidence_matrix() const;
    void set_graph_attribute(int key,const gen &val) { attributes[key]=val; }
    void set_graph_attributes(const attrib &attr) { copy_attributes(attr,attributes); }
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
    void make_unweighted();
    void underlying(graphe &G) const;
    bool isomorphic_copy(graphe &G,const ivector &sigma);
    bool relabel_nodes(const vecteur &labels);
    void induce_subgraph(const ivector &vi,graphe &G,bool copy_attrib=true) const;
    ivector maximal_independent_set() const;
    void maximize_matching(ipairs &matching);
    ipairs find_maximal_matching() const;
    bool trail(const vecteur &v);
    void color_node(int index, int c) { set_node_attribute(index,_GT_ATTRIB_COLOR,c); }
    void color_nodes(const gen &c);
    bool parse_list_of_edges(const vecteur &v,GIAC_CONTEXT);
    bool parse_matrix(const matrice &m,bool iswei,int mode,GIAC_CONTEXT);
    void parse_trail(const vecteur &v);
    layout make_layout(double K,gt_layout_style style);
    static void create_random_layout(layout &x,double K,int d);
    static gen point2gen(const point &p,bool vect=false);
    static point layout_center(const layout &x);
    static void scale_layout(layout &x,double diam);
    void tomita(ivectors &cliques) const;
    void make_sierpinski_graph(int n,int k,bool triangle,GIAC_CONTEXT);
    void make_complete_graph(const vecteur &V);
    void make_complete_multipartite_graph(const ivector &partition_sizes,GIAC_CONTEXT);
    void make_petersen_graph(int n,int k,GIAC_CONTEXT);
    void make_random_tree(int n,GIAC_CONTEXT);
    void connected_components(ivectors &components) const;
    ivector find_cut_vertices() const;
    void find_blocks(std::vector<ipairs> &blocks) const;
    ivector find_cycle() const;
    ivector find_path(int v,int w) const;
    void collapse_edge(int i,int j);
    ipairs incident_edges(const ivector &v);
    bool get_layout(std::vector<point> &positions, int &dim) const;
    void bridges(const std::vector<bool> &embedding,const ivectors &faces,std::vector<graphe> &B) const;
    static ivector range_complement(const ivector &v, int n);
    bool convex_hull(ivector &ccw_indices, const layout &x) const;
    double subgraph_area(const layout &x,const ivector &v=ivector()) const;
    void draw_edges(vecteur &v,const layout &x) const;
    void draw_nodes(vecteur &v,const layout &x) const;
    void draw_labels(vecteur &v,const layout &x) const;
    void get_leading_cycle(ivector &c) const;

    graphe &operator =(const graphe &other) { nodes.clear(); other.copy(*this); return *this; }
};

#ifndef NO_NAMESPACE_GIAC
} // namespace giac
#endif // ndef NO_NAMESPACE_GIAC
#endif // __GRAPHE_H