/*
 * graphe.cc
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
#include <giac/giac.h>
#include <sstream>
#include <ctype.h>
#include <math.h>

using namespace std;

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

#define PLASTIC_NUMBER 1.32471795724474602596090885
#define _GRAPH__VECT 30

const gen graphe::VRAI=gen(1).change_subtype(_INT_BOOLEAN);
const gen graphe::FAUX=gen(0).change_subtype(_INT_BOOLEAN);
default_random_engine graphe::random_generator;
normal_distribution<double> graphe::normal_distribution(0.0,1.0);
uniform_real_distribution<double> graphe::uniform_distribution(0.0,1.0);

/* returns vector of all nonnegative integers smaller than n and not in v, which must be sorted */
graphe::ivector graphe::range_complement(const ivector &v,int n) {
    int k=0,l=0,m=v.size();
    if (n==m)
        return ivector(0);
    assert(n>m);
    ivector res(n-m);
    for (int i=0;i<n;++i) {
        if (v[k]!=i)
            res[l++]=i;
        else ++k;
    }
    return res;
}

/* return a copy of attr */
graphe::attrib graphe::copy_attributes(const attrib &attr) {
    attrib copy;
    for (attrib_iter it=attr.begin();it!=attr.end();++it) {
        int key=it->first;
        gen val(it->second);
        copy.insert(make_pair(key,val));
    }
    return copy;
}

/* returns true iff g is a graph and writes the basic info to 'display_str' */
bool is_graphe(const gen &g,string &disp_out,GIAC_CONTEXT) {
    if (g.type!=_VECT || g.subtype!=_GRAPH__VECT)
        return false;
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr))
        return false;
    int nv=G.node_count(),ne=G.edge_count();
    string nvert=to_string(nv);
    string nedg=to_string(ne);
    string dir_spec=G.is_directed()?"directed":"undirected";
    string weight_spec=G.is_weighted()?"weighted":"unweighted";
    nvert=nvert+(nv==1?" vertex":" vertices");
    nedg=nedg+(G.is_directed()?(ne==1?" arc":" arcs"):(ne==1?" edge":" edges"));
    string name=G.name();
    if (!name.empty())
        disp_out=name+": ";
    disp_out=disp_out+"an "+dir_spec+" "+weight_spec+" graph with "+nvert+" and "+nedg;
    return true;
}

/* fill the vecteur V with first n integers (0- or 1- based, depending on the mode) */
void graphe::make_default_vertex_labels(vecteur &V,int n,int n0,GIAC_CONTEXT) {
    int ofs=array_start(contextptr);
    V.resize(n);
    for (int i=0;i<n;++i) {
        V[i]=i+ofs+n0;
    }
}

/* create identifier */
gen graphe::make_idnt(const char* name,int index,bool intern) {
    stringstream ss;
    if (intern)
        ss << " ";
    ss << string(name);
    if (index>=0)
        ss << index;
    return identificateur(ss.str().c_str());
}

/* convert string to gen */
gen graphe::str2gen(const string &str,bool isstring) {
    stringstream ss;
    gen g;
    if (isstring) ss << "\"";
    ss << str;
    if (isstring) ss << "\"";
    ss >> g;
    return g;
}

/* convert gen string to std::string (no quotes) */
string graphe::genstring2str(const gen &g) {
    assert(g.type==_STRNG);
    int len=_size(g,context0).val;
    return string(g._STRNGptr->begin(),g._STRNGptr->begin()+len);
}

/* convert gen to string */
string graphe::gen2str(const gen &g) {
    stringstream ss;
    ss << g;
    return ss.str();
}

/* graphe default constructor */
graphe::graphe() {
    set_graph_attribute(_GT_ATTRIB_DIRECTED,FAUX);
    set_graph_attribute(_GT_ATTRIB_WEIGHTED,FAUX);
}

/* graphe constructor, create a copy of G */
graphe::graphe(const graphe &G) {
    set_graph_attribute(_GT_ATTRIB_DIRECTED,boole(G.is_directed()));
    set_graph_attribute(_GT_ATTRIB_WEIGHTED,boole(G.is_weighted()));
    G.copy(*this);
}

/* export this graph as a Giac gen object */
gen graphe::to_gen() const {
    vecteur gv;
    gen_map attr;
    attrib2genmap(attributes,attr);
    gv.push_back(attr);
    vecteur uattr_ids;
    for (vector<string>::const_iterator it=user_tags.begin();it!=user_tags.end();++it) {
        uattr_ids.push_back(str2gen(*it,true));
    }
    gv.push_back(uattr_ids);
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        attrib2genmap(it->attributes,attr);
        vecteur v=makevecteur(it->symbol,attr,vecteur(it->neighbors.size()));
        int j=0;
        for (neighbor_iter jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            attrib2genmap(jt->second,attr);
            v.back()._VECTptr->at(j++)=makevecteur(jt->first,attr);
        }
        gv.push_back(v);
    }
    return change_subtype(gv,_GRAPH__VECT);
}

/* return index associated with the specified attribute tag */
int graphe::tag2index(const string &tag) {
    if (tag=="directed")
        return _GT_ATTRIB_DIRECTED;
    if (tag=="weighted")
        return _GT_ATTRIB_WEIGHTED;
    if (tag=="weight")
        return _GT_ATTRIB_WEIGHT;
    if (tag=="color")
        return _GT_ATTRIB_COLOR;
    if (tag=="pos")
        return _GT_ATTRIB_POSITION;
    return register_user_tag(tag);
}

/* return attribute tag associated with the specified index */
string graphe::index2tag(int index) const {
    int len;
    switch (index) {
    case _GT_ATTRIB_DIRECTED:
        return "directed";
    case _GT_ATTRIB_WEIGHTED:
        return "weighted";
    case _GT_ATTRIB_COLOR:
        return "color";
    case _GT_ATTRIB_WEIGHT:
        return "weight";
    case _GT_ATTRIB_POSITION:
        return "pos";
    case _GT_ATTRIB_USER:
    default:
        len=index-_GT_ATTRIB_USER;
        assert(int(user_tags.size())>len);
        return user_tags[len];
    }
}

/* register custom user attribute tag */
int graphe::register_user_tag(const string &tag) {
    int i=_GT_ATTRIB_USER;
    vector<string>::iterator it=user_tags.begin();
    for (;it!=user_tags.end();++it) {
        if (*it==tag)
            return i;
        ++i;
    }
    user_tags.push_back(tag);
    return i;
}

/* register custom user tags */
void graphe::register_user_tags(const vector<string> &tags) {
    for (vector<string>::const_iterator it=tags.begin();it!=tags.end();++it) {
        register_user_tag(*it);
    }
}

/* make unique representation of an edge in undirected case */
graphe::ipair graphe::make_edge(int i, int j) const {
    int k=is_directed()?i:(i<=j?i:j),l=is_directed()?j:(i<=j?j:i);
    return make_pair(k,l);
}

/* overloaded make_edge function */
graphe::ipair graphe::make_edge(const vecteur &v) const {
    assert(v.size()==2);
    int i=node_index(v.front()),j=node_index(v.back());
    return make_edge(i,j);
}

/* return total number of edges/arcs */
int graphe::edge_count() const {
    int count=0;
    for(node_iter it=nodes.begin();it!=nodes.end();++it) {
        count+=it->neighbors.size();
    }
    return count;
}

/* return number of arcs going in the vertex with the specified index (when directed) */
int graphe::in_degree(int index) const {
    assert(index>=0 && index<node_count());
    int i=0,count=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (i==index)
            continue;
        if (it->neighbors.find(index)!=it->neighbors.end())
            ++count;
        ++i;
    }
    return count;
}

/* return number of arcs going out of vertex with the specified index (when directed) */
int graphe::out_degree(int index) const {
    assert(index>=0 && index<node_count());
    return nodes[index].neighbors.size();
}

/* return degree of vertex with the specified index */
int graphe::degree(int index) const {
    return in_degree(index)+out_degree(index);
}

/* return adjacency matrix of this graph */
matrice graphe::adjacency_matrix() const {
    int n=node_count(),i=0,j;
    matrice m=*_matrix(makesequence(n,n,0),context0)._VECTptr;
    for (vector<vertex>::const_iterator it=nodes.begin();it!=nodes.end();++it) {
        for (map<int,attrib>::const_iterator jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            j=jt->first;
            m[i]._VECTptr->at(j)=gen(1);
            if (!is_directed())
                m[j]._VECTptr->at(i)=gen(1);
        }
        ++i;
    }
    return m;
}

/* return incidence matrix of this graph */
matrice graphe::incidence_matrix() const {
    int nr=node_count(),nc=edge_count(),i=0,j,k=0;
    matrice m=*_matrix(makesequence(nr,nc,0),context0)._VECTptr;
    for (vector<vertex>::const_iterator it=nodes.begin();it!=nodes.end();++it) {
        for (map<int,attrib>::const_iterator jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            j=jt->first;
            m[i]._VECTptr->at(k)=gen(is_directed()?-1:1);
            m[j]._VECTptr->at(k)=gen(1);
            ++k;
        }
        ++i;
    }
    return m;
}

/* return const reference to attributes of edge {i,j} or arc [i,j] */
const graphe::attrib &graphe::edge_attributes(int i, int j) const {
    ipair edge=make_edge(i,j);
    assert (edge.first>=0 && edge.first<node_count());
    const map<int,attrib> &ngh=nodes[edge.first].neighbors;
    neighbor_iter it=ngh.find(edge.second);
    assert (it!=ngh.end());
    return it->second;
}

/* return modifiable reference to attributes of edge {i,j} or arc [i,j] */
graphe::attrib &graphe::edge_attributes(int i, int j) {
    ipair edge=make_edge(i,j);
    assert (edge.first>=0 && edge.first<node_count());
    map<int,attrib> &ngh=nodes[edge.first].neighbors;
    map<int,attrib>::iterator it=ngh.find(edge.second);
    assert (it!=ngh.end());
    return it->second;
}

/* return weight associated with edge {i,j} or arc [i,j] */
gen graphe::weight(int i, int j) const {
    const attrib &attr=edge_attributes(i,j);
    attrib_iter it=attr.find(_GT_ATTRIB_WEIGHT);
    if (it==attr.end())
        return undef;
    return it->second;
}

/* return weight matrix */
matrice graphe::weight_matrix() const {
    assert(is_weighted());
    int n=node_count(),i=0;
    gen w;
    matrice m=*_matrix(makesequence(n,n,0),context0)._VECTptr;
    for (vector<vertex>::const_iterator it=nodes.begin();it!=nodes.end();++it) {
        for (map<int,attrib>::const_iterator jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            w=jt->second.find(_GT_ATTRIB_WEIGHT)->second;
            m[i]._VECTptr->at(jt->first)=w;
            if (!is_directed())
                m[jt->first]._VECTptr->at(i)=w;
        }
        ++i;
    }
    return m;
}

/* return list of vertices */
vecteur graphe::vertices() const {
    vecteur V(node_count());
    int i=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        V[i++]=it->symbol;
    }
    return V;
}

/* return list of edges/arcs */
vecteur graphe::edges(bool include_weights) const {
    vecteur edge(2),ret;
    int i=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        edge[0]=it->symbol;
        for (neighbor_iter jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            edge[1]=nodes[jt->first].symbol;
            ret.push_back(include_weights?makevecteur(edge,weight(i,jt->first)):edge);
        }
        ++i;
    }
    return ret;
}

/* write attributes to dot file */
void graphe::write_attrib(ofstream &dotfile,const attrib &attr) const {
    dotfile << "[";
    for (attrib_iter it=attr.begin();it!=attr.end();++it) {
        if (it!=attr.begin())
            dotfile << ",";
        dotfile << index2tag(it->first) << "=" << it->second;
    }
    dotfile << "]";
}

/* export this graph to dot file */
bool graphe::write_dot(const string &filename) const {
    ofstream dotfile;
    dotfile.open(filename);
    if (!dotfile.is_open())
        return false;
    ivector u,v;
    string indent("  "),edgeop(is_directed()?" -> ":" -- ");
    dotfile << (is_directed()?"digraph ":"graph ");
    if (graph_name.empty())
        dotfile << "{" << endl;
    else
        dotfile << graph_name << " {" << endl;
    if (!attributes.empty()) {
        dotfile << indent << "graph ";
        write_attrib(dotfile,attributes);
        dotfile << endl;
    }
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (!it->attributes.empty()) {
            dotfile << indent << it->symbol << " ";
            write_attrib(dotfile,it->attributes);
            dotfile << ";" << endl;
        }
        u.clear();
        v.clear();
        for (neighbor_iter jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            if (jt->second.empty())
                u.push_back(jt->first);
            else
                v.push_back(jt->first);
        }
        if (!u.empty()) {
            dotfile << indent << it->symbol << edgeop << "{ ";
            for (ivector::const_iterator kt=u.begin();kt!=u.end();++kt) {
                dotfile << nodes[*kt].symbol << " ";
            }
            dotfile << "};" << endl;
        }
        for (ivector::const_iterator kt=v.begin();kt!=v.end();++kt) {
            dotfile << indent << it->symbol << edgeop << nodes[*kt].symbol << " ";
            write_attrib(dotfile,it->neighbors.find(*kt)->second);
            dotfile << ";" << endl;
        }
    }
    dotfile << "}" << endl;
    dotfile.close();
    return true;
}

