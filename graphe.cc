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

#include "giacPCH.h"
#include "giac.h"
#include "graphe.h"
#include <sstream>
#include <ctype.h>
#include <math.h>
#include <ctime>
#include <bitset>

using namespace std;

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

#define PLASTIC_NUMBER 1.32471795724474602596090885
#define MARGIN_FACTOR 0.139680581996
#define NEGLIGIBILITY_FACTOR 0.0195106649868
#define PLESTENJAK_MAX_TRIES 6
#define SYMMETRY_DETECTION_TIMEOUT 1.5
#define PACKING_ASPECT_RATIO 3.08

const gen graphe::VRAI=gen(1).change_subtype(_INT_BOOLEAN);
const gen graphe::FAUX=gen(0).change_subtype(_INT_BOOLEAN);
bool graphe::verbose=true;
int graphe::default_edge_color=_BLUE;
int graphe::default_edge_width=_LINE_WIDTH_2;
int graphe::default_highlighted_edge_color=_RED;
int graphe::default_highlighted_vertex_color=_GREEN;
int graphe::default_vertex_color=_YELLOW;
int graphe::default_vertex_label_color=_BLACK;
// initialize special graphs
/*
 * Graphs are initialized with adjacency lists of integers or strings. Each
 * list starts with the corresponding vertex, followed by its neighbors, and
 * ends with -1 or an empty string.
 * The graph specification terminates with -2 (for integers) or NULL (for strings).
 */
const int graphe::clebsch_graph[] = {
    0,      1,2,4,8,15,-1,
    1,      3,5,9,14,-1,
    2,      3,6,10,13,-1,
    3,      7,11,12,-1,
    4,      5,6,11,12,-1,
    5,      7,10,13,-1,
    6,      7,9,14,-1,
    7,      8,15,-1,
    8,      9,10,12,-1,
    9,      11,13,-1,
    10,     11,14,-1,
    11,     15,-1,
    12,     13,14,-1,
    13,     15,-1,
    14,     15,-1,
    -2
};
const char* graphe::coxeter_graph[] = {
    "a1",       "a2","a7","z1","",
    "a2",       "a3","z2","",
    "a3",       "a4","z3","",
    "a4",       "a5","z4","",
    "a5",       "a6","z5","",
    "a6",       "a7","z6","",
    "a7",       "z7","",
    "b1",       "b3","b6","z1","",
    "b2",       "b4","b7","z2","",
    "b3",       "b5","z3","",
    "b4",       "b6","z4","",
    "b5",       "b7","z5","",
    "b6",       "z6","",
    "b7",       "z7","",
    "c1",       "c4","c5","z1","",
    "c2",       "c5","c6","z2","",
    "c3",       "c6","c7","z3","",
    "c4",       "c7","z4","",
    "c5",       "z5","",
    "c6",       "z6","",
    "c7",       "z7","",
    NULL
};
const int graphe::dyck_graph[] = {
    1,      2,20,32,-1,
    2,      3,7,-1,
    3,      4,30,-1,
    4,      5,17,-1,
    5,      6,24,-1,
    6,      7,11,-1,
    7,      8,-1,
    8,      9,21,-1,
    9,      10,28,-1,
    10,     11,15,-1,
    11,     12,-1,
    12,     13,25,-1,
    13,     14,32,-1,
    14,     15,19,-1,
    15,     16,-1,
    16,     17,29,-1,
    17,     18,-1,
    18,     19,23,-1,
    19,     20,-1,
    20,     21,-1,
    21,     22,-1,
    22,     23,27,-1,
    23,     24,-1,
    24,     25,-1,
    25,     26,-1,
    26,     27,31,-1,
    27,     28,-1,
    28,     29,-1,
    29,     30,-1,
    30,     31,-1,
    31,     32,-1,
    -2
};
const int graphe::grinberg_graph[] = {
    1,      2,3,4,-1,
    2,      5,7,-1,
    3,      6,10,-1,
    4,      8,9,-1,
    5,      6,21,-1,
    6,      22,-1,
    7,      8,30,-1,
    8,      29,-1,
    9,      10,14,-1,
    10,     13,-1,
    11,     12,13,14,-1,
    12,     15,17,-1,
    13,     16,-1,
    14,     18,-1,
    15,     16,39,-1,
    16,     46,-1,
    17,     18,40,-1,
    18,     45,-1,
    19,     20,21,22,-1,
    20,     23,25,-1,
    21,     24,-1,
    22,     26,-1,
    23,     24,36,-1,
    24,     44,-1,
    25,     26,37,-1,
    26,     46,-1,
    27,     28,29,30,-1,
    28,     31,33,-1,
    29,     32,-1,
    30,     34,-1,
    31,     32,42,-1,
    32,     45,-1,
    33,     34,43,-1,
    34,     44,-1,
    35,     36,43,44,-1,
    36,     37,-1,
    37,     38,-1,
    38,     39,46,-1,
    39,     40,-1,
    40,     41,-1,
    41,     42,45,-1,
    42,     43,-1,
    -2
};
const int graphe::grotzsch_graph[] = {
    1,      2,5,7,10,-1,
    2,      3,6,8,-1,
    3,      4,7,9,-1,
    4,      5,8,10,-1,
    5,      6,9,-1,
    6,      11,-1,
    7,      11,-1,
    8,      11,-1,
    9,      11,-1,
    10,     11,-1,
    -2
};
const int graphe::heawood_graph[] = {
    1,      2,6,14,-1,
    2,      3,11,-1,
    3,      4,8,-1,
    4,      5,13,-1,
    5,      6,10,-1,
    6,      7,-1,
    7,      8,12,-1,
    8,      9,-1,
    9,      10,14,-1,
    10,     11,-1,
    11,     12,-1,
    12,     13,-1,
    13,     14,-1,
    -2
};
const int graphe::herschel_graph[] = {
    1,      2,4,5,-1,
    2,      3,6,7,-1,
    3,      4,8,-1,
    4,      9,10,-1,
    5,      6,10,-1,
    6,      11,-1,
    7,      8,11,-1,
    8,      9,-1,
    9,      11,-1,
    10,     11,-1,
    -2
};
const int graphe::mcgee_graph[] = {
    1,      2,13,24,-1,
    2,      3,9,-1,
    3,      4,20,-1,
    4,      5,16,-1,
    5,      6,12,-1,
    6,      7,23,-1,
    7,      8,19,-1,
    8,      9,15,-1,
    9,      10,-1,
    10,     11,22,-1,
    11,     12,18,-1,
    12,     13,-1,
    13,     14,-1,
    14,     15,21,-1,
    15,     16,-1,
    16,     17,-1,
    17,     18,24,-1,
    18,     19,-1,
    19,     20,-1,
    20,     21,-1,
    21,     22,-1,
    22,     23,-1,
    23,     24,-1,
    -2
};
const int graphe::pappus_graph[] = {
    1,      2,6,18,-1,
    2,      3,9,-1,
    3,      4,14,-1,
    4,      5,11,-1,
    5,      6,16,-1,
    6,      7,-1,
    7,      8,12,-1,
    8,      9,15,-1,
    9,      10,-1,
    10,     11,17,-1,
    11,     12,-1,
    12,     13,-1,
    13,     14,18,-1,
    14,     15,-1,
    15,     16,-1,
    16,     17,-1,
    17,     18,-1,
    -2
};
const int graphe::robertson_graph[] = {
    1,      2,5,9,19,-1,
    2,      3,7,13,-1,
    3,      4,10,15,-1,
    4,      5,8,18,-1,
    5,      6,12,-1,
    6,      7,14,17,-1,
    7,      8,11,-1,
    8,      9,16,-1,
    9,      10,14,-1,
    10,     11,17,-1,
    11,     12,19,-1,
    12,     13,16,-1,
    13,     14,18,-1,
    14,     15,-1,
    15,     16,19,-1,
    16,     17,-1,
    17,     18,-1,
    18,     19,-1,
    -2
};
const int graphe::soccer_ball_graph[] = {
    1,      2,5,6,-1,
    2,      3,11,-1,
    3,      4,16,-1,
    4,      5,21,-1,
    5,      26,-1,
    6,      7,10,-1,
    7,      8,30,-1,
    8,      9,49,-1,
    9,      10,53,-1,
    10,     12,-1,
    11,     12,15,-1,
    12,     13,-1,
    13,     14,54,-1,
    14,     15,58,-1,
    15,     17,-1,
    16,     17,20,-1,
    17,     18,-1,
    18,     19,59,-1,
    19,     20,38,-1,
    20,     22,-1,
    21,     22,25,-1,
    22,     23,-1,
    23,     24,39,-1,
    24,     25,43,-1,
    25,     27,-1,
    26,     27,30,-1,
    27,     28,-1,
    28,     29,44,-1,
    29,     30,48,-1,
    31,     32,35,36,-1,
    32,     33,41,-1,
    33,     34,46,-1,
    34,     35,51,-1,
    35,     56,-1,
    36,     37,40,-1,
    37,     38,60,-1,
    38,     39,-1,
    39,     40,-1,
    40,     42,-1,
    41,     42,45,-1,
    42,     43,-1,
    43,     44,-1,
    44,     45,-1,
    45,     47,-1,
    46,     47,50,-1,
    47,     48,-1,
    48,     49,-1,
    49,     50,-1,
    50,     52,-1,
    51,     52,55,-1,
    52,     53,-1,
    53,     54,-1,
    54,     55,-1,
    55,     57,-1,
    56,     57,60,-1,
    57,     58,-1,
    58,     59,-1,
    59,     60,-1,
    -2
};
const int graphe::tetrahedron_graph[] = {
    1,      2,3,4,-1,
    2,      3,4,-1,
    3,      4,-1,
    -2
};
const int graphe::octahedron_graph[] = {
    1,      3,6,5,4,-1,
    2,      3,4,5,6,-1,
    3,      5,6,-1,
    4,      5,6,-1,
    -2
};
const int graphe::dodecahedron_graph[] = {
    1,      2,5,6,-1,
    2,      3,7,-1,
    3,      4,8,-1,
    4,      5,9,-1,
    5,      10,-1,
    6,      11,15,-1,
    7,      11,12,-1,
    8,      12,13,-1,
    9,      13,14,-1,
    10,     14,15,-1,
    11,     16,-1,
    12,     17,-1,
    13,     18,-1,
    14,     19,-1,
    15,     20,-1,
    16,     17,20,-1,
    17,     18,-1,
    18,     19,-1,
    19,     20,-1,
    -2
};
const int graphe::icosahedron_graph[] = {
    1,      2,3,4,5,9,-1,
    2,      3,5,6,7,-1,
    3,      7,8,9,-1,
    4,      5,9,10,11,-1,
    5,      6,10,-1,
    6,      7,10,12,-1,
    7,      8,12,-1,
    8,      9,11,12,-1,
    9,      11,-1,
    10,     11,12,-1,
    11,     12,-1,
    -2
};
const int graphe::levi_graph[] = {
    1,      2,18,30,-1,
    2,      3,23,-1,
    3,      4,10,-1,
    4,      5,27,-1,
    5,      6,14,-1,
    6,      7,19,-1,
    7,      8,24,-1,
    8,      9,29,-1,
    9,      10,16,-1,
    10,     11,-1,
    11,     12,20,-1,
    12,     13,25,-1,
    13,     14,30,-1,
    14,     15,-1,
    15,     16,22,-1,
    16,     17,-1,
    17,     18,26,-1,
    18,     19,-1,
    19,     20,-1,
    20,     21,-1,
    21,     22,28,-1,
    22,     23,-1,
    23,     24,-1,
    24,     25,-1,
    25,     26,-1,
    26,     27,-1,
    27,     28,-1,
    28,     29,-1,
    29,     30,-1,
    -2
};
const int graphe::ljubljana_graph_lcf[] = {
    47,-23,-31,39,25,-21,-31,-41,25,15,29,-41,-19,15,-49,33,39,-35,-21,17,-33,49,41,31,
    -15,-29,41,31,-15,-25,21,31,-51,-25,23,9,-17,51,35,-29,21,-51,-39,33,-9,-51,51,-47,
    -33,19,51,-21,29,21,-31,-39,0
};
const int graphe::harries_graph_lcf[] = {
    -35,-27,27,-23,15,-15,-9,-35,23,-27,27,9,15,-15,0
};
const int graphe::harries_wong_graph_lcf[] = {
    9,25,31,-17,17,33,9,-29,-15,-9,9,25,-25,29,17,-9,9,-27,35,-9,9,-17,21,27,-29,-9,-25,13,19,
    -9,-33,-17,19,-31,27,11,-25,29,-33,13,-13,21,-29,-21,25,9,-11,-19,29,9,-27,-19,-13,-35,-9,
    9,17,25,-9,9,27,-27,-21,15,-9,29,-29,33,-9,-25,0
};

/* messages */

void graphe::message(const char *str) {
    if (verbose)
        *logptr(ctx) << str << endl;
}

void graphe::message(const char *format,int a) {
    char buffer[256];
    sprintf(buffer,format,a);
    if (verbose)
        *logptr(ctx) << buffer << endl;
}

void graphe::message(const char *format,int a,int b) {
    char buffer[256];
    sprintf(buffer,format,a,b);
    if (verbose)
        *logptr(ctx) << buffer << endl;
}

void graphe::message(const char *format,int a,int b,int c) {
    char buffer[256];
    sprintf(buffer,format,a,b,c);
    if (verbose)
        *logptr(ctx) << buffer << endl;
}

gen graphe::plusinf() {
    return symbolic(at_plus,_IDNT_infinity());
}

/* convert list of lists of integers into vecteur of vecteurs */
void graphe::ivectors2vecteur(const ivectors &v,vecteur &res,bool sort_all) const {
    res.resize(v.size());
    for (ivectors_iter it=v.begin();it!=v.end();++it) {
        res[it-v.begin()]=sort_all?_sort(get_nodes(*it),ctx):get_nodes(*it);
    }
}

/* vertex class implementation */
graphe::vertex::vertex() {
    m_label=0;
    m_subgraph=-1;
    m_visited=false;
    m_on_stack=false;
    m_ancestor=-1;
    m_low=-1;
    m_disc=-1;
    m_prelim=0;
    m_modifier=0;
    m_isleaf=false;
    m_position=0;
    m_gaps=0;
    m_embedded=false;
}

void graphe::vertex::assign(const vertex &other) {
    m_label=other.label();
    m_subgraph=other.subgraph();
    m_visited=other.is_visited();
    m_on_stack=other.is_on_stack();
    m_ancestor=other.ancestor();
    m_low=other.low();
    m_disc=other.disc();
    m_prelim=other.prelim();
    m_modifier=other.modifier();
    m_isleaf=other.is_leaf();
    m_position=other.position();
    m_gaps=other.gaps();
    m_embedded=other.is_embedded();
    set_attributes(other.attributes());
    m_neighbors.resize(other.neighbors().size());
    m_neighbor_attributes.clear();
    for (ivector_iter it=other.neighbors().begin();it!=other.neighbors().end();++it) {
        m_neighbors[it-other.neighbors().begin()]=*it;
        copy_attributes(other.neighbor_attributes(*it),m_neighbor_attributes[*it]);
    }
}

graphe::vertex::vertex(const vertex &other) {
    assign(other);
}

graphe::vertex& graphe::vertex::operator =(const vertex &other) {
    assign(other);
    return *this;
}

void graphe::vertex::add_neighbor(int i,const attrib &attr) {
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
        assert (jt!=m_neighbor_attributes.end());
        m_neighbor_attributes.erase(jt);
    }
}

int graphe::first_neighbor_from_subgraph(const vertex &v,int sg) const {
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        if (node(*it).subgraph()==sg)
            return *it;
    }
    return -1;
}

