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

#include "giacPCH.h"
#include "giac.h"
#include "graphe.h"
#include "graphtheory.h"

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
    "graph name not recognized",                                        // 20
    "argument is not a subgraph",                                       // 21
    "graph is empty",                                                   // 22
    "a \"tag\"=value pair expected",                                    // 23
    "the given list is not a valid graphic sequence",                   // 24
    "graph is not acyclic",                                             // 25
    "a biconnected graph is required"                                   // 26
};

void gt_err_display(int code,GIAC_CONTEXT) {
    *logptr(contextptr) << "Error: " << gt_error_messages[code] << endl;
}

void gt_err_display(const gen &g,int code,GIAC_CONTEXT) {
    *logptr(contextptr) << "Error: " << g << ": " << gt_error_messages[code] << endl;
}

gen gt_err(int code,GIAC_CONTEXT) {
    gt_err_display(code,contextptr);
    return gentypeerr(contextptr);
}

gen gt_err(const gen &g,int code,GIAC_CONTEXT) {
    gt_err_display(g,code,contextptr);
    return gentypeerr(contextptr);
}

void identifier_assign(const identificateur &var,const gen &value,GIAC_CONTEXT) {
    _eval(symbolic(at_sto,makesequence(value,var)),contextptr);
}

bool has_suffix(const string &str,const string &suffix)
{
    return str.size()>=suffix.size() &&
            str.compare(str.size()-suffix.size(),suffix.size(),suffix)==0;
}

string strip_string(const string &str) {
    string res(str);
    int i=0;
    for (;res[i]==' ';++i);
    res=res.substr(i);
#if 1
    i=int(res.size())-1;
    for (;i>=0;--i){
      if (res[i]!=' ')
   break;
    }
    res=res.substr(0,i+1);
#else
    while (res.back()==' ') {
        res.pop_back();
    }
#endif
    return res;
}

string make_absolute_file_path(const string &relative_path) {
    string path=string(get_current_dir_name())+"/"+relative_path;
    vector<string> tokens;
    size_t lastpos=0,pos;
    while ((pos=path.find_first_of('/',lastpos))!=string::npos) {
        tokens.push_back(path.substr(lastpos,pos-lastpos));
        lastpos=pos+1;
    }
    tokens.push_back(path.substr(lastpos));
    vector<string>::iterator it;
    for (it=tokens.begin();it!=tokens.end();++it) {
        *it=strip_string(*it);
    }
    int i;
    while ((it=find(tokens.begin(),tokens.end(),string("..")))!=tokens.end()) {
        if (it==tokens.begin())
            return path;
        i=it-tokens.begin()-1;
        tokens.erase(it);
        tokens.erase(tokens.begin()+i);
    }
    string res;
    for (it=tokens.begin();it!=tokens.end();++it) {
        res=res+*it;
        if (it+1!=tokens.end())
            res=res+"/";
    }
    return res;
}

/* returns true iff g is a graph and writes the basic info to 'display_str' */
bool is_graphe(const gen &g,string &disp_out,GIAC_CONTEXT) {
    if (g.type!=_VECT || g.subtype!=_GRAPH__VECT)
        return false;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return false;
    int nv=G.node_count(),ne=G.edge_count();
    stringstream ss;
    ss << nv;
    string nvert(ss.str());
    ss.str("");
    ss << ne;
    bool isdir=G.is_directed();
    string nedg(ss.str());
    string dir_spec=isdir?"directed":"undirected";
    string weight_spec=G.is_weighted()?"weighted":"unweighted";
    nvert=nvert+(nv==1?" vertex":" vertices");
    nedg=nedg+(isdir?(ne==1?" arc":" arcs"):(ne==1?" edge":" edges"));
    disp_out.clear();
    string name=G.name();
    if (!name.empty())
        disp_out=name+": ";
    disp_out=disp_out+(isdir?"a ":"an ")+dir_spec+" "+weight_spec+" graph with "+nvert+" and "+nedg;
    return true;
}

/* evaluates the given command with the given inputs and returns the result */
gen gt_command(gen (*gtfunc)(const gen &,const context *),const char *args,GIAC_CONTEXT) {
    return (*gtfunc)(graphe::str2gen(args),contextptr);
}

bool vertices_from_integer_or_vecteur(const gen &g,graphe &G) {
    vecteur V;
    int n;
    if (g.is_integer()) {
        n=g.val;
        if (n<1)
            return false;
        G.make_default_labels(V,n);
    } else if (g.type==_VECT) {
        V=*g._VECTptr;
        if (V.empty())
            return false;
        n=V.size();
    } else return false;
    G.add_nodes(V);
    return true;
}

void parse_trail(graphe &G,const gen &g) {
    assert(g.is_symb_of_sommet(at_trail));
    vecteur &trail=*g._SYMBptr->feuille._VECTptr;
    int n=trail.size();
    for (int i=0;i<n-1;++i) {
        G.add_edge(trail[i],trail[i+1]);
    }
}

bool parse_vertex_colors(graphe &G,const gen &g,const graphe::ivector &nodes=graphe::ivector(0)) {
    if (g.type==_VECT) {
        assert(int(g._VECTptr->size()==nodes.empty()?G.node_count():int(nodes.size())));
        int k=0;
        for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
            if (!it->is_integer())
                return false;
            G.set_node_attribute(nodes.empty()?k:nodes[k],_GT_ATTRIB_COLOR,it->val);
            ++k;
        }
    } else if (g.is_integer()) {
        if (g.val<0)
            return false;
        if (nodes.empty()) {
            for (int k=G.node_count();k-->0;)
                G.set_node_attribute(k,_GT_ATTRIB_COLOR,g.val);
        } else {
            for (graphe::ivector_iter it=nodes.begin();it!=nodes.end();++it) {
                G.set_node_attribute(*it,_GT_ATTRIB_COLOR,g.val);
            }
        }
    } else return false;
    return true;
}

bool parse_vertex_coordinates(graphe &G,const vecteur &v,bool &size_error) {
    vecteur c;
    int i=0,dim=-1;
    for (const_iterateur it=v.begin();it!=v.end();++it) {
        if (it->is_symb_of_sommet(at_point))
            c=*it->_SYMBptr->feuille._VECTptr;
        else if (it->type==_CPLX) {
            c.resize(2);
            c[0]=*it->_CPLXptr;
            c[1]=*(it->_CPLXptr+1);
        } else if (it->type==_VECT) {
            c=*it->_VECTptr;
        } else return false;
        if (dim<0)
            dim=c.size();
        else if (dim!=int(c.size())) {
            size_error=true;
            return false;
        }
        G.set_node_attribute(i++,_GT_ATTRIB_POSITION,c);
    }
    return true;
}

bool parse_matrix(graphe &G,const matrice &m,bool is_weight_matrix,int mode,bool &size_error) {
    int n=m.size();
    size_error=false;
    if (int(m.front()._VECTptr->size())!=n || (mode>0 && G.node_count()!=n)) {
        size_error=true;
        return false;
    }
    if (!has_num_coeff(_evalf(m,context0)))
        return false;
    bool isdir=G.is_directed() || m!=mtran(m),isweighted=is_weight_matrix;
    if (mode==0) {
        vecteur V;
        G.make_default_labels(V,n);
        G.add_nodes(V);
    }
    if (mode<2) {
        G.set_directed(isdir);
        for (int i=0;i<n;++i) {
            for (int j=isdir?0:i+1;j<n;++j) {
                if (i==j)
                    continue;
                const gen &w=m[i][j];
                if (!is_zero(w)) {
                    G.add_edge(i,j);
                    if (!is_one(w))
                        isweighted=true;
                }
            }
        }
    }
    if (isweighted)
        G.make_weighted(m);
    return true;
}

bool parse_edge_with_weight(graphe &G,const vecteur &E) {
    if (E.size()!=2)
        return false;
    const vecteur &e=*E.front()._VECTptr;
    const gen &w=E.back();
    if (e.size()!=2)
        return false;
    if (!G.is_weighted())
        G.set_weighted(true);
    G.add_edge(e.front(),e.back(),w);
    return true;
}

bool parse_edges(graphe &G,const vecteur &E,bool is_set) {
    if (is_set) {
        for (const_iterateur it=E.begin();it!=E.end();++it) {
            if (it->type!=_VECT || it->_VECTptr->size()!=2)
                return false;
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

gen flights(const gen &g,bool arrive,bool all,GIAC_CONTEXT) {
    if (!all && g._VECTptr->front().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(all?g:g._VECTptr->front()))
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
        graphe::ivector adj;
        G.adjacent_nodes(i,adj);
        vecteur v;
        for (graphe::ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
            if (G.has_edge(arrive?*it:i,arrive?i:*it))
                v.push_back(G.node_label(*it));
        }
        if (!all)
            return v;
        res.push_back(_sort(v,contextptr));
    } while (++i<G.node_count());
    return res;
}

gen randomgraph(const vecteur &gv,bool directed,GIAC_CONTEXT) {
    graphe G(contextptr);
    vecteur V;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    if (gv.front().type==_VECT)
        V=*gv.front()._VECTptr;
    else if (gv.front().is_integer())
        G.make_default_labels(V,gv.front().val);
    else
        return gentypeerr(contextptr);
    if (!is_strictly_positive(gv.back(),contextptr))
        return gentypeerr(contextptr);
    G.make_random(directed,V,gv.back().DOUBLE_val());
    return G.to_gen();
}

bool compute_product_of_graphs(const vecteur &gv,graphe &P,bool cartesian,GIAC_CONTEXT) {
    stack<graphe> Gs;
    for (const_iterateur it=gv.begin();it!=gv.end();++it) {
        graphe G(contextptr);
        if (!G.read_gen(*it))
            return false;
        Gs.push(G);
    }
    P=Gs.top();
    Gs.pop();
    while (!Gs.empty()) {
        graphe G(P);
        if (cartesian)
            Gs.top().cartesian_product(G,P); // compute Cartesian product
        else Gs.top().tensor_product(G,P); // compute tensor product
        Gs.pop();
    }
    return true;
}

void add_prefix_to_vertex_label(gen &label,int prefix, stringstream &ss) {
    ss.str("");
    ss << prefix << ":"
       << (label.type==_STRNG?graphe::genstring2str(label):graphe::gen2str(label));
    label=graphe::str2gen(ss.str(),true);
}

int graphunion(graphe &G,const vecteur &gv,bool disjoint) {
    bool have_properties=false;
    int k=0,i,j;
    graphe::ipairs E;
    stringstream ss;
    gen weight;
    for (const_iterateur it=gv.begin();it!=gv.end();++it) {
        ++k;
        graphe Gk(G.giac_context());
        if (!Gk.read_gen(*it))
            return _GT_ERR_NOT_A_GRAPH;
        if (have_properties) {
            if (G.is_directed()!=Gk.is_directed())
                return G.is_directed()?_GT_ERR_DIRECTED_GRAPH_REQUIRED:_GT_ERR_UNDIRECTED_GRAPH_REQUIRED;
            if (G.is_weighted()!=Gk.is_weighted())
                return G.is_weighted()?_GT_ERR_WEIGHTED_GRAPH_REQUIRED:_GT_ERR_UNWEIGHTED_GRAPH_REQUIRED;
        } else {
            G.set_directed(Gk.is_directed());
            G.set_weighted(Gk.is_weighted());
            have_properties=true;
        }
        vecteur V=Gk.vertices();
        if (disjoint) {
            for (iterateur it=V.begin();it!=V.end();++it) {
                add_prefix_to_vertex_label(*it,k,ss);
            }
        }
        Gk.get_edges_as_pairs(E,false);
        for (graphe::ipairs_iter it=E.begin();it!=E.end();++it) {
            const gen &v=V[it->first],&w=V[it->second];
            weight=Gk.is_weighted()?Gk.weight(it->first,it->second):1;
            if (!disjoint && G.is_weighted() && (i=G.node_index(v))>=0 &&
                    (j=G.node_index(w))>=0 && G.has_edge(i,j))
                G.set_edge_attribute(i,j,_GT_ATTRIB_WEIGHT,G.weight(i,j)+weight);
            G.add_edge(v,w,weight);
        }
    }
    return -1;
}

// +--------------------------------------------------------------------------+
// |                             GIAC COMMANDS                                |
// +--------------------------------------------------------------------------+

/* USAGE:   trail(V)
 *
 * Returns a trail of vertices from sequence V (this is a dummy command, it
 * returns itself).
 */
gen _trail(const gen &g,GIAC_CONTEXT) {
    return symbolic(at_trail,g);
}
static const char _trail_s[]="trail";
static define_unary_function_eval(__trail,&_trail,_trail_s);
define_unary_function_ptr5(at_trail,alias_at_trail,&__trail,0,true)

/* USAGE:   graph(V,[opts])
 *          graph(V,E,[opts])
 *          graph(V,E,A,[opts])
 *          graph(V,A,[opts])
 *          graph(A,[opts])
 *          graph("name")
 *
 * Create an (un)directed (un)weighted graph from list of vertices V, set of edges
 * E, and/or adjacency matrix A containing edge weights. All parameters are
 * optional.
 *
 * 'opts' is a sequence of options containing weighted=true/false,
 * directed=true/false, color=c or coordinates=p. Here c is
 * integer or list of integers (color(s) to be assigned to vertices (in order))
 * and p is list of coordinates to assign to vertices (used for drawing).
 *
 * A special may be created by specifying its name as a string. Supported names
 * are: clebsch - coxeter - desargues - dodecahedron - durer - dyck - grinberg
 * - grotzsch - harries - harries-wong - heawood - herschel - icosahedron -
 * levi - ljubljana - mcgee - mobius-kantor - nauru - octahedron - pappus -
 * petersen - robertson - soccerball - shrikhande - tehtrahedron
*/
gen _graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type==_STRNG) {
        // construct special graph
        string name=graphe::genstring2str(g);
        graphe G(name,contextptr);
        if (G.is_empty())
            return gt_err(_GT_ERR_NAME_NOT_RECOGNIZED,contextptr);
        return G.to_gen();
    }
    graphe G(contextptr);
    if (g.is_integer() && g.val>=0) {
        vecteur V;
        G.make_default_labels(V,g.val);
        G.add_nodes(V);
    } else if (is_squarematrix(g) && g._VECTptr->size()>2) {
        // adjacency matrix is given
        bool size_err;
        if (!parse_matrix(G,*g._VECTptr,false,0,size_err))
            return size_err?gendimerr(contextptr):gentypeerr(contextptr);
    } else if (g.type==_VECT && g.subtype!=_SEQ__VECT) {
        // list of vertices or set of edges is given
        if (g.subtype==_SET__VECT) {
            if (!parse_edges(G,*g._VECTptr,true))
                return gentypeerr(contextptr);
        } else G.add_nodes(*g._VECTptr);
    } else if (g.is_symb_of_sommet(at_trail)) {
        // a trail is given
        parse_trail(G,g);
    } else {
        if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
            return gentypeerr(contextptr);
        vecteur &args=*g._VECTptr;
        int nargs=args.size(),n=nargs-1;
        // parse options first
        bool weighted=false,size_err;
        while(args[n].is_symb_of_sommet(at_equal)) {
            vecteur &sides=*args[n]._SYMBptr->feuille._VECTptr;
            if (!sides.front().is_integer())
                return gentypeerr(contextptr);
            switch(sides.front().val) {
            case _GT_DIRECTED:
                if (!sides.back().is_integer())
                    return gentypeerr(contextptr);
                G.set_directed((bool)sides.back().val);
                break;
            case _GT_WEIGHTED:
                if (!sides.back().is_integer())
                    return gentypeerr(contextptr);
                weighted=(bool)sides.back().val;
                break;
            }
            n--;
        }
        // parse other arguments
        for (int i=0;i<nargs;++i) {
            const gen &arg=args[i];
            if (i<=n && ckmatrix(arg) && arg.subtype!=_SET__VECT) {
                // adjacency or weight matrix
                matrice &m=*arg._VECTptr;
                if (!G.is_directed() && m!=mtran(m))
                    return gt_err(_GT_ERR_MATRIX_NOT_SYMMETRIC,contextptr);
                if (!parse_matrix(G,m,i==2 || weighted,i,size_err))
                    return size_err?gendimerr(contextptr):gentypeerr(contextptr);
            } else if (i==0 && arg.is_integer()) {
                int nv=arg.val;
                if (nv<0)
                    return gensizeerr(contextptr);
                vecteur V;
                G.make_default_labels(V,nv);
                G.add_nodes(V);
            } else if (i<2 && arg.type==_VECT) {
                int permu_size;
                const vecteur &argv=*arg._VECTptr;
                if (arg.subtype==_SET__VECT) {
                    // set of edges
                    if (!parse_edges(G,argv,true))
                        return gentypeerr(contextptr);
                } else if (i==1 && !is_zero(_is_permu(arg,contextptr)) &&
                           (permu_size=argv.size())>0) {
                    if (permu_size!=G.node_count())
                        return gensizeerr(contextptr);
                    // directed cycle
                    G.set_directed(true);
                    int offset=array_start(contextptr);
                    for (const_iterateur it=argv.begin();it!=argv.end()-1;++it) {
                        G.add_edge(it->val-offset,(it+1)->val-offset);
                    }
                } else if (i==0) // list of vertices
                    G.add_nodes(argv);
                else return gentypeerr(contextptr);
            } else if (arg.is_symb_of_sommet(at_trail)) {
                // trail
                parse_trail(G,arg);
            } else if (i>n && arg.is_symb_of_sommet(at_equal)) {
                // option
                gen &lh=arg._SYMBptr->feuille._VECTptr->front();
                gen &rh=arg._SYMBptr->feuille._VECTptr->back();
                if (lh.is_integer()) {
                    switch(lh.val) {
                    case _COLOR:
                        // vertex colors are given
                        if (rh.type==_VECT || int(rh._VECTptr->size())!=G.node_count())
                            return gensizeerr(contextptr);
                        if (!parse_vertex_colors(G,rh))
                            return gentypeerr(contextptr);
                        break;
                    }
                } else if (lh==at_coordonnees) {
                    // vertex coordinates are given
                    if (rh.type!=_VECT)
                        return gentypeerr(contextptr);
                    if (int(rh._VECTptr->size())!=G.node_count())
                        return gensizeerr(contextptr);
                    bool size_error=false;
                    if (!parse_vertex_coordinates(G,*rh._VECTptr,size_error))
                        return size_error?gensizeerr(contextptr):gentypeerr(contextptr);
                }
            } else return gentypeerr(contextptr);
        }
    }
    return G.to_gen();
}
static const char _graph_s[]="graph";
static define_unary_function_eval(__graph,&_graph,_graph_s);
define_unary_function_ptr5(at_graph,alias_at_graph,&__graph,0,true)

