/*
 * graphtheory.cc
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

#include "graphe.h"
#include "graphtheory.h"
#include <giac/giac.h>
#include <bitset>

using namespace std;

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

/* error messages */
static const char *gt_error_messages[] = {
    "unknown error",                                                    //  0
    "argument is not a graph",                                          //  1
    "a weighted graph is required",                                     //  2
    "an unweighted graph is required",                                  //  3
    "a directed graph is required",                                     //  4
    "an undirected graph is required",                                  //  5
    "does not specify an edge",                                         //  6
    "mixing edges and arcs is not allowed",                             //  7
    "weight/adjacency matrix must be symmetric for undirected graphs",  //  8
    "failed to read graph from file",                                   //  9
    "edge not found",                                                   // 10
    "vertex not found"                                                  // 11
};

void gt_err_display(int code,GIAC_CONTEXT) {
    *logptr(contextptr) << "Error: " << gt_error_messages[code] << endl;
}

void gt_err_display(const gen &g,int code,GIAC_CONTEXT) {
    *logptr(contextptr) << "Error: " << g << " " << gt_error_messages[code] << endl;
}

gen gt_err(int code,GIAC_CONTEXT) {
    gt_err_display(code,contextptr);
    return gentypeerr(contextptr);
}

gen gt_err(const gen &g,int code,GIAC_CONTEXT) {
    gt_err_display(g,code,contextptr);
    return gentypeerr(contextptr);
}

bool graphe::parse_list_of_edges(const vecteur &v,GIAC_CONTEXT) {
    int st=-2;
    bool hasweight;
    ipair c;
    for (const_iterateur it=v.begin();it!=v.end();++it) {
        if (it->type!=_VECT || int(it->_VECTptr->size())<2) {
            gt_err_display(*it,_GT_ERR_INVALID_EDGE,contextptr);
            return false;
        }
        vecteur &e = *it->_VECTptr;
        hasweight=e.front().type==_VECT;
        gen edge(hasweight?e.front():*it);
        if (int(edge._VECTptr->size())!=2)
            gt_err_display(edge,_GT_ERR_INVALID_EDGE,contextptr);
        if (st==-2)
            st=edge.subtype;
        else if (st!=edge.subtype) {
            gt_err_display(_GT_ERR_INVALID_EDGE_ARC_MIX,contextptr);
            return false;
        }
        c=add_edge(edge._VECTptr->front(),edge._VECTptr->back());
        if (hasweight)
            set_edge_attribute(c.first,c.second,_GT_ATTRIB_WEIGHT,e[1]);
    }
    set_directed(st!=_SET__VECT);
    return true;
}

bool graphe::parse_matrix(const matrice &m,bool iswei,int mode,GIAC_CONTEXT) {
    int n=m.size(),vi,vj;
    if (int(m.front()._VECTptr->size())!=n || (mode>0 && node_count()!=n))
        return false;
    for (int i=0;i<n;++i) {
        vi=mode==0?add_node(i+array_start(contextptr)):i;
        for (int j=0;j<n;++j) {
            vj=mode==0?add_node(j+array_start(contextptr)):j;
            if (!is_zero(m[i][j])) {
                if (!iswei && !is_one(m[i][j]))
                    return false;
                if (mode<2)
                    add_edge(vi,vj);
                else if (!iswei || !has_edge(vi,vj))
                    return false;
            }
            if (iswei)
                set_edge_attribute(i,j,_GT_ATTRIB_WEIGHT,m[i][j]);
        }
    }
    return true;
}

void graphe::parse_trail(const vecteur &v) {
    int last_index=-1,n=v.size();
    for (int i=1;i<n;++i) {
        int index=add_node(v[i]);
        if (last_index>=0)
            add_edge(last_index,index);
        last_index=index;
    }
}

// +--------------------------------------------------------------------------+
// |                             GIAC COMMANDS                                |
// +--------------------------------------------------------------------------+

/*
 * Usage:   graph(V,[opts])
 *          graph(V,E,[opts])
 *          graph(A,[opts])
 *          graph(V,E,A,[opts])
 *
 * Create (un)directed (un)weighted graph from list of vertices V, set of edges
 * E, list of neighbors L and/or adjacency matrix A containing edge weights.
 * All parameters are optional.
 *
 * 'opts' is a sequence of options containing gt_weighted=true/false,
 * gt_directed=true/false, gt_vertexcolor=c or gt_vertexpositions=p. Here c is
 * integer or list of integers (color(s) to be assigned to vertices (in order))
 * and p is list of coordinates to assign to vertices (used for drawing).
 */
gen _graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    graphe G;
    if (ckmatrix(g)) {
        // adjacency matrix is given
        G.parse_matrix(*g._VECTptr,false,0,contextptr);
    } else if (g.type==_VECT && g.subtype!=_SEQ__VECT) {
        // list of vertices or set of edges is given
        if (g.subtype==_SET__VECT)
            G.parse_list_of_edges(*g._VECTptr,contextptr);
        else
            G.add_nodes(*g._VECTptr);
    } else if (g.is_symb_of_sommet(at_of)) {
        vecteur &h=*g._SYMBptr->feuille._VECTptr;
        if (h.front().type!=_IDNT || string(h.front()._IDNTptr->id_name)!="Trail")
            return gentypeerr(contextptr);
        G.parse_trail(h);
    }
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &args=*g._VECTptr;
    int nargs=args.size(),n=nargs-1;
    // parse options first
    bool iswei=false;
    while(args[n].is_symb_of_sommet(at_equal)) {
        vecteur &sides=*args[n]._SYMBptr->feuille._VECTptr;
        if (!sides.front().is_integer())
            return gentypeerr(contextptr);
        switch(sides.front().val) {
        case _GT_OPT_DIRECTED:
            if (!sides.back().is_integer())
                return gentypeerr(contextptr);
            G.set_directed((bool)sides.back().val);
            break;
        case _GT_OPT_WEIGHTED:
            if (!sides.back().is_integer())
                return gentypeerr(contextptr);
            if (sides.back().val!=0)
                iswei=true;
            break;
        }
        n--;
    }
    // parse arguments
    for (int i=0;i<nargs;++i) {
        if (i<=n && ckmatrix(args[i]) && args[i].subtype!=_SET__VECT) {
            // adjacency or weight matrix
            matrice &m=*args[i]._VECTptr;
            if (!G.is_directed() && m!=mtran(m))
                return gt_err(_GT_ERR_MATRIX_NOT_SYMMETRIC,contextptr);
            if (!G.parse_matrix(m,i==2 || iswei,i,contextptr))
                return gentypeerr(contextptr);
        } else if (i<2 && args[i].type==_VECT) {
            int permu_size;
            if (args[i].subtype==_SET__VECT) {
                // set of edges
                if (!G.parse_list_of_edges(*args[i]._VECTptr,contextptr))
                    return gentypeerr(contextptr);
            } else if (i==1 && _is_permu(args[i],contextptr).val==1 &&
                       (permu_size=int(args[i]._VECTptr->size()))>0) {
                // directed cycle
                G.set_directed(true);
                if (permu_size!=G.node_count())
                    return gensizeerr(contextptr);
                int last_index=args[i]._VECTptr->back().val;
                for (const_iterateur it=args[i]._VECTptr->begin();it!=args[i]._VECTptr->end();++it) {
                    int index=it->val-array_start(contextptr);
                    G.add_edge(last_index,index);
                    last_index=index;
                }
            } else if (i==0) // list of vertices
                G.add_nodes(*args[i]._VECTptr);
            else return gentypeerr(contextptr);
        } else if (i==0 && args[i].is_symb_of_sommet(at_of)) {
            // trail
            if (n>0)
                return gentypeerr(contextptr);
            vecteur &h=*args[i]._SYMBptr->feuille._VECTptr;
            if (h.front().type!=_IDNT || string(h.front()._IDNTptr->id_name)!="Trail")
                return gentypeerr(contextptr);
            G.parse_trail(h);
        } else if (i>n && args[i].is_symb_of_sommet(at_equal)) {
            // option
            vecteur &sides=*args[i]._SYMBptr->feuille._VECTptr;
            if (!sides.front().is_integer())
                return gentypeerr(contextptr);
            switch(sides.front().val) {
            case _GT_OPT_VERTEX_COLOR:
                G.color_nodes(sides.back());
                break;
            case _GT_OPT_VERTEX_POSITIONS:

                break;
            }
        } else return gentypeerr(contextptr);
    }
    return G.to_gen();
}
static const char _graph_s []="graph";
static define_unary_function_eval(__graph,&_graph,_graph_s);
define_unary_function_ptr5(at_graph,alias_at_graph,&__graph,0,true)