/* just a safety measure, the stack should be empty before calling this function */
void graphe::clear_node_stack() {
    while (!node_stack.empty())
        node_stack.pop();
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

void graphe::dotgraph::assign(const dotgraph &other) {
    m_index=other.index();
    copy_attributes(other.vertex_attributes(),vertex_attr);
    copy_attributes(other.edge_attributes(),edge_attr);
    copy_attributes(other.chain_attributes(),chain_attr);
    m_chain=other.chain();
    pos=other.position();
}

graphe::dotgraph::dotgraph(const dotgraph &other) {
    assign(other);
}

graphe::dotgraph& graphe::dotgraph::operator =(const dotgraph &other) {
    assign(other);
    return *this;
}

/* rectangle class implementation */
graphe::rectangle::rectangle() {
    m_x=m_y=m_width=m_height=0;
    L=NULL;
}

graphe::rectangle::rectangle(double X,double Y,double W,double H,layout *ly) {
    m_x=X;
    m_y=Y;
    m_width=W;
    m_height=H;
    L=ly;
}

void graphe::rectangle::assign(const rectangle &other) {
    m_x=other.x();
    m_y=other.y();
    m_width=other.width();
    m_height=other.height();
    L=other.get_layout();
}

graphe::rectangle::rectangle(const rectangle &rect) {
    assign(rect);
}

graphe::rectangle& graphe::rectangle::operator =(const rectangle &other) {
    assign(other);
    return *this;
}

/* test if g is a real constant */
bool graphe::is_real_number(const gen &g) {
    gen e=_evalf(g,context0);
    return e.type==_DOUBLE_ || e.type==_FLOAT_;
}

/* convert number to binary format and return it as gen of type string */
gen graphe::to_binary(int number,int chars) {
    return str2gen(bitset<1024>((unsigned long)number).to_string().substr(1024-chars),true);
}

/* make a copy of attr */
void graphe::copy_attributes(const attrib &src,attrib &dest) {
    dest.clear();
    for (attrib_iter it=src.begin();it!=src.end();++it) {
        dest.insert(make_pair(it->first,gen(it->second)));
    }
}

/* fill the vecteur V with first n integers (0- or 1- based, depending on the mode) */
void graphe::make_default_labels(vecteur &labels,int n,int n0,int offset) const {
    int ofs=offset<0?array_start(ctx):offset;
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

/* convert word to gen of type string */
gen graphe::word2gen(const string &word) {
    stringstream ss;
    gen g;
    ss << "\"" << word << "\"";
    ss >> g;
    return g;
}

/* convert string to gen */
gen graphe::str2gen(const string &str,bool isstring) {
    stringstream ss(str);
    if (isstring) {
        string buf;
        vector<string> words;
        gen space=_char(32,context0);
        while (ss >> buf) {
            words.push_back(buf);
        }
        vecteur res;
        for (vector<string>::const_iterator it=words.begin();it!=words.end();++it) {
            res.push_back(word2gen(*it));
            if (it+1!=words.end())
                res.push_back(space);
        }
        return _cat(change_subtype(res,_SEQ__VECT),context0);
    }
    gen g;
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

/* compute the union of A and B and store it in U */
int graphe::sets_union(const ivector &A,const ivector &B,ivector &U) {
    U.resize(A.size()+B.size());
    ivector_iter it=set_union(A.begin(),A.end(),B.begin(),B.end(),U.begin());
    U.resize(it-U.begin());
    return U.size();
}

/* compute the intersection of A and B and store it in I */
int graphe::sets_intersection(const ivector &A,const ivector &B,ivector &I) {
    I.resize(std::max(A.size(),B.size()));
    ivector_iter it=set_intersection(A.begin(),A.end(),B.begin(),B.end(),I.begin());
    I.resize(it-I.begin());
    return I.size();
}

/* compute the set difference of A and B and store it in D */
int graphe::sets_difference(const ivector &A,const ivector &B,ivector &D) {
    D.resize(A.size());
    ivector_iter it=set_difference(A.begin(),A.end(),B.begin(),B.end(),D.begin());
    D.resize(it-D.begin());
    return D.size();
}

/* graphe default constructor */
graphe::graphe(GIAC_CONTEXT) {
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

/* graphe constructor, create special graph with the specified name */
graphe::graphe(const string &name,GIAC_CONTEXT) {
    ctx=contextptr;
    set_graph_attribute(_GT_ATTRIB_DIRECTED,FAUX);
    set_graph_attribute(_GT_ATTRIB_WEIGHTED,FAUX);
    if (name=="clebsch") {
        for (int i=0;i<16;++i) {
            add_node(i);
        }
        read_special(clebsch_graph);
        vecteur labels;
        for (int i=0;i<16;++i) {
            labels.push_back(to_binary(i,4));
        }
        relabel_nodes(labels);
    } else if (name=="coxeter") {
        stringstream ss;
        for (int i=1;i<=7;++i) {
            ss.str("");
            ss << "a" << i;
            add_node(str2gen(ss.str(),true));
        }
        read_special(coxeter_graph);
    } else if (name=="desargues") {
        make_petersen_graph(10,3);
    } else if (name=="dodecahedron") {
        for (int i=1;i<=5;++i) {
            add_node(i);
        }
        read_special(dodecahedron_graph);
    } else if (name=="durer") {
        make_petersen_graph(6,2);
    } else if (name=="dyck") {
        for (int i=1;i<=32;++i) {
            add_node(i);
        }
        read_special(dyck_graph);
    } else if (name=="grinberg") {
        for (int i=35;i<=43;++i) {
            add_node(i);
        }
        read_special(grinberg_graph);
    } else if (name=="grotzsch") {
        read_special(grotzsch_graph);
    } else if (name=="harries") {
        make_lcf_graph(harries_graph_lcf,5);
    } else if (name=="harries-wong") {
        make_lcf_graph(harries_wong_graph_lcf,1);
    } else if (name=="heawood") {
        read_special(heawood_graph);
    } else if (name=="herschel") {
        for (int i=1;i<=4;++i) {
            add_node(i);
        }
        read_special(herschel_graph);
    } else if (name=="icosahedron") {
        for (int i=1;i<=3;++i) {
            add_node(i);
        }
        read_special(icosahedron_graph);
    } else if (name=="levi") {
        for (int i=1;i<=30;++i) {
            add_node(i);
        }
        read_special(levi_graph);
    } else if (name=="ljubljana") {
        make_lcf_graph(ljubljana_graph_lcf,2);
    } else if (name=="mcgee") {
        for (int i=1;i<=24;++i) {
            add_node(i);
        }
        read_special(mcgee_graph);
    } else if (name=="mobius-kantor") {
        make_petersen_graph(8,3);
    } else if (name=="nauru") {
        make_petersen_graph(12,5);
    } else if (name=="octahedron") {
        add_node(1); add_node(3); add_node(6);
        read_special(octahedron_graph);
    } else if (name=="pappus") {
        for (int i=1;i<=18;++i) {
            add_node(i);
        }
        read_special(pappus_graph);
    } else if (name=="petersen") {
        make_petersen_graph(5,2);
    } else if (name=="robertson") {
        for (int i=1;i<=19;++i) {
            add_node(i);
        }
        read_special(robertson_graph);
    } else if (name=="soccerball") {
        for (int i=16;i<=20;++i) {
            add_node(i);
        }
        read_special(soccer_ball_graph);
    } else if (name=="shrikhande") {
        make_shrikhande_graph();
    } else if (name=="tetrahedron") {
        for (int i=2;i<=4;++i) {
            add_node(i);
        }
        read_special(tetrahedron_graph);
    }
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
    int n,j;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        n=it->neighbors().size();
        attrib2genmap(it->attributes(),attr);
        vecteur v=makevecteur(it->label(),attr,vecteur(n));
        j=0;
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
void graphe::get_marked_nodes(vecteur &V) const {
    V.clear();
    for (ivector_iter it=marked_nodes.begin();it!=marked_nodes.end();++it) {
        V.push_back(node_label(*it));
    }
}

/* fill m with marked nodes belonging to the s-th subgraph */
void graphe::get_marked_nodes_in_subgraph(int s,ivector &m) const {
    m.clear();
    m.reserve(marked_nodes.size());
    for (ivector_iter it=marked_nodes.begin();it!=marked_nodes.end();++it) {
        if (node(*it).subgraph()==s)
            m.push_back(*it);
    }
}

/* mark vertex with index v */
void graphe::mark_node(int v) {
    if (find(marked_nodes.begin(),marked_nodes.end(),v)==marked_nodes.end())
        marked_nodes.push_back(v);
}

/* remove vertex v from the list of marked vertices */
bool graphe::unmark_node(int v) {
    ivector::iterator it;
    if ((it=find(marked_nodes.begin(),marked_nodes.end(),v))!=marked_nodes.end()) {
        marked_nodes.erase(it);
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

/* convert vecteur E to list of ipairs representing edges, return false iff error occurs,
   if edge is not found set notfound=true */
bool graphe::edges2ipairs(const vecteur &E,ipairs &ev,bool &notfound) const {
    if (E.empty())
        return false;
    int i,j;
    if (ckmatrix(E)) {
        if (E.front()._VECTptr->size()!=2)
            return false;
        for (const_iterateur it=E.begin();it!=E.end();++it) {
            i=node_index(it->_VECTptr->front());
            j=node_index(it->_VECTptr->back());
            if (i<0 || j<0 || !has_edge(i,j)) {
                notfound=true;
                return false;
            }
            ev.push_back(make_pair(i,j));
        }
    } else {
        if (E.size()!=2)
            return false;
        i=node_index(E.front());
        j=node_index(E.back());
        if (i<0 || j<0 || !has_edge(i,j)) {
            notfound=true;
            return false;
        }
        ev.push_back(make_pair(i,j));
    }
    return true;
}

/* convert ipairs to ipairs */
void graphe::ipairs2edgeset(const ipairs &E,edgeset &Eset) {
    Eset.clear();
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        Eset.insert(*it);
    }
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

/* create the adjacency matrix of this graph */
void graphe::adjacency_matrix(matrice &m) const {
    int n=node_count(),i,j;
    m=*_matrix(makesequence(n,n),context0)._VECTptr;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
            if (j<0) j=-j-1;
            m[i]._VECTptr->at(j)=gen(1);
        }
    }
}

/* create the adjacency matrix as a sparse matrix */
void graphe::adjacency_sparse_matrix(sparsemat &sm) const {
    sm.clear();
    int i,j;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
            if (j<0) j=-j-1;
            sm[i][j]=1;
        }
    }
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
    int n=node_count(),i,j;
    matrice m=*_matrix(makesequence(n,n,0),context0)._VECTptr;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
            m[i]._VECTptr->at(j)=weight(i,j);
        }
    }
    return m;
}

/* return list of vertices (in the given subgraph) */
vecteur graphe::vertices(int sg) const {
    vecteur V;
    V.reserve(node_count());
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (sg<0 || it->subgraph()==sg)
            V.push_back(it->label());
    }
    return V;
}

/* make all vertices (in the given subgraph) unvisited */
void graphe::unvisit_all_nodes(int sg) {
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        if (sg<0 || it->subgraph()==sg)
            it->set_visited(false);
    }
}

/* unset ancestors for all vertices (in the given subgraph) */
void graphe::unset_all_ancestors(int sg) {
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        if (sg<0 || it->subgraph()==sg)
            it->unset_ancestor();
    }
}

/* fill the list E with edges (in the given subgraph) represented as pairs of vertex indices */
void graphe::get_edges_as_pairs(ipairs &E,bool include_temp_edges,int sg) const {
    int i,j;
    bool dir=is_directed();
    E.clear();
    E.reserve(edge_count());
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (sg>=0 && it->subgraph()!=sg)
            continue;
        i=it-nodes.begin();
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            if ((j=*jt)<0) {
                if (!include_temp_edges)
                    continue;
                j=-j-1;
            }
            if ((!dir && j<i) || (sg>=0 && node(j).subgraph()!=sg))
                continue;
            E.push_back(make_pair(i,j));
        }
    }
}

/* return list of edges/arcs (in the given subgraph) */
vecteur graphe::edges(bool include_weights,int sg) const {
    ipairs E;
    get_edges_as_pairs(E,sg);
    vecteur edge(2),ret(E.size());
    int i=0;
    bool isdir=is_directed();
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        edge[0]=nodes[it->first].label();
        edge[1]=nodes[it->second].label();
        if (!isdir)
            edge=*_sort(edge,ctx)._VECTptr;
        ret[i++]=include_weights?makevecteur(edge,weight(*it)):edge;
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
    dotfile.open(filename.c_str());
    if (!dotfile.is_open())
        return false;
    string ver=genstring2str(_version(change_subtype(vecteur(0),_SEQ__VECT),ctx));
    dotfile << "# this file was generated by " << ver << endl;
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
            dotfile << indent << it->label() << " ";
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
            dotfile << indent << it->label() << edgeop << "{ ";
            for (ivector_iter kt=u.begin();kt!=u.end();++kt) {
                dotfile << nodes[*kt].label() << " ";
            }
            dotfile << "};" << endl;
        }
        for (ivector_iter kt=v.begin();kt!=v.end();++kt) {
            dotfile << indent << it->label() << edgeop << nodes[*kt].label() << " ";
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
                    token.erase(token.end()-1);
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
        if (c=='#' && (pc==0 || pc=='\n' || pc=='\r')) {
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
    attrib::iterator it=attr.find(key);
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
    dotfile.open(filename.c_str());
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
            if (subgraphs.empty()) { error_raised=true; break; }
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
    assert(is_squarematrix(m) && int(m.size())==node_count());
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

/* randomize edge weights, generating them in segment [a,b] */
void graphe::randomize_edge_weights(double a,double b,bool integral_weights) {
    assert(a<=b && is_weighted());
    int m,n;
    if (integral_weights) {
        m=std::floor(a);
        n=std::floor(b);
    }
    int nc=node_count();
    gen w;
    for (int i=0;i<nc;++i) {
        for (int j=is_directed()?0:i+1;j<nc;++j) {
            if (!has_edge(i,j))
                continue;
            if (integral_weights)
                w=gen(rand_integer(n-m+1)+m);
            else
                w=gen(a+rand_uniform()*(b-a));
            set_edge_attribute(i,j,_GT_ATTRIB_WEIGHT,w);
        }
    }
}

/* store the underlying graph to G (convert arcs to edges and strip all attributes) */
void graphe::underlying(graphe &G) const {
    G.clear();
    G.add_nodes(vertices());
    G.set_directed(false);
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            G.add_edge(it-nodes.begin(),*jt);
        }
    }
}