/* USAGE:   digraph(V,[opts])
 *          digraph(V,E,[opts])
 *          digraph(A,[opts])
 *          digraph(V,E,A,[opts])
 *
 * Create a directed (un)weighted graph from list of vertices V, set of edges E
 * and/or adjacency matrix A containing edge weights. All parameters are
 * optional.
 *
 * 'opts' may be one of weighted=true/false, vertexcolor=c and
 * vertexpositions=p. Here c is integer or list of integers (color(s) to be
 * assigned to vertices (in order)) and p is list of coordinates to assign to
 * vertices (used for drawing).
 */
gen _digraph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    vecteur args;
    if (g.type==_VECT && g.subtype==_SEQ__VECT)
        args=*g._VECTptr;
    else args.push_back(g);
    args.push_back(symbolic(at_equal,makesequence(_GT_DIRECTED,graphe::VRAI)));
    return _graph(change_subtype(args,_SEQ__VECT),contextptr);
}
static const char _digraph_s[]="digraph";
static define_unary_function_eval(__digraph,&_digraph,_digraph_s);
define_unary_function_ptr5(at_digraph,alias_at_digraph,&__digraph,0,true)

/* USAGE:   export_graph(G,"path/to/graphname")
 *
 * Writes the graph G to the file 'graphname.dot' in directory 'path/to' using
 * dot format. Returns 1 on success and 0 on failure.
 */
gen _export_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())!=2)
        return gentypeerr(contextptr);
    gen &gr=g._VECTptr->front(),&name=g._VECTptr->back();
    graphe G(contextptr);
    if (name.type!=_STRNG)
        return gentypeerr(contextptr);
    if (!G.read_gen(gr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    string filename=graphe::genstring2str(name);
    if (!has_suffix(filename,".dot"))
        filename=filename+".dot";
    if (filename[0]!='/')
        filename=make_absolute_file_path(filename);
    return G.write_dot(filename)?1:0;
}
static const char _export_graph_s[]="export_graph";
static define_unary_function_eval(__export_graph,&_export_graph,_export_graph_s);
define_unary_function_ptr5(at_export_graph,alias_at_export_graph,&__export_graph,0,true)

/* USAGE:   import_graph("path/to/graphname[.dot]")
 *
 * Returns the graph constructed from instructions in the file
 * 'path/to/graphname.dot' (in dot format) or undef on failure.
 */
gen _import_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_STRNG)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    string filename=graphe::genstring2str(g);
    if (filename.empty())
        return undef;
    if (!has_suffix(filename,".dot"))
        filename=filename+".dot";
    if (filename[0]!='/')
        filename=make_absolute_file_path(filename);
    if (!G.read_dot(filename)) {
        gt_err_display(_GT_ERR_READING_FAILED,contextptr);
        return undef;
    }
    return G.to_gen();
}
static const char _import_graph_s[]="import_graph";
static define_unary_function_eval(__import_graph,&_import_graph,_import_graph_s);
define_unary_function_ptr5(at_import_graph,alias_at_import_graph,&__import_graph,0,true)

/* USAGE:   vertices(G)
 *
 * Return list of vertices of graph G.
 */
gen _graph_vertices(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return G.vertices();
}
static const char _graph_vertices_s[]="graph_vertices";
static define_unary_function_eval(__graph_vertices,&_graph_vertices,_graph_vertices_s);
define_unary_function_ptr5(at_graph_vertices,alias_at_graph_vertices,&__graph_vertices,0,true)

/* USAGE:   edges(G,[weights])
 *
 * Return list of edges of graph G. If second argument is the option 'weights',
 * edge weights are also returned.
 */
gen _edges(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    bool include_weights=false;
    graphe G(contextptr);
    if (g.subtype==_SEQ__VECT) {
        if (int(g._VECTptr->size())!=2)
            return gensizeerr(contextptr);
        if (g._VECTptr->front().type!=_VECT)
            return gentypeerr(contextptr);
        if (g._VECTptr->back().is_integer() && g._VECTptr->back().val==_GT_WEIGHTS)
            include_weights=true;
        if (!G.read_gen(g._VECTptr->front()))
            return gentypeerr(contextptr);
    } else if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (include_weights && !G.is_weighted())
        return gt_err(_GT_ERR_WEIGHTED_GRAPH_REQUIRED,contextptr);
    return G.edges(include_weights);
}
static const char _edges_s[]="edges";
static define_unary_function_eval(__edges,&_edges,_edges_s);
define_unary_function_ptr5(at_edges,alias_at_edges,&__edges,0,true)

/* USAGE:   has_edge(G,e)
 *
 * Returns true iff the edge e={i,j} is contained in undirected graph G.
 */
gen _has_edge(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv = *g._VECTptr;
    if (int(gv.size())!=2)
        return gensizeerr(contextptr);
    if (gv.back().type!=_VECT || !is_integer_vecteur(*gv.back()._VECTptr))
        return gentypeerr(contextptr);
    if (int(gv.back()._VECTptr->size())!=2)
        return gensizeerr(contextptr);
    vecteur e(*gv.back()._VECTptr);
    int ofs=array_start(contextptr);
    int i=e.front().val-ofs,j=e.back().val-ofs;
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    return graphe::boole(G.has_edge(i,j));
}
static const char _has_edge_s[]="has_edge";
static define_unary_function_eval(__has_edge,&_has_edge,_has_edge_s);
define_unary_function_ptr5(at_has_edge,alias_at_has_edge,&__has_edge,0,true)

/* USAGE:   has_arc(G,e)
 *
 * Returns true iff the arc e=[i,j] is contained in directed graph G. If
 * e={i,j}, true is returned if directed graph G has both edges [i,j] and
 * [j,i].
 */
gen _has_arc(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv = *g._VECTptr;
    if (int(gv.size())!=2)
        return gensizeerr(contextptr);
    if (gv.back().type!=_VECT || !is_integer_vecteur(*gv.back()._VECTptr))
        return gentypeerr(contextptr);
    if (int(gv.back()._VECTptr->size())!=2)
        return gensizeerr(contextptr);
    vecteur e(*gv.back()._VECTptr);
    bool undirected=gv.back().subtype==_SET__VECT;
    int ofs=array_start(contextptr);
    int i=e.front().val-ofs,j=e.back().val-ofs;
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    return graphe::boole(G.has_edge(i,j) && (!undirected || G.has_edge(j,i)));
}
static const char _has_arc_s[]="has_arc";
static define_unary_function_eval(__has_arc,&_has_arc,_has_arc_s);
define_unary_function_ptr5(at_has_arc,alias_at_has_arc,&__has_arc,0,true)

/* USAGE:   adjacency_matrix(G)
 *
 * Returns the adjacency matrix of a graph G whose rows and columns are indexed
 * by the vertices. The entry [i,j] of this matrix is 1 if there is an edge
 * from vertex i to vertex j and 0 otherwise.
 */
gen _adjacency_matrix(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.node_count()==0)
        return vecteur(0);
    matrice m;
    G.adjacency_matrix(m);
    return m;
}
static const char _adjacency_matrix_s[]="adjacency_matrix";
static define_unary_function_eval(__adjacency_matrix,&_adjacency_matrix,_adjacency_matrix_s);
define_unary_function_ptr5(at_adjacency_matrix,alias_at_adjacency_matrix,&__adjacency_matrix,0,true)

/* USAGE:   incidence_matrix(G)
 *
 * Returns the incidence matrix of a graph G whose rows are indexed by the
 * vertices and columns by the edges (in order defined by the command 'edges').
 * The entry [i,j] of this matrix is 1 if the i-th vertex is incident with the
 * j-th edge (for directed graphs, -1 if vertex is the tail and 1 if it is the
 * head of arc).
 */
gen _incidence_matrix(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.node_count()==0)
        return vecteur(0);
    return G.incidence_matrix();
}
static const char _incidence_matrix_s[]="incidence_matrix";
static define_unary_function_eval(__incidence_matrix,&_incidence_matrix,_incidence_matrix_s);
define_unary_function_ptr5(at_incidence_matrix,alias_at_incidence_matrix,&__incidence_matrix,0,true)

/* USAGE:   weight_matrix(G)
 *
 * Returns the weight matrix of graph G.
 */
gen _weight_matrix(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g) || !G.is_weighted())
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.node_count()==0)
        return vecteur(0);
    return G.weight_matrix();
}
static const char _weight_matrix_s[]="weight_matrix";
static define_unary_function_eval(__weight_matrix,&_weight_matrix,_weight_matrix_s);
define_unary_function_ptr5(at_weight_matrix,alias_at_weight_matrix,&__weight_matrix,0,true)

/* USAGE:   graph_complement(G)
 *
 * Return the complement of graph G, i.e. the graph with the same vertex set
 * as G, but whose edge (arc) set consists of the edges (arcs) not present in
 * G.
 */
gen _graph_complement(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr),C(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    G.complement(C);
    return C.to_gen();
}
static const char _graph_complement_s[]="graph_complement";
static define_unary_function_eval(__graph_complement,&_graph_complement,_graph_complement_s);
define_unary_function_ptr5(at_graph_complement,alias_at_graph_complement,&__graph_complement,0,true)

/* USAGE:   subgraph(G,E)
 *
 * Returns the subgraph of G defined by the edges in list E.
 */
gen _subgraph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())!=2 ||
            g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &E=*g._VECTptr->back()._VECTptr;
    graphe G(contextptr),S(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    graphe::ipairs edges;
    bool notfound=false;
    if (!G.edges2ipairs(E,edges,notfound))
        return notfound?gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr):gensizeerr(contextptr);
    G.subgraph(edges,S);
    return S.to_gen();
}
static const char _subgraph_s[]="subgraph";
static define_unary_function_eval(__subgraph,&_subgraph,_subgraph_s);
define_unary_function_ptr5(at_subgraph,alias_at_subgraph,&__subgraph,0,true)

/* USAGE:   vertex_degree(G,v)
 *
 * Returns the degree of the vertex v in graph G (number of edges incident to v).
 */
gen _vertex_degree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())<2)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int i=G.node_index(g._VECTptr->at(1));
    if (i==-1)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    return G.degree(i);
}
static const char _vertex_degree_s[]="vertex_degree";
static define_unary_function_eval(__vertex_degree,&_vertex_degree,_vertex_degree_s);
define_unary_function_ptr5(at_vertex_degree,alias_at_vertex_degree,&__vertex_degree,0,true)

/* USAGE:   vertex_in_degree(G,v)
 *
 * Returns the number of arcs ending in the vertex v of graph G.
 */
gen _vertex_in_degree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())<2)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    int i=G.node_index(g._VECTptr->at(1));
    if (i==-1)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    return G.in_degree(i);
}
static const char _vertex_in_degree_s[]="vertex_in_degree";
static define_unary_function_eval(__vertex_in_degree,&_vertex_in_degree,_vertex_in_degree_s);
define_unary_function_ptr5(at_vertex_in_degree,alias_at_vertex_in_degree,&__vertex_in_degree,0,true)

/* USAGE:   vertex_out_degree(G,v)
 *
 * Returns the number of arcs starting in the vertex v of graph G.
 */
gen _vertex_out_degree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())<2)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    int i=G.node_index(g._VECTptr->at(1));
    if (i==-1)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    return G.out_degree(i);
}
static const char _vertex_out_degree_s[]="vertex_out_degree";
static define_unary_function_eval(__vertex_out_degree,&_vertex_out_degree,_vertex_out_degree_s);
define_unary_function_ptr5(at_vertex_out_degree,alias_at_vertex_out_degree,&__vertex_out_degree,0,true)

/* USAGE:   induced_subgraph(G,V)
 *
 * Returns the subgraph of G induced by the vertices in list V.
 */
gen _induced_subgraph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())!=2 ||
            g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
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
static const char _induced_subgraph_s[]="induced_subgraph";
static define_unary_function_eval(__induced_subgraph,&_induced_subgraph,_induced_subgraph_s);
define_unary_function_ptr5(at_induced_subgraph,alias_at_induced_subgraph,&__induced_subgraph,0,true)

/* USAGE:   maximal_independent_set(G)
 *
 * Returns a maximal set of mutually independent (non-adjacent) vertices of
 * graph G. Using a method by Y.Hu (based on that of Ruge and Stuben), see
 * http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.38.2239&rep=rep1&type=pdf.
 */
gen _maximal_independent_set(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    graphe::ivector mis;
    G.maximal_independent_set(mis);
    int n=mis.size();
    vecteur V(n);
    for (int i=0;i<n;++i) {
        V[i]=G.node_label(mis[i]);
    }
    return V;
}
static const char _maximal_independent_set_s[]="maximal_independent_set";
static define_unary_function_eval(__maximal_independent_set,&_maximal_independent_set,_maximal_independent_set_s);
define_unary_function_ptr5(at_maximal_independent_set,alias_at_maximal_independent_set,&__maximal_independent_set,0,true)

/* USAGE:   maximum_matching(G)
 *
 * Returns the list of edges representing maximum matching for graph G. Jack
 * Edmonds' blossom algorithm is used.
 */
gen _maximum_matching(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    graphe::ipairs matching;
    G.maximize_matching(matching);
    vecteur res;
    for (graphe::ipairs_iter it=matching.begin();it!=matching.end();++it) {
        res.push_back(makevecteur(G.node_label(it->first),G.node_label(it->second)));
    }
    return res;
}
static const char _maximum_matching_s[]="maximum_matching";
static define_unary_function_eval(__maximum_matching,&_maximum_matching,_maximum_matching_s);
define_unary_function_ptr5(at_maximum_matching,alias_at_maximum_matching,&__maximum_matching,0,true)

/* USAGE:   make_directed(G,[A])
 *
 * Returns the copy of an undirected graph G in which every edge is converted
 * to a pair of arcs [and with weights specified by matrix A].
 */
gen _make_directed(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    bool hasweights=g.subtype==_SEQ__VECT;
    if (hasweights && gv.size()!=2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(hasweights?gv.front():g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    G.make_directed();
    if (hasweights) {
        if (gv.back().type!=_VECT)
            return gentypeerr(contextptr);
        G.make_weighted(*gv.back()._VECTptr);
    }
    return G.to_gen();
}
static const char _make_directed_s[]="make_directed";
static define_unary_function_eval(__make_directed,&_make_directed,_make_directed_s);
define_unary_function_ptr5(at_make_directed,alias_at_make_directed,&__make_directed,0,true)

/* USAGE:   underlying_graph(G)
 *
 * Returns the underlying graph of G, i.e. the graph obtained by stripping
 * directions and weights from arcs (pairs of arcs connecting the same vertices
 * are merged to a single edge).
 */
gen _underlying_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    graphe U;
    G.underlying(U);
    return U.to_gen();
}
static const char _underlying_graph_s[]="underlying_graph";
static define_unary_function_eval(__underlying_graph,&_underlying_graph,_underlying_graph_s);
define_unary_function_ptr5(at_underlying_graph,alias_at_underlying_graph,&__underlying_graph,0,true)

/* USAGE:   cycle_graph(n or V)
 *
 * Returns the cyclic graph with n vertices (or with vertices from list V).
 */
gen _cycle_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!vertices_from_integer_or_vecteur(g,G))
        return gentypeerr(contextptr);
    if (G.node_count()<3)
        return gensizeerr(contextptr);
    G.make_cycle_graph();
    stringstream ss;
    ss << "C" << G.node_count();
    G.set_name(ss.str());
    return G.to_gen();
}
static const char _cycle_graph_s[]="cycle_graph";
static define_unary_function_eval(__cycle_graph,&_cycle_graph,_cycle_graph_s);
define_unary_function_ptr5(at_cycle_graph,alias_at_cycle_graph,&__cycle_graph,0,true)

/* USAGE:   lcf_graph(jumps,[exp])
 *
 * Returns the graph constructed from LCF notation jumps[^exp]. Arguments are
 * list of integers 'jumps' [and a positive integer 'exp', by default 1].
 */
gen _lcf_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
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
    graphe G(contextptr);
    graphe::ivector ijumps;
    vecteur2vector_int(jumps,0,ijumps);
    G.make_lcf_graph(ijumps,e.val);
    return G.to_gen();
}
static const char _lcf_graph_s[]="lcf_graph";
static define_unary_function_eval(__lcf_graph,&_lcf_graph,_lcf_graph_s);
define_unary_function_ptr5(at_lcf_graph,alias_at_lcf_graph,&__lcf_graph,0,true)

/* USAGE:   hypercube_graph(n)
 *
 * Constructs and returns the hypercube graph in dimension n (with 2^n vertices).
 */
gen _hypercube_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
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
static const char _hypercube_graph_s[]="hypercube_graph";
static define_unary_function_eval(__hypercube_graph,&_hypercube_graph,_hypercube_graph_s);
define_unary_function_ptr5(at_hypercube_graph,alias_at_hypercube_graph,&__hypercube_graph,0,true)

/* USAGE:   seidel_switch(G,V)
 *
 * Returns a copy of graph G in which edges between vertices in list V and
 * vertices not in V are inverted, i.e. replaced with a set of edges from V to
 * other vertices which is not present in G.
 */
gen _seidel_switch(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || int(g._VECTptr->size())!=2 ||
            g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr),H(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
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
            if (vb[i]!=vb[j])
                b=!b;
            if (b)
                H.add_edge(i,j);
        }
    }
    return H.to_gen();
}
static const char _seidel_switch_s[]="seidel_switch";
static define_unary_function_eval(__seidel_switch,&_seidel_switch,_seidel_switch_s);
define_unary_function_ptr5(at_seidel_switch,alias_at_seidel_switch,&__seidel_switch,0,true)

/* USAGE:   draw_graph(G,[options])
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
 *    otherwise one must be specified or all vertices are placed on a circle
 *  - plot3d: draw 3D representation of graph G (possible only with the
 *    spring method and with G connected)
 *  - labels=true or false: draw (the default) or suppress node labels and
 *    weights
 *
 * An exception is raised if a method is specified but the corresponding
 * necessary conditions are not met.
 */