/*
 * Usage:   digraph(V,[opts])
 *          digraph(V,E,[opts])
 *          digraph(A,[opts])
 *          digraph(V,E,A,[opts])
 *
 * Create directed (un)weighted graph from list of vertices V, set of edges E
 * and/or adjacency matrix A containing edge weights. All parameters are
 * optional.
 *
 * 'opts' may be one of weighted=true/false, vertexcolor=c and
 * vertexpositions=p. Here c is integer or list of integers (color(s) to be
 * assigned to vertices (in order)) and p is list of coordinates to assign to
 * vertices (used for drawing).
 */
gen _digraph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    vecteur args;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    if (g.subtype==_SEQ__VECT)
        args=*g._VECTptr;
    else args.push_back(g);
    args.push_back(symbolic(at_equal,makesequence(_GT_OPT_DIRECTED,graphe::VRAI)));
    return _graph(args,contextptr);
}
static const char _digraph_s []="digraph";
static define_unary_function_eval(__digraph,&_digraph,_digraph_s);
define_unary_function_ptr5(at_digraph,alias_at_digraph,&__digraph,0,true)

/*
 * Usage:   export_graph(G,"path/to/graphname")
 *
 * Writes graph G to the file 'graphname.dot' in directory 'path/to' using dot
 * format. Returns 1 on success and 0 on failure.
 */
gen _export_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())!=2)
        return gentypeerr(contextptr);
    gen &gr=g._VECTptr->front(),&name=g._VECTptr->back();
    graphe G;
    if (gr.type!=_VECT || name.type!=_STRNG)
        return gentypeerr(contextptr);
    if (!G.read_gen(*gr._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    string filename=graphe::genstring2str(name)+".dot";
    return G.write_dot(filename)?1:0;
}
static const char _export_graph_s []="export_graph";
static define_unary_function_eval(__export_graph,&_export_graph,_export_graph_s);
define_unary_function_ptr5(at_export_graph,alias_at_export_graph,&__export_graph,0,true)

/*
 * Usage:   import_graph("path/to/graphname")
 *
 * Returns graph read from file 'graphname.dot' in directory 'path/to' (in dot
 * format). Returns 1 on success and 0 on failure.
 */
gen _import_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_STRNG)
        return gentypeerr(contextptr);
    graphe G;
    string filename=graphe::genstring2str(g)+".dot";
    if (!G.read_dot(filename)) {
        gt_err_display(_GT_ERR_READING_FAILED,contextptr);
        return undef;
    }
    return G.to_gen();
}
static const char _import_graph_s []="import_graph";
static define_unary_function_eval(__import_graph,&_import_graph,_import_graph_s);
define_unary_function_ptr5(at_import_graph,alias_at_import_graph,&__import_graph,0,true)

/*
 * Usage:   vertices(G)
 *
 * Return list of vertices of graph G.
 */
gen _vertices(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return G.vertices();
}
static const char _vertices_s []="vertices";
static define_unary_function_eval(__vertices,&_vertices,_vertices_s);
define_unary_function_ptr5(at_vecrtices,alias_at_vertices,&__vertices,0,true)

/*
 * Usage:   edges(G,[weights])
 *
 * Return list of edges of graph G. If second argument is the option 'weights',
 * edge weights are also returned.
 */
gen _edges(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    bool include_weights=false;
    graphe G;
    if (g.subtype==_SEQ__VECT) {
        if (int(g._VECTptr->size())!=2)
            return gensizeerr(contextptr);
        if (g._VECTptr->front().type!=_VECT)
            return gentypeerr(contextptr);
        if (g._VECTptr->back().is_integer() && g._VECTptr->back().val==_GT_OPT_WEIGHTS)
            include_weights=true;
        if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
            return gentypeerr(contextptr);
    } else if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (include_weights && !G.is_weighted())
        return gt_err(_GT_ERR_WEIGHTED_GRAPH_REQUIRED,contextptr);
    return G.edges(include_weights);
}
static const char _edges_s []="edges";
static define_unary_function_eval(__edges,&_edges,_edges_s);
define_unary_function_ptr5(at_edges,alias_at_edges,&__edges,0,true)

/*
 * Usage:   has_edge(G,e)
 *
 * Returns true iff edge e={i,j} is contained in undirected graph G.
 */
gen _has_edge(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv = *g._VECTptr;
    if (int(gv.size())!=2)
        return gensizeerr(contextptr);
    if (gv.front().type!=_VECT || gv.back().type!=_VECT || !is_integer_vecteur(*gv.back()._VECTptr))
        return gentypeerr(contextptr);
    if (int(gv.back()._VECTptr->size())!=2)
        return gensizeerr(contextptr);
    vecteur e(*gv.back()._VECTptr);
    int ofs=array_start(contextptr);
    int i=e.front().val-ofs,j=e.back().val-ofs;
    graphe G;
    if (!G.read_gen(*gv.front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(!G.is_directed() && G.has_edge(i,j));
}
static const char _has_edge_s []="has_edge";
static define_unary_function_eval(__has_edge,&_has_edge,_has_edge_s);
define_unary_function_ptr5(at_has_edge,alias_at_has_edge,&__has_edge,0,true)

/*
 * Usage:   has_arc(G,e)
 *
 * Returns true iff arc e=[i,j] is contained in directed graph G. If e={i,j},
 * true is returned if directed graph G has both edges [i,j] and [j,i].
 */
gen _has_arc(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv = *g._VECTptr;
    if (int(gv.size())!=2)
        return gensizeerr(contextptr);
    if (gv.front().type!=_VECT || gv.back().type!=_VECT || !is_integer_vecteur(*gv.back()._VECTptr))
        return gentypeerr(contextptr);
    if (int(gv.back()._VECTptr->size())!=2)
        return gensizeerr(contextptr);
    vecteur e(*gv.back()._VECTptr);
    bool undirected=gv.back().subtype==_SET__VECT;
    int ofs=array_start(contextptr);
    int i=e.front().val-ofs,j=e.back().val-ofs;
    graphe G;
    if (!G.read_gen(*gv.front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_directed() && G.has_edge(i,j) && (!undirected || G.has_edge(j,i)));
}
static const char _has_arc_s []="has_arc";
static define_unary_function_eval(__has_arc,&_has_arc,_has_arc_s);
define_unary_function_ptr5(at_has_arc,alias_at_has_arc,&__has_arc,0,true)

/*
 * Usage:   adjacency_matrix(G)
 *
 * Returns the adjacency matrix of a graph G whose rows and columns are indexed
 * by the vertices. The entry [i,j] of this matrix is 1 if there is an edge
 * from vertex i to vertex j and 0 otherwise.
 */
gen _adjacency_matrix(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.node_count()==0)
        return vecteur(0);
    return G.adjacency_matrix();
}
static const char _adjacency_matrix_s []="adjacency_matrix";
static define_unary_function_eval(__adjacency_matrix,&_adjacency_matrix,_adjacency_matrix_s);
define_unary_function_ptr5(at_adjacency_matrix,alias_at_adjacency_matrix,&__adjacency_matrix,0,true)

/*
 * Usage:   incidence_matrix(G)
 *
 * Returns the incidence matrix of a graph G whose rows are indexed by the
 * vertices and columns by the edges (in order defined by the command 'edges').
 * The entry [i,j] of this matrix is 1 if the i-th vertex is incident with the
 * j-th edge (for directed graphs, -1 if vertex is the tail and 1 if it is the
 * head of arc).
 */
gen _incidence_matrix(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.node_count()==0)
        return vecteur(0);
    return G.incidence_matrix();
}
static const char _incidence_matrix_s []="incidence_matrix";
static define_unary_function_eval(__incidence_matrix,&_incidence_matrix,_incidence_matrix_s);
define_unary_function_ptr5(at_incidence_matrix,alias_at_incidence_matrix,&__incidence_matrix,0,true)

/*
 * Usage:   weight_matrix(G)
 *
 * Returns the weight matrix of graph G.
 */
gen _weight_matrix(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr) || !G.is_weighted())
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.node_count()==0)
        return vecteur(0);
    return G.weight_matrix();
}
static const char _weight_matrix_s []="weight_matrix";
static define_unary_function_eval(__weight_matrix,&_weight_matrix,_weight_matrix_s);
define_unary_function_ptr5(at_weight_matrix,alias_at_weight_matrix,&__weight_matrix,0,true)