/* store the complement of this graph in G */
void graphe::complement(graphe &G) const {
    int n=node_count();
    G.add_nodes(vertices());
    bool isdir=is_directed();
    G.set_directed(isdir);
    for (int i=0;i<n;++i) {
        for (int j=G.is_directed()?0:i+1;j<n;++j) {
            if (!has_edge(i,j))
                G.add_edge(i,j);
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
bool graphe::read_gen(const gen &g) {
    if (g.type!=_VECT || g.subtype!=_GRAPH__VECT)
        return false;
    this->clear();
    vecteur &gv=*g._VECTptr;
    if (gv.empty() || gv.front().type!=_MAP || !genmap2attrib(*gv.front()._MAPptr,attributes))
        return false;
    if (int(gv.size())<2 || gv[1].type!=_VECT)
        return false;
    for (const_iterateur it=gv[1]._VECTptr->begin();it!=gv[1]._VECTptr->end();++it) {
        if (it->type!=_STRNG)
            return false;
        register_user_tag(genstring2str(*it));
    }
    for (const_iterateur it=gv.begin()+2;it!=gv.end();++it) {
        if (it->type!=_VECT || int(it->_VECTptr->size())<3)
            return false;
        vertex &vt=nodes[add_node(it->_VECTptr->front())];
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

/* read special graph from a list of adjacency lists of integers */
void graphe::read_special(const int *special_graph) {
    int state=1;
    gen v,w;
    for(const int *p=special_graph;*p!=-2;++p) {
        if (*p==-1) {
            state=1;
        } else if (state==1) {
            v=gen(*p);
            state=2;
        } else if (state==2) {
            w=gen(*p);
            add_edge(v,w);
        }
    }
}

/* read special graph from a list of adjacency lists of strings */
void graphe::read_special(const char **special_graph) {
    const char **p=special_graph;
    int state=1;
    gen v,w;
    string s;
    do {
        s=string(*p);
        if (s.empty()) {
            state=1;
        } else if (state==1) {
            state=2;
            v=str2gen(s,true);
        } else if (state==2) {
            w=str2gen(s,true);
            add_edge(v,w);
        }
        ++p;
    } while (*p!=NULL);
}

/* make a copy of this graph and store it in G */
void graphe::copy(graphe &G) const {
    G.set_name(graph_name);
    G.register_user_tags(user_tags);
    G.set_graph_attributes(attributes);
    G.copy_nodes(nodes);
    G.copy_marked_nodes(get_marked_nodes());
}

/* returns true iff graph has edge {i,j} */
bool graphe::has_edge(int i,int j) const {
    if (i<0 || i>=node_count() || j<0 || j>=node_count())
        return false;
    return nodes[i].has_neighbor(j);
}

/* returns true iff i-th and j-th vertices are adjacent */
bool graphe::nodes_are_adjacent(int i,int j) const {
    return nodes[i].has_neighbor(j) || nodes[j].has_neighbor(i);
}

/* return const reference to the attributes assigned to edge [i,j] */
const graphe::attrib &graphe::edge_attributes(int i, int j) const {
    if (is_directed())
        return nodes[i].neighbor_attributes(j);
    return nodes[i<j?i:j].neighbor_attributes(i<j?j:i);
}

/* return the modifiable reference to the attributes assigned to edge [i,j] */
graphe::attrib &graphe::edge_attributes(int i, int j) {
    if (is_directed())
        return nodes[i].neighbor_attributes(j);
    return nodes[i<j?i:j].neighbor_attributes(i<j?j:i);
}

/* convert attrib to pair of vecteurs */
void graphe::attrib2vecteurs(const attrib &attr,vecteur &tags,vecteur &values) const {
    for (attrib_iter it=attr.begin();it!=attr.end();++it) {
        tags.push_back(str2gen(index2tag(it->first),true));
        values.push_back(it->second);
    }
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
void graphe::add_edge(int i,int j,const attrib &attr) {
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
        if (it->label()==v)
            return it-nodes.begin();
    }
    vertex vert;
    vert.set_label(v);
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
    ivector adj;
    adjacent_nodes(i,adj);
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

/* remove all nodes with indices from V */
void graphe::remove_nodes(const ivector &V) {
    ivector isolated_nodes,adj;
    int i;
    for (ivector_iter it=V.begin();it!=V.end();++it) {
        i=*it;
        if (i<0)
            continue;
        adjacent_nodes(i,adj);
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

/* remove all nodes with labels in V */
void graphe::remove_nodes(const vecteur &V) {
    ivector I(V.size());
    int i=0;
    for (const_iterateur it=V.begin();it!=V.end();++it) {
        I[i++]=node_index(*it);
    }
    remove_nodes(I);
}

/* return vector of node labels */
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
        if (it->label()==v)
            return it-nodes.begin();
    }
    return -1;
}

/* set 'subgraph' field of all nodes in v to s */
void graphe::set_subgraph(const ivector &v,int s) {
    for (ivector_iter it=v.begin();it!=v.end();++it) {
        node(*it).set_subgraph(s);
    }
}

/* merge subgraphs s and t, such that the resulting subgraph has index s */
void graphe::merge_subgraphs(int s,int t) {
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        if (it->subgraph()==t)
            it->set_subgraph(s);
    }
}

/* return maximal subgraph index in this graph */
int graphe::max_subgraph_index() const {
    int mi=-2;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->subgraph()>mi)
            mi=it->subgraph();
    }
    return mi;
}

/* make cycle graph with n vertices */
void graphe::make_cycle_graph() {
    int n=node_count();
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

/* return the value corresponding to the graph attribute specified by key */
bool graphe::get_graph_attribute(int key,gen &val) const {
    attrib_iter it=attributes.find(key);
    if (it==attributes.end()) {
        val=undef;
        return false;
    }
    val=it->second;
    return true;
}

/* return the value corresponding to the i-th node attribute specified by key */
bool graphe::get_node_attribute(int i,int key,gen &val) const {
    assert (i>=0 && i<node_count());
    const attrib &attr=node(i).attributes();
    attrib_iter it=attr.find(key);
    if (it==attr.end()) {
        val=undef;
        return false;
    }
    val=it->second;
    return true;
}

/* return the value corresponding to the attribute assigned to edge [i,j] and specified by key */
bool graphe::get_edge_attribute(int i,int j,int key,gen &val) const {
    const attrib &attr=edge_attributes(i,j);
    attrib_iter it=attr.find(key);
    if (it==attr.end()) {
        val=undef;
        return false;
    }
    val=it->second;
    return true;
}

/* discard the graph attribute specified by key */
void graphe::discard_graph_attribute(int key) {
    attrib::iterator it=attributes.find(key);
    if (it!=attributes.end())
        attributes.erase(it);
}

/* discard the attribute assigned to i-th node and specified by key */
void graphe::discard_node_attribute(int i,int key) {
    attrib &attr=node(i).attributes();
    attrib::iterator it=attr.find(key);
    if (it!=attr.end())
        attr.erase(it);
}

/* discard the attribute assigned to edge [i,j] and specified by key */
void graphe::discard_edge_attribute(int i,int j,int key) {
    attrib &attr=edge_attributes(i,j);
    attrib::iterator it=attr.find(key);
    if (it!=attr.end())
        attr.erase(it);
}

/* return true iff the graph is directed */
bool graphe::is_directed() const {
    gen g;
    assert(get_graph_attribute(_GT_ATTRIB_DIRECTED,g) && g.is_integer());
    return (bool)g.val;
}

/* return true iff the graph is weighted */
bool graphe::is_weighted() const {
    gen g;
    assert(get_graph_attribute(_GT_ATTRIB_WEIGHTED,g) && g.is_integer());
    return (bool)g.val;
}

/* create the subgraph defined by vertices from 'vi' and store it in G */
void graphe::induce_subgraph(const ivector &vi,graphe &G,bool copy_attrib) const {
    G.clear();
    for (ivector_iter it=vi.begin();it!=vi.end();++it) {
        gen v_label=node_label(*it);
        const attrib &attri=nodes[*it].attributes();
        if (copy_attrib)
            G.add_node(v_label,attri);
        else
            G.add_node(v_label);
    }
    ivector_iter kt;
    for (ivector_iter it=vi.begin();it!=vi.end();++it) {
        const vertex &v=node(*it);
        for (ivector_iter jt=v.neighbors().begin();jt!=v.neighbors().end();++jt) {
            if ((kt=find(vi.begin(),vi.end(),*jt))==vi.end())
                continue;
            G.add_edge(it-vi.begin(),kt-vi.begin());
        }
    }
}

/* create the subgraph G defined by a list of edges E */
void graphe::subgraph(const ipairs &E,graphe &G,bool copy_attrib) const {
    G.clear();
    G.set_directed(is_directed());
    int i,j;
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        const vertex &v=node(it->first),&w=node(it->second);
        if (copy_attrib) {
            i=G.add_node(v.label(),v.attributes());
            j=G.add_node(w.label(),w.attributes());
            G.add_edge(i,j,edge_attributes(it->first,it->second));
        } else
            G.add_edge(v.label(),w.label());
    }
}

/* return true iff this graph is subgraph of G */
bool graphe::is_subgraph(const graphe &G) const {
    if (is_directed() != G.is_directed() ||
            node_count()>G.node_count() ||
            edge_count()>G.edge_count())
        return false;
    int i,j;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=G.node_index(it->label());
        if (i<0)
            return false;
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=G.node_index(node(*jt).label());
            if (j<0 || !G.has_edge(i,j))
                return false;
        }
    }
    return true;
}

/* fill the list adj with vertices adjacent to the i-th one */
void graphe::adjacent_nodes(int i,ivector &adj,bool include_temp_edges) const {
    assert(i>=0 && i<node_count());
    const vertex &v=node(i);
    adj.clear();
    adj.reserve(node_count());
    int j;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        j=*it;
        if (include_temp_edges || j>=0)
            adj.push_back(j<0?-j-1:j);
    }
    if (is_directed()) {
        for (node_iter it=nodes.begin();it!=nodes.end();++it) {
            j=it-nodes.begin();
            if (i==j)
                continue;
            const ivector &ngh=it->neighbors();
            if (find(ngh.begin(),ngh.end(),i)!=ngh.end() ||
                    (include_temp_edges && find(ngh.begin(),ngh.end(),-i-1)!=ngh.end())) {
                if (find(adj.begin(),adj.end(),j)==adj.end())
                    adj.push_back(j);
            }
        }
    }
}

/* return a maximal independent set of vertices in undirected graph */
void graphe::maximal_independent_set(ivector &ind) const {
    int n=node_count(),i;
    ivector V(n),gain(n);
    for (i=0;i<n;++i) {
        V[i]=i;
        gain[i]=degree(i);
    }
    ivector::iterator pos;
    ind.clear();
    ind.reserve(n);
    while (!V.empty()) {
        i=V.front();
        pos=V.begin();
        for (ivector::iterator it=V.begin();it!=V.end();++it) {
            if (gain[i]<gain[*it]) {
                i=*it;
                pos=it;
            }
        }
        V.erase(pos);
        ind.push_back(i);
        const vertex &v=node(i);
        for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
            if ((pos=find(V.begin(),V.end(),*it))==V.end())
                continue;
            V.erase(pos);
            const vertex &w=node(*it);
            for (ivector_iter jt=w.neighbors().begin();jt!=w.neighbors().end();++jt) {
                if (find(V.begin(),V.end(),*jt)==V.end())
                    continue;
                gain[*jt]++;
            }
        }
    }
    sort(ind.begin(),ind.end());
}

/*
 * matching_maximizer class implementation ****************************
 */

graphe::matching_maximizer::matching_maximizer(graphe *gr) {
    G=gr;
}