gen _draw_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    bool labels=true,has_opts=g.subtype==_SEQ__VECT;
    graphe G_orig(contextptr);
    if (!G_orig.read_gen(has_opts?gv.front():g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    bool isdir=G_orig.is_directed();
    vecteur root_nodes,cycle;
    int method=_GT_STYLE_DEFAULT;
    if (has_opts) {
        // parse options
        int opt_counter=0;
        for (const_iterateur it=gv.begin()+1;it!=gv.end();++it) {
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
    int i,comp_method=method;
    vector<graphe> Cv;
    vector<graphe::layout> layouts;
    graphe::layout main_layout;
    vecteur drawing;
    if (method==_GT_STYLE_3D) {
        if (!G.is_connected())
            return gt_err(_GT_ERR_CONNECTED_GRAPH_REQUIRED,contextptr);
        G.make_spring_layout(main_layout,3);
        Cv.push_back(G);
    } else {
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
                    return gt_err(*it,_GT_ERR_VERTEX_NOT_FOUND,contextptr);
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
        }
        layouts.resize(nc);
        vector<graphe::rectangle> bounding_rects(nc);
        bool check=method!=_GT_STYLE_DEFAULT;
        double sep=1.0;
        // draw the components separately
        for (graphe::ivectors::iterator it=components.begin();it!=components.end();++it) {
            i=it-components.begin();
            sort(it->begin(),it->end());
            Cv.resize(Cv.size()+1);
            graphe &C=Cv.back();
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
                    if (!C.get_leading_cycle(outerface) && !C.find_cycle(outerface)) {
                        outerface.resize(G.node_count());
                        for (int cnt=G.node_count();cnt-->0;) {
                            outerface[cnt]=cnt;
                        }
                    }
                    C.make_circular_layout(x,outerface,false);
                    outerface.clear();
                } else
                    C.make_circular_layout(x,outerface);
                break;
            }
            if (comp_method!=_GT_STYLE_TREE) {
                C.layout_best_rotation(x);
                graphe::scale_layout(x,sep*std::sqrt((double)C.node_count()));
            }
       }
        // combine component layouts
        for (int i=0;i<nc;++i) {
            bounding_rects[i]=graphe::layout_bounding_rect(layouts[i],sep/4.0);
        }
        graphe::rectangle::comparator comp;
        sort(bounding_rects.begin(),bounding_rects.end(),comp);
        graphe::dpairs embedding;
        graphe::pack_rectangles(bounding_rects,embedding);
        graphe::point dx(2);
        for (graphe::dpairs::const_iterator it=embedding.begin();it!=embedding.end();++it) {
            graphe::rectangle &brect=bounding_rects[it-embedding.begin()];
            dx.front()=it->first-brect.x();
            dx.back()=it->second-brect.y();
            graphe::translate_layout(*brect.get_layout(),dx);
        }
        int i,j;
        main_layout.resize(G.node_count());
        for (vector<graphe>::const_iterator it=Cv.begin();it!=Cv.end();++it) {
            graphe::layout &x=layouts[it-Cv.begin()];
            for (graphe::layout_iter jt=x.begin();jt!=x.end();++jt) {
                const graphe::vertex &v=it->node(jt-x.begin());
                i=G_orig.node_index(v.label());
                main_layout[i]=*jt;
                if (isdir) {
                    for (graphe::ivector_iter nt=v.neighbors().begin();nt!=v.neighbors().end();++nt) {
                        j=G_orig.node_index(it->node(*nt).label());
                        const graphe::attrib &attr=v.neighbor_attributes(*nt);
                        if (G_orig.has_edge(i,j))
                            G_orig.set_edge_attribute(i,j,_GT_ATTRIB_POSITION,attr.find(_GT_ATTRIB_POSITION)->second);
                    }
                }
            }
        }
    }
    G_orig.edge_labels_placement(main_layout);
    G_orig.draw_edges(drawing,main_layout);
    G_orig.draw_nodes(drawing,main_layout);
    if (labels)
        G_orig.draw_labels(drawing,main_layout);
    return drawing;
}
static const char _draw_graph_s[]="draw_graph";
static define_unary_function_eval(__draw_graph,&_draw_graph,_draw_graph_s);
define_unary_function_ptr5(at_draw_graph,alias_at_draw_graph,&__draw_graph,0,true)

/* USAGE:   sierpinski_graph(n,k,[triangle])
 *
 * Returns Sierpinski (triangle) graph S(n,k) (resp. ST(n,k)).
 */
gen _sierpinski_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    int n,k=3;
    bool trng=false;
    if (g.is_integer())
        n=g.val;
    else {
        if (g.type!=_VECT || g.subtype!=_SEQ__VECT ||
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
static const char _sierpinski_graph_s[]="sierpinski_graph";
static define_unary_function_eval(__sierpinski_graph,&_sierpinski_graph,_sierpinski_graph_s);
define_unary_function_ptr5(at_sierpinski_graph,alias_at_sierpinski_graph,&__sierpinski_graph,0,true)

/* USAGE:   complete_graph(n or V)
 *          complete_graph(m,n)
 *          complete_graph(n1,n2,...,nk)
 *
 * Create and return a complete graph with vertices from list V or enumerated
 * with first n integers. When two positive integers m and n are given, a
 * complete bipartite graph is returned (or complete k-partite graph if k
 * integers n1,n2,...,nk are given).
 */
gen _complete_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (g.type==_VECT && g.subtype!=_SEQ__VECT) {
        G.add_nodes(*g._VECTptr);
        G.make_complete_graph();
    } else if (g.is_integer() && g.val>0){
        vecteur V;
        G.make_default_labels(V,g.val);
        G.add_nodes(V);
        G.make_complete_graph();
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
static const char _complete_graph_s[]="complete_graph";
static define_unary_function_eval(__complete_graph,&_complete_graph,_complete_graph_s);
define_unary_function_ptr5(at_complete_graph,alias_at_complete_graph,&__complete_graph,0,true)

/* USAGE:   petersen_graph(n,[k])
 *
 * Returns the generalized Petersen graph G(n,k), where n and k are positive
 * integers. Parameter k defaults to 2.
 */
gen _petersen_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    int n=5,k=2;
    if (g.is_integer()) {
        n=g.val;
        if (n<=0)
            return gentypeerr(contextptr);
    } else if (g.type==_VECT && g.subtype==_SEQ__VECT) {
        if ((g._VECTptr->size())!=2)
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
static const char _petersen_graph_s[]="petersen_graph";
static define_unary_function_eval(__petersen_graph,&_petersen_graph,_petersen_graph_s);
define_unary_function_ptr5(at_petersen_graph,alias_at_petersen_graph,&__petersen_graph,0,true)

/* USAGE:   random_graph(n or V,p)
 *          random_graph(n or V,m)
 *
 * Returns a random undirected unweighted graph with n vertices where two
 * vertices are connected with probability p. Alternatively, m edges are
 * created at random. Instead of number n of vertices, a list V of vertex
 * labels may be specified.
 */
gen _random_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    return randomgraph(*g._VECTptr,false,contextptr);
}
static const char _random_graph_s[]="random_graph";
static define_unary_function_eval(__random_graph,&_random_graph,_random_graph_s);
define_unary_function_ptr5(at_random_graph,alias_at_random_graph,&__random_graph,0,true)

/* USAGE:   random_digraph(n or V,p)
 *          random_digraph(n or V,m)
 *
 * Returns a random directed unweighted graph with n vertices where two
 * vertices are connected with probability p. Alternatively, m edges are
 * created at random. Instead of number n of vertices, a list V of vertex
 * labels may be specified.
 */
gen _random_digraph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    return randomgraph(*g._VECTptr,true,contextptr);
}
static const char _random_digraph_s[]="random_digraph";
static define_unary_function_eval(__random_digraph,&_random_digraph,_random_digraph_s);
define_unary_function_ptr5(at_random_digraph,alias_at_random_digraph,&__random_digraph,0,true)

/* USAGE:   random_bipartite_graph(n or [a,b],p)
 *          random_bipartite_graph(n or [a,b],m)
 *
 * Returns a random undirected unweighted bipartite graph with n vertices where
 * each possible edge is present with probability p. Alternatively, m edges are
 * created at random. Also, when first argument is list [a,b] of integers, two
 * groups of vertices with sizes a and b are created.
 */
gen _random_bipartite_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
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
    } else return gentypeerr(contextptr);
    G.make_default_labels(V,a,0);
    G.make_default_labels(W,b,a);
    G.make_random_bipartite(V,W,p);
    return G.to_gen();
}
static const char _random_bipartite_graph_s[]="random_bipartite_graph";
static define_unary_function_eval(__random_bipartite_graph,&_random_bipartite_graph,_random_bipartite_graph_s);
define_unary_function_ptr5(at_random_bipartite_graph,alias_at_random_bipartite_graph,&__random_bipartite_graph,0,true)

/* USAGE:   random_tournament(n or V)
 *
 * Returns a random tournament graph with n vertices, which may be specified as
 * list V of their labels.
 */
gen _random_tournament(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    G.set_directed(true);
    if (!vertices_from_integer_or_vecteur(g,G))
        return gentypeerr(contextptr);
    int n=G.node_count();
    if (n<2)
        return gensizeerr(contextptr);
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
static const char _random_tournament_s[]="random_tournament";
static define_unary_function_eval(__random_tournament,&_random_tournament,_random_tournament_s);
define_unary_function_ptr5(at_random_tournament,alias_at_random_tournament,&__random_tournament,0,true)

/* USAGE:   random_regular_graph(n or V,d,[connected])
 *
 * Returns a random d-regular graph with n vertices, which may be specified as
 * list V of their labels.
 */
gen _random_regular_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    vecteur V;
    vecteur &gv=*g._VECTptr;
    if (gv.front().is_integer())
        G.make_default_labels(V,gv.front().val);
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
static const char _random_regular_graph_s[]="random_regular_graph";
static define_unary_function_eval(__random_regular_graph,&_random_regular_graph,_random_regular_graph_s);
define_unary_function_ptr5(at_random_regular_graph,alias_at_random_regular_graph,&__random_regular_graph,0,true)

/* USAGE:   random_tree(n or V,[d])
 *
 * Returns a random tree graph with n vertices, which may be specified as list
 * V of their labels. Optional parameter d is a positive integer which
 * represents the upper bound for degree of graph.
 */
gen _random_tree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
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
    } else return gentypeerr(contextptr);
    if (n<1)
        return gensizeerr(contextptr);
    if (V.empty())
        G.make_default_labels(V,n);
    G.make_random_tree(V,maxd);
    return G.to_gen();
}
static const char _random_tree_s[]="random_tree";
static define_unary_function_eval(__random_tree,&_random_tree,_random_tree_s);
define_unary_function_ptr5(at_random_tree,alias_at_random_tree,&__random_tree,0,true)

/* USAGE:   random_planar_graph(n or V)
 *
 * Return random biconnected planar graph with n vertices, which can also be
 * specified as a list V of their labels.
 */
gen _random_planar_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    double p=0.5;
    int connectivity=1;
    gen spec;
    if (g.is_integer() || (g.type==_VECT && g.subtype!=_SEQ__VECT))
        spec=g;
    else if (g.type==_VECT && g.subtype==_SEQ__VECT) {
        vecteur &gv=*g._VECTptr;
        if (gv.size()>1 && gv.size()<=3) {
            spec=gv.front();
            if (_evalf(gv[1],contextptr).type!=_DOUBLE_)
                return gentypeerr(contextptr);
            p=gv[1].DOUBLE_val();
            if (p<0 || p>=1)
                return gentypeerr(contextptr);
            if (gv.size()==3) {
                if (!gv.back().is_integer() || gv.back().val<0 || gv.back().val>3)
                    return gentypeerr(contextptr);
                connectivity=gv.back().val;
            }
        } else return gensizeerr(contextptr);
    } else return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!vertices_from_integer_or_vecteur(spec,G))
        return gentypeerr(contextptr);
    G.make_random_planar(p,connectivity);
    return G.to_gen();
}
static const char _random_planar_graph_s[]="random_planar_graph";
static define_unary_function_eval(__random_planar_graph,&_random_planar_graph,_random_planar_graph_s);
define_unary_function_ptr5(at_random_planar_graph,alias_at_random_planar_graph,&__random_planar_graph,0,true)

/* USAGE:   assign_edge_weights(G,m,n)
 *          assign_edge_weights(G,a..b)
 *
 * Assigns random edge weights to the edges of graph G and returns a modified
 * copy of G. If integers n and m such that n>=m are specified, weights are
 * integers randomly chosen in [m,n]. If an interval a..b is specified, weights
 * are uniformly distributed in the interval [a,b).
 */
gen _assign_edge_weights(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    G.set_weighted(true);
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
        if (!graphe::is_real_number(a) || !graphe::is_real_number(b))
            return gentypeerr(contextptr);
        G.randomize_edge_weights(a.DOUBLE_val(),b.DOUBLE_val(),false);
    }
    return G.to_gen();
}
static const char _assign_edge_weights_s[]="assign_edge_weights";
static define_unary_function_eval(__assign_edge_weights,&_assign_edge_weights,_assign_edge_weights_s);
define_unary_function_ptr5(at_assign_edge_weights,alias_at_assign_edge_weights,&__assign_edge_weights,0,true)

/* USAGE:   articulation_points(G)
 *
 * Returns the list of articulation points (i.e. cut vertices) of graph G.
 */
gen _articulation_points(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    graphe::ivector V;
    G.find_cut_vertices(V);
    vecteur res;
    for (graphe::ivector::const_iterator it=V.begin();it!=V.end();++it) {
        res.push_back(G.node_label(*it));
    }
    return res;
}
static const char _articulation_points_s[]="articulation_points";
static define_unary_function_eval(__articulation_points,&_articulation_points,_articulation_points_s);
define_unary_function_ptr5(at_articulation_points,alias_at_articulation_points,&__articulation_points,0,true)

/* USAGE:   biconnected_components(G)
 *
 * Returns the list of biconnected components of graph G. Every component is
 * given as a list of vertices belonging to that component.
 */
gen _biconnected_components(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr),H(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vector<vector<graphe::ipair> > blocks;
    G.find_blocks(blocks);
    vecteur res;
    for (vector<vector<graphe::ipair> >::const_iterator it=blocks.begin();it!=blocks.end();++it) {
        G.subgraph(*it,H,false);
        res.push_back(H.vertices());
    }
    return res;
}
static const char _biconnected_components_s[]="biconnected_components";
static define_unary_function_eval(__biconnected_components,&_biconnected_components,_biconnected_components_s);
define_unary_function_ptr5(at_biconnected_components,alias_at_biconnected_components,&__biconnected_components,0,true)

/* USAGE:   add_arc(G,e)
 *
 * Returns graph G (which must be directed) with added arc e (or trail or list
 * of arcs).
 */
gen _add_arc(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &E=*g._VECTptr->back()._VECTptr;
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    if (!parse_edges(G,E,ckmatrix(g._VECTptr->back())))
        return gendimerr(contextptr);
    return G.to_gen();
}
static const char _add_arc_s[]="add_arc";
static define_unary_function_eval(__add_arc,&_add_arc,_add_arc_s);
define_unary_function_ptr5(at_add_arc,alias_at_add_arc,&__add_arc,0,true)

/* USAGE:   delete_arc(G,e)
 *
 * Returns graph G (which must be directed) with arc e (or trail or list of
 * arcs) removed.
 */
gen _delete_arc(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &E=*g._VECTptr->back()._VECTptr;
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    if (!delete_edges(G,E))
        return gendimerr(contextptr);
    return G.to_gen();
}
static const char _delete_arc_s[]="delete_arc";
static define_unary_function_eval(__delete_arc,&_delete_arc,_delete_arc_s);
define_unary_function_ptr5(at_delete_arc,alias_at_delete_arc,&__delete_arc,0,true)

/* USAGE:   add_edge(G,e)
 *
 * Returns graph G (which must be undirected) with added edge e (or trail or
 * list of edges).
 */
gen _add_edge(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &E=*g._VECTptr->back()._VECTptr;
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    if (!parse_edges(G,E,ckmatrix(g._VECTptr->back())))
        return gendimerr(contextptr);
    return G.to_gen();
}
static const char _add_edge_s[]="add_edge";
static define_unary_function_eval(__add_edge,&_add_edge,_add_edge_s);
define_unary_function_ptr5(at_add_edge,alias_at_add_edge,&__add_edge,0,true)

/* USAGE:   delete_edge(G,e)
 *
 * Returns graph G (which must be undirected) with edge e (or trail or list of
 * edges) removed.
 */
gen _delete_edge(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &E=*g._VECTptr->back()._VECTptr;
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    if (!delete_edges(G,E))
        return gendimerr(contextptr);
    return G.to_gen();
}
static const char _delete_edge_s[]="delete_edge";
static define_unary_function_eval(__delete_edge,&_delete_edge,_delete_edge_s);
define_unary_function_ptr5(at_delete_edge,alias_at_delete_edge,&__delete_edge,0,true)

/* USAGE:   add_vertex(G,v)
 *
 * Returns graph G with added vertex v (or vertices from v if v is a list).
 */
gen _add_vertex(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    gen &V=g._VECTptr->back();
    if (V.type==_VECT)
        G.add_nodes(*V._VECTptr);
    else
        G.add_node(V);
    return G.to_gen();
}
static const char _add_vertex_s[]="add_vertex";
static define_unary_function_eval(__add_vertex,&_add_vertex,_add_vertex_s);
define_unary_function_ptr5(at_add_vertex,alias_at_add_vertex,&__add_vertex,0,true)

/* USAGE:   delete_vertex(G,v)
 *
 * Returns graph G with vertex v (or vertices from v if v is a list) removed.
 */