/*
 * Usage:   graph_complement(G)
 *
 * Return the complement of graph G, that is the graph with the same vertex set
 * as G, but whose edge (arc) set consists of the edges (arcs) not present in
 * G.
 */
gen _graph_complement(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G,C;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int n=G.node_count();
    C.add_nodes(G.vertices());
    C.set_directed(G.is_directed());
    for (int i=0;i<n;++i) {
        for (int j=0;j<G.is_directed()?n:i;++j) {
            if (!G.has_edge(i,j))
                C.add_edge(i,j);
        }
    }
    return C.to_gen();
}
static const char _graph_complement_s []="graph_complement";
static define_unary_function_eval(__graph_complement,&_graph_complement,_graph_complement_s);
define_unary_function_ptr5(at_graph_complement,alias_at_graph_complement,&__graph_complement,0,true)

/*
 * Usage:   subgraph(G,E)
 *
 * Returns the subgraph of G defined by edges in list E.
 */
gen _subgraph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())!=2 ||
            g._VECTptr->front().type!=_VECT || g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &ev=*g._VECTptr->back()._VECTptr;
    graphe G;
    graphe::ipair edge;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    graphe S(G);
    for (const_iterateur it=ev.begin();it!=ev.end();++it) {
        if (it->type!=_VECT || int(it->_VECTptr->size())!=2)
            return gentypeerr(contextptr);
        edge=S.make_edge(*it->_VECTptr);
        if (!G.has_edge(edge))
            return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
        S.add_node(G.node(edge.first),G.node_attributes(edge.first));
        S.add_node(G.node(edge.second),G.node_attributes(edge.second));
        S.add_edge(edge,G.edge_attributes(edge.first,edge.second));
    }
    return S.to_gen();
}
static const char _subgraph_s []="subgraph";
static define_unary_function_eval(__subgraph,&_subgraph,_subgraph_s);
define_unary_function_ptr5(at_subgraph,alias_at_subgraph,&__subgraph,0,true)

/*
 * Usage:   vertex_degree(G,v)
 *
 * Returns the degree of vertex v in graph G (number of edges incident to v).
 */
gen _vertex_degree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())<2 || g._VECTptr->front().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int i=G.node_index(g._VECTptr->at(1));
    if (i==-1)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    return G.degree(i);
}
static const char _vertex_degree_s []="vertex_degree";
static define_unary_function_eval(__vertex_degree,&_vertex_degree,_vertex_degree_s);
define_unary_function_ptr5(at_vertex_degree,alias_at_vertex_degree,&__vertex_degree,0,true)

/*
 * Usage:   vertex_in_degree(G,v)
 *
 * Returns number of arcs ending in vertex v of graph G.
 */
gen _vertex_in_degree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())<2 || g._VECTptr->front().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    int i=G.node_index(g._VECTptr->at(1));
    if (i==-1)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    return G.in_degree(i);
}
static const char _vertex_in_degree_s []="vertex_in_degree";
static define_unary_function_eval(__vertex_in_degree,&_vertex_in_degree,_vertex_in_degree_s);
define_unary_function_ptr5(at_vertex_in_degree,alias_at_vertex_in_degree,&__vertex_in_degree,0,true)

/*
 * Usage:   vertex_out_degree(G,v)
 *
 * Returns number of arcs starting in vertex v of graph G.
 */
gen _vertex_out_degree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())<2 || g._VECTptr->front().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    int i=G.node_index(g._VECTptr->at(1));
    if (i==-1)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    return G.out_degree(i);
}
static const char _vertex_out_degree_s []="vertex_out_degree";
static define_unary_function_eval(__vertex_out_degree,&_vertex_out_degree,_vertex_out_degree_s);
define_unary_function_ptr5(at_vertex_out_degree,alias_at_vertex_out_degree,&__vertex_out_degree,0,true)

/*
 * Usage:   induced_subgraph(G,V)
 *
 * Returns the subgraph of G induced by vertices in list V.
 */
gen _induced_subgraph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())!=2 ||
            g._VECTptr->front().type!=_VECT || g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur &V=*g._VECTptr->back()._VECTptr;
    int i=0,index;
    vector<int> vi(V.size());
    for (const_iterateur it=V.begin();it!=V.end();++it) {
        index=G.node_index(*it);
        if (index==-1)
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        vi[i++]=index;
    }
    graphe S(G);
    G.make_induced_subgraph(vi,S);
    return S.to_gen();
}
static const char _induced_subgraph_s []="induced_subgraph";
static define_unary_function_eval(__induced_subgraph,&_induced_subgraph,_induced_subgraph_s);
define_unary_function_ptr5(at_induced_subgraph,alias_at_induced_subgraph,&__induced_subgraph,0,true)

/*
 * Usage:   maximal_independent_set(G)
 *
 * Returns a maximal set of mutually independent (non-adjacent) vertices of
 * graph G. Using a method by Y.Hu (based on that of Ruge and Stuben), see
 * http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.38.2239&rep=rep1&type=pdf.
 */
gen _maximal_independent_set(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vector<int> mis=G.maximal_independent_set();
    int n=mis.size();
    vecteur V(n);
    for (int i=0;i<n;++i) {
        V[i]=G.node(mis[i]);
    }
    return V;
}
static const char _maximal_independent_set_s []="maximal_independent_set";
static define_unary_function_eval(__maximal_independent_set,&_maximal_independent_set,_maximal_independent_set_s);
define_unary_function_ptr5(at_maximal_independent_set,alias_at_maximal_independent_set,&__maximal_independent_set,0,true)