/* return the vertex which matches v in matching, return -1 if v is not matched */
int graphe::matching_maximizer::mate(const ipairs &matching, int v) {
    for (ipairs_iter it=matching.begin();it!=matching.end();++it) {
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
    std::reverse(path.begin(),path.end());
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
    ivector adj;
    G->adjacent_nodes(v,adj);
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
        G->adjacent_nodes(v,res);
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
                std::reverse(bit->second.begin(),bit->second.end());
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
                        std::reverse(short_path.begin(),short_path.end());
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
 * end of matching_maximizer class implementation ************************************
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
    if (D.empty())
        return;
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
            ipairs::iterator it=C.begin();
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

void graphe::triangulator::fold(const ivector &B,bool triangulate) {
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
            if (triangulate) {
                ivector P1,P2;
                path(q,r,B,P1);
                path(u,t,B,P2);
                P1.insert(P1.end(),P2.begin(),P2.end());
                zigzag(P1);
            }
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
            if (triangulate) {
                ivector P;
                path(q,r,B,P);
                zigzag(P);
                path(r,q,B,P);
                zigzag(P);
            }
        } else {
            ivector P(0);
            ivector Q;
            for (s=0;s<k;++s) {
                i=s<k-1?C[s+1].first:C.front().first;
                j=C[s].second;
                q=predecessor(j,n);
                r=successor(i,n);
                addedge(B[q],B[r]);
                if (triangulate) {
                    path(q,r,B,Q);
                    zigzag(Q);
                }
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
            fold(F,true);
    }
}

/*
 * END TRIANGULATOR IMPLEMENTATION
 */

/* extend given matching to a maximum matching using Edmonds' blossom algorithm */
void graphe::maximize_matching(ipairs &matching) {
    matching_maximizer ed(this);
    ed.find_maximum_matching(matching);
}

/* find a maximal matching in an undirected graph */
void graphe::find_maximal_matching(ipairs &matching) const {
    int i,j,n=node_count();
    ivector match(n,-1);
    vector<bool> node_marked(n,false);
    while (true) {
        for (i=0;i<n;++i) {
            if (!node_marked[i] && match[i]<0)
                break;
        }
        if (i==n)
            break;
        node_marked[i]=true;
        const vertex &v=node(i);
        for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
            if (match[*it]<0) {
                match[*it]=i;
                match[i]=*it;
                break;
            }
        }
    }
    ivector skip;
    matching.clear();
    for (i=0;i<int(match.size());++i) {
        j=match[i];
        if (j<0 || find(skip.begin(),skip.end(),i)!=skip.end())
            continue;
        skip.push_back(j);
        matching.push_back(make_pair(i,j));
    }
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

/* return the distance between points p and q */
double graphe::point_distance(const point &p,const point &q,point &pq) {
    copy_point(q,pq);
    subtract_point(pq,p);
    return point_displacement(pq);
}

/* write d1*p+d2*q to res */
void graphe::point_lincomb(const point &p,const point &q,double d1,double d2,point &res) {
    copy_point(p,res);
    if (d2==0)
        scale_point(res,d1);
    else  {
        scale_point(res,d1/d2);
        add_point(res,q);
        scale_point(res,d2);
    }
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
    scale_point(p,radius/std::sqrt(R));
}

/* convert point from cartesian to polar representation (2d only) */
void graphe::point2polar(point &p,double &r,double &phi) {
    int d=p.size();
    assert(d==2);
    double x=p[0],y=p[1];
    r=std::sqrt(std::pow(x,2.0)+std::pow(y,2.0));
    phi=atan2(y,x);
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
    double r,phi0;
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        point &p=*it;
        point2polar(p,r,phi0);
        p[0]=r*std::cos(phi0+phi);
        p[1]=r*std::sin(phi0+phi);
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

/* determine the diameter of the layout (distance between the pair of farthest vertices) */
double graphe::layout_diam(const layout &x) {
    point p(2);
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
    return std::sqrt(diam);
}

/* return the bounding rectangle of layout x */
graphe::rectangle graphe::layout_bounding_rect(layout &ly,double padding) {
    double xmin=DBL_MAX,xmax=-DBL_MAX,ymin=DBL_MAX,ymax=-DBL_MAX,x,y;
    for (layout_iter it=ly.begin();it!=ly.end();++it) {
        x=it->front();
        y=it->at(1);
        if (x<xmin)
            xmin=x;
        if (x>xmax)
            xmax=x;
        if (y<ymin)
            ymin=y;
        if (y>ymax)
            ymax=y;
    }
    return rectangle(xmin-padding,ymin-padding,xmax-xmin+2*padding,ymax-ymin+2*padding,&ly);
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
void graphe::create_random_layout(layout &x,int dim) {
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        it->resize(dim);
        it->at(0)=rand_uniform();
        it->at(1)=rand_uniform();
        if (dim==3)
            it->at(2)=rand_uniform();
    }
}

/* add the force applying to p because of being repulsed by q */
void graphe::accumulate_repulsive_force(const point &p,const point &q,double R,double K,double eps,point &force,bool sq_dist) {
    assert(p.size()==q.size() && p.size()==force.size());
    point f(p.size());
    double norm=point_distance(q,p,f),C=0.01;
    if (norm>R)
        return;
    if (norm==0)
        rand_point(f,norm=eps);
    scale_point(f,C*K*K/(norm*norm*(sq_dist?norm:1.0)));
    add_point(force,f);
}

/* lay out the graph using a force-directed algorithm with spring-electrical model */
void graphe::force_directed_placement(layout &x,double K,double R,double tol,bool ac) {
    double step_length=K,shrinking_factor=0.9,eps=K*tol;
    double energy=DBL_MAX,energy0,norm,max_displacement;
    int progress=0,n=x.size(),i,j;
    if (n==0)
        return;
    assert (n==node_count() && n>0);
    int d=x.front().size();
    point force(d),p(d);
    // keep updating the positions until the system freezes
    do {
        energy0=energy;
        energy=0;
        max_displacement=0;
        for (node_iter nt=nodes.begin();nt!=nodes.end();++nt) {
            i=nt-nodes.begin();
            point &xi=x[i];
            clear_point_coords(force);
            // compute attractive forces between vertices adjacent to the i-th vertex
            for (ivector_iter it=nt->neighbors().begin();it!=nt->neighbors().end();++it) {
                j=*it;
                if (j<0) j=-j-1;
                scale_point(p,point_distance(xi,x[j],p)/K);
                add_point(force,p);
            }
            // compute repulsive forces for all vertices j!=i which are not too far from the i-th vertex
            for (layout_iter it=x.begin();it!=x.end();++it) {
                j=it-x.begin();
                if (i!=j)
                    accumulate_repulsive_force(xi,*it,R,K,shrinking_factor*eps,force);
            }
            // move the location of the i-th vertex in the direction of the force f
            norm=point_displacement(force);
            if (norm==0)
                continue;
            if (step_length<norm) {
                scale_point(force,step_length/norm);
                norm=step_length;
            }
            add_point(xi,force);
            // update the maximal displacement for this iteration
            if (norm>max_displacement)
                max_displacement=norm;
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
    } while (max_displacement>eps);
}

/* compute optimal positions of edge labels and store them as "position" attributes of the respective edges */
void graphe::edge_labels_placement(const layout &x,double tol) {
    if (x.empty())
        return;
    int dim=x.front().size();
    ipairs E;
    get_edges_as_pairs(E,false);
    int n=E.size(),k;
    layout y(n),dir(n);
    vector<double> D(n);
    point force(dim),v(dim);
    double max_displacement,norm,maxnorm,K;
    double step_length=1.0,shrinking_factor=0.9;
    // generate an initial placement of edge labels and store edge directions
    for (int i=0;i<n;++i) {
        ipair &edge=E[i];
        const point &p1=x[edge.first],&p2=x[edge.second];
        point &p=y[i],&u=dir[i];
        double &d=D[i];
        p.resize(dim);
        u.resize(dim);
        if ((d=point_distance(p1,p2,u))==0)
            continue;
        scale_point(u,1.0/d);
        copy_point(u,p);
        scale_point(p,d/3);
        add_point(p,p1);
    }
    if (n<300) {
        // keep updating the positions until the system freezes
        do {
            // recompute positions of all edge labels by applying the repulsive forces
            max_displacement=0;
            for (ipairs_iter it=E.begin();it!=E.end();++it) {
                k=it-E.begin();
                const point &p1=x[it->first],&p2=x[it->second],&u=dir[k];
                point &p=y[k];
                K=D[k];
                clear_point_coords(force);
                // compute the repulsive forces applying to p
                for (int i=0;i<n;++i) {
                    if (i!=k)
                        accumulate_repulsive_force(p,y[i],DBL_MAX,K,shrinking_factor*tol,force,true);
                }
                accumulate_repulsive_force(p,p1,DBL_MAX,K,shrinking_factor*tol,force);
                accumulate_repulsive_force(p,p2,DBL_MAX,K,shrinking_factor*tol,force);
                // compute the attractive forces applying to p
                scale_point(v,point_distance(p,p1,v)/K);
                add_point(force,v);
                scale_point(v,point_distance(p,p2,v)/K);
                add_point(force,v);
                // compute projection of the resultant force onto the vector u
                norm=point_displacement(force);
                if (norm==0)
                    continue;
                norm=point_dotprod(force,u);
                copy_point(u,force);
                scale_point(force,norm);
                norm=std::abs(norm);
                // update the position of p, assuring that it stays between p1 and p2
                if (step_length*K<norm) {
                    scale_point(force,step_length*K/norm);
                    norm=step_length*K;
                }
                for (int i=0;i<2;++i) {
                    maxnorm=point_distance(p,i==0?p1:p2,v)*shrinking_factor;
                    if (point_dotprod(force,v)>0) {
                        if (norm>maxnorm) {
                            scale_point(force,maxnorm/norm);
                            norm=maxnorm;
                        }
                    }
                }
                add_point(p,force);
                // update the maximal displacement for this iteration
                norm/=K;
                if (norm>max_displacement)
                    max_displacement=norm;
            }
            step_length*=shrinking_factor; // simple cooling scheme
        } while (max_displacement>tol);
    }
    // store edge label positions as edge attributes
    double d1,d2;
    for (layout_iter it=y.begin();it!=y.end();++it) {
        const ipair &edge=E[it-y.begin()];
        const point &p=x[edge.first],&q=x[edge.second];
        d1=point_distance(p,*it,v);
        d2=point_distance(q,*it,v);
        set_edge_attribute(edge.first,edge.second,_GT_ATTRIB_POSITION,d2/(d1+d2));
    }
}

/* retrieve element in A at position (i,j) and store it in val, return true iff there is such element */
bool graphe::sparse_matrix_element(const sparsemat &A,int i,int j,double &val) {
    sparsemat::const_iterator it;
    map<int,double>::const_iterator jt;
    if ((it=A.find(i))==A.end() || (jt=it->second.find(j))==it->second.end())
        return false;
    val=jt->second;
    return true;
}

/* compute the product A*B and store it in P (ncols is equal to the number of columns of A),
 * if A*B=B*A and A and B are symmetric then enabling 'symmetric=true' speeds up the computation */
void graphe::multiply_sparse_matrices(const sparsemat &A,const sparsemat &B,sparsemat &P,int ncols,bool symmetric) {
    int i,isempty;
    double val,p;
    for (sparsemat::const_iterator it=A.begin();it!=A.end();++it) {
        i=it->first;
        map<int,double> &row=P[i];
        isempty=true;
        for (int j=symmetric?i:0;j<ncols;++j) {
            p=0;
            for (map<int,double>::const_iterator jt=it->second.begin();jt!=it->second.end();++jt) {
                if (sparse_matrix_element(B,jt->first,j,val))
                    p+=jt->second*val;
            }
            if (p!=0) {
                isempty=false;
                row[j]=p;
                if (symmetric)
                    P[j][i]=p;
            }
        }
        if (isempty)
            P.erase(P.find(i));
    }
}

/* store the transposition of A in T */
void graphe::transpose_sparsemat(const sparsemat &A, sparsemat &T) {
    for (sparsemat::const_iterator it=A.begin();it!=A.end();++it) {
        for(map<int,double>::const_iterator jt=it->second.begin();jt!=it->second.end();++jt) {
            T[jt->first][it->first]=jt->second;
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

/* coarsening of the graph with respect to the prolongation matrix P */
void graphe::coarsening(graphe &G,const sparsemat &P,const ivector &V) const {
    sparsemat Q,I,R,IG;
    int n=node_count();
    transpose_sparsemat(P,Q);
    // create sparse symmetric incidence matrix I of this graph
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        const ivector &ngh=it->neighbors();
        if (ngh.empty())
            continue;
        map<int,double> &row=I[it-nodes.begin()];
        for (ivector_iter jt=ngh.begin();jt!=ngh.end();++jt) {
            row[*jt]=1;
        }
    }
    // use Galerkin product Q*I*P as the incidence matrix IG for graph G
    multiply_sparse_matrices(Q,I,R,n);
    multiply_sparse_matrices(R,P,IG,n);
    for (ivector_iter it=V.begin();it!=V.end();++it) {
        G.add_node(node_label(*it));
    }
    int i=0,j;
    for (sparsemat::const_iterator it=IG.begin();it!=IG.end();++it) {
        i=it->first;
        for (map<int,double>::const_iterator jt=it->second.begin();jt!=it->second.end();++jt) {
            j=jt->first;
            if (i<j && jt->second!=0)
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
    ivector removed_nodes,V;
    for (ipairs_iter it=M.begin();it!=M.end();++it) {
        removed_nodes.push_back(it->second);
    }
    int n=node_count(),m=removed_nodes.size(),J;
    ivector::iterator ivt;
    for (int i=0;i<n;++i) {
        if ((ivt=find(removed_nodes.begin(),removed_nodes.end(),i))==removed_nodes.end())
            V.push_back(i);
        else
            removed_nodes.erase(ivt);
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

void graphe::multilevel_recursion(layout &x,int d,double R,double K,double tol,int depth) {
    ivector mis;
    ipairs M;
    if (multilevel_mis)
        maximal_independent_set(mis);
    else
        find_maximal_matching(M);
    int n=node_count(),m=multilevel_mis?mis.size():n-int(M.size());
    x.resize(n);
    if (m>0.75*n) {
        // coarsening is slow, switch from EC to MIS method since it's faster
        multilevel_mis=true;
        multilevel_recursion(x,d,R,K,tol,depth);
        return;
    }
    if (m<2) {
        // the coarsest level, apply force directed algorithm on a random initial layout
        multilevel_depth=depth;
        create_random_layout(x,d);
        force_directed_placement(x,K,R*(depth+1)*K,tol,false);
    } else {
        // create coarser graph H and lay it out
        graphe G(ctx);
        sparsemat P; // prolongation matrix
        if (multilevel_mis)
            coarsening_mis(mis,G,P);
        else
            coarsening_ec(M,G,P);
        layout y;
        G.multilevel_recursion(y,d,R,K,tol,depth+1);
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
        // the current depth level and subsequently refine x
        double L=K*std::pow(PLASTIC_NUMBER,depth-multilevel_depth);
        force_directed_placement(x,L,R*(depth+1)*L,tol,false);
    }
}

/* apply multilevel force directed algorithm to layout x */
void graphe::make_spring_layout(layout &x,int d,double tol) {
    int n=node_count();
    if (n==0)
        return;
    multilevel_mis=false;
    multilevel_recursion(x,d,DBL_MAX,10.0,tol);
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
void graphe::make_circular_layout(layout &x,const ivector &outer_face,bool planar,double tol) {
    int n=node_count(),iter_count=0,maxper=0,tries=0,j;
    vector<bool> is_outer(n,false);
    x.resize(n);
    for (int i=0;i<n;++i) {
        if (find(outer_face.begin(),outer_face.end(),i)!=outer_face.end())
            is_outer[i]=true;
    }
    ivector per;
    // compute vertex periphericity
    per.resize(n);
    periphericity(outer_face,per);
    for (ivector_iter pt=per.begin();pt!=per.end();++pt) {
        if (*pt>maxper)
            maxper=*pt;
    }
    // place facial vertices on the unit circle and all other vertices in the origin
    make_regular_polygon_layout(x,outer_face);
    if (node_count()==int(outer_face.size()))
        return; // there are no vertices to place inside the circle
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
    double A=2.0,d,cool,C=std::sqrt((double)n/M_PI),eps=tol;
    ipairs E;
    if (planar)
        get_edges_as_pairs(E,false);
    while (true) {
        ++iter_count;
        // compute the resultant force for each non-outer vertex
        for (int i=0;i<n;++i) {
            if (is_outer[i])
                continue;
            point &pt=P[i],&p=x[i];
            clear_point_coords(pt);
            vertex &v=node(i);
            for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
                j=*it;
                if (j<0) j=-j-1;
                point &q=x[j];
                point r(2);
                copy_point(q,r);
                subtract_point(r,p);
                d=C*std::exp(A*(2.0*maxper-per[i]-per[j])/(double)maxper)*point_displacement(r,false);
                scale_point(r,d);
                add_point(pt,r);
            }
        }
        // move each vertex in the direction of the force
        cool=1.0/(C+std::pow(iter_count,1.5)/C);
        for (int i=0;i<n;++i) {
            if (is_outer[i])
                continue;
            point &pt=P[i],&p=x[i];
            d=point_displacement(pt);
            if (d>0) {
                scale_point(pt,std::min(d,cool)/d);
                add_point(p,pt);
            }
        }
        if (cool<eps) {
            if (!planar || !has_crossing_edges(x,E) || ++tries>PLESTENJAK_MAX_TRIES)
                break;
            eps/=2.0;
        }
    }
    if (tries>PLESTENJAK_MAX_TRIES)
        message("Error: failed to obtain planar layout");
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
        sets_union(P,X,PUX);
        int i=PUX.front(),mn=0,n,v;
        ivector adj,S;
        for (it=PUX.begin();it!=PUX.end();++it) {
            adjacent_nodes(*it,adj);
            S.resize(std::max(adj.size(),P.size()));
            jt=set_intersection(adj.begin(),adj.end(),P.begin(),P.end(),S.begin());
            if ((n=int(jt-S.begin()))>mn) {
                mn=n;
                i=*it;
            }
        }
        // for each vertex in P\N(i) do recursion
        adjacent_nodes(i,adj);
        sets_difference(P,adj,S);
        ivector RUv,PP,XX;
        for (jt=S.begin();jt!=S.end();++jt) {
            v=*jt;
            RUv=R;
            RUv.push_back(v);
            sort(RUv.begin(),RUv.end());
            adjacent_nodes(v,adj);
            sets_intersection(P,adj,PP);
            sets_intersection(X,adj,XX);
            tomita_recurse(RUv,PP,XX,cliques);
            P.erase(find(P.begin(),P.end(),v));
            X.push_back(v);
            sort(X.begin(),X.end());
        }
    }
}

/* a modified version of the Bron-Kerbosch algorithm for listing all maximal cliques,
 * developed by Tomita et al. */
void graphe::tomita(ivectors &maximal_cliques) const {
    ivector R,X,P(node_count());
    for (int i=0;i<node_count();++i) P[i]=i;
    tomita_recurse(R,P,X,maximal_cliques);
}

/* find maximum clique in this graph and return its size */
int graphe::maximum_clique(ivector &clique) const {
    ivectors maximal_cliques;
    tomita(maximal_cliques);
    int k=-1,maxsize=0;
    for (ivectors_iter it=maximal_cliques.begin();it!=maximal_cliques.end();++it) {
        if (int(it->size())>maxsize) {
            maxsize=it->size();
            k=it-maximal_cliques.begin();
        }
    }
    clique=maximal_cliques[k];
    return maxsize;
}

/* return true iff the graph can be covered with exactly k maximal cliques
 * (also provide indices of those cliques) */
bool graphe::has_k_clique_cover(int k,const ivectors &maximal_cliques,ivector &cv) const {
    int n=maximal_cliques.size(),nv=node_count();
    int nchoosek=comb(n,k).val;
    vector<ulong> nk_sets(nchoosek);
    generate_nk_sets(n,k,nk_sets);
    ivector U,V;
    int cnt,ncov;
    cv.resize(k);
    for (vector<ulong>::const_iterator it=nk_sets.begin();it!=nk_sets.end();++it) {
        cnt=0;
        for (int i=0,m=1;i<n;++i,m*=2) {
            if ((*it & m)!=0)
                cv[cnt++]=i;
        }
        for (ivector_iter jt=cv.begin();jt!=cv.end();++jt) {
            const ivector &clique=maximal_cliques[*jt];
            if ((ncov=(jt-cv.begin())%2==0?sets_union(U,clique,V):sets_union(V,clique,U))==nv)
                return true;
            for (ivector_iter kt=jt+1;kt!=cv.end();++kt) {
                ncov+=maximal_cliques[*kt].size();
            }
            if (ncov<nv)
                break;
        }
    }
    return false;
}

/* returns true iff there is a clique cover of order not larger than k and finds that cover */
bool graphe::clique_cover(ivectors &cover,int k) {
    if (is_triangle_free()) {
        // clique cover consists of matched edges and singleton vertex sets
        ipairs matching;
        matching_maximizer maximizer(this);
        maximizer.find_maximum_matching(matching);
        int m=matching.size(),n=node_count(),i=0;
        if (k>0 && n-m>k)
            return false;
        vector<bool> matched(n);
        cover.resize(n-m);
        for (ipairs_iter it=matching.begin();it!=matching.end();++it) {
            ivector &clique=cover[i++];
            clique.resize(2);
            matched[(clique.front()=it->first)]=true;
            matched[(clique.back()=it->second)]=true;
        }
        for (vector<bool>::const_iterator it=matched.begin();it!=matched.end();++it) {
            if (!*it) {
                ivector &clique=cover[i++];
                clique.resize(1);
                clique.front()=it-matched.begin();
            }
        }
        return true;
    }
    // a naive algorithn for clique cover
    ivectors maximal_cliques;
    ivector indices;
    tomita(maximal_cliques);
    int mcsize=maximal_cliques.size();
    for (int i=1;i<=(k==0?mcsize:k);++i) {
        if (has_k_clique_cover(i,maximal_cliques,indices)) {
            cover.resize(i);
            for (ivector_iter it=indices.begin();it!=indices.end();++it) {
                cover[it-indices.begin()]=maximal_cliques[*it];
            }
            return true;
        }
    }
    return false;
}

/* return true iff the graph is complete (i.e., a clique) */
bool graphe::is_clique() const {
    assert(!is_directed());
    int i,j;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        for (node_iter jt=it+1;jt!=nodes.end();++jt) {
            j=jt-nodes.begin();
            if (!has_edge(i,j))
                return false;
        }
    }
    return true;
}

/* find maximum independent set in this graph and return its size */
int graphe::maximum_independent_set(ivector &v) const {
    graphe C(ctx);
    complement(C);
    return C.maximum_clique(v);
}

/* return true iff the graph is triangle-free */
bool graphe::is_triangle_free() const {
    assert(!is_directed());
    sparsemat M,M2;
    adjacency_sparse_matrix(M);
    multiply_sparse_matrices(M,M,M2,node_count(),true);
    double val,trace=0;
    for (sparsemat::const_iterator it=M.begin();it!=M.end();++it) {
        for (map<int,double>::const_iterator jt=it->second.begin();jt!=it->second.end();++jt) {
            if (sparse_matrix_element(M2,jt->first,it->first,val)) {
                trace+=jt->second*val;
                if (trace>0)
                    return false;
            }
        }
    }
    return true;
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
    ivector adj;
    adjacent_nodes(j,adj);
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

/* put all edges incident to nodes in V to set E */
void graphe::incident_edges(const ivector &V,edgeset &E) {
    if (is_directed()) {
        ivector adj;
        for (ivector_iter it=V.begin();it!=V.end();++it) {
            adjacent_nodes(*it,adj);
            for (ivector_iter jt=adj.begin();jt!=adj.end();++jt) {
                if (has_edge(*it,*jt))
                    E.insert(make_pair(*it,*jt));
                if (has_edge(*jt,*it))
                    E.insert(make_pair(*jt,*it));
            }
        }
    } else {
        for (ivector_iter it=V.begin();it!=V.end();++it) {
            vertex &v=node(*it);
            for (ivector_iter jt=v.neighbors().begin();jt!=v.neighbors().end();++jt) {
                E.insert(make_pair(*it,*jt));
            }
        }
    }
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

/* create a graph from decreasing degree sequence L by using Havel-Hakimi algorithm,
 * return false iff L is not a graphic sequence */
bool graphe::hakimi(const ivector &L) {
    this->clear();
    int n=L.size(),d,i,z;
    if (n==0)
        return true;
    vecteur V;
    make_default_labels(V,n);
    add_nodes(V);
    ipairs D(n);
    for (int i=0;i<n;++i) {
        D[i]=make_pair(i,L[i]);
    }
    ipairs_comparator comp;
    do {
        sort(D.begin(),D.end(),comp);
        i=D.back().first;
        d=D.back().second;
        D.pop_back();
        for (int k=0;k<d;++k) {
            ipair &p=*(D.rbegin()+k);
            if (--p.second<0)
                return false;
            add_edge(i,p.first);
        }
        z=0;
        for (ipairs_iter it=D.begin();it!=D.end();++it) {
            if (it->second==0)
                ++z;
        }
    } while (z<int(D.size()));
    return true;
}

/* return the (odd) girth of this graph (the length of the shortest (odd) cycle),
 * return -1 if there are no cycles */
int graphe::girth(bool odd,int sg) {
    clear_node_stack();
    int g=-1,h,i,j;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        unvisit_all_nodes(sg);
        i=it-nodes.begin();
        vertex &v=node(i);
        if (sg>=0 && v.subgraph()!=sg)
            continue;
        node_stack.push(i);
        v.unset_ancestor();
        v.set_disc(0);
        while (!node_stack.empty()) {
            j=node_stack.top();
            node_stack.pop();
            vertex &w=node(j);
            w.set_visited(true);
            for (ivector_iter jt=w.neighbors().begin();jt!=w.neighbors().end();++jt) {
                if (*jt==w.ancestor())
                    continue;
                vertex &u=node(*jt);
                if (sg>=0 && u.subgraph()!=sg)
                    continue;
                if (!u.is_visited()) {
                    u.set_ancestor(j);
                    u.set_disc(w.disc()+1);
                    node_stack.push(*jt);
                } else {
                    h=w.disc()+u.disc()+1;
                    if (!odd || h%2!=0)
                        g=g<0?h:std::min(g,h);
                }
            }
        }
    }
    return g;
}

/* create a graph from LCF notation [jumps]^e */
void graphe::make_lcf_graph(const ivector &jumps,int e) {
    int m=jumps.size(),n=m*e;
    vecteur V;
    make_default_labels(V,n);
    add_nodes(V);
    make_cycle_graph();
    int j=0,k;
    for (int i=0;i<n;++i) {
        k=(i+jumps[j])%n;
        if (k<0)
            k+=n;
        add_edge(i,k);
        j=(j+1)%m;
    }
}

/* create LCF graph from int pointer (list must end with 0) */
void graphe::make_lcf_graph(const int *j,int e) {
    ivector jumps;
    const int *it=j;
    while (*it!=0) {
        jumps.push_back(*it);
        ++it;
    }
    make_lcf_graph(jumps,e);
}

/* create Sierpinski (triangle) graph */
void graphe::make_sierpinski_graph(int n, int k, bool triangle) {
    ivectors tuples;
    ivector elem(n,0);
    ntupk(tuples,n,k,elem,0);
    int N=std::pow(k,n);
    vecteur V;
    make_default_labels(V,N,0);
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

/* create the Shrikhande graph */
void graphe::make_shrikhande_graph() {
    this->clear();
    vecteur V;
    make_default_labels(V,16);
    add_nodes(V);
    ipairs v(16);
    int k=0,m,n;
    for (int i=0;i<4;++i) {
        for (int j=0;j<4;++j) {
            v[k++]=make_pair(i,j);
        }
    }
    for (int i=0;i<16;++i) {
        const ipair &vi=v[i];
        for (int j=i+1;j<16;++j) {
            const ipair &vj=v[j];
            m=(vi.first-vj.first+4)%4;
            n=(vi.second-vj.second+4)%4;
            if ((m*n==0 && (m+n)%2!=0) || (m==n && (m*n)%2!=0))
                add_edge(i,j);
        }
    }
}

/* create complete graph with vertices from list V */
void graphe::make_complete_graph() {
    int n=node_count();
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
        make_default_labels(v,n,n0);
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
    vecteur V;
    make_default_labels(V,2*n);
    add_nodes(V);
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

/* generate all k-sets in a n-set */
void graphe::generate_nk_sets(int n,int k,vector<ulong> &v) {
    ulong N=std::pow(2,n);
    int i=0;
    for (ulong m=1;m<N;++m) {
        bitset<32> b(m);
        if (b.count()==(size_t)k)
            v[i++]=m;
    }
}

/* create Kneser graph with parameters n (<=20) and k */
bool graphe::make_kneser_graph(int n,int k) {
    assert(n>1 && n<21 && k>0 && k<n);
    int nchoosek=comb(n,k).val; // number of vertices
    if (nchoosek>100000) {
        message("Error: graph too large");
        return false;
    }
    vecteur V;
    make_default_labels(V,nchoosek);
    add_nodes(V);
    vector<ulong> vert(nchoosek);
    generate_nk_sets(n,k,vert);
    ulong a,b;
    for (int i=0;i<nchoosek;++i) {
        a=vert[i];
        for (int j=i+1;j<nchoosek;++j) {
            b=vert[j];
            if ((a & b)==0)
                add_edge(i,j);
        }
    }
    return true;
}

/* create path graph with n vertices */
void graphe::make_path_graph() {
    int n=node_count();
    for (int i=0;i<n-1;++i) {
        add_edge(i,i+1);
    }
}

/* create n times m (torus) grid graph */
void graphe::make_grid_graph(int m,int n,bool torus) {
    vecteur V;
    graphe X(ctx);
    X.make_default_labels(V,m);
    X.add_nodes(V);
    if (torus)
        X.make_cycle_graph();
    else X.make_path_graph();
    graphe Y(ctx);
    Y.make_default_labels(V,n);
    Y.add_nodes(V);
    if (torus)
        Y.make_cycle_graph();
    else Y.make_path_graph();
    X.cartesian_product(Y,*this);
}

/* create n times m web graph as the cartesian product of n-cycle and m-path graphs */
void graphe::make_web_graph(int n,int m) {
    vecteur V;
    graphe C(ctx);
    C.make_default_labels(V,n);
    C.add_nodes(V);
    C.make_cycle_graph();
    graphe P(ctx);
    P.make_default_labels(V,m);
    P.add_nodes(V);
    P.make_path_graph();
    C.cartesian_product(P,*this);
}

/* create wheel graph with n+1 vertices */
void graphe::make_wheel_graph(int n) {
    vecteur V;
    make_default_labels(V,n,0,1);
    add_nodes(V);
    make_cycle_graph();
    int i=add_node(0);
    for (int j=0;j<n;++j) {
        add_edge(i,j);
    }
}

/* create antiprism graph of order n (with 2n vertices and 4n edges) */
void graphe::make_antiprism_graph(int n) {
    vecteur V;
    make_default_labels(V,2*n,0);
    add_nodes(V);
    int j;
    for (int i=0;i<n;++i) {
        j=(i+1)%n;
        add_edge(2*i,2*j);
        add_edge(2*i+1,2*j+1);
        add_edge(2*j,2*i+1);
        add_edge(2*j,2*j+1);
    }
}

/* create the complete k-ary tree with depth d */
void graphe::make_complete_kary_tree(int k,int d) {
    assert(k>=2);
    int n=((int)std::pow(k,d+1)-1)/(k-1),v=0,w=1,len=1;
    vecteur V;
    make_default_labels(V,n);
    add_nodes(V);
    for (int i=0;i<d;++i) {
        for (int j=0;j<len;++j) {
            for (int m=0;m<k;++m) {
                add_edge(v,w++);
            }
            ++v;
        }
        len*=k;
    }
}

/* find all connected components of an undirected graph and store them */
void graphe::connected_components(ivectors &components,int sg,bool skip_embedded,int *count) {
    unvisit_all_nodes(sg);
    unset_all_ancestors(sg);
    disc_time=0;
    if (count==NULL)
        components.resize(node_count());
    int c=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if ((sg<0 || it->subgraph()==sg) && (!skip_embedded || !it->is_embedded()) && !it->is_visited())
            dfs(it-nodes.begin(),true,false,&components[c++],sg,skip_embedded);
    }
    if (count==NULL)
        components.resize(c);
    else
        *count=c;
}

void graphe::strongconnect_dfs(ivectors &components,int i,int sg) {
    vertex &v=node(i);
    v.set_visited(true);
    v.set_disc(disc_time++);
    v.set_low(v.disc());
    node_stack.push(i);
    v.set_on_stack(true);
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        vertex &w=node(*it);
        if (sg>=0 && w.subgraph()!=sg)
            continue;
        if (!w.is_visited()) {
            strongconnect_dfs(components,*it,sg);
            v.set_low(std::min(v.low(),w.low()));
        } else if (w.is_on_stack())
            v.set_low(std::min(v.low(),w.disc()));
    }
    if (v.low()==v.disc()) {
        // output a strongly connected component
        components.resize(components.size()+1);
        ivector &component=components.back();
        do {
            component.push_back(node_stack.top());
            node_stack.pop();
            v.set_on_stack(false);
        } while (component.back()!=i);
    }
}

/* find all strongly connected components in directed graph using Tarjan's algorithm */
void graphe::strongly_connected_components(ivectors &components,int sg) {
    unvisit_all_nodes(sg);
    disc_time=0;
    clear_node_stack();
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if ((sg<0 || it->subgraph()==sg) && !it->is_visited())
            strongconnect_dfs(components,it-nodes.begin(),sg);
    }
}

/* return true iff the connected graph is not biconnected (i.e. has an articulation point) */
bool graphe::has_cut_vertex(int sg,int i) {
    vertex &v=node(i);
    if (i==0) {
        unvisit_all_nodes();
        unset_all_ancestors();
        disc_time=0;
    }
    if (sg>=0 && v.subgraph()!=sg)
        return i==node_count()-1?false:has_cut_vertex(sg,i+1);
    v.set_visited(true);
    ++disc_time;
    v.set_disc(disc_time);
    v.set_low(disc_time);
    int children=0;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        int j=*it;
        if (j<0) j=-j-1;
        vertex &w=node(j);
        if (sg>=0 && w.subgraph()!=sg)
            continue;
        if (!w.is_visited()) {
            ++children;
            w.set_ancestor(i);
            if (has_cut_vertex(sg,j))
                return true;
            if (v.ancestor()<0) {
                if (children==2)
                    return true;
            } else {
                v.set_low(std::min(v.low(),w.low()));
                if (w.low()>=v.disc())
                    return true;
            }
        } else if (j!=v.ancestor() && w.disc()<v.disc())
            v.set_low(std::min(v.low(),w.disc()));
    }
    return false;
}

void graphe::find_cut_vertices_dfs(int i,ivector &ap,int sg) {
    vertex &v=node(i);
    v.set_visited(true);
    ++disc_time;
    v.set_disc(disc_time);
    v.set_low(disc_time);
    int children=0;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        int j=*it;
        if (j<0) j=-j-1;
        vertex &w=node(j);
        if (sg>=0 && w.subgraph()!=sg)
            continue;
        if (!w.is_visited()) {
            ++children;
            w.set_ancestor(i);
            find_cut_vertices_dfs(j,ap,sg);
            if (v.ancestor()<0) {
                if (children==2)
                    ap.push_back(i);
            } else {
                v.set_low(std::min(v.low(),w.low()));
                if (w.low()>=v.disc())
                    ap.push_back(i);
            }
        } else if (j!=v.ancestor() && w.disc()<v.disc())
            v.set_low(std::min(v.low(),w.disc()));
    }
}

/* return list of cut vertices obtained by using depth-first search, time complexity O(n+m) */
void graphe::find_cut_vertices(ivector &articulation_points,int sg) {
    unvisit_all_nodes();
    unset_all_ancestors();
    disc_time=0;
    articulation_points.clear();
    articulation_points.reserve(node_count());
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if ((sg<0 || it->subgraph()==sg) && !it->is_visited())
            find_cut_vertices_dfs(it-nodes.begin(),articulation_points,sg);
    }
}

void graphe::find_blocks_dfs(int i,vector<ipairs> &blocks,int sg) {
    ++disc_time;
    vertex &v=node(i);
    v.set_disc(disc_time);
    v.set_low(disc_time);
    v.set_visited(true);
    int j;
    ipair edge;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        j=*it;
        if (j<0) j=-j-1;
        vertex &w=node(j);
        if (sg>=0 && w.subgraph()!=sg)
            continue;
        edge=make_pair(i<j?i:j,i<j?j:i);
        if (!w.is_visited()) {
            w.set_ancestor(i);
            edge_stack.push(edge);
            find_blocks_dfs(j,blocks,sg);
            v.set_low(std::min(v.low(),w.low()));
            if (w.low()>=v.disc()) {
                // output biconnected component to 'blocks'
                blocks.resize(blocks.size()+1);
                ipairs &block=blocks.back();
                do {
                    block.push_back(edge_stack.top());
                    edge_stack.pop();
                } while (block.back()!=edge);
            }
        } else if (w.disc()<v.disc() && j!=v.ancestor()) {
            edge_stack.push(edge);
            v.set_low(std::min(v.low(),w.disc()));
        }
    }
}

/* create list of all biconnected components (as lists of edges) of the graph */
void graphe::find_blocks(vector<ipairs> &blocks,int sg) {
    unvisit_all_nodes(sg);
    unset_all_ancestors(sg);
    disc_time=0;
    assert(edge_stack.empty());
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if ((sg<0 || it->subgraph()==sg) && !it->is_visited())
            find_blocks_dfs(it-nodes.begin(),blocks,sg);
    }
    while (!edge_stack.empty()) edge_stack.pop();
}

void graphe::find_bridges_dfs(int i,ipairs &B,int sg) {
    vertex &v=node(i);
    v.set_visited(true);
    ++disc_time;
    v.set_disc(disc_time);
    v.set_low(disc_time);
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        int j=*it;
        if (j<0) j=-j-1;
        vertex &w=node(j);
        if (sg>=0 && w.subgraph()!=sg)
            continue;
        if (!w.is_visited()) {
            w.set_ancestor(i);
            find_bridges_dfs(j,B,sg);
            v.set_low(std::min(v.low(),w.low()));
            if (w.low()>v.disc())
                B.push_back(make_pair(i<j?i:j,i<j?j:i));
        } else if (j!=v.ancestor() && w.disc()<v.disc())
            v.set_low(std::min(v.low(),w.disc()));
    }
}


/* create list B of all bridges in an undirected graph */
void graphe::find_bridges(ipairs &B,int sg) {
    assert(!is_directed());
    unvisit_all_nodes(sg);
    unset_all_ancestors(sg);
    disc_time=0;
    B.clear();
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if ((sg<0 || it->subgraph()==sg) && !it->is_visited())
            find_bridges_dfs(it-nodes.begin(),B,sg);
    }
}

/* attempt to find an eulerian path in this graph using Fleury's algorithm,
 * return true iff one exists */
bool graphe::find_eulerian_path(ivector &path) const {
    assert(!is_directed());
    graphe G(*this);
    if (!G.is_connected())
        return false;
    int n=G.node_count(),count=0,start_node=0;
    for (int i=0;i<n;++i) {
        if (G.degree(i)%2!=0) {
            ++count;
            start_node=i;
        }
    }
    if (count!=0 && count!=2)
        return false;
    int m=G.edge_count(),i=start_node,j;
    path.resize(m+1);
    path.back()=i;
    ipairs bridges;
    ipair edge;
    while (m>0) {
        vertex &v=G.node(i);
        G.find_bridges(bridges);
        for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
            j=*it;
            edge=make_pair(i<j?i:j,i<j?j:i);
            if (find(bridges.begin(),bridges.end(),edge)==bridges.end())
                break;
        }
        G.remove_edge(edge);
        path[--m]=j;
        i=j;
    }
    return true;
}

