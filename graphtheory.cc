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
    "vertex not found",                                                 // 11
    "graph is not a tree",                                              // 12
    "exactly one root node must be specified per connected component",  // 13
    "invalid root node specification",                                  // 14
    "graph is not planar",                                              // 15
    "a connected graph is required",                                    // 16
    "drawing method specification is invalid",                          // 17
    "does not specify a cycle in the given graph",                      // 18
    "no cycle found",                                                   // 19
    "graph name not recognized"                                         // 20
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

bool graphe::parse_list_of_edges(const vecteur &v) {
    int st=-2;
    bool hasweight;
    ipair c;
    for (const_iterateur it=v.begin();it!=v.end();++it) {
        if (it->type!=_VECT || int(it->_VECTptr->size())<2) {
            gt_err_display(*it,_GT_ERR_INVALID_EDGE,ctx);
            return false;
        }
        vecteur &e = *it->_VECTptr;
        hasweight=e.front().type==_VECT;
        gen edge(hasweight?e.front():*it);
        if (int(edge._VECTptr->size())!=2)
            gt_err_display(edge,_GT_ERR_INVALID_EDGE,ctx);
        if (st==-2)
            st=edge.subtype;
        else if (st!=edge.subtype) {
            gt_err_display(_GT_ERR_INVALID_EDGE_ARC_MIX,ctx);
            return false;
        }
        c=add_edge(edge._VECTptr->front(),edge._VECTptr->back());
        if (hasweight)
            set_edge_attribute(c.first,c.second,_GT_ATTRIB_WEIGHT,e[1]);
    }
    set_directed(st!=_SET__VECT);
    return true;
}