/*
 * Usage:   maximum_matching(G)
 *
 * Returns a list of edges representing maximum matching for graph G. Jack
 * Edmonds' blossom algorithm is used.
 */
gen _maximum_matching(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    vector<graphe::ipair> M;
    G.maximize_matching(M);
    vecteur res;
    for (vector<graphe::ipair>::const_iterator it=M.begin();it!=M.end();++it) {
        res.push_back(makevecteur(G.node(it->first),G.node(it->second)));
    }
    return res;
}
static const char _maximum_matching_s []="maximum_matching";
static define_unary_function_eval(__maximum_matching,&_maximum_matching,_maximum_matching_s);
define_unary_function_ptr5(at_maximum_matching,alias_at_maximum_matching,&__maximum_matching,0,true)

/*
 * Usage:   make_directed(G,[A])
 *
 * Returns directed graph made from G by converting every edge to pair of arcs
 * [with weights specified by matrix A].
 */
gen _make_directed(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || (g.subtype==_SEQ__VECT && g._VECTptr->front().type!=_VECT))
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(g.subtype==_SEQ__VECT?*g._VECTptr->front()._VECTptr:*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    G.make_directed();
    if (g.subtype==_SEQ__VECT && int(g._VECTptr->size())>1) {
        if (g._VECTptr->at(1).type!=_VECT)
            return gentypeerr(contextptr);
        G.make_weighted(*g._VECTptr->at(1)._VECTptr);
    }
    return G.to_gen();
}
static const char _make_directed_s []="make_directed";
static define_unary_function_eval(__make_directed,&_make_directed,_make_directed_s);
define_unary_function_ptr5(at_make_directed,alias_at_make_directed,&__make_directed,0,true)

/*
 * Usage:   underlying_graph(G)
 *
 * Returns underlying graph of G, i.e. the graph obtained by stripping
 * directions and weights from arcs (pairs of arcs connecting the same vertices
 * are merged to a single edge).
 */
gen _underlying_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    G.make_underlying();
    return G.to_gen();
}
static const char _underlying_graph_s []="make_underlying";
static define_unary_function_eval(__underlying_graph,&_underlying_graph,_underlying_graph_s);
define_unary_function_ptr5(at_underlying_graph,alias_at_underlying_graph,&__underlying_graph,0,true)

/*
 * Usage:   cycle_graph(n)
 *          cycle_graph(V)
 *
 * Returns cyclic graph with n vertices (or with vertices from list V).
 */
gen _cycle_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    vecteur V;
    if (g.type==_VECT)
        V=*g._VECTptr;
    else if (g.is_integer())
        graphe::make_default_vertex_labels(V,g.val,0,contextptr);
    graphe G;
    G.make_cycle(V);
    G.set_name(string("C")+to_string(G.node_count()));
    return G.to_gen();
}
static const char _cycle_graph_s []="cycle_graph";
static define_unary_function_eval(__cycle_graph,&_cycle_graph,_cycle_graph_s);
define_unary_function_ptr5(at_cycle_graph,alias_at_cycle_graph,&__cycle_graph,0,true)

/*
 * Usage:   lcf_graph(jumps,[exp])
 *
 * Returns the graph constructed specified LCF notation jumps[^exp]. Arguments
 * are list of integers 'jumps' [and a positive integer 'exp', by default 1].
 */
gen _lcf_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    vecteur jumps;
    gen e;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    if (g.subtype==_SEQ__VECT) {
        if (int(g._VECTptr->size())<2)
            return gensizeerr(contextptr);
        e=g._VECTptr->at(1);
        if (!e.is_integer() || e.val<=0 || g._VECTptr->front().type!=_VECT)
            return gentypeerr(contextptr);
        jumps=*g._VECTptr->front()._VECTptr;
    } else {
        jumps=*g._VECTptr;
        e=gen(1);
    }
    if (jumps.empty())
        return gensizeerr(contextptr);
    if (!is_integer_vecteur(jumps))
        return gentypeerr(contextptr);
    int m=jumps.size(),n=m*e.val;
    graphe G;
    vecteur V;
    graphe::make_default_vertex_labels(V,n,0,contextptr);
    G.make_cycle(V);
    int j=0,k;
    for (int i=0;i<n;++i) {
        k=(i+jumps[j].val)%n;
        if (k<0)
            k+=n;
        G.add_edge(i,k);
        j=(j+1)%m;
    }
    return G.to_gen();
}
static const char _lcf_graph_s []="lcf_graph";
static define_unary_function_eval(__lcf_graph,&_lcf_graph,_lcf_graph_s);
define_unary_function_ptr5(at_lcf_graph,alias_at_lcf_graph,&__lcf_graph,0,true)

/*
 * Usage:   hypercube_graph(n)
 *
 * Constructs and returns the hypercube graph in dimension n (with 2^n vertices).
 */
gen _hypercube_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (!g.is_integer() || g.val<=0)
        return gentypeerr(contextptr);
    int n=g.val,N=std::pow(2,n);
    graphe G;
    for (int i=0;i<N;++i) {
        G.add_node(graphe::str2gen(bitset<1024>((unsigned long)i).to_string().substr(1024-n),true));
    }
    for (int i=0;i<N;++i) {
        for (int j=i+1;j<N;++j) {
            if (_hamdist(makesequence(i,j),contextptr).val==1)
                G.add_edge(i,j);
        }
    }
    return G.to_gen();
}
static const char _hypercube_graph_s []="hypercube_graph";
static define_unary_function_eval(__hypercube_graph,&_hypercube_graph,_hypercube_graph_s);
define_unary_function_ptr5(at_hypercube_graph,alias_at_hypercube_graph,&__hypercube_graph,0,true)

/*
 * Usage:   seidel_switch(G,V)
 *
 * Returns copy of graph G in which edges between vertices in list V and
 * vertices not in V are inverted, i.e. replaced with a set of edges from V to
 * other vertices which is not present in G.
 */
gen _seidel_switch(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())!=2 ||
            g._VECTptr->front().type!=_VECT || g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G,H;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    if (G.is_weighted())
        return gt_err(_GT_ERR_UNWEIGHTED_GRAPH_REQUIRED,contextptr);
    int n=G.node_count();
    vecteur &V=*g._VECTptr->back()._VECTptr;
    vector<bool> vb(n,false);
    for (const_iterateur it=V.begin();it!=V.end();++it) {
        int index=G.node_index(*it);
        if (index==-1)
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        vb[index]=true;
    }
    H.add_nodes(G.vertices());
    for (int i=0;i<n;++i) {
        for (int j=i+1;j<n;++j) {
            bool b=G.has_edge(i,j);
            if (vb[i] != vb[j])
                b=!b;
            if (b)
                H.add_edge(i,j);
        }
    }
    return H.to_gen();
}
static const char _seidel_switch_s []="seidel_switch";
static define_unary_function_eval(__seidel_switch,&_seidel_switch,_seidel_switch_s);
define_unary_function_ptr5(at_seidel_switch,alias_at_seidel_switch,&__seidel_switch,0,true)

gen edge_disp(int color,int width) {
    return symbolic(at_equal,makesequence(at_display,change_subtype(color | width,_INT_COLOR)));
}

gen draw_node(const gen &p,const gen &g,int width,int color,int quadrant) {
    int d=_POINT_PLUS | width | color | quadrant;
    gen disp=symbolic(at_equal,makesequence(at_display,change_subtype(d,_INT_COLOR)));
    gen pos=symbolic(at_point,p);
    return symbolic(at_legende,makesequence(pos,g,disp));
}