int graphe::find_cycle_dfs(int i,int sg) {
    vertex &v=node(i);
    v.set_visited(true);
    node_stack.push(i);
    int j,k;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        j=*it;
        if (j<0) j=-j-1;
        vertex &w=node(j);
        if (sg>=0 && w.subgraph()!=sg)
            continue;
        if (w.is_visited()) {
            if (v.ancestor()!=j)
                return j;
            continue;
        }
        w.set_ancestor(i);
        k=find_cycle_dfs(j,sg);
        if (k>=0)
            return k;
    }
    node_stack.pop();
    return -1;
}

/* return a cycle in this graph using DFS (graph is assumed to be connected),
 * or an empty list if there is no cycle (i.e. if the graph is a tree) */
bool graphe::find_cycle(ivector &cycle,int sg) {
    clear_node_stack();
    if (is_empty())
        return false;
    int n=node_count(),initv,i;
    unvisit_all_nodes(sg);
    unset_all_ancestors(sg);
    cycle.clear();
    cycle.reserve(n);
    if (sg>=0) {
        for (initv=0;initv<n && node(initv).subgraph()!=sg;++initv);
        if (initv==n)
            return false;
    } else initv=rand_integer(n);
    if ((i=find_cycle_dfs(initv,sg))>=0) {
        do {
            cycle.push_back(node_stack.top());
            node_stack.pop();
        } while (cycle.back()!=i);
        while (!node_stack.empty()) node_stack.pop();
        return true;
    }
    return false;
}

bool graphe::find_path_dfs(int dest,int i,int sg,bool skip_embedded) {
    node_stack.push(i);
    if (i==dest)
        return true;
    vertex &v=node(i);
    v.set_visited(true);
    int j;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        j=*it;
        if (j<0) j=-j-1;
        vertex &w=node(j);
        if ((skip_embedded && w.is_embedded()) || w.subgraph()!=sg || w.is_visited())
            continue;
        if (find_path_dfs(dest,j,sg,skip_embedded))
            return true;
    }
    node_stack.pop();
    return false;
}

/* return a path from i-th to j-th vertex using DFS (graph is assumed to be connected) */
bool graphe::find_path(int i,int j,ivector &path,int sg,bool skip_embedded) {
    unvisit_all_nodes(sg);
    clear_node_stack();
    if (find_path_dfs(i,j,sg,skip_embedded)) {
        path.resize(node_stack.size());
        while (!node_stack.empty()) {
            path[node_stack.size()-1]=node_stack.top();
            node_stack.pop();
        }
        return true;
    }
    return false;
}

/* insert edges from G to this graph */
void graphe::join_edges(const graphe &G) {
    bool weighted=G.is_weighted() && this->is_weighted();
    vecteur E=G.edges(weighted);
    gen v,w,wgh(1);
    for (const_iterateur it=E.begin();it!=E.end();++it) {
        if (weighted) {
            v=it->_VECTptr->front()._VECTptr->front();
            w=it->_VECTptr->front()._VECTptr->back();
            wgh=it->_VECTptr->back();
        } else {
            v=it->_VECTptr->front();
            w=it->_VECTptr->back();
        }
        add_edge(v,w,wgh);
    }
}

/* clear the graph, deleting all nodes and vertices */
void graphe::clear() {
    unmark_all_nodes();
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

/* set 'embedded' field of all given nodes to 'true' */
void graphe::set_nodes_embedded(const ivector &v,bool yes) {
    for (ivector_iter it=v.begin();it!=v.end();++it) {
        node(*it).set_embedded(yes);
    }
}

/* set 'embedded' filed for all nodes to 'false' */
void graphe::clear_embedding() {
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->set_embedded(false);
    }
}

/* extract path from i-th to j-th node of the given cycle, going counterclockwise */
void graphe::extract_path_from_cycle(const ivector &cycle,int i,int j,ivector &path) {
    int n=cycle.size();
    path.clear();
    for (int k=i;k!=j;k=(k+1)%n) {
        path.push_back(cycle[k]);
    }
    path.push_back(cycle[j]);
}

/* finds planar embedding of a biconnected graph as a list of faces,
 * returns true iff the graph is planar */