static int dot_comment_type;
static int dot_subgraph_level;
static bool dot_reading_attributes;
static bool dot_reading_value;
static int dot_newline_count;
static int dot_token_type;

/* returns true iff last read token is an identifier, number or string */
bool dot_token_is_id() {
    return dot_token_type==_GT_DOT_TOKEN_TYPE_IDENTIFIER ||
            dot_token_type==_GT_DOT_TOKEN_TYPE_NUMBER ||
            dot_token_type==_GT_DOT_TOKEN_TYPE_STRING;
}

/* return true iff char c is one of dot format delimiters */
bool dot_is_id_delim(const char &c) {
    return c=='{' || c=='}' || c=='[' || c==']' || c=='-' ||
            c==',' || c==';' || c=='=' || c=='/' || isspace(c);
}

/* read next token in dot file 'dotfile' */
int dot_read_token(ifstream &dotfile,string &token) {
    char c=0,pc,nc;
    bool last=false;
    dot_token_type=0;
    token=string("");
    while (true) {
        if ((dot_token_type==_GT_DOT_TOKEN_TYPE_IDENTIFIER || dot_token_type==_GT_DOT_TOKEN_TYPE_NUMBER)
                && dot_is_id_delim(dotfile.peek()))
            break;
        pc=c;
        if (!dotfile.get(c))
            break;
        if (c=='\n')
            ++dot_newline_count;
        if (dot_token_type==_GT_DOT_TOKEN_TYPE_IDENTIFIER) {
            if (isalnum(c) || c=='_')
                token.push_back(c);
            else return 0;
            continue;
        }
        if (dot_token_type==_GT_DOT_TOKEN_TYPE_NUMBER) {
            if (isdigit(c) || c=='.')
                token.push_back(c);
            else return 0;
            continue;
        }
        nc=dotfile.peek();
        if (nc==EOF)
            last=true;
        if (dot_comment_type!=0) {
            if (((c=='\n' || c=='\r') && dot_comment_type==1) ||
                    ((c=='/' && pc=='*') && dot_comment_type==2)) {
                if (token.empty()) {
                    dot_comment_type=0;
                    continue;
                }
                break;
            }
            continue;
        }
        if (c=='"') {
            if (dot_token_type==_GT_DOT_TOKEN_TYPE_STRING) {
                if (pc=='\\')  {
                    token.pop_back();
                    token.push_back(c);
                    continue;
                }
                break;
            }
            dot_token_type=_GT_DOT_TOKEN_TYPE_STRING;
            continue;
        }
        if (dot_token_type==_GT_DOT_TOKEN_TYPE_STRING) {
            if (last)
                return 0;
            token.push_back(c);
            continue;
        }
        if (isblank(c) || (dot_subgraph_level>0 && c==';') ||
                (dot_reading_attributes && c==',') || (!dot_reading_attributes && isspace(c))) {
            if (isspace(c))
                continue;
            token=string(1,c);
            dot_token_type=_GT_DOT_TOKEN_TYPE_DELIMITER;
            return 1;
        }
        if (c=='{' || c=='}') {
            dot_subgraph_level+=c=='{'?1:-1;
            token=string(1,c);
            dot_token_type=_GT_DOT_TOKEN_TYPE_DELIMITER;
            return 1;
        }
        if (c=='[' || c==']') {
            dot_reading_attributes=c=='[';
            token=string(1,c);
            dot_token_type=_GT_DOT_TOKEN_TYPE_DELIMITER;
            return 1;
        }
        if (c=='=') {
            dot_reading_value=true;
            token=string(1,c);
            dot_token_type=_GT_DOT_TOKEN_TYPE_OPERATOR;
            return 1;
        }
        if (c=='#' && (pc=='\n' || pc=='\r')) {
            dot_comment_type=1;
            continue;
        }
        if (c=='/') {
            if (last)
                return 0;
            if (nc=='/')
                dot_comment_type=1;
            if (nc=='*')
                dot_comment_type=2;
            return 0;
        }
        if (token.empty() && (isalpha(c) || c=='_')) {
            dot_token_type=_GT_DOT_TOKEN_TYPE_IDENTIFIER;
            token.push_back(c);
            continue;
        }
        if (token.empty() && (isdigit(c) || (c=='-' && isdigit(nc)))) {
            dot_token_type=_GT_DOT_TOKEN_TYPE_NUMBER;
            token.push_back(c);
            continue;
        }
        if ((c=='-' || c=='>') && pc=='-') {
            dot_token_type=_GT_DOT_TOKEN_TYPE_OPERATOR;
            token=string("-")+c;
            return 1;
        }
    }
    if (dot_token_is_id())
        dot_reading_value=false;
    return dot_token_type==0?-1:1;
}

/* insert attribute key=val to attr [and overwrite existing value] */
bool graphe::insert_attribute(attrib &attr,int key,const gen &val,bool overwrite) {
    pair<attrib::iterator,bool> res;
    if ((res=attr.insert(make_pair(key,val))).second==false && overwrite)
        (res.first)->second=val;
    return res.second;
}

/* remove attribute with key 'key' from attr */
bool graphe::remove_attribute(attrib &attr, int key) {
    attrib_iter it=attr.find(key);
    if (it==attr.end())
        return false;
    attr.erase(it);
    return true;
}

/* parse attributes from dot file */
bool graphe::dot_parse_attributes(ifstream &dotfile,attrib &attr) {
    string token;
    int key;
    while(true) {
        if (dot_read_token(dotfile,token)!=1)
            return false;
        if (token=="]")
            break;
        if (token==";" || token==",")
            continue;
        if (!dot_token_is_id())
            return false;
        key=tag2index(token);
        if (key==-1 || dot_read_token(dotfile,token)!=1 || token!="=" ||
                dot_read_token(dotfile,token)!=1 || dot_reading_value || !dot_token_is_id())
            return false;
        insert_attribute(attr,key,str2gen(token,dot_token_type==_GT_DOT_TOKEN_TYPE_STRING));
    }
    return true;
}

/* initialize graph from dot file */
bool graphe::read_dot(const string &filename) {
    ifstream dotfile;
    dotfile.open(filename);
    if (!dotfile.is_open())
        return false;
    dot_subgraph_level=0;
    dot_reading_attributes=false;
    dot_reading_value=false;
    dot_comment_type=0;
    dot_newline_count=0;
    string token;
    int res,gtype,subgraph_count=0,index;
    bool strict=false,error_raised=false,has_root_graph=false;
    vector<dotgraph> subgraphs;
    while (true) {
        res=dot_read_token(dotfile,token);
        if (res!=1) { error_raised=(bool)(res+1); break; }
        switch (dot_token_type) {
        case _GT_DOT_TOKEN_TYPE_IDENTIFIER:
            if (token=="strict") {
                strict=true;
                continue;
            } else if (token=="graph" || token=="digraph" || token=="subgraph") {
                if (token=="digraph") {
                    if (dot_subgraph_level>0) { error_raised=true; break; }
                    set_directed(true);
                    gtype=1;
                } else if (token=="subgraph") {
                    gtype=2;
                } else gtype=0;
                if (gtype!=2 && strict)
                    strict=false;
                if (dot_read_token(dotfile,token)!=1) { error_raised=true; break; }
                if (dot_token_type!=_GT_DOT_TOKEN_TYPE_DELIMITER) {
                    if (!dot_token_is_id()) { error_raised=true; break; }
                    if (gtype!=2)
                        graph_name=token;
                    if (dot_read_token(dotfile,token)!=1 ||
                            dot_token_type!=_GT_DOT_TOKEN_TYPE_DELIMITER) { error_raised=true; break; }
                } else if (token=="[") {
                    if (gtype!=0 || !dot_parse_attributes(dotfile,attributes))
                        error_raised=true;
                    break;
                }
                if ((gtype!=2 && has_root_graph) || token!="{")
                    error_raised=true;
                subgraphs.push_back(dotgraph(subgraph_count++));
                if (gtype!=2)
                    has_root_graph=true;
                break;
            } else if (token=="node" || token=="edge") {
                if (dot_read_token(dotfile,token)!=1 || dot_token_type!=_GT_DOT_TOKEN_TYPE_DELIMITER ||
                        token!="[" || !dot_parse_attributes(dotfile,token=="node"?
                                                            subgraphs.back().vertex_attributes:
                                                            subgraphs.back().edge_attributes))
                    error_raised=true;
                break;
            }
        case _GT_DOT_TOKEN_TYPE_NUMBER:
        case _GT_DOT_TOKEN_TYPE_STRING:
            index=add_node(str2gen(token,true));
            nodes[index].subgraph=subgraphs.back().index;
            subgraphs.back().set(index+1);
            break;
        case _GT_DOT_TOKEN_TYPE_DELIMITER:
            if (token==";") {
                if (subgraphs.back().chain_empty())
                    continue;
                if (!subgraphs.back().chain_completed()) { error_raised=true; break; }
                ivector &ch=subgraphs.back().chain;
                attrib &attr=subgraphs.back().chain_attributes;
                if (subgraphs.back().pos==0) {
                    if (ch.front()<=0) { error_raised=true; break; }
                    nodes[ch.front()-1].attributes=subgraphs.back().chain_attributes;
                }
                int lh,rh;
                for (int i=0;i<=subgraphs.back().pos-1;++i) {
                    lh=ch[i];
                    rh=ch[i+1];
                    if (lh==0 || rh==0) { error_raised=true; break; }
                    if (lh>0 && rh>0)
                        add_edge(lh-1,rh-1,attr);
                    else if (lh<0 && rh<0) {
                        int j=0,k=0;
                        for (node_iter it=nodes.begin();it!=nodes.end();++it) {
                            if (it->subgraph==-lh) {
                                for (node_iter jt=nodes.begin();jt!=nodes.end();++jt) {
                                    if (jt->subgraph==-rh)
                                        add_edge(j,k,attr);
                                    ++k;
                                }
                            }
                            ++j;
                        }
                    } else {
                        int j=0;
                        for (node_iter it=nodes.begin();it!=nodes.end();++it) {
                            if (it->subgraph==(lh>0?-rh:-lh))
                                add_edge(lh>0?lh-1:j,lh>0?j:rh-1,attr);
                            ++j;
                        }
                    }
                }
                subgraphs.back().clear_chain();
            } else if (token=="[") {
                attrib &attr=subgraphs.back().chain_attributes;
                if (!attr.empty() || !dot_parse_attributes(dotfile,attr))
                    error_raised=true;
            } else if (token=="{") {
                if (dot_subgraph_level<2) { error_raised=true; break; }
                subgraphs.push_back(dotgraph(subgraph_count++));
            } else if (token=="}") {
                if (subgraphs.empty()) { error_raised=true; break; }
                index=subgraphs.back().index;
                subgraphs.pop_back();
                if (!subgraphs.empty())
                    subgraphs.back().set(-index);
            }
            break;
        case _GT_DOT_TOKEN_TYPE_OPERATOR:
            if (dot_subgraph_level<1) { error_raised=true; break; }
            if (token=="--" || token=="->") {
                if ((is_directed() && token=="--") || (!is_directed() && token=="->")) { error_raised=true; break; }
                subgraphs.back().incr();
            }
            break;
        default:
            cout << "Unknown token: " << token << endl;
            break;
        }
        if (error_raised || strict)
            break;
    }
    dotfile.close();
    if (dot_subgraph_level!=0 || strict || dot_reading_value || dot_reading_attributes)
        error_raised=true;
    return !error_raised;
}

/* assign weights from matrix m to edges/arcs of this graph */
void graphe::make_weighted(const matrice &m) {
    assert (is_squarematrix(m) && int(m.size())==node_count());
    int i=0;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        for (map<int,attrib>::iterator jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            insert_attribute(jt->second,_GT_ATTRIB_WEIGHT,m[i][jt->first]);
        }
        ++i;
    }
}

/* convert every edge to pair of arcs */
void graphe::make_directed() {
    set_directed(true);
    int i=0;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        for (map<int,attrib>::iterator jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            add_edge(jt->first,i,jt->second);
        }
        ++i;
    }
}

/* make this graph unweighted */
void graphe::strip_weights() {
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        for (map<int,attrib>::iterator nt=it->neighbors.begin();nt!=it->neighbors.end();++nt) {
            remove_attribute(nt->second,_GT_ATTRIB_WEIGHT);
        }
    }
    set_graph_attribute(_GT_ATTRIB_WEIGHTED,FAUX);
}