gen _delete_vertex(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
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
static const char _delete_vertex_s[]="delete_vertex";
static define_unary_function_eval(__delete_vertex,&_delete_vertex,_delete_vertex_s);
define_unary_function_ptr5(at_delete_vertex,alias_at_delete_vertex,&__delete_vertex,0,true)

/* USAGE:   contract_edge(G,e)
 *
 * Returns graph G with edge e contracted (collapsed).
 */
gen _contract_edge(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if (g._VECTptr->back().type!=_VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
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
static const char _contract_edge_s[]="contract_edge";
static define_unary_function_eval(__contract_edge,&_contract_edge,_contract_edge_s);
define_unary_function_ptr5(at_contract_edge,alias_at_contract_edge,&__contract_edge,0,true)

/* USAGE:   connected_components(G)
 *
 * Returns list of lists of vertices, each sublist representing a connected
 * component of graph G. Individual components can be made available as
 * subgraphs of G by applying the induced_subgraph command.
 */
gen _connected_components(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    graphe::ivectors components;
    G.connected_components(components);
    vecteur res;
    for (graphe::ivectors_iter it=components.begin();it!=components.end();++it) {
        res.push_back(G.get_node_labels(*it));
    }
    return res;
}
static const char _connected_components_s[]="connected_components";
static define_unary_function_eval(__connected_components,&_connected_components,_connected_components_s);
define_unary_function_ptr5(at_connected_components,alias_at_connected_components,&__connected_components,0,true)

/* USAGE:   departures(G,[v])
 *
 * Returns the list of vertices of directed graph G which are connected by v
 * with arcs such that tails are in v. If v is omitted, list of departures is
 * computed for every vertex and a list of lists is returned.
 */
gen _departures(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    return flights(g,false,g.subtype!=_SEQ__VECT,contextptr);
}
static const char _departures_s[]="departures";
static define_unary_function_eval(__departures,&_departures,_departures_s);
define_unary_function_ptr5(at_departures,alias_at_departures,&__departures,0,true)

/* USAGE:   arrivals(G,[v])
 *
 * Returns the list of vertices of directed graph G which are connected by v
 * with arcs such that heads are in v. If v is omitted, list of arrivals is
 * computed for every vertex and a list of lists is returned.
 */
gen _arrivals(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    return flights(g,true,g.subtype!=_SEQ__VECT,contextptr);
}
static const char _arrivals_s[]="arrivals";
static define_unary_function_eval(__arrivals,&_arrivals,_arrivals_s);
define_unary_function_ptr5(at_arrivals,alias_at_arrivals,&__arrivals,0,true)

/* USAGE:   incident_edges(G,v)
 *
 * Returns the list of all edges incident to the vertex v (or to the vertices in
 * the list v).
 */
gen _incident_edges(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    if (g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
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
    graphe::edgeset E;
    G.incident_edges(indices,E);
    vecteur res;
    for (graphe::edgeset_iter it=E.begin();it!=E.end();++it) {
        res.push_back(makevecteur(G.node_label(it->first),G.node_label(it->second)));
    }
    return res;
}
static const char _incident_edges_s[]="incident_edges";
static define_unary_function_eval(__incident_edges,&_incident_edges,_incident_edges_s);
define_unary_function_ptr5(at_incident_edges,alias_at_incident_edges,&__incident_edges,0,true)

/* USAGE:   make_weighted(G,[M])
 *
 * Returns the copy of graph G with edge/arc weights set as specified by matrix
 * M. If M is omitted, a suqare matrix of ones is used. If G is undirected, M
 * is assumed to be symmetric.
 */
gen _make_weighted(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    bool has_matrix=g.subtype==_SEQ__VECT;
    if (has_matrix && g._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(has_matrix?g._VECTptr->front():g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_weighted())
        return gt_err(_GT_ERR_UNWEIGHTED_GRAPH_REQUIRED,contextptr);
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
static const char _make_weighted_s[]="make_weighted";
static define_unary_function_eval(__make_weighted,&_make_weighted,_make_weighted_s);
define_unary_function_ptr5(at_make_weighted,alias_at_make_weighted,&__make_weighted,0,true)

/* USAGE:   set_graph_attribute(G,attr1,attr2,...)
 *
 * Stores the attributes attr1, attr2, ..., each in form tag=value, where tag
 * is string, and returns the modified copy of G. Attributes may also be
 * specified in a list or as two lists [tag1,tag2,...] and [value1,value2,...].
 */
gen _set_graph_attribute(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr,attr;
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (gv.size()==2 && gv.back().type==_VECT)
        attr=*gv.back()._VECTptr;
    else if (gv.size()==3 && gv[1].type==_VECT && gv[2].type==_VECT)
        attr=*_zip(makesequence(at_equal,gv[1],gv[2]),contextptr)._VECTptr;
    else attr=vecteur(gv.begin()+1,gv.end());
    int key;
    for (const_iterateur it=attr.begin();it!=attr.end();++it) {
        if (!it->is_symb_of_sommet(at_equal) || it->_SYMBptr->feuille._VECTptr->front().type!=_STRNG)
            return gt_err(_GT_ERR_TAGVALUE_PAIR_EXPECTED,contextptr);
        key=G.tag2index(graphe::genstring2str(it->_SYMBptr->feuille._VECTptr->front()));
        G.set_graph_attribute(key,it->_SYMBptr->feuille._VECTptr->back());
    }
    return G.to_gen();
}
static const char _set_graph_attribute_s[]="set_graph_attribute";
static define_unary_function_eval(__set_graph_attribute,&_set_graph_attribute,_set_graph_attribute_s);
define_unary_function_ptr5(at_set_graph_attribute,alias_at_set_graph_attribute,&__set_graph_attribute,0,true)

/* USAGE:   set_vertex_attribute(G,v,attr1,attr2,...)
 *
 * Stores the attributes attr1, attr2, ..., each in form tag=value, where tag
 * is string, to vertex v and returns the modified copy of G. Attributes may
 * also be specified in a list or as two lists [tag1,tag2,...] and
 * [value1,value2,...].
 */
gen _set_vertex_attribute(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr,attr;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int v,key;
    if ((v=G.node_index(gv[1]))<0)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    if (gv.size()==3 && gv.back().type==_VECT)
        attr=*gv.back()._VECTptr;
    else if (gv.size()==4 && gv[2].type==_VECT && gv[3].type==_VECT)
        attr=*_zip(makesequence(at_equal,gv[2],gv[3]),contextptr)._VECTptr;
    else attr=vecteur(gv.begin()+2,gv.end());
    for (const_iterateur it=attr.begin();it!=attr.end();++it) {
        if (!it->is_symb_of_sommet(at_equal) || it->_SYMBptr->feuille._VECTptr->front().type!=_STRNG)
            return gt_err(_GT_ERR_TAGVALUE_PAIR_EXPECTED,contextptr);
        key=G.tag2index(graphe::genstring2str(it->_SYMBptr->feuille._VECTptr->front()));
        G.set_node_attribute(v,key,it->_SYMBptr->feuille._VECTptr->back());
    }
    return G.to_gen();
}
static const char _set_vertex_attribute_s[]="set_vertex_attribute";
static define_unary_function_eval(__set_vertex_attribute,&_set_vertex_attribute,_set_vertex_attribute_s);
define_unary_function_ptr5(at_set_vertex_attribute,alias_at_set_vertex_attribute,&__set_vertex_attribute,0,true)

/* USAGE:   set_edge_attribute(G,e,attr1,attr2,...)
 *
 * Stores the attributes attr1, attr2, ..., each in form tag=value, where tag
 * is string, to edge e and returns the modified copy of G. Attributes may also
 * be specified in a list or as two lists [tag1,tag2,...] and
 * [value1,value2,...].
 */
gen _set_edge_attribute(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr,attr;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int i,j,key;
    if (gv[1].type!=_VECT)
        return gentypeerr(contextptr);
    if (gv[1]._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    if ((i=G.node_index(gv[1]._VECTptr->front()))<0 ||
            (j=G.node_index(gv[1]._VECTptr->back()))<0)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    if (!G.has_edge(i,j))
        return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
    if (gv.size()==3 && gv.back().type==_VECT)
        attr=*gv.back()._VECTptr;
    else if (gv.size()==4 && gv[2].type==_VECT && gv[3].type==_VECT)
        attr=*_zip(makesequence(at_equal,gv[2],gv[3]),contextptr)._VECTptr;
    else attr=vecteur(gv.begin()+2,gv.end());
    for (const_iterateur it=attr.begin();it!=attr.end();++it) {
        if (!it->is_symb_of_sommet(at_equal) || it->_SYMBptr->feuille._VECTptr->front().type!=_STRNG)
            return gt_err(_GT_ERR_TAGVALUE_PAIR_EXPECTED,contextptr);
        key=G.tag2index(graphe::genstring2str(it->_SYMBptr->feuille._VECTptr->front()));
        G.set_edge_attribute(i,j,key,it->_SYMBptr->feuille._VECTptr->back());
    }
    return G.to_gen();
}
static const char _set_edge_attribute_s[]="set_edge_attribute";
static define_unary_function_eval(__set_edge_attribute,&_set_edge_attribute,_set_edge_attribute_s);
define_unary_function_ptr5(at_set_edge_attribute,alias_at_set_edge_attribute,&__set_edge_attribute,0,true)

/* USAGE:   get_graph_attribute(G,tag1,tag2,...)
 *
 * Get the graph attributes tag1, tag2, ..., i.e. return the sequence of values
 * corresponding to the given tags, which may also be specified in a list.
 */
gen _get_graph_attribute(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr,tags,values;
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    bool istagvec=gv.size()==2 && gv.back().type==_VECT;
    if (istagvec)
        tags=*gv.back()._VECTptr;
    else tags=vecteur(gv.begin()+1,gv.end());
    int key;
    gen value;
    for (const_iterateur it=tags.begin();it!=tags.end();++it) {
        if (it->type!=_STRNG)
            return gentypeerr(contextptr);
        key=G.tag2index(graphe::genstring2str(*it));
        G.get_graph_attribute(key,value);
        values.push_back(value);
    }
    return istagvec?values:change_subtype(values,_SEQ__VECT);
}
static const char _get_graph_attribute_s[]="get_graph_attribute";
static define_unary_function_eval(__get_graph_attribute,&_get_graph_attribute,_get_graph_attribute_s);
define_unary_function_ptr5(at_get_graph_attribute,alias_at_get_graph_attribute,&__get_graph_attribute,0,true)

/* USAGE:   get_vertex_attribute(G,v,tag1,tag2,...)
 *
 * Get the attributes tag1, tag2, ... assigned to vertex v in graph G, i.e.
 * return the sequence of values corresponding to the given tags, which may
 * also be specified in a list.
 */
gen _get_vertex_attribute(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr,tags,values;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int v,key;
    if ((v=G.node_index(gv[1]))<0)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    bool istagvec=gv.size()==3 && gv.back().type==_VECT;
    if (istagvec)
        tags=*gv.back()._VECTptr;
    else tags=vecteur(gv.begin()+2,gv.end());
    gen value;
    for (const_iterateur it=tags.begin();it!=tags.end();++it) {
        if (it->type!=_STRNG)
            return gentypeerr(contextptr);
        key=G.tag2index(graphe::genstring2str(*it));
        G.get_node_attribute(v,key,value);
        values.push_back(value);
    }
    return istagvec?values:change_subtype(values,_SEQ__VECT);
}
static const char _get_vertex_attribute_s[]="get_vertex_attribute";
static define_unary_function_eval(__get_vertex_attribute,&_get_vertex_attribute,_get_vertex_attribute_s);
define_unary_function_ptr5(at_get_vertex_attribute,alias_at_get_vertex_attribute,&__get_vertex_attribute,0,true)

/* USAGE:   get_edge_attribute(G,e,tag1,tag2,...)
 *
 * Get the attributes tag1, tag2, ... assigned to edge e in graph G, i.e.
 * return the sequence of values corresponding to the given tags, which may
 * also be specified in a list.
 */
gen _get_edge_attribute(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr,tags,values;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (gv[1].type!=_VECT)
        return gentypeerr(contextptr);
    if (gv[1]._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    int i,j,key;
    if ((i=G.node_index(gv[1]._VECTptr->front()))<0 ||
            (j=G.node_index(gv[1]._VECTptr->back()))<0)
        return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
    if (!G.has_edge(i,j))
        return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
    bool istagvec=gv.size()==3 && gv.back().type==_VECT;
    if (istagvec)
        tags=*gv.back()._VECTptr;
    else tags=vecteur(gv.begin()+2,gv.end());
    if (tags.empty()) {
        const graphe::attrib &attr=G.edge_attributes(i,j);
        for (graphe::attrib_iter it=attr.begin();it!=attr.end();++it) {
            values.push_back(symbolic(at_equal,makesequence(graphe::str2gen(G.index2tag(it->first),true),it->second)));
        }
    } else {
        gen value;
        for (const_iterateur it=tags.begin();it!=tags.end();++it) {
            if (it->type!=_STRNG)
                return gentypeerr(contextptr);
            key=G.tag2index(graphe::genstring2str(*it));
            G.get_edge_attribute(i,j,key,value);
            values.push_back(value);
        }
    }
    return istagvec?values:change_subtype(values,_SEQ__VECT);
}
static const char _get_edge_attribute_s[]="get_edge_attribute";
static define_unary_function_eval(__get_edge_attribute,&_get_edge_attribute,_get_edge_attribute_s);
define_unary_function_ptr5(at_get_edge_attribute,alias_at_get_edge_attribute,&__get_edge_attribute,0,true)

/* USAGE:   discard_graph_attribute(G,tag1,tag2,...)
 *
 * Discards the graph attributes with tags tag1, tag2, ..., which may also be
 * specified in a list, and returns the modified copy of G.
 */
gen _discard_graph_attribute(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr,tags;
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (gv.size()==2 && gv.back().type==_VECT)
        tags=*gv.back()._VECTptr;
    else tags=vecteur(gv.begin()+1,gv.end());
    int key;
    for (const_iterateur it=tags.begin();it!=tags.end();++it) {
        if (it->type!=_STRNG)
            return gentypeerr(contextptr);
        key=G.tag2index(graphe::genstring2str(*it));
        G.discard_graph_attribute(key);
    }
    return G.to_gen();
}
static const char _discard_graph_attribute_s[]="discard_graph_attribute";
static define_unary_function_eval(__discard_graph_attribute,&_discard_graph_attribute,_discard_graph_attribute_s);
define_unary_function_ptr5(at_discard_graph_attribute,alias_at_discard_graph_attribute,&__discard_graph_attribute,0,true)

/* USAGE:   discard_vertex_attribute(G,v,tag1,tag2,...)
 *
 * Discards the attributes with tags tag1, tag2, ... assigned to vertex v in
 * graph G, which may also be specified in a list, and returns the modified
 * copy of G.
 */
gen _discard_vertex_attribute(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr,tags;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int v,key;
    if ((v=G.node_index(gv[1]))<0)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    if (gv.size()==3 && gv.back().type==_VECT)
        tags=*gv.back()._VECTptr;
    else tags=vecteur(gv.begin()+2,gv.end());
    for (const_iterateur it=tags.begin();it!=tags.end();++it) {
        if (it->type!=_STRNG)
            return gentypeerr(contextptr);
        key=G.tag2index(graphe::genstring2str(*it));
        G.discard_node_attribute(v,key);
    }
    return G.to_gen();
}
static const char _discard_vertex_attribute_s[]="discard_vertex_attribute";
static define_unary_function_eval(__discard_vertex_attribute,&_discard_vertex_attribute,_discard_vertex_attribute_s);
define_unary_function_ptr5(at_discard_vertex_attribute,alias_at_discard_vertex_attribute,&__discard_vertex_attribute,0,true)

/* USAGE:   discard_edge_attribute(G,e,tag1,tag2,...)
 *
 * Discards the attributes with tags tag1, tag2, ... assigned to edge e in
 * graph G, which may also be specified in a list, and returns the modified
 * copy of G.
 */
gen _discard_edge_attribute(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr,tags;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (gv[1].type!=_VECT)
        return gentypeerr(contextptr);
    if (gv[1]._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    int i,j,key;
    if ((i=G.node_index(gv[1]._VECTptr->front()))<0 ||
            (j=G.node_index(gv[1]._VECTptr->back()))<0)
        return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
    if (!G.has_edge(i,j))
        return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
    if (gv.size()==3 && gv.back().type==_VECT)
        tags=*gv.back()._VECTptr;
    else tags=vecteur(gv.begin()+2,gv.end());
    for (const_iterateur it=tags.begin();it!=tags.end();++it) {
        if (it->type!=_STRNG)
            return gentypeerr(contextptr);
        key=G.tag2index(graphe::genstring2str(*it));
        G.discard_edge_attribute(i,j,key);
    }
    return G.to_gen();
}
static const char _discard_edge_attribute_s[]="discard_edge_attribute";
static define_unary_function_eval(__discard_edge_attribute,&_discard_edge_attribute,_discard_edge_attribute_s);
define_unary_function_ptr5(at_discard_edge_attribute,alias_at_discard_edge_attribute,&__discard_edge_attribute,0,true)

/* USAGE:   list_graph_attributes(G)
 *
 * Returns the list of graph attributes in form tag=value.
 */
gen _list_graph_attributes(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur tags,values;
    G.attrib2vecteurs(G.graph_attributes(),tags,values);
    return _zip(makesequence(at_equal,tags,values),contextptr);
}
static const char _list_graph_attributes_s[]="list_graph_attributes";
static define_unary_function_eval(__list_graph_attributes,&_list_graph_attributes,_list_graph_attributes_s);
define_unary_function_ptr5(at_list_graph_attributes,alias_at_list_graph_attributes,&__list_graph_attributes,0,true)

/* USAGE:   list_vertex_attributes(G,v)
 *
 * Returns the list of attributes assigned to vertex v in form tag=value.
 */
gen _list_vertex_attributes(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int i;
    if ((i=G.node_index(gv.back()))<0)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    vecteur tags,values;
    G.attrib2vecteurs(G.node_attributes(i),tags,values);
    return _zip(makesequence(at_equal,tags,values),contextptr);
}
static const char _list_vertex_attributes_s[]="list_vertex_attributes";
static define_unary_function_eval(__list_vertex_attributes,&_list_vertex_attributes,_list_vertex_attributes_s);
define_unary_function_ptr5(at_list_vertex_attributes,alias_at_list_vertex_attributes,&__list_vertex_attributes,0,true)

/* USAGE:   list_edge_attributes(G,e)
 *
 * Returns the list of attributes assigned to edge e in form tag=value.
 */
gen _list_edge_attributes(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (gv.back().type!=_VECT)
        return gentypeerr(contextptr);
    if (gv.back()._VECTptr->size()!=2)
        return gensizeerr(contextptr);
    int i,j;
    if ((i=G.node_index(gv.back()._VECTptr->front()))<0 ||
            (j=G.node_index(gv.back()._VECTptr->back()))<0)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    if (!G.has_edge(i,j))
        return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
    vecteur tags,values;
    G.attrib2vecteurs(G.edge_attributes(i,j),tags,values);
    return _zip(makesequence(at_equal,tags,values),contextptr);
}
static const char _list_edge_attributes_s[]="list_edge_attributes";
static define_unary_function_eval(__list_edge_attributes,&_list_edge_attributes,_list_edge_attributes_s);
define_unary_function_ptr5(at_list_edge_attributes,alias_at_list_edge_attributes,&__list_edge_attributes,0,true)

/* USAGE:   number_of_edges(G)
 *
 * Returns the number of edges/arcs of graph G.
 */
gen _number_of_edges(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return G.edge_count();
}
static const char _number_of_edges_s[]="number_of_edges";
static define_unary_function_eval(__number_of_edges,&_number_of_edges,_number_of_edges_s);
define_unary_function_ptr5(at_number_of_edges,alias_at_number_of_edges,&__number_of_edges,0,true)

/* USAGE:   number_of_vertices(G)
 *
 * Returns the number of vertices of graph G.
 */
gen _number_of_vertices(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return G.node_count();
}
static const char _number_of_vertices_s[]="number_of_vertices";
static define_unary_function_eval(__number_of_vertices,&_number_of_vertices,_number_of_vertices_s);
define_unary_function_ptr5(at_number_of_vertices,alias_at_number_of_vertices,&__number_of_vertices,0,true)

/* USAGE:   get_edge_weight(G,e)
 *
 * Returns the weight of the edge e in graph G (which must be weighted).
 */
gen _get_edge_weight(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()!=2)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
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
static const char _get_edge_weight_s[]="get_edge_weight";
static define_unary_function_eval(__get_edge_weight,&_get_edge_weight,_get_edge_weight_s);
define_unary_function_ptr5(at_get_edge_weight,alias_at_get_edge_weight,&__get_edge_weight,0,true)

/* USAGE:   set_edge_weight(G,e,w)
 *
 * Sets weight of the edge e in graph G (which must be weighted) to w and
 * returns the modified copy of G.
 */
gen _set_edge_weight(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT || g._VECTptr->size()!=3)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(g._VECTptr->front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_weighted())
        return gt_err(_GT_ERR_WEIGHTED_GRAPH_REQUIRED,contextptr);
    gen &E=g._VECTptr->at(1);
    if (E.type!=_VECT || E._VECTptr->size()!=2)
        return gentypeerr(contextptr);
    int i=G.node_index(E._VECTptr->front()),j=G.node_index(E._VECTptr->back());
    if (i==-1 || j==-1)
        return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
    G.set_edge_attribute(i,j,_GT_ATTRIB_WEIGHT,g._VECTptr->back());
    return G.to_gen();
}
static const char _set_edge_weight_s[]="set_edge_weight";
static define_unary_function_eval(__set_edge_weight,&_set_edge_weight,_set_edge_weight_s);
define_unary_function_ptr5(at_set_edge_weight,alias_at_set_edge_weight,&__set_edge_weight,0,true)

/* USAGE:   is_directed(G)
 *
 * Returns true iff G is a directed graph.
 */
gen _is_directed(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_directed());
}
static const char _is_directed_s[]="is_directed";
static define_unary_function_eval(__is_directed,&_is_directed,_is_directed_s);
define_unary_function_ptr5(at_is_directed,alias_at_is_directed,&__is_directed,0,true)

/* USAGE:   neighbors(G,[v])
 *
 * Returns the list of vertices adjacent to v. If v is omitted, a list of
 * adjacency lists of all vertices in G is returned.
 */
gen _neighbors(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    if (g.subtype==_SEQ__VECT && g._VECTptr->size()!=2) {
        return gensizeerr(contextptr);
    }
    graphe G(contextptr);
    if (!G.read_gen(g.subtype==_SEQ__VECT?g._VECTptr->front():g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur res;
    if (g.subtype==_SEQ__VECT) {
        gen &v=g._VECTptr->back();
        int i=G.node_index(v);
        if (i==-1)
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        graphe::ivector adj;
        G.adjacent_nodes(i,adj);
        res=G.get_node_labels(adj);
    } else {
        int n=G.node_count();
        graphe::ivector adj;
        for (int i=0;i<n;++i) {
            G.adjacent_nodes(i,adj,false);
            res.push_back(_sort(G.get_node_labels(adj),contextptr));
        }
    }
    return res;
}
static const char _neighbors_s[]="neighbors";
static define_unary_function_eval(__neighbors,&_neighbors,_neighbors_s);
define_unary_function_ptr5(at_neighbors,alias_at_neighbors,&__neighbors,0,true)

/* USAGE:   minimum_degree(G)
 *
 * Returns the smallest degree among vertices in graph G.
 */
gen _minimum_degree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
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
static const char _minimum_degree_s[]="minimum_degree";
static define_unary_function_eval(__minimum_degree,&_minimum_degree,_minimum_degree_s);
define_unary_function_ptr5(at_minimum_degree,alias_at_minimum_degree,&__minimum_degree,0,true)

/* USAGE:   maximum_degree(G)
 *
 * Returns the largest degree among vertices in graph G.
 */
gen _maximum_degree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
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
static const char _maximum_degree_s[]="maximum_degree";
static define_unary_function_eval(__maximum_degree,&_maximum_degree,_maximum_degree_s);
define_unary_function_ptr5(at_maximum_degree,alias_at_maximum_degree,&__maximum_degree,0,true)

/* USAGE:   is_regular(G)
 *
 * Returns true iff max and min degrees of graph G are equal.
 */
gen _is_regular(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
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
static const char _is_regular_s[]="is_regular";
static define_unary_function_eval(__is_regular,&_is_regular,_is_regular_s);
define_unary_function_ptr5(at_is_regular,alias_at_is_regular,&__is_regular,0,true)

/* USAGE:   isomorphic_copy(G,sigma)
 *
 * Returns a new graph H with vertices reordered according to the permutation sigma.
 */
gen _isomorphic_copy(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    if (gv[1].type!=_VECT || !is_integer_vecteur(*gv[1]._VECTptr))
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur &sigma=*gv[1]._VECTptr;
    graphe::ivector v(sigma.size());
    int offset=array_start(contextptr);
    for (const_iterateur it=sigma.begin();it!=sigma.end();++it) {
        v[it-sigma.begin()]=it->val-offset;
    }
    graphe H;
    if (!G.isomorphic_copy(H,v))
        return gentypeerr(contextptr);
    return H.to_gen();
}
static const char _isomorphic_copy_s[]="isomorphic_copy";
static define_unary_function_eval(__isomorphic_copy,&_isomorphic_copy,_isomorphic_copy_s);
define_unary_function_ptr5(at_isomorphic_copy,alias_at_isomorphic_copy,&__isomorphic_copy,0,true)

/* USAGE:   permute_vertices(G,V)
 *
 * Returns a copy of graph G with vertices reordered according to the order in
 * the list of vertices V.
 */
gen _permute_vertices(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    if (gv[1].type!=_VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur &sigma=*gv[1]._VECTptr,V=G.vertices();
    if (sigma.size()!=V.size())
        return gensizeerr(contextptr);
    graphe::ivector v(sigma.size(),-1);
    const_iterateur jt;
    int i;
    for (const_iterateur it=sigma.begin();it!=sigma.end();++it) {
        if ((jt=find(V.begin(),V.end(),*it))==V.end())
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        i=jt-V.begin();
        if (find(v.begin(),v.end(),i)!=v.end())
            return gentypeerr(contextptr);
        v[it-sigma.begin()]=i;
    }
    graphe H;
    if (!G.isomorphic_copy(H,v))
        return gentypeerr(contextptr);
    return H.to_gen();
}
static const char _permute_vertices_s[]="permute_vertices";
static define_unary_function_eval(__permute_vertices,&_permute_vertices,_permute_vertices_s);
define_unary_function_ptr5(at_permute_vertices,alias_at_permute_vertices,&__permute_vertices,0,true)

/* USAGE:   relabel_vertices(G,V)
 *
 * Returns a new graph H with vertex labels changed to those in V.
 */
gen _relabel_vertices(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    if (gv[1].type!=_VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur &labels=*gv[1]._VECTptr;
    if (int(labels.size())!=G.node_count())
        return gensizeerr(contextptr);
    if (!G.relabel_nodes(labels))
        return gentypeerr(contextptr);
    return G.to_gen();
}
static const char _relabel_vertices_s[]="relabel_vertices";
static define_unary_function_eval(__relabel_vertices,&_relabel_vertices,_relabel_vertices_s);
define_unary_function_ptr5(at_relabel_vertices,alias_at_relabel_vertices,&__relabel_vertices,0,true)

/* USAGE:   is_tree(G)
 *
 * Returns true iff the graph G is a tree, i.e. an undirected connected graph with
 * exactly n-1 edges, where n is the number of nodes of G.
 */
gen _is_tree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_tree());
}
static const char _is_tree_s[]="is_tree";
static define_unary_function_eval(__is_tree,&_is_tree,_is_tree_s);
define_unary_function_ptr5(at_is_tree,alias_at_is_tree,&__is_tree,0,true)

/* USAGE:   is_forest(G)
 *
 * Returns true iff the graph G is a forest, i.e. an undirected graph whose
 * connected components are all trees.
 */
gen _is_forest(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_forest());
}
static const char _is_forest_s[]="is_forest";
static define_unary_function_eval(__is_forest,&_is_forest,_is_forest_s);
define_unary_function_ptr5(at_is_forest,alias_at_is_forest,&__is_forest,0,true)

/* USAGE:   is_tournament(G)
 *
 * Returns true iff the graph G is a tournament, i.e. a complete graph with a
 * direction for each edge.
 */
gen _is_tournament(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_tournament());
}
static const char _is_tournament_s[]="is_tournament";
static define_unary_function_eval(__is_tournament,&_is_tournament,_is_tournament_s);
define_unary_function_ptr5(at_is_tournament,alias_at_is_tournament,&__is_tournament,0,true)

/* USAGE:   tree_height(T,r)
 *
 * Returns the height of the tree T with r as the root node.
 */
gen _tree_height(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
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
static const char _tree_height_s[]="tree_height";
static define_unary_function_eval(__tree_height,&_tree_height,_tree_height_s);
define_unary_function_ptr5(at_tree_height,alias_at_tree_height,&__tree_height,0,true)

/* USAGE:   is_triangle_free(G)
 *
 * Returns true iff undirected graph G is triangle-free, i.e. contains no
 * 3-cliques.
 */
gen _is_triangle_free(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_triangle_free());
}
static const char _is_triangle_free_s[]="is_triangle_free";
static define_unary_function_eval(__is_triangle_free,&_is_triangle_free,_is_triangle_free_s);
define_unary_function_ptr5(at_is_triangle_free,alias_at_is_triangle_free,&__is_triangle_free,0,true)

/* USAGE:   is_connected(G)
 *
 * Returns true iff graph G is connected.
 */
gen _is_connected(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_connected());
}
static const char _is_connected_s[]="is_connected";
static define_unary_function_eval(__is_connected,&_is_connected,_is_connected_s);
define_unary_function_ptr5(at_is_connected,alias_at_is_connected,&__is_connected,0,true)

/* USAGE:   is_biconnected(G)
 *
 * Returns true iff graph G is biconnected. Running time is O(n+m).
 */
gen _is_biconnected(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_biconnected());
}
static const char _is_biconnected_s[]="is_biconnected";
static define_unary_function_eval(__is_biconnected,&_is_biconnected,_is_biconnected_s);
define_unary_function_ptr5(at_is_biconnected,alias_at_is_biconnected,&__is_biconnected,0,true)

/* USAGE:   is_triconnected(G)
 *
 * Returns true iff graph G is triconnected. Running time is O(n*(n+m)).
 */
gen _is_triconnected(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_triconnected());
}
static const char _is_triconnected_s[]="is_triconnected";
static define_unary_function_eval(__is_triconnected,&_is_triconnected,_is_triconnected_s);
define_unary_function_ptr5(at_is_triconnected,alias_at_is_triconnected,&__is_triconnected,0,true)

/* USAGE:   is_weighted(G)
 *
 * Returns true iff graph G is weighted.
 */
gen _is_weighted(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_weighted());
}
static const char _is_weighted_s[]="is_weighted";
static define_unary_function_eval(__is_weighted,&_is_weighted,_is_weighted_s);
define_unary_function_ptr5(at_is_weighted,alias_at_is_weighted,&__is_weighted,0,true)

/* USAGE:   is_planar(G,[F])
 *
 * Returns true iff graph G is planar [and store the list of its faces to F if G
 * is biconnected].
 */
gen _is_planar(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    gen F=undef;
    if (g.subtype==_SEQ__VECT) {
        if (g._VECTptr->size()!=2)
            return gensizeerr(contextptr);
        if (g._VECTptr->back().type!=_IDNT)
            return gentypeerr(contextptr);
        F=g._VECTptr->back();
    }
    graphe G(contextptr),U(contextptr);
    if (!G.read_gen(g.subtype==_SEQ__VECT?g._VECTptr->front():g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    graphe::ivectors faces;
    G.underlying(U);
    if (!U.is_planar(faces))
        return G.boole(false);
    if (!is_undef(F)) {
        if (!U.is_biconnected())
            return gt_err(_GT_ERR_BICONNECTED_GRAPH_REQUIRED,contextptr);
        vecteur res;
        for (graphe::ivectors_iter it=faces.begin();it!=faces.end();++it) {
            res.push_back(G.get_node_labels(*it));
        }
        _eval(symbolic(at_sto,res,F),contextptr);
    }
    return G.boole(true);
}
static const char _is_planar_s[]="is_planar";
static define_unary_function_eval(__is_planar,&_is_planar,_is_planar_s);
define_unary_function_ptr5(at_is_planar,alias_at_is_planar,&__is_planar,0,true)

/* USAGE:   complete_binary_tree(n)
 *
 * Returns the complete binary tree with depth equal to n.
 */
gen _complete_binary_tree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (!g.is_integer() || g.val<0)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    G.make_complete_kary_tree(2,g.val);
    return G.to_gen();
}
static const char _complete_binary_tree_s[]="complete_binary_tree";
static define_unary_function_eval(__complete_binary_tree,&_complete_binary_tree,_complete_binary_tree_s);
define_unary_function_ptr5(at_complete_binary_tree,alias_at_complete_binary_tree,&__complete_binary_tree,0,true)

/* USAGE:   complete_kary_tree(k,n)
 *
 * Returns the complete k-ary tree with depth equal to n.
 */
gen _complete_kary_tree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    if (!gv.front().is_integer() || !gv.back().is_integer())
        return gentypeerr(contextptr);
    int k=gv.front().val,n=gv.back().val;
    if (k<2 || n<1)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    G.make_complete_kary_tree(k,n);
    return G.to_gen();
}
static const char _complete_kary_tree_s[]="complete_kary_tree";
static define_unary_function_eval(__complete_kary_tree,&_complete_kary_tree,_complete_kary_tree_s);
define_unary_function_ptr5(at_complete_kary_tree,alias_at_complete_kary_tree,&__complete_kary_tree,0,true)

/* USAGE:   prism_graph(n)
 *
 * Returns the prism graph of order n, i.e. the generalized Petersen graph
 * GP(n,1).
 */
gen _prism_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (!g.is_integer() || g.val<3)
        return gentypeerr(contextptr);
    return _petersen_graph(makesequence(g,1),contextptr);
}
static const char _prism_graph_s[]="prism_graph";
static define_unary_function_eval(__prism_graph,&_prism_graph,_prism_graph_s);
define_unary_function_ptr5(at_prism_graph,alias_at_prism_graph,&__prism_graph,0,true)

/* USAGE:   antiprism_graph(n)
 *
 * Returns the antiprism graph of order n.
 */
gen _antiprism_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (!g.is_integer() || g.val<3)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    G.make_antiprism_graph(g.val);
    return G.to_gen();
}
static const char _antiprism_graph_s[]="antiprism_graph";
static define_unary_function_eval(__antiprism_graph,&_antiprism_graph,_antiprism_graph_s);
define_unary_function_ptr5(at_antiprism_graph,alias_at_antiprism_graph,&__antiprism_graph,0,true)

/* USAGE:   star_graph(n)
 *
 * Returns the star graph with n+1 vertices, i.e. the complete bipartite graph
 * complete_graph(1,n).
 */
gen _star_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (!g.is_integer() || g.val<1)
        return gentypeerr(contextptr);
    return _complete_graph(makesequence(1,g),contextptr);
}
static const char _star_graph_s[]="star_graph";
static define_unary_function_eval(__star_graph,&_star_graph,_star_graph_s);
define_unary_function_ptr5(at_star_graph,alias_at_star_graph,&__star_graph,0,true)

/* USAGE:   wheel_graph(n)
 *
 * Returns the wheel graph with n+1 vertices.
 */
gen _wheel_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (!g.is_integer() || g.val<3)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    G.make_wheel_graph(g.val);
    return G.to_gen();
}
static const char _wheel_graph_s[]="wheel_graph";
static define_unary_function_eval(__wheel_graph,&_wheel_graph,_wheel_graph_s);
define_unary_function_ptr5(at_wheel_graph,alias_at_wheel_graph,&__wheel_graph,0,true)

