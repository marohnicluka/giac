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
#include <set>

using namespace std;

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

#define PLASTIC_NUMBER 1.32471795724474602596090885
#define PURCHASE_TIMEOUT 2.5
#define PLESTENJAK_MAX_TRIES 6
#define _GRAPH__VECT 30

const gen graphe::VRAI=gen(1).change_subtype(_INT_BOOLEAN);
const gen graphe::FAUX=gen(0).change_subtype(_INT_BOOLEAN);

/* messages */

void graphe::message(const char *str) {
    *logptr(ctx) << str << endl;
}

void graphe::message(const char *format,int a) {
    char buffer[256];
    sprintf(buffer,format,a);
    *logptr(ctx) << buffer << endl;
}

void graphe::message(const char *format,int a,int b) {
    char buffer[256];
    sprintf(buffer,format,a,b);
    *logptr(ctx) << buffer << endl;
}

void graphe::message(const char *format,int a,int b,int c) {
    char buffer[256];
    sprintf(buffer,format,a,b,c);
    *logptr(ctx) << buffer << endl;
}

/* vertex class implementation */
graphe::vertex::vertex() {
    m_symbol=0;
    m_subgraph=-1;
    m_visited=false;
    m_parent=-1;
    m_prelim=0;
    m_modifier=0;
    m_isleaf=false;
    m_position=0;
}

graphe::vertex::vertex(const vertex &other) {
    m_symbol=other.symbol();
    m_subgraph=other.subgraph();
    m_visited=other.is_visited();
    m_parent=other.parent();
    m_prelim=other.prelim();
    m_modifier=other.modifier();
    m_isleaf=other.is_leaf();
    m_position=other.position();
    set_attributes(other.attributes());
    m_neighbors.resize(other.neighbors().size());
    m_neighbor_attributes.clear();
    for (ivector_iter it=other.neighbors().begin();it!=other.neighbors().end();++it) {
        m_neighbors[it-other.neighbors().begin()]=*it;
        copy_attributes(other.neighbor_attributes(*it),m_neighbor_attributes[*it]);
    }
}

graphe::vertex& graphe::vertex::operator =(const vertex &other) {
    m_symbol=other.symbol();
    m_subgraph=other.subgraph();
    m_visited=other.is_visited();
    m_parent=other.parent();
    m_prelim=other.prelim();
    m_modifier=other.modifier();
    m_isleaf=other.is_leaf();
    m_position=other.position();
    set_attributes(other.attributes());
    m_neighbors.resize(other.neighbors().size());
    m_neighbor_attributes.clear();
    for (ivector_iter it=other.neighbors().begin();it!=other.neighbors().end();++it) {
        m_neighbors[it-other.neighbors().begin()]=*it;
        copy_attributes(other.neighbor_attributes(*it),m_neighbor_attributes[*it]);
    }
    return *this;
}

void graphe::vertex::add_neighbor(int i, const attrib &attr) {
    m_neighbors.push_back(i);
    copy_attributes(attr,m_neighbor_attributes[i]);
}

graphe::attrib &graphe::vertex::neighbor_attributes(int i) {
    map<int,attrib>::iterator it=m_neighbor_attributes.find(i);
    if (it==m_neighbor_attributes.end())
        it=m_neighbor_attributes.find(-i-1);
    assert(it!=m_neighbor_attributes.end());
    return it->second;
}

const graphe::attrib &graphe::vertex::neighbor_attributes(int i) const {
    neighbor_iter it=m_neighbor_attributes.find(i);
    if (it==m_neighbor_attributes.end())
        it=m_neighbor_attributes.find(-i-1);
    assert(it!=m_neighbor_attributes.end());
    return it->second;
}

bool graphe::vertex::has_neighbor(int i,bool include_temp_edges) const {
    return m_neighbor_attributes.find(i)!=m_neighbor_attributes.end() ||
            (include_temp_edges && m_neighbor_attributes.find(-i-1)!=m_neighbor_attributes.end());
}

void graphe::vertex::remove_neighbor(int i) {
    ivector::iterator it=find(m_neighbors.begin(),m_neighbors.end(),i);
    if (it!=m_neighbors.end()) {
        m_neighbors.erase(it);
        map<int,attrib>::iterator jt=m_neighbor_attributes.find(i);
        m_neighbor_attributes.erase(jt);
    }
}

/* dotgraph class implementation */
graphe::dotgraph::dotgraph() {
    m_index=0;
    pos=0;
    m_chain=ivector(1,0);
}

graphe::dotgraph::dotgraph(int i) {
    m_index=i;
    pos=0;
    m_chain=ivector(1,0);
}

graphe::dotgraph::dotgraph(const dotgraph &other) {
    m_index=other.index();
    copy_attributes(other.vertex_attributes(),vertex_attr);
    copy_attributes(other.edge_attributes(),edge_attr);
    copy_attributes(other.chain_attributes(),chain_attr);
    m_chain=other.chain();
    pos=other.position();
}

graphe::dotgraph& graphe::dotgraph::operator =(const dotgraph &other) {
    set_index(other.index());
    copy_attributes(other.vertex_attributes(),vertex_attr);
    copy_attributes(other.edge_attributes(),edge_attr);
    copy_attributes(other.chain_attributes(),chain_attr);
    m_chain=other.chain();
    pos=other.position();
    return *this;
}

/* rectangle class implementation */
graphe::rectangle::rectangle() {
    m_x=m_y=m_width=m_height=0;
}

graphe::rectangle::rectangle(const rectangle &rect) {
    m_x=rect.x();
    m_y=rect.y();
    m_width=rect.width();
    m_height=rect.height();
}

graphe::rectangle::rectangle(double X,double Y,double W,double H) {
    m_x=X;
    m_y=Y;
    m_width=W;
    m_height=H;
}

graphe::rectangle& graphe::rectangle::operator =(const rectangle &other) {
    m_x=other.x();
    m_y=other.y();
    m_width=other.width();
    m_height=other.height();
    return *this;
}

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

/* make a copy of attr */
void graphe::copy_attributes(const attrib &src,attrib &dest) {
    dest.clear();
    for (attrib_iter it=src.begin();it!=src.end();++it) {
        dest.insert(make_pair(it->first,gen(it->second)));
    }
}

/* returns true iff g is a graph and writes the basic info to 'display_str' */
bool is_graphe(const gen &g,string &disp_out,GIAC_CONTEXT) {
    if (g.type!=_VECT || g.subtype!=_GRAPH__VECT)
        return false;
    graphe G(contextptr);
    if (!G.read_gen(*g._VECTptr))
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
void graphe::make_default_vertex_labels(vecteur &labels,int n,int n0) const {
    int ofs=array_start(ctx);
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
graphe::graphe(const context *contextptr) {
    ctx=contextptr;
    set_graph_attribute(_GT_ATTRIB_DIRECTED,FAUX);
    set_graph_attribute(_GT_ATTRIB_WEIGHTED,FAUX);
}

/* graphe constructor, create a copy of G */
graphe::graphe(const graphe &G) {
    set_graph_attribute(_GT_ATTRIB_DIRECTED,boole(G.is_directed()));
    set_graph_attribute(_GT_ATTRIB_WEIGHTED,boole(G.is_weighted()));
    ctx=G.giac_context();
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
    int n;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        n=it->neighbors().size();
        attrib2genmap(it->attributes(),attr);
        vecteur v=makevecteur(it->symbol(),attr,vecteur(n));
        int j=0;
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            attrib2genmap(it->neighbor_attributes(*jt),attr);
            v.back()._VECTptr->at(j++)=makevecteur(*jt,attr);
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

/* fill V with marked vertices */
void graphe::get_marked_vertices(vecteur &V) const {
    V.clear();
    for (ivector_iter it=marked_vertices.begin();it!=marked_vertices.end();++it) {
        V.push_back(node_label(*it));
    }
}

/* mark vertex with index v */
void graphe::mark_vertex(int v) {
    if (find(marked_vertices.begin(),marked_vertices.end(),v)==marked_vertices.end())
        marked_vertices.push_back(v);
}

/* remove vertex v from the list of marked vertices */
bool graphe::unmark_vertex(int v) {
    ivector::iterator it;
    if ((it=find(marked_vertices.begin(),marked_vertices.end(),v))!=marked_vertices.end()) {
        marked_vertices.erase(it);
        return true;
    }
    return false;
}

/* return edge as pair of vertex indices */
graphe::ipair graphe::make_edge(const vecteur &v) const {
    assert(v.size()==2);
    int i=node_index(v.front()),j=node_index(v.back());
    return make_pair(i,j);
}

/* return total number of edges/arcs */
int graphe::edge_count() const {
    int count=0;
    for(node_iter it=nodes.begin();it!=nodes.end();++it) {
        count+=it->neighbors().size();
    }
    if (!is_directed())
        count/=2;
    return count;
}

/* return number of arcs going in the vertex with the specified index (when directed) */
int graphe::in_degree(int index,bool count_temp_edges) const {
    assert(index>=0 && index<node_count());
    int i=0,count=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (i==index)
            continue;
        if (it->has_neighbor(index,count_temp_edges))
            count++;
        ++i;
    }
    return count;
}

/* return number of arcs going out of vertex with the specified index (when directed) */
int graphe::out_degree(int index,bool count_temp_edges) const {
    assert(index>=0 && index<node_count());
    const vertex &vert=nodes[index];
    if (count_temp_edges)
        return vert.neighbors().size();
    int count=0;
    for (ivector_iter it=vert.neighbors().begin();it!=vert.neighbors().end();++it) {
        if (count_temp_edges || *it>=0)
            count++;
    }
    return count;
}

/* return degree of vertex with the specified index */
int graphe::degree(int index,bool count_temp_edges) const {
    if (is_directed())
        return in_degree(index,count_temp_edges)+
                out_degree(index,count_temp_edges);
    return nodes[index].neighbors().size();
}

/* return adjacency matrix of this graph */
matrice graphe::adjacency_matrix() const {
    int n=node_count(),j;
    matrice m=*_matrix(makesequence(n,n,0),context0)._VECTptr;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
            if (j<0)
                j=-j-1;
            m[it-nodes.begin()]._VECTptr->at(j)=gen(1);
        }
    }
    return m;
}