/* convert arcs to edges (remove duplicates) and strip edge weights */
void graphe::make_underlying() {
    set_directed(false);
    int i=0;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        for (int j=0;j<i;++j) {
            neighbor_iter nt=it->neighbors.find(j);
            if (nt!=it->neighbors.end())
                it->neighbors.erase(nt);
        }
        ++i;
    }
    strip_weights();
}

/* read contents of gen_map and write it to attrib */
bool graphe::genmap2attrib(const gen_map &m,attrib &attr) {
    attr.clear();
    for (gen_map::const_iterator it=m.begin();it!=m.end();++it) {
        if (!it->first.is_integer())
            return false;
        attr.insert(make_pair(it->first.val,it->second));
    }
    return true;
}

/* convert attrib to gen_map */
void graphe::attrib2genmap(const attrib &attr, gen_map &m) {
    m.clear();
    for (attrib_iter it=attr.begin();it!=attr.end();++it) {
        m[it->first]=it->second;
    }
}

/* initialize graph from Giac gen object */
bool graphe::read_gen(const vecteur &v,GIAC_CONTEXT) {
    if (v.empty() || v.front().type!=_MAP || !genmap2attrib(*v.front()._MAPptr,attributes))
        return false;
    if (int(v.size())<2 || v[1].type!=_VECT)
        return false;
    for (const_iterateur it=v[1]._VECTptr->begin();it!=v[1]._VECTptr->end();++it) {
        if (it->type!=_STRNG)
            return false;
        register_user_tag(genstring2str(*it));
    }
    int index;
    for (const_iterateur it=v.begin()+2;it!=v.end();++it) {
        if (it->type!=_VECT || int(it->_VECTptr->size())<3)
            return false;
        index=add_node(it->_VECTptr->front());
        vertex &vt=nodes[index];
        if (it->_VECTptr->at(1).type!=_MAP || it->_VECTptr->at(2).type!=_VECT)
            return false;
        genmap2attrib(*it->_VECTptr->at(1)._MAPptr,vt.attributes);
        const_iterateur jt=it->_VECTptr->at(2)._VECTptr->begin();
        for (;jt!=it->_VECTptr->at(2)._VECTptr->end();++jt) {
            if (jt->type!=_VECT || !jt->_VECTptr->front().is_integer() || jt->_VECTptr->at(1).type!=_MAP)
                return false;
            attrib attr;
            genmap2attrib(*jt->_VECTptr->at(1)._MAPptr,attr);
            vt.neighbors.insert(make_pair(jt->_VECTptr->front().val,attr));
        }
    }
    return true;
}

/* make a copy of this graph and store it in G (initialized with 'graphe(this)') */
void graphe::copy(graphe &G) const {
    G.set_name(graph_name);
    G.register_user_tags(user_tags);
    G.set_graph_attributes(attributes);
    for (vector<vertex>::const_iterator it=nodes.begin();it!=nodes.end();++it) {
        G.add_node(it->symbol,it->attributes);
    }
    int i=0;
    for (vector<vertex>::const_iterator it=nodes.begin();it!=nodes.end();++it) {
        for (map<int,attrib>::const_iterator jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            G.add_edge(i,jt->first,jt->second);
        }
        ++i;
    }
}

/* returns true iff graph has edge {i,j}, this function is fast */
bool graphe::has_edge(int i,int j) const {
    if (i<0 || i>=node_count() || j<0 || j>=node_count())
        return false;
    ipair edge=make_edge(i,j);
    const map<int,attrib> &attr=nodes[edge.first].neighbors;
    return attr.find(edge.second)!=attr.end();
}

/* add edge {i,j} or arc [i,j], depending on the type (undirected or directed) */
bool graphe::add_edge(int i, int j, const gen &w) {
    if (i<0 || i>=node_count() || j<0 || j>=node_count() || has_edge(i,j))
        return false;
    ipair edge=make_edge(i,j);
    if (!nodes[edge.first].neighbors.insert(make_pair(edge.second,attrib())).second)
        return false;
    if (is_weighted())
        set_edge_attribute(i,j,_GT_ATTRIB_WEIGHT,w);
    return true;
}

/* add edge {i,j} or arc [i,j] with attributes */
bool graphe::add_edge(int i, int j,const attrib &attr) {
    if (i<0 || i>=node_count() || j<0 || j>=node_count() || has_edge(i,j))
        return false;
    ipair edge=make_edge(i,j);
    attrib attr_copy=copy_attributes(attr);
    return nodes[edge.first].neighbors.insert(make_pair(edge.second,attr_copy)).second;
}

/* add edge {v,w} or arc [v,w], adding vertices v and/or w if necessary */
graphe::ipair graphe::add_edge(const gen &v,const gen &w,const gen &weight) {
    int i=add_node(v),j=add_node(w);
    add_edge(i,j,weight);
    return make_edge(i,j);
}

/* remove edge {v,w} or arc [v,w] */
bool graphe::remove_edge(int i, int j) {
    if (!has_edge(i,j))
        return false;
    ipair edge=make_edge(i,j);
    map<int,attrib>::iterator it;
    map<int,attrib> &ngh=nodes[edge.first].neighbors;
    it=ngh.find(edge.second);
    ngh.erase(it);
    return true;
}

/* add vertex v to graph */
int graphe::add_node(const gen &v) {
    int index=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->symbol==v)
            return index;
        ++index;
    }
    nodes.push_back(vertex(v));
    return node_count()-1;
}

/* add vertices from list v to graph */
void graphe::add_nodes(const vecteur &v) {
    for (const_iterateur it=v.begin();it!=v.end();++it) {
        add_node(*it);
    }
}

/* remove the i-th node */
bool graphe::remove_node(int i) {
    if (i<0 || i>=node_count())
        return false;
    ivector adj=adjacent_nodes(i);
    for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
        remove_edge(i,*it);
        if (is_directed())
            remove_edge(*it,i);
    }
    remove_isolated_node(i);
    return true;
}

/* remove node v */
bool graphe::remove_node(const gen &v) {
    int i=node_index(v);
    if (i==-1)
        return false;
    return remove_node(i);
}

/* remove all nodes from list v */
void graphe::remove_nodes(const vecteur &v) {
    ivector I(v.size());
    int i=0;
    for (const_iterateur it=v.begin();it!=v.end();++it) {
        I[i++]=node_index(*it);
    }
    ivector isolated_nodes;
    for (ivector::const_iterator it=I.begin();it!=I.end();++it) {
        i=*it;
        if (i<0)
            continue;
        ivector adj=adjacent_nodes(i);
        for (ivector::const_iterator jt=adj.begin();jt!=adj.end();++jt) {
            remove_edge(i,*jt);
            if (is_directed())
                remove_edge(*jt,i);
        }
        isolated_nodes.push_back(i);
    }
    sort(isolated_nodes.begin(),isolated_nodes.end());
    for (i=int(isolated_nodes.size())-1;i>=0;--i) {
        remove_isolated_node(isolated_nodes[i]);
    }
}

/* return index of vertex v */
int graphe::node_index(const gen &v) const {
    int i=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->symbol==v)
            return i;
        ++i;
    }
    return -1;
}

/* make cycle graph with vertices from V */
void graphe::make_cycle(const vecteur &v) {
    int n=v.size();
    add_nodes(v);
    for (int i=0;i<n;++i) {
        add_edge(i,(i+1)%n);
    }
}

/* set vertex attribute key=val */
void graphe::set_node_attribute(int index,int key,const gen &val) {
    assert (index>=0 && index<node_count());
    nodes[index].attributes[key]=val;
}

/* set edge {i,j} attribute key=val */
void graphe::set_edge_attribute(int i, int j, int key, const gen &val) {
    attrib &attr=edge_attributes(i,j);
    attr[key]=val;
}

bool graphe::get_graph_attribute(int key,gen &val) const {
    attrib_iter it;
    if ((it=attributes.find(key))==attributes.end()) {
        val=undef;
        return false;
    }
    val=it->second;
    return true;
}

bool graphe::get_node_attribute(int index,int key,gen &val) const {
    assert (index>=0 && index<node_count());
    attrib_iter it;
    const attrib &attr=nodes[index].attributes;
    if ((it=attr.find(key))==attr.end()) {
        val=undef;
        return false;
    }
    val=it->second;
    return true;
}

bool graphe::get_edge_attribute(int i,int j,int key,gen &val) const {
    const attrib &attr=edge_attributes(i,j);
    attrib_iter it;
    if ((it=attr.find(key))==attr.end()) {
        val=undef;
        return false;
    }
    val=it->second;
    return true;
}

bool graphe::is_directed() const {
    gen g;
    assert(get_graph_attribute(_GT_ATTRIB_DIRECTED,g) && g.is_integer());
    return (bool)g.val;
}

bool graphe::is_weighted() const {
    gen g;
    assert(get_graph_attribute(_GT_ATTRIB_WEIGHTED,g) && g.is_integer());
    return (bool)g.val;
}

void graphe::color_nodes(const gen &c) {
    bool isvect=false;
    if (c.type==_VECT) {
        isvect=true;
        assert (int(c._VECTptr->size())==node_count() && is_integer_vecteur(*c._VECTptr,true));
    } else assert (c.is_integer() && c.val>=0);
    for (int i=0;i<node_count();++i) {
        color_node(i,(isvect?c._VECTptr->at(i):c).val);
    }
}

/* create the subgraph defined by vertices from 'vi' and store it in G */
void graphe::make_induced_subgraph(const ivector &vi,graphe &G,bool copy_attrib) const {
    int i,j;
    for (ivector::const_iterator it=vi.begin();it!=vi.end();++it) {
        ivector adj=adjacent_nodes(*it);
        gen v=node(*it);
        const attrib &attri=nodes[*it].attributes;
        if (copy_attrib)
            i=G.add_node(v,attri);
        else
            G.add_node(v);
        for (ivector::const_iterator jt=adj.begin();jt!=adj.end();++jt) {
            if (find(vi.begin(),vi.end(),*jt)==vi.end())
                continue;
            gen w=node(*jt);
            if (copy_attrib) {
                const attrib &attrj=nodes[*jt].attributes;
                j=G.add_node(w,attrj);
                ipair edge=make_edge(*it,*jt);
                const map<int,attrib> &ngh=nodes[edge.first].neighbors;
                neighbor_iter nt=ngh.find(edge.second);
                G.add_edge(i,j,nt->second);
            } else
                G.add_edge(v,w);
        }
    }
}

/* return list of vertices adjacent to i-th one */
graphe::ivector graphe::adjacent_nodes(int i) const {
    assert(i>=0 && i<node_count());
    ivector res;
    for (int j=0;j<node_count();++j) {
        if (i==j)
            continue;
        if (has_edge(i,j) || (is_directed() && has_edge(j,i)))
            res.push_back(j);
    }
    sort(res.begin(),res.end());
    return res;
}

/* return a maximal independent set of vertices */
graphe::ivector graphe::maximal_independent_set() const {
    ivector V(node_count()),vc,vu,adj,adjj,gain(node_count());
    for (int i=0;i<node_count();++i) {
        V[i]=i;
        gain[i]=degree(i);
    }
    vu=V;
    int imax;
    ivector::const_iterator pos;
    while (!vu.empty()) {
        imax=vu.front();
        pos=vu.begin();
        for (ivector::const_iterator it=vu.begin();it!=vu.end();++it) {
            if (gain[imax]<gain[*it]) {
                imax=*it;
                pos=it;
            }
        }
        vu.erase(pos);
        vc.push_back(imax);
        adj=adjacent_nodes(imax);
        for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
            if ((pos=find(vu.begin(),vu.end(),*it))==vu.end())
                continue;
            vu.erase(pos);
            adjj=adjacent_nodes(*it);
            for (ivector::const_iterator jt=adjj.begin();jt!=adjj.end();++jt) {
                if (find(vu.begin(),vu.end(),*jt)==vu.end())
                    continue;
                gain[*jt]++;
            }
        }
    }
    sort(vc.begin(),vc.end());
    return vc;
}

/*
 * EDMONDS' BLOSSOM ALGORITHM FOR MAXIMUM MATCHING ****************************
 */

graphe::edmonds::edmonds(graphe *gr) {
    G=gr;
}

int graphe::edmonds::mate(const vector<ipair> &matching, int v) {
    for (vector<ipair>::const_iterator it=matching.begin();it!=matching.end();++it) {
        if (it->first==v)
            return it->second;
        if (it->second==v)
            return it->first;
    }
    return -1;
}

int graphe::edmonds::find_root(int v) {
    map<int,int>::const_iterator it=forest.find(v);
    if (it==forest.end())
        return -1;
    while (it->second!=-1) {
        it=forest.find(it->second);
    }
    return it->first;
}

int graphe::edmonds::root_distance(map<int,int>::const_iterator it) {
    int d=0;
    while (it->second!=-1) {
        it=forest.find(it->second);
        ++d;
    }
    return d;
}