/* USAGE:   grid_graph(m,n)
 *
 * Returns the grid graph on m*n vertices, where m,n>=2.
 */
gen _grid_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    if (!gv.front().is_integer() || !gv.back().is_integer())
        return gentypeerr(contextptr);
    int m=gv.front().val,n=gv.back().val;
    if (m<2 || n<2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    G.make_grid_graph(m,n);
    return G.to_gen();
}
static const char _grid_graph_s[]="grid_graph";
static define_unary_function_eval(__grid_graph,&_grid_graph,_grid_graph_s);
define_unary_function_ptr5(at_grid_graph,alias_at_grid_graph,&__grid_graph,0,true)

/* USAGE:   torus_grid_graph(m,n)
 *
 * Returns the torus grid graph on m*n vertices, where m,n>=3.
 */
gen _torus_grid_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    if (!gv.front().is_integer() || !gv.back().is_integer())
        return gentypeerr(contextptr);
    int m=gv.front().val,n=gv.back().val;
    if (m<3 || n<3)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    G.make_grid_graph(m,n,true);
    return G.to_gen();
}
static const char _torus_grid_graph_s[]="torus_grid_graph";
static define_unary_function_eval(__torus_grid_graph,&_torus_grid_graph,_torus_grid_graph_s);
define_unary_function_ptr5(at_torus_grid_graph,alias_at_torus_grid_graph,&__torus_grid_graph,0,true)

/* USAGE:   web_graph(a,b)
 *
 * Returns the web graph on a*b vertices, where a>=3 and b>=2.
 */
gen _web_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    if (!gv.front().is_integer() || !gv.back().is_integer())
        return gentypeerr(contextptr);
    int a=gv.front().val,b=gv.back().val;
    if (a<3 || b<2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    G.make_web_graph(a,b);
    return G.to_gen();
}
static const char _web_graph_s[]="web_graph";
static define_unary_function_eval(__web_graph,&_web_graph,_web_graph_s);
define_unary_function_ptr5(at_web_graph,alias_at_web_graph,&__web_graph,0,true)

/* USAGE:   cartesian_product(G1,G2,...)
 *
 * Returns Cartesian product of graphs G1, G2, ... Vertices in the resulting
 * graph are labelled as "u:v:..." where u, v, ... are vertices from G1, G2,
 * ..., respectively.
 */