/* return incidence matrix of this graph */
matrice graphe::incidence_matrix() const {
    int nr=node_count(),nc=edge_count(),k=0;
    matrice m=*_matrix(makesequence(nr,nc,0),context0)._VECTptr;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            m[it-nodes.begin()]._VECTptr->at(k)=gen(is_directed()?-1:1);
            m[*jt]._VECTptr->at(k)=gen(1);
            ++k;
        }
    }
    return m;
}

/* return weight associated with edge {i,j} or arc [i,j] */
gen graphe::weight(int i,int j) const {
    const attrib &attr=edge_attributes(i,j);
    attrib_iter it=attr.find(_GT_ATTRIB_WEIGHT);
    if (it==attr.end())
        return undef;
    return it->second;
}

/* return weight matrix */
matrice graphe::weight_matrix() const {
    assert(is_weighted());
    int n=node_count();
    matrice m=*_matrix(makesequence(n,n,0),context0)._VECTptr;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            m[it-nodes.begin()]._VECTptr->at(*jt)=it->neighbor_attributes(*jt).find(_GT_ATTRIB_WEIGHT)->second;
        }
    }
    return m;
}

/* return list of vertices */
vecteur graphe::vertices() const {
    vecteur V(node_count());
    int i=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        V[i++]=it->symbol();
    }
    return V;
}

/* make all vertices unvisited */
void graphe::unvisit_all_vertices() {
    for (std::vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->set_visited(false);
    }
}

/* unset ancestors for all vertices */
void graphe::unset_all_ancestors() {
    for (std::vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->unset_parent();
    }
}

/* fill the list E with edges represented as pairs of vertex indices */
void graphe::get_edges_as_pairs(ipairs &E,bool include_temp_edges) const {
    set<ipair> pairs;
    int i,j;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
            if (j<0) {
                if (!include_temp_edges)
                    continue;
                j=-j-1;
            }
            pairs.insert(is_directed()?make_pair(i,j):make_pair(i<j?i:j,i<j?j:i));
        }
    }
    E.resize(pairs.size());
    i=0;
    for (set<ipair>::const_iterator it=pairs.begin();it!=pairs.end();++it) {
        E[i++]=*it;
    }
}

/* return list of edges/arcs */
vecteur graphe::edges(bool include_weights) const {
    ipairs E;
    get_edges_as_pairs(E);
    vecteur edge(2),ret(E.size());
    for (ipairs::const_iterator it=E.begin();it!=E.end();++it) {
        edge[0]=nodes[it->first].symbol();
        edge[1]=nodes[it->second].symbol();
        ret[it-E.begin()]=include_weights?makevecteur(edge,weight(it->first,it->second)):edge;
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
        if (!it->attributes().empty()) {
            dotfile << indent << it->symbol() << " ";
            write_attrib(dotfile,it->attributes());
            dotfile << ";" << endl;
        }
        u.clear();
        v.clear();
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            if (it->neighbor_attributes(*jt).empty())
                u.push_back(*jt);
            else
                v.push_back(*jt);
        }
        if (!u.empty()) {
            dotfile << indent << it->symbol() << edgeop << "{ ";
            for (ivector_iter kt=u.begin();kt!=u.end();++kt) {
                dotfile << nodes[*kt].symbol() << " ";
            }
            dotfile << "};" << endl;
        }
        for (ivector_iter kt=v.begin();kt!=v.end();++kt) {
            dotfile << indent << it->symbol() << edgeop << nodes[*kt].symbol() << " ";
            write_attrib(dotfile,it->neighbor_attributes(*kt));
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
bool graphe::remove_attribute(attrib &attr,int key) {
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
                                                            subgraphs.back().vertex_attributes():
                                                            subgraphs.back().edge_attributes()))
                    error_raised=true;
                break;
            }
        case _GT_DOT_TOKEN_TYPE_NUMBER:
        case _GT_DOT_TOKEN_TYPE_STRING:
            index=add_node(str2gen(token,true));
            nodes[index].set_subgraph(subgraphs.back().index());
            subgraphs.back().set_index(index+1);
            break;
        case _GT_DOT_TOKEN_TYPE_DELIMITER:
            if (token==";") {
                if (subgraphs.back().chain_empty())
                    continue;
                if (!subgraphs.back().chain_completed()) { error_raised=true; break; }
                ivector &ch=subgraphs.back().chain();
                attrib &attr=subgraphs.back().chain_attributes();
                if (subgraphs.back().position()==0) {
                    if (ch.front()<=0) { error_raised=true; break; }
                    nodes[ch.front()-1].set_attributes(subgraphs.back().chain_attributes());
                }
                int lh,rh;
                for (int i=0;i<=subgraphs.back().position()-1;++i) {
                    lh=ch[i];
                    rh=ch[i+1];
                    if (lh==0 || rh==0) { error_raised=true; break; }
                    if (lh>0 && rh>0)
                        add_edge(lh-1,rh-1,attr);
                    else if (lh<0 && rh<0) {
                        int j=0,k=0;
                        for (node_iter it=nodes.begin();it!=nodes.end();++it) {
                            if (it->subgraph()==-lh) {
                                for (node_iter jt=nodes.begin();jt!=nodes.end();++jt) {
                                    if (jt->subgraph()==-rh)
                                        add_edge(j,k,attr);
                                    ++k;
                                }
                            }
                            ++j;
                        }
                    } else {
                        int j=0;
                        for (node_iter it=nodes.begin();it!=nodes.end();++it) {
                            if (it->subgraph()==(lh>0?-rh:-lh))
                                add_edge(lh>0?lh-1:j,lh>0?j:rh-1,attr);
                            ++j;
                        }
                    }
                }
                subgraphs.back().clear_chain();
            } else if (token=="[") {
                attrib &attr=subgraphs.back().chain_attributes();
                if (!attr.empty() || !dot_parse_attributes(dotfile,attr))
                    error_raised=true;
            } else if (token=="{") {
                if (dot_subgraph_level<2) { error_raised=true; break; }
                subgraphs.push_back(dotgraph(subgraph_count++));
            } else if (token=="}") {
                if (subgraphs.empty()) { error_raised=true; break; }
                index=subgraphs.back().index();
                subgraphs.pop_back();
                if (!subgraphs.empty())
                    subgraphs.back().set_index(-index);
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
    int i;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            if (is_directed() || i<*jt)
                insert_attribute(it->neighbor_attributes(*jt),_GT_ATTRIB_WEIGHT,m[i][*jt]);
        }
    }
    set_graph_attribute(_GT_ATTRIB_WEIGHTED,VRAI);
}

/* make this graph unweighted */
void graphe::make_unweighted() {
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            remove_attribute(it->neighbor_attributes(*jt),_GT_ATTRIB_WEIGHT);
        }
    }
    set_graph_attribute(_GT_ATTRIB_WEIGHTED,FAUX);
}

/* store the underlying graph to G (convert arcs to edges and strip all attributes) */
void graphe::underlying(graphe &G) const {
    G.add_nodes(vertices());
    G.set_directed(false);
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            G.add_edge(it-nodes.begin(),*jt);
        }
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
bool graphe::read_gen(const vecteur &v) {
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
        genmap2attrib(*it->_VECTptr->at(1)._MAPptr,vt.attributes());
        const_iterateur jt=it->_VECTptr->at(2)._VECTptr->begin();
        for (;jt!=it->_VECTptr->at(2)._VECTptr->end();++jt) {
            if (jt->type!=_VECT || !jt->_VECTptr->front().is_integer() || jt->_VECTptr->at(1).type!=_MAP)
                return false;
            attrib attr;
            genmap2attrib(*jt->_VECTptr->at(1)._MAPptr,attr);
            vt.add_neighbor(jt->_VECTptr->front().val,attr);
        }
    }
    return true;
}

/* make a copy of this graph and store it in G */
void graphe::copy(graphe &G) const {
    G.set_name(graph_name);
    G.register_user_tags(user_tags);
    G.set_graph_attributes(attributes);
    G.copy_nodes(nodes);
    G.copy_marked_vertices(get_marked_vertices());
}

/* returns true iff graph has edge {i,j} */
bool graphe::has_edge(int i,int j) const {
    if (i<0 || i>=node_count() || j<0 || j>=node_count())
        return false;
    return nodes[i].has_neighbor(j);
}

/* returns true iff i-th and j-th vertices are adjacent */
bool graphe::is_adjacent(int i,int j) const {
    return nodes[i].has_neighbor(j) || nodes[j].has_neighbor(i);
}

const graphe::attrib &graphe::edge_attributes(int i, int j) const {
    if (is_directed())
        return nodes[i].neighbor_attributes(j);
    return nodes[i<j?i:j].neighbor_attributes(i<j?j:i);
}

graphe::attrib &graphe::edge_attributes(int i, int j) {
    if (is_directed())
        return nodes[i].neighbor_attributes(j);
    return nodes[i<j?i:j].neighbor_attributes(i<j?j:i);
}

/* add edge {i,j} or arc [i,j], depending on the type (undirected or directed) */
void graphe::add_edge(int i,int j,const gen &w) {
    assert(i>=0 && i<node_count() && j>=0 && j<node_count());
    if (has_edge(i,j))
        return;
    nodes[i].add_neighbor(j);
    if (!is_directed())
        nodes[j].add_neighbor(i);
    if (is_weighted())
        set_edge_attribute(i,j,_GT_ATTRIB_WEIGHT,w);
}

/* add edge {i,j} or arc [i,j] with attributes */
void graphe::add_edge(int i, int j,const attrib &attr) {
    assert(i>=0 && i<node_count() && j>=0 && j<node_count());
    if (has_edge(i,j))
        return;
    if (is_directed())
        nodes[i].add_neighbor(j,attr);
    else {
        int v=i<j?i:j,w=i<j?j:i;
        nodes[v].add_neighbor(w,attr);
        nodes[w].add_neighbor(v);
    }
}

/* add edge {v,w} or arc [v,w], adding vertices v and/or w if necessary */
graphe::ipair graphe::add_edge(const gen &v,const gen &w,const gen &weight) {
    int i=add_node(v),j=add_node(w);
    add_edge(i,j,weight);
    if (is_directed())
        return make_pair(i,j);
    return make_pair(i<j?i:j,i<j?j:i);
}

/* add temporary edge from i-th to j-th vertex */
void graphe::add_temporary_edge(int i,int j) {
    assert(!has_edge(i,j));
    nodes[i].add_neighbor(-j-1);
    if (!is_directed())
        nodes[j].add_neighbor(-i-1);
}