bool graphe::demoucron(ivectors &faces) {
    ivector cycle,path,face1,face2;
    ivectors bridges,components(node_count());
    ipairs admissible_faces;
    int i,j,k,n,f,s=-1,vc=node_count(),ec=edge_count(),fc=ec-vc+2,bc=0,cnt;
    ivectors_iter ft;
    ivector_iter ct;
    set<int> contact_nodes;
    clear_embedding();
    // adding two initial faces, obtained by finding a cycle in graph
    assert(find_cycle(cycle));
    set_nodes_embedded(cycle);
    faces.clear();
    faces.reserve(fc);
    faces.push_back(ivector(cycle.begin(),cycle.end()));
    faces.push_back(ivector(cycle.rbegin(),cycle.rend()));
    while (true) {
        for (node_iter it=nodes.begin();it!=nodes.end();++it) {
            const vertex &v=*it;
            if (!v.is_embedded() || (s>=0 && v.subgraph()!=s))
                continue;
            i=it-nodes.begin();
            for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
                const vertex &w=node(j=*jt);
                if (!w.is_embedded() || (w.subgraph()==s && j<i))
                    continue;
                for (ft=faces.begin();ft!=faces.end();++ft) {
                    if (face_has_edge(*ft,i,j)>=0)
                        break;
                }
                if (ft==faces.end()) {
                    // create a bridge consisting only of edge (i,j)
                    ivector bridge(4);
                    bridge.front()=++bc; // subgraph index
                    bridge[1]=0; // bridge size
                    bridge[2]=i; // contact vertex 1
                    bridge[3]=j; // contact vertex 2
                    bridges.push_back(bridge);
                }
            }
        }
        n=bridges.size();
        connected_components(components,s,true,&cnt);
        bridges.resize(n+cnt);
        for (k=0;k<cnt;++k) {
            ivector comp=components[k];
            ivector &bridge=bridges[n++];
            bridge.clear();
            bridge.push_back(++bc);
            bridge.push_back(comp.size());
            contact_nodes.clear();
            for (ivector_iter it=comp.begin();it!=comp.end();++it) {
                vertex &v=node(*it);
                v.set_subgraph(bc);
                for (ivector_iter jt=v.neighbors().begin();jt!=v.neighbors().end();++jt) {
                    if (node(*jt).is_embedded())
                        contact_nodes.insert(*jt);
                }
            }
            bridge.resize(2+contact_nodes.size());
            i=2;
            for (set<int>::const_iterator it=contact_nodes.begin();it!=contact_nodes.end();++it) {
                bridge[i++]=*it;
            }
        }
        if (bridges.empty())
            break;
        // for each bridge, find all faces in which it can
        // be drawn (these faces are called 'admissible')
        admissible_faces.resize(bridges.size());
        for (ivectors_iter it=bridges.begin();it!=bridges.end();++it) {
            ipair &admissible=admissible_faces[it-bridges.begin()];
            admissible.first=0;
            for (ivectors_iter ft=faces.begin();ft!=faces.end();++ft) {
                for (ct=it->begin()+2;ct!=it->end();++ct) {
                    if (find(ft->begin(),ft->end(),*ct)==ft->end())
                        break;
                }
                if (ct==it->end()) {
                    admissible.first++;
                    admissible.second=ft-faces.begin();
                }
            }
            if (admissible.first==0) // the graph is not planar
                return false;
        }
        // select the first bridge with the smallest number of admissible faces
        n=RAND_MAX;
        for (ipairs_iter it=admissible_faces.begin();it!=admissible_faces.end();++it) {
            if (it->first<n) {
                k=it-admissible_faces.begin();
                if ((n=it->first)==1)
                    break;
            }
        }
        ivector &bridge=bridges[k];
        // draw a path between the first two connecting vertices
        // to the admissible face, splitting it
        s=bridge.front(); // subgraph index
        if ((n=bridge[1])>0) {
            i=first_neighbor_from_subgraph(node(bridge[2]),s);
            j=first_neighbor_from_subgraph(node(bridge[3]),s);
            assert(i>=0 && j>=0 && find_path(i,j,path,s,true));
            set_nodes_embedded(path);
        } else path.clear();
        f=admissible_faces[k].second;
        ivector &face=faces[f];
        i=find(face.begin(),face.end(),bridge[2])-face.begin();
        j=find(face.begin(),face.end(),bridge[3])-face.begin();
        extract_path_from_cycle(face,i,j,face1);
        extract_path_from_cycle(face,j,i,face2);
        if (path.size()>0) {
            face1.insert(face1.end(),path.begin(),path.end());
            face2.insert(face2.end(),path.rbegin(),path.rend());
        }
        faces.erase(faces.begin()+f);
        faces.push_back(face1);
        faces.push_back(face2);
        bridges.erase(bridges.begin()+k); // we're done with this bridge
    }
    // we shuld have had found exactly ec-nc+2 faces (by Euler's theorem)
    assert(int(faces.size())==fc);
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
        std::reverse(path.begin(),path.end());
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
    // split graph to blocks
    vector<ipairs> blocks;
    find_blocks(blocks);
    if (blocks.size()==1) {
        if (!demoucron(faces))
            return -1;
    } else {
        // there exist at least one articulation point
        vector<ivectors> blocks_faces(blocks.size());
        ivector cv;
        find_cut_vertices(cv);
        int i=0,nf;
        for (vector<ipairs>::const_iterator it=blocks.begin();it!=blocks.end();++it) {
            // test each block separately
            graphe G(ctx);
            subgraph(*it,G,false);
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
    int v,n=largest_integer_label_value();
    for (ivectors_iter ft=faces.begin();ft!=faces.end();++ft) {
        if (f0==int(ft-faces.begin()))
            continue;
        v=add_node(++n);
        for (ivector_iter it=ft->begin();it!=ft->end();++it) {
            add_edge(v,*it);
        }
    }
}

/* compute the periphericity of the vertices with respect to
 * the outer face by applying BFS (starting in each of the outer vertices) */
void graphe::periphericity(const ivector &outer_face,ivector &p) {
    queue<int> q;
    int level,i,j;
    std::fill(p.begin(),p.end(),RAND_MAX);
    for (ivector_iter jt=outer_face.begin();jt!=outer_face.end();++jt) {
        p[*jt]=0;
    }
    for (ivector_iter it=outer_face.begin();it!=outer_face.end();++it) {
        unvisit_all_nodes();
        q.push(*it);
        level=1;
        while (!q.empty()) {
            i=q.front();
            vertex &v=node(i);
            for (ivector_iter jt=v.neighbors().begin();jt!=v.neighbors().end();++jt) {
                j=*jt;
                if (j<0) j=-j-1;
                vertex &w=node(j);
                if (w.is_visited() || p[j]==0) // skip the outer vertices
                    continue;
                if (level<p[j])
                    p[j]=level;
                q.push(j);
                w.set_visited(true);
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
            w.set_ancestor(i);
        walk(j,pass,level+1,modsum+m);
        isleaf=false;
    }
    if (pass==1) {
        v.set_is_leaf(isleaf);
        ++node_counters[level];
        depth=std::max(depth,level+1);
    } else if (pass==2) {
        if (node_counters[level]>0 && G->node(levels[level][node_counters[level]-1]).ancestor()!=v.ancestor())
            ++gap_counters[level];
        v.set_position(node_counters[level]++);
        levels[level][v.position()]=i;
        if (!isleaf) {
            v.set_gaps(gap_counters[level]);
            gap_counters[level]=0;
        }
    }
}

/* set prelim and modifier for each node in V (they are on the same level) */
void graphe::tree_node_positioner::process_level(int i) {
    ivector &L=levels[i];
    int lastp=G->node(L.front()).ancestor(),p,n=0,m=placed.size();
    double ppos=0,xpos=0,dist,shift=0,ssep=0,step=1,min_dist;
    if (m>0) {
        vertex &w=G->node(placed.front());
        xpos=w.prelim()-w.position()*hsep;
    }
    for (ivector_iter it=L.begin();it!=L.end();++it) {
        vertex &v=G->node(*it);
        p=v.ancestor();
        if (p!=lastp) {
            G->node(lastp).set_prelim(ppos/n);
            placed.push(lastp);
            lastp=p;
            ppos=0;
            n=0;
            xpos+=ssep;
        }
        if (v.is_leaf()) {
            v.set_prelim(xpos);
        } else {
            v.set_prelim(v.prelim()+shift);
            v.set_modifier(shift);
            xpos=v.prelim();
            step=1;
            ssep=0;
            placed.pop();
            --m;
            if (m>0) {
                vertex &w=G->node(placed.front());
                dist=w.prelim()+shift-xpos;
                min_dist=(w.position()-v.position())*hsep;
                if (dist<min_dist)
                    shift+=min_dist-dist;
                else if (v.ancestor()==w.ancestor())
                    step=dist/min_dist;
                else
                    ssep=(dist-min_dist)/w.gaps();
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
    walk(apex,1); // first walk: determine tree depth and set node ancestors
    // allocate memory for level lists
    clock_t alloc_start=clock();
    levels.resize(depth);
    gap_counters.resize(depth,0);
    for (int i=0;i<depth;++i) {
        levels[i].resize(node_counters[i]);
        node_counters[i]=0;
    }
    double alloc_time=double(clock()-alloc_start)/CLOCKS_PER_SEC;
    G->unvisit_all_nodes();
    walk(apex,2); // second walk: create levels
    // do node positioning for each level (except the top one) in a single sweep
    for (int level=depth;level-->1;) {
        process_level(level);
    }
    G->unvisit_all_nodes();
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
    node_counters.resize(n,0);
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
    unvisit_all_nodes();
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->set_prelim(-1);
        it->set_modifier(0);
    }
    // layout the tree
    double hsep=sep,vsep=sep*std::pow(PLASTIC_NUMBER,2);
    tree_node_positioner P(this,&x,hsep,vsep);
    return P.positioning(apex);
}

/* create a random tree with n vertices and degree not larger than maxd */
void graphe::make_random_tree(const vecteur &V, int maxd) {
    this->clear();
    add_nodes(V);
    vecteur src,labels=*_randperm(V,ctx)._VECTptr;
    src.push_back(labels.pop_back());
    gen v,w;
    while (!labels.empty()) {
        v=_rand(src,ctx);
        w=labels.pop_back();
        add_edge(v,w);
        src.push_back(w);
        if (degree(node_index(v))==maxd) {
            iterateur it=find(src.begin(),src.end(),v);
            assert(it!=src.end());
            src.erase(it);
        }
    }
}

/* use DFS traversal to compute tree depth */
void graphe::tree_height_dfs(int i,int level,int &depth) {
    vertex &v=node(i);
    v.set_visited(true);
    depth=std::max(depth,level);
    int j;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        j=*it;
        if (j<0) j=-j-1;
        vertex &w=node(j);
        if (!w.is_visited())
            tree_height_dfs(j,level+1,depth);
    }
}

/* return the height of this tree (no check is made) */
int graphe::tree_height(int root) {
    // assuming that this is a tree
    if (node_count()==1)
        return 0;
    unvisit_all_nodes();
    int depth=0;
    tree_height_dfs(root,0,depth);
    return depth;
}


/* create a random biconnected planar graph with n vertices */
void graphe::make_random_planar(bool biconnected) {
    set_directed(false);
    // start with a random maximal planar graph with n vertices
    int n=node_count();
    ivectors faces;
    ivector face(3),old_face;
    for (int i=0;i<3;++i) {
        face[i]=i;
        add_edge(i,(i+1)%3);
    }
    faces.push_back(face);
    ivector outer_face(face.begin(),face.end());
    std::reverse(outer_face.begin(),outer_face.end());
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
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        d1=degree(it->first,false);
        d2=degree(it->second,false);
        if (d1<4 || d2<4)
            continue;
        k=giac_rand(ctx);
        m=(d1+d2)/4;
        if (k%m!=0)
            remove_edge(*it);
    }
    // check that the resulting graph is connected; if not, repeat the process
    if ((biconnected && !is_biconnected()) || !is_connected()) {
        for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
            it->clear_neighbors();
        }
        make_random_planar();
    }
}

/* create a random (directed) graph with vertices from V */
void graphe::make_random(bool dir,const vecteur &V,double p) {
    this->clear();
    set_directed(dir);
    add_nodes(V);
    int n=node_count(),m=std::floor(p),i,j,k;
    ipairs E;
    E.reserve(n*n);
    for (i=0;i<n;++i) {
        for (j=dir?0:i+1;j<n;++j) {
            if (m==0) {
                if (i!=j && rand_uniform()<p)
                    add_edge(i,j);
            } else
                E.push_back(make_pair(i,j));
        }
    }
    if (m>0) {
        for (int c=0;c<m;++c) {
            k=rand_integer(E.size());
            add_edge(E[k]);
            E.erase(E.begin()+k);
        }
    }
}

/* create a random bipartite graph with two groups of vertices V and W */
void graphe::make_random_bipartite(const vecteur &V,const vecteur &W,double p) {
    this->clear();
    set_directed(false);
    int a=V.size(),b=W.size(),m=std::floor(p),k;
    add_nodes(V);
    add_nodes(W);
    ipairs E;
    E.reserve(a*b);
    for (int i=0;i<a;++i) {
        for (int j=a;j<a+b;++j) {
            if (m==0) {
                if (rand_uniform()<p)
                    add_edge(i,j);
            } else
                E.push_back(make_pair(i,j));
        }
    }
    if (m>0) {
        for (int c=0;c<m;++c) {
            k=rand_integer(E.size());
            add_edge(E[k]);
            E.erase(E.begin()+k);
        }
    }
}

/* create a random d-regular graph with vertices fromm V */
void graphe::make_random_regular(const vecteur &V,int d,bool connected) {
    set_directed(false);
    ipairs E;
    int n=V.size();
    E.reserve(n*n);
    ivector prob,degrees(n);
    prob.reserve(n*n);
    int prob_total,maxd=0,k,dd;
    double r;
    ipair edge;
    do {
        if (connected)
            make_random_tree(V,d);
        else {
            this->clear();
            add_nodes(V);
        }
        for (int i=0;i<n;++i) {
            degrees[i]=degree(i);
        }
        E.clear();
        for (int i=0;i<n;++i) {
            if ((dd=degrees[i])>maxd)
                maxd=dd;
            if (dd<d) {
                for (int j=i+1;j<n;++j) {
                    if (!nodes_are_adjacent(i,j) && degrees[j]<d)
                        E.push_back(make_pair(i,j));
                }
            }
        }
        while (!E.empty()) {
            prob.resize(E.size());
            prob_total=0;
            for (ipairs_iter it=E.begin();it!=E.end();++it) {
                prob_total+=(maxd-degrees[it->first])*(maxd-degrees[it->second]);
                prob[it-E.begin()]=prob_total;
            }
            r=rand_uniform()*prob_total;
            k=0;
            for (ivector_iter it=prob.begin();it!=prob.end();++it) {
                if (r<*it)
                    break;
                ++k;
            }
            edge=E[k];
            E.erase(E.begin()+k);
            if (++degrees[edge.first]>maxd || ++degrees[edge.second]>maxd)
                ++maxd;
            add_edge(edge);
            for (k=E.size();k-->0;) {
                ipair &e=E[k];
                if (degrees[e.first]==d || degrees[e.second]==d)
                    E.erase(E.begin()+k);
            }
        }
    } while (!is_regular(d));
}

/* return true iff the graph is d-regular */
bool graphe::is_regular(int d) const {
    int n=node_count();
    for (int i=0;i<n;++i) {
        if (degree(i)!=d)
            return false;
    }
    return true;
}

/* create a random flow network */
void graphe::make_random_flow_network(const vecteur &V,int capacity) {
    assert(!V.empty());
    add_nodes(V);
    make_random_planar(true);
}

/* return true iff this graph is equal to G */
bool graphe::is_equal(const graphe &G) const {
    if (is_directed()!=G.is_directed() || is_weighted()!=G.is_weighted())
        return false;
    vecteur V1=vertices(),V2=G.vertices();
    if (V1!=V2)
        return false;
    ipairs E1,E2;
    edgeset edg,edG;
    get_edges_as_pairs(E1,false);
    G.get_edges_as_pairs(E2,false);
    ipairs2edgeset(E1,edg);
    ipairs2edgeset(E2,edG);
    if (edg!=edG)
        return false;
    if (is_weighted()) {
        for (edgeset_iter it=edg.begin();it!=edg.end();++it) {
            if (weight(*it)!=G.weight(*it))
                return false;
        }
    }
    return true;
}

/* sort rectangles by height */
void graphe::sort_rectangles(vector<rectangle> &rectangles) {
    rectangle::comparator comp;
    sort(rectangles.begin(),rectangles.end(),comp);
}

/* packing rectangles (sorted by height) into an enclosing rectangle with specified dimensions,
 * returns true if embedding has changed */
bool graphe::embed_rectangles(const vector<rectangle> &rectangles,dpairs &embedding,double ew,double eh,double eps) {
    vector<rectangle> blanks;
    blanks.push_back(rectangle(0,0,ew,eh));
    double xpos,ypos,w,h;
    int k;
    bool embedding_changed=false;
    for (vector<rectangle>::const_iterator it=rectangles.begin();it!=rectangles.end();++it) {
        // find the leftmost blank which can hold the rectangle
        k=-1;
        for (vector<rectangle>::const_iterator jt=blanks.begin();jt!=blanks.end();++jt) {
            if (jt->width()-it->width()>-eps &&
                    jt->height()-it->height()>-eps &&
                    (k<0 || jt->x()<xpos)) {
                k=jt-blanks.begin();
                xpos=jt->x();
            }
        }
        assert(k>=0);
        // store blank dimensions and position, for splitting
        rectangle &blank=blanks[k];
        ypos=blank.y();
        w=blank.width();
        h=blank.height();
        blanks.erase(blanks.begin()+k); // delete blank in which the rectangle is inserted
        dpair newpos=make_pair(xpos,ypos);
        dpair &pos=embedding[it-rectangles.begin()];
        if (newpos!=pos) {
            pos=newpos;
            embedding_changed=true;
        }
        // add new (smaller) blanks obtained by splitting the deleted blank
        blanks.push_back(rectangle(xpos+it->width(),ypos,w-it->width(),it->height()));
        blanks.push_back(rectangle(xpos,ypos+it->height(),w,h-it->height()));
        // move iterator to the next rectangle and start over
    }
    return embedding_changed;
}

/* pack rectangles (sorted by height) to an enclosing rectangle with minimal perimeter and wasted space */
void graphe::pack_rectangles(const vector<rectangle> &rectangles,dpairs &best_embedding) {
    int n=rectangles.size();
    // compute total area occupied by the rectangles
    double total_area=0;
    for (vector<rectangle>::const_iterator it=rectangles.begin();it!=rectangles.end();++it) {
        total_area+=it->width()*it->height();
    }
    // step = the length of a negligible part of the shorter side of the smallest rectangle
    double maxwidth=0,minwidth=DBL_MAX,minheight=rectangles.back().height();
    for (vector<rectangle>::const_iterator it=rectangles.begin();it!=rectangles.end();++it) {
        if (it->width()>maxwidth)
            maxwidth=it->width();
        if (it->width()<minwidth)
            minwidth=it->width();
    }
    double step=std::min(minwidth,minheight)*NEGLIGIBILITY_FACTOR;
    double ew=DBL_MAX,eh=rectangles.front().height(); // initial enclosing rectangle has an "unlimited" width
    double perim,best_perim=DBL_MAX,d;
    dpairs embedding(n,make_pair(-1,-1));
    while (ew>maxwidth+step) { // loop breaks after a stacked embedding is obtained
        if (embed_rectangles(rectangles,embedding,ew,eh,step*MARGIN_FACTOR)) {
            ew=eh=0;
            // find the smallest enclosing rectangle containing the embedding
            for (dpairs::const_iterator it=embedding.begin();it!=embedding.end();++it) {
                const rectangle &rect=rectangles[it-embedding.begin()];
                if ((d=it->first+rect.width())>ew)
                    ew=d;
                if ((d=it->second+rect.height())>eh)
                    eh=d;
            }
            // find the embedding with the smaller perimeter (when scaled by the wasted ratio)
            if ((perim=(ew+PACKING_ASPECT_RATIO*eh)*ew*PACKING_ASPECT_RATIO*eh/total_area)<best_perim) {
                best_perim=perim;
                best_embedding=embedding;
            }
        }
        // increase enclosing rectangle height (rectangles will end up stacked eventually)
        eh+=step;
    }
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
        it->set_label(labels[i++]);
    }
    return true;
}

/* check for crossing edges with respect to the layout x */
bool graphe::has_crossing_edges(const layout &x,const ipairs &E) const {
    point crossing(2);
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        for (ipairs_iter jt=it+1;jt!=E.end();++jt) {
            if (edges_crossing(*it,*jt,x,crossing))
                return true;
        }
    }
    return false;
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

/* return the value of the largest integer node label */
int graphe::largest_integer_label_value() const {
    int n,m=-1;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->label().is_integer() && (n=it->label().val)>m)
            m=n;
    }
    return m;
}