gen _cartesian_product(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    graphe P(contextptr);
    if (!compute_product_of_graphs(gv,P,true,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return P.to_gen();
}
static const char _cartesian_product_s[]="cartesian_product";
static define_unary_function_eval(__cartesian_product,&_cartesian_product,_cartesian_product_s);
define_unary_function_ptr5(at_cartesian_product,alias_at_cartesian_product,&__cartesian_product,0,true)

/* USAGE:   tensor_product(G1,G2,...)
 *
 * Returns tensor product of graphs G1, G2, ... Vertices in the resulting graph
 * are labelled as "u:v:..." where u, v, ... are vertices from G1, G2, ...,
 * respectively.
 */
gen _tensor_product(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    graphe P(contextptr);
    if (!compute_product_of_graphs(gv,P,false,contextptr))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return P.to_gen();
}
static const char _tensor_product_s[]="tensor_product";
static define_unary_function_eval(__tensor_product,&_tensor_product,_tensor_product_s);
define_unary_function_ptr5(at_tensor_product,alias_at_tensor_product,&__tensor_product,0,true)

/* USAGE:   path_graph(n or V)
 *
 * Returns a path graph with n vertices, which can also be specified as list of
 * their labels.
 */
gen _path_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!vertices_from_integer_or_vecteur(g,G))
        return gentypeerr(contextptr);
    int n=G.node_count();
    if (n<2)
        return gensizeerr(contextptr);
    for (int i=0;i<n-1;++i) {
        G.add_edge(i,i+1);
    }
    return G.to_gen();
}
static const char _path_graph_s[]="path_graph";
static define_unary_function_eval(__path_graph,&_path_graph,_path_graph_s);
define_unary_function_ptr5(at_path_graph,alias_at_path_graph,&__path_graph,0,true)

/* USAGE:   eulerian_path(G,[V])
 *
 * Returns true iff graph G is eulerian, i.e. if it has eulerian path. If
 * identifier V is specified as the second argument, that path is written to it.
 */
gen _is_eulerian(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    bool has_path_idnt=g.subtype==_SEQ__VECT;
    graphe G(contextptr);
    if (!G.read_gen(has_path_idnt?g._VECTptr->front():g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    graphe::ivector path;
    bool iscycle; // dummy
    if ((has_path_idnt && !G.find_eulerian_path(path)) ||
            (!has_path_idnt && !G.eulerian_path_start(iscycle)))
        return graphe::boole(false);
    if (has_path_idnt) {
        if (g._VECTptr->size()!=2)
            return gensizeerr(contextptr);
        // output path as vecteur V
        gen V=g._VECTptr->back();
        if (V.type!=_IDNT)
            return gentypeerr(contextptr);
        vecteur P(path.size());
        int i=0;
        for (graphe::ivector_iter it=path.begin();it!=path.end();++it) {
            P[i++]=G.node_label(*it);
        }
        identifier_assign(*V._IDNTptr,P,contextptr);
    }
    return graphe::boole(true);
}
static const char _is_eulerian_s[]="is_eulerian";
static define_unary_function_eval(__is_eulerian,&_is_eulerian,_is_eulerian_s);
define_unary_function_ptr5(at_is_eulerian,alias_at_is_eulerian,&__is_eulerian,0,true)

/* USAGE:   kneser_graph(n,k)
 *
 * Returns Kneser graph K(n,k) with comb(n,k) vertices. The largest acceptable
 * value of n is 20. Kneser graphs with more than 10000 vertices will not be
 * created.
 */
gen _kneser_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    if (!gv.front().is_integer() || !gv.back().is_integer())
        return gentypeerr(contextptr);
    int n=gv.front().val,k=gv.back().val;
    if (n<2 || n>20 || k<1 || k>=n)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.make_kneser_graph(n,k))
        return gensizeerr(contextptr);
    return G.to_gen();
}
static const char _kneser_graph_s[]="kneser_graph";
static define_unary_function_eval(__kneser_graph,&_kneser_graph,_kneser_graph_s);
define_unary_function_ptr5(at_kneser_graph,alias_at_kneser_graph,&__kneser_graph,0,true)

/* USAGE:   odd_graph(n)
 *
 * Returns odd graph of order n as Kneser graph K(2n-1,n-1). The largest
 * acceptable value of n is 8.
 */
gen _odd_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (!g.is_integer())
        return gentypeerr(contextptr);
    int n=g.val;
    if (n<2 || n>8)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    assert(G.make_kneser_graph(2*n-1,n-1));
    return G.to_gen();
}
static const char _odd_graph_s[]="odd_graph";
static define_unary_function_eval(__odd_graph,&_odd_graph,_odd_graph_s);
define_unary_function_ptr5(at_odd_graph,alias_at_odd_graph,&__odd_graph,0,true)

/* USAGE:   highlight_vertex(G,V,[C])
 *
 * Change color of vertex or list of vertices V in graph G to C (or green, if C
 * is not specified) and return the modified graph.
 */
gen _highlight_vertex(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur V;
    if (gv[1].type==_VECT)
        V=*gv[1]._VECTptr;
    else
        V.push_back(gv[1]);
    graphe::ivector indices;
    int index;
    for (const_iterateur it=V.begin();it!=V.end();++it) {
        index=G.node_index(*it);
        if (index<0)
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        indices.push_back(index);
    }
    gen C=gv.size()==3?gv.back():graphe::default_highlighted_vertex_color;
    if (!parse_vertex_colors(G,C,indices))
        return gentypeerr(contextptr);
    return G.to_gen();
}
static const char _highlight_vertex_s[]="highlight_vertex";
static define_unary_function_eval(__highlight_vertex,&_highlight_vertex,_highlight_vertex_s);
define_unary_function_ptr5(at_highlight_vertex,alias_at_highlight_vertex,&__highlight_vertex,0,true)

/* USAGE:   highlight_edges(G,E,[C])
 *
 * Change color of edge or list of edges E in graph V to C (or red, if C is not
 * specified) and return the modified graph.
 */
gen _highlight_edges(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (gv[1].type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &E=*gv[1]._VECTptr;
    graphe::ipairs edges;
    bool notfound=false;
    if (!G.edges2ipairs(E,edges,notfound))
        return notfound?gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr):gensizeerr(contextptr);
    gen C=gv.size()==3?gv.back():graphe::default_highlighted_edge_color;
    for (graphe::ipairs_iter it=edges.begin();it!=edges.end();++it) {
        G.set_edge_attribute(it->first,it->second,_GT_ATTRIB_COLOR,C);
    }
    return G.to_gen();
}
static const char _highlight_edges_s[]="highlight_edges";
static define_unary_function_eval(__highlight_edges,&_highlight_edges,_highlight_edges_s);
define_unary_function_ptr5(at_highlight_edges,alias_at_highlight_edges,&__highlight_edges,0,true)

/* USAGE:   highlight_subgraph(G,S,[C1,C2])
 *
 * Change color of edges and vertices from S (which is a subgraph of G or a
 * list of subgraphs of G) to C1 and C2, respectively (by default, C1=red and
 * C2=green).
 */
gen _highlight_subgraph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    if (gv[1].type!=_VECT)
        return gentypeerr(contextptr);
    int C1=graphe::default_highlighted_edge_color,C2=graphe::default_highlighted_vertex_color;
    if (gv.size()==4) {
        if (!gv[2].is_integer() || !gv[3].is_integer())
            return gentypeerr(contextptr);
        C1=gv[2].val;
        C2=gv[3].val;
    }
    gen modG;
    if (gv[1]._VECTptr->front().type==_VECT) {
        modG=gv.front();
        for (const_iterateur it=gv[1]._VECTptr->begin();it!=gv[1]._VECTptr->end();++it) {
            modG=_highlight_subgraph(makesequence(modG,*it,C1,C2),contextptr);
        }
        return modG;
    }
    graphe G(contextptr),S(contextptr);
    if (!G.read_gen(gv[0]) || !S.read_gen(gv[1]))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!S.is_subgraph(G))
        return gt_err(_GT_ERR_NOT_A_SUBGRAPH,contextptr);
    vecteur V=S.vertices(),E=S.edges(false);
    modG=_highlight_edges(makesequence(G.to_gen(),E,C1),contextptr);
    return _highlight_vertex(makesequence(modG,V,C2),contextptr);
}
static const char _highlight_subgraph_s[]="highlight_subgraph";
static define_unary_function_eval(__highlight_subgraph,&_highlight_subgraph,_highlight_subgraph_s);
define_unary_function_ptr5(at_highlight_subgraph,alias_at_highlight_subgraph,&__highlight_subgraph,0,true)

/* USAGE:   highlight_trail(G,T,[C])
 *
 * Change color of edges in G which lie along the trail T to C (or red, if C is
 * not specified).
 */
gen _highlight_trail(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    if (gv[1].type!=_VECT || (gv.size()==3 && !gv.back().is_integer()))
        return gentypeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    vecteur V=*gv[1]._VECTptr;
    int C=gv.size()==3?gv.back().val:_RED,i,j;
    for (const_iterateur it=V.begin();it!=V.end()-1;++it) {
        const gen &v=*it,&w=*(it+1);
        if ((i=G.node_index(v))<0 || (j=G.node_index(w))<0)
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        if (!G.has_edge(i,j))
            return gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
        G.set_edge_attribute(i,j,_GT_ATTRIB_COLOR,C);
    }
    return G.to_gen();
}
static const char _highlight_trail_s[]="highlight_trail";
static define_unary_function_eval(__highlight_trail,&_highlight_trail,_highlight_trail_s);
define_unary_function_ptr5(at_highlight_trail,alias_at_highlight_trail,&__highlight_trail,0,true)

/* USAGE:   disjoint_union(G1,G2,...)
 *
 * Returns the disjoint union of the input graphs. Vertices in the resulting
 * graph are labelled with "k:v", where k is index of the corresponding k-th
 * graph Gk and v is vertex in Gk.
 */
gen _disjoint_union(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    int err;
    if ((err=graphunion(G,*g._VECTptr,true))>=0)
        return gt_err(err,contextptr);
    return G.to_gen();
}
static const char _disjoint_union_s[]="disjoint_union";
static define_unary_function_eval(__disjoint_union,&_disjoint_union,_disjoint_union_s);
define_unary_function_ptr5(at_disjoint_union,alias_at_disjoint_union,&__disjoint_union,0,true)

/* USAGE:   graph_union(G1,G2,...)
 *
 * Returns the union of the input graphs. Set of vertices of the resulting
 * graph is the union of the sets of vertices of the input graphs and the set
 * of edges of the resulting graph is the union of sets of edges of the input
 * graphs. If graphs G1 and G2 are both weighted, the weight of any common edge
 * is the sum of the weights of that edge in G1 and G2.
 */
gen _graph_union(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    graphe G(contextptr);
    int err;
    if ((err=graphunion(G,*g._VECTptr,false))>=0)
        return gt_err(err,contextptr);
    return G.to_gen();
}
static const char _graph_union_s[]="graph_union";
static define_unary_function_eval(__graph_union,&_graph_union,_graph_union_s);
define_unary_function_ptr5(at_graph_union,alias_at_graph_union,&__graph_union,0,true)

/* USAGE:   graph_join(G,H)
 *
 * Returns the graph obtained by connecting every vertex from G with every
 * vertex from H. The vertex labels in the resulting graph are strings of form
 * "1:u" and "2:v" where u and v are vertices from G and H, respectively.
 */
gen _graph_join(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    graphe G(contextptr),G1(contextptr),G2(contextptr);
    if (!G1.read_gen(gv.front()) || !G2.read_gen(gv.back()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G1.is_directed() || G2.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    if (G1.is_weighted() || G2.is_weighted())
        return gt_err(_GT_ERR_UNWEIGHTED_GRAPH_REQUIRED,contextptr);
    vecteur V=G1.vertices(),W=G2.vertices();
    stringstream ss;
    for (iterateur it=V.begin();it!=V.end();++it) {
        add_prefix_to_vertex_label(*it,1,ss);
    }
    for (iterateur it=W.begin();it!=W.end();++it) {
        add_prefix_to_vertex_label(*it,2,ss);
    }
    graphunion(G,gv,true);
    for (const_iterateur it=V.begin();it!=V.end();++it) {
        for (const_iterateur jt=W.begin();jt!=W.end();++jt) {
            G.add_edge(*it,*jt);
        }
    }
    return G.to_gen();
}
static const char _graph_join_s[]="graph_join";
static define_unary_function_eval(__graph_join,&_graph_join,_graph_join_s);
define_unary_function_ptr5(at_graph_join,alias_at_graph_join,&__graph_join,0,true)

/* USAGE:   graph_equal(G1,G2)
 *
 * Returns true iff G1 is equal to G2, that is when the sets of vertices and
 * edges of G1 and G2, as well as the orderings of vertices in both graphs,
 * mutually coincide. If the graphs are weighted (they must both be
 * (un)weighted and (un)directed), weights given to the same edge in two graphs
 * must be equal.
 */
gen _graph_equal(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    graphe G1(contextptr),G2(contextptr);
    if (!G1.read_gen(gv.front()) || !G2.read_gen(gv.back()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G1.is_equal(G2));
}
static const char _graph_equal_s[]="graph_equal";
static define_unary_function_eval(__graph_equal,&_graph_equal,_graph_equal_s);
define_unary_function_ptr5(at_graph_equal,alias_at_graph_equal,&__graph_equal,0,true)

/* USAGE:   reverse_graph(G)
 *
 * Returns reverse graph of G, i.e. the graph G with the directions of all edges
 * reversed.
 */
gen _reverse_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr),H(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return G.to_gen();
    G.reverse(H);
    return H.to_gen();
}
static const char _reverse_graph_s[]="reverse_graph";
static define_unary_function_eval(__reverse_graph,&_reverse_graph,_reverse_graph_s);
define_unary_function_ptr5(at_reverse_graph,alias_at_reverse_graph,&__reverse_graph,0,true)

/* USAGE:   interval_graph(a..b,c..d,...)
 *          interval_graph([a..b,c..d,...])
 *
 * Returns the interval graph with respect to intervals a..b, c..d, ... on the
 * real line. It has one vertex per interval and two vertices are connected iff
 * the corresponding intervals intersect.
 */
gen _interval_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    gen a,b;
    stringstream ss;
    int n=gv.size();
    vecteur V;
    V.reserve(n);
    for (const_iterateur it=gv.begin();it!=gv.end();++it) {
        if (!it->is_symb_of_sommet(at_interval))
            return gentypeerr(contextptr);
        a=it->_SYMBptr->feuille._VECTptr->front();
        b=it->_SYMBptr->feuille._VECTptr->back();
        if (!graphe::is_real_number(a) || !graphe::is_real_number(b))
            return gentypeerr(contextptr);
        ss.str("");
        ss << a << " .. " << b;
        V.push_back(graphe::str2gen(ss.str(),true));
    }
    graphe G(contextptr);
    G.add_nodes(V);
    for (const_iterateur it=gv.begin();it!=gv.end();++it) {
        const gen &a1=it->_SYMBptr->feuille._VECTptr->front();
        const gen &b1=it->_SYMBptr->feuille._VECTptr->back();
        for (const_iterateur jt=it+1;jt!=gv.end();++jt) {
            const gen &a2=jt->_SYMBptr->feuille._VECTptr->front();
            const gen &b2=jt->_SYMBptr->feuille._VECTptr->back();
            if (is_greater(b2,a1,contextptr) && is_greater(b1,a2,contextptr))
                G.add_edge(it-gv.begin(),jt-gv.begin());
        }
    }
    return G.to_gen();
}
static const char _interval_graph_s[]="interval_graph";
static define_unary_function_eval(__interval_graph,&_interval_graph,_interval_graph_s);
define_unary_function_ptr5(at_interval_graph,alias_at_interval_graph,&__interval_graph,0,true)

/* USAGE:   subdivide_edges(G,E,[r])
 *
 * Inserts r (by default 1) new vertices to each edge/arc from G contained in
 * the list E (which may be a single edge/arc) and returns a modified copy of
 * G. New vertices are labeled with the smallest available integers.
 */
gen _subdivide_edges(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv[1].type!=_VECT)
        return gentypeerr(contextptr);
    int r=1;
    if (gv.size()==3) {
        if(!gv[2].is_integer())
            return gentypeerr(contextptr);
        if ((r=gv[2].val)<1)
            return gensizeerr(contextptr);
    }
    vecteur &E=*gv[1]._VECTptr;
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int i,j,l=G.largest_integer_label();
    graphe::ipairs edges;
    if (ckmatrix(E)) {
        // a list of edges/arcs is given
        if (E.front()._VECTptr->size()!=2)
            return gensizeerr(contextptr);
        edges.resize(E.size());
        int k=0;
        for (const_iterateur it=E.begin();it!=E.end();++it) {
            const vecteur &e=*(it->_VECTptr);
            i=G.node_index(e.front());
            j=G.node_index(e.back());
            if (i<0 || j<0)
                return gt_err(i<0?e.front():e.back(),_GT_ERR_VERTEX_NOT_FOUND,contextptr);
            if (!G.has_edge(i,j))
                return gt_err(e,_GT_ERR_EDGE_NOT_FOUND,contextptr);
            edges[k++]=make_pair(i,j);
        }
    } else {
        // a single edge/arc is given
        if (E.size()!=2)
            return gensizeerr(contextptr);
        edges.push_back(make_pair(G.node_index(E.front()),G.node_index(E.back())));
    }
    int v,w;
    for (graphe::ipairs_iter it=edges.begin();it!=edges.end();++it) {
        G.remove_edge(*it);
        v=it->first;
        for (int k=0;k<r;++k) {
           w=G.add_node(++l);
           G.add_edge(v,w);
           v=w;
        }
        G.add_edge(v,it->second);
    }
    return G.to_gen();
}
static const char _subdivide_edges_s[]="subdivide_edges";
static define_unary_function_eval(__subdivide_edges,&_subdivide_edges,_subdivide_edges_s);
define_unary_function_ptr5(at_subdivide_edges,alias_at_subdivide_edges,&__subdivide_edges,0,true)

/* USAGE:   graph_power(G,k)
 *
 * Returns the k-th power of graph G, where two vertices are connected iff there
 * exists a path of length at most k in the original graph.
 */
gen _graph_power(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=2)
        return gensizeerr(contextptr);
    if (!gv.back().is_integer())
        return gentypeerr(contextptr);
    int k=gv.back().val;
    if (k<1)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int n=G.node_count();
    matrice m,mpow;
    G.adjacency_matrix(m);
    mpow=m;
    for (int i=1;i<k;++i) {
        for (int j=0;j<n;++j) {
            mpow[j]._VECTptr->at(j)+=1;
        }
        mpow=mmult(mpow,m);
    }
    for (int i=0;i<n;++i) {
        for (int j=0;j<n;++j) {
            gen &mij=mpow[i]._VECTptr->at(j);
            if (!is_zero(mij))
                mij=1;
        }
    }
    gen opt=symbolic(at_equal,makesequence(_GT_DIRECTED,G.is_directed()));
    return _graph(makesequence(G.vertices(),mpow,opt),contextptr);
}
static const char _graph_power_s[]="graph_power";
static define_unary_function_eval(__graph_power,&_graph_power,_graph_power_s);
define_unary_function_ptr5(at_graph_power,alias_at_graph_power,&__graph_power,0,true)