int graphe::edmonds::root_distance(int v) {
    map<int,int>::const_iterator it=forest.find(v);
    if (it==forest.end())
        return -1;
    return root_distance(it);
}

int graphe::edmonds::find_base(int v, int w) {
    map<int,int>::const_iterator it=forest.find(v);
    if (it==forest.end())
        return -1;
    ivector path;
    while (it->second!=-1) {
        path.push_back(it->first);
        it=forest.find(it->second);
    }
    it=forest.find(w);
    if (it==forest.end())
        return -1;
    while (find(path.begin(),path.end(),it->first)==path.end()) {
        if (it->second==-1)
            return -1;
        it=forest.find(it->second);
    }
    return it->first;
}

bool graphe::edmonds::tree_path(int v, int w, ivector &path) {
    path.clear();
    map<int,int>::const_iterator it=forest.find(w);
    if (it==forest.end())
        return false;
    while (true) {
        path.push_back(it->first);
        if (it->second==-1)
            break;
        it=forest.find(it->second);
    }
    reverse(path.begin(),path.end());
    if ((it=forest.find(v))==forest.end())
        return false;
    while (true) {
        path.push_back(it->first);
        if (it->second==-1)
            break;
        it=forest.find(it->second);
    }
    return true;
}

map<int,graphe::ivector>::iterator graphe::edmonds::in_blossom(int v) {
    map<int,ivector>::iterator it=blossoms.begin();
    for (;it!=blossoms.end();++it) {
        if (find(it->second.begin(),it->second.end(),v)!=it->second.end())
            break;
    }
    return it;
}

map<int,graphe::ivector>::iterator graphe::edmonds::is_blossom_base(int v) {
    map<int,ivector>::iterator it=blossoms.begin();
    for (;it!=blossoms.end();++it) {
        if (v==it->first)
            break;
    }
    return it;
}

void graphe::edmonds::append_non_blossom_adjacents(int v,map<int,ivector>::const_iterator bit,ivector &lst) {
    ivector adj=G->adjacent_nodes(v);
    int n=adj.size(),a;
    for (int i=n;i>=0;--i) {
        a=adj[i];
        if (a==bit->first || find(bit->second.begin(),bit->second.end(),a)!=bit->second.end())
            adj.erase(adj.begin()+i);
    }
    lst.insert(lst.begin(),adj.begin(),adj.end());
}

graphe::ivector graphe::edmonds::adjacent(int v) {
    map<int,ivector>::const_iterator bit=is_blossom_base(v);
    ivector res;
    if (bit!=blossoms.end()) {
        // v is the base of a blossom
        append_non_blossom_adjacents(v,bit,res);
        for (ivector::const_iterator it=bit->second.begin();it!=bit->second.end();++it) {
            append_non_blossom_adjacents(*it,bit,res);
        }
    } else {
        // v does not belong to any of blossoms
        res=G->adjacent_nodes(v);
        for (ivector::iterator it=res.begin();it!=res.end();++it) {
            map<int,ivector>::iterator bit=in_blossom(*it);
            if (bit!=blossoms.end())
                *it=bit->first;
        }
    }
    return res;
}

bool graphe::edmonds::find_augmenting_path(const vector<ipair> &matching, ivector &path) {
    map<int,bool> node_marked;
    map<int,map<int,bool> > edge_marked;
    // collect exposed (free) vertices and create a forest of singleton trees
    forest.clear();
    path.clear();
    for (int i=0;i<G->node_count();++i) {
        if (mate(matching,i)==-1)
            forest.insert(make_pair(i,-1));
    }
    // iterate over unmarked vertices v in F with even root distance
    while (true) {
        map<int,int>::const_iterator fit=forest.begin();
        for (;fit!=forest.end();++fit) {
            if (!node_marked[fit->first] && in_blossom(fit->first)==blossoms.end() && root_distance(fit)%2==0)
                break;
        }
        if (fit==forest.end())
            break;
        int v=fit->first,rv=find_root(v);
        // iterate over unmarked edges {v,w} (edges in the matching are marked by default)
        while (true) {
            ivector adj=adjacent(v);
            ivector::const_iterator wit=adj.begin();
            for (;wit!=adj.end();++wit) {
                int i=v<*wit?v:*wit,j=v<*wit?*wit:v;
                if (mate(matching,*wit)==-1 && !edge_marked[i][j])
                    break;
            }
            if (wit==adj.end())
                break;
            int w=*wit;
            if (forest.find(w)==forest.end()) {
                // update forest
                int x=mate(matching,w);
                assert(x>=0);
                forest[w]=v;
                forest[x]=w;
                edge_marked[w<x?w:x][w<x?x:w]=true; // mark edge {w,x}
            } else if (root_distance(w)%2!=0) {
                // do nothing
                ;
            } else if (rv!=find_root(w)) {
                // augmenting path found
                assert(tree_path(v,w,path));
                return true;
            } else {
                // a blossom is found
                int b=find_base(v,w);
                assert(b>=0);
                // store blossom vertices
                map<int,ivector>::iterator bit=blossoms.insert(make_pair(b,ivector())).first;
                int k=w;
                while (k!=b) {
                    bit->second.push_back(k);
                    assert((k=forest[k])>=0);
                }
                reverse(bit->second.begin(),bit->second.end());
                // find augmenting path with blossom contracted
                ivector short_path;
                if (!find_augmenting_path(matching,short_path))
                    return false;
                ivector::const_iterator sit=short_path.begin();
                int prev=-1;
                for (;sit!=short_path.end();++sit) {
                    if (*sit==b)
                        break;
                    prev=*sit;
                }
                if (sit==short_path.end())
                    path=short_path;
                else {
                    // unfold the blossom
                    if (sit!=short_path.begin() && prev!=forest[*sit])
                        reverse(short_path.begin(),short_path.end());
                    for (sit=short_path.begin();sit!=short_path.end();++sit) {
                        path.push_back(*sit);
                        if (*sit==b) {
                            bit=blossoms.find(b);
                            if (!G->has_edge(bit->first,*(sit+1))) {
                                for (ivector::const_iterator it=bit->second.begin();it!=bit->second.end();++it) {
                                    path.push_back(*it);
                                    if (G->has_edge(*it,*(sit+1)))
                                        break;
                                }
                            }
                        }
                    }
                    blossoms.erase(blossoms.find(b));
                }
                return true;
            }
            edge_marked[v<w?v:w][v<w?w:v]=true; // mark edge {v,w}
        }
        node_marked[v]=true; // mark node v
    }
    return false;
}

void graphe::edmonds::find_maximum_matching(vector<ipair> &matching) {
    ivector path;
    while (find_augmenting_path(matching,path)) {
        // augmenting path was found, extend the matching
        int len=path.size();
        assert(len>=2);
        if (len==2) {
            matching.push_back(make_edge(path.front(),path.back()));
        } else {
            for (int i=0;i<len-2;i+=2) {
                ipair edge=make_edge(path[i+1],path[i+2]);
                vector<ipair>::iterator it=find(matching.begin(),matching.end(),edge);
                assert(it!=matching.end());
                *it=make_edge(path[i],path[i+1]);
            }
            matching.push_back(make_edge(path[len-2],path[len-1]));
        }
    }
}

/*
 * END OF BLOSSOM ALGORITHM IMPLEMENTATION ************************************
 */

/* extend given matching to the maximum matching using Edmonds' blossom algorithm */
void graphe::maximize_matching(vector<ipair> &matching) {
    edmonds ed(this);
    ed.find_maximum_matching(matching);
}

/* find a maximal matching in the current graph */
vector<graphe::ipair> graphe::find_maximal_matching() const {
    map<int,int> match;
    map<int,bool> node_marked;
    int v;
    while (true) {
        for (v=0;v<node_count();++v) {
            if (!node_marked[v] && match[v]==0)
                break;
        }
        if (v==node_count())
            break;
        node_marked[v]=true;
        ivector adj=adjacent_nodes(v);
        for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
            if (match[*it]==0) {
                match[*it]=v;
                match[v]=*it;
                break;
            }
        }
    }
    vector<ipair> matching;
    for (map<int,int>::const_iterator it=match.begin();it!=match.end();++it) {
        int i=it->first,j=it->second;
        ipair edge=make_pair(i<j?i:j,i<j?j:i);
        if (find(matching.begin(),matching.end(),edge)==matching.end())
            matching.push_back(edge);
    }
    return matching;
}

/* add rvec b to rvec a */
void graphe::add_rvec(rvec &a, const rvec &b) {
    int d=a.size();
    assert(int(b.size())==d);
    for (int i=0;i<d;++i) {
        a[i]+=b[i];
    }
}

/* subtract rvec b from rvec a */
void graphe::subtract_rvec(rvec &a, const rvec &b) {
    int d=a.size();
    assert(int(b.size())==d);
    for (int i=0;i<d;++i) {
        a[i]-=b[i];
    }
}

/* scale coordinates of rvec p by factor s */
void graphe::scale_rvec(rvec &p, double s) {
    int d=p.size();
    for (int i=0;i<d;++i) {
        p[i]*=s;
    }
}

/* set all coordinates of rvec p to zero */
void graphe::clear_rvec_coords(rvec &p) {
    for (rvec::iterator it=p.begin();it!=p.end();++it) {
        *it=0;
    }
}

/* convert rvec to Giac point representation point(x,y) or point(x,y,z) */
gen graphe::rvec2gen(const rvec &p,bool pt) {
    vecteur coords;
    for (rvec::const_iterator it=p.begin();it!=p.end();++it) {
        coords.push_back(*it);
    }
    if (pt)
        return symbolic(at_point,_feuille(coords,context0));
    return coords;
}

/* compute the square root of sum of squares of coordinates of rvec p */
double graphe::rvec_norm(const rvec &p, bool sqroot) {
    double norm=0;
    for (rvec::const_iterator it=p.begin();it!=p.end();++it) {
        norm+=std::pow(*it,2.0);
    }
    return sqroot?std::pow(norm,0.5):norm;
}

/* copy layout src to dest (both must be initialized first) */
void graphe::copy_layout(const layout &src, layout &dest) {
    layout::const_iterator st=src.begin();
    layout::iterator dt=dest.begin();
    for (;st!=src.end() && dt!=dest.end();++st,++dt) {
        *dt=*st;
    }
}

/* copy rvec src to dest (both must be initialized first) */
void graphe::copy_rvec(const rvec &src, rvec &dest) {
    rvec::const_iterator st=src.begin();
    rvec::iterator dt=dest.begin();
    for (;st!=src.end() && dt!=dest.end();++st,++dt) {
        *dt=*st;
    }
}

/* generate uniformly random point p on the origin-centered sphere with the specified radius */
void graphe::rand_rvec(rvec &p, double radius) {
    double R;
    do {
        R=0;
        for (rvec::iterator it=p.begin();it!=p.end();++it) {
            *it=normal_distribution(random_generator);
            R+=std::pow(*it,2.0);
        }
    } while (R==0);
    scale_rvec(p,radius/std::pow(R,0.5));
}

/* convert rvec from cartesian to polar representation (2d only) */
void graphe::make_rvec_polar(rvec &p) {
    int d=p.size();
    assert(d==2);
    double x=p[0],y=p[1];
    p[0]=std::pow(std::pow(x,2.0)+std::pow(y,2.0),0.5);
    p[1]=atan2(y,x);
}

/* translate layout x by dx */
void graphe::translate_layout(layout &x, const rvec &dx) {
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        add_rvec(*it,dx);
    }
}

/* rotate layout x about the point p by the angle phi (2d only) */
void graphe::rotate_layout(layout &x, double phi) {
    if (x.empty())
        return;
    int d=x.front().size();
    assert(d==2);
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        it->at(1)+=phi;
    }
}

/* determine minimum of d-th components among layout points */
double graphe::layout_min(const layout &x, int d) {
    double m=DBL_MAX;
    for (layout::const_iterator it=x.begin();it!=x.end();++it) {
        if (it->at(d)<m)
            m=it->at(d);
    }
    return m;
}

/* compute the center of layout x */
graphe::rvec graphe::layout_center(const layout &x) {
    assert(!x.empty());
    int d=x.front().size();
    rvec center(d);
    clear_rvec_coords(center);
    for (layout::const_iterator it=x.begin();it!=x.end();++it) {
        add_rvec(center,*it);
    }
    scale_rvec(center,1.0/x.size());
    return center;
}

/* scale layout x to have the diameter equal to diam */
void graphe::scale_layout(layout &x, double diam) {
    if (x.empty())
        return;
    int d=x.front().size();
    assert(d==2 || d==3);
    rvec M(d,-DBL_MAX),m(d,DBL_MAX);
    for (layout::const_iterator it=x.begin();it!=x.end();++it) {
        const rvec &p=*it;
        for (int i=0;i<d;++i) {
            if (p[i]<m[i])
                m[i]=p[i];
            if (p[i]>M[i])
                M[i]=p[i];
        }
    }
    double D=0;
    for (int i=0;i<d;++i) {
        if (M[i]-m[i]>D)
            D=M[i]-m[i];
    }
    if (D==0)
        return;
    double s=diam/D;
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        scale_rvec(*it,s);
    }
}