/*
 * Usage:   draw_graph(G,[opts])
 *
 * Returns the graphic representation of graph G obtained by using
 * force-directed algorithm (which can optionally be fine tuned by appending a
 * sequence 'opts' of equalities 'option=value' after the first argument).
 */
gen _draw_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    bool has_opts=false;
    if (g.type!=_VECT || ((has_opts=g.subtype==_SEQ__VECT) && g._VECTptr->front().type!=_VECT))
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(has_opts?*g._VECTptr->front()._VECTptr:*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (has_opts) {
        // parse options
        for (const_iterateur it=g._VECTptr->begin()+1;it!=g._VECTptr->end();++it) {
            ;
        }
    }
    int n=G.node_count();
    graphe::layout x;
    int d=3,q;
    double K=100;
    //x.resize(n);
    //graphe::create_random_layout(x,K,d);
    //G.force_directed(x,K,0);
    G.multilevel_force_directed(x,d,K);
    graphe::scale_layout(x,K);
    if (d==2)
        graphe::make_layout_unique(x,K);
    vecteur res;
    gen p1,p2;
    // draw edges
    int line_color=d==2?_BLUE:0,line_width=d==2?_LINE_WIDTH_2:_LINE_WIDTH_1,node_width;
    if (n<30)
        node_width=_POINT_WIDTH_3;
    else if (n<100)
        node_width=_POINT_WIDTH_2;
    else
        node_width=_POINT_WIDTH_1;
    for (int i=0;i<n;++i) {
        for (int j=G.is_directed()?0:i+1;j<n;++j) {
            if (i==j)
                continue;
            if (G.has_edge(i,j)) {
                p1=graphe::rvec2gen(x[i],true);
                p2=graphe::rvec2gen(x[j],true);
                res.push_back(symbolic(at_segment,makesequence(p1,p2,edge_disp(line_color,line_width))));
            }
        }
    }
    graphe::rvec center=G.layout_center(x);
    // draw vertices
    for (int i=0;i<n;++i) {
        if (d==2) {
            switch (G.node_label_best_quadrant(x,center,i)) {
            case 1:
                q=_QUADRANT4;
                break;
            case 2:
                q=_QUADRANT1;
                break;
            case 3:
                q=_QUADRANT2;
                break;
            case 4:
                q=_QUADRANT3;
                break;
            default:
                assert(false);
            }
        }
        res.push_back(draw_node(graphe::rvec2gen(x[i]),G.node(i),node_width,0,q));
    }
    return res;
}
static const char _draw_graph_s []="draw_graph";
static define_unary_function_eval(__draw_graph,&_draw_graph,_draw_graph_s);
define_unary_function_ptr5(at_draw_graph,alias_at_draw_graph,&__draw_graph,0,true)

/*
 * Usage:   sierpinski_graph(n,k,[triangle])
 *
 * Creates Sierpinski (triangle) graph S(n,k) (resp. ST(n,k)) and returns it.
 */
gen _sierpinski_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    int n,k=3;
    bool trng=false;
    if (g.is_integer())
        n=g.val;
    else {
        if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())<2 ||
                !g._VECTptr->front().is_integer() || !g._VECTptr->at(1).is_integer())
            return gentypeerr(contextptr);
        n=g._VECTptr->front().val;
        k=g._VECTptr->at(1).val;
        if (int(g._VECTptr->size())>2 && g._VECTptr->at(2)==at_triangle)
            trng=true;
    }
    graphe G;
    G.make_sierpinski_graph(n,k,trng,contextptr);
    return G.to_gen();
}
static const char _sierpinski_graph_s []="sierpinski_graph";
static define_unary_function_eval(__sierpinski_graph,&_sierpinski_graph,_sierpinski_graph_s);
define_unary_function_ptr5(at_sierpinski_graph,alias_at_sierpinski_graph,&__sierpinski_graph,0,true)

/*
 * Usage:   complete_graph(V)
 *          complete_graph(n)
 *          complete_graph(m,n)
 *          complete_graph(n1,n2,...,nk)
 *
 * Create and return a complete graph with vertices from list V or enumerated
 * with first n integers. When two positive integers m and n are given, a
 * complete bipartite graph is returned (or complete k-partite graph if k
 * integers n1,n2,...,nk are given).
 */
gen _complete_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    graphe G;
    if (g.type==_VECT && g.subtype!=_SEQ__VECT) {
        G.make_complete_graph(*g._VECTptr);
    } else if (g.is_integer() && g.val>0){
        vecteur V;
        graphe::make_default_vertex_labels(V,g.val,0,contextptr);
        G.make_complete_graph(V);
    } else if (g.type==_VECT && g.subtype==_SEQ__VECT) {
        // construct multipartite graph
        vecteur &partition_sizes_gen=*g._VECTptr;
        vector<int> partition_sizes;
        for (const_iterateur it=partition_sizes_gen.begin();it!=partition_sizes_gen.end();++it) {
            if (!it->is_integer() || it->val<=0)
                return gentypeerr(contextptr);
            partition_sizes.push_back(it->val);
        }
        G.make_complete_multipartite_graph(partition_sizes,contextptr);
    } else
        return gentypeerr(contextptr);
    return G.to_gen();
}
static const char _complete_graph_s []="complete_graph";
static define_unary_function_eval(__complete_graph,&_complete_graph,_complete_graph_s);
define_unary_function_ptr5(at_complete_graph,alias_at_complete_graph,&__complete_graph,0,true)

/*
 * Usage:   petersen_graph([n],[k])
 *
 * Returns the generalized Petersen graph G(n,k), where n and k are positive
 * integers. Parameter k defaults to 2 and n defaults to 5.
 */
gen _petersen_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    int n=5,k=2;
    if (g.is_integer()) {
        n=g.val;
        if (n<=0)
            return gentypeerr(contextptr);
    }
    if (g.type==_VECT && g.subtype==_SEQ__VECT) {
        if ((g._VECTptr->size())<2)
            return gensizeerr(contextptr);
        if (!g._VECTptr->front().is_integer() || !g._VECTptr->at(1).is_integer())
            return gentypeerr(contextptr);
        n=g._VECTptr->front().val;
        k=g._VECTptr->at(1).val;
        if (n<=0 || k<=0)
            return gentypeerr(contextptr);
    }
    graphe G;
    G.make_petersen_graph(n,k,contextptr);
    return G.to_gen();
}
static const char _petersen_graph_s []="petersen_graph";
static define_unary_function_eval(__petersen_graph,&_petersen_graph,_petersen_graph_s);
define_unary_function_ptr5(at_petersen_graph,alias_at_petersen_graph,&__petersen_graph,0,true)

/*
 * Usage:   articulation_points(G)
 *
 * Returns the list of articulation points (i.e. cut vertices) of graph G.
 */
gen _articulation_points(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    graphe::ivector V=G.find_cut_vertices();
    vecteur v;
    for (graphe::ivector::const_iterator it=V.begin();it!=V.end();++it) {
        v.push_back(G.node(*it));
    }
    return v;
}
static const char _articulation_points_s []="articulation_points";
static define_unary_function_eval(__articulation_points,&_articulation_points,_articulation_points_s);
define_unary_function_ptr5(at_articulation_points,alias_at_articulation_points,&__articulation_points,0,true)

/*
 * Usage:   biconnected_components(G)
 *
 * Returns the list of biconnected components of graph G. Every component is
 * given as a list of edges of G belonging to that component.
 */