/* USAGE:   vertex_distance(G,s,t)
 *          vertex_distance(G,s,T)
 *
 * Returns the number of edges in the shortest path from vertex s to vertex t
 * in graph G. If such path does not exist, returns +infinity. For vector T of
 * vertices from G returns the list of distances from s to each vertex t in T.
 */
gen _vertex_distance(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=3)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int i,j;
    if ((i=G.node_index(gv[1]))<0)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    vecteur T;
    bool single=false;
    if (gv[2].type==_VECT)
        T=*gv[2]._VECTptr;
    else {
        T.push_back(gv[2]);
        single=true;
    }
    graphe::ivector J(T.size()),dist;
    for (const_iterateur it=T.begin();it!=T.end();++it) {
        if ((j=G.node_index(*it))<0)
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        J[it-T.begin()]=j;
    }
    G.distance(i,J,dist);
    vecteur res(T.size());
    for (graphe::ivector_iter it=dist.begin();it!=dist.end();++it) {
        res[it-dist.begin()]=*it>=0?gen(*it):graphe::plusinf();
    }
    return single?res.front():res;
}
static const char _vertex_distance_s[]="vertex_distance";
static define_unary_function_eval(__vertex_distance,&_vertex_distance,_vertex_distance_s);
define_unary_function_ptr5(at_vertex_distance,alias_at_vertex_distance,&__vertex_distance,0,true)

/* USAGE:   shortest_path(G,s,t)
 *          shortest_path(G,s,T)
 *
 * Returns the shortest path from vertex s to vertex t in graph G. If such path
 * does not exist, returns an empty list. If vector T of vertices from G is
 * given, the list of shortest paths from s to each t int T is returned.
 */
gen _shortest_path(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=3)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int i,j;
    if ((i=G.node_index(gv[1]))<0)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    vecteur T;
    bool single=false;
    if (gv[2].type==_VECT)
        T=*gv[2]._VECTptr;
    else {
        T.push_back(gv[2]);
        single=true;
    }
    graphe::ivector J(T.size()),dist;
    for (const_iterateur it=T.begin();it!=T.end();++it) {
        if ((j=G.node_index(*it))<0)
            return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        J[it-T.begin()]=j;
    }
    graphe::ivectors shortest_paths;
    G.distance(i,J,dist,&shortest_paths);
    vecteur res(T.size());
    for (graphe::ivectors_iter it=shortest_paths.begin();it!=shortest_paths.end();++it) {
        i=it-shortest_paths.begin();
        res[it-shortest_paths.begin()]=dist[i]>=0?G.get_node_labels(*it):vecteur(0);
    }
    return single?res.front():res;
}
static const char _shortest_path_s[]="shortest_path";
static define_unary_function_eval(__shortest_path,&_shortest_path,_shortest_path_s);
define_unary_function_ptr5(at_shortest_path,alias_at_shortest_path,&__shortest_path,0,true)

/* USAGE:   allpairs_distance(G)
 *
 * Returns a square matrix D of order n(=number of vertices in G) such that
 * D(i,j) is the distance between i-th and j-th vertex of (weighted) graph G,
 * computed by using Floyd-Warshall algorithm with complexity O(n^3). If For
 * some vertex pair no path exists, the corresponding entry in D is equal to
 * +infinity. Edges may have positive or negative weights but G shouldn't
 * contain negative cycles.
 */
gen _allpairs_distance(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    matrice dist;
    if (!G.is_empty())
        G.allpairs_distance(dist);
    return dist;
}
static const char _allpairs_distance_s[]="allpairs_distance";
static define_unary_function_eval(__allpairs_distance,&_allpairs_distance,_allpairs_distance_s);
define_unary_function_ptr5(at_allpairs_distance,alias_at_allpairs_distance,&__allpairs_distance,0,true)

/* USAGE:   graph_diameter(G)
 *
 * Returns the diameter of graph G, i.e. the maximum distance between a pair of
 * vertices in G. If G is disconnected, its diameter is equal to +infinity.
 */
gen _graph_diameter(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_empty())
        return gt_err(_GT_ERR_GRAPH_IS_EMPTY,contextptr);
    if (!G.is_connected())
        return graphe::plusinf();
    matrice D;
    G.allpairs_distance(D);
    int n=G.node_count();
    gen max_dist(symbolic(at_neg,_IDNT_infinity()));
    for (int i=0;i<n;++i) {
        for (int j=0;j<n;++j) {
            const gen &dist=D[i][j];
            if (is_inf(dist))
                continue;
            max_dist=_max(makesequence(max_dist,dist),contextptr);
        }
    }
    return max_dist;
}
static const char _graph_diameter_s[]="graph_diameter";
static define_unary_function_eval(__graph_diameter,&_graph_diameter,_graph_diameter_s);
define_unary_function_ptr5(at_graph_diameter,alias_at_graph_diameter,&__graph_diameter,0,true)

/* USAGE:   dijkstra(G,v,w)
 *          dijkstra(G,v,W)
 *          dijkstra(G,v)
 *
 * Returns the cheapest weighted path from vertex v to w in graph G. Output is
 * in form [[v1,v2,...,vk],d] where v1,v2,...,vk are vertices along the path
 * and d is the weight of the path. If no such path exists, returns
 * [[],+infinity]. Also, when list W of vertices is specified, a sequence of
 * cheapest paths to vertices from W is returned. If W is omitted, it is
 * assumed that W=vertices(G).
 */
gen _dijkstra(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<2)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int v,i;
    if ((v=G.node_index(gv[1]))<0)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    int n=G.node_count();
    vecteur V,path_weights,paths;
    graphe::ivector dest;
    if (gv.size()==2) {
        V=G.vertices();
        dest.resize(n);
        for (i=0;i<n;++i) {
            dest[i]=i;
        }
    } else {
        if (gv[2].type==_VECT)
            V=*gv[2]._VECTptr;
        else V.push_back(gv[2]);
        dest.resize(V.size());
        i=0;
        for (const_iterateur it=V.begin();it!=V.end();++it) {
            if ((dest[i++]=G.node_index(*it))<0)
                return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        }
    }
    graphe::ivectors cheapest_paths;
    G.dijkstra(v,dest,path_weights,&cheapest_paths);
    paths.resize(dest.size());
    i=0;
    for (graphe::ivectors_iter it=cheapest_paths.begin();it!=cheapest_paths.end();++it) {
        vecteur &path=*(paths[i++]=vecteur(it->size()))._VECTptr;
        for (graphe::ivector_iter jt=it->begin();jt!=it->end();++jt) {
            path[jt-it->begin()]=G.node_label(*jt);
        }
    }
    if (gv.size()>2 && gv[2].type!=_VECT) {
        gen &w=path_weights.front();
        return makevecteur(is_inf(w)?vecteur(0):paths.front(),w);
    }
    vecteur res(dest.size());
    i=0;
    for (const_iterateur it=paths.begin();it!=paths.end();++it) {
        vecteur &path=*(it->_VECTptr);
        gen &w=path_weights[i];
        res[i++]=makevecteur(is_inf(w)?vecteur(0):path,w);
    }
    return change_subtype(res,_SEQ__VECT);
}
static const char _dijkstra_s[]="dijkstra";
static define_unary_function_eval(__dijkstra,&_dijkstra,_dijkstra_s);
define_unary_function_ptr5(at_dijkstra,alias_at_dijkstra,&__dijkstra,0,true)

/* USAGE:   topologic_sort(G)
 *
 * Returns the list of vertices sorted according to the topological ordering in
 * a directed acyclic graph G.
 */
gen _topologic_sort(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    graphe::ivector ordering;
    if (!G.topologic_sort(ordering))
        return gt_err(_GT_ERR_NOT_ACYCLIC_GRAPH,contextptr);
    vecteur res(ordering.size());
    for (graphe::ivector_iter it=ordering.begin();it!=ordering.end();++it) {
        res[it-ordering.begin()]=G.node_label(*it);
    }
    return res;
}
static const char _topologic_sort_s[]="topologic_sort";
static define_unary_function_eval(__topologic_sort,&_topologic_sort,_topologic_sort_s);
define_unary_function_ptr5(at_topologic_sort,alias_at_topologic_sort,&__topologic_sort,0,true)

gen _topological_sort(const gen &g,GIAC_CONTEXT) {
    return _topologic_sort(g,contextptr);
}
static const char _topological_sort_s[]="topological_sort";
static define_unary_function_eval(__topological_sort,&_topological_sort,_topological_sort_s);
define_unary_function_ptr5(at_topological_sort,alias_at_topological_sort,&__topological_sort,0,true)

/* USAGE:   is_acyclic(G)
 *
 * Returns true iff the directed graph G is acyclic, i.e. has no topological
 * ordering.
 */
gen _is_acyclic(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    graphe::ivector ordering;
    return graphe::boole(G.topologic_sort(ordering));
}
static const char _is_acyclic_s[]="is_acyclic";
static define_unary_function_eval(__is_acyclic,&_is_acyclic,_is_acyclic_s);
define_unary_function_ptr5(at_is_acyclic,alias_at_is_acyclic,&__is_acyclic,0,true)

/* USAGE:   is_clique(G)
 *
 * Returns true iff graph G is a clique (i.e. a complete graph).
 */
gen _is_clique(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    return graphe::boole(G.is_clique());
}
static const char _is_clique_s[]="is_clique";
static define_unary_function_eval(__is_clique,&_is_clique,_is_clique_s);
define_unary_function_ptr5(at_is_clique,alias_at_is_clique,&__is_clique,0,true)

/* USAGE:   maximum_clique(G)
 *
 * Returns maximum clique of undirected graph G as a list of vertices.
 */
gen _maximum_clique(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    graphe::ivector clique;
    G.maximum_clique(clique);
    return G.get_node_labels(clique);
}
static const char _maximum_clique_s[]="maximum_clique";
static define_unary_function_eval(__maximum_clique,&_maximum_clique,_maximum_clique_s);
define_unary_function_ptr5(at_maximum_clique,alias_at_maximum_clique,&__maximum_clique,0,true)

/* USAGE:   clique_number(G)
 *
 * Returns the clique number of graph G, which is equal to the size of maximum
 * clique.
 */
gen _clique_number(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    graphe::ivector clique;
    return G.maximum_clique(clique);
}
static const char _clique_number_s[]="clique_number";
static define_unary_function_eval(__clique_number,&_clique_number,_clique_number_s);
define_unary_function_ptr5(at_clique_number,alias_at_clique_number,&__clique_number,0,true)

/* USAGE:   clique_cover(G,[k])
 *
 * Returns a clique vertex cover of graph G [containing at most k cliques].
 */
gen _clique_cover(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    int k=0;
    if (g.type==_VECT && g.subtype==_SEQ__VECT) {
        if (g._VECTptr->size()!=2)
            return gensizeerr(contextptr);
        if (!g._VECTptr->back().is_integer())
            return gentypeerr(contextptr);
        k=g._VECTptr->back().val;
    }
    graphe G(contextptr);
    if (!G.read_gen(g.subtype==_SEQ__VECT?g._VECTptr->front():g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    graphe::ivectors cover;
    if (!G.clique_cover(cover,k))
        return vecteur(0);
    vecteur res;
    G.ivectors2vecteur(cover,res);
    return res;
}
static const char _clique_cover_s[]="clique_cover";
static define_unary_function_eval(__clique_cover,&_clique_cover,_clique_cover_s);
define_unary_function_ptr5(at_clique_cover,alias_at_clique_cover,&__clique_cover,0,true)

/* USAGE:   clique_cover_number(G)
 *
 * Returns the clique cover number of graph G (i.e. the chromatic number of the
 * graph complement of G).
 */
gen _clique_cover_number(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    graphe::ivectors cover;
    assert(G.clique_cover(cover));
    return cover.size();
}
static const char _clique_cover_number_s[]="clique_cover_number";
static define_unary_function_eval(__clique_cover_number,&_clique_cover_number,_clique_cover_number_s);
define_unary_function_ptr5(at_clique_cover_number,alias_at_clique_cover_number,&__clique_cover_number,0,true)

/* USAGE:   chromatic_number(G)
 *
 * Returns the chromatic number of graph G (i.e. the clique cover number of the
 * graph complement of G).
 */
gen _chromatic_number(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr),C(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    G.complement(C);
    graphe::ivectors cover;
    assert(C.clique_cover(cover));
    return cover.size();
}
static const char _chromatic_number_s[]="chromatic_number";
static define_unary_function_eval(__chromatic_number,&_chromatic_number,_chromatic_number_s);
define_unary_function_ptr5(at_chromatic_number,alias_at_chromatic_number,&__chromatic_number,0,true)

/* USAGE:   maximum_independent_set(G)
 *
 * Returns the maximum independent vertex set of graph G (i.e. maximum clique of
 * the graph complement of G).
 */
gen _maximum_independent_set(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr),C(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    G.complement(C);
    graphe::ivector clique;
    C.maximum_clique(clique);
    return C.get_node_labels(clique);
}
static const char _maximum_independent_set_s[]="maximum_independent_set";
static define_unary_function_eval(__maximum_independent_set,&_maximum_independent_set,_maximum_independent_set_s);
define_unary_function_ptr5(at_maximum_independent_set,alias_at_maximum_independent_set,&__maximum_independent_set,0,true)

/* USAGE:   independence_number(G)
 *
 * Returns the independence number of graph G (i.e. the size of maximum
 * independent set).
 */
gen _independence_number(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr),C(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    G.complement(C);
    graphe::ivector clique;
    return C.maximum_clique(clique);
}
static const char _independence_number_s[]="independence_number";
static define_unary_function_eval(__independence_number,&_independence_number,_independence_number_s);
define_unary_function_ptr5(at_independence_number,alias_at_independence_number,&__independence_number,0,true)

/* USAGE:   strongly_connected_components(G)
 *
 * Returns the list of strongly connected components of directed graph G.
 */
gen _strongly_connected_components(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    graphe::ivectors components;
    G.strongly_connected_components(components);
    vecteur res;
    G.ivectors2vecteur(components,res,true);
    return res;
}
static const char _strongly_connected_components_s[]="strongly_connected_components";
static define_unary_function_eval(__strongly_connected_components,&_strongly_connected_components,_strongly_connected_components_s);
define_unary_function_ptr5(at_strongly_connected_components,alias_at_strongly_connected_components,&__strongly_connected_components,0,true)

/* USAGE:   is_strongly_connected(G)
 *
 * Returns true iff the directed graph G is strongly connected.
 */
gen _is_strongly_connected(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_directed())
        return gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
    graphe::ivectors components;
    G.strongly_connected_components(components);
    return graphe::boole(components.size()==1);
}
static const char _is_strongly_connected_s[]="is_strongly_connected";
static define_unary_function_eval(__is_strongly_connected,&_is_strongly_connected,_is_strongly_connected_s);
define_unary_function_ptr5(at_is_strongly_connected,alias_at_is_strongly_connected,&__is_strongly_connected,0,true)

/* USAGE:   degree_sequence(G)
 *
 * Returns the list of degrees of vertices of graph G (arc directions are
 * ignored).
 */
gen _degree_sequence(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return G.degree_sequence();
}
static const char _degree_sequence_s[]="degree_sequence";
static define_unary_function_eval(__degree_sequence,&_degree_sequence,_degree_sequence_s);
define_unary_function_ptr5(at_degree_sequence,alias_at_degree_sequence,&__degree_sequence,0,true)

/* USAGE:   is_graphic_sequence(L)
 *
 * Returns true iff there exists a graph with degree sequence equal to the list
 * L. The algorithm is based on Erdos-Gallai theorem.
 */
gen _is_graphic_sequence(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    int n=g._VECTptr->size(),k=1;
    for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
        if (!it->is_integer() || !is_positive(*it,contextptr) || !is_strictly_greater(n,*it,contextptr))
            return graphe::FAUX;
    }
    vecteur deg=*_SortD(g,contextptr)._VECTptr;
    if (is_zero(_even(_sum(deg,contextptr),contextptr)))
        return graphe::FAUX;
    gen S(0),M;
    for (const_iterateur it=deg.begin();it!=deg.end();++it,++k) {
        S+=*it;
        M=0;
        for (int i=k;i<n;++i) {
            M+=_min(makesequence(deg[i],k),contextptr);
        }
        if (is_strictly_greater(S,gen(k*(k-1))+M,contextptr))
            return graphe::FAUX;
    }
    return graphe::VRAI;
}
static const char _is_graphic_sequence_s[]="is_graphic_sequence";
static define_unary_function_eval(__is_graphic_sequence,&_is_graphic_sequence,_is_graphic_sequence_s);
define_unary_function_ptr5(at_is_graphic_sequence,alias_at_is_graphic_sequence,&__is_graphic_sequence,0,true)

/* USAGE:   sequence_graph(L)
 *
 * Returns an undirected graph with the degree sequence equal to the list L, as
 * constructed by Havel-Hakimi algorithm.
 */
gen _sequence_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    int n=g._VECTptr->size();
    graphe::ivector deg(n);
    for (const_iterateur it=g._VECTptr->begin();it!=g._VECTptr->end();++it) {
        if (!it->is_integer() || !is_positive(*it,contextptr) || !is_strictly_greater(n,*it,contextptr))
            return gt_err(_GT_ERR_NOT_A_GRAPHIC_SEQUENCE,contextptr);
        deg[it-g._VECTptr->begin()]=it->val;
    }
    graphe G(contextptr);
    if (!G.hakimi(deg))
        return gt_err(_GT_ERR_NOT_A_GRAPHIC_SEQUENCE,contextptr);
    return G.to_gen();
}
static const char _sequence_graph_s[]="sequence_graph";
static define_unary_function_eval(__sequence_graph,&_sequence_graph,_sequence_graph_s);
define_unary_function_ptr5(at_sequence_graph,alias_at_sequence_graph,&__sequence_graph,0,true)

/* USAGE:   girth(G)
 *
 * Returns the girth of undirected and unweighted graph G (i.e. the length of
 * the shortest cycle in G).
 */
gen _girth(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    if (G.is_weighted())
        return gt_err(_GT_ERR_UNWEIGHTED_GRAPH_REQUIRED,contextptr);
    int grth=G.girth();
    return grth<0?graphe::plusinf():gen(grth);
}
static const char _girth_s[]="girth";
static define_unary_function_eval(__girth,&_girth,_girth_s);
define_unary_function_ptr5(at_girth,alias_at_girth,&__girth,0,true)