/* make layout x unique up to the order of the vertices (using first vertex as a reference) */
void graphe::make_layout_unique(layout &x,double K) {
    if (x.empty())
        return;
    int d=x.front().size();
    assert(d==2);
    rvec mc(d),p(d);
    layout::const_iterator it=x.begin();
    for (;it->at(0)==0 && it->at(1)==0;++it);
    copy_rvec(*it,p);
    rvec c=layout_center(x);
    subtract_rvec(p,c);
    make_rvec_polar(p);
    double phi=p[1];
    // compute the center of layout x
    copy_rvec(c,mc);
    scale_rvec(mc,-1);
    translate_layout(x,mc);
    // make layout polar
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        make_rvec_polar(*it);
    }
    rotate_layout(x,M_PI_2-phi);
    // make layout cartesian
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        rvec &p=*it;
        double r=p[0],phi=p[1];
        p[0]=r*std::cos(phi);
        p[1]=r*std::sin(phi);
    }
    double mx=layout_min(x,0),my=layout_min(x,1),mz=0;
    if (d==3)
        mz=layout_min(x,2);
    rvec dx(d);
    dx[0]=K-mx;
    dx[1]=K-my;
    if (d==3)
        dx[2]=K-mz;
    translate_layout(x,dx);
}

/* create a random layout in the first quadrant/octant */
void graphe::create_random_layout(layout &x, double K, int d) {
    rvec p(d);
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        p[0]=K+normal_distribution(random_generator)*K/3;
        p[1]=K+normal_distribution(random_generator)*K/3;
        if (d==3)
            p[2]=K+normal_distribution(random_generator)*K/3;
        *it=p;
    }
}

/* count how many edges go to vertex i and pass through the specified quadrant (2d only) */
int graphe::count_node_label_obstacles(const layout &x, int i, int quadrant) const {
    const rvec &p=x[i];
    ivector adj=adjacent_nodes(i);
    int count=0;
    for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
        const rvec &q=x[*it];
        double x=q[0]-p[0],y=q[1]-p[1],phi=atan2(y,x);
        if ((quadrant==1 && phi>0 && phi<M_PI_2) || (quadrant==2 && phi>M_PI_2) ||
                (quadrant==3 && phi<-M_PI_2) || (quadrant==4 && phi<0 && phi>-M_PI_2))
            ++count;
    }
    return count;
}

/* return the index of the best quadrant (1, 2, 3 or 4) for placement of i-th vertex label (2d only) */
int graphe::node_label_best_quadrant(const layout &x, const rvec &center, int i) {
    ivector bq;
    int c,min_c=RAND_MAX;
    // mark quadrants with lowest number of edges crossing them
    // (these are candidates for the best quadrant)
    for (int q=1;q<=4;++q) {
        c=count_node_label_obstacles(x,i,q);
        if (c<min_c) {
            min_c=c;
            bq.clear();
            bq.push_back(q);
        } else if (c==min_c)
            bq.push_back(q);
    }
    assert(!bq.empty());
    if (bq.size()==1)
        return bq.front();
    // when there are more candidates, choose the quadrant
    // which is most distant from the center of the graph,
    // thus pushing labels towards infinity (in all directions)
    int nbq=bq.front();
    double d=0,dq;
    const rvec &p=x[i];
    rvec r(2);
    for (int q=1;q<=4;++q) {
        copy_rvec(p,r);
        r[0]+=(q==1 || q==4)?1:-1;
        r[1]+=(q==1 || q==2)?1:-1;
        subtract_rvec(r,center);
        dq=rvec_norm(r,false);
        if (dq>d) {
            d=dq;
            nbq=q;
        }
    }
    return nbq;
}

/* return graph layout obtained by a force-directed algorithm */
void graphe::force_directed(layout &x, double K, double R, double tol, bool ac) {
    double step=K,t=0.9; // initial step length and the shrinking factor;
    double E=DBL_MAX,E0; // energy
    double C=0.01;
    double D=C*std::pow(K,2.0);
    double Rsq=std::pow(R,2.0);
    double norm,m;
    double eps=K*tol;
    int progress=0;
    int n=x.size(),i,j;
    if (n==0)
        return;
    assert (n==node_count());
    int d=x.front().size();
    rvec f(d),p(d),xi(d);
    layout x0(n);
    ivector adj;
    do {
        copy_layout(x,x0);
        E0=E;
        E=0;
        for (i=0;i<n;++i) {
            copy_rvec(x[i],xi);
            clear_rvec_coords(f);
            // compute attractive forces between vertices adjacent to the i-th vertex
            adj=adjacent_nodes(i);
            for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
                j=*it;
                copy_rvec(xi,p);
                subtract_rvec(p,x[j]);
                scale_rvec(p,rvec_norm(p)/K);
                subtract_rvec(f,p);
            }
            // compute repulsive forces for all vertices not too far from the i-th vertex
            for (j=0;j<n;++j) {
                if (i==j)
                    continue;
                copy_rvec(xi,p);
                subtract_rvec(p,x[j]);
                norm=rvec_norm(p,false);
                if (Rsq>0 && norm>Rsq)
                    continue;
                if (norm==0)
                    rand_rvec(p,norm=t*eps);
                scale_rvec(p,D/norm);
                add_rvec(f,p);
            }
            // move the location of the i-th vertex
            norm=rvec_norm(f);
            if (norm==0)
                continue;
            if (step<norm)
                scale_rvec(f,step/norm);
            add_rvec(x[i],f);
            E+=std::pow(norm,2.0);
        }
        // update step length
        if (ac) {
            // use adaptive cooling scheme
            if (E<E0) {
                ++progress;
                if (progress>=5) {
                    progress=0;
                    step/=t;
                }
            } else {
                progress=0;
                step*=t;
            }
        } else step*=t; // use simple cooling scheme
        // compute total movement magnitude
        m=0;
        layout::const_iterator xit=x.begin(),x0it=x0.begin();
        for (;xit!=x.end();++xit,++x0it) {
            copy_rvec(*xit,p);
            subtract_rvec(p,*x0it);
            norm=rvec_norm(p);
            if (norm>m)
                m=norm;
        }
    } while (m>=eps);
}

bool graphe::sparse_matrix_element(const sparsemat &A, int i, int j, double &val) {
    sparsemat::const_iterator it;
    map<int,double>::const_iterator jt;
    if ((it=A.find(i))==A.end() || (jt=it->second.find(j))==it->second.end())
        return false;
    val=jt->second;
    return true;
}

void graphe::multiply_sparse_matrices(const sparsemat &A, const sparsemat &B, sparsemat &prod) {
    int i,nc=0;
    double val;
    // determine number of columns of matrix B
    for (sparsemat::const_iterator it=B.begin();it!=B.end();++it) {
        for (map<int,double>::const_iterator jt=it->second.begin();jt!=it->second.end();++jt) {
            if (jt->first>nc)
                nc=jt->first;
        }
    }
    // compute the matrix product A*B
    for (sparsemat::const_iterator it=A.begin();it!=A.end();++it) {
        i=it->first;
        for (int j=0;j<=nc;++j) {
            for (map<int,double>::const_iterator jt=it->second.begin();jt!=it->second.end();++jt) {
                if (sparse_matrix_element(B,jt->first,j,val) && val!=0)
                    prod[i][j]+=jt->second*val;
            }
        }
    }
}

void graphe::transpose_sparsemat(const sparsemat &A, sparsemat &transp) {
    for (sparsemat::const_iterator it=A.begin();it!=A.end();++it) {
        for(map<int,double>::const_iterator jt=it->second.begin();jt!=it->second.end();++jt) {
            transp[jt->first][it->first]=jt->second;
        }
    }
}

/* return number of vertices in V which are incident to the i-th vertex */
int graphe::mdeg(const ivector &V, int i) const {
    int d=0;
    for (ivector::const_iterator it=V.begin();it!=V.end();++it) {
        if (i==*it)
            return 1;
        if (has_edge(i,*it))
            ++d;
    }
    return d;
}

/* coarsening of the graph with respect to prolongation matrix P */
void graphe::coarsening(graphe &G,const sparsemat &P,const ivector &V) const {
    sparsemat Q,I,R,IG;
    int m=V.size(),n=node_count();
    transpose_sparsemat(P,Q);
    // create sparse symmetric incidence matrix I of this graph
    for (int i=0;i<n;++i) {
        for (int j=i+1;j<n;++j) {
            if (has_edge(i,j)) {
                I[i][j]=1;
                I[j][i]=1;
            }
        }
    }
    // use Galerkin product Q*I*P as the incidence matrix IG for graph G
    multiply_sparse_matrices(Q,I,R);
    multiply_sparse_matrices(R,P,IG);
    // create vertices of G
    double val;
    for (ivector::const_iterator it=V.begin();it!=V.end();++it) {
        G.add_node(node(*it));
    }
    // create edges of G
    for (int i=0;i<m;++i) {
        for (int j=i+1;j<m;++j) {
            if (i!=j && sparse_matrix_element(IG,i,j,val) && val!=0)
                G.add_edge(i,j);
        }
    }
}

/* make coarser graph G by restricting to maximal independent set */
void graphe::coarsening_mis(const ivector &V, graphe &G, sparsemat &P) const {
    int n=node_count(),m=V.size();
    for (int i=0;i<n;++i) {
        int md=mdeg(V,i);
        assert(md>0);
        for (int j=0;j<m;++j) {
            if (i==V[j])
                P[i][j]=1;
            else if (has_edge(i,V[j]))
                P[i][j]=1.0/md;
        }
    }
    coarsening(G,P,V);
}

/* make coarser graph by collapsing edges found in maximal matching */
void graphe::coarsening_ec(const vector<ipair> &M, graphe &G, sparsemat &P) const {
    ivector removed_vertices,V;
    for (vector<ipair>::const_iterator it=M.begin();it!=M.end();++it) {
        removed_vertices.push_back(it->second);
    }
    int n=node_count(),m=removed_vertices.size(),J;
    ivector::iterator ivt;
    for (int i=0;i<n;++i) {
        if ((ivt=find(removed_vertices.begin(),removed_vertices.end(),i))==removed_vertices.end())
            V.push_back(i);
        else
            removed_vertices.erase(ivt);
    }
    for (int i=0;i<n;++i) {
        for (int j=0;j<n-m;++j) {
            if (i==(J=V[j]) || find(M.begin(),M.end(),make_pair(J,i))!=M.end())
                P[i][j]=1;
        }
    }
    coarsening(G,P,V);
}

static int multilevel_depth;
static bool multilevel_mis;

void graphe::multilevel_recursion(layout &x,graphe &G,int d,double R,double K,double tol,int depth) {
    ivector mis;
    vector<ipair> M;
    if (multilevel_mis)
        mis=G.maximal_independent_set();
    else
        M=G.find_maximal_matching();
    int n=G.node_count(),m=multilevel_mis?mis.size():n-int(M.size());
    x.resize(n);
    if (m>0.75*n) {
        // coarsening is slow, switch from EC to MIS method since it's faster
        multilevel_mis=true;
        multilevel_recursion(x,G,d,R,K,tol,depth);
        return;
    }
    if (m<2) {
        // the coarsest level, apply force directed algorithm on random initial layout
        multilevel_depth=depth;
        create_random_layout(x,K,d);
        G.force_directed(x,K,R*(depth+1)*K,tol,false);
    } else {
        // create coarser graph H and lay it out
        graphe H;
        sparsemat P; // prolongation matrix
        if (multilevel_mis)
            G.coarsening_mis(mis,H,P);
        else
            G.coarsening_ec(M,H,P);
        layout y;
        multilevel_recursion(y,H,d,R,K,tol,depth+1);
        // compute x=P*y (layout lifting)
        double pij;
        rvec yj(d);
        for (int i=0;i<n;++i) {
            x[i]=rvec(d,0);
            for (int j=0;j<m;++j) {
                if (sparse_matrix_element(P,i,j,pij)) {
                    copy_rvec(y[j],yj);
                    scale_rvec(yj,pij);
                    add_rvec(x[i],yj);
                }
            }
        }
        // make the natural spring length K shorter with respect to
        // the current depth level and subsequently refine layout x
        double L=K*std::pow(PLASTIC_NUMBER,depth-multilevel_depth);
        G.force_directed(x,L,R*(depth+1)*L,tol,false);
    }
}

/* apply multilevel force directed algorithm to layout x */
void graphe::multilevel_force_directed(layout &x,int d,double K,double tol) {
    int n=node_count();
    if (n==0)
        return;
    multilevel_mis=false;
    return multilevel_recursion(x,*this,d,DBL_MAX,K,tol);
}