/* return true iff two edges are incident to each other */
bool graphe::edges_incident(const ipair &e1,const ipair &e2) {
    return e1.first==e2.first || e1.first==e2.second || e1.second==e2.first || e1.second==e2.second;
}

/* return true iff two edges cross each other (also compute the crossing point) */
bool graphe::edges_crossing(const ipair &e1,const ipair &e2,const layout &x,point &crossing) const {
    if (edges_incident(e1,e2))
        return false;
    point p(2),q(2),r(2),s(2);
    copy_point(x[e1.first],p);
    copy_point(x[e2.first],q);
    copy_point(x[e1.second],r);
    copy_point(x[e2.second],s);
    subtract_point(r,p);
    subtract_point(s,q);
    return segments_crossing(p,r,q,s,crossing);
}

/* convert point to vecteur */
vecteur graphe::point2vecteur(const point &p) {
    vecteur res;
    for (point::const_iterator it=p.begin();it!=p.end();++it) {
        res.push_back(*it);
    }
    return res;
}

/* return true iff the points p and q coincide with each other within the given tolerance radius */
bool graphe::points_coincide(const point &p,const point &q,double tol) {
    assert(q.size()==p.size());
    point r(p.size());
    return point_distance(p,q,r)<=tol;
}

/*
 * IMPLEMENTATION OF AXIS CLASS
 */

graphe::axis::axis(const layout &x,int i,int j,const point &center) {
    m_v=i;
    m_w=j;
    const point &p=x[i],&q=x[j];
    double x1=p[0],x2=q[0],y1=p[1],y2=q[1],x0=center[0],y0=center[1];
    m_a=2*(y2-y1);
    m_b=2*(x2-x1);
    m_c=x1*x1+y1*y1-(x2*x2+y2*y2);
    m_d=std::abs(x0*(y2-y1)-y0*(x2-x1)+x2*y1-y2*x1)/
            std::sqrt(std::pow(y2-y1,2)+std::pow(x2-x1,2));
}

graphe::axis::axis(const axis &other) {
    assign(other);
}

graphe::axis& graphe::axis::operator =(const axis &other) {
    assign(other);
    return *this;
}

void graphe::axis::assign(const axis &other) {
    m_a=other.a();
    m_b=other.b();
    m_c=other.c();
    m_d=other.d();
    m_v=other.v();
    m_w=other.w();
}

void graphe::axis::mirror(const point &src,point &dest) const {
    assert(src.size()==2);
    double p=src[0],q=src[1],a2=a()*a(),b2=b()*b(),r=a2+b2,s=a2-b2;
    dest.resize(2);
    dest[0]=(p*s-2*b()*(a()*q+c()))/r;
    dest[1]=-(q*s+2*a()*(b()*p+c()))/r;
}

double graphe::axis::distance(const point &p) const {
    assert(p.size()==2);
    double num=a()*p.front()+b()*p.back()+c();
    double den=a()*a()+b()*b();
    return std::sqrt(num*num/den);
}

/*
 * END OF AXIS CLASS
 */

/*
 * IMPLEMENTATION OF THE DISJOINT SET DATA STRUCTURE
 */

bool graphe::disjoint_set::is_stored(int id) {
    assert(id>=0 && id<int(elements.size()));
    for (vector<element>::const_iterator it=elements.begin();it!=elements.end();++it) {
        if (it->id==id)
            return true;
    }
    return false;
}

void graphe::disjoint_set::make_set(int id) {
    if (is_stored(id))
        return;
    element &e=elements[id];
    e.id=id;
    e.parent=id;
    e.rank=0;
    e.size=1;
}

int graphe::disjoint_set::find(int id) {
    assert(id>=0 && id<int(elements.size()));
    element &e=elements[id];
    assert(e.id>=0);
    if (e.parent!=id)
        e.parent=find(e.parent);
    return e.parent;
}

void graphe::disjoint_set::unite(int id1,int id2,bool by_rank) {
    int p1=find(id1),p2=find(id2);
    if (p1==p2)
        return;
    element &x=elements[p1],&y=elements[p2];
    if ((by_rank && x.rank<y.rank) || (!by_rank && x.size<y.size))
        merge(y,x);
    else
        merge(x,y);
}

void graphe::disjoint_set::merge(element &x,element &y) {
   // merge y into x
   y.parent=x.id;
   x.size+=y.size;
   if (x.rank==y.rank)
       ++x.rank;
}

/*
 * END OF DISJOINT_SET CLASS
 */

/* promote all edge crossings to vertices (2D layout is required) */
void graphe::promote_edge_crossings(layout &x) {
    ipairs E;
    get_edges_as_pairs(E);
    int n=largest_integer_label_value(),i;
    point crossing;
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        for (ipairs_iter jt=it+1;jt!=E.end();++jt) {
            if (edges_crossing(*it,*jt,x,crossing)) {
                remove_edge(it->first,it->second);
                remove_edge(jt->first,jt->second);
                i=add_node(++n);
                add_edge(it->first,i);
                add_edge(it->second,i);
                add_edge(jt->first,i);
                add_edge(jt->second,i);
                set_node_attribute(i,_GT_ATTRIB_POSITION,point2gen(crossing));
                x.push_back(crossing);
            }
        }
    }
}

/* return the best axis of symmetry for this graph (layout is required) */
graphe::axis graphe::axis_of_symmetry(layout &x,const point &center,bool promote_crossings) {
    int nc=node_count();
    assert(nc>1);
    // find layout diameter
    double diam=layout_diam(x),tol=diam*std::sqrt(M_PI/nc)*MARGIN_FACTOR;
    double axtol=diam*MARGIN_FACTOR/2.0;
    if (promote_crossings)
        promote_edge_crossings(x);
    vector<axis> axes;
    for (int v=0;v<nc;++v) {
        for (int w=v+1;w<nc;++w) {
            axis ax(x,v,w,center);
            if (axes.empty() || ax.d()<axtol)
                axes.push_back(ax);
        }
    }
    axis::comparator comp;
    sort(axes.begin(),axes.end(),comp);
    axis best_axis;
    double best_score=-1,score;
    clock_t start=clock();
    point p1(2),p2(2),r(2);
    ipairs E;
    get_edges_as_pairs(E);
    vector<bool> visited(E.size());
    for (vector<axis>::const_iterator it=axes.begin();it!=axes.end();++it) {
        score=0;
        fill(visited.begin(),visited.end(),false);
        for (ipairs_iter et=E.begin();et!=E.end();++et) {
            if (visited[et-E.begin()])
                continue;
            it->mirror(x[et->first],p1);
            it->mirror(x[et->second],p2);
            for (ipairs_iter ft=et;ft!=E.end();++ft) {
                if (visited[ft-E.begin()])
                    continue;
                const point &q1=x[ft->first];
                const point &q2=x[ft->second];
                if ((points_coincide(p1,q1,tol) && points_coincide(p2,q2,tol)) ||
                        (points_coincide(p1,q2,tol) && points_coincide(q1,p2,tol))) {
                    score+=(point_distance(p1,p2,r)+point_distance(q1,q2,r))/2.0;
                    visited[ft-E.begin()]=true;
                }
            }
        }
        if (score>best_score) {
            best_score=score;
            best_axis=*it;
        }
        if (double(clock()-start)/CLOCKS_PER_SEC>SYMMETRY_DETECTION_TIMEOUT)
            break;
    }
    x.resize(nc);
    return best_axis;
}

/* make planar layout */
bool graphe::make_planar_layout(layout &x) {
    int n=node_count(),m,f,N,v,w,d,dl,dr;
    ivectors faces;
    faces.clear();
    // create the faces (adding temporary edges if necessary),
    // return the index of the outer face
    f=planar_embedding(faces);
    if (f<0)
        return false; // the graph is not planar
    ivector &outer_face=faces[f];
    m=outer_face.size();
    if (n>100 || !is_triconnected()) {
        //subdivide_faces(faces,f);
        triangulator T(this,&faces);
        T.triangulate(f);
    }
    // create a fake outer face
    ivector new_outer_face(m),degrees(m);
    N=largest_integer_label_value();
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
    make_circular_layout(x,new_outer_face,true);
    // remove temporary vertices
    while (node_count()>n) {
        remove_node(node_count()-1);
    }
    x.resize(n);
    //remove_temporary_edges();
    return true;
}

/* rotate layout x such that left to right symmetry is exposed */
void graphe::layout_best_rotation(layout &x) {
    // center layout in the origin
    point c=layout_center(x);
    scale_point(c,-1);
    translate_layout(x,c);
    // rotate layout around suitable bisector of a pair of vertices
    axis ax=axis_of_symmetry(x,c);
    if (ax.a()!=0)
        rotate_layout(x,M_PI_2+std::atan(ax.b()/ax.a()));
    // determine the highest and the lowest y coordinate: if yh+yl<0, rotate layout for 180°
    double yh=0,yl=0;
    for (layout::const_iterator it=x.begin();it!=x.end();++it) {
        double y=it->back();
        if (y>yh)
            yh=y;
        else if (y<yl)
            yl=y;
    }
    if (yh+yl<0)
        rotate_layout(x,M_PI);
}

/* return true iff the degrees of vertices in v all coincide */
bool graphe::degrees_equal(const ivector &v,int deg) const {
    int D=deg,d;
    for (ivector_iter it=v.begin();it!=v.end();++it) {
        d=degree(*it);
        if (D==0)
            D=d;
        else if (d!=D)
            return false;
    }
    return true;
}

/* guess the style to be used for drawing when no method is specified */
int graphe::guess_drawing_style() {
    ivector cycle;
    if (is_tree()) {
        return _GT_STYLE_TREE;
    } else if (get_leading_cycle(cycle) &&
            node_count()<100+int(cycle.size()) && degrees_equal(cycle,3) &&
            (node_count()==int(cycle.size()) || is_triconnected())) {
        return _GT_STYLE_CIRCLE;
    } else if (node_count()<100 && is_planar()) {
        return _GT_STYLE_PLANAR;
    }
    return _GT_STYLE_SPRING;
}

/* customize the Giac display */
gen customize_display(int options) {
    return symbolic(at_equal,makesequence(at_display,change_subtype(options,_INT_COLOR)));
}

/* append the line segment [p,q] to vecteur v */
void graphe::append_segment(vecteur &drawing,const point &p,const point &q,int color,int width,bool arrow) const {
    gen P=point2gen(p),Q=point2gen(q),args=makesequence(P,Q,customize_display(color | width));
    drawing.push_back(arrow?_vector(args,ctx):_segment(args,ctx));
}

/* append the vertex (as a circle) to vecteur v */
void graphe::append_vertex(vecteur &drawing,const point &p,int color,int width) const {
    gen P=point2gen(p,true);
    drawing.push_back(_point(makesequence(P,customize_display(color | width | _POINT_POINT)),ctx));
}

/* append label to vecteur v at the specified quadrant */
void graphe::append_label(vecteur &drawing,const point &p,const gen &label,int quadrant,int color) const {
    gen P=point2gen(p);
    drawing.push_back(_legende(makesequence(P,label,customize_display(quadrant | color)),ctx));
}

/* extract position attribute from attr */
bool graphe::get_position(const attrib &attr,point &p) {
    attrib_iter it=attr.find(_GT_ATTRIB_POSITION);
    if (it==attr.end())
        return false;
    const gen &pos=it->second;
    if (pos.type==_VECT || pos.is_symb_of_sommet(at_point)) {
        vecteur &v=pos.type==_VECT?*pos._VECTptr:*pos._SYMBptr->feuille._VECTptr;
        p.resize(v.size());
        for (const_iterateur it=v.begin();it!=v.end();++it) {
            if (!is_real_number(*it))
                return false;
            p[it-v.begin()]=it->DOUBLE_val();
        }
    } else { // assuming that pos is a complex number
        p.resize(2);
        if (pos.type==_CPLX) {
            gen &real=*pos._CPLXptr,&imag=*(pos._CPLXptr+1);
            if (!is_real_number(real) || !is_real_number(imag))
                return false;
            p.front()=real.DOUBLE_val();
            p.back()=imag.DOUBLE_val();
        } else {
            if (!is_real_number(pos))
                return false;
            p.front()=pos.DOUBLE_val();
            p.back()=0;
        }
    }
    return true;
}

/* append line segments representing edges (arcs) of the graph to vecteur v */
void graphe::draw_edges(vecteur &drawing,const layout &x) {
    if (x.empty())
        return;
    int i,j,color,width=default_edge_width;
    bool isdir=is_directed();
    double d;
    point r(x.front().size());
    attrib_iter ait;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        const point &p=x[i];
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
            if (j<0) j=-j-1;
            if (!isdir && j<i)
                continue;
            const point &q=x[j];
            const attrib &attr=it->neighbor_attributes(j);
            color=default_edge_color;
            if ((ait=attr.find(_GT_ATTRIB_COLOR))!=attr.end())
                color=ait->second.val;
            if (isdir) {
                assert((ait=attr.find(_GT_ATTRIB_POSITION))!=attr.end());
                d=ait->second.DOUBLE_val();
                point_lincomb(p,q,d,1-d,r);
                append_segment(drawing,p,r,color,width,true);
                append_segment(drawing,r,q,color,width,false);
            } else
                append_segment(drawing,p,q,color,width);
        }
    }
}

/* append points representing vertices of the graph to vecteur v */
void graphe::draw_nodes(vecteur &drawing,const layout &x) const {
    int color,width,n=node_count();
    if (n<=30)
        width=_POINT_WIDTH_4;
    else if (n<=130)
        width=_POINT_WIDTH_3;
    else if (n<=330)
        width=_POINT_WIDTH_2;
    else
        width=_POINT_WIDTH_1;
    attrib_iter ait;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        const attrib &attr=it->attributes();
        const point &p=x[it-nodes.begin()];
        color=default_vertex_color;
        if ((ait=attr.find(_GT_ATTRIB_COLOR))!=attr.end())
            color=ait->second.val;
        append_vertex(drawing,p,color,width);
    }
}

/* return the best quadrant for the placement of the i-th vertex label
 * (minimize the collision with the adjacent edges) */
