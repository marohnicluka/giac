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

#include "graphtheory.h"
#include <giac/giac.h>
#include <sstream>
#include <ctype.h>
#include <bitset>
#include <math.h>

using namespace std;

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

#define PLASTIC_NUMBER 1.32471795724474602596090885

/* error messages */
static const char *gt_error_messages[] = {
    "unknown error"                                                     //  0
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
const gen graphe::VRAI=gen(1).change_subtype(_INT_BOOLEAN);
const gen graphe::FAUX=gen(0).change_subtype(_INT_BOOLEAN);
default_random_engine graphe::random_generator;
normal_distribution<double> graphe::normal_distribution(0.0,1.0);
uniform_real_distribution<double> graphe::uniform_distribution(0.0,1.0);

/* returns vector of all nonnegative integers smaller than n and not in v */
vector<int> range_complement(const vector<int> &v,int n) {
    int k=0,l=0,m=v.size();
    if (n==m)
        return vector<int>(0);
    assert(n>m);
    vector<int> res(n-m),vs(v);
    sort(vs.begin(),vs.end());
    for (int i=0;i<n;++i) {
        if (vs[k]!=i)
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

/* returns true iff g is graph, also sets basic info to display_str */
bool is_graphe(const gen &g,string &disp_out,GIAC_CONTEXT) {
    if (g.type!=_VECT)
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
void make_default_vertex_labels(vecteur &V,int n,int n0,GIAC_CONTEXT) {
    int ofs=array_start(contextptr);
    V.resize(n);
    for (int i=0;i<n;++i) {
        V[i]=i+ofs+n0;
    }
}

/* create identifier */
gen make_idnt(const char* name,int index=-1,bool intern=true) {
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

/* display error message */
void gt_err(int code,GIAC_CONTEXT) {
    *logptr(contextptr) << "Error: " << gt_error_messages[code] << endl;
}

/* display error message */
void gt_err(const gen &g,int code,GIAC_CONTEXT) {
    *logptr(contextptr) << "Error: " << g << " " << gt_error_messages[code] << endl;
}

/* graphe default constructor */
graphe::graphe() {
    set_graph_attribute(_GT_ATTRIB_DIRECTED,FAUX);
    set_graph_attribute(_GT_ATTRIB_WEIGHTED,FAUX);
}

/* graphe constructor, inherits basic properties of G */
graphe::graphe(const graphe &G) {
    set_graph_attribute(_GT_ATTRIB_DIRECTED,boole(G.is_directed()));
    set_graph_attribute(_GT_ATTRIB_WEIGHTED,boole(G.is_weighted()));
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
    return gv;
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
graphe::ipair graphe::make_edge(const vecteur &v,GIAC_CONTEXT) const {
    assert(v.size()==2 && v.front().is_integer() && v.back().is_integer());
    int ofs=array_start(contextptr);
    int i=v.front().val-ofs,j=v.back().val-ofs;
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

/* make a copy of this graph and store it in G = graphe(this) */
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
bool graphe::add_edge(int i, int j) {
    if (i<0 || i>=node_count() || j<0 || j>=node_count() || has_edge(i,j))
        return false;
    ipair edge=make_edge(i,j);
    return nodes[edge.first].neighbors.insert(make_pair(edge.second,attrib())).second;
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
graphe::ipair graphe::add_edge(const gen &v, const gen &w) {
    int i=add_node(v),j=add_node(w);
    ipair edge=make_edge(i,j);
    add_edge(edge);
    return edge;
}

/* remove edge {v,w} or arc [v,w] */
bool graphe::remove_edge(int i, int j) {
    if (!has_edge(i,j))
        return false;
    ipair edge=make_edge(i,j);
    map<int,attrib>::iterator it;
    it=nodes[edge.first].neighbors.find(edge.second);
    nodes[edge.first].neighbors.erase(it);
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

bool graphe::parse_list_of_edges(const vecteur &v,GIAC_CONTEXT) {
    int st=-2;
    bool hasweight;
    ipair c;
    for (const_iterateur it=v.begin();it!=v.end();++it) {
        if (it->type!=_VECT || int(it->_VECTptr->size())<2) {
            gt_err(*it,_GT_ERR_INVALID_EDGE,contextptr);
            return false;
        }
        vecteur &e = *it->_VECTptr;
        hasweight=e.front().type==_VECT;
        gen edge(hasweight?e.front():*it);
        if (int(edge._VECTptr->size())!=2)
            gt_err(edge,_GT_ERR_INVALID_EDGE,contextptr);
        if (st==-2)
            st=edge.subtype;
        else if (st!=edge.subtype) {
            gt_err(_GT_ERR_INVALID_EDGE_ARC_MIX,contextptr);
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

void graphe::make_induced_subgraph(const ivector &vi, graphe &G) const {
    for (ivector::const_iterator it=vi.begin();it!=vi.end();++it) {
        G.add_node(node(*it),nodes[*it].attributes);
    }
    int i=0,j;
    neighbor_iter nt;
    for (ivector::const_iterator it=vi.begin();it!=vi.end();++it) {
        const map<int,attrib> &ngh=nodes[*it].neighbors;
        j=0;
        for (ivector::const_iterator jt=vi.begin();jt!=vi.end();++jt) {
            if (it==jt)
                continue;
            if ((nt=ngh.find(*jt))!=ngh.end())
                G.add_edge(i,j,nt->second);
            ++j;
        }
        ++i;
    }
}

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

void graphe::print_sparsemat(const sparsemat &A) {
    for (sparsemat::const_iterator it=A.begin();it!=A.end();++it) {
        cout << "Row " << it->first << " contains ";
        for (map<int,double>::const_iterator jt=it->second.begin();jt!=it->second.end();++jt) {
            cout << jt->first << ":" << jt->second << ", ";
        }
        cout << endl;
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

/* collapse edge {i,j} in undirected graph */
void graphe::collapse_edge(int v,int w) {
    int i=v<w?v:w,j=v<w?w:v;
    ivector adj=adjacent_nodes(j);
    for (ivector::const_iterator it=adj.begin();it!=adj.end();++it) {
        remove_edge(*it,j);
        if (*it!=i)
            add_edge(*it,i);
    }
}

/* make a list of all n-tuples of k integers 0,1,...,k-1 */
void ntupk(vector<vector<int> > &v,int n,int k,const vector<int> &elem,int i) {
    for (int j=0;j<k;++j) {
        vector<int> e(elem);
        e[i]=j;
        if (i<n-1)
            ntupk(v,n,k,e,i+1);
        else
            v.push_back(e);
    }
}

/* return true iff the vertices u and v of sierpinski graph are adjacent */
bool is_sierpinski_match(const vector<int> &u,const vector<int> &v,int n,int h) {
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
    vector<vector<int> > tuples;
    vector<int> elem(n,0);
    ntupk(tuples,n,k,elem,0);
    int N=std::pow(k,n);
    vecteur V;
    make_default_vertex_labels(V,N,0,contextptr);
    add_nodes(V);
    for (int i=0;i<N;++i) {
        vector<int> &u=tuples[i];
        for (int j=i+1;j<N;++j) {
            vector<int> &v=tuples[j];
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
                isolated_nodes.push_back(v<w?w:v);
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

/* return list of biconnected components (as lists of edges) of the graph */
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
            if (!G.is_directed() && m!=mtran(m)) {
                gt_err(_GT_ERR_MATRIX_NOT_SYMMETRIC,contextptr);
                return gentypeerr(contextptr);
            }
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
    if (!G.read_gen(*gr._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
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
        gt_err(_GT_ERR_READING_FAILED,contextptr);
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
    if (!G.read_gen(*g._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    return G.vertices();
}
static const char _vertices_s []="vertices";
static define_unary_function_eval(__vertices,&_vertices,_vertices_s);
define_unary_function_ptr5(at_vecrtices,alias_at_vertices,&__vertices,0,true)

gen _number_of_vertices(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    return G.node_count();
}
static const char _number_of_vertices_s []="number_of_vertices";
static define_unary_function_eval(__number_of_vertices,&_number_of_vertices,_number_of_vertices_s);
define_unary_function_ptr5(at_number_of_vertices,alias_at_number_of_vertices,&__number_of_vertices,0,true)

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
    } else if (!G.read_gen(*g._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    if (include_weights && !G.is_weighted()) {
        gt_err(_GT_ERR_WEIGHTED_GRAPH_REQUIRED,contextptr);
        return gentypeerr(contextptr);
    }
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
    if (!G.read_gen(*gv.front()._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
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
    if (!G.read_gen(*gv.front()._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
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
    if (!G.read_gen(*g._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    int n;
    if ((n=G.node_count())==0)
        return vecteur(0);
    matrice am=*_matrix(makesequence(n,n,0),contextptr)._VECTptr;
    for (int i=0;i<n;++i) {
        for (int j=0;j<n;++j) {
            if (G.has_edge(i,j))
                am[i]._VECTptr->at(j)=gen(1);
        }
    }
    return am;
}
static const char _adjacency_matrix_s []="adjacency_matrix";
static define_unary_function_eval(__adjacency_matrix,&_adjacency_matrix,_adjacency_matrix_s);
define_unary_function_ptr5(at_adjacency_matrix,alias_at_adjacency_matrix,&__adjacency_matrix,0,true)

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
    if (!G.read_gen(*g._VECTptr,contextptr) || !G.is_weighted()) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    int n;
    if ((n=G.node_count())==0)
        return vecteur(0);
    matrice wm=*_matrix(makesequence(n,n,0),contextptr)._VECTptr;
    gen w;
    for (int i=0;i<n;++i) {
        for (int j=0;j<0;++j) {
            if (G.has_edge(i,j)) {
                G.get_edge_attribute(i,j,_GT_ATTRIB_WEIGHT,w);
                wm[i]._VECTptr->at(j)=w;
            }
        }
    }
    return wm;
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
    if (!G.read_gen(*g._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
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
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    graphe S(G);
    for (const_iterateur it=ev.begin();it!=ev.end();++it) {
        if (it->type!=_VECT || int(it->_VECTptr->size())!=2 ||
                !it->_VECTptr->front().is_integer() || !it->_VECTptr->back().is_integer())
            return gentypeerr(contextptr);
        edge=S.make_edge(*it->_VECTptr,contextptr);
        if (!G.has_edge(edge)) {
            gt_err(_GT_ERR_EDGE_NOT_FOUND,contextptr);
            return gentypeerr(contextptr);
        }
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
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    int i=G.node_index(g._VECTptr->at(1));
    if (i==-1) {
        gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        return gentypeerr(contextptr);
    }
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
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    if (!G.is_directed()) {
        gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
        return gentypeerr(contextptr);
    }
    int i=G.node_index(g._VECTptr->at(1));
    if (i==-1) {
        gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        return gentypeerr(contextptr);
    }
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
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    if (!G.is_directed()) {
        gt_err(_GT_ERR_DIRECTED_GRAPH_REQUIRED,contextptr);
        return gentypeerr(contextptr);
    }
    int i=G.node_index(g._VECTptr->at(1));
    if (i==-1) {
        gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
        return gentypeerr(contextptr);
    }
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
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    vecteur &V=*g._VECTptr->back()._VECTptr;
    int i=0,index;
    vector<int> vi(V.size());
    for (const_iterateur it=V.begin();it!=V.end();++it) {
        index=G.node_index(*it);
        if (index==-1) {
            gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
            return gentypeerr(contextptr);
        }
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
    if (!G.read_gen(*g._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
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
    if (!G.read_gen(*g._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    if (G.is_directed()) {
        gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
        return gentypeerr(contextptr);
    }
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
    vecteur &gv=g.subtype==_SEQ__VECT?*g._VECTptr->front()._VECTptr:*g._VECTptr;
    if (!G.read_gen(gv,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    if (G.is_directed())
        return gv;
    G.set_directed(true);
    int n=G.node_count();
    for (int i=0;i<n;++i) {
        for (int j=i+1;j<n;++j) {
            if (G.has_edge(i,j))
                G.add_edge(j,i);
        }
    }
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
    if (!G.read_gen(*g._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
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
        make_default_vertex_labels(V,g.val,0,contextptr);
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
    make_default_vertex_labels(V,n,0,contextptr);
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
    if (!G.read_gen(*g._VECTptr->front()._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    if (G.is_directed()) {
        gt_err(_GT_ERR_UNDIRECTED_GRAPH_REQUIRED,contextptr);
        return gentypeerr(contextptr);
    }
    if (G.is_weighted()) {
        gt_err(_GT_ERR_UNWEIGHTED_GRAPH_REQUIRED,contextptr);
        return gentypeerr(contextptr);
    }
    int n=G.node_count();
    vecteur &V=*g._VECTptr->back()._VECTptr;
    vector<bool> vb(n,false);
    for (const_iterateur it=V.begin();it!=V.end();++it) {
        int index=G.node_index(*it);
        if (index==-1) {
            gt_err(_GT_ERR_VERTEX_NOT_FOUND,contextptr);
            return gentypeerr(contextptr);
        }
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

gen node_disp(int color,int width,int quadrant) {
    int d = _POINT_POINT | width | color | quadrant;
    return symbolic(at_equal,makesequence(at_display,change_subtype(d,_INT_COLOR)));
}

gen node_label(const gen &g) {
    return symbolic(at_equal,makesequence(at_legende,g));
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
    if (!G.read_gen(has_opts?*g._VECTptr->front()._VECTptr:*g._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
    if (has_opts) {
        // parse options
        for (const_iterateur it=g._VECTptr->begin()+1;it!=g._VECTptr->end();++it) {
            ;
        }
    }
    int n=G.node_count();
    graphe::layout x;
    int d=2,q;
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
    else if (n<130)
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
                q=_QUADRANT1;
                break;
            case 2:
                q=_QUADRANT2;
                break;
            case 3:
                q=_QUADRANT3;
                break;
            case 4:
                q=_QUADRANT4;
                break;
            default:
                assert(false);
            }
        }
        p1=graphe::rvec2gen(x[i]);
        gen S=makesequence(p1,node_disp(0,node_width,d==2?q:0));
        if (d==2)
            S._VECTptr->push_back(node_label(G.node(i)));
        res.push_back(symbolic(at_point,S));
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
        make_default_vertex_labels(V,g.val,0,contextptr);
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

gen _articulation_points(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
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

gen _graph_blocks(const gen &g,GIAC_CONTEXT) {
    if (g.type==_STRNG &&g.subtype==-1) return g;
    if (g.type!=_VECT)
        return gentypeerr(contextptr);
    graphe G;
    if (!G.read_gen(*g._VECTptr,contextptr)) {
        gt_err(_GT_ERR_NOT_A_GRAPH,contextptr);
        return gentypeerr(contextptr);
    }
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
static const char _graph_blocks_s []="graph_blocks";
static define_unary_function_eval(__graph_blocks,&_graph_blocks,_graph_blocks_s);
define_unary_function_ptr5(at_graph_blocks,alias_at_graph_blocks,&__graph_blocks,0,true)

#ifndef NO_NAMESPACE_GIAC
}
#endif // ndef NO_NAMESPACE_GIAC