bool graphe::parse_matrix(const matrice &m,bool iswei,int mode) {
    int n=m.size(),vi,vj;
    if (int(m.front()._VECTptr->size())!=n || (mode>0 && node_count()!=n))
        return false;
    for (int i=0;i<n;++i) {
        vi=mode==0?add_node(i+array_start(ctx)):i;
        for (int j=0;j<n;++j) {
            vj=mode==0?add_node(j+array_start(ctx)):j;
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
 *          graph("name")
 *
 * Create (un)directed (un)weighted graph from list of vertices V, set of edges
 * E, and/or adjacency matrix A containing edge weights. All parameters are
 * optional.
 *
 * 'opts' is a sequence of options containing gt_weighted=true/false,
 * gt_directed=true/false, gt_vertexcolor=c or gt_vertexpositions=p. Here c is
 * integer or list of integers (color(s) to be assigned to vertices (in order))
 * and p is list of coordinates to assign to vertices (used for drawing).
 *
 * A special may be created by specifying its name as a string. Supported names
 * are: clebsch - coxeter - desargues - dodecahedron - durer - dyck - grinberg
 * - grotzsch - heawood - herschel - icosahedron - levi - mcgee - mobius-kantor
 * - nauru - octahedron - pappus - petersen - robertson - soccerball -
 * tehtrahedron
 */
gen _graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    graphe G(contextptr);
    if (g.type==_STRNG) {
        // construct special graph
        string name=graphe::genstring2str(g);
        if (name=="clebsch") {
            for (int i=0;i<16;++i) {
                G.add_node(i);
            }
            G.read_special(graphe::clebsch_graph);
            vecteur labels;
            for (int i=0;i<16;++i) {
                labels.push_back(graphe::to_binary(i,4));
            }
            G.relabel_nodes(labels);
        } else if (name=="coxeter") {
            stringstream ss;
            for (int i=1;i<=7;++i) {
                ss.str("");
                ss << "a" << i;
                G.add_node(graphe::str2gen(ss.str(),true));
            }
            G.read_special(graphe::coxeter_graph);
        } else if (name=="desargues") {
            return _petersen_graph(makesequence(10,3),contextptr);
        } else if (name=="dodecahedron") {
            return _petersen_graph(makesequence(10,2),contextptr);
        } else if (name=="durer") {
            return _petersen_graph(makesequence(6,2),contextptr);
        } else if (name=="dyck") {
            for (int i=1;i<=32;++i) {
                G.add_node(i);
            }
            G.read_special(graphe::dyck_graph);
        } else if (name=="grinberg") {
            for (int i=35;i<=43;++i) {
                G.add_node(i);
            }
            G.read_special(graphe::grinberg_graph);
        } else if (name=="grotzsch") {
            G.read_special(graphe::grotzsch_graph);
        } else if (name=="heawood") {
            G.read_special(graphe::heawood_graph);
        } else if (name=="herschel") {
            for (int i=1;i<=4;++i) {
                G.add_node(i);
            }
            G.read_special(graphe::herschel_graph);
        } else if (name=="icosahedron") {
            for (int i=1;i<=3;++i) {
                G.add_node(i);
            }
            G.read_special(graphe::icosahedron_graph);
        } else if (name=="levi") {
            for (int i=1;i<=30;++i) {
                G.add_node(i);
            }
            G.read_special(graphe::levi_graph);
        } else if (name=="mcgee") {
            for (int i=1;i<=24;++i) {
                G.add_node(i);
            }
            G.read_special(graphe::mcgee_graph);
        } else if (name=="mobius-kantor") {
            return _petersen_graph(makesequence(8,3),contextptr);
        } else if (name=="nauru") {
            return _petersen_graph(makesequence(12,5),contextptr);
        } else if (name=="octahedron") {
            G.add_node(1); G.add_node(3); G.add_node(6);
            G.read_special(graphe::octahedron_graph);
        } else if (name=="pappus") {
            for (int i=1;i<=18;++i) {
                G.add_node(i);
            }
            G.read_special(graphe::pappus_graph);
        } else if (name=="petersen") {
            return _petersen_graph(makesequence(5,2),contextptr);
        } else if (name=="robertson") {
            for (int i=1;i<=19;++i) {
                G.add_node(i);
            }
            G.read_special(graphe::robertson_graph);
        } else if (name=="soccerball") {
            for (int i=16;i<=20;++i) {
                G.add_node(i);
            }
            G.read_special(graphe::soccer_ball_graph);
        } else if (name=="tetrahedron") {
            for (int i=2;i<=4;++i) {
                G.add_node(i);
            }
            G.read_special(graphe::tetrahedron_graph);
        } else return gt_err(_GT_ERR_NAME_NOT_RECOGNIZED,contextptr);
    } else {
        if (ckmatrix(g)) {
            // adjacency matrix is given
            G.parse_matrix(*g._VECTptr,false,0);
        } else if (g.type==_VECT && g.subtype!=_SEQ__VECT) {
            // list of vertices or set of edges is given
            if (g.subtype==_SET__VECT)
                G.parse_list_of_edges(*g._VECTptr);
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
                if (!G.parse_matrix(m,i==2 || iswei,i))
                    return gentypeerr(contextptr);
            } else if (i<2 && args[i].type==_VECT) {
                int permu_size;
                if (args[i].subtype==_SET__VECT) {
                    // set of edges
                    if (!G.parse_list_of_edges(*args[i]._VECTptr))
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
    graphe G(contextptr);
    if (gr.type!=_VECT || name.type!=_STRNG)
        return gentypeerr(contextptr);
    if (!G.read_gen(*gr._VECTptr))
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
    graphe G(contextptr);
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
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
    graphe G(contextptr);
    if (g.subtype==_SEQ__VECT) {
        if (int(g._VECTptr->size())!=2)
            return gensizeerr(contextptr);
        if (g._VECTptr->front().type!=_VECT)
            return gentypeerr(contextptr);
        if (g._VECTptr->back().is_integer() && g._VECTptr->back().val==_GT_OPT_WEIGHTS)
            include_weights=true;
        if (!G.read_gen(*g._VECTptr->front()._VECTptr))
            return gentypeerr(contextptr);
    } else if (!G.read_gen(*g._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*gv.front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*gv.front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr) || !G.is_weighted())
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
    graphe G(contextptr),C(contextptr);
    if (!G.read_gen(*g._VECTptr))
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
    graphe G(contextptr);
    graphe::ipair edge;
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    graphe S(G);
    for (const_iterateur it=ev.begin();it!=ev.end();++it) {
        if (it->type!=_VECT || int(it->_VECTptr->size())!=2)
            return gentypeerr(contextptr);
        edge=S.make_edge(*it->_VECTptr);
        if (!G.has_edge(edge))
            return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
        S.add_node(G.node_label(edge.first),G.node_attributes(edge.first));
        S.add_node(G.node_label(edge.second),G.node_attributes(edge.second));
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    G.induce_subgraph(vi,S);
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vector<int> mis=G.maximal_independent_set();
    int n=mis.size();
    vecteur V(n);
    for (int i=0;i<n;++i) {
        V[i]=G.node_label(mis[i]);
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    vector<graphe::ipair> M;
    G.maximize_matching(M);
    vecteur res;
    for (vector<graphe::ipair>::const_iterator it=M.begin();it!=M.end();++it) {
        res.push_back(makevecteur(G.node_label(it->first),G.node_label(it->second)));
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
    graphe G(contextptr);
    if (!G.read_gen(g.subtype==_SEQ__VECT?*g._VECTptr->front()._VECTptr:*g._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    graphe U;
    G.underlying(U);
    return U.to_gen();
}
static const char _underlying_graph_s []="underlying_graph";
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
    graphe G(contextptr);
    if (g.type==_VECT)
        V=*g._VECTptr;
    else if (g.is_integer())
        G.make_default_vertex_labels(V,g.val,0);
    G.make_cycle(V);
    stringstream ss;
    ss << "C" << G.node_count();
    G.set_name(ss.str());
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
    graphe G(contextptr);
    vecteur V;
    G.make_default_vertex_labels(V,n,0);
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
    graphe G(contextptr);
    for (int i=0;i<N;++i) {
        G.add_node(graphe::to_binary(i,n));
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
 * Returns a copy of graph G in which edges between vertices in list V and
 * vertices not in V are inverted, i.e. replaced with a set of edges from V to
 * other vertices which is not present in G.
 */
gen _seidel_switch(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())!=2 ||
            g._VECTptr->front().type!=_VECT || g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr),H(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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

/*
 * Usage:   draw_graph(G,[options])
 *
 * Returns the graphic representation of graph G obtained by using
 * various algorithms (which can optionally be fine tuned by appending a
 * sequence of options after the first argument).
 *
 * Supported options are:
 *
 *  - spring: use force-directed method to draw graph G (the default)
 *  - tree[=r or [r1,r2,...]]: draw tree or forest G [with optional
 *    specification of root nodes]
 *  - plane or planar: draw planar graph G
 *  - circle[=<cycle>]: draw graph G as circular using the leading cycle,
 *    otherwise one must be specified
 *  - plot3d: draw 3D representation of graph G (possible only with the
 *    spring method)
 *  - labels=true or false: draw (the default) or suppress node labels and
 *    weights
 *
 * An exception is raised if a method is specified but the corresponding
 * necessary conditions are not met.
 */
gen _draw_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    bool has_opts=false,labels=true;
    if (g.type!=_VECT || ((has_opts=g.subtype==_SEQ__VECT) && g._VECTptr->front().type!=_VECT))
        return gentypeerr(contextptr);
    graphe G_orig(contextptr);
    if (!G_orig.read_gen(has_opts?*g._VECTptr->front()._VECTptr:*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur root_nodes,cycle;
    int method=_GT_STYLE_DEFAULT;
    if (has_opts) {
        // parse options
        int opt_counter;
        for (const_iterateur it=g._VECTptr->begin()+1;it!=g._VECTptr->end();++it) {
            opt_counter++;
            const gen &opt=*it;
            if (opt.is_symb_of_sommet(at_equal)) {
                gen &lh=opt._SYMBptr->feuille._VECTptr->front();
                gen &rh=opt._SYMBptr->feuille._VECTptr->back();
                if (lh.is_integer()) {
                    switch (lh.val) {
                    case _GT_TREE:
                        if (rh.type==_VECT)
                            root_nodes=*rh._VECTptr;
                        else
                            root_nodes.push_back(rh);
                        method=_GT_STYLE_TREE;
                        break;
                    case _LABELS:
                        if (!rh.is_integer())
                            return gentypeerr(contextptr);
                        labels=(bool)rh.val;
                        opt_counter--;
                        break;
                    }
                } else if (lh==at_cercle) {
                    if (rh.type!=_VECT)
                        return gentypeerr(contextptr);
                    cycle=*rh._VECTptr;
                    method=_GT_STYLE_CIRCLE;
                }
            } else if (opt==at_cercle)
                method=_GT_STYLE_CIRCLE;
            else if (opt==at_plan)
                method=_GT_STYLE_PLANAR;
            else if (opt==at_plot3d)
                method=_GT_STYLE_3D;
            else if (opt.is_integer()) {
                switch (opt.val) {
                case _GT_TREE:
                    method=_GT_STYLE_TREE;
                    break;
                case _GT_SPRING:
                    method=_GT_STYLE_SPRING;
                    break;
                case _GT_PLANAR:
                    method=_GT_STYLE_PLANAR;
                }
            }
        }
        if (opt_counter>1)
            return gt_err(_GT_ERR_INVALID_DRAWING_METHOD,contextptr);
    }
    graphe G(contextptr);
    G_orig.underlying(G);
    int n=G.node_count(),i,comp_method=method;
    vecteur drawing;
    graphe::layout main_layout(n);
    if (method==_GT_STYLE_3D) {
        G.make_spring_layout(main_layout,3);
    } else {
        vecteur V,original_labels=G.vertices();
        graphe::ivectors components;
        G.connected_components(components);
        int nc=components.size();
        graphe::ivector roots,outerface;
        if (!root_nodes.empty()) {
            // get the root nodes for forest drawing
            if (int(root_nodes.size())!=nc)
                return gt_err(_GT_ERR_INVALID_NUMBER_OF_ROOTS,contextptr);
            graphe::ivector indices(nc);
            roots.resize(nc);
            for (const_iterateur it=root_nodes.begin();it!=root_nodes.end();++it) {
                i=G.node_index(*it);
                if (i==-1)
                    return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
                indices[it-root_nodes.begin()]=i;
            }
            for (int i=0;i<nc;++i) {
                const graphe::ivector &comp=components[i];
                graphe::ivector::iterator it=indices.begin();
                for (;it!=indices.end();++it) {
                    if (find(comp.begin(),comp.end(),*it)!=comp.end())
                        break;
                }
                if (it==indices.end())
                    return gt_err(_GT_ERR_INVALID_ROOT,contextptr);
                roots[i]=*it;
                indices.erase(it);
            }
        }
        if (!cycle.empty()) {
            if (nc>1)
                return gt_err(_GT_ERR_CONNECTED_GRAPH_REQUIRED,contextptr);
            // get the outer face for circular drawing
            int m=cycle.size();
            outerface.resize(m);
            for (const_iterateur it=cycle.begin();it!=cycle.end();++it) {
                i=G.node_index(*it);
                if (i==-1)
                    return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
                outerface[it-cycle.begin()]=i;
            }
            // check if the specification is indeed a cycle
            for (i=0;i<m;++i) {
                if (!G.has_edge(outerface[i],outerface[(i+1)%m]))
                    return gt_err(cycle,_GT_ERR_NOT_A_CYCLE,contextptr);
            }
        }
        G.make_default_vertex_labels(V,n,0);
        G.relabel_nodes(V);
        vector<graphe::layout> layouts(nc);
        vector<graphe::rectangle> bounding_rects(nc);
        bool check=method!=_GT_STYLE_DEFAULT;
        double sep=1.0;
        // draw the components separately
        for (graphe::ivectors_iter it=components.begin();it!=components.end();++it) {
            i=it-components.begin();
            graphe C(contextptr);
            G.induce_subgraph(*it,C,false);
            graphe::layout &x=layouts[i];
            if (method==_GT_STYLE_DEFAULT)
                comp_method=C.guess_drawing_style();
            switch (comp_method) {
            case _GT_STYLE_SPRING:
                C.make_spring_layout(x,2);
                break;
            case _GT_STYLE_TREE:
                if (check && !C.is_tree())
                    return gt_err(_GT_ERR_NOT_A_TREE,contextptr);
                C.make_tree_layout(x,sep,roots.empty()?0:roots[i]);
                break;
            case _GT_STYLE_PLANAR:
                if (!C.make_planar_layout(x))
                    return gt_err(_GT_ERR_NOT_PLANAR,contextptr);
                break;
            case _GT_STYLE_CIRCLE:
                if (outerface.empty()) {
                    if (!C.get_leading_cycle(outerface)) {
                        outerface=C.find_cycle();
                        if (outerface.empty())
                            return gt_err(_GT_ERR_CYCLE_NOT_FOUND,contextptr);
                    }
                    C.make_circular_layout(x,outerface);
                    outerface.clear();
                } else
                    C.make_circular_layout(x,outerface);
                break;
            }
            if (comp_method!=_GT_STYLE_TREE) {
                C.layout_best_rotation(x);
                graphe::scale_layout(x,std::sqrt((double)C.node_count()));
            }
        }
        // combine component layouts

    }
    G_orig.draw_edges(drawing,main_layout);
    G_orig.draw_nodes(drawing,main_layout);
    if (labels)
        G_orig.draw_labels(drawing,main_layout);
    return drawing;
}
static const char _draw_graph_s []="draw_graph";
static define_unary_function_eval(__draw_graph,&_draw_graph,_draw_graph_s);
define_unary_function_ptr5(at_draw_graph,alias_at_draw_graph,&__draw_graph,0,true)

/*
 * Usage:   sierpinski_graph(n,k,[triangle])
 *
 * Returns Sierpinski (triangle) graph S(n,k) (resp. ST(n,k)).
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
    graphe G(contextptr);
    G.make_sierpinski_graph(n,k,trng);
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
    graphe G(contextptr);
    if (g.type==_VECT && g.subtype!=_SEQ__VECT) {
        G.make_complete_graph(*g._VECTptr);
    } else if (g.is_integer() && g.val>0){
        vecteur V;
        G.make_default_vertex_labels(V,g.val,0);
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
        G.make_complete_multipartite_graph(partition_sizes);
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
    graphe G(contextptr);
    G.make_petersen_graph(n,k);
    return G.to_gen();
}
static const char _petersen_graph_s []="petersen_graph";
static define_unary_function_eval(__petersen_graph,&_petersen_graph,_petersen_graph_s);
define_unary_function_ptr5(at_petersen_graph,alias_at_petersen_graph,&__petersen_graph,0,true)

gen randomgraph(const vecteur &gv,bool directed,GIAC_CONTEXT) {
    graphe G(contextptr);
    vecteur V;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    if (gv.front().type==_VECT)
        V=*gv.front()._VECTptr;
    else if (gv.front().is_integer())
        G.make_default_vertex_labels(V,gv.front().val,0);
    else
        return gentypeerr(contextptr);
    if (!is_strictly_positive(gv.back(),contextptr))
        return gentypeerr(contextptr);
    G.make_random(directed,V,gv.back().DOUBLE_val());
    return G.to_gen();
}

/*
 * Usage:   random_graph(n or V,p)
 *          random_graph(n or V,m)
 *
 * Returns a random undirected unweighted graph with n vertices where two
 * vertices are connected with probability p. Alternatively, m edges are
 * created at random. Instead of number n of vertices, a list V of vertex
 * labels may be specified.
 */
gen _random_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    return randomgraph(*g._VECTptr,false,contextptr);
}
static const char _random_graph_s []="random_graph";
static define_unary_function_eval(__random_graph,&_random_graph,_random_graph_s);
define_unary_function_ptr5(at_random_graph,alias_at_random_graph,&__random_graph,0,true)

/*
 * Usage:   random_digraph(n or V,p)
 *          random_digraph(n or V,m)
 *
 * Returns a random directed unweighted graph with n vertices where two
 * vertices are connected with probability p. Alternatively, m edges are
 * created at random. Instead of number n of vertices, a list V of vertex
 * labels may be specified.
 */
gen _random_digraph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    return randomgraph(*g._VECTptr,true,contextptr);
}
static const char _random_digraph_s []="random_digraph";
static define_unary_function_eval(__random_digraph,&_random_digraph,_random_digraph_s);
define_unary_function_ptr5(at_random_digraph,alias_at_random_digraph,&__random_digraph,0,true)

/*
 * Usage:   random_bipartite_graph(n or [a,b],p)
 *          random_bipartite_graph(n or [a,b],m)
 *
 * Returns a random undirected unweighted bipartite graph with n vertices where
 * each possible edge is present with probability p. Alternatively, m edges are
 * created at random. Also, when first argument is list [a,b] of integers, two
 * groups of vertices with sizes a and b are created.
 */
gen _random_bipartite_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    if (!is_strictly_positive(gv.back(),contextptr))
        return gentypeerr(contextptr);
    double p=gv.back().DOUBLE_val();
    int n,a,b;
    vecteur V,W;
    graphe G(contextptr);
    if (gv.front().is_integer()) {
        n=gv.front().val;
        if (n<1)
            return gensizeerr(contextptr);
        a=G.rand_integer(n-1)+1;
        b=n-a;
    } else if (gv.front().type==_VECT && gv.front()._VECTptr->size()==2) {
        vecteur &ab=*gv.front()._VECTptr;
        if (!ab.front().is_integer() || !ab.back().is_integer())
            return gentypeerr(contextptr);
        a=ab.front().val;
        b=ab.back().val;
    }
    G.make_default_vertex_labels(V,a,0);
    G.make_default_vertex_labels(W,b,a);
    G.make_random_bipartite(V,W,p);
    return G.to_gen();
}
static const char _random_bipartite_graph_s []="random_bipartite_graph";
static define_unary_function_eval(__random_bipartite_graph,&_random_bipartite_graph,_random_bipartite_graph_s);
define_unary_function_ptr5(at_random_bipartite_graph,alias_at_random_bipartite_graph,&__random_bipartite_graph,0,true)

/*
 * Usage:   random_tournament(n or V)
 *
 * Returns a random tournament graph with n vertices, which may be specified as
 * list V of their labels.
 */
gen _random_tournament(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (!g.is_integer() || g.val<=0)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    G.set_directed(true);
    int n=g.val;
    for (int i=0;i<n;++i) {
        for (int j=i+1;j<n;++j) {
            if (giac_rand(contextptr)%2==0)
                G.add_edge(i,j);
            else
                G.add_edge(j,i);
        }
    }
    return G.to_gen();
}
static const char _random_tournament_s []="random_tournament";
static define_unary_function_eval(__random_tournament,&_random_tournament,_random_tournament_s);
define_unary_function_ptr5(at_random_tournament,alias_at_random_tournament,&__random_tournament,0,true)

/*
 * Usage:   random_regular_graph(n or V,d,[connected])
 *
 * Returns a random d-regular graph with n vertices, which may be specified as
 * list V of their labels.
 */
gen _random_regular_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    vecteur V;
    vecteur &gv=*g._VECTptr;
    if (gv.front().is_integer())
        G.make_default_vertex_labels(V,gv.front().val,0);
    else if (gv.front().type==_VECT)
        V=*gv.front()._VECTptr;
    else
        return gentypeerr(contextptr);
    if (gv.size()<2)
        return gensizeerr(contextptr);
    if (!gv[1].is_integer() || !is_strictly_positive(gv[1],contextptr))
        return gentypeerr(contextptr);
    int d=gv[1].val;
    bool connected=false;
    if (gv.size()>2 && gv[2].is_integer() && gv[2].val==_GT_CONNECTED)
        connected=true;
    G.make_random_regular(V,d,connected);
    return G.to_gen();
}
static const char _random_regular_graph_s []="random_regular_graph";
static define_unary_function_eval(__random_regular_graph,&_random_regular_graph,_random_regular_graph_s);
define_unary_function_ptr5(at_random_regular_graph,alias_at_random_regular_graph,&__random_regular_graph,0,true)

/*
 * Usage:   random_tree(n or V,[d])
 *
 * Returns a random tree graph with n vertices, which may be specified as list
 * V of their labels. Optional parameter d is a positive integer which
 * represents the upper bound for degree of graph.
 */
gen _random_tree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    int maxd=RAND_MAX,n;
    vecteur V;
    graphe G(contextptr);
    if (g.is_integer()) {
        n=g.val;
    } else if (g.type==_VECT) {
        vecteur &gv=*g._VECTptr;
        if (g.subtype==_SEQ__VECT) {
            if (gv.front().is_integer()) {
                n=gv.front().val;
            } else if (gv.front().type==_VECT) {
                V=*gv.front()._VECTptr;
                n=V.size();
            } else
                return gentypeerr(contextptr);
            if (gv.size()>1 && gv[1].is_integer()) {
                maxd=gv[1].val;
                if (maxd<1)
                    return gensizeerr(contextptr);
            }
        } else {
            V=gv;
            n=V.size();
        }
    }
    if (n<1)
        return gensizeerr(contextptr);
    if (V.empty())
        G.make_default_vertex_labels(V,n,0);
    G.make_random_tree(V,maxd);
    return G.to_gen();
}
static const char _random_tree_s []="random_tree";
static define_unary_function_eval(__random_tree,&_random_tree,_random_tree_s);
define_unary_function_ptr5(at_random_tree,alias_at_random_tree,&__random_tree,0,true)

/*
 * Usage:   random_planar_graph(n or V)
 *
 * Return random biconnected planar graph with n vertices, which can also be
 * specified as a list V of their labels.
 */
gen _random_planar_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    vecteur V;
    graphe G(contextptr);
    if (g.type==_VECT)
        V=*g._VECTptr;
    else if (g.is_integer()) {
        int n=g.val;
        if (n<1)
            return gensizeerr(contextptr);
        G.make_default_vertex_labels(V,n,0);
    } else
        return gentypeerr(contextptr);
    G.make_random_planar(V);
    return G.to_gen();
}
static const char _random_planar_graph_s []="random_planar_graph";
static define_unary_function_eval(__random_planar_graph,&_random_planar_graph,_random_planar_graph_s);
define_unary_function_ptr5(at_random_planar_graph,alias_at_random_planar_graph,&__random_planar_graph,0,true)


/*
 * Usage:   assign_edge_weights(G,m,n)
 *          random_edge_weights(G,a..b)
 *
 * Assigns random edge weights to the edges of graph G and returns a modified
 * copy of G. If integers n and m such that n>=m are specified, weights are
 * integers randomly chosen in [m,n]. If an interval a..b is specified, weights
 * are uniformly distributed in the interval [a,b).
 */
gen _assign_edge_weights(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.front().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(*gv.front()._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (gv.size()==3) {
        if (!gv[1].is_integer() || !gv[2].is_integer())
            return gentypeerr(contextptr);
        int m=gv[1].val,n=gv[2].val;
        if (m>n)
            return gensizeerr(contextptr);
        G.randomize_edge_weights(m,n,true);
    } else if (gv.size()==2) {
        if (!gv.back().is_symb_of_sommet(at_interval))
            return gentypeerr(contextptr);
        gen a=gv.back()._SYMBptr->feuille._VECTptr->front(),
                b=gv.back()._SYMBptr->feuille._VECTptr->back();
        if (_evalf(a,contextptr).type!=_DOUBLE_ || _evalf(b,contextptr).type!=_DOUBLE_)
            return gentypeerr(contextptr);
        G.randomize_edge_weights(a.DOUBLE_val(),b.DOUBLE_val(),false);
    }
    return G.to_gen();
}
static const char _assign_edge_weights_s []="assign_edge_weights";
static define_unary_function_eval(__assign_edge_weights,&_assign_edge_weights,_assign_edge_weights_s);
define_unary_function_ptr5(at_assign_edge_weights,alias_at_assign_edge_weights,&__assign_edge_weights,0,true)

/*
 * Usage:   articulation_points(G)
 *
 * Returns the list of articulation points (i.e. cut vertices) of graph G.
 */
gen _articulation_points(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    graphe::ivector V=G.find_cut_vertices();
    vecteur v;
    for (graphe::ivector::const_iterator it=V.begin();it!=V.end();++it) {
        v.push_back(G.node_label(*it));
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vector<vector<graphe::ipair> > blocks;
    G.find_blocks(blocks);
    vecteur res;
    for (vector<vector<graphe::ipair> >::const_iterator it=blocks.begin();it!=blocks.end();++it) {
        vecteur v;
        for (vector<graphe::ipair>::const_iterator jt=it->begin();jt!=it->end();++jt) {
            v.push_back(makevecteur(G.node_label(jt->first),G.node_label(jt->second)));
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vector<graphe::ivector> components;
    G.connected_components(components);
    vecteur res;
    for (vector<graphe::ivector>::const_iterator it=components.begin();it!=components.end();++it) {
        vecteur component;
        for (graphe::ivector::const_iterator jt=it->begin();jt!=it->end();++jt) {
            component.push_back(G.node_label(*jt));
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
    graphe G(contextptr);
    if (!G.read_gen(all?*g._VECTptr:*g._VECTptr->front()._VECTptr))
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
                v.push_back(G.node_label(*it));
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
        res.push_back(makevecteur(G.node_label(it->first),G.node_label(it->second)));
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
    graphe G(contextptr);
    if (!G.read_gen(has_matrix?*g._VECTptr->front()._VECTptr:*g._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr->front()._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    gen &v=g._VECTptr->back();
    int i=G.node_index(v);
    if (i==-1)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    graphe::ivector adj=G.adjacent_nodes(i);
    vecteur res;
    for (graphe::ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
        res.push_back(G.node_label(*it));
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
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
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
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

/* Usage:   isomorphic_copy(G,sigma)
 *
 * Returns a new graph H with vertices reordered according to permutation sigma.
 */
gen _isomorphic_copy(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    if (gv[1].type!=_VECT || !is_integer_vecteur(*gv[1]._VECTptr))
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(*gv.front()._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur &sigma=*gv[1]._VECTptr;
    graphe::ivector v(sigma.size());
    for (const_iterateur it=sigma.begin();it!=sigma.end();++it) {
        v[it-sigma.begin()]=it->val;
    }
    graphe H;
    if (!G.isomorphic_copy(H,v))
        return gentypeerr(contextptr);
    return H.to_gen();
}
static const char _isomorphic_copy_s []="isomorphic_copy";
static define_unary_function_eval(__isomorphic_copy,&_isomorphic_copy,_isomorphic_copy_s);
define_unary_function_ptr5(at_isomorphic_copy,alias_at_isomorphic_copy,&__isomorphic_copy,0,true)

/* Usage:   relabel_vertices(G,V)
 *
 * Returns a new graph H with vertex labels changed to those in V.
 */
gen _relabel_vertices(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    if (gv[1].type!=_VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(*gv.front()._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur &labels=*gv[1]._VECTptr;
    if (int(labels.size())!=G.node_count())
        return gensizeerr(contextptr);
    if (!G.relabel_nodes(labels))
        return gentypeerr(contextptr);
    return G.to_gen();
}
static const char _relabel_vertices_s []="relabel_vertices";
static define_unary_function_eval(__relabel_vertices,&_relabel_vertices,_relabel_vertices_s);
define_unary_function_ptr5(at_relabel_vertices,alias_at_relabel_vertices,&__relabel_vertices,0,true)

/* Usage:   is_tree(G)
 *
 * Returns true iff graph G is a tree, i.e. an undirected connected graph with
 * exactly n-1 edges, where n is number of nodes.
 */
gen _is_tree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    graphe G(contextptr);
    if (g.type!=_VECT || !G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_tree());
}
static const char _is_tree_s []="is_tree";
static define_unary_function_eval(__is_tree,&_is_tree,_is_tree_s);
define_unary_function_ptr5(at_is_tree,alias_at_is_tree,&__is_tree,0,true)

/* Usage:   is_forest(G)
 *
 * Returns true iff graph G is a forest, i.e. an undirected graph whose
 * connected components are all trees.
 */
gen _is_forest(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    graphe G(contextptr);
    if (g.type!=_VECT || !G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_forest());
}
static const char _is_forest_s []="is_forest";
static define_unary_function_eval(__is_forest,&_is_forest,_is_forest_s);
define_unary_function_ptr5(at_is_forest,alias_at_is_forest,&__is_forest,0,true)

/* Usage:   is_tournament(G)
 *
 * Returns true iff graph G is a tournament, i.e. a complete graph with a
 * direction for each edge.
 */
gen _is_tournament(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    graphe G(contextptr);
    if (g.type!=_VECT || !G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_tournament());
}
static const char _is_tournament_s []="is_tournament";
static define_unary_function_eval(__is_tournament,&_is_tournament,_is_tournament_s);
define_unary_function_ptr5(at_is_tournament,alias_at_is_tournament,&__is_tournament,0,true)

/* Usage:   tree_height(T,r)
 *
 * Returns the height of tree T with vertex r as root node.
 */
gen _tree_height(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (gv.front().type!=_VECT || !G.read_gen(*gv.front()._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.node_count()==1)
        return 0;
    if (!G.is_tree())
        return gt_err(_GT_ERR_NOT_A_TREE,contextptr);
    int root;
    if ((root=G.node_index(gv.back()))==-1)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    return G.tree_height(root);
}
static const char _tree_height_s []="tree_height";
static define_unary_function_eval(__tree_height,&_tree_height,_tree_height_s);
define_unary_function_ptr5(at_tree_height,alias_at_tree_height,&__tree_height,0,true)

/* Usage:   is_connected(G)
 *
 * Returns true iff graph G is connected.
 */
gen _is_connected(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    graphe G(contextptr);
    if (g.type!=_VECT || !G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_connected());
}
static const char _is_connected_s []="is_connected";
static define_unary_function_eval(__is_connected,&_is_connected,_is_connected_s);
define_unary_function_ptr5(at_is_connected,alias_at_is_connected,&__is_connected,0,true)

/* Usage:   is_biconnected(G)
 *
 * Returns true iff graph G is biconnected.
 */
gen _is_biconnected(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    graphe G(contextptr);
    if (g.type!=_VECT || !G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_biconnected());
}
static const char _is_biconnected_s []="is_biconnected";
static define_unary_function_eval(__is_biconnected,&_is_biconnected,_is_biconnected_s);
define_unary_function_ptr5(at_is_biconnected,alias_at_is_biconnected,&__is_biconnected,0,true)

/* Usage:   is_weighted(G)
 *
 * Returns true iff graph G is weighted.
 */
gen _is_weighted(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    graphe G(contextptr);
    if (g.type!=_VECT || !G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_weighted());
}
static const char _is_weighted_s []="is_weighted";
static define_unary_function_eval(__is_weighted,&_is_weighted,_is_weighted_s);
define_unary_function_ptr5(at_is_weighted,alias_at_is_weighted,&__is_weighted,0,true)

/* Usage:   is_planar(G)
 *
 * Returns true iff graph G is planar.
 */
gen _is_planar(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    graphe G(contextptr);
    if (g.type!=_VECT || !G.read_gen(*g._VECTptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_planar());
}
static const char _is_planar_s []="is_planar";
static define_unary_function_eval(__is_planar,&_is_planar,_is_planar_s);
define_unary_function_ptr5(at_is_planar,alias_at_is_planar,&__is_planar,0,true)


#ifndef NO_NAMESPACE_GIAC
}
#endif // ndef NO_NAMESPACE_GIAC