gen _biconnected_components(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vector<vector<graphe::ipair> > blocks;
    G.find_blocks(blocks);
    vecteur res;
    for (vector<vector<graphe::ipair> >::const_iterator it=blocks.begin();it!=blocks.end();++it) {
        vecteur v;
        for (vector<graphe::ipair>::const_iterator jt=it->begin();jt!=it->end();++jt) {
            v.push_back(makevecteur(G.node(jt->first),G.node(jt->second)));
        }
        res.push_back(v);
    }
    return res;
}
static const char _biconnected_components_s []="biconnected_components";
static define_unary_function_eval(__biconnected_components,&_biconnected_components,_biconnected_components_s);
define_unary_function_ptr5(at_biconnected_components,alias_at_biconnected_components,&__biconnected_components,0,true)

bool parse_edge_with_weight(graphe &G,const vecteur &E) {
    if (E.size()!=2)
        return false;
    const vecteur &e=*E.front()._VECTptr;
    const gen &w=E.back();
    if (e.size()!=2)
        return false;
    G.add_edge(e.front(),e.back(),w);
    return true;
}

bool parse_edges(graphe &G,const vecteur &E) {
    if (ckmatrix(E,true)) {
        if (E.front()._VECTptr->size()!=2)
            return false;
        for (const_iterateur it=E.begin();it!=E.end();++it) {
            if (it->_VECTptr->front().type!=_VECT)
                G.add_edge(it->_VECTptr->front(),it->_VECTptr->back());
            else {
                if (!parse_edge_with_weight(G,*it->_VECTptr))
                    return false;
            }
        }
    } else {
        int n=E.size();
        if (n<2)
            return false;
        if (E.front().type==_VECT) {
            if (!parse_edge_with_weight(G,E))
                return false;
        } else {
            for (int i=0;i<n-1;++i) {
                G.add_edge(E[i],E[i+1]);
            }
        }
    }
    return true;
}

bool delete_edges(graphe &G,const vecteur &E) {
    if (ckmatrix(E)) {
        if (E.front()._VECTptr->size()!=2)
            return false;
        for (const_iterateur it=E.begin();it!=E.end();++it) {
            int i=G.node_index(it->_VECTptr->front()),j=G.node_index(it->_VECTptr->back());
            if (i>=0 && j>=0)
                G.remove_edge(i,j);
        }
    } else {
        int n=E.size();
        if (n<2)
            return false;
        for (int k=0;k<n-1;++k) {
            int i=G.node_index(E[k]),j=G.node_index(E[k+1]);
            G.remove_edge(i,j);
        }
    }
    return true;
}

/*
 * Usage:   add_arc(G,e)
 *
 * Returns graph G (which must be directed) with added arc e (or trail or list
 * of arcs).
 */
gen _add_arc(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->front().type!=_VECT || g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &E=*g._VECTptr->back()._VECTptr;
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    if (!parse_edges(G,E))
        return gendimerr(contextptr);
    return G.to_gen();
}
static const char _add_arc_s []="add_arc";
static define_unary_function_eval(__add_arc,&_add_arc,_add_arc_s);
define_unary_function_ptr5(at_add_arc,alias_at_add_arc,&__add_arc,0,true)

/*
 * Usage:   delete_arc(G,e)
 *
 * Returns graph G (which must be directed) with arc e (or trail or list of
 * arcs) removed.
 */
gen _delete_arc(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->front().type!=_VECT || g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &E=*g._VECTptr->back()._VECTptr;
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    if (!delete_edges(G,E))
        return gendimerr(contextptr);
    return G.to_gen();
}
static const char _delete_arc_s []="delete_arc";
static define_unary_function_eval(__delete_arc,&_delete_arc,_delete_arc_s);
define_unary_function_ptr5(at_delete_arc,alias_at_delete_arc,&__delete_arc,0,true)

/*
 * Usage:   add_edge(G,e)
 *
 * Returns graph G (which must be undirected) with added edge e (or trail or
 * list of edges).
 */
gen _add_edge(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->front().type!=_VECT || g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &E=*g._VECTptr->back()._VECTptr;
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    if (!parse_edges(G,E))
        return gendimerr(contextptr);
    return G.to_gen();
}
static const char _add_edge_s []="add_edge";
static define_unary_function_eval(__add_edge,&_add_edge,_add_edge_s);
define_unary_function_ptr5(at_add_edge,alias_at_add_edge,&__add_edge,0,true)

/*
 * Usage:   delete_edge(G,e)
 *
 * Returns graph G (which must be undirected) with edge e (or trail or list of
 * edges) removed.
 */
gen _delete_edge(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->front().type!=_VECT || g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &E=*g._VECTptr->back()._VECTptr;
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    if (!delete_edges(G,E))
        return gendimerr(contextptr);
    return G.to_gen();
}
static const char _delete_edge_s []="delete_edge";
static define_unary_function_eval(__delete_edge,&_delete_edge,_delete_edge_s);
define_unary_function_ptr5(at_delete_edge,alias_at_delete_edge,&__delete_edge,0,true)

/*
 * Usage:   add_vertex(G,v)
 *
 * Returns graph G with added vertex v (or vertices from v if v is a list).
 */
gen _add_vertex(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->front().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    gen &V=g._VECTptr->back();
    if (V.type==_VECT)
        G.add_nodes(*V._VECTptr);
    else
        G.add_node(V);
    return G.to_gen();
}
static const char _add_vertex_s []="add_vertex";
static define_unary_function_eval(__add_vertex,&_add_vertex,_add_vertex_s);
define_unary_function_ptr5(at_add_vertex,alias_at_add_vertex,&__add_vertex,0,true)

/*
 * Usage:   delete_vertex(G,v)
 *
 * Returns graph G with vertex v (or vertices from v if v is a list) removed.
 */
gen _delete_vertex(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->front().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    gen &V=g._VECTptr->back();
    if (V.type==_VECT) {
        G.remove_nodes(*V._VECTptr);
    } else {
        if (!G.remove_node(V))
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    }
    return G.to_gen();
}
static const char _delete_vertex_s []="delete_vertex";
static define_unary_function_eval(__delete_vertex,&_delete_vertex,_delete_vertex_s);
define_unary_function_ptr5(at_delete_vertex,alias_at_delete_vertex,&__delete_vertex,0,true)

/*
 * Usage:   contract_edge(G,E)
 *
 * Returns graph G with edge E contracted (collapsed).
 */
gen _contract_edge(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->front().type!=_VECT || g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur &E=*g._VECTptr->back()._VECTptr;
    if (E.size()!=2)
        return gensizeerr(contextptr);
    int i=G.node_index(E.front()),j=G.node_index(E.back());
    if (i<0 || j<0 || !G.has_edge(i,j))
        return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
    G.collapse_edge(i,j);
    G.remove_node(j);
    return G.to_gen();
}
static const char _contract_edge_s []="contract_edge";
static define_unary_function_eval(__contract_edge,&_contract_edge,_contract_edge_s);
define_unary_function_ptr5(at_contract_edge,alias_at_contract_edge,&__contract_edge,0,true)

/*
 * Usage:   connected_components(G)
 *
 * Returns list of lists of vertices, each sublist representing a connected
 * component of graph G. Individual components can be made available as
 * subgraphs of G by applying the induced_subgraph command.
 */
gen _connected_components(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vector<graphe::ivector> components;
    G.connected_components(components);
    vecteur res;
    for (vector<graphe::ivector>::const_iterator it=components.begin();it!=components.end();++it) {
        vecteur component;
        for (graphe::ivector::const_iterator jt=it->begin();jt!=it->end();++jt) {
            component.push_back(G.node(*jt));
        }
        res.push_back(component);
    }
    return res;
}
static const char _connected_components_s []="connected_components";
static define_unary_function_eval(__connected_components,&_connected_components,_connected_components_s);
define_unary_function_ptr5(at_connected_components,alias_at_connected_components,&__connected_components,0,true)