/* remove all temporary edges */
void graphe::remove_temporary_edges() {
    stack<int> to_remove;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            if (*jt<0)
                to_remove.push(*jt);
        }
        while (!to_remove.empty()) {
            it->remove_neighbor(to_remove.top());
            to_remove.pop();
        }
    }
}

/* remove edge {v,w} or arc [v,w] */
bool graphe::remove_edge(int i,int j) {
    if (!has_edge(i,j))
        return false;
    nodes[i].remove_neighbor(j);
    if (!is_directed())
        nodes[j].remove_neighbor(i);
    return true;
}

/* add vertex v to graph */
int graphe::add_node(const gen &v) {
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->symbol()==v)
            return it-nodes.begin();
    }
    vertex vert;
    vert.set_symbol(v);
    nodes.push_back(vert);
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
    for (unsigned j=isolated_nodes.size();j-->0;) {
        remove_isolated_node(isolated_nodes[j]);
    }
}

/* return vector of node symbols */
vecteur graphe::get_nodes(const ivector &v) const {
    vecteur V(v.size());
    for (ivector_iter it=v.begin();it!=v.end();++it) {
        V[it-v.begin()]=node_label(*it);
    }
    return V;
}

/* return index of vertex v */
int graphe::node_index(const gen &v) const {
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->symbol()==v)
            return it-nodes.begin();
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
    nodes[index].set_attribute(key,val);
}

/* set edge {i,j} attribute key=val */
void graphe::set_edge_attribute(int i,int j,int key,const gen &val) {
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
    const attrib &attr=nodes[index].attributes();
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
        gen v=node_label(*it);
        const attrib &attri=nodes[*it].attributes();
        if (copy_attrib)
            i=G.add_node(v,attri);
        else
            G.add_node(v);
        for (ivector_iter jt=adj.begin();jt!=adj.end();++jt) {
            if (find(vi.begin(),vi.end(),*jt)==vi.end())
                continue;
            gen w=node_label(*jt);
            if (copy_attrib) {
                const attrib &attrj=nodes[*jt].attributes();
                j=G.add_node(w,attrj);
                G.add_edge(i,j,edge_attributes(i,j));
            } else
                G.add_edge(v,w);
        }
    }
}

