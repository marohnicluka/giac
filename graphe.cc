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
#include <ctime>

using namespace std;

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

#define PLASTIC_NUMBER 1.32471795724474602596090885
#define PURCHASE_TIMEOUT 2.5
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
void graphe::make_default_vertex_labels(vecteur &labels,int n,int n0,GIAC_CONTEXT) {
    int ofs=array_start(contextptr);
    labels.resize(n);
    for (int i=0;i<n;++i) {
        labels[i]=i+ofs+n0;
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
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (neighbor_iter jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
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
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (neighbor_iter jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
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
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (neighbor_iter jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
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
            for (ivector_iter kt=u.begin();kt!=u.end();++kt) {
                dotfile << nodes[*kt].symbol << " ";
            }
            dotfile << "};" << endl;
        }
        for (ivector_iter kt=v.begin();kt!=v.end();++kt) {
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
    set_graph_attribute(_GT_ATTRIB_WEIGHTED,VRAI);
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
void graphe::make_unweighted() {
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        for (map<int,attrib>::iterator nt=it->neighbors.begin();nt!=it->neighbors.end();++nt) {
            remove_attribute(nt->second,_GT_ATTRIB_WEIGHT);
        }
    }
    set_graph_attribute(_GT_ATTRIB_WEIGHTED,FAUX);
}

/* store the underlying graph to G (convert arcs to edges and strip all attributes) */
void graphe::underlying(graphe &G) const {
    G.add_nodes(vertices());
    G.set_directed(false);
    int i=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        const map<int,attrib> &ngh=it->neighbors;
        for (neighbor_iter nt=ngh.begin();nt!=ngh.end();++nt) {
            G.add_edge(i,nt->first);
        }
        ++i;
    }
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
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        G.add_node(it->symbol,it->attributes);
    }
    int i=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (neighbor_iter jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
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
    for (ivector_iter it=adj.begin();it!=adj.end();++it) {
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
    for (ivector_iter it=I.begin();it!=I.end();++it) {
        i=*it;
        if (i<0)
            continue;
        ivector adj=adjacent_nodes(i);
        for (ivector_iter jt=adj.begin();jt!=adj.end();++jt) {
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
void graphe::induce_subgraph(const ivector &vi,graphe &G,bool copy_attrib) const {
    int i,j;
    for (ivector_iter it=vi.begin();it!=vi.end();++it) {
        ivector adj=adjacent_nodes(*it);
        gen v=node(*it);
        const attrib &attri=nodes[*it].attributes;
        if (copy_attrib)
            i=G.add_node(v,attri);
        else
            G.add_node(v);
        for (ivector_iter jt=adj.begin();jt!=adj.end();++jt) {
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
    node_iter vt=nodes.begin()+i;
    neighbor_iter it;
    for (it=vt->neighbors.begin();it!=vt->neighbors.end();++it) {
        res.push_back(it->first);
    }
    int j=0;
    for (node_iter wt=nodes.begin();wt!=(is_directed()?nodes.end():vt);++wt) {
        if (wt==vt)
            continue;
        if (wt->neighbors.find(i)!=wt->neighbors.end())
            res.push_back(j);
        ++j;
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
    ivector_iter pos;
    while (!vu.empty()) {
        imax=vu.front();
        pos=vu.begin();
        for (ivector_iter it=vu.begin();it!=vu.end();++it) {
            if (gain[imax]<gain[*it]) {
                imax=*it;
                pos=it;
            }
        }
        vu.erase(pos);
        vc.push_back(imax);
        adj=adjacent_nodes(imax);
        for (ivector_iter it=adj.begin();it!=adj.end();++it) {
            if ((pos=find(vu.begin(),vu.end(),*it))==vu.end())
                continue;
            vu.erase(pos);
            adjj=adjacent_nodes(*it);
            for (ivector_iter jt=adjj.begin();jt!=adjj.end();++jt) {
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

/* construct edmonds class */
graphe::edmonds::edmonds(graphe *gr) {
    G=gr;
}

/* return the vertex which matches v in matching, return -1 if v is not matched */
int graphe::edmonds::mate(const ipairs &matching, int v) {
    for (ipairs::const_iterator it=matching.begin();it!=matching.end();++it) {
        if (it->first==v)
            return it->second;
        if (it->second==v)
            return it->first;
    }
    return -1;
}

int graphe::edmonds::find_root(int k) {
    map<int,int>::const_iterator it=forest.find(k);
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
        for (ivector_iter it=bit->second.begin();it!=bit->second.end();++it) {
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

bool graphe::edmonds::find_augmenting_path(const ipairs &matching, ivector &path) {
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
            ivector_iter wit=adj.begin();
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
                ivector_iter sit=short_path.begin();
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
                                for (ivector_iter it=bit->second.begin();it!=bit->second.end();++it) {
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

void graphe::edmonds::find_maximum_matching(ipairs &matching) {
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
                ipairs::iterator it=find(matching.begin(),matching.end(),edge);
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
void graphe::maximize_matching(ipairs &matching) {
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
        for (ivector_iter it=adj.begin();it!=adj.end();++it) {
            if (match[*it]==0) {
                match[*it]=v;
                match[v]=*it;
                break;
            }
        }
    }
    ipairs matching;
    for (map<int,int>::const_iterator it=match.begin();it!=match.end();++it) {
        int i=it->first,j=it->second;
        ipair edge=make_pair(i<j?i:j,i<j?j:i);
        if (find(matching.begin(),matching.end(),edge)==matching.end())
            matching.push_back(edge);
    }
    return matching;
}

/* add point b to point a */
void graphe::add_point(point &a, const point &b) {
    int d=a.size();
    assert(int(b.size())==d);
    for (int i=0;i<d;++i) {
        a[i]+=b[i];
    }
}

/* subtract point b from point a */
void graphe::subtract_point(point &a, const point &b) {
    int d=a.size();
    assert(int(b.size())==d);
    for (int i=0;i<d;++i) {
        a[i]-=b[i];
    }
}

/* scale coordinates of point p by factor s */
void graphe::scale_point(point &p, double s) {
    int d=p.size();
    for (int i=0;i<d;++i) {
        p[i]*=s;
    }
}

/* compute v_x*w_y-v_y*w_x */
double graphe::point_vecprod2d(const point &v, const point &w) {
    assert(v.size()==2 && w.size()==2);
    return v[0]*w[1]-v[1]*w[0];
}

/* set all coordinates of point p to zero */
void graphe::clear_point_coords(point &p) {
    for (point::iterator it=p.begin();it!=p.end();++it) {
        *it=0;
    }
}

/* convert point to Giac representation (cplx or point(x,y,[z])) */
gen graphe::point2gen(const point &p, bool vect) {
    /*
    if (p.size()==2)
        return makecomplex(p[0],p[1]);
    */
    vecteur coords;
    for (point::const_iterator it=p.begin();it!=p.end();++it) {
        coords.push_back(*it);
    }
    if (vect)
        return coords;
    return symbolic(at_point,_feuille(coords,context0));
}

/* compute the square root of sum of squares of coordinates of point p */
double graphe::point_displacement(const point &p,bool sqroot) {
    double norm=0,d;
    for (point::const_iterator it=p.begin();it!=p.end();++it) {
        d=*it;
        norm+=d*d;
    }
    return sqroot?std::sqrt(norm):norm;
}

/* copy layout src to dest (both must be initialized first) */
void graphe::copy_layout(const layout &src, layout &dest) {
    layout_iter st=src.begin();
    layout::iterator dt=dest.begin();
    for (;st!=src.end() && dt!=dest.end();++st,++dt) {
        *dt=*st;
    }
}

/* copy point src to dest (both must be initialized first) */
void graphe::copy_point(const point &src, point &dest) {
    point::const_iterator st=src.begin();
    point::iterator dt=dest.begin();
    for (;st!=src.end() && dt!=dest.end();++st,++dt) {
        *dt=*st;
    }
}

/* generate uniformly random point p on the origin-centered sphere with the specified radius */
void graphe::rand_point(point &p, double radius) {
    double R;
    do {
        R=0;
        for (point::iterator it=p.begin();it!=p.end();++it) {
            *it=normal_distribution(random_generator);
            R+=std::pow(*it,2.0);
        }
    } while (R==0);
    scale_point(p,radius/std::pow(R,0.5));
}

/* convert point from cartesian to polar representation (2d only) */
void graphe::point2polar(point &p) {
    int d=p.size();
    assert(d==2);
    double x=p[0],y=p[1];
    p[0]=std::pow(std::pow(x,2.0)+std::pow(y,2.0),0.5);
    p[1]=atan2(y,x);
}

/* translate layout x by dx */
void graphe::translate_layout(layout &x, const point &dx) {
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        add_point(*it,dx);
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
    for (layout_iter it=x.begin();it!=x.end();++it) {
        if (it->at(d)<m)
            m=it->at(d);
    }
    return m;
}

/* compute the center of layout x */
graphe::point graphe::layout_center(const layout &x) {
    assert(!x.empty());
    int d=x.front().size();
    point center(d);
    clear_point_coords(center);
    for (layout_iter it=x.begin();it!=x.end();++it) {
        add_point(center,*it);
    }
    scale_point(center,1.0/x.size());
    return center;
}

/* scale layout x to have the diameter equal to diam */
void graphe::scale_layout(layout &x,double diam) {
    if (x.empty())
        return;
    int d=x.front().size();
    assert(d==2 || d==3);
    point M(d,-DBL_MAX),m(d,DBL_MAX);
    for (layout_iter it=x.begin();it!=x.end();++it) {
        const point &p=*it;
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
        scale_point(*it,s);
    }
}

/* randomize layout x using a Gaussian random variable generator */
void graphe::create_random_layout(layout &x,double K,int d) {
    point p(d);
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        p[0]=normal_distribution(random_generator)*K/3;
        p[1]=normal_distribution(random_generator)*K/3;
        if (d==3)
            p[2]=normal_distribution(random_generator)*K/3;
        *it=p;
    }
}

/* return graph layout obtained by a force-directed algorithm */
void graphe::force_directed(layout &x,double K,double R,double tol,bool ac) {
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
    point f(d),p(d),xi(d);
    layout x0(n);
    ivector adj;
    do {
        copy_layout(x,x0);
        E0=E;
        E=0;
        for (i=0;i<n;++i) {
            copy_point(x[i],xi);
            clear_point_coords(f);
            // compute attractive forces between vertices adjacent to the i-th vertex
            adj=adjacent_nodes(i);
            for (ivector_iter it=adj.begin();it!=adj.end();++it) {
                j=*it;
                copy_point(xi,p);
                subtract_point(p,x[j]);
                scale_point(p,point_displacement(p)/K);
                subtract_point(f,p);
            }
            // compute repulsive forces for all vertices not too far from the i-th vertex
            for (j=0;j<n;++j) {
                if (i==j)
                    continue;
                copy_point(xi,p);
                subtract_point(p,x[j]);
                norm=point_displacement(p,false);
                if (Rsq>0 && norm>Rsq)
                    continue;
                if (norm==0)
                    rand_point(p,norm=t*eps);
                scale_point(p,D/norm);
                add_point(f,p);
            }
            // move the location of the i-th vertex
            norm=point_displacement(f);
            if (norm==0)
                continue;
            if (step<norm)
                scale_point(f,step/norm);
            add_point(x[i],f);
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
        layout_iter xit=x.begin(),x0it=x0.begin();
        for (;xit!=x.end();++xit,++x0it) {
            copy_point(*xit,p);
            subtract_point(p,*x0it);
            norm=point_displacement(p);
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
    for (ivector_iter it=V.begin();it!=V.end();++it) {
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
    for (ivector_iter it=V.begin();it!=V.end();++it) {
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
void graphe::coarsening_mis(const ivector &V,graphe &G,sparsemat &P) const {
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
void graphe::coarsening_ec(const ipairs &M,graphe &G,sparsemat &P) const {
    ivector removed_vertices,V;
    for (ipairs::const_iterator it=M.begin();it!=M.end();++it) {
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
    ipairs M;
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
        point yj(d);
        for (int i=0;i<n;++i) {
            x[i]=point(d,0);
            for (int j=0;j<m;++j) {
                if (sparse_matrix_element(P,i,j,pij)) {
                    copy_point(y[j],yj);
                    scale_point(yj,pij);
                    add_point(x[i],yj);
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

/* layout face as a regular polygon inscribed in circle of radius R */
void graphe::make_regular_polygon(layout &x,const ivector &face,double R) {
    int n=face.size(),v;
    for (int k=0;k<n;++k) {
        v=face[k];
        point &p=x[v];
        p.resize(2);
        p[0]=R*std::cos(2*k*M_PI/n);
        p[1]=R*std::sin(2*k*M_PI/n);
    }
}

/* apply force directed algorithm to this graph (must be triconnected) with the specified outer face,
 * using the algorithm described by Bor Plestenjak in the paper "An Algorithm for Drawing Planar Graphs"
 */
void graphe::planar_force_directed(layout &x,const ivector &face,double tol) {
    int n=node_count(),count=0;
    x.resize(n);
    vector<bool> is_facial(n,false);
    for (int i=0;i<n;++i) {
        if (find(face.begin(),face.end(),i)!=face.end())
            is_facial[i]=true;
    }
    // place facial vertices on the unit circle and all other vertices in the origin
    make_regular_polygon(x,face);
    vector<point> F(n);
    for (int i=0;i<n;++i) {
        F[i].resize(2);
        if (is_facial[i])
            continue;
        point &p=x[i];
        p.resize(2);
        p[0]=p[1]=0;
    }
    // cool the system down through iterations
    double d,d0,max_d0,cool,C=std::sqrt(n/M_PI);
    do {
        ++count;
        // compute the resultant force for each non-facial vertex
        for (int i=0;i<n;++i) {
            if (is_facial[i])
                continue;
            point &f=F[i],&p=x[i];
            clear_point_coords(f);
            ivector adj=adjacent_nodes(i);
            scale_point(f,-adj.size());
            for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
                point &q=x[*it];
                point r(2);
                copy_point(q,r);
                subtract_point(r,p);
                scale_point(r,C*point_displacement(r,false));
                add_point(f,r);
            }
        }
        // move each vertex in direction of the force
        max_d0=0;
        cool=1.0/(C+std::pow(count,1.5)/C);
        for (int i=0;i<n;++i) {
            if (is_facial[i])
                continue;
            point &f=F[i],&p=x[i];
            d=point_displacement(f);
            if (d==0)
                continue;
            d0=std::min(d,cool);
            scale_point(f,d0/d);
            add_point(p,f);
            if (d0>max_d0)
                max_d0=d0;
        }
    } while (max_d0>=tol);
}

/* Tomita recursive algorithm */
void graphe::tomita_recurse(const ivector &R,const ivector &P_orig,const ivector &X_orig,ivectors &cliques) const {
    if (P_orig.empty() && X_orig.empty())
        cliques.push_back(R);
    else {
        ivector P(P_orig),X(X_orig);
        // choose as the pivot element the node with the highest number of neighbors in P, say i-th
        ivector PUX;
        ivector_iter it,jt;
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
void graphe::tomita(ivectors &cliques) const {
    ivector R,X,P(node_count());
    for (int i=0;i<node_count();++i) P[i]=i;
    tomita_recurse(R,P,X,cliques);
}

/* remove i-th node which is assumed to be isolated */
void graphe::remove_isolated_node(int i) {
    ipairs ev;
    int v=0,w;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        for (neighbor_iter jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            w=jt->first;
            ev.push_back(make_edge(v>i?v-1:v,w>i?w-1:w));
        }
        it->neighbors.clear();
        ++v;
    }
    nodes.erase(nodes.begin()+i);
    for (ipairs::const_iterator it=ev.begin();it!=ev.end();++it) {
        add_edge(*it);
    }
}

/* collapse edge {i,j}, leaving j-th node isolated (not connected to any other node) */
void graphe::collapse_edge(int i,int j) {
    ivector adj=adjacent_nodes(j);
    for (ivector_iter it=adj.begin();it!=adj.end();++it) {
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
    for (ivector_iter it=v.begin();it!=v.end();++it) {
        sw[*it]=true;
    }
    ipairs res;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        b=sw[i];
        for (neighbor_iter jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
            if (b || sw[jt->first])
                res.push_back(make_pair(i,jt->first));
        }
        ++i;
    }
    return res;
}

/* make a list of all n-tuples of k integers 0,1,...,k-1 */
void ntupk(graphe::ivectors &v,int n,int k,const graphe::ivector &elem,int i) {
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
    ivectors tuples;
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
        ivectors cliques;
        tomita(cliques);
        ivector isolated_nodes;
        for (ivectors_iter it=cliques.begin();it!=cliques.end();++it) {
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
    ivectors partitions;
    int n0=0;
    vecteur v;
    for (ivector_iter it=partition_sizes.begin();it!=partition_sizes.end();++it) {
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
    // add the outer cycle first
    for (int i=0;i<n;++i) {
        add_edge(i,(i+1)%n);
    }
    // add the inner edges
    for (int i=0;i<n;++i) {
        add_edge(i,i+n);
        add_edge(i+n,(i+k)%n+n);
    }
}

void graphe::make_connected_component(int i, int ci, map<int,int> &indices) const {
    indices[i]=ci;
    ivector adj=adjacent_nodes(i);
    // recurse over unvisited nodes adjacent to i-th node,
    // visiting all vertices of the component eventually
    for (ivector_iter it=adj.begin();it!=adj.end();++it) {
        if (indices[*it]==0)
            make_connected_component(*it,ci,indices);
    }
}

/* find all connected components of an undirected graph and store them */
void graphe::connected_components(ivectors &components) const {
    map<int,int> indices;
    int i=0,ci=0;
    while (i<node_count()) {
        if (indices[i]==0)
            make_connected_component(i,++ci,indices);
        ++i;
    }
    components=ivectors(ci);
    for (map<int,int>::const_iterator it=indices.begin();it!=indices.end();++it) {
        components[it->second-1].push_back(it->first);
    }
}

static map<int,bool> dfs_visited;
static graphe::ivector dfs_parent;

void graphe::find_cut_vertices_dfs(int u,int &t,ivector &disc,ivector &low,vector<bool> &ap) const {
    int children=0;
    dfs_visited[u]=true;
    disc[u]=low[u]=t;
    ++t;
    ivector adj=adjacent_nodes(u);
    bool is_articulation=false;
    for (ivector_iter it=adj.begin();it!=adj.end();++it) {
        int v=*it;
        if (!dfs_visited[v]) {
            dfs_parent[v]=u;
            find_cut_vertices_dfs(v,t,disc,low,ap);
            ++children;
            if (low[v]>=disc[u])
                is_articulation=true;
            low[u]=std::min(low[u],low[v]);
        } else if (v!=dfs_parent[u])
            low[u]=std::min(low[u],disc[v]);
    }
    if ((dfs_parent[u]>=0 && is_articulation) || (dfs_parent[u]<0 && children>1))
        ap[u]=true;
}

/* return list of cut vertices obtained by using depth-first search, complexity O(V+E) */
graphe::ivector graphe::find_cut_vertices() const {
    dfs_parent=ivector(node_count(),-1);
    ivector low(node_count(),-1),disc(node_count(),-1);
    vector<bool> ap(node_count(),false);
    int t=0;
    for (int i=0;i<node_count();++i) {
        if (!dfs_visited[i])
            find_cut_vertices_dfs(i,t,disc,low,ap);
    }
    dfs_visited.clear();
    dfs_parent.clear();
    ivector res;
    for (vector<bool>::const_iterator it=ap.begin();it!=ap.end();++it) {
        if (*it)
            res.push_back(it-ap.begin());
    }
    return res;
}

void graphe::find_blocks_dfs(int v,int u,int &t,ivector &dfi,ivector &p,ipairs &stck,vector<ipairs> &blocks) const {
    dfi[v]=p[v]=t;
    ++t;
    ivector adj=adjacent_nodes(v);
    int w,n;
    ipair edge;
    for (ivector_iter it=adj.begin();it!=adj.end();++it) {
        w=*it;
        edge=make_edge(v,w);
        if (dfi[w]==0) {
            stck.push_back(edge);
            find_blocks_dfs(w,v,t,dfi,p,stck,blocks);
            p[v]=std::min(p[v],p[w]);
            if (p[w]>=dfi[v]) {
                // output biconnected component to 'blocks'
                n=blocks.size();
                blocks.resize(n+1);
                ipairs &block=blocks.back();
                do {
                    block.push_back(stck.back());
                    stck.pop_back();
                } while (block.back()!=edge);
            }
        } else if (dfi[w]<dfi[v] && w!=u) {
            stck.push_back(edge);
            p[v]=std::min(p[v],dfi[w]);
        }
    }
}

/* create list of biconnected components (as lists of edges) of the graph */
void graphe::find_blocks(vector<ipairs> &blocks) const {
    int t=1;
    ipairs stck;
    ivector dfi(node_count(),0);
    ivector p(node_count());
    ivector_iter it;
    while ((it=find(dfi.begin(),dfi.end(),0))!=dfi.end()) {
        find_blocks_dfs(it-dfi.begin(),-1,t,dfi,p,stck,blocks);
    }
}

int graphe::find_cycle_dfs(int v, stack<int> &path, vector<bool> &visited, ivector &parent) const {
    visited[v]=true;
    path.push(v);
    ivector adj=adjacent_nodes(v);
    int w,z,p=parent[v];
    for (ivector_iter it=adj.begin();it!=adj.end();++it) {
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
    for (ivector_iter it=adj.begin();it!=adj.end();++it) {
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
void graphe::get_edges(ipairs &E) const {
    int i=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (neighbor_iter jt=it->neighbors.begin();jt!=it->neighbors.end();++jt) {
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
void graphe::translate_indices_to(const graphe &G,const ivector &indices,ivector &dest) const {
    int n=indices.size(),i=0;
    dest.resize(n);
    for (ivector_iter it=indices.begin();it!=indices.end();++it) {
        dest[i]=G.node_index(node(*it));
        ++i;
    }
}

/* translate indices of vertices in G (a subgraph of this) to match those in this graph */
void graphe::translate_indices_from(const graphe &G,const ivector &indices,ivector &dest) const {
    int n=indices.size(),i=0;
    dest.resize(n);
    for (ivector_iter it=indices.begin();it!=indices.end();++it) {
        dest[i]=node_index(G.node(*it));
        ++i;
    }
}

/* return true iff the given face contains edge {i,j} */
bool face_has_edge(const graphe::ivector &face,int i,int j) {
    int n=face.size(),u,v;
    for (int k=0;k<n;++k) {
        u=face[k];
        v=face[(k+1)%n];
        if (i==std::min(u,v) && j==std::max(u,v))
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
void graphe::bridges(const vector<bool> &embedding,const ivectors &faces,vector<graphe> &B) const {
    ivector rest,adj;
    int i=0,N=B.size();
    for (vector<bool>::const_iterator it=embedding.begin();it!=embedding.end();++it) {
        if (!*it)
            rest.push_back(i);
        ++i;
    }
    ivectors components;
    graphe G;
    if (!rest.empty()) {
        induce_subgraph(rest,G,false);
        G.connected_components(components);
        for (ivectors::iterator ct=components.begin();ct!=components.end();++ct) {
            ivector tmp;
            translate_indices_from(G,*ct,tmp);
            ct->swap(tmp);
        }
    }
    // find non-facial edges between embedded vertices
    ipairs v;
    ivectors_iter it;
    int n=node_count();
    for (i=0;i<n;++i) {
        if (!embedding[i])
            continue;
        for (int j=i+1;j<n;++j) {
            if (!embedding[j] || !has_edge(i,j))
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
    // create bridges from edges in v
    int m=components.size();
    n=v.size();
    for (i=0;i<n;++i) {
        const ipair &p=v[i];
        B[N+m+i].add_edge(node(p.first),node(p.second));
    }
    if (!rest.empty()) {
        // create bridges from components
        i=0;
        for (it=components.begin();it!=components.end();++it) {
            graphe &bridge=B[N+i];
            induce_subgraph(*it,bridge,false);
            // add edges connecting the component with embedding to bridge
            for (ivector_iter jt=it->begin();jt!=it->end();++jt) {
                adj=adjacent_nodes(*jt);
                gen v=node(*jt);
                for (ivector_iter kt=adj.begin();kt!=adj.end();++kt) {
                    if (embedding[*kt])
                        bridge.add_edge(v,node(*kt));
                }
            }
            ++i;
        }
        // a fix of the Gibbons' implementation of Demoucron algorithm:
        // combine bridges with the same two points of contact.
        // See the paper "Errors in graph embedding algorithms" by W.Myrvold et.al for details.
        vector<bool> is_joined(m+n,false);
        ivectors C(m+n);
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
}

/* finds planar embedding of a biconnected graph as a list of faces, returns true iff the graph is planar */
bool graphe::planar_embedding_block(ivectors &faces) const {
    ivector cycle=find_cycle(); // a cycle in the graph
    vector<bool> embedding(node_count(),false); // embedded vertices
    for (ivector_iter it=cycle.begin();it!=cycle.end();++it) {
        embedding[*it]=true;
    }
    faces=ivectors(2,cycle); // initial two faces, front is always the exterior one
    vector<graphe> B;
    // find bridges
    bridges(embedding,faces,B);
    ivectors F,C;
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
            ivector_iter ct;
            int f=0;
            F[i].clear();
            for (ivectors_iter ft=faces.begin();ft!=faces.end();++ft) {
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
        for (ivectors_iter ft=faces.begin();ft!=faces.end();++ft) {
            n=ft->size();
            for (i=0;i<n;++i) {
                G.add_edge(node(ft->at(i)),node(ft->at((i+1)%n)));
            }
        }
        // get the first face in which bridge can be drawn
        f=F[k].front();
        ivector &c=C[k];
        // find a path between the first two points of contact
        i=bridge.node_index(node(c[0]));
        j=bridge.node_index(node(c[1]));
        ivector path=bridge.find_path(i,j);
        for (ivector::iterator it=path.begin();it!=path.end();++it) {
            embedding[*it=node_index(bridge.node(*it))]=true;
        }
        B.erase(B.begin()+k); // we're done with this bridge
        // draw path to the face f, splitting the latter in two with the previously found path
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
        ivectors G_faces(faces.size());
        for (ivectors_iter ft=faces.begin();ft!=faces.end();++ft) {
            translate_indices_to(G,*ft,G_faces[ft-faces.begin()]);
        }
        G.bridges(G_embedding,G_faces,B);
    }
    assert(int(faces.size())==edge_count()-node_count()+2);
    return true;
}

/* return the common element of two sorted lists of integers if there is one, else return -1 */
int graphe::common_element(const ivector &v1,const ivector &v2,int offset) {
    ivector_iter it1=v1.begin()+offset,it2=v2.begin()+offset;
    if (it1==v1.end() || it2==v2.end())
        return -1;
    while (it1!=v1.end() && it2!=v2.end()) {
        if (*it1==*it2)
            return *it1;
        while (*it1<*it2 && it1!=v1.end()) ++it1;
        while (*it2<*it1 && it2!=v2.end()) ++it2;
    }
    return -1;
}

/* recursively build up a tree of blocks connected with articulation points */
void graphe::find_block_neighbors(int i,ivectors &blocks) {
    int n=blocks.size(),c;
    // block structure: [is_visited (0 or 1), parent_index, connecting_vertex, ap_1, ap_2, ..., ap_n]
    ivector &parent=blocks[i];
    parent.front()=1; // mark block as visited
    for (int j=0;j<n;++j) {
        // find children
        ivector &block=blocks[j];
        if (i==j || block.front()==1 || (c=common_element(parent,block,3))==-1)
            continue;
        // block is a child connected in the vertex c
        block.erase(find(block.begin()+3,block.end(),c));
        block[1]=i;
        block[2]=c;
        find_block_neighbors(j,blocks);
    }
}

/* embed children blocks to its parent, for each embedding add one temporary edge */
void graphe::embed_children_blocks(int i,ivectors &block_tree,vector<ivectors> &blocks_faces,ipairs &temp_edges) {
    // find all children of the i-th block
    ivector children;
    for (ivectors_iter it=block_tree.begin();it!=block_tree.end();++it) {
        if (it->at(1)==i)
            children.push_back(it-block_tree.begin());
    }
    if (children.empty())
        return; // this is a leaf node in the tree
    // embed each child to a largest available face of the parent
    ivectors &parent_faces=blocks_faces[i];
    int c,d,pf,cf,k,n;
    ivector::iterator vt,wt;
    for (ivector_iter it=children.begin();it!=children.end();++it) {
        embed_children_blocks(*it,block_tree,blocks_faces,temp_edges);
        ivectors &child_faces=blocks_faces[*it];
        c=block_tree[*it][2]; // articulation vertex connecting child with its parent
        // find the largest parent and child faces that contains c
        pf=cf=-1;
        d=0;
        for (ivectors_iter ft=parent_faces.begin();ft!=parent_faces.end();++ft) {
            const ivector &face=*ft;
            if (find(face.begin(),face.end(),c)!=face.end() && int(face.size())>d) {
                pf=ft-parent_faces.begin();
                d=face.size();
            }
        }
        d=0;
        for (ivectors_iter ft=child_faces.begin();ft!=child_faces.end();++ft) {
            const ivector &face=*ft;
            if (find(face.begin(),face.end(),c)!=face.end() && int(face.size())>d) {
                cf=ft-child_faces.begin();
                d=face.size();
            }
        }
        n=parent_faces.size();
        parent_faces.resize(n+child_faces.size());
        // Add all child faces different than child_face to parent_faces,
        // add a new triangular face obtained by connecting neighbors of c in child and parent,
        // and modify the parent_face by adding child_face edges and removing two edges.
        k=n;
        for (ivectors::iterator ft=child_faces.begin();ft!=child_faces.end();++ft) {
            if (int(ft-child_faces.begin())==cf)
                continue;
            parent_faces[k++].swap(*ft);
        }
        ivector &parent_face=parent_faces[pf],&child_face=child_faces[cf],&new_face=parent_faces.back();
        // get a1, a2 as neighbors of c in parent resp. child face
        vt=find(parent_face.begin(),parent_face.end(),c)+1;
        if (vt==parent_face.end())
            vt=parent_face.begin();
        wt=find(child_face.begin(),child_face.end(),c)+1;
        if (wt==child_face.end())
            wt=child_face.begin();
        add_edge(*vt,*wt);
        temp_edges.push_back(make_edge(*vt,*wt)); // add the temporary edge {a1,a2}
        // construct the new face
        new_face.resize(3);
        new_face[0]=c;
        new_face[1]=*vt;
        new_face[2]=*wt;
        // modify parent_face
        ivector path(child_face.size()-1);
        k=0;
        while (*wt!=c) {
            path[k++]=*(wt++);
            if (wt==child_face.end())
                wt=child_face.begin();
        }
        reverse(path.begin(),path.end());
        parent_face.insert(vt,path.begin(),path.end());
    }
}

/* finds planar embedding of a connected graph as a list of faces, returns true iff the graph is planar */
bool graphe::planar_embedding_connected(ivectors &faces,ipairs &temp_edges) {
    if (edge_count()+6>3*node_count())
        return false;
    // split graph to blocks
    vector<ipairs> blocks;
    find_blocks(blocks);
    if (blocks.size()==1)
        return planar_embedding_block(faces);
    // there exist at least one articulation point
    vector<ivectors> blocks_faces(blocks.size());
    ivector cv=find_cut_vertices();
    int i=0,nf;
    for (vector<ipairs>::const_iterator it=blocks.begin();it!=blocks.end();++it) {
        // test each block separately
        graphe G;
        for (ipairs::const_iterator jt=it->begin();jt!=it->end();++jt) {
            G.add_edge(node(jt->first),node(jt->second));
        }
        ivectors &block_faces=blocks_faces[i++];
        if (G.node_count()>2) {
            // block has three or more vertices
            if (int(it->size())+6>3*G.node_count() || !G.planar_embedding_block(block_faces))
                return false;
        } else {
            // block contains only a single edge
            ivector bin_face(2,0);
            bin_face[1]=1;
            block_faces.push_back(bin_face);
        }
        // push back a vector of articulation points which belong to this component
        ivector tmp_face;
        for (ivectors::iterator ft=block_faces.begin();ft!=block_faces.end();++ft) {
            translate_indices_from(G,*ft,tmp_face);
            ft->swap(tmp_face);
        }
        nf=block_faces.size();
        block_faces.resize(block_faces.size()+1);
        ivector &articulation_points=block_faces.back();
        for (int k=0;k<nf;++k) {
            ivector &face=block_faces[k];
            for (ivector_iter cvit=cv.begin();cvit!=cv.end();++cvit) {
                if (find(face.begin(),face.end(),*cvit)!=face.end())
                    articulation_points.push_back(*cvit);
            }
        }
        assert(!articulation_points.empty());
        sort(articulation_points.begin(),articulation_points.end());
    }
    /* Graph is planar and we have a list of faces for each block.
       Now blocks are embedded into each other, starting from peripheral blocks,
       by adding temporary edges, which need to be deleted after a layout is obtained. */
    // make a tree of blocks
    ivectors block_tree(blocks.size());
    for (ivectors::iterator it=block_tree.begin();it!=block_tree.end();++it) {
        it->push_back(0);
        it->push_back(-1);
        it->push_back(-1);
        ivectors &fv=blocks_faces[it-block_tree.begin()];
        it->insert(it->end(),fv.back().begin(),fv.back().end());
    }
    find_block_neighbors(0,block_tree);
    for (vector<ivectors>::iterator it=blocks_faces.begin();it!=blocks_faces.end();++it) {
        it->pop_back();
    }
    // embed all blocks to the root block by climbing up the tree recursively
    for (ivectors_iter bt=block_tree.begin();bt!=block_tree.end();++bt) {
        if (bt->at(1)<0) {
            // root found
            i=bt-block_tree.begin();
            embed_children_blocks(i,block_tree,blocks_faces,temp_edges);
            ivectors &block_faces=blocks_faces[i];
            faces.resize(block_faces.size());
            for (ivectors::iterator it=block_faces.begin();it!=block_faces.end();++it) {
                faces[it-block_faces.begin()].swap(*it);
            }
            break;
        }
    }
    return true;
}

/* add a vertex to each non-outer face and connect it to all the vertices of the face
 * (total of E-V+2 vertices will be added), returns new index of the outer face */
int graphe::two_dimensional_subdivision(ivectors &faces,int outer) {
    gen &last_node=nodes.back().symbol;
    assert(last_node.is_integer());
    int N=last_node.val,i,m,n;
    ivectors new_faces;
    int new_outer;
    for (ivectors::const_iterator ft=faces.begin();ft!=faces.end();++ft) {
        const ivector &face=*ft;
        if (int(ft-faces.begin())==outer) {
            new_outer=new_faces.size();
            new_faces.push_back(face);
            continue;
        }
        if (face.size()<4) {
            new_faces.push_back(face);
            continue;
        }
        i=add_node(++N);
        m=face.size();
        n=new_faces.size();
        new_faces.resize(n+m);
        for (int k=0;k<m;++k) {
            add_edge(i,face[k]);
            ivector &new_face=new_faces[n+k];
            new_face.resize(3);
            new_face[0]=face[k];
            new_face[1]=face[(k+1)%m];
            new_face[2]=i;
        }
    }
    faces.resize(new_faces.size());
    i=0;
    for (ivectors::iterator ft=new_faces.begin();ft!=new_faces.end();++ft) {
        faces[i++].swap(*ft);
    }
    return new_outer;
}

/*
 * A NODE-POSITIONING ALGORITHM FOR GENERAL TREES *****************************
 */

/* walker class constructor, initialize global variables */
graphe::walker::walker(graphe *g) {
    G=g;
    int n=G->node_count();
    parent=ivector(n);
    first_child=ivector(n);
    left_sibling=ivector(n);
    right_sibling=ivector(n);
    xcoord=vector<double>(n);
    ycoord=vector<double>(n);
    prelim=vector<double>(n);
    modifier=vector<double>(n);
    left_neighbor=ivector(n);
    max_depth=RAND_MAX; // no limit on tree depth
    level_zero_ptr=NULL;
}

/* walker class destructor */
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
    return first_child[node]==-1;
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
    bool result=true;
    int s;
    if (level<=max_depth) {
        double xtemp=x_top_adjustment+prelim[node]+modsum;
        double ytemp=y_top_adjustment+level*level_separation;
        if (check_extents_range(xtemp,ytemp)) {
            xcoord[node]=xtemp;
            ycoord[node]=ytemp;
            if (!is_leaf(node)) // apply the modifier value for this node to all its offsprings
                result=second_walk(first_child[node],level+1,modsum+modifier[node]);
            if (result && (s=right_sibling[node])>=0)
                result=second_walk(s,level,modsum);
        } else // continuing would put the tree outside of the drawable extents range
            result=false;
    }
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
    while (leftmost<0 && (s=right_sibling[rightmost])>=0) {
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
 * the maximum tree size is known beforehand, this list can be replaced with a
 * fixed-size array, and these procedures become trivial. Each list element
 * contains two fields: INDEX-the previous node at this level, and
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
    for (ivector_iter it=adj.begin();it!=adj.end();++it) {
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

/* get a node suitable as an apex of the tree (for which the tree has minimal depth) */
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

/* initialize parent, first_child, left_sibling and right_sibling */
void graphe::walker::init_structure(int node,vector<bool> &visited) {
    visited[node]=true;
    ivector adj=G->adjacent_nodes(node);
    int &fc=first_child[node];
    fc=-1;
    int prev=-1;
    for (ivector_iter it=adj.begin();it!=adj.end();++it) {
        if (visited[*it])
            continue;
        if (fc==-1)
            fc=*it;
        if (prev>=0) {
            left_sibling[*it]=prev;
            right_sibling[prev]=*it;
        }
        parent[*it]=node;
        init_structure(*it,visited);
        prev=*it;
    }
}

/* calculate node positions and save them as attributes to graph G */
bool graphe::walker::calculate_node_positions(double topleft_x, double topleft_y,
                                              double &width, double &height, double sep, int apex) {
    int n=G->node_count();
    if (n==0) // G is empty
        return true;
    // obtain an apex node, if none is given
    int ap=apex<0?best_apex():apex;
    parent[ap]=-1;
    fill(left_sibling.begin(),left_sibling.end(),-1);
    fill(right_sibling.begin(),right_sibling.end(),-1);
    vector<bool> visited(n,false);
    init_structure(ap,visited);
    // set separators
    sibling_separation=sep;
    subtree_separation=sep*PLASTIC_NUMBER; // about 4/3 of sep
    level_separation=sep*std::pow(PLASTIC_NUMBER,2); // about 7/4 of sep
    uniform_node_size=sep*std::pow(PLASTIC_NUMBER,-5); // about 1/4 of sep
    // initialize global variables
    left_size=vector<double>(n,uniform_node_size/2.0);
    right_size=vector<double>(n,uniform_node_size/2.0);
    fill(modifier.begin(),modifier.end(),0);
    fill(prelim.begin(),prelim.end(),0);
    fill(xcoord.begin(),xcoord.end(),0);
    fill(ycoord.begin(),ycoord.end(),0);
    ycoord[ap]=topleft_y;
    // compute node positions, return false on error
    if (!position_tree(ap))
        return false;
    // compute width and height of the drawing
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
    // save each node position as an attribute (vecteur [x,y]) to graph G
    point p(2);
    for (int i=0;i<n;++i) {
        p[0]=topleft_x+xcoord[i]-x_min;
        p[1]=topleft_y-ycoord[i];
        G->set_node_attribute(i,_GT_ATTRIB_POSITION,point2gen(p));
    }
    return true;
}

/*
 * END OF NODE-POSITIONING ALGORITHM ******************************************
 */

bool graphe::tree_node_positions(const dpair &topleft, dpair &dim, double sep, int apex) {
    walker W(this);
    return W.calculate_node_positions(topleft.first,topleft.second,dim.first,dim.second,sep,apex);
}

/* sort rectangles by height */
void graphe::sort_rectangles(vector<rectangle> &rectangles) {
    sort(rectangles.begin(),rectangles.end(),[](const rectangle &r1,const rectangle &r2) {
        return r1.height<r2.height;
    });
}

/* pack rectangles (sorted by height) into an enclosing rectangle with specified dimensions,
 * returns true if embedding has changed */
bool graphe::pack_rectangles(const vector<rectangle> &rectangles,dpairs &embedding,double ew,double eh) {
    vector<rectangle> blanks;
    blanks.push_back(rectangle(0,0,ew,eh));
    vector<rectangle>::const_iterator it;
    double xpos,ypos,w,h,dw,dh;
    int k;
    bool embedding_changed=false;
    while (it!=rectangles.end()) {
        // find the leftmost blank which can hold the rectangle
        k=-1;
        for (int i=0;i<int(blanks.size());++i) {
            if (blanks[i].width>=it->width && blanks[i].height>=it->height && (k==-1 || blanks[i].x<xpos)) {
                k=i;
                xpos=blanks[k].x;
            }
        }
        assert(k>=0);
        // store blank dimensions and position, for splitting
        rectangle &blank=blanks[k];
        ypos=blank.y;
        w=blank.width;
        h=blank.height;
        blanks.erase(blanks.begin()+k); // delete blank in which the rectangle is inserted
        // insert the rectangle (breaking the deleted blank into two or four pieces)
        dpair newpos=make_pair(xpos,ypos);
        dpair &oldpos=embedding[it-rectangles.begin()];
        if (newpos!=oldpos) {
            oldpos=newpos;
            embedding_changed=true;
        }
        // add new (smaller) blanks obtained by splitting the deleted blank
        if ((dw=w-it->width)>0)
            blanks.push_back(rectangle(xpos+it->width,ypos,dw,it->height));
        if ((dh=h-it->height)>0)
            blanks.push_back(rectangle(xpos,ypos+it->height,it->width,dh));
        if (dw>0 && dh>0)
            blanks.push_back(rectangle(xpos+it->width,ypos+it->height,dw,dh));
        // move iterator to the next rectangle and start over
        ++it;
    }
    return embedding_changed;
}

/* pack rectangles (sorted by height) to an enclosing rectangle with minimal perimeter and space wasted */
graphe::dpairs graphe::pack_rectangles_neatly(const vector<rectangle> &rectangles) {
    int n=rectangles.size(),i;
    // compute total area occupied by the rectangles
    double total_area=0;
    for (vector<rectangle>::const_iterator it=rectangles.begin();it!=rectangles.end();++it) {
        total_area+=it->width*it->height;
    }
    // step = the length of a negligible part of the shorter side of the smallest rectangle
    const rectangle &smallest=rectangles.back(),&largest=rectangles.front();
    double step=std::min(smallest.width,smallest.height)*std::pow(PLASTIC_NUMBER,-14);
    double ew=DBL_MAX,eh=largest.height; // initial enclosing rectangle has an "unlimited" width
    double perimeter,best_perimeter=DBL_MAX,d;
    dpairs embedding(n,make_pair(-1,-1)),best_embedding;
    while (ew>largest.width) { // loop breaks after a stacked embedding is obtained
        if (pack_rectangles(rectangles,embedding,ew,eh)) {
            ew=eh=0;
            i=0;
            // find the smallest enclosing rectangle containing embedding
            for (dpairs::const_iterator it=embedding.begin();it!=embedding.end();++it) {
                if ((d=it->first+rectangles[i].width)>ew)
                    ew=d;
                if ((d=it->second+rectangles[i].height)>eh)
                    eh=d;
                ++i;
            }
            // find teh embedding with the smaller perimeter (when scaled by the wasted ratio)
            if ((perimeter=(ew+eh)*ew*eh/total_area)<best_perimeter) {
                best_perimeter=perimeter;
                best_embedding=embedding;
            }
        }
        // increase enclosing rectangle height (rectangles will end up stacked eventually)
        eh+=step;
    }
    return best_embedding;
}

/* return true iff the vertices were successfully relabeled with 'labels' */
bool graphe::relabel_nodes(const vecteur &labels) {
    int n=node_count();
    if (int(labels.size())<n)
        return false;
    int i=0;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->symbol=labels[i++];
    }
    return true;
}

/* return true iff the graph has a valid 2D or 3D layout x */
bool graphe::get_layout(layout &x,int &dim) const {
    int n=node_count();
    x.resize(n);
    gen pos;
    int d;
    dim=0;
    for (int i=0;i<n;++i) {
        if (!get_node_attribute(i,_GT_ATTRIB_POSITION,pos))
            return false;
        point &p=x[i];
        if (pos.type==_VECT || pos.is_symb_of_sommet(at_point)) {
            vecteur &v=pos.type==_VECT?*pos._VECTptr:*pos._SYMBptr->feuille._VECTptr;
            d=v.size();
            if (dim>0 && d!=dim)
                return false;
            dim=d;
            p.resize(d);
            for (int j=0;j<d;++j) {
                gen &g=v[j];
                if (g.type!=_DOUBLE_)
                    return false;
                p[j]=g._DOUBLE_val;
            }
        } else {
            // assume that pos is a complex number
            if (dim==3)
                return false;
            dim=2;
            p.resize(2);
            if (pos.type==_CPLX) {
                gen &real=*pos._CPLXptr,&imag=*(pos._CPLXptr+1);
                if (real.type!=_DOUBLE_ || imag.type!=_DOUBLE_)
                    return false;
                p.front()=real._DOUBLE_val;
                p.back()=imag._DOUBLE_val;
            } else {
                if (pos.type!=_DOUBLE_)
                    return false;
                p.front()=pos._DOUBLE_val;
                p.back()=0;
            }
        }
    }
    return true;
}

/* return positive for counter-clockwise, negative for clockwise and zero for collinear points p1,p2,p3 */
double graphe::ccw(const point &p1, const point &p2, const point &p3) {
    return (p2[0]-p1[0])*(p3[1]-p1[1])-(p2[1]-p1[1])*(p3[0]-p1[0]);
}

/* return true iff the graph is laid out and fill ccw_indices with convex hull vertices
 * in counterclockwise order (Graham scan is used, works only in 2D) */
bool graphe::convex_hull(ivector &ccw_indices,const layout &x) const {
    int N=node_count(),i,j;
    if (N<3)
        return false;
    ivector vp(N);
    i=0;
    for (ivector::iterator it=vp.begin();it!=vp.end();++it) {
        *it=i++;
    }
    // find the point with the lowest y coordinate (or with the lowest x between those with lowest y)
    double ymin=DBL_MAX;
    int i_ymin=-1,n;
    i=0;
    for (layout_iter it=x.begin();it!=x.end();++it) {
        if (it->at(1)==ymin) {
            assert(i_ymin>=0);
            if (it->at(0)<x[i_ymin][0])
                i_ymin=i;
        } else if (it->at(1)<ymin) {
            ymin=it->at(1);
            i_ymin=i;
        }
        ++i;
    }
    const point &lp=x[i_ymin];
    // sort points by angle
    sort(vp.begin(),vp.end(),[x,lp](int i,int j) {
        const point &pt1=x[i],&pt2=x[j];
        if (pt1[1]==pt2[1])
            return pt1[0]<pt2[0];
        double dx1=pt1[0]-lp[0],dx2=pt2[0]-lp[0];
        double dy1=pt1[1]-lp[1],dy2=pt2[1]-lp[1];
        double r=std::sqrt((dx2*dx2+dy2*dy2)/(dx1*dx1+dy1*dy1));
        return dx2<dx1*r;
    });
    // determine the vertices of the convex hull
    ccw_indices.push_back(vp[0]);
    ccw_indices.push_back(vp[1]);
    i=2;
    while (i<=N) {
        n=ccw_indices.size();
        j=vp[i%N];
        if (ccw(x[ccw_indices[n-2]],x[ccw_indices[n-1]],x[j])<=0)
            ccw_indices.pop_back();
        ccw_indices.push_back(j);
        ++i;
    }
    return true;
}

/* return true iff the segments from p to p+r and from q to q+s intersect (compute the intersection point) */
bool graphe::edge_crossing(const point &p,const point &r,const point &q,const point &s, point &crossing) {
    point qq(2),pp(2),rr(2);
    copy_point(p,pp);
    copy_point(q,qq);
    copy_point(r,rr);
    subtract_point(qq,p);
    double vp1=point_vecprod2d(qq,r),vp2=point_vecprod2d(r,s);
    if (vp1==0 && vp2==0) // collinear edges
        return false;
    if (vp1!=0 && vp2==0) // parallel and non-intersecting edges
        return false;
    if (vp2!=0) {
        double t=point_vecprod2d(qq,s)/vp2,u=vp1/vp2;
        if (t>=0 && u>=0 && t<=1 && u<=1) {
            // edges intersect each other, obtain the intersection point
            scale_point(rr,t);
            add_point(pp,rr);
            crossing.resize(2);
            crossing.front()=pp[0];
            crossing.back()=pp[1];
            return true;
        }
    }
    // edges are non-intersecting
    return false;
}

/* return the area enclosed by a convex polygon with vertices v (sorted in counterclockwise order) */
double graphe::convexpoly_area(const layout &x) {
    int n=x.size();
    double a=0;
    for (int i=0;i<n;++i) {
        a+=x[i][0]*x[(i+1)%n][1]-x[i][1]*x[(i+1)%n][0];
    }
    return a/2;
}

/* return the area of subgraph induced by a list of vertices v (layout is required) */
double graphe::subgraph_area(const layout &x,const ivector &v) const {
    ivector ccw_indices;
    layout hull;
    if (v.empty()) {
        // compute the area of the whole graph
        if (!convex_hull(ccw_indices,x))
            return 0;
        for (ivector_iter it=ccw_indices.begin();it!=ccw_indices.end();++it) {
            hull.push_back(x[*it]);
        }
        return convexpoly_area(hull);
    }
    // v defines a subgraph
    graphe G;
    induce_subgraph(v,G);
    return G.subgraph_area(x);
}

/* copy this graph to G with edge crossings promoted to vertices (2D layout is required) */
void graphe::promote_edge_crossings(layout &x) {
    // collect all edges
    ipairs E;
    int i=0,j,i1,j1,i2,j2;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->attributes[_GT_ATTRIB_POSITION]=point2gen(x[i]);
        const map<int,attrib> &ngh=it->neighbors;
        for (neighbor_iter nt=ngh.begin();nt!=ngh.end();++nt) {
            j=nt->first;
            E.push_back(make_pair(i,j));
        }
        ++i;
    }
    // get the last symbol (must be an integer)
    gen &ls=nodes.back().symbol;
    assert(ls.is_integer());
    int N=ls.val;
    // find edge crossings
    point crossing;
    for (ipairs::const_iterator it=E.begin();it!=E.end();++it) {
        for (ipairs::const_iterator jt=it+1;jt!=E.end();++jt) {
            i1=it->first;
            j1=it->second;
            i2=jt->first;
            j2=jt->second;
            if (i1==i2 || j1==j2 || i1==j2 || j1==i2)
                continue;
            point p(2),q(2),r(2),s(2);
            copy_point(x[i1],p);
            copy_point(x[i2],q);
            copy_point(x[j1],r);
            copy_point(x[j2],s);
            subtract_point(r,p);
            subtract_point(s,q);
            if (edge_crossing(p,r,q,s,crossing)) {
                // promote edge crossing to a vertex and update x
                remove_edge(i1,j1);
                remove_edge(i2,j2);
                i=add_node(++N);
                add_edge(i1,i);
                add_edge(j1,i);
                add_edge(i2,i);
                add_edge(j2,i);
                set_node_attribute(i,_GT_ATTRIB_POSITION,point2gen(crossing));
                x.push_back(crossing);
            }
        }
    }
}

/* reflect src around axis ay+bx+c=0 and write the result in dest */
void graphe::point_reflection(const point &src,const point &axis,point &dest) {
    assert(src.size()==2 && axis.size()==3);
    double p=src[0],q=src[1],a=axis[0],b=axis[1],c=axis[2],a2=a*a,b2=b*b,r=a2+b2,s=a2-b2;
    dest.resize(2);
    dest[0]=(p*s-2*b*(a*q+c))/r;
    dest[1]=-(q*s+2*a*(b*p+c))/r;
}

/* return true iff the points p and q coincide with each other within the given tolerance radius */
bool graphe::points_coincide(const point &p,const point &q,double tol) {
    assert(q.size()==p.size());
    point r(p.size());
    copy_point(p,r);
    subtract_point(r,q);
    return point_displacement(r)<=tol;
}

/* obtain Purchase measure of symmetry for this graph when axis goes between vertices v and w */
double graphe::purchase(const layout &x,int v,int w,int orig_node_count,const point &axis,
                        const ipairs &E,vector<double> &sc,double tol) const {
    // compute edge scores
    int i1,j1,i2,j2,cnt=0;
    double s1,s2;
    point ip1,jp1;
    fill(sc.begin(),sc.end(),-1);
    for (ipairs::const_iterator et=E.begin();et!=E.end();++et) {
        i1=et->first;
        j1=et->second;
        point_reflection(x[i1],axis,ip1);
        point_reflection(x[j1],axis,jp1);
        for (ipairs::const_iterator it=et+1;it!=E.end();++it) {
            i2=it->first;
            j2=it->second;
            const point &ip2=x[i2];
            const point &jp2=x[j2];
            s1=s2=0;
            if (points_coincide(ip1,ip2,tol)) {
                s1=(i1<orig_node_count)==(i2<orig_node_count)?1.0:0.5;
                if (points_coincide(jp1,jp2,tol))
                    s2=(j1<orig_node_count)==(j2<orig_node_count)?1.0:0.5;
            } else if (points_coincide(ip1,jp2,tol)) {
                s1=(i1<orig_node_count)==(j2<orig_node_count)?1.0:0.5;
                if (points_coincide(ip2,jp1,tol))
                    s2=(i2<orig_node_count)==(j1<orig_node_count)?1.0:0.5;
            }
            if (s1*s2>0) {
                ++cnt;
                break;
            }
        }
        sc[et-E.begin()]=s1*s2;
    }
    if (cnt<3)
        return 0;
    // construct symmetric subgraph
    graphe G;
    map<ipair,double> edge_score;
    int k=0,i,j;
    for (vector<double>::const_iterator it=sc.begin();it!=sc.end();++it) {
        if (*it>0) {
            const ipair &e=E[k];
            const vertex &V=nodes[e.first],&W=nodes[e.second];
            i=G.add_node(V.symbol,V.attributes);
            j=G.add_node(W.symbol,W.attributes);
            G.add_edge(i,j);
            edge_score[G.make_edge(i,j)]=*it;
        }
        ++k;
    }
    // obtain connected components and their average scores
    ivectors components;
    G.connected_components(components);
    vector<double> area(components.size());
    double score=0;
    for (ivectors_iter it=components.begin();it!=components.end();++it) {
        double avg_score=0;
        int n=0;
        for (ivector_iter ct=it->begin();ct!=it->end();++ct) {
            for (ivector_iter dt=ct+1;dt!=it->end();++dt) {
                if (!G.has_edge(*ct,*dt))
                    continue;
                avg_score+=edge_score[G.make_edge(*ct,*dt)];
                ++n;
            }
        }
        score+=(area[it-components.begin()]=G.subgraph_area(x,*it))*avg_score/n;
    }
    // compute and return Purchase score
    double total_area=0;
    for (vector<double>::const_iterator it=area.begin();it!=area.end();++it) {
        total_area+=*it;
    }
    return score/std::max(total_area,subgraph_area(x));
}

/* obtain perpendicular bisector of the vertices v and w */
void graphe::bisector(int v,int w,const layout &x,point &bsec) {
    const point &vp=x[v],&wp=x[w];
    double x1=vp[0],x2=wp[0],y1=vp[1],y2=wp[1];
    bsec[0]=2*(y2-y1);
    bsec[1]=2*(x2-x1);
    bsec[2]=x1*x1+y1*y1-(x2*x2+y2*y2);
}

/* return squared distance between point p=(x0,y0) and line ay+bx+c=0 */
double graphe::squared_distance(const point &p, const point &line) {
    assert(p.size()==2 && line.size()==3);
    double num=line[0]*p[0]+line[1]*p[1]+line[2];
    double den=line[0]*line[0]+line[1]*line[1];
    return num*num/den;
}

/* return the best axis of symmetry for this graph (layout is required) */
graphe::point graphe::axis_of_symmetry(layout &x) const {
    // find graph diameter
    point p(2);
    point center=layout_center(x);
    double diam=0,d;
    for (layout_iter it=x.begin();it!=x.end();++it) {
        for (layout_iter jt=it+1;jt!=x.end();++jt) {
            copy_point(*it,p);
            subtract_point(p,*jt);
            d=point_displacement(p,false);
            if (d>diam)
                diam=d;
        }
    }
    diam=std::sqrt(diam);
    // define the maximum negligible displacement of a node
    double tol=diam*std::pow(PLASTIC_NUMBER,-14);
    // promote edge crossings
    graphe G;
    underlying(G);
    vecteur labels;
    int nc=G.node_count();
    make_default_vertex_labels(labels,nc,0,context0);
    G.relabel_nodes(labels);
    G.promote_edge_crossings(x);
    ipairs E;
    G.get_edges(E);
    // obtain the best axis by computing Purchase score for each pair of vertices
    vector<pair<ipair,point> > axes;
    int n=G.node_count();
    for (int v=0;v<n;++v) {
        for (int w=v+1;w<n;++w) {
            point axis(3);
            bisector(v,w,x,axis);
            axes.push_back(make_pair(make_pair(v,w),axis));
        }
    }
    sort(axes.begin(),axes.end(),[x,center](const pair<ipair,point> &p1,const pair<ipair,point> &p2) {
        return squared_distance(center,p1.second)<squared_distance(center,p2.second);
    });
    point best_axis(3,0);
    vector<double> sc(E.size());
    double best_score=0,score,d0=-1;
    clock_t start;
    double duration;
    start=clock();
    for (vector<pair<ipair,point> >::const_iterator it=axes.begin();it!=axes.end();++it) {
        const point &axis=it->second;
        const ipair &P=it->first;
        d=std::sqrt(squared_distance(center,axis));
        if (d0<0)
            d0=d;
        else if (d>d0+tol)
            break;
        score=G.purchase(x,P.first,P.second,nc,axis,E,sc,tol);
        if (score>best_score) {
            best_score=score;
            copy_point(axis,best_axis);

        }
        duration=(clock()-start)/(double)CLOCKS_PER_SEC;
        if (duration>PURCHASE_TIMEOUT)
            break;
    }
    x.resize(nc);
    return best_axis;
}

/* make planar layout */
bool graphe::make_planar_layout(layout &x,double K) const {
    graphe G;
    underlying(G);
    ivectors faces;
    ipairs temp_edges;
    if (!G.planar_embedding_connected(faces,temp_edges))
        return false;
    int lf=-1;
    for (ivectors_iter it=faces.begin();it!=faces.end();++it) {
        if (lf==-1 || faces[lf].size()<it->size())
            lf=int(it-faces.begin());
    }
    vecteur labels;
    make_default_vertex_labels(labels,node_count(),0,context0);
    G.relabel_nodes(labels);
    lf=G.two_dimensional_subdivision(faces,lf);
    G.planar_force_directed(x,faces[lf]);
    x.resize(node_count());
    scale_layout(x,K);
    return true;
}

/* compute node positions and store them as POSITION attributes (and also return the layout) */
graphe::layout graphe::make_layout(double K,gt_layout_style style,const ivector &face) {
    int dim=2;
    layout x;
    switch (style) {
    case _GT_STYLE_3D:
        dim=3;
    case _GT_STYLE_SPRING:
        multilevel_force_directed(x,dim,K);
        break;
    case _GT_STYLE_PLANAR:
        if (!make_planar_layout(x,K))
            return layout(0);
        break;
    case _GT_STYLE_CIRCLE:

        break;
    }
    // center the layout in origin
    point c=layout_center(x);
    scale_point(c,-1);
    translate_layout(x,c);
    if (dim==2) {
        // Find the best symmetry axis, and rotate the graph
        // to make the symmetry more prominent to a human.
        point axis=axis_of_symmetry(x);
        double a=axis[0],b=axis[1];
        if (a!=0) {
            double phi=M_PI_2+std::atan(b/a);
            // convert layout to polar coordinates and rotate it
            for (layout::iterator it=x.begin();it!=x.end();++it) {
                point2polar(*it);
            }
            rotate_layout(x,phi);
            // determine the highest and the lowest y coordinate: if yh+yl<0, rotate layout for 180°
            double yh=0,yl=0;
            for (layout::const_iterator it=x.begin();it!=x.end();++it) {
                double y=it->front()*std::sin(it->back());
                if (y>yh)
                    yh=y;
                else if (y<yl)
                    yl=y;
            }
            if (yh+yl<0)
                rotate_layout(x,M_PI);
            // convert layout back to cartesian coordinates
            for (layout::iterator it=x.begin();it!=x.end();++it) {
                point &p=*it;
                double r=p[0],phi=p[1];
                p[0]=r*std::cos(phi);
                p[1]=r*std::sin(phi);
            }
        }
    }
    return x;
}

/* customize th Giac display */
gen customize_display(int options) {
    return symbolic(at_equal,makesequence(at_display,change_subtype(options,_INT_COLOR)));
}

/* append the line segment [p,q] to vecteur v */
void graphe::append_segment(vecteur &v,const point &p,const point &q,int color,int width,bool arrow) {
    gen P=point2gen(p),Q=point2gen(q);
    v.push_back(symbolic(arrow?at_vector:at_segment,makesequence(P,Q,customize_display(color | width))));
}

/* append the vertex (as a circle) to vecteur v */
void graphe::append_vertex(vecteur &v, const point &p, int color, int width) {
    gen P=point2gen(p,true);
    v.push_back(symbolic(at_point,makesequence(P,customize_display(color | width | _POINT_POINT))));
}

/* append label to vecteur v at the specified quadrant */
void graphe::append_label(vecteur &v, const point &p, const gen &label, int quadrant) {
    gen P=point2gen(p);
    v.push_back(symbolic(at_legende,makesequence(P,label,customize_display(quadrant))));
}

/* append line segments representing edges (arcs) of the graph to vecteur v */
void graphe::draw_edges(vecteur &v,const layout &x) const {
    if (x.size()<2)
        return;
    attrib_iter ait;
    int i=0,j;
    int color,width=x.front().size()==2?_LINE_WIDTH_2:_LINE_WIDTH_1;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        const point &p=x[i];
        for (neighbor_iter nt=it->neighbors.begin();nt!=it->neighbors.end();++nt) {
            j=nt->first;
            color=x.front().size()==2?_BLUE:_BLACK;
            if ((ait=nt->second.find(_GT_ATTRIB_COLOR))!=nt->second.end())
                color=ait->second.val;
            const point &q=x[j];
            if (is_directed()) {
                point c(p.size());
                copy_point(p,c);
                add_point(c,q);
                scale_point(c,0.5);
                append_segment(v,p,c,color,width,true);
                append_segment(v,c,q,color,width,false);
            } else
                append_segment(v,p,q,color,width);
        }
        ++i;
    }
}

/* append points representing vertices of the graph to vecteur v */
void graphe::draw_nodes(vecteur &v, const layout &x) const {
    if (x.empty())
        return;
    int color,width,n=node_count();
    gen val;
    if (n<=15)
        width=_POINT_WIDTH_4;
    else if (n<=40)
        width=_POINT_WIDTH_3;
    else if (n<=107)
        width=_POINT_WIDTH_2;
    else
        width=_POINT_WIDTH_1;
    for (int i=0;i<n;++i) {
        const point &p=x[i];
        color=_YELLOW;
        if (get_node_attribute(i,_GT_ATTRIB_COLOR,val))
            color=val.val;
        append_vertex(v,p,color,width);
    }
}

/* return the best quadrant for the placement of the i-th vertex label
 * (minimize collision with the adjacent edges) */
int graphe::node_label_best_quadrant(const layout &x,const point &center,int i) const {
    ivector adj=adjacent_nodes(i);
    int n=adj.size();
    vector<double> adj_phi(n);
    const point &p=x[i];
    for (int j=0;j<n;++j) {
        const point &q=x[adj[j]];
        adj_phi[j]=std::atan2(q[1]-p[1],q[0]-p[0]);
    }
    sort(adj_phi.begin(),adj_phi.end());
    double bisector[]={M_PI/4,3*M_PI/4,-3*M_PI/4,-M_PI/4},score[4];
    for (int quadrant=0;quadrant<4;++quadrant) {
        double b=bisector[quadrant];
        int j=0;
        for (;j<n;++j) {
            if (adj_phi[j]>b)
                break;
        }
        double phi1=adj_phi[j==0?n-1:j-1],phi2=adj_phi[j%n];
        score[quadrant]=std::min(std::min(std::abs(b-phi1),std::abs(b-phi2)),M_PI/4);
    }
    ivector candidates;
    double best_score=-1,s;
    for (int quadrant=0;quadrant<4;++quadrant) {
        s=score[quadrant];
        if (s>=best_score) {
            if (s>best_score) {
                best_score=s;
                candidates.clear();
            }
            candidates.push_back(quadrant);
        }
    }
    assert(!candidates.empty());
    int best_quadrant;
    if (candidates.size()==1)
        best_quadrant=candidates.front();
    else {
        double dist=0,d;
        double phi0=std::atan2(center[1]-p[1],center[0]-p[0]);
        for (ivector::const_iterator it=candidates.begin();it!=candidates.end();++it) {
            int phi=bisector[*it];
            d=std::min(std::abs(phi-phi0),std::abs(std::abs(phi-phi0)-2*M_PI));
            if (d>dist) {
                dist=d;
                best_quadrant=*it;
            }
        }
    }
    switch (best_quadrant) {
    case 0:
        return _QUADRANT1;
    case 1:
        return _QUADRANT2;
    case 2:
        return _QUADRANT3;
    case 3:
        return _QUADRANT4;
    default:
        break;
    }
    // unreachable
    return -1;
}

/* append labels of the nodes of this graph to vecteur v */
void graphe::draw_labels(vecteur &v,const layout &x) const {
    int quadrant;
    point center=layout_center(x);
    for (int i=0;i<node_count();++i) {
        const point &p=x[i];
        quadrant=node_label_best_quadrant(x,center,i);
        append_label(v,p,nodes[i].symbol,quadrant);
    }
}

/* extract the largest leading cycle from graph (first k vertices) */
void graphe::get_leading_cycle(ivector &c) const {
    for (int i=0;i<node_count();++i) {
        if (i==0 || has_edge(i,i-1)) {
            c.push_back(i);
        } else break;
    }
}

#ifndef NO_NAMESPACE_GIAC
}
#endif // ndef NO_NAMESPACE_GIAC