gen flights(const gen &g,bool arrive,bool all,GIAC_CONTEXT) {
    if (!all && g._VECTptr->front().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(all?*g._VECTptr:*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    int i=0;
    if (!all) {
        i=G.node_index(g._VECTptr->at(1));
        if (i==-1)
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    }
    vecteur res;
    do {
        graphe::ivector adj=G.adjacent_nodes(i);
        vecteur v;
        for (graphe::ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
            if (G.has_edge(arrive?*it:i,arrive?i:*it))
                v.push_back(G.node(*it));
        }
        if (!all)
            return v;
        res.push_back(v);
    } while (i++<G.node_count());
    return res;
}

/*
 * Usage:   departures(G,[v])
 *
 * Returns list of vertices of directed graph G which are connected by v with
 * arcs such that tails are in v. If v is omitted, list of departures is
 * computed for every vertex and a list of lists is returned.
 */
gen _departures(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    return flights(g,false,g.subtype!=_SEQ__VECT,contextptr);
}
static const char _departures_s []="departures";
static define_unary_function_eval(__departures,&_departures,_departures_s);
define_unary_function_ptr5(at_departures,alias_at_departures,&__departures,0,true)

/*
 * Usage:   arrivals(G,[v])
 *
 * Returns list of vertices of directed graph G which are connected by v with
 * arcs such that heads are in v. If v is omitted, list of departures is
 * computed for every vertex and a list of lists is returned.
 */
gen _arrivals(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    return flights(g,true,g.subtype!=_SEQ__VECT,contextptr);
}
static const char _arrivals_s []="arrivals";
static define_unary_function_eval(__arrivals,&_arrivals,_arrivals_s);
define_unary_function_ptr5(at_arrivals,alias_at_arrivals,&__arrivals,0,true)

/*
 * Usage:   incident_edges(G,v)
 *
 * Returns list of all edges incident to vertex v (or vertices in the list v).
 */
gen _incident_edges(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur V;
    if (g._VECTptr->back().type==_VECT)
        V=*g._VECTptr->back()._VECTptr;
    else
        V.push_back(g._VECTptr->back());
    graphe::ivector indices;
    int i;
    for (const_iterateur it=V.begin();it!=V.end();++it) {
        if ((i=G.node_index(*it))==-1)
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        indices.push_back(i);
    }
    vector<graphe::ipair> E=G.incident_edges(indices);
    vecteur res;
    for (vector<graphe::ipair>::const_iterator it=E.begin();it!=E.end();++it) {
        res.push_back(makevecteur(G.node(it->first),G.node(it->second)));
    }
    return res;
}
static const char _incident_edges_s []="incident_edges";
static define_unary_function_eval(__incident_edges,&_incident_edges,_incident_edges_s);
define_unary_function_ptr5(at_incident_edges,alias_at_incident_edges,&__incident_edges,0,true)

/*
 * Usage:   make_weighted(G,[M])
 *
 * Returns graph G with edge/arc weights set as specified by matrix M. If M is
 * omitted, a suqare matrix of ones is used. If G is undirected, M is assumed
 * to be symmetric.
 */
gen _make_weighted(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    bool has_matrix=g.subtype==_SEQ__VECT;
    if (has_matrix && g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    graphe G;
    if (!G.read_gen(has_matrix?*g._VECTptr->front()._VECTptr:*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int n=G.node_count();
    matrice m=*_matrix(makesequence(n,n,1),contextptr)._VECTptr;
    if (has_matrix) {
        m=*g._VECTptr->back()._VECTptr;
        if (int(m.size())!=n || int(m.front()._VECTptr->size())!=n)
            return gendimerr(contextptr);
    }
    G.make_weighted(m);
    return G.to_gen();
}

bool parse_attribute(const gen &g,gen_map &m) {
    assert(g.type==_SYMB);
    vecteur &f=*g._SYMBptr->feuille._VECTptr;
    gen tag;
    if (f.size()!=2 || (tag=f.front()).type!=_STRNG)
        return false;
    m[tag]=f.back();
    return true;
}

bool parse_attributes(const gen &g,gen_map &m,GIAC_CONTEXT) {
    if (g.is_symb_of_sommet(at_equal))
        return parse_attribute(g,m);
    if (g.type==_VECT) {
        for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
            if (!parse_attribute(*it,m))
                return false;
        }
        return true;
    }
    return false;
}

bool parse_gen_attr(const gen &g,gen_map &m,int k,GIAC_CONTEXT) {
    if (g._VECTptr->at(2).type==_VECT) {
        if (!parse_attributes(g._VECTptr->at(k),m,contextptr))
            return false;
    } else for (const_iterateur it=g._VECTptr->begin()+k;it!=g._VECTptr->end();++it) {
        if (!parse_attribute(*it,m))
            return false;
    }
    return true;
}

/*
 * Usage:   set_graph_attribute(G,[v or E],attr)
 *
 * Stores the attribute attr (or attributes from list attr) in form tag=value,
 * where tag is string, (to vertex v or edge E of graph G) and return G.
 */
gen _set_graph_attribute(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    gen &v=g._VECTptr->at(1);
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int i=-1,j=-1,k=2,key;
    if (v.type==_VECT) {
        // edge attribute
        if (v._VECTptr->size()!=2)
            return gensizeerr(contextptr);
        i=G.node_index(v._VECTptr->front());
        j=G.node_index(v._VECTptr->back());
        if (i==-1 || j==-1)
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        if (!G.has_edge(i,j))
            return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
    } else if (v.is_symb_of_sommet(at_equal)) {
        // graph attribute
        k=1;
    } else {
        // vertex attribute
        if ((i=G.node_index(v))==-1)
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    }
    gen_map m;
    if (!parse_gen_attr(g,m,k,contextptr))
        return gentypeerr(contextptr);
    for (gen_map::const_iterator it=m.begin();it!=m.end();++it) {
        key=G.tag2index(graphe::genstring2str(it->first));
        if (i<0 && j<0)
            G.set_graph_attribute(key,it->second);
        else if (j<0)
            G.set_node_attribute(i,key,it->second);
        else
            G.set_edge_attribute(i,j,key,it->second);
    }
    return G.to_gen();
}
static const char _set_graph_attribute_s []="set_graph_attribute";
static define_unary_function_eval(__set_graph_attribute,&_set_graph_attribute,_set_graph_attribute_s);
define_unary_function_ptr5(at_set_graph_attribute,alias_at_set_graph_attribute,&__set_graph_attribute,0,true)

/*
 * Usage:   set_graph_attribute(G,[v or E],attr)
 *
 * Stores the attribute attr (or attributes from list attr) in form tag=value,
 * where tag is string, (to vertex v or edge E of graph G) and returns G.
 */
gen _get_graph_attribute(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    gen &attr=g._VECTptr->back();
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int i,j;
    vector<string> tags;
    if (attr.type==_VECT) {
        // list of tags
        for (const_iterateur it=attr._VECTptr->begin();it!=attr._VECTptr->end();++it) {
            if (it->type!=_STRNG)
                return gentypeerr(contextptr);
            tags.push_back(graphe::genstring2str(*it));
        }
    } else if (attr.type==_STRNG) {
        // a single tag
        tags.push_back(graphe::genstring2str(attr));
    } else return gentypeerr(contextptr);
    vecteur res;
    gen value;
    if (g._VECTptr->size()==2) {
        // get graph attribute(s)
        for (vector<string>::const_iterator it=tags.begin();it!=tags.end();++it) {
            G.get_graph_attribute(G.tag2index(*it),value);
            res.push_back(value);
        }
    } else if (g._VECTptr->size()==3) {
        gen &v=g._VECTptr->at(1);
        if (v.type==_VECT) {
            // get edge attribute(s)
            if (v._VECTptr->size()!=2)
                return gensizeerr(contextptr);
            i=G.node_index(v._VECTptr->front());
            j=G.node_index(v._VECTptr->back());
            if (i==-1 || j==-1)
                return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
            if (!G.has_edge(i,j))
                return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
            for (vector<string>::const_iterator it=tags.begin();it!=tags.end();++it) {
                G.get_edge_attribute(i,j,G.tag2index(*it),value);
                res.push_back(value);
            }
        } else {
            i=G.node_index(v);
            if (i==-1)
                return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
            for (vector<string>::const_iterator it=tags.begin();it!=tags.end();++it) {
                G.get_node_attribute(i,G.tag2index(*it),value);
                res.push_back(value);
            }
        }
    }
    return res;
}
static const char _get_graph_attribute_s []="get_graph_attribute";
static define_unary_function_eval(__get_graph_attribute,&_get_graph_attribute,_get_graph_attribute_s);
define_unary_function_ptr5(at_get_graph_attribute,alias_at_get_graph_attribute,&__get_graph_attribute,0,true)

/*
 * Usage:   number_of_edges(G)
 *
 * Returns number of edges/arcs of graph G.
 */
gen _number_of_edges(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return G.edge_count();
}
static const char _number_of_edges_s []="number_of_edges";
static define_unary_function_eval(__number_of_edges,&_number_of_edges,_number_of_edges_s);
define_unary_function_ptr5(at_number_of_edges,alias_at_number_of_edges,&__number_of_edges,0,true)

/*
 * Usage:   number_of_vertices(G)
 *
 * Returns number of vertices of graph G.
 */
gen _number_of_vertices(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return G.node_count();
}
static const char _number_of_vertices_s []="number_of_vertices";
static define_unary_function_eval(__number_of_vertices,&_number_of_vertices,_number_of_vertices_s);
define_unary_function_ptr5(at_number_of_vertices,alias_at_number_of_vertices,&__number_of_vertices,0,true)

/*
 * Usage:   get_edge_weight(G,E)
 *
 * Returns weight of edge E (graph G must be weighted).
 */
gen _get_edge_weight(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()!=2)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_weighted())
        return gt_err(_GT_ERR_WEIGHTED_GRAPH_REQUIRED,contextptr);
    gen &E=g._VECTptr->back();
    if (E.type!=_VECT || E._VECTptr->size()!=2)
        return gentypeerr(contextptr);
    int i=G.node_index(E._VECTptr->front()),j=G.node_index(E._VECTptr->back());
    if (i==-1 || j==-1)
        return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
    return G.weight(i,j);
}
static const char _get_edge_weight_s []="get_edge_weight";
static define_unary_function_eval(__get_edge_weight,&_get_edge_weight,_get_edge_weight_s);
define_unary_function_ptr5(at_get_edge_weight,alias_at_get_edge_weight,&__get_edge_weight,0,true)

/*
 * Usage:   set_edge_weight(G,E,w)
 *
 * Sets weight of edge E (graph G must be weighted) to w and returns the old weight.
 */
gen _set_edge_weight(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()!=3)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_weighted())
        return gt_err(_GT_ERR_WEIGHTED_GRAPH_REQUIRED,contextptr);
    gen &E=g._VECTptr->at(1);
    if (E.type!=_VECT || E._VECTptr->size()!=2)
        return gentypeerr(contextptr);
    int i=G.node_index(E._VECTptr->front()),j=G.node_index(E._VECTptr->back());
    if (i==-1 || j==-1)
        return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
    gen oldweight=G.weight(i,j);
    G.set_edge_attribute(i,j,_GT_ATTRIB_WEIGHT,g._VECTptr->back());
    return oldweight;
}
static const char _set_edge_weight_s []="set_edge_weight";
static define_unary_function_eval(__set_edge_weight,&_set_edge_weight,_set_edge_weight_s);
define_unary_function_ptr5(at_set_edge_weight,alias_at_set_edge_weight,&__set_edge_weight,0,true)