/* USAGE:   odd_girth(G)
 *
 * Returns the length of the shortest odd cycle in undirected and unweighted
 * graph G.
 */
gen _odd_girth(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    if (G.is_weighted())
        return gt_err(_GT_ERR_UNWEIGHTED_GRAPH_REQUIRED,contextptr);
    int grth=G.girth(true);
    return grth<0?graphe::plusinf():gen(grth);
}
static const char _odd_girth_s[]="odd_girth";
static define_unary_function_eval(__odd_girth,&_odd_girth,_odd_girth_s);
define_unary_function_ptr5(at_odd_girth,alias_at_odd_girth,&__odd_girth,0,true)

/* USAGE:   is_arborescence(G)
 *
 * Returns true iff directed and unweighted graph G is an arborescence.
 */
gen _is_arborescence(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    return graphe::boole(G.is_arborescence());
}
static const char _is_arborescence_s[]="is_arborescence";
static define_unary_function_eval(__is_arborescence,&_is_arborescence,_is_arborescence_s);
define_unary_function_ptr5(at_is_arborescence,alias_at_is_arborescence,&__is_arborescence,0,true)

//
// GENERAL COMMMANDS ***********************************************************
//

/* USAGE:   foldl(op,id,r1,r2,...)
 *
 * Returns the composition of the binary operator or function op, with identity
 * or initial value id onto its arguments r1, r2, ..., associating from the
 * left. For example, given three arguments a, b and c and an initial value id,
 * foldl(op,id,a,b,c) is equivalent to op(op(op(id,a),b),c).
 */
gen _foldl(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT  || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<3)
        return gensizeerr(contextptr);
    gen &op=gv.front();
    gen arg=gv[1];
    for (const_iterateur it=gv.begin()+2;it!=gv.end();++it) {
        arg=symbolic(at_of,makesequence(op,makesequence(arg,*it)));
    }
    return _eval(arg,contextptr);
}
static const char _foldl_s[]="foldl";
static define_unary_function_eval(__foldl,&_foldl,_foldl_s);
define_unary_function_ptr5(at_foldl,alias_at_foldl,&__foldl,0,true)

/* USAGE:   foldr(op,id,r1,r2,...)
 *
 * Returns the composition of the binary operator or function op, with identity
 * or initial value id onto its arguments r1, r2, ..., associating from the
 * right. For example, given three arguments a, b and c and an initial value id,
 * foldl(op,id,a,b,c) is equivalent to op(a,op(b,op(c,id))).
 */
gen _foldr(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT  || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<3)
        return gensizeerr(contextptr);
    gen &op=gv.front();
    gen arg=gv[1];
    for (int i=gv.size();i-->2;) {
        arg=symbolic(at_of,makesequence(op,makesequence(gv[i],arg)));
    }
    return _eval(arg,contextptr);
}
static const char _foldr_s[]="foldr";
static define_unary_function_eval(__foldr,&_foldr,_foldr_s);
define_unary_function_ptr5(at_foldr,alias_at_foldr,&__foldr,0,true)

/* UASGE:   graph_spectrum(G)
 *
 * Returns the graph spectrum of G. The return value is a list of lists with two
 * elements, each containing an eigenvalue and its multiplicity.
 */
gen _graph_spectrum(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    matrice A,res;
    G.adjacency_matrix(A);
    vecteur ev=*_eigenvals(A,contextptr)._VECTptr;
    gen_map ev_map;
    for (const_iterateur it=ev.begin();it!=ev.end();++it) {
        ev_map[*it]+=1;
    }
    for (gen_map::const_iterator it=ev_map.begin();it!=ev_map.end();++it) {
        res.push_back(makevecteur(it->first,it->second));
    }
    return res;
}
static const char _graph_spectrum_s[]="graph_spectrum";
static define_unary_function_eval(__graph_spectrum,&_graph_spectrum,_graph_spectrum_s);
define_unary_function_ptr5(at_graph_spectrum,alias_at_graph_spectrum,&__graph_spectrum,0,true)

/* UASGE:   seidel_spectrum(G)
 *
 * Returns the Seidel spectrum of G. The return value is a list of lists with two
 * elements, each containing an eigenvalue and its multiplicity.
 */
gen _seidel_spectrum(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    int n=G.node_count();
    matrice A,I,J,res;
    G.adjacency_matrix(A);
    I=*_idn(n,contextptr)._VECTptr;
    J=*_matrix(makesequence(n,n,1),contextptr)._VECTptr;
    vecteur ev=*_eigenvals(J-I-A-A,contextptr)._VECTptr;
    gen_map ev_map;
    for (const_iterateur it=ev.begin();it!=ev.end();++it) {
        ev_map[*it]+=1;
    }
    for (gen_map::const_iterator it=ev_map.begin();it!=ev_map.end();++it) {
        res.push_back(makevecteur(it->first,it->second));
    }
    return res;
}
static const char _seidel_spectrum_s[]="seidel_spectrum";
static define_unary_function_eval(__seidel_spectrum,&_seidel_spectrum,_seidel_spectrum_s);
define_unary_function_ptr5(at_seidel_spectrum,alias_at_seidel_spectrum,&__seidel_spectrum,0,true)

/* USAGE:   graph_charpoly(G,[x])
 *
 * Returns the value p(x) of the characteristic polynomial p of graph G. If x is
 * omitted, a list of coefficients of p is returned.
 */
gen _graph_charpoly(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    identificateur x(" x");
    gen val(undef);
    graphe G(contextptr);
    if (g.subtype==_SEQ__VECT) {
        if (g._VECTptr->size()!=2)
            return gensizeerr(contextptr);
        val=g._VECTptr->back();
    }
    if (!G.read_gen(g.subtype==_SEQ__VECT?g._VECTptr->front():g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    matrice A;
    G.adjacency_matrix(A);
    if (is_undef(val))
        return _eval(symbolic(at_charpoly,A),contextptr);
    gen p=_eval(symbolic(at_charpoly,makesequence(A,x)),contextptr);
    return _subs(makesequence(p,x,val),contextptr);
}
static const char _graph_charpoly_s[]="graph_charpoly";
static define_unary_function_eval(__graph_charpoly,&_graph_charpoly,_graph_charpoly_s);
define_unary_function_ptr5(at_graph_charpoly,alias_at_graph_charpoly,&__graph_charpoly,0,true)

/* UASGE:   is_integer_graph(G)
 *
 * Returns true iff the spectrum of graph G consists only of integers.
 */
gen _is_integer_graph(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    matrice A;
    G.adjacency_matrix(A);
    return graphe::boole(is_integer_vecteur(*_eigenvals(A,contextptr)._VECTptr,true));
}
static const char _is_integer_graph_s[]="is_integer_graph";
static define_unary_function_eval(__is_integer_graph,&_is_integer_graph,_is_integer_graph_s);
define_unary_function_ptr5(at_is_integer_graph,alias_at_is_integer_graph,&__is_integer_graph,0,true)

/* USAGE:   spanning_tree(G,[r])
 *
 * Returns a spanning tree of the undirected graph G [with the vertex r as the
 * root node].
 */
gen _spanning_tree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    gen root(undef);
    graphe G(contextptr),T(contextptr);
    if (g.subtype==_SEQ__VECT) {
        if (g._VECTptr->size()!=2)
            return gensizeerr(contextptr);
        root=g._VECTptr->back();
    }
    if (!G.read_gen(g.subtype==_SEQ__VECT?g._VECTptr->front():g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    int i=is_undef(root)?0:G.node_index(root);
    if (i<0)
        return gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    G.spanning_tree(i,T);
    return T.to_gen();
}
static const char _spanning_tree_s[]="spanning_tree";
static define_unary_function_eval(__spanning_tree,&_spanning_tree,_spanning_tree_s);
define_unary_function_ptr5(at_spanning_tree,alias_at_spanning_tree,&__spanning_tree,0,true)

/* USAGE:   number_of_spanning_trees(G)
 *
 * Returns the number of spanning trees in the undirected graph G.
 */
gen _number_of_spanning_trees(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    if (!G.is_connected())
        return 0;
    matrice A;
    G.adjacency_matrix(A);
    vecteur d=G.degree_sequence();
    matrice D=*_diag(d,contextptr)._VECTptr;
    D=D-A;
    D.pop_back();
    D=mtran(D);
    D.pop_back();
    return _det(D,contextptr);
}
static const char _number_of_spanning_trees_s[]="number_of_spanning_trees";
static define_unary_function_eval(__number_of_spanning_trees,&_number_of_spanning_trees,_number_of_spanning_trees_s);
define_unary_function_ptr5(at_number_of_spanning_trees,alias_at_number_of_spanning_trees,&__number_of_spanning_trees,0,true)

/* USAGE:   minimal_spanning_tree(G)
 *
 * Returns the minimal spanning tree of the undirected graph G.
 */
gen _minimal_spanning_tree(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    graphe G(contextptr),T(contextptr);
    if (!G.read_gen(g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (G.is_directed())
        return gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
    if (!G.is_weighted())
        G.spanning_tree(0,T);
    else
        G.minimal_spanning_tree(T);
    return T.to_gen();
}
static const char _minimal_spanning_tree_s[]="minimal_spanning_tree";
static define_unary_function_eval(__minimal_spanning_tree,&_minimal_spanning_tree,_minimal_spanning_tree_s);
define_unary_function_ptr5(at_minimal_spanning_tree,alias_at_minimal_spanning_tree,&__minimal_spanning_tree,0,true)

/* USAGE:   graph_rank(G,[E])
 *
 * Returns the graph rank of G. If optional set E of edges is given, rank of the
 * spanning subgraph of G with edge set E is returned.
 */
gen _graph_rank(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    vecteur E;
    if (g.subtype==_SEQ__VECT) {
        if (g._VECTptr->size()!=2)
            return gensizeerr(contextptr);
        if (g._VECTptr->back().type!=_VECT)
            return gentypeerr(contextptr);
        E=*g._VECTptr->back()._VECTptr;
    }
    graphe G(contextptr),H(contextptr);
    if (!G.read_gen(g.subtype==_SEQ__VECT?g._VECTptr->front():g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (E.empty())
        return G.node_count()-G.connected_components_count();
    else {
        graphe::ipairs ev;
        bool notfound=false;
        if (!G.edges2ipairs(E,ev,notfound))
            return notfound?gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr):gentypeerr(contextptr);
        G.subgraph(ev,H,false);
        H.add_nodes(G.vertices());
        return G.node_count()-H.connected_components_count();
    }
}
static const char _graph_rank_s[]="graph_rank";
static define_unary_function_eval(__graph_rank,&_graph_rank,_graph_rank_s);
define_unary_function_ptr5(at_graph_rank,alias_at_graph_rank,&__graph_rank,0,true)

/* USAGE:   lowest_common_ancestor(T,r,u,v)
 *          lowest_common_ancestor(T,r,L)
 *
 * Returns the lowest common ancestor of the nodes u,v in the tree T with
 * respect to the root node r. If a list L of node pairs is given, common
 * ancestors of all pairs are returned in a list. Tarjan's offline algorithm is
 * used.
 */
gen _lowest_common_ancestor(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()<3)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_tree())
        return gt_err(_GT_ERR_NOT_A_TREE,contextptr);
    int r=G.node_index(gv[1]);
    if (r<0)
        return gt_err(gv[1],_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    if (gv.size()==4) {
        int i=G.node_index(gv[2]),j=G.node_index(gv[3]);
        if (i<0 || j<0)
            return gt_err(i<0?gv[1]:gv[2],_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        return G.node_label(G.lowest_common_ancestor(i,j,r));
    } else if (gv.size()==3) {
        if (!ckmatrix(gv.back()))
            return gentypeerr(contextptr);
        if (gv.back()._VECTptr->front()._VECTptr->size()!=2)
            return gendimerr(contextptr);
        graphe::ipairs p;
        matrice &m=*gv.back()._VECTptr;
        int n=m.size(),i,j;
        for (int k=0;k<n;++k) {
            vecteur &row=*m[k]._VECTptr;
            i=G.node_index(row.front());
            j=G.node_index(row.back());
            if (i<0 || j<0)
                return gt_err(i<0?row.front():row.back(),_GT_ERR_VERTEX_NOT_FOUND,contextptr);
            p.push_back(make_pair(i,j));
        }
        graphe::ivector lca;
        G.lowest_common_ancestors(r,p,lca);
        return G.get_node_labels(lca);
    } else return gensizeerr(contextptr);
}
static const char _lowest_common_ancestor_s[]="lowest_common_ancestor";
static define_unary_function_eval(__lowest_common_ancestor,&_lowest_common_ancestor,_lowest_common_ancestor_s);
define_unary_function_ptr5(at_lowest_common_ancestor,alias_at_lowest_common_ancestor,&__lowest_common_ancestor,0,true)

/* USAGE:   st_ordering(G,s,t)
 *
 * Returns ST numbering for the biconnected graph G with source s and sink (target) t.
 */
gen _st_ordering(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype!=_SEQ__VECT)
        return gentypeerr(contextptr);
    vecteur &gv=*g._VECTptr;
    if (gv.size()!=3)
        return gensizeerr(contextptr);
    graphe G(contextptr);
    if (!G.read_gen(gv.front()))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (!G.is_biconnected())
        return gt_err(_GT_ERR_BICONNECTED_GRAPH_REQUIRED,contextptr);
    int s=G.node_index(gv[1]),t=G.node_index(gv[2]);
    if (s<0 || t<0)
        return gt_err(s<0?gv[1]:gv[2],_GT_ERR_VERTEX_NOT_FOUND,contextptr);
    if (!G.has_edge(s,t))
        return gt_err(makevecteur(gv[1],gv[2]),_GT_ERR_EDGE_NOT_FOUND,contextptr);
    G.compute_st_numbering(s,t);
    return G.get_st_numbering();
}
static const char _st_ordering_s[]="st_ordering";
static define_unary_function_eval(__st_ordering,&_st_ordering,_st_ordering_s);
define_unary_function_ptr5(at_st_ordering,alias_at_st_ordering,&__st_ordering,0,true)

/* USAGE:   greedy_color(G,[p])
 *
 * Returns the list of vertex colors (positive integers) obtained by coloring
 * vertices one at a time, assigning to it the smallest available color. If a
 * permutation p is given, vertices will be colored in the order it specifies.
 */
gen _greedy_color(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe::ivector p,colors;
    if (g.subtype==_SEQ__VECT) {
        if (g._VECTptr->size()!=2)
            return gensizeerr(contextptr);
        if (is_zero(_is_permu(g._VECTptr->back(),contextptr)))
            return gentypeerr(contextptr);
        p=vecteur_2_vector_int(*g._VECTptr->back()._VECTptr);
        int offset=array_start(contextptr);
        for (graphe::ivector::iterator it=p.begin();it!=p.end();++it) {
            *it-=offset;
        }
    }
    graphe G(contextptr);
    if (!G.read_gen(g.subtype==_SEQ__VECT?g._VECTptr->front():g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    if (p.empty()) { // construct the identity permutation
        p.resize(G.node_count());
        for (graphe::ivector::iterator it=p.begin();it!=p.end();++it) {
            *it=it-p.begin();
        }
    } else if (G.node_count()!=int(p.size()))
        return gensizeerr(contextptr);
    G.greedy_vertex_coloring(p);
    G.get_vertex_colors(colors);
    return vector_int_2_vecteur(colors);
}
static const char _greedy_color_s[]="greedy_color";
static define_unary_function_eval(__greedy_color,&_greedy_color,_greedy_color_s);
define_unary_function_ptr5(at_greedy_color,alias_at_greedy_color,&__greedy_color,0,true)

/* USAGE:   is_bipartite(G,[P])
 *
 * Returns true iff the graph G is bipartite [and assign to P the list of partitions
 * represented as lists of vertices].
 */
gen _is_bipartite(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    gen P=undef;
    if (g.subtype==_SEQ__VECT) {
        if (g._VECTptr->size()!=2)
            return gensizeerr(contextptr);
        if ((P=g._VECTptr->back()).type!=_IDNT)
            return gentypeerr(contextptr);
    }
    graphe G(contextptr),U(contextptr);
    if (!G.read_gen(g.subtype==_SEQ__VECT?g._VECTptr->front():g))
        return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
    G.underlying(U);
    graphe::ivector V1,V2;
    if (!U.is_bipartite(V1,V2))
        return G.boole(false);
    if (!is_undef(P)) {
        _eval(symbolic(at_sto,makevecteur(_sort(G.get_node_labels(V1),contextptr),
                                          _sort(G.get_node_labels(V2),contextptr)),
                       P),contextptr);
    }
    return G.boole(true);
}
static const char _is_bipartite_s[]="is_bipartite";
static define_unary_function_eval(__is_bipartite,&_is_bipartite,_is_bipartite_s);
define_unary_function_ptr5(at_is_bipartite,alias_at_is_bipartite,&__is_bipartite,0,true)

/* USAGE:   plane_dual(G)
 *          plane_dual(F)
 *
 * Returns the plane dual of a biconnected planar graph G or constructed from
 * the list of faces F.
 */
gen _plane_dual(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG && g.subtype==-1) return g;
    if (g.type!=_VECT || g.subtype==_SEQ__VECT)
        return gentypeerr(contextptr);
    graphe::ivectors faces;
    graphe D(contextptr);
    if (g.subtype==_GRAPH__VECT) {
        graphe G(contextptr);
        if (!G.read_gen(g))
            return gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        if (!G.is_biconnected())
            return gt_err(_GT_ERR_BICONNECTED_GRAPH_REQUIRED,contextptr);
        if (!G.demoucron(faces))
            return gt_err(_GT_ERR_NOT_PLANAR,contextptr);
    } else {
        gen_map m;
        int k=0;
        vecteur &gv=*g._VECTptr;
        for (const_iterateur it=gv.begin();it!=gv.end();++it) {
            if (it->type!=_VECT)
                return gentypeerr(contextptr);
            graphe::ivector face;
            for (const_iterateur jt=it->_VECTptr->begin();jt!=it->_VECTptr->end();++jt) {
                if (is_zero(m[*jt]))
                    m[*jt]=++k;
                face.push_back(m[*jt].val-1);
            }
            faces.push_back(face);
        }
    }
    D.make_plane_dual(faces);
    return D.to_gen();
}
static const char _plane_dual_s[]="plane_dual";
static define_unary_function_eval(__plane_dual,&_plane_dual,_plane_dual_s);
define_unary_function_ptr5(at_plane_dual,alias_at_plane_dual,&__plane_dual,0,true)

#ifndef NO_NAMESPACE_GIAC
}
#endif // ndef NO_NAMESPACE_GIAC