void graphe::tomita_recurse(const ivector &R, const ivector &P_orig, const ivector &X_orig,
                            vector<graphe::ivector> &cliques) const {
    if (P_orig.empty() && X_orig.empty())
        cliques.push_back(R);
    else {
        ivector P(P_orig),X(X_orig);
        // choose as the pivot element the node with the highest number of neighbors in P, say i-th
        ivector PUX;
        ivector::const_iterator it,jt;
        PUX.resize(P.size()+X.size());
        it=set_union(P.begin(),P.end(),X.begin(),X.end(),PUX.begin());
        PUX.resize(it-PUX.begin());
        int i=PUX.front(),mn=0,n,v;
        ivector adj,S;
        for (it=PUX.begin();it!=PUX.end();++it) {
            adj=adjacent_nodes(*it);
            S.resize(std::max(adj.size(),P.size()));
            jt=set_intersection(adj.begin(),adj.end(),P.begin(),P.end(),S.begin());
            if ((n=int(jt-S.begin()))>mn) {
                mn=n;
                i=*it;
            }
        }
        // for each vertex in P\N(i) do recursion
        adj=adjacent_nodes(i);
        S.clear();
        S.resize(P.size());
        it=set_difference(P.begin(),P.end(),adj.begin(),adj.end(),S.begin());
        S.resize(it-S.begin());
        for (jt=S.begin();jt!=S.end();++jt) {
            v=*jt;
            ivector RUv(R),PP,XX;
            RUv.push_back(v);
            sort(RUv.begin(),RUv.end());
            adj=adjacent_nodes(v);
            PP.resize(std::max(P.size(),adj.size()));
            it=set_intersection(P.begin(),P.end(),adj.begin(),adj.end(),PP.begin());
            PP.resize(it-PP.begin());
            XX.resize(std::max(X.size(),adj.size()));
            it=set_intersection(X.begin(),X.end(),adj.begin(),adj.end(),XX.begin());
            XX.resize(it-XX.begin());
            tomita_recurse(RUv,PP,XX,cliques);
            P.erase(find(P.begin(),P.end(),v));
            X.push_back(v);
            sort(X.begin(),X.end());
        }
    }
}

/* a modified version of the Bron-Kerbosch algorithm developed by Tomita et al. */
void graphe::tomita(vector<ivector> &cliques) const {
    ivector R,X,P(node_count());
    for (int i=0;i<node_count();++i) P[i]=i;
    tomita_recurse(R,P,X,cliques);
}

/* remove i-th node which is assumed to be isolated */
void graphe::remove_isolated_node(int i) {
    vector<ipair> ev;
    int v=0,w;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        for (map<int,attrib>::const_iterator jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            w=jt->first;
            ev.push_back(make_edge(v>i?v-1:v,w>i?w-1:w));
        }
        it->neighbors.clear();
        ++v;
    }
    nodes.erase(nodes.begin()+i);
    for (vector<ipair>::const_iterator it=ev.begin();it!=ev.end();++it) {
        add_edge(*it);
    }
}

/* collapse edge {i,j}, leaving j-th node isolated (not connected to any other node) */
void graphe::collapse_edge(int i,int j) {
    ivector adj=adjacent_nodes(j);
    for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
        remove_edge(*it,j);
        if (*it!=i)
            add_edge(*it,i);
        if (is_directed()) {
            remove_edge(j,*it);
            if (*it!=i)
                add_edge(i,*it);
        }
    }
}

/* return list of edges incident to vertices in v */
vector<graphe::ipair> graphe::incident_edges(const ivector &v) {
    int i=0;
    bool b;
    map<int,bool> sw;
    for (ivector::const_iterator it=v.begin();it!=v.end();++it) {
        sw[*it]=true;
    }
    vector<ipair> res;
    for (vector<vertex>::const_iterator it=nodes.begin();it!=nodes.end();++it) {
        b=sw[i];
        for (map<int,attrib>::const_iterator jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            if (b || sw[jt->first])
                res.push_back(make_pair(i,jt->first));
        }
        ++i;
    }
    return res;
}

/* make a list of all n-tuples of k integers 0,1,...,k-1 */
void ntupk(vector<graphe::ivector> &v,int n,int k,const graphe::ivector &elem,int i) {
    for (int j=0;j<k;++j) {
        graphe::ivector e(elem);
        e[i]=j;
        if (i<n-1)
            ntupk(v,n,k,e,i+1);
        else
            v.push_back(e);
    }
}

/* return true iff the vertices u and v of sierpinski graph are adjacent */
bool is_sierpinski_match(const graphe::ivector &u,const graphe::ivector &v,int n,int h) {
    int t=0;
    for (;t<=h-1;++t) {
        if (u[t]!=v[t])
            return false;
    }
    if (u[h]==v[h])
        return false;
    for (t=h+1;t<n;++t) {
        if (u[t]!=v[h] || v[t]!=u[h])
            return false;
    }
    return true;
}

/* create Sierpinski (triangle) graph */
void graphe::make_sierpinski_graph(int n, int k, bool triangle,GIAC_CONTEXT) {
    vector<ivector> tuples;
    ivector elem(n,0);
    ntupk(tuples,n,k,elem,0);
    int N=std::pow(k,n);
    vecteur V;
    make_default_vertex_labels(V,N,0,contextptr);
    add_nodes(V);
    for (int i=0;i<N;++i) {
        ivector &u=tuples[i];
        for (int j=i+1;j<N;++j) {
            ivector &v=tuples[j];
            int h=0;
            for (;h<n;++h) {
                if (is_sierpinski_match(u,v,n,h))
                    break;
            }
            if (h<n)
                add_edge(i,j);
        }
    }
    if (triangle) {
        // remove all non-clique edges to obtain Sierpinski triangle graph
        vector<ivector> cliques;
        tomita(cliques);
        ivector isolated_nodes;
        for (vector<ivector>::const_iterator it=cliques.begin();it!=cliques.end();++it) {
            if (it->size()==2) {
                int v=it->front(),w=it->back();
                collapse_edge(v,w);
                isolated_nodes.push_back(w);
            }
        }
        sort(isolated_nodes.begin(),isolated_nodes.end());
        for (int i=int(isolated_nodes.size())-1;i>=0;--i) {
            remove_isolated_node(isolated_nodes[i]);
        }
    }
}

/* create complete graph with vertices from list V */
void graphe::make_complete_graph(const vecteur &V) {
    add_nodes(V);
    int n=V.size();
    for (int i=0;i<n;++i) {
        for (int j=i+1;j<n;++j) {
            add_edge(i,j);
        }
    }
}

/* create complete k-partite graph with given partition sizes */
void graphe::make_complete_multipartite_graph(const ivector &partition_sizes,GIAC_CONTEXT) {
    int k=partition_sizes.size();
    vector<ivector> partitions;
    int n0=0;
    vecteur v;
    for (ivector::const_iterator it=partition_sizes.begin();it!=partition_sizes.end();++it) {
        int n=*it;
        make_default_vertex_labels(v,n,n0,contextptr);
        add_nodes(v);
        ivector iv(n);
        for (int i=0;i<n;++i) iv[i]=n0+i;
        partitions.push_back(iv);
        n0+=n;
    }
    for (int ip=0;ip<k;++ip) {
        ivector &pi=partitions[ip];
        for (int jp=ip+1;jp<k;++jp) {
            ivector &pj=partitions[jp];
            for (int i=0;i<int(pi.size());++i) {
                for (int j=0;j<int(pj.size());++j) {
                    add_edge(pi[i],pj[j]);
                }
            }
        }
    }
}

/* create generalized Petersen graph G(n,k) using Watkins' notation */
void graphe::make_petersen_graph(int n, int k,GIAC_CONTEXT) {
    vecteur u,v;
    make_default_vertex_labels(u,n,0,contextptr);
    make_default_vertex_labels(v,n,n,contextptr);
    add_nodes(mergevecteur(u,v));
    for (int i=0;i<n;++i) {
        add_edge(i,(i+1)%n);
        add_edge(i,i+n);
        add_edge(i+n,(i+k)%n+n);
    }
}

void graphe::make_connected_component(int i, int ci, map<int,int> &indices) const {
    indices[i]=ci;
    ivector adj=adjacent_nodes(i);
    // recurse over unvisited nodes adjacent to i-th node,
    // visiting all vertices of the component eventually
    for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
        if (indices[*it]==0)
            make_connected_component(*it,ci,indices);
    }
}

/* find all connected components of an undirected graph and store them */
void graphe::connected_components(vector<ivector> &components) const {
    map<int,int> indices;
    int i=0,ci=0;
    while (i<node_count()) {
        if (indices[i]==0)
            make_connected_component(i,++ci,indices);
        ++i;
    }
    components=vector<ivector>(ci);
    for (map<int,int>::const_iterator it=indices.begin();it!=indices.end();++it) {
        components[it->second-1].push_back(it->first);
    }
}

static map<int,bool> dfs_visited;
static graphe::ivector dfs_parent;

void graphe::find_cut_vertices_dfs(int u,int &t,ivector &disc,ivector &low,vector<bool> &ap) const {
    int children=0;
    dfs_visited[u]=true;
    disc[u]=low[u]=++t;
    ivector adj=adjacent_nodes(u);
    for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
        int v=*it;
        if (!dfs_visited[v]) {
            ++children;
            dfs_parent[v]=u;
            find_cut_vertices_dfs(v,t,disc,low,ap);
            low[u]=std::min(low[u],low[v]);
            if ((dfs_parent[u]==-1 && children>1) || (dfs_parent[u]!=-1 && low[v]>=disc[u]))
                ap[u]=true;
        } else if (v!=dfs_parent[u])
            low[u]=std::min(low[u],disc[v]);
    }
}

/* return list of cut vertices obtained by using depth-first search, complexity O(V+E) */
graphe::ivector graphe::find_cut_vertices() const {
    dfs_parent=ivector(node_count(),-1);
    ivector low(node_count()),disc(node_count());
    vector<bool> ap(node_count(),false);
    int t=0;
    for (int i=0;i<node_count();++i) {
        if (!dfs_visited[i])
            find_cut_vertices_dfs(i,t,disc,low,ap);
    }
    dfs_visited.clear();
    dfs_parent.clear();
    ivector res;
    int i=0;
    for (vector<bool>::const_iterator it=ap.begin();it!=ap.end();++it) {
        if (*it)
            res.push_back(i);
        ++i;
    }
    return res;
}

void graphe::find_blocks_dfs(int v,int &t,ivector &dfi,ivector &p,stack<ipair> &s,
                             map<int,ivector> &stacked,vector<vector<ipair> > &blocks) const {
    dfi[v]=p[v]=t++;
    ivector adj=adjacent_nodes(v);
    int w,n;
    ipair edge,block_edge;
    for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
        w=*it;
        edge=make_edge(v,w);
        map<int,ivector>::const_iterator st;
        if ((st=stacked.find(edge.first))==stacked.end() ||
                find(st->second.begin(),st->second.end(),edge.second)==st->second.end()) {
            s.push(edge);
            // keep track of stacked edges
            stacked[edge.first].push_back(edge.second);
        }
        if (dfi[w]==0) {
            dfs_parent[w]=v;
            find_blocks_dfs(w,t,dfi,p,s,stacked,blocks);
            if (p[w]>=dfi[v]) {
                // output biconnected component to 'blocks'
                n=blocks.size();
                blocks.resize(n+1);
                do {
                    block_edge=s.top();
                    s.pop();
                    blocks[n].push_back(block_edge);
                    map<int,ivector>::iterator pt=stacked.find(block_edge.first);
                    assert(pt!=stacked.end());
                    ivector::iterator rt=find(pt->second.begin(),pt->second.end(),block_edge.second);
                    assert(rt!=pt->second.end());
                    pt->second.erase(rt);
                    if (pt->second.empty())
                        stacked.erase(pt);
                } while (block_edge!=edge);
            }
            p[v]=std::min(p[v],p[w]);
        } else if (w!=dfs_parent[v])
            p[v]=std::min(p[v],dfi[w]);
    }
}

/* create list of biconnected components (as lists of edges) of the graph */
void graphe::find_blocks(vector<vector<ipair> > &blocks) const {
    int t=1;
    stack<ipair> s;
    ivector dfi(node_count(),0);
    ivector p(node_count());
    map<int,ivector> stacked;
    ivector::const_iterator it;
    dfs_parent=ivector(node_count(),-1);
    while ((it=find(dfi.begin(),dfi.end(),0))!=dfi.end()) {
        find_blocks_dfs(it-dfi.begin(),t,dfi,p,s,stacked,blocks);
    }
    dfs_visited.clear();
    dfs_parent.clear();
}

int graphe::find_cycle_dfs(int v, stack<int> &path, vector<bool> &visited, ivector &parent) const {
    visited[v]=true;
    path.push(v);
    ivector adj=adjacent_nodes(v);
    int w,z,p=parent[v];
    for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
        w=*it;
        if (visited[w]) {
            if (p!=w)
                return w;
            continue;
        }
        parent[w]=v;
        z=find_cycle_dfs(w,path,visited,parent);
        if (z>=0)
            return z;
    }
    path.pop();
    return -1;
}