/*
 * Usage:   is_directed(G)
 *
 * Returns true if G is a directed graph, else returns false.
 */
gen _is_directed(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_directed());
}
static const char _is_directed_s []="is_directed";
static define_unary_function_eval(__is_directed,&_is_directed,_is_directed_s);
define_unary_function_ptr5(at_is_directed,alias_at_is_directed,&__is_directed,0,true)

/*
 * Usage:   neighbors(G,v)
 *
 * Returns list of adjacent vertices of vertex v.
 */
gen _neighbors(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()!=2)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    gen &v=g._VECTptr->back();
    int i=G.node_index(v);
    if (i==-1)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    graphe::ivector adj=G.adjacent_nodes(i);
    vecteur res;
    for (graphe::ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
        res.push_back(G.node(*it));
    }
    return res;
}
static const char _neighbors_s []="neighbors";
static define_unary_function_eval(__neighbors,&_neighbors,_neighbors_s);
define_unary_function_ptr5(at_neighbors,alias_at_neighbors,&__neighbors,0,true)

/*
 * Usage:   minimum_degree(G)
 *
 * Returns the smallest degree among vertices in graph G.
 */
gen _minimum_degree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.node_count()==0)
        return 0;
    int mindeg=RAND_MAX,d;
    for (int i=0;i<G.node_count();++i) {
        if ((d=G.degree(i))<mindeg)
            mindeg=d;
    }
    return mindeg;
}
static const char _minimum_degree_s []="minimum_degree";
static define_unary_function_eval(__minimum_degree,&_minimum_degree,_minimum_degree_s);
define_unary_function_ptr5(at_minimum_degree,alias_at_minimum_degree,&__minimum_degree,0,true)

/*
 * Usage:   maximum_degree(G)
 *
 * Returns the largest degree among vertices in graph G.
 */
gen _maximum_degree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.node_count()==0)
        return 0;
    int maxdeg=0,d;
    for (int i=0;i<G.node_count();++i) {
        if ((d=G.degree(i))>maxdeg)
            maxdeg=d;
    }
    return maxdeg;
}
static const char _maximum_degree_s []="maximum_degree";
static define_unary_function_eval(__maximum_degree,&_maximum_degree,_maximum_degree_s);
define_unary_function_ptr5(at_maximum_degree,alias_at_maximum_degree,&__maximum_degree,0,true)

/*
 * Usage:   is_regular(G)
 *
 * Returns true if max and min degrees of graph G are equal, else returns false.
 */
gen _is_regular(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.node_count()==0)
        return graphe::boole(false);
    int d=G.degree(0);
    for (int i=1;i<G.node_count();++i) {
        if (G.degree(i)!=d)
            return graphe::boole(false);
    }
    return graphe::boole(true);
}
static const char _is_regular_s []="is_regular";
static define_unary_function_eval(__is_regular,&_is_regular,_is_regular_s);
define_unary_function_ptr5(at_is_regular,alias_at_is_regular,&__is_regular,0,true)

#ifndef NO_NAMESPACE_GIAC
}
#endif // ndef NO_NAMESPACE_GIAC