/* return sorted list of vertices adjacent to the i-th one */
graphe::ivector graphe::adjacent_nodes(int i,bool include_temp_edges) const {
    assert(i>=0 && i<node_count());
    node_iter vt=nodes.begin()+i;
    ivector res;
    res.reserve(vt->neighbors().size());
    int j;
    for (ivector_iter it=vt->neighbors().begin();it!=vt->neighbors().end();++it) {
        j=*it;
        if (include_temp_edges || j>=0)
            res.push_back(j<0?-j-1:j);
    }
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
graphe::matching_maximizer::matching_maximizer(graphe *gr) {
    G=gr;
}

/* return the vertex which matches v in matching, return -1 if v is not matched */
int graphe::matching_maximizer::mate(const ipairs &matching, int v) {
    for (ipairs::const_iterator it=matching.begin();it!=matching.end();++it) {
        if (it->first==v)
            return it->second;
        if (it->second==v)
            return it->first;
    }
    return -1;
}

int graphe::matching_maximizer::find_root(int k) {
    map<int,int>::const_iterator it=forest.find(k);
    if (it==forest.end())
        return -1;
    while (it->second!=-1) {
        it=forest.find(it->second);
    }
    return it->first;
}

int graphe::matching_maximizer::root_distance(map<int,int>::const_iterator it) {
    int d=0;
    while (it->second!=-1) {
        it=forest.find(it->second);
        ++d;
    }
    return d;
}

int graphe::matching_maximizer::root_distance(int v) {
    map<int,int>::const_iterator it=forest.find(v);
    if (it==forest.end())
        return -1;
    return root_distance(it);
}

int graphe::matching_maximizer::find_base(int v, int w) {
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

bool graphe::matching_maximizer::tree_path(int v, int w, ivector &path) {
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

map<int,graphe::ivector>::iterator graphe::matching_maximizer::in_blossom(int v) {
    map<int,ivector>::iterator it=blossoms.begin();
    for (;it!=blossoms.end();++it) {
        if (find(it->second.begin(),it->second.end(),v)!=it->second.end())
            break;
    }
    return it;
}

map<int,graphe::ivector>::iterator graphe::matching_maximizer::is_blossom_base(int v) {
    map<int,ivector>::iterator it=blossoms.begin();
    for (;it!=blossoms.end();++it) {
        if (v==it->first)
            break;
    }
    return it;
}

void graphe::matching_maximizer::append_non_blossom_adjacents(int v,map<int,ivector>::const_iterator bit,ivector &lst) {
    ivector adj=G->adjacent_nodes(v);
    int a;
    for (unsigned i=adj.size();i-->0;) {
        a=adj[i];
        if (a==bit->first || find(bit->second.begin(),bit->second.end(),a)!=bit->second.end())
            adj.erase(adj.begin()+i);
    }
    lst.insert(lst.begin(),adj.begin(),adj.end());
}

graphe::ivector graphe::matching_maximizer::adjacent(int v) {
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

bool graphe::matching_maximizer::find_augmenting_path(const ipairs &matching, ivector &path) {
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

void graphe::matching_maximizer::find_maximum_matching(ipairs &matching) {
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

/*
 * TRIANGULATOR FOR BICONNECTED GRAPHS
 */

graphe::triangulator::triangulator(graphe *gr,ivectors *Gt) {
    G=gr;
    embedding=Gt;
    degrees.resize(G->node_count());
    for (ivector::iterator it=degrees.begin();it!=degrees.end();++it) {
        *it=G->degree(it-degrees.begin());
    }
}

int graphe::triangulator::predecessor(int i,int n) {
    if (i>0)
        return i-1;
    return n-1;
}

int graphe::triangulator::successor(int i,int n) {
    if (i<n-1)
        return i+1;
    return 0;
}

void graphe::triangulator::addedge(int v,int w) {
    G->add_temporary_edge(v,w);
    ++degrees[v];
    ++degrees[w];
}

void graphe::triangulator::path(int i, int j, const ivector &B,ivector &P) {
    int n=B.size(),k=i,m=j-i,l=0;
    if (m<0)
        m+=n;
    P.resize(m+1);
    P.front()=B[i];
    while (k++!=j) {
        if (k==n)
            k=0;
        P[++l]=B[k];
    }
}

void graphe::triangulator::creases(const ivector &B,ipairs &C) {
    int n=B.size(),h=n%2==0?n/2:(n-1)/2;
    ivector D;
    for (int k=0;k<n;++k) {
        if (degrees[B[k]]>2)
            D.push_back(k);
    }
    int m=D.size(),k=0,i,j,k_next,l,v,w,r;
    while (true) {
        i=D[k];
        v=B[i];
        l=k;
        k_next=successor(k,m);
        ipair c=make_pair(i,-1);
        while (true) {
            l=successor(l,m);
            j=D[l];
            r=j-i;
            if (r<0)
                r+=n;
            if (r==0 || r>h || (n%2==0 && r==h && i>=h))
                break;
            w=B[j];
            if (r>1 && G->has_edge(v,w)) {
                c.second=j;
                k_next=l;
            }
        }
        if (c.second>=0)
            C.push_back(c);
        if (k_next<k)
            break;
        k=k_next;
    }
    if (C.size()>1) {
        int end=C.back().second;
        if (end<C.back().first) {
            ipairs::const_iterator it=C.begin();
            while (it->first<end) {
                ++it;
            }
            C.erase(C.begin(),it);
        }
    }
}

void graphe::triangulator::zigzag(const ivector &B) {
    int n=B.size(),h=n%2==0?n/2:(n-1)/2,m=n%2==0?h:n,k=0,l=h,s,t=l,d=0,dtotal=0,d1,d2,dmax;
    for (s=0;s<m;++s) {
        d1=degrees[B[s]];
        d2=degrees[B[t]];
        dmax=std::max(d1,d2);
        if (dmax>d || (dmax==d && d1+d2>dtotal)) {
            d=dmax;
            dtotal=d1+d2;
            k=s;
            l=t;
        }
        t=successor(t,n);
    }
    int i=predecessor(k,n),j=successor(k,n),v=B[i],w=B[j];
    if (n==4)
        addedge(v,w);
    else if (n>=5) {
        s=predecessor(l,n);
        t=successor(l,n);
        int vv=B[s],ww=B[t];
        k=degrees[v]+degrees[vv]<degrees[w]+degrees[ww]?1:0;
        for (s=0;s<n-3;++s) {
            addedge(B[i],B[j]);
            if (k%2==0)
                i=predecessor(i,n);
            else
                j=successor(j,n);
            ++k;
        }
    }
}

void graphe::triangulator::fold(const ivector &B) {
    ipairs C;
    creases(B,C);
    int n=B.size(),k=C.size(),i,j,q,r,s,t,u,i1,j1,i2,j2;
    if (k==0) {
        zigzag(B);
    } else if (k==1) {
        i=C.front().first;
        q=successor(i,n);
        t=predecessor(i,n);
        addedge(B[q],B[t]);
        if (n>4) {
            j=C.front().second;
            r=predecessor(j,n);
            u=successor(j,n);
            addedge(B[r],B[u]);
            ivector P1,P2;
            path(q,r,B,P1);
            path(u,t,B,P2);
            P1.insert(P1.end(),P2.begin(),P2.end());
            zigzag(P1);
        }
    } else { // if k>1
        i1=C.front().first;
        j1=C.front().second;
        i2=C[1].first;
        j2=C[1].second;
        q=successor(i1,n);
        r=successor(i2,n);
        if (k==2 && q==predecessor(j1,n) && r==predecessor(j2,n)) {
            addedge(B[q],B[r]);
            ivector P;
            path(q,r,B,P);
            zigzag(P);
            path(r,q,B,P);
            zigzag(P);
        } else {
            ivector P(0);
            ivector Q;
            for (s=0;s<k;++s) {
                i=s<k-1?C[s+1].first:C.front().first;
                j=C[s].second;
                q=predecessor(j,n);
                r=successor(i,n);
                addedge(B[q],B[r]);
                path(q,r,B,Q);
                zigzag(Q);
                path(successor(C[s].first,n),q,B,Q);
                P.insert(P.end(),Q.begin(),Q.end());
            }
            if (P.size()>3)
                fold(P);
        }
    }
}

void graphe::triangulator::triangulate(int outer_face) {
    int n=embedding->size();
    for (int i=0;i<n;++i) {
        const ivector &F=embedding->at(i);
        if (i!=outer_face && F.size()>3)
            fold(F);
    }
}

/*
 * END TRIANGULATOR IMPLEMENTATION
 */

/* extend given matching to the maximum matching using Edmonds' blossom algorithm */
void graphe::maximize_matching(ipairs &matching) {
    matching_maximizer ed(this);
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
void graphe::add_point(point &a,const point &b) {
    int d=a.size();
    assert(int(b.size())==d);
    for (int i=0;i<d;++i) {
        a[i]+=b[i];
    }
}

/* subtract point b from point a */
void graphe::subtract_point(point &a,const point &b) {
    int d=a.size();
    assert(int(b.size())==d);
    for (int i=0;i<d;++i) {
        a[i]-=b[i];
    }
}

/* scale coordinates of point p by factor s */
void graphe::scale_point(point &p,double s) {
    int d=p.size();
    for (int i=0;i<d;++i) {
        p[i]*=s;
    }
}

/* compute v_x*w_y-v_y*w_x */
double graphe::point_vecprod2d(const point &v,const point &w) {
    assert(v.size()==2 && w.size()==2);
    return v[0]*w[1]-v[1]*w[0];
}

/* compute dot product of vectors p and q */
double graphe::point_dotprod(const point &p,const point &q) {
    int n=p.size();
    assert((n==2 || n==3) && n==int(q.size()));
    double res=0;
    for (int i=0;i<n;++i) {
        res+=p[i]*q[i];
    }
    return res;
}

/* set all coordinates of point p to zero */
void graphe::clear_point_coords(point &p) {
    for (point::iterator it=p.begin();it!=p.end();++it) {
        *it=0;
    }
}

/* convert point to Giac representation (cplx or point(x,y,[z])) */
gen graphe::point2gen(const point &p,bool vect) {
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
void graphe::copy_layout(const layout &src,layout &dest) {
    layout_iter st=src.begin();
    layout::iterator dt=dest.begin();
    for (;st!=src.end() && dt!=dest.end();++st,++dt) {
        *dt=*st;
    }
}

/* copy point src to dest (both must be initialized first) */
void graphe::copy_point(const point &src,point &dest) {
    point::const_iterator st=src.begin();
    point::iterator dt=dest.begin();
    for (;st!=src.end() && dt!=dest.end();++st,++dt) {
        *dt=*st;
    }
}

/* generate uniformly random point p on the origin-centered sphere with the specified radius */
void graphe::rand_point(point &p,double radius) {
    double R;
    do {
        R=0;
        for (point::iterator it=p.begin();it!=p.end();++it) {
            *it=rand_normal();
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
void graphe::translate_layout(layout &x,const point &dx) {
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        add_point(*it,dx);
    }
}

/* rotate layout x about the point p by the angle phi (2d only) */
void graphe::rotate_layout(layout &x,double phi) {
    if (x.empty())
        return;
    int d=x.front().size();
    assert(d==2);
    // convert layout to polar coordinates and rotate it
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        point2polar(*it);
    }
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        it->at(1)+=phi;
    }
    // convert layout back to cartesian coordinates
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        point &p=*it;
        double r=p[0],phi=p[1];
        p[0]=r*std::cos(phi);
        p[1]=r*std::sin(phi);
    }
}

/* determine minimum of d-th components among layout points */
double graphe::layout_min(const layout &x,int d) {
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
        p[0]=rand_uniform();
        p[1]=rand_uniform();
        if (d==3)
            p[2]=rand_uniform();
        *it=p;
    }
}

/* lay out the graph using a force-directed algorithm with spring-electrical model */
void graphe::force_directed_placement(layout &x,double K,double R,double tol,bool ac) {
    double step_length=K,shrinking_factor=0.9;
    double energy=DBL_MAX,energy0;
    double C=0.01,D=C*K*K,R_squared=R*R,eps=K*tol;
    double norm,norm_squared,displacement,max_displacement;
    int progress=0,n=x.size(),i,j;
    if (n==0)
        return;
    assert (n==node_count());
    int d=x.front().size();
    point force(d),p(d);
    ivector adj;
    do {
        energy0=energy;
        energy=0;
        max_displacement=0;
        for (i=0;i<n;++i) {
            point &xi=x[i];
            clear_point_coords(force);
            // compute attractive forces between vertices adjacent to the i-th vertex
            adj=adjacent_nodes(i);
            for (ivector_iter it=adj.begin();it!=adj.end();++it) {
                j=*it;
                copy_point(xi,p);
                subtract_point(p,x[j]);
                scale_point(p,point_displacement(p)/K);
                subtract_point(force,p);
            }
            // compute repulsive forces for all vertices j!=i which are not too far from the i-th vertex
            for (j=0;j<n;++j) {
                if (i==j)
                    continue;
                copy_point(xi,p);
                subtract_point(p,x[j]);
                norm_squared=point_displacement(p,false);
                if (R_squared>0 && norm_squared>R_squared)
                    continue;
                if (norm_squared==0)
                    rand_point(p,norm_squared=shrinking_factor*eps);
                scale_point(p,D/norm_squared);
                add_point(force,p);
            }
            // move the location of the i-th vertex in the direction of the force f
            norm=point_displacement(force);
            if (norm==0)
                continue;
            if (step_length<norm)
                scale_point(force,step_length/norm);
            add_point(x[i],force);
            displacement=std::min(norm,step_length);
            if (displacement>max_displacement)
                max_displacement=displacement;
            energy+=norm*norm;
        }
        // update step length
        if (ac) {
            // adaptive cooling scheme
            if (energy<energy0) {
                ++progress;
                if (progress>=5) {
                    progress=0;
                    step_length/=shrinking_factor;
                }
            } else {
                progress=0;
                step_length*=shrinking_factor;
            }
        } else step_length*=shrinking_factor; // simple cooling scheme
    } while (max_displacement>=eps);
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
        G.add_node(node_label(*it));
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
        // the coarsest level, apply force directed algorithm on a random initial layout
        multilevel_depth=depth;
        create_random_layout(x,K,d);
        G.force_directed_placement(x,K,R*(depth+1)*K,tol,false);
    } else {
        // create coarser graph H and lay it out
        graphe H(G.giac_context());
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
        G.force_directed_placement(x,L,R*(depth+1)*L,tol,false);
    }
}

/* apply multilevel force directed algorithm to layout x */
void graphe::force_directed_placement_multilevel(layout &x,int d,double K,double tol) {
    int n=node_count();
    if (n==0)
        return;
    multilevel_mis=false;
    return multilevel_recursion(x,*this,d,DBL_MAX,K,tol);
}

/* layout face as a regular polygon inscribed in circle of radius R */
void graphe::make_regular_polygon_layout(layout &x,const ivector &face,double R) {
    int n=face.size(),v;
    for (int k=0;k<n;++k) {
        v=face[k];
        point &p=x[v];
        p.resize(2);
        p[0]=R*std::cos(2*k*M_PI/n);
        p[1]=R*std::sin(2*k*M_PI/n);
    }
}

/* apply force directed algorithm to this graph (must be triconnected), with the specified outer face,
 * using the algorithm by Bor Plestenjak in "An Algorithm for Drawing Planar Graphs" */
void graphe::plestenjak_layout(layout &x,const ivector &outer_face,double A,double tol) {
    int n=node_count(),iter_count=0,maxper=0,tries=0;
    vector<bool> is_outer(n,false);
    for (int i=0;i<n;++i) {
        if (find(outer_face.begin(),outer_face.end(),i)!=outer_face.end())
            is_outer[i]=true;
    }
    ivector per;
    if (A>0) {
        // compute vertex periphericity
        per.resize(n);
        periphericity(outer_face,per);
        for (ivector_iter pt=per.begin();pt!=per.end();++pt) {
            if (*pt>maxper)
                maxper=*pt;
        }
    }
    // place facial vertices on the unit circle and all other vertices in the origin
    make_regular_polygon_layout(x,outer_face);
    layout P(n);
    for (int i=0;i<n;++i) {
        P[i].resize(2);
        if (is_outer[i])
            continue;
        point &p=x[i];
        p.resize(2);
        p[0]=p[1]=0;
    }
    // cool down the system iteratively
    double displacement,cool,C=std::sqrt((double)n/M_PI),Ci,eps=tol;
    ipairs E;
    get_edges_as_pairs(E,false);
    while (true) {
        ++iter_count;
        // compute the resultant force for each non-outer vertex
        for (int i=0;i<n;++i) {
            if (is_outer[i])
                continue;
            point &pt=P[i],&p=x[i];
            clear_point_coords(pt);
            ivector adj=adjacent_nodes(i);
            for (ivector_iter it=adj.begin();it!=adj.end();++it) {
                point &q=x[*it];
                point r(2);
                copy_point(q,r);
                subtract_point(r,p);
                Ci=C;
                if (A>0)
                    Ci*=std::exp(A*(2.0*maxper-per[i]-per[*it])/(double)maxper);
                displacement=Ci*point_displacement(r,false);
                scale_point(r,displacement);
                add_point(pt,r);
            }
        }
        // move each vertex in the direction of the force
        cool=1.0/(C+std::pow(iter_count,1.5)/C);
        for (int i=0;i<n;++i) {
            if (is_outer[i])
                continue;
            point &pt=P[i],&p=x[i];
            displacement=point_displacement(pt);
            if (displacement>0) {
                scale_point(pt,std::min(displacement,cool)/displacement);
                add_point(p,pt);
            }
        }
        if (cool<eps) {
            if (!has_crossing_edges(x,E) || ++tries>PLESTENJAK_MAX_TRIES)
                break;
            eps/=2.0;
        }
    }
    if (tries>PLESTENJAK_MAX_TRIES)
        message("Error: failed to obtain planar layout");
    else
        message("Layout completed in %d iterations",iter_count);
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
    nodes.erase(nodes.begin()+i);
    int j;
    stack<int> adj;
    stack<attrib> attr;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
            if (j<0)
                adj.push(-j-1>i?j+1:j);
            else
                adj.push(j>i?j-1:j);
            attr.push(it->neighbor_attributes(j));
        }
        it->clear_neighbors();
        while (!adj.empty()) {
            it->add_neighbor(adj.top(),attr.top());
            adj.pop();
            attr.pop();
        }
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
    map<int,bool> sw;
    for (ivector_iter it=v.begin();it!=v.end();++it) {
        sw[*it]=true;
    }
    ipairs E,res;
    get_edges_as_pairs(E);
    for (ipairs::const_iterator it=E.begin();it!=E.end();++it) {
        if (sw[it->first] || sw[it->second])
            res.push_back(*it);
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
void graphe::make_sierpinski_graph(int n, int k, bool triangle) {
    ivectors tuples;
    ivector elem(n,0);
    ntupk(tuples,n,k,elem,0);
    int N=std::pow(k,n);
    vecteur V;
    make_default_vertex_labels(V,N,0);
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
        for (unsigned i=isolated_nodes.size();i-->0;) {
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
void graphe::make_complete_multipartite_graph(const ivector &partition_sizes) {
    int k=partition_sizes.size();
    ivectors partitions;
    int n0=0;
    vecteur v;
    for (ivector_iter it=partition_sizes.begin();it!=partition_sizes.end();++it) {
        int n=*it;
        make_default_vertex_labels(v,n,n0);
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
void graphe::make_petersen_graph(int n, int k) {
    vecteur u,v;
    make_default_vertex_labels(u,n,0);
    make_default_vertex_labels(v,n,n);
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

void graphe::find_cut_vertices_dfs(int u,int &t,ivector &disc,ivector &low,vector<bool> &ap) {
    int children=0;
    vertex &vert=nodes[u];
    vert.set_visited(true);
    disc[u]=low[u]=t;
    ++t;
    ivector adj=adjacent_nodes(u);
    bool is_articulation=false;
    for (ivector_iter it=adj.begin();it!=adj.end();++it) {
        int v=*it;
        vertex &wert=nodes[v];
        if (!wert.is_visited()) {
            wert.set_parent(u);
            find_cut_vertices_dfs(v,t,disc,low,ap);
            ++children;
            if (low[v]>=disc[u])
                is_articulation=true;
            low[u]=std::min(low[u],low[v]);
        } else if (v!=vert.parent())
            low[u]=std::min(low[u],disc[v]);
    }
    if ((vert.parent()>=0 && is_articulation) || (vert.parent()<0 && children>1))
        ap[u]=true;
}

/* return list of cut vertices obtained by using depth-first search, complexity O(V+E) */
graphe::ivector graphe::find_cut_vertices() {
    unvisit_all_vertices();
    unset_all_ancestors();
    int n=node_count();
    ivector low(n,-1),disc(n,-1);
    vector<bool> ap(n,false);
    int t=0;
    for (int i=0;i<n;++i) {
        if (!nodes[i].is_visited())
            find_cut_vertices_dfs(i,t,disc,low,ap);
    }
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
        edge=make_pair(v<w?v:w,v<w?w:v);
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

int graphe::find_cycle_dfs(int v,stack<int> &path) {
    vertex &vert=nodes[v];
    vert.set_visited(true);
    path.push(v);
    ivector adj=adjacent_nodes(v);
    int w,z,p=vert.parent();
    for (ivector_iter it=adj.begin();it!=adj.end();++it) {
        w=*it;
        vertex &wert=nodes[w];
        if (wert.is_visited()) {
            if (p!=w)
                return w;
            continue;
        }
        wert.set_parent(v);
        z=find_cycle_dfs(w,path);
        if (z>=0)
            return z;
    }
    path.pop();
    return -1;
}

/* return a cycle in this graph using DFS (graph is assumed to be connected),
 * or an empty list if there is no cycle (i.e. if the graph is a tree) */
graphe::ivector graphe::find_cycle(bool randomize) {
    unvisit_all_vertices();
    unset_all_ancestors();
    stack<int> path;
    ivector cycle;
    int n=node_count(),initial_vertex=0;
    if (randomize)
        initial_vertex=rand_integer(n);
    if (n>0) {
        int v=find_cycle_dfs(initial_vertex,path);
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

bool graphe::find_path_dfs(int dest,int v,stack<int> &path) {
    path.push(v);
    if (v==dest)
        return true;
    nodes[v].set_visited(true);
    ivector adj=adjacent_nodes(v);
    int w;
    for (ivector_iter it=adj.begin();it!=adj.end();++it) {
        w=*it;
        if (nodes[w].is_visited())
            continue;
        if (find_path_dfs(dest,w,path))
            return true;
    }
    path.pop();
    return false;
}

/* return a path from vertex v to vertex w using DFS (graph is assumed to be connected) */
graphe::ivector graphe::find_path(int v, int w) {
    unvisit_all_vertices();
    stack<int> path;
    ivector res;
    if (find_path_dfs(v,w,path)) {
        while (!path.empty()) {
            res.push_back(path.top());
            path.pop();
        }
    }
    return res;
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

/* clear the graph, deleting all nodes and vertices */
void graphe::clear() {
    clear_marked_vertices();
    nodes.clear();
}

/* translate indices to match those in graph G (which must be a subgraph of this graph) */
void graphe::translate_indices_to(const graphe &G,const ivector &indices,ivector &dest) const {
    int n=indices.size(),i=0;
    dest.resize(n);
    for (ivector_iter it=indices.begin();it!=indices.end();++it) {
        dest[i]=G.node_index(node_label(*it));
        ++i;
    }
}

/* translate indices of vertices in G (a subgraph of this) to match those in this graph */
void graphe::translate_indices_from(const graphe &G,const ivector &indices,ivector &dest) const {
    int n=indices.size(),i=0;
    dest.resize(n);
    for (ivector_iter it=indices.begin();it!=indices.end();++it) {
        dest[i++]=node_index(G.node_label(*it));
    }
}

/* return true iff the given face contains the edge {i,j} */
int graphe::face_has_edge(const ivector &face,int i,int j) {
    int v,w,k;
    for (ivector_iter it=face.begin();it!=face.end();++it) {
        v=*it;
        k=(it-face.begin()+1)%face.size();
        w=face[k];
        if ((i==v && j==w) || (i==w && j==v))
            return k;
    }
    return -1;
}

/* append all bridges w.r.t. embedding */
void graphe::demoucron_bridges(const vector<bool> &embedding,const ivectors &faces,vector<graphe> &bridges) const {
    ivector rest,adj;
    int N=bridges.size();
    for (vector<bool>::const_iterator it=embedding.begin();it!=embedding.end();++it) {
        if (!*it)
            rest.push_back(it-embedding.begin());
    }
    ivectors components;
    graphe G(ctx);
    if (!rest.empty()) {
        induce_subgraph(rest,G,false);
        G.connected_components(components);
        for (ivectors::iterator ct=components.begin();ct!=components.end();++ct) {
            ivector tmp;
            translate_indices_from(G,*ct,tmp);
            ct->swap(tmp);
        }
    }
    // find inner edges between embedded vertices
    ipairs inner_edges;
    int n=node_count();
    for (int i=0;i<n;++i) {
        if (!embedding[i])
            continue;
        for (int j=i+1;j<n;++j) {
            if (!embedding[j] || !has_edge(i,j))
                continue;
            ivectors_iter it;
            for (it=faces.begin();it!=faces.end();++it) {
                if (face_has_edge(*it,i,j)>=0)
                    break;
            }
            if (it==faces.end())
                inner_edges.push_back(make_pair(i,j));
        }
    }
    bridges.resize(N+components.size()+inner_edges.size());
    // create bridges from inner edges
    int m=components.size();
    n=inner_edges.size();
    for (int i=0;i<n;++i) {
        const ipair &p=inner_edges[i];
        graphe &bridge=bridges[N+m+i];
        const gen &v=node_label(p.first);
        const gen &w=node_label(p.second);
        bridge.add_edge(v,w);
        bridge.mark_vertex(v);
        bridge.mark_vertex(w);
    }
    if (!rest.empty()) {
        // create bridges from components
        for (ivectors_iter it=components.begin();it!=components.end();++it) {
            graphe &bridge=bridges[N+it-components.begin()];
            induce_subgraph(*it,bridge,false);
            // add edges connecting the component with embedding to bridge
            for (ivector_iter jt=it->begin();jt!=it->end();++jt) {
                adj=adjacent_nodes(*jt);
                const gen &v=node_label(*jt);
                for (ivector_iter kt=adj.begin();kt!=adj.end();++kt) {
                    if (embedding[*kt]) {
                        const gen &w=node_label(*kt);
                        bridge.add_edge(v,w);
                        bridge.mark_vertex(w);
                    }
                }
            }
        }
        // A fix to the Gibbons' implementation of Demoucron algorithm:
        // combine bridges with the same two points of contact.
        // See "Errors in graph embedding algorithms" by W.Myrvold et al.
        vector<bool> is_joined(m+n,false);
        int i,j;
        ivector mv,mw;
        for (vector<graphe>::iterator it=bridges.begin()+N;it!=bridges.end();++it) {
            i=it-bridges.begin()-N;
            if (is_joined[i] || it->get_marked_vertices().size()!=2)
                continue;
            translate_indices_from(*it,it->get_marked_vertices(),mv);
            for (vector<graphe>::iterator jt=it+1;jt!=bridges.end();++jt) {
                j=jt-bridges.begin()-N;
                if (is_joined[j] || jt->get_marked_vertices().size()!=2)
                    continue;
                translate_indices_from(*jt,jt->get_marked_vertices(),mw);
                if ((mv.front()==mw.front() && mv.back()==mw.back()) ||
                        (mv.front()==mw.back() && mv.back()==mw.front())) {
                    it->join(*jt);
                    is_joined[j]=true;
                }
            }
        }
        // remove joined bridges
        for (i=m+n;i-->0;) {
            if (is_joined[i])
                bridges.erase(bridges.begin()+N+i);
        }
    }
}

/* finds planar embedding of a biconnected graph as a list of faces, returns true iff the graph is planar */
bool graphe::demoucron(ivectors &faces) {
    ivector contact_vertices,cycle=find_cycle(); // a cycle in the graph
    ivectors admissible,G_faces;
    vector<bool> embedding(node_count(),false),G_embedding; // embedded vertices
    int i,j,k,n,f,nc=node_count(),ec=edge_count(),nf=ec-nc+2;
    gen v,w;
    for (ivector_iter it=cycle.begin();it!=cycle.end();++it) {
        embedding[*it]=true;
    }
    faces.clear();
    faces.reserve(nf);
    faces.push_back(cycle);
    reverse(cycle.begin(),cycle.end());
    faces.push_back(cycle);
    G_faces.reserve(nf);
    G_embedding.reserve(nc);
    vector<graphe> bridges;
    // find bridges with respect to the initial cycle
    demoucron_bridges(embedding,faces,bridges);
    // iterate the Demoucron algorithm, creating one face at a time
    while (!bridges.empty()) {
        // for each bridge, find all faces in which it can be drawn
        admissible.resize(bridges.size());
        for (vector<graphe>::const_iterator it=bridges.begin();it!=bridges.end();++it) {
            i=it-bridges.begin();
            translate_indices_from(*it,it->get_marked_vertices(),contact_vertices);
            assert(contact_vertices.size()>1);
            ivector_iter ct;
            admissible[i].clear();
            for (ivectors_iter ft=faces.begin();ft!=faces.end();++ft) {
                for (ct=contact_vertices.begin();ct!=contact_vertices.end();++ct) {
                    if (find(ft->begin(),ft->end(),*ct)==ft->end())
                        break;
                }
                if (ct==contact_vertices.end())
                    admissible[i].push_back(ft-faces.begin());
            }
            if (admissible[i].empty()) {
                // the graph is not planar
                return false;
            }
        }
        k=-1;
        for (vector<graphe>::const_iterator it=bridges.begin();it!=bridges.end();++it) {
            i=it-bridges.begin();
            if (admissible[i].size()==1) {
                k=i;
                break;
            }
        }
        if (k<0)
            k=rand_integer(bridges.size());
        graphe &bridge=bridges[k];
        graphe G(bridge);
        for (ivectors_iter ft=faces.begin();ft!=faces.end();++ft) {
            n=ft->size();
            for (i=0;i<n;++i) {
                G.add_edge(node_label(ft->at(i)),node_label(ft->at((i+1)%n)));
            }
        }
        // find a path between two points of contact (remove the other contact points)
        v=bridge.node_label(bridge.get_marked_vertices().front());
        w=bridge.node_label(bridge.get_marked_vertices().back());
        vecteur unused_contact_vertices=bridge.get_nodes(bridge.get_marked_vertices());
        unused_contact_vertices.pop_back();
        unused_contact_vertices.erase(unused_contact_vertices.begin());
        if (!unused_contact_vertices.empty())
            bridge.remove_nodes(unused_contact_vertices);
        ivector path=bridge.find_path(bridge.node_index(v),bridge.node_index(w));
        for (ivector::iterator it=path.begin();it!=path.end();++it) {
            embedding[*it=node_index(bridge.node_label(*it))]=true;
        }
        bridges.erase(bridges.begin()+k); // we're done with this bridge
        ivector &admissible_faces=admissible[k];
        // draw to a random admissible face
        f=admissible_faces[rand_integer(admissible_faces.size())];
        ivector &face=faces[f];
        i=find(face.begin(),face.end(),node_index(v))-face.begin();
        j=find(face.begin(),face.end(),node_index(w))-face.begin();
        n=face.size();
        ivector face1((j-i-1+n)%n),face2((i-j-1+n)%n);
        for (int k=(i+1)%n,c=0;k!=j;k=(k+1)%n,++c) face1[c]=face[k];
        for (int k=(j+1)%n,c=0;k!=i;k=(k+1)%n,++c) face2[c]=face[k];
        face2.insert(face2.end(),path.begin(),path.end());
        reverse(path.begin(),path.end());
        face1.insert(face1.end(),path.begin(),path.end());
        faces.erase(faces.begin()+f);
        faces.push_back(face1);
        faces.push_back(face2);
        // append new bridges
        G_embedding.resize(G.node_count());
        fill(G_embedding.begin(),G_embedding.end(),false);
        for (i=0;i<nc;++i) {
            if (embedding[i])
                G_embedding[G.node_index(node_label(i))]=true;
        }
        G_faces.resize(faces.size());
        for (ivectors_iter ft=faces.begin();ft!=faces.end();++ft) {
            translate_indices_to(G,*ft,G_faces[ft-faces.begin()]);
        }
        G.demoucron_bridges(G_embedding,G_faces,bridges);
    }
    // in the end we need to have exactly ec-nc+2 faces by Euler's theorem
    assert(int(faces.size())==ec-nc+2);
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

/* recursively build up a tree of blocks */
void graphe::build_block_tree(int i,ivectors &blocks) {
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
        build_block_tree(j,blocks);
    }
}

/* choose a face to embed between those containing the vertex v */
int graphe::choose_embedding_face(const ivectors &faces,int v) {
    ivector candidates;
    for (ivectors_iter ft=faces.begin();ft!=faces.end();++ft) {
        if (find(ft->begin(),ft->end(),v)!=ft->end())
            candidates.push_back(ft-faces.begin());
    }
    return candidates[rand_integer(candidates.size())];
}

/* embed children blocks to its parent, for each embedding add one temporary edge */
void graphe::embed_children_blocks(int i,ivectors &block_tree,vector<ivectors> &blocks_faces) {
    // find all children of the i-th block
    ivector children;
    for (ivectors_iter it=block_tree.begin();it!=block_tree.end();++it) {
        if (it->at(1)==i)
            children.push_back(it-block_tree.begin());
    }
    if (children.empty())
        return; // this is a leaf node in the tree
    // embed each child to the largest available face of the parent
    ivectors &parent_faces=blocks_faces[i];
    int c,pf,cf,k,n;
    ivector::iterator vt,wt;
    for (ivector_iter it=children.begin();it!=children.end();++it) {
        embed_children_blocks(*it,block_tree,blocks_faces);
        ivectors &child_faces=blocks_faces[*it];
        c=block_tree[*it][2]; // articulation vertex connecting the child with its parent
        // find the suitable parent and child faces that contain c
        pf=choose_embedding_face(parent_faces,c);
        cf=choose_embedding_face(child_faces,c);
        assert(pf>=0 && cf>=0);
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
        assert(*vt!=*wt);
        add_temporary_edge(*vt,*wt);
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

/* return the index of the largest face of the given embedding */
int graphe::choose_outer_face(const ivectors &faces) {
    int f,fsize,maxsize=0;
    for (ivectors_iter it=faces.begin();it!=faces.end();++it) {
        if ((fsize=it->size())>maxsize) {
            maxsize=fsize;
            f=it-faces.begin();
        }
    }
    return f;
}

/* finds planar embedding of a connected graph as a list of faces,
 * returns the index of the outer face or -1 if the graph is not planar */
int graphe::planar_embedding(ivectors &faces) {
    if (edge_count()+6>3*node_count())
        return -1;
    // split graph to blocks
    vector<ipairs> blocks;
    find_blocks(blocks);
    if (blocks.size()==1) {
        if (!demoucron(faces))
            return -1;
    } else {
        // there exist at least one articulation point
        vector<ivectors> blocks_faces(blocks.size());
        ivector cv=find_cut_vertices();
        int i=0,nf;
        for (vector<ipairs>::const_iterator it=blocks.begin();it!=blocks.end();++it) {
            // test each block separately
            graphe G(ctx);
            for (ipairs::const_iterator jt=it->begin();jt!=it->end();++jt) {
                G.add_edge(node_label(jt->first),node_label(jt->second));
            }
            ivectors &block_faces=blocks_faces[i++];
            if (G.node_count()>2) {
                // block has three or more vertices
                if (int(it->size())+6>3*G.node_count() || !G.demoucron(block_faces))
                    return -1;
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
           by adding temporary edges. */
        // make a tree of blocks
        ivectors block_tree(blocks.size());
        for (ivectors::iterator it=block_tree.begin();it!=block_tree.end();++it) {
            it->push_back(0);
            it->push_back(-1);
            it->push_back(-1);
            ivectors &fv=blocks_faces[it-block_tree.begin()];
            it->insert(it->end(),fv.back().begin(),fv.back().end());
        }
        build_block_tree(0,block_tree);
        for (vector<ivectors>::iterator it=blocks_faces.begin();it!=blocks_faces.end();++it) {
            it->pop_back();
        }
        // embed all blocks to the root block by climbing up the tree recursively
        for (ivectors_iter bt=block_tree.begin();bt!=block_tree.end();++bt) {
            if (bt->at(1)<0) {
                // root found
                i=bt-block_tree.begin();
                embed_children_blocks(i,block_tree,blocks_faces);
                ivectors &block_faces=blocks_faces[i];
                faces.resize(block_faces.size());
                for (ivectors::iterator it=block_faces.begin();it!=block_faces.end();++it) {
                    faces[it-block_faces.begin()].swap(*it);
                }
                break;
            }
        }
    }
    return choose_outer_face(faces);
}

/* triangulate faces by adding a vertex in the center of each face and
 * connecting it with the other vertices of the face */
void graphe::subdivide_faces(const ivectors &faces,int f0) {
    int v,N=node_label(node_count()-1).val;
    for (ivectors_iter ft=faces.begin();ft!=faces.end();++ft) {
        if (f0==int(ft-faces.begin()))
            continue;
        v=add_node(++N);
        for (ivector_iter it=ft->begin();it!=ft->end();++it) {
            add_edge(v,*it);
        }
    }
}

/* compute periphericity of the vertices with respect to
 * the outer face using BFS starting in each of outer vertices */
void graphe::periphericity(const ivector &outer_face,ivector &p) {
    queue<int> q;
    int level,v,w;
    std::fill(p.begin(),p.end(),RAND_MAX);
    for (ivector_iter jt=outer_face.begin();jt!=outer_face.end();++jt) {
        p[*jt]=0;
    }
    for (ivector_iter it=outer_face.begin();it!=outer_face.end();++it) {
        unvisit_all_vertices();
        q.push(*it);
        level=1;
        while (!q.empty()) {
            v=q.front();
            ivector adj=adjacent_nodes(v);
            for (ivector_iter wt=adj.begin();wt!=adj.end();++wt) {
                w=*wt;
                vertex &wert=nodes[w];
                if (wert.is_visited() || p[w]==0) // skip the outer vertices
                    continue;
                if (level<p[w])
                    p[w]=level;
                q.push(w);
                wert.set_visited(true);
            }
            ++level;
            q.pop();
        }
    }
}

/*
 * TREE NODE POSITIONING ALGORITHM
 */

/* traverse the tree from top to bottom using DFS */
void graphe::tree_node_positioner::walk(int i,int pass,int level,double modsum) {
    vertex &v=G->node(i);
    v.set_visited(true);
    double m=0;
    if (pass==3) {
        point &p=x->at(i);
        p.resize(2);
        p[0]=v.prelim()+modsum;
        p[1]=-level*vsep;
        m=v.modifier();
    }
    int j;
    bool isleaf=true;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        j=*it;
        if (j<0) j=-j-1;
        vertex &w=G->node(j);
        if (w.is_visited())
            continue;
        if (pass==1)
            w.set_parent(i);
        walk(j,pass,level+1,modsum+m);
        isleaf=false;
    }
    if (pass==1) {
        v.set_is_leaf(isleaf);
        ++counters[level];
        depth=std::max(depth,level+1);
    } else if (pass==2) {
        v.set_position(counters[level]++);
        levels[level][v.position()]=i;
    }
}

/* set prelim and modifier for each node in V (they are on the same level) */
void graphe::tree_node_positioner::process_level(int i) {
    ivector &L=levels[i];
    int lastp=G->node(L.front()).parent(),p,n=0,m=placed.size();
    double ppos=0,xpos=0,dist,shift=0,step=1,min_dist;
    if (m>0) {
        vertex &w=G->node(placed.front());
        xpos=w.prelim()-w.position()*hsep;
    }
    for (ivector_iter it=L.begin();it!=L.end();++it) {
        vertex &v=G->node(*it);
        p=v.parent();
        if (p!=lastp) {
            G->node(lastp).set_prelim(ppos/n);
            placed.push(lastp);
            lastp=p;
            ppos=0;
            n=0;
            if (m>0 && v.is_leaf()) {
                vertex &w=G->node(placed.front());
                if (p==w.parent())
                    xpos=w.prelim()+shift-(w.position()-v.position())*hsep;
            }
        }
        if (v.is_leaf()) {
            v.set_prelim(xpos);
        } else {
            v.set_prelim(v.prelim()+shift);
            v.set_modifier(shift);
            xpos=v.prelim();
            step=1;
            placed.pop();
            --m;
            if (m>0) {
                vertex &w=G->node(placed.front());
                dist=w.prelim()+shift-xpos;
                min_dist=(w.position()-v.position())*hsep;
                if (dist<min_dist)
                    shift+=min_dist-dist;
                else if (v.parent()==w.parent())
                    step=dist/min_dist;
            }
        }
        ppos+=v.prelim();
        ++n;
        xpos+=hsep*step;
    }
    assert(m==0);
    G->node(lastp).set_prelim(ppos/n);
    placed.push(lastp);
}

/* node positioning procedure */
double graphe::tree_node_positioner::positioning(int apex) {
    clock_t start=clock();
    walk(apex,1); // first walk: determine tree depth and set ancestors
    // allocate memory for level lists
    clock_t alloc_start=clock();
    levels.resize(depth);
    for (int i=0;i<depth;++i) {
        levels[i].resize(counters[i]);
        counters[i]=0;
    }
    double alloc_time=double(clock()-alloc_start)/CLOCKS_PER_SEC;
    G->unvisit_all_vertices();
    walk(apex,2); // second walk: create levels
    // do node positioning for each level (except the top one) in a single sweep
    for (int level=depth;level-->1;) {
        process_level(level);
    }
    G->unvisit_all_vertices();
    walk(apex,3); // third walk: sum up the modifiers (i.e. move subtrees)
    return double(clock()-start)/CLOCKS_PER_SEC-alloc_time;
}

graphe::tree_node_positioner::tree_node_positioner(graphe *gr,layout *ly,double hs,double vs) {
    G=gr;
    x=ly;
    hsep=hs;
    vsep=vs;
    depth=0;
    int n=G->node_count();
    counters.resize(n,0);
}

/*
 * END OF TREE NODE POSITIONING ALGORITHM IMPLEMENTATION
 */

/* calculate node positions and store them to the specified layout */
double graphe::make_tree_layout(layout &x,double sep,int apex) {
    int n=node_count();
    x.resize(n);
    if (n==0) return 0;
    unset_all_ancestors();
    unvisit_all_vertices();
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->set_prelim(-1);
        it->set_modifier(0);
    }
    // layout the tree
    double hsep=sep,vsep=sep*std::pow(PLASTIC_NUMBER,3);
    tree_node_positioner P(this,&x,hsep,vsep);
    return P.positioning(apex);
}

/* create a random tree with n vertices */
void graphe::make_random_tree(int n) {
    this->clear();
    vecteur labels;
    make_default_vertex_labels(labels,n,0);
    add_nodes(labels);
    labels=*_randperm(labels,ctx)._VECTptr;
    vecteur src;
    src.push_back(labels.pop_back());
    gen v,w;
    while (!labels.empty()) {
        v=_rand(src,ctx);
        w=labels.pop_back();
        add_edge(v,w);
        src.push_back(w);
    }
}

/* measure tree layout speed */
void graphe::tree_layout_test() {
    layout x;
    double time=0;
    sleep(1);
    for (int k=0;k<=5;++k) {
        make_random_tree(5000);
        for (int i=0;i<10;++i) {
            time+=1000.0*make_tree_layout(x,20,rand_integer(5000));
        }
    }
    time/=50.0;
    cout << time << "," << endl;
}

/* create a random planar biconnected graph with n vertices */
void graphe::make_random_planar(int n) {
    this->clear();
    // start with a random maximal planar graph with n vertices
    vecteur labels;
    make_default_vertex_labels(labels,n,0);
    add_nodes(labels);
    ivectors faces;
    ivector face(3),old_face;
    for (int i=0;i<3;++i) {
        face[i]=i;
        add_edge(i,(i+1)%3);
    }
    faces.push_back(face);
    ivector outer_face(face.begin(),face.end());
    reverse(outer_face.begin(),outer_face.end());
    faces.push_back(outer_face);
    int k;
    for (int i=3;i<n;++i) {
        k=rand_integer(faces.size());
        ivector &face_k=faces[k];
        old_face.assign(face_k.begin(),face_k.end());
        face[0]=i;
        for (int j=0;j<3;++j) {
            face[1]=old_face[j];
            face[2]=old_face[(j+1)%3];
            add_edge(i,old_face[j]);
            faces.push_back(face);
        }
        faces.erase(faces.begin()+k);
    }
    // remove each edge with a certain probability
    ipairs E;
    get_edges_as_pairs(E,false);
    int d1,d2,m;
    for (ipairs::const_iterator it=E.begin();it!=E.end();++it) {
        d1=degree(it->first,false);
        d2=degree(it->second,false);
        if (d1<4 || d2<4)
            continue;
        k=giac_rand(ctx);
        m=(d1+d2)/4;
        if (k%m!=0)
            remove_edge(*it);
    }
    // check that the resulting graph is biconnected; if not, repeat the process
    vector<ipairs> blocks;
    find_blocks(blocks);
    if (blocks.size()>1)
        make_random_planar(n);
}

/* measure speed of the triangulator */
void graphe::triangulator_test() {
    ivectors faces;
    double time;
    int f;
    for (int k=25;k<=700;k+=25) {
        make_random_planar(k);
        faces.clear();
        assert(demoucron(faces));
        f=rand_integer(faces.size());
        time=0.0;
        for (int i=0;i<10;++i) {
            triangulator T(this,&faces);
            clock_t start=clock();
            T.triangulate(f);
            clock_t end=clock();
            time+=1000*double(end-start)/CLOCKS_PER_SEC;
            remove_temporary_edges();
        }
        time/=10.0;
        cout << time << "," << endl;
    }
}

/* sort rectangles by height */
void graphe::sort_rectangles(vector<rectangle> &rectangles) {
    struct comparator {
        bool operator()(const rectangle &r1,const rectangle &r2) const {
            return r1.height()<r2.height();
        }
    } comp;
    sort(rectangles.begin(),rectangles.end(),comp);
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
            if (blanks[i].width()>=it->width() && blanks[i].height()>=it->height() &&
                    (k==-1 || blanks[i].x()<xpos)) {
                k=i;
                xpos=blanks[k].x();
            }
        }
        assert(k>=0);
        // store blank dimensions and position, for splitting
        rectangle &blank=blanks[k];
        ypos=blank.y();
        w=blank.width();
        h=blank.height();
        blanks.erase(blanks.begin()+k); // delete blank in which the rectangle is inserted
        // insert the rectangle (breaking the deleted blank into two or four pieces)
        dpair newpos=make_pair(xpos,ypos);
        dpair &oldpos=embedding[it-rectangles.begin()];
        if (newpos!=oldpos) {
            oldpos=newpos;
            embedding_changed=true;
        }
        // add new (smaller) blanks obtained by splitting the deleted blank
        if ((dw=w-it->width())>0)
            blanks.push_back(rectangle(xpos+it->width(),ypos,dw,it->height()));
        if ((dh=h-it->height())>0)
            blanks.push_back(rectangle(xpos,ypos+it->height(),it->width(),dh));
        if (dw>0 && dh>0)
            blanks.push_back(rectangle(xpos+it->width(),ypos+it->height(),dw,dh));
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
        total_area+=it->width()*it->height();
    }
    // step = the length of a negligible part of the shorter side of the smallest rectangle
    const rectangle &smallest=rectangles.back(),&largest=rectangles.front();
    double step=std::min(smallest.width(),smallest.height())*std::pow(PLASTIC_NUMBER,-14);
    double ew=DBL_MAX,eh=largest.height(); // initial enclosing rectangle has an "unlimited" width
    double perimeter,best_perimeter=DBL_MAX,d;
    dpairs embedding(n,make_pair(-1,-1)),best_embedding;
    while (ew>largest.width()) { // loop breaks after a stacked embedding is obtained
        if (pack_rectangles(rectangles,embedding,ew,eh)) {
            ew=eh=0;
            i=0;
            // find the smallest enclosing rectangle containing embedding
            for (dpairs::const_iterator it=embedding.begin();it!=embedding.end();++it) {
                if ((d=it->first+rectangles[i].width())>ew)
                    ew=d;
                if ((d=it->second+rectangles[i].height())>eh)
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

/* return true iff an isomorphic copy with vertices permuted according to sigma is constructed */
bool graphe::isomorphic_copy(graphe &G,const ivector &sigma) {
    if (int(sigma.size())!=node_count())
        return false;
    G.set_name(graph_name);
    G.register_user_tags(user_tags);
    G.set_graph_attributes(attributes);
    // add vertices
    for (ivector_iter it=sigma.begin();it!=sigma.end();++it) {
        if (*it<0 || *it>=node_count())
            return false;
        G.add_node(node_label(*it),nodes[*it].attributes());
    }
    if (G.node_count()!=node_count())
        return false;
    // add edges
    int i;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=sigma[it-nodes.begin()];
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            if (is_directed() || int(it-nodes.begin())<*jt) {
                G.add_edge(i,sigma[*jt],it->neighbor_attributes(*jt));
            }
        }
    }
    return true;
}

/* return true iff the vertices were successfully relabeled with 'labels' */
bool graphe::relabel_nodes(const vecteur &labels) {
    int n=node_count();
    if (int(labels.size())<n)
        return false;
    int i=0;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->set_symbol(labels[i++]);
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

/* remove every edge from E that is not crossed by some other edge */
bool graphe::has_crossing_edges(const layout &x,const ipairs &E) const {
    point r(2),s(2),crossing(2);
    int i1,j1,i2,j2;
    for (ipairs::const_iterator it=E.begin();it!=E.end();++it) {
        i1=it->first;
        j1=it->second;
        for (ipairs::const_iterator jt=it+1;jt!=E.end();++jt) {
            i2=jt->first;
            j2=jt->second;
            if (i1==i2 || j1==j2 || i1==j2 || i2==j1)
                continue;
            const point &p=x[i1];
            const point &q=x[i2];
            copy_point(x[j1],r);
            copy_point(x[j2],s);
            subtract_point(r,p);
            subtract_point(s,q);
            if (segments_crossing(p,r,q,s,crossing))
                return true;
        }
    }
    return false;
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
    // sort points by angle
    struct comparator {
        const layout *x;
        const point *lp;
        comparator(const layout *x_orig,const point *lp_orig) {
            x=x_orig;
            lp=lp_orig;
        }
        bool operator()(int i,int j) const {
            const point &pt1=x->at(i),&pt2=x->at(j);
            if (pt1[1]==pt2[1])
                return pt1[0]<pt2[0];
            double dx1=pt1[0]-lp->front(),dx2=pt2[0]-lp->front();
            double dy1=pt1[1]-lp->back(),dy2=pt2[1]-lp->back();
            double r=std::sqrt((dx2*dx2+dy2*dy2)/(dx1*dx1+dy1*dy1));
            return dx2<dx1*r;
        }
    } comp(&x,&x[i_ymin]);
    sort(vp.begin(),vp.end(),comp);
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
bool graphe::segments_crossing(const point &p,const point &r,const point &q,const point &s,point &crossing) {
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

/* return true iff the point p can be projected onto a segment [q,r] (also compute the projection) */
bool graphe::point2segment_projection(const point &p,const point &q,const point &r,point &proj) {
    assert(p.size()==2 && q.size()==2 && r.size()==2);
    proj.resize(2);
    point a(2),b(2),c(2);
    copy_point(p,a);
    subtract_point(a,q);
    copy_point(r,b);
    subtract_point(b,q);
    copy_point(r,c);
    subtract_point(c,p);
    double ab;
    if ((ab=point_dotprod(a,b))*point_dotprod(b,c)<=0)
        return false;
    copy_point(b,proj);
    double bn2=point_displacement(b,false);
    if (bn2==0)
        return false;
    scale_point(proj,ab/bn2);
    add_point(proj,q);
    return true;
}

/* return the area enclosed by a non self-intersecting polygon
 * with vertices v (sorted in counterclockwise order) */
double graphe::polyarea(const layout &x) {
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
        return polyarea(hull);
    }
    // v defines a subgraph
    graphe G(ctx);
    induce_subgraph(v,G);
    return G.subgraph_area(x);
}

/* copy this graph to G with edge crossings promoted to vertices (2D layout is required) */
void graphe::promote_edge_crossings(layout &x) {
    ipairs E;
    int i,i1,j1,i2,j2;
    get_edges_as_pairs(E);
    // get the last symbol (must be an integer)
    const gen &last_node=nodes.back().symbol();
    assert(last_node.is_integer());
    int N=last_node.val;
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
            if (segments_crossing(p,r,q,s,crossing)) {
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
    graphe G(ctx);
    map<ipair,double> edge_score;
    int k=0,i,j;
    for (vector<double>::const_iterator it=sc.begin();it!=sc.end();++it) {
        if (*it>0) {
            const ipair &e=E[k];
            const vertex &V=nodes[e.first],&W=nodes[e.second];
            i=G.add_node(V.symbol(),V.attributes());
            j=G.add_node(W.symbol(),W.attributes());
            G.add_edge(i,j);
            edge_score[make_pair(i<j?i:j,i<j?j:i)]=*it;
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
            i=*ct;
            for (ivector_iter dt=ct+1;dt!=it->end();++dt) {
                j=*dt;
                if (!G.has_edge(i,j))
                    continue;
                avg_score+=edge_score[make_pair(i<j?i:j,i<j?j:i)];
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
    graphe G(ctx);
    underlying(G);
    vecteur labels;
    int nc=G.node_count();
    make_default_vertex_labels(labels,nc,0);
    G.relabel_nodes(labels);
    G.promote_edge_crossings(x);
    ipairs E;
    G.get_edges_as_pairs(E);
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
    struct comparator {
        point *c;
        comparator(point *c_orig) {
            c=c_orig;
        }
        bool operator()(const pair<ipair,point> &p1,const pair<ipair,point> &p2) const {
            return squared_distance(*c,p1.second)<squared_distance(*c,p2.second);
        }
    } comp(&center);
    sort(axes.begin(),axes.end(),comp);
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
bool graphe::planar_layout(layout &x,double K) {
    int n=node_count(),m,f,N,v,w,d,dl,dr;
    vecteur labels,old_labels=vertices();
    make_default_vertex_labels(labels,n,0);
    relabel_nodes(labels);
    ivectors faces;
    faces.clear();
    // create the faces (adding temporary edges if necessary),
    // return the index of the outer face
    f=planar_embedding(faces);
    if (f<0)
        return false; // the graph is not planar
    //subdivide_faces(faces,f);
    triangulator T(this,&faces);
    T.triangulate(f);
    // create a fake outer face
    ivector &outer_face=faces[f];
    N=node_label(node_count()-1).val;
    m=outer_face.size();
    ivector new_outer_face(m),degrees(m);
    for (int i=0;i<m;++i) {
        v=add_node(++N);
        w=outer_face[i];
        add_edge(v,w);
        new_outer_face[i]=v;
    }
    for (int i=0;i<m;++i) {
        v=(i+m-1)%m;
        w=(i+1)%m;
        d=degrees[i];
        dl=degrees[v];
        dr=degrees[w];
        if (d<dl)
            add_edge(outer_face[i],new_outer_face[v]);
        if (d<dr)
            add_edge(outer_face[i],new_outer_face[w]);
    }
    // create the layout
    x.resize(node_count());
    plestenjak_layout(x,new_outer_face);
    // remove temporary vertices and edges, if any
    while (node_count()>n) {
        remove_node(node_count()-1);
    }
    remove_temporary_edges();
    x.resize(n);
    scale_layout(x,K);
    relabel_nodes(old_labels);
    return true;
}

/* compute vertex positions */
graphe::layout graphe::make_layout(double K,gt_layout_style style) {
    int dim=2;
    layout x;
    bool is_tree=false;
    switch (style) {
    case _GT_STYLE_3D:
        dim=3;
    case _GT_STYLE_SPRING:
        force_directed_placement_multilevel(x,dim,K);
        break;
    case _GT_STYLE_PLANAR:
        if (find_cycle().size()==0) {
            // graph is a tree
            is_tree=true;
            make_tree_layout(x,K/(double)node_count(),0);
        } else if (!planar_layout(x,K))
            return layout(0);
        break;
    case _GT_STYLE_CIRCLE:

        break;
    }
    // center the layout in origin
    point c=layout_center(x);
    scale_point(c,-1);
    translate_layout(x,c);
    // rotate layout to expose symmetry
    if (!is_tree && dim==5) {
        // Find the best symmetry axis, and rotate the graph
        // to make the symmetry more prominent to a human.
        point axis=axis_of_symmetry(x);
        double a=axis[0],b=axis[1];
        if (a!=0) {
            rotate_layout(x,M_PI_2+std::atan(b/a));
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
        }
    }
    return x;
}

/* customize the Giac display */
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
    int i,j;
    int color,width=x.front().size()==2?_LINE_WIDTH_2:_LINE_WIDTH_1;
    width=_LINE_WIDTH_1;
    ipairs E;
    get_edges_as_pairs(E);
    attrib_iter jt;
    for (ipairs::const_iterator it=E.begin();it!=E.end();++it) {
        i=it->first;
        j=it->second;
        const point &p=x[i],&q=x[j];
        const attrib &attr=edge_attributes(i,j);
        color=_BLACK;
        if ((jt=attr.find(_GT_ATTRIB_COLOR))!=attr.end())
            color=jt->second.val;
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
    width=_POINT_WIDTH_2;
    for (int i=0;i<n;++i) {
        const point &p=x[i];
        color=_YELLOW;
        color=_BLACK;
        if (get_node_attribute(i,_GT_ATTRIB_COLOR,val))
            color=val.val;
        append_vertex(v,p,color,width);
    }
}

/* return the best quadrant for the placement of the i-th vertex label
 * (minimize collision with the adjacent edges) */
int graphe::node_label_best_quadrant(const layout &x,const point &center,int i) const {
    ivector adj=adjacent_nodes(i);
    int n=adj.size(),best_quadrant;
    vector<double> adj_phi(n);
    const point &p=x[i];
    for (int j=0;j<n;++j) {
        const point &q=x[adj[j]];
        adj_phi[j]=std::atan2(q[1]-p[1],q[0]-p[0]);
    }
    if (adj_phi.size()==1) {
        double phi=adj_phi.front();
        if (phi<=-M_PI_2)
            return _QUADRANT1;
        if (phi<=0)
            return _QUADRANT2;
        if (phi<=M_PI_2)
            return _QUADRANT3;
        return _QUADRANT4;
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
        append_label(v,p,nodes[i].symbol(),quadrant);
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