int graphe::best_quadrant(const point &p,const layout &adj,const point &center) {
    int bestq,n=adj.size();
    if (n==0)
        return _QUADRANT1;
    vector<double> adj_phi(n);
    for (layout_iter it=adj.begin();it!=adj.end();++it) {
        adj_phi[it-adj.begin()]=std::atan2(it->at(1)-p[1],it->at(0)-p[0]);
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
        bestq=candidates.front();
    else {
        double dist=0,d;
        double phi0=std::atan2(center[1]-p[1],center[0]-p[0]);
        for (ivector::const_iterator it=candidates.begin();it!=candidates.end();++it) {
            int phi=bisector[*it];
            d=std::min(std::abs(phi-phi0),std::abs(std::abs(phi-phi0)-2*M_PI));
            if (d>dist) {
                dist=d;
                bestq=*it;
            }
        }
    }
    switch (bestq) {
    case 0: return _QUADRANT1;
    case 1: return _QUADRANT2;
    case 2: return _QUADRANT3;
    case 3: return _QUADRANT4;
    default: break;
    }
    // unreachable
    return -1;
}

/* append labels of the nodes of this graph to vecteur v */
void graphe::draw_labels(vecteur &drawing,const layout &x) const {
    if (is_empty())
        return;
    assert(!x.empty());
    point center=layout_center(x);
    ivector adjv;
    layout adj(2);
    attrib_iter ait;
    double d;
    int color,n=node_count();
    point r(x.front().size());
    bool isdir=is_directed(),isweighted=is_weighted();
    if (isweighted && x.front().size()==2) {
        // draw weight labels
        for (int i=0;i<n;++i) {
            const point &p=x[i];
            const vertex &v=node(i);
            for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
                if (!isdir && *it<i)
                    continue;
                const point &q=x[*it];
                const attrib &attr=v.neighbor_attributes(*it);
                color=default_edge_color;
                if ((ait=attr.find(_GT_ATTRIB_COLOR))!=attr.end())
                    color=ait->second.val;
                assert((ait=attr.find(_GT_ATTRIB_POSITION))!=attr.end());
                d=ait->second.DOUBLE_val();
                point_lincomb(p,q,d,1-d,r);
                adj.front()=p;
                adj.back()=q;
                assert((ait=attr.find(_GT_ATTRIB_WEIGHT))!=attr.end());
                append_label(drawing,r,ait->second,best_quadrant(r,adj,center),color);
            }
        }
    }
    // draw vertex labels
    for (int i=0;i<n;++i) {
        const point &p=x[i];
        adjacent_nodes(i,adjv,false);
        adj.resize(adjv.size());
        for (ivector_iter it=adjv.begin();it!=adjv.end();++it) {
            adj[it-adjv.begin()]=x[*it];
        }
        append_label(drawing,p,nodes[i].label(),best_quadrant(p,adj,center));
    }
}

/* extract the largest leading cycle from this graph and return true iff it exists */
bool graphe::get_leading_cycle(ivector &c) const {
    c.clear();
    int n=node_count();
    for (int i=0;i<n;++i) {
        if (i==0 || has_edge(i,i-1)) {
            c.push_back(i);
        } else break;
    }
    if (c.size()<3)
        return false;
    for (int i=c.size();i-->2;) {
        if (has_edge(c[i],c.front())) {
            c.resize(i+1);
            return true;
        }
    }
    return false;
}

/* depth-first graph traversal with O(n+m) time and O(m) space complexity */
void graphe::dfs(int root,bool rec,bool clr,ivector *D,int sg,bool skip_embedded) {
    if (clr) {
        unvisit_all_nodes(sg);
        unset_all_ancestors(sg);
        disc_time=0;
    }
    ivector &d=D!=NULL?*D:discovered_nodes;
    if (rec) {
        d.clear();
        d.reserve(node_count());
    }
    clear_node_stack();
    node_stack.push(root);
    int i,j,oldsize;
    while (!node_stack.empty()) {
        i=node_stack.top();
        node_stack.pop();
        vertex &v=node(i);
        if (!v.is_visited()) {
            v.set_disc(disc_time++);
            if (rec)
                d.push_back(i);
            v.set_visited(true);
            oldsize=node_stack.size();
            for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
                j=*it;
                if (j<0) j=-j-1;
                vertex &w=node(j);
                if ((sg>=0 && w.subgraph()!=sg) || (skip_embedded && w.is_embedded()))
                    continue;
                if (!w.is_visited()) {
                    w.set_ancestor(i);
                    node_stack.push(j);
                }
            }
            v.set_is_leaf(int(node_stack.size())==oldsize);
        }
    }
}

/* breadth-first graph traversal with O(n+m) time and O(m) space complexity */
void graphe::bfs(int root,bool rec,bool clr,ivector *D,int sg,bool skip_embedded) {
    if (clr) {
        unvisit_all_nodes(sg);
        unset_all_ancestors(sg);
        disc_time=0;
    }
    ivector &d=D!=NULL?*D:discovered_nodes;
    if (rec) {
        d.clear();
        d.reserve(node_count());
    }
    queue<int> node_queue;
    node_queue.push(root);
    int i,j;
    while (!node_queue.empty()) {
        i=node_queue.front();
        node_queue.pop();
        vertex &v=node(i);
        if (!v.is_visited()) {
            v.set_disc(disc_time++);
            if (rec)
                d.push_back(i);
            v.set_visited(true);
            for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
                j=*it;
                if (j<0) j=-j-1;
                vertex &w=node(j);
                if ((sg>=0 && w.subgraph()!=sg) || (skip_embedded && w.is_embedded()))
                    continue;
                if (!w.is_visited()) {
                    if (w.ancestor()<0)
                        w.set_ancestor(i);
                    node_queue.push(j);
                }
            }
        }
    }
}

/* return true iff the graph is connected */
bool graphe::is_connected(int sg) {
    assert(!is_empty() && !is_directed());
    node_iter it=nodes.begin();
    if (sg>=0)
        for (;it->subgraph()!=sg && it!=nodes.end();++it);
    assert(it!=nodes.end());
    dfs(it-nodes.begin(),false,true,NULL,sg);
    for (;it!=nodes.end();++it) {
        if ((sg<0 || it->subgraph()==sg) && !it->is_visited())
            return false;
    }
    return true;
}

/* return true iff the graph is biconnected */
bool graphe::is_biconnected(int sg) {
    assert(!is_empty() && !is_directed());
    return is_connected(sg) && !has_cut_vertex(sg);
}

/* return true iff the graph is triconnected, using a simple O(n*(n+m)) algorithm */
bool graphe::is_triconnected(int sg) {
    assert(!is_empty() && !is_directed());
    int color=max_subgraph_index()+1;
    for (int i=node_count();i-->0;) {
        vertex &v=node(i);
        if (sg>=0 && v.subgraph()!=sg)
            continue;
        if (degree(i)<3)
            return false;
        v.set_subgraph(color);
    }
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        if (it->subgraph()!=color)
            continue;
        it->set_subgraph(color+1);
        if (!is_biconnected(color))
            return false;
        it->set_subgraph(color);
    }
    return true;
}

/* return true iff the graph is a forest (check that the connected components are all trees) */
bool graphe::is_forest() {
    assert(!is_empty() && !is_directed());
    ivectors components;
    connected_components(components);
    graphe G(ctx);
    for (ivectors_iter it=components.begin();it!=components.end();++it) {
        induce_subgraph(*it,G,false);
        if (G.edge_count()+1!=int(it->size()))
            return false;
    }
    return true;
}

/* return true iff the graph is a tournament */
bool graphe::is_tournament() {
    int n=node_count();
    if (!is_directed() || edge_count()!=n*(n-1)/2)
        return false;
    for (int i=0;i<n;++i) {
        for (int j=0;j<n;++j) {
            if (has_edge(i,j)==has_edge(j,i))
                return false;
        }
    }
    return true;
}

/* return true iff the graph is planar */
bool graphe::is_planar() {
    graphe U(ctx);
    underlying(U);
    ivectors components,faces;
    int m;
    U.connected_components(components);
    for (ivectors_iter it=components.begin();it!=components.end();++it) {
        if (it->size()<5)
            continue;
        graphe G(ctx);
        U.induce_subgraph(*it,G,false);
        m=G.edge_count();
        if (m>3*int(it->size())-6)
            return false;
        if (m<9)
            continue;
        vector<ipairs> blocks;
        G.find_blocks(blocks);
        for (vector<ipairs>::const_iterator jt=blocks.begin();jt!=blocks.end();++jt) {
            if (jt->size()<9)
                continue;
            graphe H(ctx);
            G.subgraph(*jt,H);
            if (H.node_count()<5)
                continue;
            if (!H.demoucron(faces))
                return false;
        }
    }
    return true;
}

/* create set of vertices for product P of this graph and graph G */
void graphe::make_product_nodes(const graphe &G,graphe &P) const {
    int n=node_count(),m=G.node_count();
    stringstream ss;
    for (int i=0;i<n;++i) {
        for (int j=0;j<m;++j) {
            const gen &v=node_label(i),&w=G.node_label(j);
            ss.str("");
            if (v.type==_STRNG)
                ss << genstring2str(v);
            else ss << v;
            ss << ":";
            if (w.type==_STRNG)
                ss << genstring2str(w);
            else ss << w;
            P.add_node(str2gen(ss.str(),true));
        }
    }
}

/* compute the cartesian product of this graph and graph G and store it in P */
void graphe::cartesian_product(const graphe &G,graphe &P) const {
    P.clear();
    make_product_nodes(G,P);
    int n=node_count(),m=G.node_count();
    for (int i=0;i<n;++i) {
        for (int j=0;j<m;++j) {
            const vertex &w=G.node(j);
            for (ivector_iter it=w.neighbors().begin();it!=w.neighbors().end();++it) {
                if (*it>j)
                    P.add_edge(i*m+j,i*m+(*it));
            }
        }
    }
    for (int j=0;j<m;++j) {
        for (int i=0;i<n;++i) {
            const vertex &v=node(i);
            for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
                if (*it>i)
                    P.add_edge(i*m+j,(*it)*m+j);
            }
        }
    }
}

/* compute tensor product of this graph and graph G and store it in P */
void graphe::tensor_product(const graphe &G,graphe &P) const {
    P.clear();
    make_product_nodes(G,P);
    int n=node_count(),m=G.node_count();
    for (int i=0;i<n;++i) {
        const vertex &v=node(i);
        for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
            for (int j=0;j<m;++j) {
                const vertex &w=G.node(j);
                for (ivector_iter jt=w.neighbors().begin();jt!=w.neighbors().end();++jt) {
                    if (*jt>j)
                        P.add_edge(i*m+j,(*it)*m+(*jt));
                }
            }
        }
    }
}

/* highlight all edges from E using the specified color */
void graphe::highlight_edges(const ipairs &E,int color) {
    gen val;
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        val=change_subtype(color,_INT_COLOR);
        set_edge_attribute(it->first,it->second,_GT_ATTRIB_COLOR,val);
    }
}

/* highlight all nodes with indices in V using the specified color */
void graphe::highlight_nodes(const ivector &V,int color) {
    gen val;
    for (ivector_iter it=V.begin();it!=V.end();++it) {
        val=change_subtype(color,_INT_COLOR);
        set_node_attribute(*it,_GT_ATTRIB_COLOR,val);
    }
}

/* compute the distance between i-th and J nodes using BFS and store them to dist
 * (also output paths if shortest_paths!=NULL) */
void graphe::distance(int i,const ivector &J,ivector &dist,ivectors *shortest_paths) {
    bfs(i,false);
    int k,p,len;
    if (shortest_paths!=NULL) {
        shortest_paths->resize(J.size());
    }
    dist.resize(J.size());
    ivector *shortest_path=NULL;
    for (ivector_iter it=J.begin();it!=J.end();++it) {
        k=*it;
        len=0;
        if (shortest_paths!=NULL) {
            shortest_path=&shortest_paths->at(it-J.begin());
            shortest_path->clear();
            shortest_path->push_back(*it);
        }
        while (k!=i) {
            if ((p=node(k).ancestor())<0) {
                len=-1;
                break;
            }
            k=p;
            if (shortest_path!=NULL)
                shortest_path->push_back(k);
            ++len;
        }
        if (shortest_path!=NULL)
            std::reverse(shortest_path->begin(),shortest_path->end());
        dist[it-J.begin()]=len;
    }
}

/* compute the distances between all pairs of vertices using Floyd-Warshall algorithm */
void graphe::allpairs_distance(matrice &m) const {
    int n=node_count(),i,j,k;
    m.reserve(n);
    for (i=0;i<n;++i) {
        m.push_back(vecteur(n,plusinf()));
    }
    gen s;
    bool isweighted=is_weighted();
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        m[i]._VECTptr->at(i)=0;
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
            m[i]._VECTptr->at(j)=isweighted?weight(i,j):gen(1);
        }
    }
    for (k=0;k<n;++k) {
        for (i=0;i<n;++i) {
            const gen &mik=m[i][k];
            for (j=0;j<n;++j) {
                s=mik+m[k][j];
                gen &mij=m[i]._VECTptr->at(j);
                if (is_strictly_greater(mij,s,ctx))
                    mij=s;
            }
        }
    }
}

/* return the length of the shortest path from src to dest in weighted
 * graph (Dijkstra's algorithm), also fill shortest_path with the respective vertices */
void graphe::dijkstra(int src,const ivector &dest,vecteur &path_weights,ivectors *cheapest_paths) {
    int n=node_count();
    ivector Q(n),prev(n);
    vecteur dist(n);
    bool isweighted=is_weighted();
    for (int i=0;i<n;++i) {
        Q[i]=i;
        dist[i]=i==src?gen(0):plusinf();
        prev[i]=-1;
    }
    unvisit_all_nodes();
    gen min_dist,alt;
    int pos,u;
    while (!Q.empty()) {
        min_dist=plusinf();
        for (ivector_iter it=Q.begin();it!=Q.end();++it) {
            const gen &d=dist[*it];
            if (is_strictly_greater(min_dist,d,ctx)) {
                min_dist=d;
                pos=it-Q.begin();
                u=*it;
            }
        }
        Q.erase(Q.begin()+pos);
        vertex &v=node(u);
        v.set_visited(true);
        for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
            if (node(*it).is_visited())
                continue;
            alt=dist[u]+(isweighted?weight(u,*it):gen(1));
            if (is_strictly_greater(dist[*it],alt,ctx)) {
                dist[*it]=alt;
                prev[*it]=u;
            }
        }
    }
    if (cheapest_paths!=NULL) {
        cheapest_paths->resize(dest.size());
        for (ivector_iter it=dest.begin();it!=dest.end();++it) {
            ivector &path=cheapest_paths->at(it-dest.begin());
            path.clear();
            path.push_back(*it);
            int p=*it;
            while ((p=prev[p])>=0) path.push_back(p);
            std::reverse(path.begin(),path.end());
        }
    }
    path_weights.resize(dest.size());
    for (ivector_iter it=dest.begin();it!=dest.end();++it) {
        path_weights[it-dest.begin()]=dist[*it];
    }
}

/* return true iff the graph has a topological ordering and output the indices
 * of thus sorted vertices in list v, use Kahn's algorithm with time complexity O(n+m) */
bool graphe::topologic_sort(ivector &ordering) {
    assert(is_directed() && node_stack.empty());
    int n=node_count(),v;
    ordering.clear();
    ordering.reserve(n);
    for (int i=0;i<n;++i) {
        if (in_degree(i)==0)
            node_stack.push(i);
    }
    graphe G(*this);
    while (!node_stack.empty()) {
        v=node_stack.top();
        node_stack.pop();
        ordering.push_back(v);
        vertex &vert=G.node(v);
        for (ivector_iter it=vert.neighbors().begin();it!=vert.neighbors().end();++it) {
            if (G.in_degree(*it)==1)
                node_stack.push(*it);
        }
        vert.clear_neighbors();
    }
    return G.edge_count()==0;
}

/* return true iff the graph is an arborescence, i.e. a directed tree */
bool graphe::is_arborescence() const {
    assert(is_directed());
    bool has_root=false;
    ivector deg(node_count(),0);
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            ++deg[*jt];
        }
    }
    for (ivector_iter it=deg.begin();it!=deg.end();++it) {
        if (*it==0) {
            if (has_root)
                return false;
            has_root=true;
         } else if (*it>1)
            return false;
    }
    return has_root;
}

/* write this (directed) graph with arc directions reversed to G */
void graphe::reverse(graphe &G) const {
    assert(is_directed());
    G.set_directed(true);
    G.set_graph_attributes(attributes);
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        G.add_node(it->label(),it->attributes());
    }
    int i,j;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        const vertex &v=node(i);
        for (ivector_iter jt=v.neighbors().begin();jt!=v.neighbors().end();++jt) {
            j=*jt;
            if (j<0) j=-j-1;
            G.add_edge(j,i,v.neighbor_attributes(j));
        }
    }
}

/* write a spanning tree of this graph with i-th node as root to T, time complexity O(n+m) */
void graphe::spanning_tree(int i,graphe &T,int sg) {
    T.clear();
    vecteur V=vertices(sg);
    T.add_nodes(V);
    ivector indices(V.size());
    int v,p;
    if (sg>=0) {
        for (const_iterateur it=V.begin();it!=V.end();++it) {
            v=it-V.begin();
            indices[v]=node_index(*it);
        }
    }
    dfs(i,false,true,NULL,sg);
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (sg>=0 && it->subgraph()!=sg)
            continue;
        if ((p=it->ancestor())>=0) {
            if (sg>=0)
                T.add_edge(indices[it-nodes.begin()],indices[p]);
            else
                T.add_edge(it-nodes.begin(),p);
        }
    }
}

/* write the minimal spanning tree of this graph to T,
 * use Kruskal's algorithm with time complexity O(m*log(n)) */
void graphe::minimal_spanning_tree(graphe &T,int sg) {
    assert(!is_directed() && is_weighted());
    ipairs E,res;
    get_edges_as_pairs(E,false,sg);
    edges_comparator comp(this);
    sort(E.begin(),E.end(),comp);
    int v,u;
    disjoint_set ds(node_count());
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        ds.make_set(it->first);
        ds.make_set(it->second);
    }
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        u=it->first;
        v=it->second;
        if (ds.find(u)!=ds.find(v)) {
            res.push_back(*it);
            ds.unite(u,v);
        }
    }
    subgraph(res,T,false);
}

#ifndef NO_NAMESPACE_GIAC
}
#endif // ndef NO_NAMESPACE_GIAC