/* return a cycle in this graph using DFS (graph is assumed to be connected) */
graphe::ivector graphe::find_cycle() const {
    vector<bool> visited(node_count(),false);
    ivector parent(node_count(),-1);
    stack<int> path;
    ivector cycle;
    if (node_count()>0) {
        int v=find_cycle_dfs(0,path,visited,parent);
        if (v>=0) {
            int i;
            do {
                i=path.top();
                cycle.push_back(i);
                path.pop();
            } while (i!=v);
        }
    }
    return cycle;
}

bool graphe::find_path_dfs(int dest,int v,stack<int> &path,vector<bool> &visited) const {
    path.push(v);
    if (v==dest)
        return true;
    visited[v]=true;
    ivector adj=adjacent_nodes(v);
    int w;
    for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
        w=*it;
        if (visited[w])
            continue;
        if (find_path_dfs(dest,w,path,visited))
            return true;
    }
    path.pop();
    return false;
}

/* return a path from vertex v to vertex w using DFS (graph is assumed to be connected) */
graphe::ivector graphe::find_path(int v, int w) const {
    vector<bool> visited(node_count(),false);
    stack<int> path;
    ivector res;
    if (find_path_dfs(v,w,path,visited)) {
        while (!path.empty()) {
            res.push_back(path.top());
            path.pop();
        }
    }
    return res;
}

/* get all edges as ipairs */
void graphe::get_edges(vector<ipair> &E) const {
    int i=0;
    for (vector<vertex>::const_iterator it=nodes.begin();it!=nodes.end();++it) {
        for (map<int,attrib>::const_iterator jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            E.push_back(make_pair(i,jt->first));
        }
        ++i;
    }
}

/* insert edges from G to this graph */
void graphe::join(const graphe &G) {
    bool weighted=G.is_weighted() && this->is_weighted();
    vecteur E=G.edges(weighted);
    gen v,w,weight(1);
    for (const_iterateur it=E.begin();it!=E.end();++it) {
        if (weighted) {
            v=it->_VECTptr->front()._VECTptr->front();
            w=it->_VECTptr->front()._VECTptr->back();
            weight=it->_VECTptr->back();
        } else {
            v=it->_VECTptr->front();
            w=it->_VECTptr->back();
        }
        add_edge(v,w,weight);
    }
}

/* translate indices to match those in graph G (which must be a subgraph of this graph) */
graphe::ivector graphe::translate_indices_to(const graphe &G, const ivector &indices) const {
    int n=indices.size(),i=0;
    ivector res(n);
    for (ivector::const_iterator it=indices.begin();it!=indices.end();++it) {
        res[i]=G.node_index(node(*it));
        ++i;
    }
    return res;
}

/* translate indices of vertices in G (a subgraph of this) to match those in this graph */
graphe::ivector graphe::translate_indices_from(const graphe &G, const ivector &indices) const {
    int n=indices.size(),i=0;
    ivector res(n);
    for (ivector::const_iterator it=indices.begin();it!=indices.end();++it) {
        res[i]=node_index(G.node(*it));
        ++i;
    }
    return res;
}

bool face_has_edge(const graphe::ivector &face,int i,int j) {
    int n=face.size(),u,v;
    for (int k=0;k<n;++k) {
        u=face[k];
        v=face[(k+1)%n];
        if ((i==u && j==v) || (i==v && j==u))
            return true;
    }
    return false;
}

/* return sorted list of indices of all nodes shared by a bridge and planar embedding */
void graphe::bridge_contact_vertices(const graphe &bridge,const vector<bool> &embedding,ivector &cp) const {
    vecteur V=bridge.vertices();
    int i;
    cp.clear();
    for (const_iterateur it=V.begin();it!=V.end();++it) {
        i=node_index(*it);
        if (embedding[i])
            cp.push_back(i);
    }
    sort(cp.begin(),cp.end());
}

/* construct all bridges to the planar embedding defined by the specified faces and append them to B */
void graphe::bridges(const vector<bool> &embedding,const vector<ivector> &faces,vector<graphe> &B) const {
    ivector rest,adj;
    int i=0,N=B.size();
    for (vector<bool>::const_iterator it=embedding.begin();it!=embedding.end();++it) {
        if (!*it)
            rest.push_back(i);
        ++i;
    }
    if (rest.empty())
        return;
    graphe G;
    make_induced_subgraph(rest,G,false);
    vector<ivector> components;
    G.connected_components(components);
    for (vector<ivector>::iterator ct=components.begin();ct!=components.end();++ct) {
        *ct=translate_indices_from(G,*ct);
    }
    // find non-facial edges which connect embedded vertices
    vector<ipair> v;
    vector<ivector>::const_iterator it;
    for (i=0;i<node_count();++i) {
        if (!embedding[i])
            continue;
        for (int j=is_directed()?0:i+1;j<node_count();++j) {
            if (i==j || !embedding[j] || !has_edge(i,j))
                continue;
            for (it=faces.begin();it!=faces.end();++it) {
                if (face_has_edge(*it,i,j))
                    break;
            }
            if (it==faces.end())
                v.push_back(make_pair(i,j));
        }
    }
    B.resize(N+components.size()+v.size());
    // create bridges from components
    i=0;
    for (it=components.begin();it!=components.end();++it) {
        graphe &bridge=B[N+i];
        make_induced_subgraph(*it,bridge,false);
        // add edges connecting the component with embedding to bridge
        for (ivector::const_iterator jt=it->begin();jt!=it->end();++jt) {
            adj=adjacent_nodes(*jt);
            gen v=node(*jt);
            for (ivector::const_iterator kt=adj.begin();kt!=adj.end();++kt) {
                if (embedding[*kt])
                    bridge.add_edge(v,node(*kt));
            }
        }
        ++i;
    }
    // create bridges from edges in v
    int m=components.size(),n=v.size();
    for (i=0;i<n;++i) {
        const ipair &p=v[i];
        B[N+m+i].add_edge(node(p.first),node(p.second));
    }
    // This is a fix of the Gibbons' implementation of Demoucron algorithm:
    // combine bridges with the same two points of contact.
    // See the paper "Errors in graph embedding algorithms" by W.Myrvold et.al for details.
    vector<bool> is_joined(m+n,false);
    vector<ivector> C(m+n);
    for (i=0;i<m+n;++i) {
        bridge_contact_vertices(B[N+i],embedding,C[i]);
    }
    for (i=0;i<m+n;++i) {
        if (is_joined[i])
            continue;
        graphe &bridge=B[N+i];
        ivector &ci=C[i];
        if (C[i].size()==2) {
            for (int j=i+1;j<m+n;++j) {
                if (is_joined[j])
                    continue;
                ivector &cj=C[j];
                if (cj.size()==2 && ci.front()==cj.front() && ci.back()==cj.back()) {
                    bridge.join(B[N+j]);
                    is_joined[j]=true;
                }
            }
        }
    }
    // remove joined bridges
    for (i=m+n-1;i>=0;--i) {
        if (is_joined[i])
            B.erase(B.begin()+N+i);
    }
}

/* finds planar embedding of a biconnected graph as a list of faces, returns true iff the graph is planar */
bool graphe::planar_embedding_block(vector<ivector> &faces) const {
    ivector cycle=find_cycle(); // a cycle in the graph
    vector<bool> embedding(node_count(),false); // planar embedding as a set of vertices
    for (ivector::const_iterator it=cycle.begin();it!=cycle.end();++it) {
        embedding[*it]=true;
    }
    faces=vector<ivector>(2,cycle); // initial two faces, front is always the exterior one
    vector<graphe> B;
    // find bridges
    bridges(embedding,faces,B);
    vector<ivector> F,C;
    int i,j,k,n,f,s;
    while (!B.empty()) {
        // for each bridge, find all faces in which it can be drawn
        i=0;
        F.resize(B.size());
        C.resize(B.size());
        for (vector<graphe>::const_iterator it=B.begin();it!=B.end();++it) {
            bridge_contact_vertices(*it,embedding,C[i]);
            ivector &c=C[i];
            assert(c.size()>1);
            ivector::const_iterator ct;
            int f=0;
            F[i].clear();
            for (vector<ivector>::const_iterator ft=faces.begin();ft!=faces.end();++ft) {
                for (ct=c.begin();ct!=c.end();++ct) {
                    if (find(ft->begin(),ft->end(),*ct)==ft->end())
                        break;
                }
                if (ct==c.end())
                    F[i].push_back(f);
                ++f;
            }
            if (F[i].empty())
                return false; // graph is not planar
            ++i;
        }
        // get i-th bridge such that F[i] is minimal (>=1)
        n=F.front().size();
        k=0;
        for (i=1;i<int(F.size());++i) {
            if ((s=F[i].size())<n) {
                n=s;
                k=i;
            }
        }
        graphe &bridge=B[k];
        graphe G(bridge);
        bridge.copy(G);
        for (vector<ivector>::const_iterator ft=faces.begin();ft!=faces.end();++ft) {
            n=ft->size();
            for (i=0;i<n;++i) {
                G.add_edge(node(ft->at(i)),node(ft->at((i+1)%n)));
            }
        }
        // get the first face in which bridge can be drawn
        f=F[k].front();
        ivector &c=C[k]; // points of contact for the bridge
        // find a path between first two points of contact
        i=bridge.node_index(node(c[0]));
        j=bridge.node_index(node(c[1]));
        ivector path=bridge.find_path(i,j);
        for (ivector::iterator it=path.begin();it!=path.end();++it) {
            embedding[*it=node_index(bridge.node(*it))]=true;
        }
        B.erase(B.begin()+k);
        // draw path to the face f, splitting the latter in two
        ivector &face=faces[f];
        i=find(face.begin(),face.end(),c[0])-face.begin();
        j=find(face.begin(),face.end(),c[1])-face.begin();
        ivector::iterator pt=face.begin()+(i<j?i:j),qt=face.begin()+(i<j?j:i);
        ivector face_part(pt+1,qt);
        if (i>j)
            reverse(path.begin(),path.end());
        face.erase(pt,qt+1);
        face.insert(face.begin()+(i<j?i:j),path.begin(),path.end());
        reverse(face_part.begin(),face_part.end());
        path.insert(path.end(),face_part.begin(),face_part.end());
        faces.push_back(path);
        // add new bridges to B
        vector<bool> G_embedding(G.node_count(),false);
        n=embedding.size();
        for (i=0;i<n;++i) {
            if (embedding[i])
                G_embedding[G.node_index(node(i))]=true;
        }
        vector<ivector> G_faces;
        for (vector<ivector>::const_iterator ft=faces.begin();ft!=faces.end();++ft) {
            G_faces.push_back(translate_indices_to(G,*ft));
        }
        G.bridges(G_embedding,G_faces,B);
    }
    assert(int(faces.size())==edge_count()-node_count()+2);
    return true;
}

/* finds planar embedding of a connected graph as a list of faces, returns true iff the graph is planar */
bool graphe::planar_embedding_connected(vector<ivector> &faces) const {
    if (edge_count()+6>3*node_count())
        return false;
    // split graph to blocks
    vector<vector<ipair> > blocks;
    find_blocks(blocks);
    for (vector<vector<ipair> >::const_iterator it=blocks.begin();it!=blocks.end();++it) {
        // test each block separately
        graphe G;
        for (vector<ipair>::const_iterator jt=it->begin();jt!=it->end();++jt) {
            G.add_edge(node(jt->first),node(jt->second));
        }
        vector<ivector> block_faces;
        if (int(it->size())+6>3*G.node_count() || !G.planar_embedding_block(block_faces))
            return false;
        for (vector<ivector>::const_iterator ft=block_faces.begin();ft!=block_faces.end();++ft) {
            faces.push_back(translate_indices_from(G,*ft));
        }
    }
    return true;
}

/******************************************************************************
 *              A NODE-POSITIONING ALGORITHM FOR GENERAL TREES                *
 ******************************************************************************/

graphe::walker::walker(graphe *g) {
    G=g;
    int n=G->node_count();
    parent=ivector(n,-1);
    first_child=ivector(n,-1);
    left_sibling=ivector(n,-1);
    right_sibling=ivector(n,-1);
    xcoord=vector<double>(n,0);
    ycoord=vector<double>(n,0);
    prelim=vector<double>(n,0);
    modifier=vector<double>(n,0);
    left_neighbor=ivector(n,-1);
    max_depth=RAND_MAX;
}

graphe::walker::~walker() {
    // delete prevnode list
    prevnode *p=level_zero_ptr;
    while (p!=NULL) {
        prevnode *q=p;
        p=p->next_level;
        delete q;
    }
}

/*
 * This function determines the coordinates for each node in a tree. A pointer
 * to the apex node of the tree is passed as input. This assumes that the x and
 * y coordinates of the apex node are set as desired, since the tree underneath
 * it will be positioned with respect to those coordinates. Returns TRUE if no
 * errors, otherwise returns FALSE.
 */
bool graphe::walker::position_tree(int node) {
    if (node!=-1) {
        // initialize the list of previous nodes at each level
        init_prev_node_list();
        // do the preliminary positioning with a postorder walk
        first_walk(node,0);
        // determine how to adjust all the nodes with respect to the location of the root
        x_top_adjustment=xcoord[node]-prelim[node];
        y_top_adjustment=ycoord[node];
        // do the final positioning with preorder walk
        return second_walk(node,0,0);
    }
    // the trivial case
    return true;
}

/* return true iff the node is a leaf, i.e. if there is no node with it as the parent */
bool graphe::walker::is_leaf(int node) {
    for (ivector::const_iterator it=parent.begin();it!=parent.end();++it) {
        if (*it==node)
            return false;
    }
    return true;
}

/*
 * In this first postorder walk, every node of the tree is assigned a
 * preliminary x-coordinate (held in field PRELIM(Node)). In addition, internal
 * nodes are given modifiers, which will be used to move their offspring to the
 * right (held in field MODIFIER(Node)).
 */
void graphe::walker::first_walk(int node, int level) {
    left_neighbor[node]=get_prev_node_at_level(level);
    set_prev_node_at_level(level,node);
    modifier[node]=0;
    int s;
    if (is_leaf(node) || level==max_depth) {
        if ((s=left_sibling[node])>=0) {
            // Determine the preliminary x-coordinate based on:
            // the prelim x-coordinate of the left sibling, the separation
            // between sibling nodes and the mean size of the left sibling
            // and the current node.
            prelim[node]=prelim[s]+sibling_separation+mean_mode_size(s,node);
        } else {
            // no sibling on the left to worry about
            prelim[node]=0;
        }
    } else {
        // this node is not a leaf, so call this procedure recursively for each of its offsprings
        int leftmost=first_child[node],rightmost=leftmost;
        first_walk(leftmost,level+1);
        while (right_sibling[rightmost]>=0) {
            rightmost=right_sibling[rightmost];
            first_walk(rightmost,level+1);
        }
        double midpoint=(prelim[leftmost]+prelim[rightmost])/2.0;
        if ((s=left_sibling[node])>=0) {
            prelim[node]=prelim[s]+sibling_separation+mean_mode_size(s,node);
            modifier[node]=prelim[node]-midpoint;
            apportion(node,level);
        } else
            prelim[node]=midpoint;
    }
}

/*
 * During a second preorder walk, each node is given a final x-coordinate by
 * summing its preliminary x-coordinate and the modifiers of all the node's
 * ancestors. The y-coordinate depends on the height of the tree. If the actual
 * position of an interior node is right of its preliminary place, the subtree
 * rooted at the node must be moved right to center the sons around the father.
 * Rather than immediately readjust all the nodes in the subtree, each node
 * remembers the distance to the provisional place in a modifier field
 * (MODIFIER(Node)). In this second pass down the tree, modifiers are
 * accumulated and applied to every node. Returns TRUE if no errors, otherwise
 * returns FALSE.
 */
bool graphe::walker::second_walk(int node, int level, double modsum) {
    bool result;
    int s;
    if (level<=max_depth) {
        double xtemp=x_top_adjustment+prelim[node]+modsum;
        double ytemp=y_top_adjustment+level*level_separation;
        if (check_extents_range(xtemp,ytemp)) {
            xcoord[node]=xtemp;
            ycoord[node]=ytemp;
            if (!is_leaf(node)) {
                // apply the modifier value for this node to all its offsprings
                bool result=second_walk(first_child[node],level+1,modsum+modifier[node]);
                if (result && (s=right_sibling[node])>=0)
                    result=second_walk(s,level+1,modsum);
            }
        } else // continuing would put the tree outside of the drawable extents range
            result=false;
    } else // we are at a level deeper than what we want to draw
        result=true;
    return result;
}

/*
 * This procedure cleans up the positioning of small sibling subtrees, thus
 * fixing the "left-to-right gluing" problem evident in earlier algorithms. When
 * moving a new subtree farther and farther to the right, gaps may open up among
 * smaller subtrees that were previously sandwiched between larger subtrees.
 * Thus, when moving the new, larger subtree to the right, the distance it is
 * moved is also apportioned to smaller, interior subtrees, creating a pleasing
 * aesthetic placement.
 */
void graphe::walker::apportion(int node, int level) {
    int leftmost=first_child[node];
    int neighbor=left_neighbor[leftmost];
    int compare_depth=1;
    int depth_to_stop=max_depth-level;
    while (leftmost>=0 && neighbor>=0 && compare_depth<=depth_to_stop) {
        // compute the location of leftmost and where it should be w.r.t. neighbor
        double left_modsum=0,right_modsum=0;
        int ancestor_leftmost=leftmost,ancestor_neighbor=neighbor;
        for (int i=0;i<compare_depth;++i) {
            ancestor_leftmost=parent[ancestor_leftmost];
            ancestor_neighbor=parent[ancestor_neighbor];
            right_modsum+=modifier[ancestor_leftmost];
            left_modsum+=modifier[ancestor_neighbor];
        }
        // Find the movedistance and apply it to node's subtree.
        // Add appropriate portions to smaller interior subtrees.
        double movedistance=prelim[neighbor]+left_modsum+subtree_separation+
                mean_mode_size(leftmost,neighbor)-prelim[leftmost]-right_modsum;
        if (movedistance>0) {
            // count interior sibling trees in left_siblings
            int t=node;
            int left_siblings=0;
            while (t>=0 && t!=ancestor_neighbor) {
                left_siblings++;
                t=left_sibling[t];
            }
            if (t>=0) {
                // apply portions to appropriate left_sibling subtrees
                double portion=movedistance/left_siblings;
                t=node;
                while (t==ancestor_neighbor) {
                    prelim[t]+=movedistance;
                    modifier[t]+=movedistance;
                    movedistance-=portion;
                    t=left_sibling[t];
                }
            } else {
                // Don't need to move anything--ancestor_neighbor and ancestor_leftmost
                // are not siblings of each other.
                return;
            }
        }
        // Determine the leftmost descendant of node at the next lower
        // level to compare its positioning against that of the neighbor.
        compare_depth++;
        if (is_leaf(leftmost))
            leftmost=get_leftmost(node,0,compare_depth);
        else
            leftmost=first_child[leftmost];
    }
}

/*
 * This function returns the leftmost descendant of a node at a given Depth.
 * This is implemented using a postorder walk of the subtree under Node, down
 * to the level of Depth. Level here is not the absolute tree level used in the
 * two main tree walks; it refers to the level below the node whose leftmost
 * descendant is being found.
 */
int graphe::walker::get_leftmost(int node, int level, int depth) {
    if (level>=depth)
        return node;
    if (is_leaf(node))
        return -1;
    int s,rightmost=first_child[node];
    int leftmost=get_leftmost(rightmost,level+1,depth);
    // do a postorder walk of the subtree below node
    while (leftmost<0 || (s=right_sibling[rightmost])>=0) {
        leftmost=get_leftmost(s,level+1,depth);
        rightmost=s;
    }
    return leftmost;
}

/*
 * This function returns the mean size of the two passed nodes. It adds the size
 * of the right half of lefthand node to the left half of righthand node. If all
 * nodes are the same size, this is a trivial calculation.
 */
double graphe::walker::mean_mode_size(int left_node, int right_node) {
    double nodesize=0;
    if (left_node>=0)
        nodesize+=right_size[left_node];
    if (right_node>=0)
        nodesize+=left_size[right_node];
    return nodesize;
}

/*
 * This function verifies that the passed x- and y-coordinates are within the
 * coordinate system being used for the drawing.
 */
bool graphe::walker::check_extents_range(double xvalue, double yvalue) {
    // for now, make no restrictions
    return true;
}

/*
 * Initialize the list of previous nodes at each level. Three list-maintenance
 * procedures, GETPREVNODEATLEVEL, SETPREVNODEATLEVEL and INITPREVNODELIST
 * maintain a singly-linked list. Each entry in the list corresponds to the node
 * previous to the current node at a given level (for example, element 2 in the
 * list corresponds to the node to the left of the current node at level 2). If
 * the maximum tree size is known beforehand, this Jist can be replaced with a
 * fixed-size array, and these procedures become trivial. Each list element
 * contains two fields: PREVNODE-the previous node at this level, and
 * NEXTLEVEL-a forward pointer to the next list element. The list is does not
 * need to be cleaned up between calls to POSITIONTREE, for performance.
 */
void graphe::walker::init_prev_node_list() {
    // start with the node at level 0 - the apex of the tree
    prevnode *p=level_zero_ptr;
    while (p!=NULL) {
        p->index=-1;
        p=p->next_level;
    }
}

int graphe::walker::get_prev_node_at_level(int level) {
    // start with the apex of the tree
    prevnode *p=level_zero_ptr;
    int i=0;
    while (p!=NULL) {
        if (i==level)
            return p->index;
        p=p->next_level;
        ++i;
    }
    // there was no node at the specified level
    return -1;
}

void graphe::walker::set_prev_node_at_level(int level, int node) {
    // start with the apex of the tree
    prevnode *p=level_zero_ptr;
    int i=0;
    while (p!=NULL) {
        if (i==level) {
            // at this level, replace the existing list element with the passed-in node
            p->index=node;
            return;
        }
        if (p->next_level==NULL) {
            // There isn't a lfst element yet at this level, so add one
            prevnode *elem=new prevnode();
            elem->index=-1;
            elem->next_level=NULL;
            p->next_level=elem;
        }
        // prepare to move to the next level, to look again
        p=p->next_level;
        ++i;
    }
    // should only get here if level_zero_ptr is NULL
    assert(level_zero_ptr==NULL);
    level_zero_ptr=new prevnode();
    level_zero_ptr->index=node;
    level_zero_ptr->next_level=NULL;
}

int graphe::walker::tree_depth(int node, int depth, int incumbent_depth, vector<bool> &visited) {
    if (depth==incumbent_depth)
        return -1;
    ivector adj=G->adjacent_nodes(node);
    int d=depth,dd;
    visited[node]=true;
    for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
        if (visited[*it])
            continue;
        dd=tree_depth(*it,depth+1,incumbent_depth,visited);
        if (dd<0)
            return -1;
        if (dd>d)
            d=dd;
    }
    return d;
}

/* get a node suitable as an apex of the tree (such that tree depth is minimal) */
int graphe::walker::best_apex() {
    int n=G->node_count();
    int depth=RAND_MAX,d,apex;
    vector<bool> visited(n);
    for (int i=0;i<n;++i) {
        fill(visited.begin(),visited.end(),false);
        d=tree_depth(i,0,depth,visited);
        if (d>=0 && d<depth) {
            depth=d;
            apex=i;
        }
    }
    return apex;
}

/* calculate node positions and save them as attributes to graph G */
bool graphe::walker::calculate_node_positions(double topleft_x, double topleft_y,
                                              double &width, double &height, double sep, int apex) {
    int n=G->node_count();
    if (n==0)
        return true;
    int ap=apex<0?best_apex():apex;
    // set separators to something meaningful
    sibling_separation=sep;
    subtree_separation=sep*PLASTIC_NUMBER;
    level_separation=sep*std::pow(PLASTIC_NUMBER,2);
    uniform_node_size=sep*std::pow(PLASTIC_NUMBER,-5);
    left_size=vector<double>(n,uniform_node_size/2.0);
    right_size=vector<double>(n,uniform_node_size/2.0);
    ycoord[ap]=topleft_y;
    // compute node positions
    if (!position_tree(ap))
        return false;
    // compute width and height
    double x_min=0,x_max=0;
    for (vector<double>::const_iterator it=xcoord.begin();it!=xcoord.end();++it) {
        if (*it<x_min)
            x_min=*it;
        if (*it>x_max)
            x_max=*it;
    }
    width=x_max-x_min;
    height=0;
    for (vector<double>::const_iterator it=ycoord.begin();it!=ycoord.end();++it) {
        if (*it>height)
            height=*it;
    }
    // save node positions as attributes to graph G
    double x,y;
    for (int i=0;i<n;++i) {
        x=topleft_x+xcoord[i]-x_min;
        y=topleft_y-ycoord[i];
        G->set_node_attribute(i,_GT_ATTRIB_POSITION,makevecteur(x,y));
    }
    return true;
}

/*
 * END OF NODE-POSITIONING ALGORITHM
 ******************************************************************************/

bool graphe::tree_node_positions(const dpair &topleft, dpair &dim, double sep, int apex) {
    walker W(this);
    return W.calculate_node_positions(topleft.first,topleft.second,dim.first,dim.second,sep,apex);
}

#ifndef NO_NAMESPACE_GIAC
}
#endif // ndef NO_NAMESPACE_GIAC
