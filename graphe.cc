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
#include <stdio.h>
#ifdef HAVE_LIBNAUTY
#include "nautywrapper.h"
#endif
using namespace std;

#ifndef NO_NAMESPACE_GIAC
namespace giac {
#endif // ndef NO_NAMESPACE_GIAC

const gen graphe::VRAI=gen(1).change_subtype(_INT_BOOLEAN);
const gen graphe::FAUX=gen(0).change_subtype(_INT_BOOLEAN);
bool graphe::verbose=true;
int graphe::default_edge_color=_BLUE;
int graphe::default_edge_width=_LINE_WIDTH_2;
int graphe::bold_edge_width=_LINE_WIDTH_4;
int graphe::default_highlighted_edge_color=_RED;
int graphe::default_highlighted_vertex_color=_GREEN;
int graphe::default_vertex_color=_YELLOW;
int graphe::default_vertex_label_color=_BLACK;
/*
 * Special graphs are initialized with adjacency lists of integers or strings.
 * Each list starts with the corresponding vertex, followed by its neighbors,
 * and ends with -1 or an empty string. The graph specification terminates with
 * -2 (for integers) or NULL (for strings).
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

void graphe::message(const char *str) const {
    if (verbose)
        *logptr(ctx) << str << endl;
}

void graphe::message(const char *format,int a) const {
    char buffer[256];
    sprintf(buffer,format,a);
    if (verbose)
        *logptr(ctx) << buffer << endl;
}

void graphe::message(const char *format,int a,int b) const {
    char buffer[256];
    sprintf(buffer,format,a,b);
    if (verbose)
        *logptr(ctx) << buffer << endl;
}

void graphe::message(const char *format,int a,int b,int c) const {
    char buffer[256];
    sprintf(buffer,format,a,b,c);
    if (verbose)
        *logptr(ctx) << buffer << endl;
}

string graphe::giac_version() const {
    return genstring2str(_version(change_subtype(vecteur(0),_SEQ__VECT),ctx));
}

gen graphe::plusinf() {
    return symbolic(at_plus,_IDNT_infinity());
}

/* convert list of lists of integers into vecteur of vecteurs */
void graphe::ivectors2vecteur(const ivectors &v,vecteur &res,bool sort_all) const {
    res.resize(v.size());
    for (ivectors_iter it=v.begin();it!=v.end();++it) {
        res[it-v.begin()]=sort_all?_sort(get_node_labels(*it),ctx):get_node_labels(*it);
    }
}

/* vertex class implementation */
void graphe::vertex::assign_defaults() {
    m_sorted=true;
    m_subgraph=-1;
    m_visited=false;
    m_on_stack=false;
    m_ancestor=-1;
    m_low=-1;
    m_disc=-1;
    m_color=0; // white
    m_prelim=0;
    m_modifier=0;
    m_children=0;
    m_position=0;
    m_gaps=0;
    m_left=-1;
    m_right=-1;
    m_x_offset=0;
    m_y=0;
    m_embedded=false;
    m_number=-1;
}

graphe::vertex::vertex() {
    assign_defaults();
}

graphe::vertex::vertex(const gen &lab) {
    assign_defaults();
    set_label(lab);
}

void graphe::vertex::assign(const vertex &other) {
    m_sorted=other.is_sorted();
    m_subgraph=other.subgraph();
    m_visited=other.is_visited();
    m_on_stack=other.is_on_stack();
    m_ancestor=other.ancestor();
    m_low=other.low();
    m_disc=other.disc();
    m_color=other.color();
    m_prelim=other.prelim();
    m_modifier=other.modifier();
    m_children=other.children();
    m_position=other.position();
    m_gaps=other.gaps();
    m_left=other.left();
    m_right=other.right();
    m_x_offset=other.x_offset();
    m_y=other.y();
    m_embedded=other.is_embedded();
    m_number=other.number();
    m_edge_faces=other.edge_faces();
    set_attributes(other.attributes());
    m_neighbors.resize(other.neighbors().size());
    m_neighbor_attributes.clear();
    for (ivector_iter it=other.neighbors().begin();it!=other.neighbors().end();++it) {
        m_neighbors[it-other.neighbors().begin()]=*it;
        copy_attributes(other.neighbor_attributes(*it),m_neighbor_attributes[*it]);
    }
}

gen graphe::vertex::label() const {
    attrib_iter it=m_attributes.find(_GT_ATTRIB_LABEL);
    if (it==m_attributes.end())
        return undef;
    return it->second;
}

graphe::vertex::vertex(const vertex &other) {
    assign(other);
}

graphe::vertex& graphe::vertex::operator =(const vertex &other) {
    assign(other);
    return *this;
}

void graphe::vertex::add_neighbor(int i,const attrib &attr) {
    ivector::iterator it=m_neighbors.begin();
    while (it!=m_neighbors.end() && *it<i) ++it;
    assert(it==m_neighbors.end() || *it!=i);
    m_neighbors.insert(it,i);
    copy_attributes(attr,m_neighbor_attributes[i]);
}

bool graphe::vertex::is_temporary(int i) const {
    const attrib &attr=neighbor_attributes(i);
    attrib_iter it=attr.find(_GT_ATTRIB_TEMPORARY);
    return it!=attr.end() && is_one(it->second);
}

graphe::attrib &graphe::vertex::neighbor_attributes(int i) {
    map<int,attrib>::iterator it=m_neighbor_attributes.find(i);
    assert(it!=m_neighbor_attributes.end());
    return it->second;
}

const graphe::attrib &graphe::vertex::neighbor_attributes(int i) const {
    neighbor_iter it=m_neighbor_attributes.find(i);
    assert(it!=m_neighbor_attributes.end());
    return it->second;
}

bool graphe::vertex::has_neighbor(int i,bool include_temp_edges) const {
    bool yes;
    if (m_sorted)
        yes=binary_search(m_neighbors.begin(),m_neighbors.end(),i);
    else yes=find(m_neighbors.begin(),m_neighbors.end(),i)!=m_neighbors.end();
    if (!yes)
        return false;
    attrib_iter ait;
    const attrib &attr=neighbor_attributes(i);
    return include_temp_edges || (ait=attr.find(_GT_ATTRIB_TEMPORARY))==attr.end() || !is_one(ait->second);
}

void graphe::vertex::remove_neighbor(int i) {
    ivector::iterator it=find(m_neighbors.begin(),m_neighbors.end(),i);
    if (it!=m_neighbors.end()) {
        m_neighbors.erase(it);
        map<int,attrib>::iterator jt=m_neighbor_attributes.find(i);
        assert(jt!=m_neighbor_attributes.end());
        m_neighbor_attributes.erase(jt);
    }
    if (m_neighbors.empty())
        m_sorted=true;
}

void graphe::vertex::move_neighbor(int i,int j,bool after) {
    ivector::iterator it=find(m_neighbors.begin(),m_neighbors.end(),i);
    assert(it!=m_neighbors.end());
    m_neighbors.erase(it);
    it=j<0?m_neighbors.begin():find(m_neighbors.begin(),m_neighbors.end(),j);
    assert(it!=m_neighbors.end());
    if (after && j>=0)
        ++it;
    m_neighbors.insert(it,i);
    m_sorted=false;
}

void graphe::vertex::incident_faces(ivector &F) const {
    F.resize(m_edge_faces.size());
    int i=0,f;
    for (map<int,int>::const_iterator it=m_edge_faces.begin();it!=m_edge_faces.end();++it) {
        assert((f=it->second)>0);
        F[i++]=f-1;
    }
}

/* set the given planar embedding */
void graphe::set_embedding(const ivectors &faces) {
    int n,f;
    for (ivectors_iter it=faces.begin();it!=faces.end();++it) {
        f=it-faces.begin();
        const ivector &face=*it;
        n=face.size();
        for (int i=0;i<n;++i) {
            vertex &v=node(face[i]);
            v.add_edge_face(face[(i+1)%n],f);
        }
    }
}

/* clear the previously set embedding */
void graphe::clear_embedding() {
    for (std::vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->clear_edge_faces();
    }
}

/* get the first neighbor of v in the subgraph sg */
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

/* clear the node queue */
void graphe::clear_node_queue() {
    while (!node_queue.empty())
        node_queue.pop();
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

bool graphe::rectangle::intersects(const rectangle &other) const {
    return x()<other.x()+other.width() &&
            x()+width()>other.x() &&
            y()<other.y()+other.height() &&
            y()+height()>other.y();
}

bool graphe::rectangle::intersects(const vector<rectangle> &rectangles) const {
    vector<rectangle>::const_iterator it=rectangles.begin();
    for (;it!=rectangles.end();++it) {
        if (intersects(*it))
            return true;
    }
    return false;
}

bool graphe::rectangle::intersects(vector<rectangle>::const_iterator first,vector<rectangle>::const_iterator last) const {
    vector<rectangle>::const_iterator it=first;
    for (;it!=last;++it) {
        if (intersects(*it))
            return true;
    }
    return false;
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

/* return random permutation of {0,1,..,n-1} */
graphe::ivector graphe::rand_permu(int n) {
    return vecteur_2_vector_int(*_randperm(n,context0)._VECTptr);
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
    //nodes.reserve(1024);
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
    ivector hull;
    layout x;
    if (name=="clebsch") {
        reserve_nodes(16);
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
        read_special(coxeter_graph);
        stringstream ss;
        for (int i=1;i<=7;++i) {
            ss.str("");
            ss << "a" << i;
            hull.push_back(node_index(str2gen(ss.str(),true)));
        }
        make_circular_layout(x,hull,3.5);
    } else if (name=="desargues") {
        make_petersen_graph(10,3,&x);
    } else if (name=="dodecahedron") {
        read_special(dodecahedron_graph);
        for (int i=1;i<=5;++i) hull.push_back(node_index(i));
        make_circular_layout(x,hull,2.5);
    } else if (name=="durer") {
        make_petersen_graph(6,2,&x);
    } else if (name=="dyck") {
        read_special(dyck_graph);
        for (int i=0;i<node_count();++i) hull.push_back(i);
        make_circular_layout(x,hull);
    } else if (name=="grinberg") {
        read_special(grinberg_graph);
        make_planar_layout(x);
        layout_best_rotation(x);
    } else if (name=="grotzsch") {
        read_special(grotzsch_graph);
        for (int i=1;i<=5;++i) hull.push_back(node_index(i));
        make_circular_layout(x,hull,2.5);
    } else if (name=="harries") {
        make_lcf_graph(harries_graph_lcf,5);
        for (int i=0;i<node_count();++i) hull.push_back(i);
        make_circular_layout(x,hull);
    } else if (name=="harries-wong") {
        make_lcf_graph(harries_wong_graph_lcf,1);
        for (int i=0;i<node_count();++i) hull.push_back(i);
        make_circular_layout(x,hull);
    } else if (name=="heawood") {
        read_special(heawood_graph);
    } else if (name=="herschel") {
        read_special(herschel_graph);
        make_planar_layout(x);
        layout_best_rotation(x);
    } else if (name=="icosahedron") {
        read_special(icosahedron_graph);
        make_planar_layout(x);
        layout_best_rotation(x);
    } else if (name=="levi") {
        read_special(levi_graph);
        for (int i=0;i<node_count();++i) hull.push_back(i);
        make_circular_layout(x,hull);
    } else if (name=="ljubljana") {
        make_lcf_graph(ljubljana_graph_lcf,2);
    } else if (name=="mcgee") {
        read_special(mcgee_graph);
        for (int i=0;i<node_count();++i) hull.push_back(i);
        make_circular_layout(x,hull);
    } else if (name=="mobius-kantor") {
        make_petersen_graph(8,3,&x);
    } else if (name=="nauru") {
        make_petersen_graph(12,5,&x);
    } else if (name=="octahedron") {
        read_special(octahedron_graph);
        make_planar_layout(x);
        layout_best_rotation(x);
    } else if (name=="pappus") {
        read_special(pappus_graph);
        for (int i=0;i<node_count();++i) hull.push_back(i);
        make_circular_layout(x,hull);
    } else if (name=="petersen") {
        make_petersen_graph(5,2,&x);
    } else if (name=="robertson") {
        read_special(robertson_graph);
        for (int i=0;i<node_count();++i) hull.push_back(i);
        make_circular_layout(x,hull);
    } else if (name=="soccerball") {
        read_special(soccer_ball_graph);
        for (int i=16;i<=20;++i) hull.push_back(node_index(i));
        make_circular_layout(x,hull,2.5);
    } else if (name=="shrikhande") {
        make_shrikhande_graph();
        for (int i=0;i<4;++i) {
            add_temporary_edge(i,i+8);
            hull.push_back(i);
        }
        add_temporary_edge(8,10);
        add_temporary_edge(9,11);
        add_temporary_edge(4,6);
        add_temporary_edge(5,7);
        make_circular_layout(x,hull);
        remove_temporary_edges();
    } else if (name=="tetrahedron") {
        read_special(tetrahedron_graph);
        make_planar_layout(x);
        layout_best_rotation(x);
    }
    if (!x.empty()) {
        double sep=1.0;
        scale_layout(x,sep*std::sqrt((double)node_count()));
        rectangle rect=layout_bounding_rect(x,sep/PLASTIC_NUMBER_CUBED);
        translate_layout(x,make_point(-rect.x(),-rect.y()));
        store_layout(x);
    }
}

/* export this graph as a Giac gen object */
gen graphe::to_gen() {
    int n=node_count();
    vecteur res(3+n+edge_count()*(is_directed()?1:2));
    int cnt=0;
    gen_map attr;
    attrib2genmap(attributes,attr);
    res[cnt++]=n;
    res[cnt++]=attr;
    vecteur uattr_ids(user_tags.size());
    for (vector<string>::const_iterator it=user_tags.begin();it!=user_tags.end();++it) {
        uattr_ids[it-user_tags.begin()]=str2gen(*it,true);
    }
    res[cnt++]=uattr_ids;
    for (int i=0;i<n;++i) {
        gen_map vattr;
        const vertex &v=node(i);
        attrib2genmap(v.attributes(),vattr);
        vattr[-1]=v.neighbors().size();
        res[cnt++]=vattr;
    }
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        const vertex &v=*it;
        for (ivector_iter jt=v.neighbors().begin();jt!=v.neighbors().end();++jt) {
            gen_map nattr;
            attrib2genmap(v.neighbor_attributes(*jt),nattr);
            nattr[-1]=*jt;
            res[cnt++]=nattr;
        }
    }
    return change_subtype(res,_GRAPH__VECT);
}

/* allocate, initialize and return an integer array of adjacency lists of this graph,
 * in form [a11,a12,...,-1,a21,a22,...,-1,...] */
int *graphe::to_array() const {
    int sz=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        sz+=it->neighbors().size();
    }
    int *res=new int[sz+node_count()];
    int i=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            res[i++]=*jt;
        }
        res[i++]=-1;
    }
    return res;
}

/* return index associated with the specified attribute tag */
int graphe::tag2index(const string &tag) {
    if (tag=="label")
        return _GT_ATTRIB_LABEL;
    if (tag=="directed")
        return _GT_ATTRIB_DIRECTED;
    if (tag=="weighted")
        return _GT_ATTRIB_WEIGHTED;
    if (tag=="weight")
        return _GT_ATTRIB_WEIGHT;
    if (tag=="color")
        return _GT_ATTRIB_COLOR;
    if (tag=="style")
        return _GT_ATTRIB_STYLE;
    if (tag=="shape")
        return _GT_ATTRIB_SHAPE;
    if (tag=="pos")
        return _GT_ATTRIB_POSITION;
    if (tag=="name")
        return _GT_ATTRIB_NAME;
    if (tag=="temp")
        return _GT_ATTRIB_TEMPORARY;
    return register_user_tag(tag);
}

/* return attribute tag associated with the specified index */
string graphe::index2tag(int index) const {
    int len;
    switch (index) {
    case _GT_ATTRIB_LABEL:
        return "label";
    case _GT_ATTRIB_DIRECTED:
        return "directed";
    case _GT_ATTRIB_WEIGHTED:
        return "weighted";
    case _GT_ATTRIB_COLOR:
        return "color";
    case _GT_ATTRIB_STYLE:
        return "style";
    case _GT_ATTRIB_SHAPE:
        return "shape";
    case _GT_ATTRIB_WEIGHT:
        return "weight";
    case _GT_ATTRIB_POSITION:
        return "pos";
    case _GT_ATTRIB_NAME:
        return "name";
    case _GT_ATTRIB_TEMPORARY:
        return "temp";
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
    vector<string>::const_iterator it=user_tags.begin();
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

/* set edge {i,j} visited */
void graphe::set_edge_visited(int i,int j) {
    if (visited_edges.empty())
        visited_edges.resize(node_count());
    ivector &ngh=visited_edges[i<j?i:j];
    int k=i<j?j:i;
    if (ngh.empty())
        ngh.push_back(k);
    else {
        ivector::iterator it=ngh.begin();
        while (it!=ngh.end() && *it<k) ++it;
        ngh.insert(it,k);
    }
}

/* return true iff edge {i,j} is visited */
bool graphe::is_edge_visited(int i,int j) const {
    if (visited_edges.empty())
        return false;
    const ivector &ngh=visited_edges[i<j?i:j];
    return binary_search(ngh.begin(),ngh.end(),i<j?j:i);
}

/* move neighbor w of v right before (or after) ref */
void graphe::move_neighbor(int v,int w,int ref,bool after) {
    node(v).move_neighbor(w,ref,after);
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
    int count=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->has_neighbor(index,count_temp_edges))
            count++;
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
        if (count_temp_edges || !is_temporary_edge(index,*it))
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

/* return the degree sequence of this graph */
vecteur graphe::degree_sequence(int sg) const {
    vecteur res;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (sg>=0 && it->subgraph()!=sg)
            continue;
        res.push_back(degree(it-nodes.begin()));
    }
    return res;
}

/* create the adjacency matrix of this graph */
void graphe::adjacency_matrix(matrice &m) const {
    int n=node_count(),i,j;
    m=*_matrix(makesequence(n,n),context0)._VECTptr;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
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
            sm[i][j]=1;
        }
    }
}

/* return incidence matrix of this graph */
matrice graphe::incidence_matrix() const {
    ipairs E;
    get_edges_as_pairs(E);
    int nr=node_count(),nc=E.size();
    matrice m=*_matrix(makesequence(nr,nc,0),context0)._VECTptr;
    bool isdir=is_directed();
    for (int i=0;i<nr;++i) {
        for (int j=0;j<nc;++j) {
            gen &mij=m[i]._VECTptr->at(j);
            const ipair &e=E[j];
            if (isdir) {
                if (i==e.first)
                    mij=-1;
                else if (i==e.second)
                    mij=1;
            } else if (i==e.first || i==e.second)
                mij=1;
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

/* turn the color of each vertex to white */
void graphe::uncolor_all_nodes(int base_color,int sg) {
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        if (sg<0 || it->subgraph()==sg)
            it->set_color(base_color);
    }
}

/* fill the list E with edges (in the given subgraph) represented as pairs of vertex indices */
void graphe::get_edges_as_pairs(ipairs &E,bool include_temp_edges,int sg) const {
    int i,j;
    bool isdir=is_directed();
    E.clear();
    E.reserve(edge_count());
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (sg>=0 && it->subgraph()!=sg)
            continue;
        i=it-nodes.begin();
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
            if ((include_temp_edges || !is_temporary_edge(i,j)) && (isdir || j>i) && (sg<0 || node(j).subgraph()==sg))
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

/* export the drawing of this graph in latex format */
bool graphe::write_latex(const string &filename,const gen &drawing) const {
    ofstream texfile;
    texfile.open(filename.c_str());
    if (!texfile.is_open())
        return false;
    show_axes(0,ctx);
    string picture=genstring2str(_latex(drawing,ctx));
    texfile << "% this file was generated by " << giac_version() << endl;
    texfile << "\\documentclass{standalone}" << endl << "\\usepackage[T1]{fontenc}" << endl
            << "\\usepackage[utf8]{inputenc}" << endl << "\\usepackage{pstricks}" << endl
            << "\\begin{document}" << endl << picture << "\\end{document}" << endl;
    texfile.close();
    return true;
}

/* export this graph to dot file */
bool graphe::write_dot(const string &filename) const {
    ofstream dotfile;
    dotfile.open(filename.c_str());
    if (!dotfile.is_open())
        return false;
    dotfile << "# this file was generated by " << giac_version() << endl;
    ivector u,v;
    string indent("  "),edgeop(is_directed()?" -> ":" -- ");
    dotfile << (is_directed()?"digraph ":"graph ");
    string graph_name=name();
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
    map<int,attrib> delayed_attributes;
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
                        set_name(token);
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
            index=add_node(str2gen(token,dot_token_type!=_GT_DOT_TOKEN_TYPE_NUMBER));
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
                    delayed_attributes[ch.front()-1]=subgraphs.back().chain_attributes();
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
            message((string("Error: unknown token '")+token+"'").c_str());
            break;
        }
        if (error_raised || strict)
            break;
    }
    dotfile.close();
    if (dot_subgraph_level!=0 || strict || dot_reading_value || dot_reading_attributes)
        error_raised=true;
    else {
        /* set delayed attributes to nodes */
        for (map<int,attrib>::const_iterator it=delayed_attributes.begin();it!=delayed_attributes.end();++it) {
            vertex &v=nodes[it->first];
            for (attrib_iter ait=it->second.begin();ait!=it->second.end();++ait) {
                v.set_attribute(ait->first,ait->second);
            }
        }
    }
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
    int i;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        G.node(i).set_subgraph(it->subgraph());
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            G.add_edge(i,*jt);
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

bool gmap_find(const gen_map &gmap,const gen &key,gen &val) {
    gen_map::const_iterator it=gmap.find(key);
    if (it==gmap.end())
        return false;
    val=it->second;
    return true;
}

/* initialize graph from Giac gen object */
bool graphe::read_gen(const gen &g) {
    if (g.type!=_VECT || g.subtype!=_GRAPH__VECT)
        return false;
    this->clear();
    int n;
    const vecteur &gv=*g._VECTptr;
    if (gv.empty() || !gv.front().is_integer() ||
            (n=gv.front().val)<0 || int(gv.size())<3+n || gv[1].type!=_MAP || gv[2].type!=_VECT)
        return false;
    if (!genmap2attrib(*gv[1]._MAPptr,this->attributes))
        return false;
    const vecteur &tags=*gv[2]._VECTptr;
    user_tags.resize(tags.size());
    for (const_iterateur it=tags.begin();it!=tags.end();++it) {
        if (it->type!=_STRNG)
            return false;
        register_user_tag(genstring2str(*it));
    }
    gen val;
    attrib attr;
    int deg,start=n+3,k;
    for (int i=0;i<n;++i) {
        const gen &elm=gv[i+3];
        if (elm.type!=_MAP)
            return false;
        gen_map &mp=*elm._MAPptr;
        if (!gmap_find(mp,-1,val) || !val.is_integer() || (deg=val.val)<0 || !genmap2attrib(mp,attr))
            return false;
        attr.erase(attr.find(-1));
        vertex vert;
        vert.set_attributes(attr);
        if (int(gv.size())<start+deg)
            return false;
        for (int j=0;j<deg;++j) {
            const gen &ngh=gv[start+j];
            if (ngh.type!=_MAP)
                return false;
            gen_map &nmap=*ngh._MAPptr;
            attrib nattr;
            if (!gmap_find(nmap,-1,val) || !val.is_integer() || (k=val.val)<0 || k>=n || !genmap2attrib(nmap,nattr))
                return false;
            nattr.erase(nattr.find(-1));
            vert.add_neighbor(k,nattr);
        }
        start+=deg;
        nodes.push_back(vert);
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
    G.set_name(name());
    G.register_user_tags(user_tags);
    G.set_graph_attributes(attributes);
    G.copy_nodes(nodes);
    G.copy_marked_nodes(get_marked_nodes());
}

/* returns true iff graph has edge {i,j} */
bool graphe::has_edge(int i,int j,int sg) const {
    if (i<0 || i>=node_count() || j<0 || j>=node_count())
        return false;
    if (sg>=0 && (node(i).subgraph()!=sg || node(j).subgraph()!=sg))
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

/* add edge {v,w} or arc [v,w], adding vertices v and/or w if necessary */
graphe::ipair graphe::add_edge(const gen &v,const gen &w,const attrib &attr) {
    int i=add_node(v),j=add_node(w);
    add_edge(i,j,attr);
    if (is_directed())
        return make_pair(i,j);
    return make_pair(i<j?i:j,i<j?j:i);
}

/* add temporary edge from i-th to j-th vertex */
void graphe::add_temporary_edge(int i,int j) {
    assert(!has_edge(i,j));
    add_edge(i,j);
    set_edge_attribute(i,j,_GT_ATTRIB_TEMPORARY,boole(true));
}

/* return true iff the edge {i,j} is temporary */
bool graphe::is_temporary_edge(int i,int j) const {
    assert(has_edge(i,j));
    gen val;
    get_edge_attribute(i,j,_GT_ATTRIB_TEMPORARY,val);
    return is_one(val);
}

/* remove all temporary edges */
void graphe::remove_temporary_edges() {
    stack<ipair> edges;
    int i;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            i=it-nodes.begin();
            if (is_temporary_edge(i,*jt))
                edges.push(make_pair(i,*jt));
        }
    }
    while (!edges.empty()) {
        remove_edge(edges.top());
        edges.pop();
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
    nodes.push_back(vertex(v));
    return node_count()-1;
}

/* add vertices from list v to graph */
void graphe::add_nodes(const vecteur &v) {
    nodes.clear();
    nodes.reserve(v.size());
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
vecteur graphe::get_node_labels(const ivector &v) const {
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

/* return index of edge (i,j) as returned by get_edges_as_pairs */
int graphe::edge_index(const ipair &e) const {
    int i,j,cnt=0;
    bool isdir=is_directed();
    ipair edge=e;
    if (!isdir && e.first>e.second) {
        int tmp=edge.first;
        edge.first=edge.second;
        edge.second=tmp;
    }
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
            if (isdir || j>i) {
                if (i==edge.first && j==edge.second)
                    return cnt;
                ++cnt;
            }
        }
    }
    return -1;
}

/* set 'subgraph' field of all nodes in v to s */
void graphe::set_subgraph(const ivector &v,int s) {
    for (ivector_iter it=v.begin();it!=v.end();++it) {
        node(*it).set_subgraph(s);
    }
}

/* collect all vertices within the given subgraph in list v */
void graphe::get_subgraph(int sg,ivector &v) const {
    v.clear();
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->subgraph()==sg)
            v.push_back(it-nodes.begin());
    }
}

/* return the index of the first vertex with subgraph field equal to sg,
 * or -1 if no such vertices exist */
int graphe::first_vertex_from_subgraph(int sg) const {
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->subgraph()==sg)
            return it-nodes.begin();
    }
    return -1;
}

/* return the size of subgraph sg */
int graphe::subgraph_size(int sg) const {
    int cnt=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->subgraph()==sg)
            ++cnt;
    }
    return cnt;
}

/* merge subgraphs s and t, such that the resulting subgraph has index s */
void graphe::merge_subgraphs(int s,int t) {
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        if (it->subgraph()==t)
            it->set_subgraph(s);
    }
}

/* set subgraph field to default_sg (by default -1) for every vertex in the graph */
void graphe::unset_subgraphs(int default_sg) {
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->set_subgraph(default_sg);
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
    G.reserve_nodes(vi.size());
    if (copy_attrib) {
        G.set_directed(is_directed());
        G.set_weighted(is_weighted());
    }
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
    if (copy_attrib) {
        G.set_directed(is_directed());
        G.set_weighted(is_weighted());
    }
    set<int> nds;
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        nds.insert(it->first);
        nds.insert(it->second);
    }
    G.reserve_nodes(nds.size());
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
        if (include_temp_edges || !is_temporary_edge(i,j))
            adj.push_back(j);
    }
    if (is_directed()) {
        for (node_iter it=nodes.begin();it!=nodes.end();++it) {
            j=it-nodes.begin();
            if (i==j)
                continue;
            const ivector &ngh=it->neighbors();
            if (find(ngh.begin(),ngh.end(),i)!=ngh.end() &&
                    (include_temp_edges || !is_temporary_edge(i,j)))
                adj.push_back(j);
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

graphe::edmonds::edmonds(graphe *gr) {
    G=gr;
}

/* return the vertex which matches v in matching, return -1 if v is not matched */
int graphe::edmonds::mate(const ipairs &matching, int v) {
    for (ipairs_iter it=matching.begin();it!=matching.end();++it) {
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

graphe::ivector graphe::edmonds::adjacent(int v) {
    map<int,ivector>::const_iterator bit=is_blossom_base(v);
    ivector res;
    if (bit!=blossoms.end()) {
        /* v is the base of a blossom */
        append_non_blossom_adjacents(v,bit,res);
        for (ivector_iter it=bit->second.begin();it!=bit->second.end();++it) {
            append_non_blossom_adjacents(*it,bit,res);
        }
    } else {
        /* v does not belong to any of blossoms */
        G->adjacent_nodes(v,res);
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
    /* collect exposed (free) vertices and create a forest of singleton trees */
    forest.clear();
    path.clear();
    for (int i=0;i<G->node_count();++i) {
        if (mate(matching,i)==-1)
            forest.insert(make_pair(i,-1));
    }
    /* iterate over unmarked vertices v in F with even root distance */
    while (true) {
        map<int,int>::const_iterator fit=forest.begin();
        for (;fit!=forest.end();++fit) {
            if (!node_marked[fit->first] && in_blossom(fit->first)==blossoms.end() && root_distance(fit)%2==0)
                break;
        }
        if (fit==forest.end())
            break;
        int v=fit->first,rv=find_root(v);
        /* iterate over unmarked edges {v,w} (edges in the matching are marked by default) */
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
                /* update forest */
                int x=mate(matching,w);
                assert(x>=0);
                forest[w]=v;
                forest[x]=w;
                edge_marked[w<x?w:x][w<x?x:w]=true; // mark edge {w,x}
            } else if (root_distance(w)%2!=0) {
                /* do nothing */
                ;
            } else if (rv!=find_root(w)) {
                /* augmenting path found */
                assert(tree_path(v,w,path));
                return true;
            } else {
                /* a blossom is found */
                int b=find_base(v,w);
                assert(b>=0);
                /* store blossom vertices */
                map<int,ivector>::iterator bit=blossoms.insert(make_pair(b,ivector())).first;
                int k=w;
                while (k!=b) {
                    bit->second.push_back(k);
                    assert((k=forest[k])>=0);
                }
                std::reverse(bit->second.begin(),bit->second.end());
                /* find augmenting path with blossom contracted */
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
                    /* unfold the blossom */
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

void graphe::edmonds::find_maximum_matching(ipairs &matching) {
    ivector path;
    while (find_augmenting_path(matching,path)) {
        /* augmenting path was found, extend the matching */
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

/* extend given matching to a maximum matching using Edmonds'
 * blossom algorithm with time complexity O(m*n^2) */
void graphe::maximize_matching(ipairs &matching) {
    edmonds maximizer(this);
    maximizer.find_maximum_matching(matching);
}

/* find a maximal matching in an undirected graph (fast algorithm) */
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

int graphe::pred(int i,int n) {
    if (i>0)
        return i-1;
    return n-1;
}

int graphe::succ(int i,int n) {
    if (i<n-1)
        return i+1;
    return 0;
}

void graphe::arc_path(int i,int j,const ivector &cycle,ivector &path) {
    int n=cycle.size(),k=i,m=j-i,l=0;
    if (m<0)
        m+=n;
    path.resize(m+1);
    path.front()=cycle[i];
    while (k++!=j) {
        if (k==n)
            k=0;
        path[++l]=cycle[k];
    }
}

/* find the main chords of the given face */
void graphe::find_chords(const ivector &face,ipairs &chords) {
    int n=face.size(),h=n%2==0?n/2:(n-1)/2;
    ivector D;
    for (int k=0;k<n;++k) {
        if (degree(face[k])>2)
            D.push_back(k);
    }
    if (D.empty())
        return;
    chords.clear();
    int m=D.size(),k=0,i,j,k_next,l,v,w,r,f;
    map<int,bool> face_switch;
    ivector ifaces;
    while (true) {
        i=D[k];
        v=face[i];
        l=k;
        k_next=succ(k,m);
        ipair c=make_pair(i,-1);
        face_switch.clear();
        node(v).incident_faces(ifaces);
        for (ivector_iter it=ifaces.begin();it!=ifaces.end();++it) {
            face_switch[*it]=true;
        }
        while (true) {
            l=succ(l,m);
            j=D[l];
            r=j-i;
            if (r<0)
                r+=n;
            if (r==0 || r>h || (n%2==0 && r==h && i>=h))
                break;
            w=face[j];
            node(w).incident_faces(ifaces);
            f=-1;
            for (ivector_iter it=ifaces.begin();it!=ifaces.end();++it) {
                if (face_switch[*it])
                    face_switch[f=*it]=false;
            }
            if (r>1 && (f>=0 || has_edge(v,w))) {
                c.second=j;
                k_next=l;
            }
        }
        if (c.second>=0)
            chords.push_back(c);
        if (k_next<k)
            break;
        k=k_next;
    }
    if (chords.size()>1) {
        int end=chords.back().second;
        if (end<chords.back().first) {
            ipairs::iterator it=chords.begin();
            while (it->first<end) ++it;
            chords.erase(chords.begin(),it);
        }
    }
}

/* fold the face along its main chords */
void graphe::fold_face(const ivector &face,bool subdivide,int &label) {
    ipairs chords;
    find_chords(face,chords);
    int n=face.size(),k=chords.size(),i,j,p,q,r,s,t,u;
    if (k==0)
        return;
    if (subdivide) {
        vector<bool> visited(n,false);
        i=add_node(++label);
        for (ipairs_iter it=chords.begin();it!=chords.end();++it) {
            p=it->first;
            q=it->second;
            visited[p]=visited[q]=true;
            for (j=(p+1)%n;j!=q;j=(j+1)%n) {
                add_edge(i,face[j]);
                visited[j]=true;
            }
        }
        for (j=0;j<n;++j) {
            if (!visited[j])
                add_edge(i,face[j]);
        }
    } else if (k==1) {
        i=chords.front().first;
        q=succ(i,n);
        t=pred(i,n);
        add_temporary_edge(face[q],face[t]);
        if (n>4) {
            j=chords.front().second;
            r=pred(j,n);
            u=succ(j,n);
            add_temporary_edge(face[r],face[u]);
        }
    } else if (k>1) {
        q=succ(chords.front().first,n);
        r=succ(chords[1].first,n);
        if (k==2 && q==pred(chords.front().second,n) && r==pred(chords[1].second,n)) {
            add_temporary_edge(face[q],face[r]);
        } else {
            ivector P(0);
            ivector Q;
            for (s=0;s<k;++s) {
                i=s<k-1?chords[s+1].first:chords.front().first;
                j=chords[s].second;
                q=pred(j,n);
                r=succ(i,n);
                add_temporary_edge(face[q],face[r]);
                arc_path(succ(chords[s].first,n),q,face,Q);
                P.insert(P.end(),Q.begin(),Q.end());
            }
            if (P.size()>3)
                fold_face(P,subdivide,label);
        }
    }
}

/* augment the biconnected planar graph for spring drawing */
void graphe::augment(const ivectors &faces,int outer_face,bool subdivide) {
    set_embedding(faces);
    int label=largest_integer_label();
    for (ivectors_iter it=faces.begin();it!=faces.end();++it) {
        if (it->size()<4 || outer_face==int(it-faces.begin()))
            continue;
        fold_face(*it,subdivide,label);
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

/* compute the point dest which is a reflection of src wrt. the line ax+by+c=0 */
void graphe::point_mirror(double a,double b,double c,const point &src,point &dest) {
    double p=src[0],q=src[1],a2=a*a,b2=b*b,r=a2+b2,s=a2-b2;
    dest.resize(2);
    dest[0]=(p*s-2*b*(a*q+c))/r;
    dest[1]=-(q*s+2*a*(b*p+c))/r;
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
double graphe::layout_diameter(const layout &x) {
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

/* randomize layout x using a Gaussian random generator */
void graphe::create_random_layout(layout &x,int dim) {
    for (layout::iterator it=x.begin();it!=x.end();++it) {
        it->resize(dim);
        it->at(0)=rand_uniform();
        it->at(1)=rand_uniform();
        if (dim==3)
            it->at(2)=rand_uniform();
    }
}

/* lay out the graph using a force-directed algorithm with spring-electrical model */
void graphe::force_directed_placement(layout &x,double K,double R,double tol,bool ac) {
    double step_length=K,shrinking_factor=0.9,eps=K*tol,C=0.01,D=C*K*K;
    double energy=DBL_MAX,energy0,norm,max_displacement;
    int progress=0,n=x.size(),i,j;
    if (n==0)
        return;
    assert (n==node_count() && n>0);
    int d=x.front().size();
    point force(d),p(d),f(d);
    /* keep updating the positions until the system freezes */
    do {
        energy0=energy;
        energy=0;
        max_displacement=0;
        for (node_iter nt=nodes.begin();nt!=nodes.end();++nt) {
            i=nt-nodes.begin();
            point &xi=x[i];
            clear_point_coords(force);
            /* compute the attractive forces between vertices adjacent to the i-th vertex */
            for (ivector_iter it=nt->neighbors().begin();it!=nt->neighbors().end();++it) {
                j=*it;
                scale_point(p,point_distance(xi,x[j],p)/K);
                add_point(force,p);
            }
            /* compute the repulsive forces for all vertices j!=i which are not too far from the i-th vertex */
            for (layout_iter it=x.begin();it!=x.end();++it) {
                j=it-x.begin();
                if (i!=j) {
                    norm=point_distance(*it,xi,f);
                    if (norm>R)
                        continue;
                    if (norm==0)
                        rand_point(f,norm=shrinking_factor*eps);
                    scale_point(f,D/(norm*norm));
                    add_point(force,f);
                }
            }
            /* move the location of the i-th vertex in the direction of the force f */
            norm=point_displacement(force);
            if (norm==0)
                continue;
            if (step_length<norm) {
                scale_point(force,step_length/norm);
                norm=step_length;
            }
            add_point(xi,force);
            /* update the maximal displacement for this iteration */
            if (norm>max_displacement)
                max_displacement=norm;
            energy+=norm*norm;
        }
        /* update step length */
        if (ac) {
            /* adaptive cooling scheme */
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
        } else step_length*=shrinking_factor; /* simple cooling scheme */
    } while (max_displacement>eps);
}

/* compute optimal positions of edge labels and store them as "position" attributes of the respective edges */
void graphe::edge_labels_placement(const layout &x) {
    if (x.empty())
        return;
    int dim=x.front().size();
    ipairs E;
    get_edges_as_pairs(E);
    int n=E.size();
    vector<double> D(n);
    bool initialize=n>300;
    point pq(dim);
    bool isdir=is_directed();
    for (int i=0;i<n;++i) {
        ipair &edge=E[i];
        D[i]=point_distance(x[edge.first],x[edge.second],pq);
        if (initialize || D[i]==0)
            set_edge_attribute(edge.first,edge.second,_GT_ATTRIB_POSITION,isdir?0.6:0.5);
    }
    if (!initialize) {
        vector<double> dist;
        int i,k0;
        double maxs,s;
        point c(dim);
        for (ipairs_iter it=E.begin();it!=E.end();++it) {
            i=it-E.begin();
            double &d=D[i];
            if (d==0)
                continue;
            const point &p=x[it->first];
            dist.clear();
            for (ipairs_iter jt=E.begin();jt!=E.end();++jt) {
                if (edges_incident(*it,*jt))
                    continue;
                if (edges_crossing(*it,*jt,x,c))
                    dist.push_back(point_distance(p,c,pq)/d);
            }
            sort(dist.begin(),dist.end());
            dist.insert(dist.begin(),MARGIN_FACTOR);
            for (int k=dist.size();k-->1;) {
                if (dist[k]<=dist.front())
                    dist.erase(dist.begin()+k);
            }
            while (dist.size()>1 && dist.back()>=1.0-MARGIN_FACTOR) {
                dist.pop_back();
            }
            dist.push_back(1.0-MARGIN_FACTOR);
            n=dist.size();
            maxs=0;
            k0=-1;
            for (int k=0;k<n-1;++k) {
                s=dist[k+1]-dist[k];
                if (s>maxs) {
                    maxs=s;
                    k0=k;
                }
            }
            assert(k0>=0);
            set_edge_attribute(it->first,it->second,_GT_ATTRIB_POSITION,1.0-(dist[k0]+maxs*(isdir?0.4:0.5)));
        }
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
    /* create sparse symmetric incidence matrix I of this graph */
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        const ivector &ngh=it->neighbors();
        if (ngh.empty())
            continue;
        map<int,double> &row=I[it-nodes.begin()];
        for (ivector_iter jt=ngh.begin();jt!=ngh.end();++jt) {
            row[*jt]=1;
        }
    }
    /* use Galerkin product Q*I*P as the incidence matrix IG for graph G */
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
        /* coarsening is slow, switch from EC to MIS method since it's faster */
        multilevel_mis=true;
        multilevel_recursion(x,d,R,K,tol,depth);
        return;
    }
    if (m<2) {
        /* the coarsest level, apply force directed algorithm on a random initial layout */
        multilevel_depth=depth;
        create_random_layout(x,d);
        force_directed_placement(x,K,R*(depth+1)*K,tol,false);
    } else {
        /* create coarser graph H and lay it out */
        graphe G(ctx);
        sparsemat P; // prolongation matrix
        if (multilevel_mis)
            coarsening_mis(mis,G,P);
        else
            coarsening_ec(M,G,P);
        layout y;
        G.multilevel_recursion(y,d,R,K,tol,depth+1);
        /* compute x=P*y (layout lifting) */
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
        /* make the natural spring length K shorter with respect to
     * the current depth level and subsequently refine x */
        double L=K*std::pow(PLASTIC_NUMBER,depth-multilevel_depth);
        force_directed_placement(x,L,R*(depth+1)*L,K*tol/L,false);
    }
}

/* apply multilevel force directed algorithm to layout x */
void graphe::make_spring_layout(layout &x,int d,double tol) {
    int n=node_count();
    if (n==0)
        return;
    if (n==1) {
        x.resize(1);
        x.front().resize(d,0);
    } else if (n<30) {
        x.resize(n);
        create_random_layout(x,d);
        force_directed_placement(x,10.0,DBL_MAX,tol);
    } else {
        multilevel_mis=false;
        multilevel_recursion(x,d,DBL_MAX,10.0,tol);
    }
    if (d==3) {
        /* z-center the layout */
        double minz=DBL_MAX,maxz=-DBL_MAX;
        for (layout_iter it=x.begin();it!=x.end();++it) {
            if (it->back()>maxz)
                maxz=it->back();
            if (it->back()<minz)
                minz=it->back();
        }
        double dz=-(minz+maxz)/2.0;
        for (layout::iterator it=x.begin();it!=x.end();++it) {
            it->back()+=dz;
        }
    }
}

/* layout face as a regular polygon inscribed in circle of radius R */
void graphe::make_regular_polygon_layout(layout &x,const ivector &v,double R) {
    int n=v.size(),i;
    double step=2.0*M_PI/(double)n,phi=(n%2)==0?M_PI_2*(1+2.0/n):M_PI_2;
    for (int k=0;k<n;++k) {
        i=v[k];
        point &p=x[i];
        p.resize(2);
        p[0]=R*std::cos(phi);
        p[1]=R*std::sin(phi);
        phi-=step;
    }
}

/* apply force directed algorithm to this graph (must be triconnected), with the specified outer hull,
* using the algorithm by Bor Plestenjak in "An Algorithm for Drawing Planar Graphs" */
void graphe::make_circular_layout(layout &x,const ivector &hull,double A,double tol) {
    int n=node_count(),iter_count=0,maxper=0,j;
    /* place facial vertices on the unit circle and all other vertices in the origin */
    x.resize(n);
    make_regular_polygon_layout(x,hull);
    if (n==int(hull.size())) // there are no vertices to place inside the circle
        return;
    vector<bool> is_hull_vertex(n,false);
    for (ivector_iter it=hull.begin();it!=hull.end();++it) {
        is_hull_vertex[*it]=true;
    }
    /* compute vertex periphericity */
    ivector per(n);
    periphericity(hull,per);
    for (ivector_iter pt=per.begin();pt!=per.end();++pt) {
        if (*pt>maxper)
            maxper=*pt;
    }
    layout force(n);
    for (int i=0;i<n;++i) {
        force[i].resize(2);
        if (is_hull_vertex[i])
            continue;
        point &p=x[i];
        p.resize(2,0.0);
    }
    /* cool down the system iteratively */
    double d,cool,C=std::sqrt((double)n/M_PI),eps=tol/std::sqrt((double)n);
    do {
        ++iter_count;
        /* compute the resultant force for each non-outer vertex */
        for (int i=0;i<n;++i) {
            if (is_hull_vertex[i])
                continue;
            vertex &v=node(i);
            point &f=force[i],&p=x[i];
            clear_point_coords(f);
            for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
                j=*it;
                point &q=x[j];
                point r(2);
                copy_point(q,r);
                subtract_point(r,p);
                d=C*std::exp(A*(2.0*maxper-per[i]-per[j])/(double)maxper)*point_displacement(r,false);
                scale_point(r,d);
                add_point(f,r);
            }
        }
        /* move each vertex in the direction of the force */
        cool=1.0/(C+std::pow(iter_count,1.5)/C);
        for (int i=0;i<n;++i) {
            if (is_hull_vertex[i])
                continue;
            point &f=force[i],&p=x[i];
            d=point_displacement(f);
            if (d>0) {
                scale_point(f,std::min(d,cool)/d);
                add_point(p,f);
            }
        }
    } while (cool>eps);
}

/* apply Tutte's barycentric method for vertex placement */
void graphe::make_tutte_layout(layout &x,const ivector &outer_face) {
    /* place facial vertices on the unit circle and all other vertices in the origin */
    int n=node_count();
    x.resize(n);
    make_regular_polygon_layout(x,outer_face);
    if (n==int(outer_face.size()))
        return; /* there are no vertices to place inside the circle */
    vector<bool> is_outer(n,false);
    for (ivector_iter it=outer_face.begin();it!=outer_face.end();++it) {
        is_outer[*it]=true;
    }
    for (int i=0;i<n;++i) {
        if (!is_outer[i]) {
            point &p=x[i];
            p.resize(2);
            p[0]=p[1]=0;
        }
    }
    double tol=1e-5;
    point old(2),r(2);
    bool converged;
    int j;
    do {
        converged=true;
        for (int i=0;i<n;++i) {
            if (is_outer[i])
                continue;
            vertex &v=node(i);
            point &p=x[i];
            copy_point(p,old);
            clear_point_coords(p);
            for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
                j=*it;
                add_point(p,x[j]);
            }
            scale_point(p,1.0/(double)v.neighbors().size());
            if (point_distance(p,old,r)>tol)
                converged=false;
        }
    } while (!converged);
}

/* Tomita recursive algorithm (a variant of Bron-Kerbosch) for maximal cliques */
void graphe::tomita(ivector &R,ivector &P,ivector &X,map<int,int> &m,int mode) {
    if (P.empty() && X.empty()) {
        if (mode==1) { // store matching
            if (R.size()==2)
                m[R.front()]=R.back();
        } else if (mode==2) { // store the clique
            maxcliques.push_back(R);
        } else ++m[R.size()];
    } else {
        /* choose as the pivot element the node with the highest number of neighbors in P, say i-th */
        ivector PUX,PP,XX;
        ivector::iterator it,jt;
        sets_union(P,X,PUX);
        int i=PUX.front(),mn=0,n,v;
        ivector S;
        for (it=PUX.begin();it!=PUX.end();++it) {
            const vertex &w=node(*it);
            S.resize(std::max(w.neighbors().size(),P.size()));
            jt=set_intersection(w.neighbors().begin(),w.neighbors().end(),P.begin(),P.end(),S.begin());
            if ((n=int(jt-S.begin()))>mn) {
                mn=n;
                i=*it;
            }
        }
        PUX.clear(); // not needed any more
        sets_difference(P,node(i).neighbors(),S);
        for (it=S.begin();it!=S.end();++it) {
            v=*it;
            const vertex &w=node(v);
            for (jt=R.begin();jt!=R.end();++jt) {
                if (*jt>v) break;
            }
            R.insert(jt,v);
            sets_intersection(P,w.neighbors(),PP);
            sets_intersection(X,w.neighbors(),XX);
            tomita(R,PP,XX,m,mode);
            R.erase(find(R.begin(),R.end(),v));
            P.erase(find(P.begin(),P.end(),v));
            for (jt=X.begin();jt!=X.end();++jt) {
                if (*jt>v) break;
            }
            X.insert(jt,v);
        }
    }
}

/* a modified version of the Bron-Kerbosch algorithm for listing all maximal cliques,
* developed by Tomita et al. Number of k-cliques will be stored to m[k] for each k.
* If store_matching is true, store 2-cliques (v,w) as m[v]=w. */
void graphe::clique_stats(map<int,int> &m,bool store_matching) {
    ivector R,X,P(node_count());
    for (int i=0;i<node_count();++i)
        P[i]=i;
    tomita(R,P,X,m,store_matching?1:0);
}

/* generate a clique vertex cover of this graph */
void graphe::find_maximal_cliques() {
    map<int,int> m;
    ivector R,X,P(node_count());
    for (int i=0;i<node_count();++i)
        P[i]=i;
    clear_maximal_cliques();
    tomita(R,P,X,m,2);
}

/* CP recursive subroutine */
void graphe::cp_recurse(ivector &C,ivector &P,ivector &incumbent) {
    if (C.size()>incumbent.size())
        incumbent=C;
    if (C.size()+P.size()>incumbent.size()) {
        int p,k;
        ivector Q,Cup;
        while (!P.empty()) {
            p=P.back();
            P.pop_back();
            Cup=C;
            if (find(Cup.begin(),Cup.end(),p)==Cup.end())
                Cup.push_back(p);
            const vertex &v=node(p);
            Q.clear();
            Q.resize(std::min(P.size(),v.neighbors().size()));
            k=0;
            for (ivector_iter it=P.begin();it!=P.end();++it) {
                if (v.has_neighbor(*it))
                    Q[k++]=*it;
            }
            Q.resize(k);
            cp_recurse(Cup,Q,incumbent);
        }
    }
}

/* Carraghan-Pardalos algorithm for finding maximum clique in a sparse graph */
int graphe::cp_maxclique(ivector &clique) {
    int n=node_count();
    ivector P(n),C;
    /* initialize P and sort all adjacency lists */
    for (int i=0;i<n;++i)
        P[i]=i;
    /* sort vertices by degree in descending order */
    degree_comparator comp(this);
    sort(P.begin(),P.end(),comp);
    std::reverse(P.begin(),P.end());
    cp_recurse(C,P,clique);
    return clique.size();
}

/*
* Östergård class for finding maximum clique
*
* implemented as described in "A fast algorithm for the maximum clique problem",
* Discrete Applied Mathematics 120 (2002) 197–207
*/

void graphe::ostergard::recurse(ivector &U, int size) {
    if (U.empty()) {
        if (size>maxsize) {
            maxsize=size;
            incumbent=clique_nodes;
            found=true;
        }
        return;
    }
    if (timeout>0 && double(clock()-start)/CLOCKS_PER_SEC>timeout && !incumbent.empty()) {
        timed_out=true;
        return;
    }
    int i,j,minpos,p;
    ivector::iterator it;
    ivector V;
    while (!U.empty()) {
        if (size+int(U.size())<=maxsize)
            break;
        i=U.front();
        it=U.begin();
        minpos=-1;
        for (ivector::iterator jt=U.begin();jt!=U.end();++jt) {
            j=*jt;
            p=G->node(j).position();
            if (minpos<0 || p<minpos) {
                minpos=p;
                i=j;
                it=jt;
            }
        }
        const vertex &v=G->node(i);
        assert(v.low()>0);
        if (size+v.low()<=maxsize)
            break;
        U.erase(it);
        V.resize(std::min(U.size(),v.neighbors().size()));
        j=0;
        for (it=U.begin();it!=U.end();++it) {
            if (*it<v.neighbors().front() || *it>v.neighbors().back())
                continue;
            if (v.has_neighbor(*it))
                V[j++]=*it;
        }
        V.resize(j);
        clique_nodes.push_back(i);
        recurse(V,size+1);
        clique_nodes.pop_back();
        if (found || timed_out)
            break;
    }
}

/* return the size of maximum clique */
int graphe::ostergard::maxclique(ivector &clique) {
    int n=G->node_count();
    ivector S(n),U;
    for (int i=0;i<n;++i) {
        S[i]=i;
        vertex &v=G->node(i);
        v.set_low(0);
    }
    G->greedy_vertex_coloring_biggs(S);
    std::reverse(S.begin(),S.end());
    G->node(S.back()).set_low(1);
    for (ivector_iter it=S.begin();it!=S.end();++it) {
        G->node(*it).set_position(it-S.begin());
    }
    maxsize=0;
    clique_nodes.clear();
    int k;
    start=clock();
    timed_out=false;
    for (int i=n;i-->0;) {
        found=false;
        k=S[i];
        vertex &v=G->node(k);
        U.clear();
        for (ivector_iter it=S.begin()+i;it!=S.end();++it) {
            if (v.has_neighbor(*it))
                U.push_back(*it);
        }
        clique_nodes.push_back(k);
        recurse(U,1);
        if (timed_out)
            break;
        clique_nodes.pop_back();
        v.set_low(maxsize);
    }
    clique=incumbent;
    return clique.size();
}

/*
* End of ostergard class
*/

/* find maximum clique in this graph and return its size */
int graphe::maximum_clique(ivector &clique) {
    assert(!is_directed());
    int n=node_count(),m=edge_count();
    if (20*m<=n*(n-1)) // edge density smaller than or equal to 0.1
        return cp_maxclique(clique);
    ostergard ost(this);
    return ost.maxclique(clique);
}

/* remove a maximal clique from vertex set V using a fast greedy algorithm
* (Johnson, J. Comp. Syst. Sci. 1974) */
void graphe::remove_maximal_clique(ivector &V) const {
    ivector U=V,W,tmp;
    int s,smax,i;
    do {
        smax=i=0;
        W.clear();
        for (ivector_iter it=U.begin();it!=U.end();++it) {
            if ((s=sets_intersection(U,node(*it).neighbors(),tmp))>smax) {
                smax=s;
                W=tmp;
                i=it-U.begin();
            }
        }
        V.erase(find(V.begin(),V.end(),U[i]));
    } while (!(U=W).empty());
}

/* approximate neighborhood clique cover number for every vertex */
void graphe::greedy_neighborhood_clique_cover_numbers(ivector &cover_numbers) {
    int n=node_count();
    cover_numbers.resize(n,0);
    ivector V;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        V=it->neighbors();
        int &cn=cover_numbers[it-nodes.begin()];
        while (!V.empty()) {
            remove_maximal_clique(V);
            ++cn;
        }
    }
}

/* return the number of differently colored and uncolored vertices adjacent to the i-th vertex */
graphe::ipair graphe::adjacent_color_count(int i) const {
    std::set<int> colors;
    int c,unc=0;
    const vertex &v=node(i);
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        c=node(*it).color();
        if (c>0)
            colors.insert(c);
        else ++unc;
    }
    return make_pair(colors.size(),unc);
}

/* insert colors adjacent to the i-th vertex to the given set */
bool graphe::adjacent_colors(int i,std::set<int> &colors) const {
    const vertex &v=node(i);
    if (v.color()>0)
        return false;
    int c;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        if ((c=node(*it).color())>0)
            colors.insert(c);
    }
    return true;
}

/*
* painter class implementation
*/

#ifdef HAVE_LIBGLPK
void graphe::painter::compute_bounds(const ivector &icol,int max_colors) {
    G->greedy_neighborhood_clique_cover_numbers(cover_number);
    G->uncolor_all_nodes();
    lb=(initially_colored=icol).size();
    ub=max_colors;
    if (ub==0) {
        for (ivector_iter it=initially_colored.begin();it!=initially_colored.end();++it) {
            G->set_node_color(*it,it-initially_colored.begin()+1);
        }
        G->dsatur();
        ub=G->color_count();
    }
    if ((generate_clique_cuts=G->node_count()<=100))
        G->find_maximal_cliques();
}

void graphe::painter::make_values() {
    int n=G->node_count(),col=0,k;
    ivector_iter pos;
    values.resize(n);
    for (int i=0;i<n;++i) {
        ivector &x=values[i];
        x.resize(ub);
        pos=find(initially_colored.begin(),initially_colored.end(),i);
        k=pos==initially_colored.end()?-1:int(pos-initially_colored.begin());
        for (int j=0;j<ub;++j) {
            if (k<0) {
                if (j<lb && G->node(initially_colored[j]).has_neighbor(i))
                    x[j]=-2;
                else {
                    x[j]=++col;
                    col2ij.push_back(make_pair(i,j));
                }
            } else
                x[j]=j==k?-1:-2;
        }
    }
    nxcols=col;
}

void graphe::painter::formulate_mip() {
    mip=glp_create_prob();
    glp_set_obj_dir(mip,GLP_MIN);
    int n=G->node_count(),i,j,k;
    iscliq.resize(n,false);
    for (ivector_iter it=initially_colored.begin();it!=initially_colored.end();++it) {
        iscliq[*it]=true;
    }
    /* create row variables and count nonzero entries in the constraint matrix */
    int nonzeros=0,cnt=0,rs,cn,val;
    int nrows=n*(ub+2)-3*lb;
    glp_add_rows(mip,nrows);
    for (i=0;i<n;++i) {
        if (!iscliq[i]) {
            glp_set_row_bnds(mip,++cnt,GLP_FX,1.0,1.0);
            ivector &vals=values[i];
            for (ivector_iter it=vals.begin();it!=vals.end();++it) {
                if (*it>0)
                    ++nonzeros;
            }
        }
    }
    for (k=0;k<n;++k) {
        cn=cover_number[k];
        const vertex &v=G->node(k);
        for (j=0;j<ub;++j) {
            if (j<lb && k==initially_colored[j])
                continue;
            nonzeros+=2;
            rs=j<lb?cn:0;
            for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
                i=*it;
                if ((val=values[i][j])>0)
                    ++nonzeros;
                else rs-=val+2;
            }
            glp_set_row_bnds(mip,++cnt,GLP_UP,0.0,rs);
        }
    }
    for (i=0;i<n;++i) {
        if (!iscliq[i]) {
            glp_set_row_bnds(mip,++cnt,GLP_UP,0.0,lb);
            nonzeros+=ub-lb;
            ivector &vals=values[i];
            for (ivector_iter it=vals.begin();it!=vals.end();++it) {
                if (*it>0)
                    ++nonzeros;
            }
        }
    }
    assert(cnt==nrows);
    /* create column variables */
    int ncols=nxcols+ub-lb;
    glp_add_cols(mip,ncols);
    for (i=0;i<ncols;++i) {
        glp_set_col_kind(mip,i+1,GLP_BV);
        if (i>=nxcols)
            glp_set_obj_coef(mip,i+1,1.0);
    }
    /* create the constraint matrix */
    int *ia=new int[nonzeros+1];
    int *ja=new int[nonzeros+1];
    double *ar=new double[nonzeros+1];
    int row=0,col;
    bool ok;
    cnt=0;
    for (i=0;i<n;++i) {
        if (iscliq[i])
            continue;
        ++row;
        ivector &vals=values[i];
        ok=false;
        for (ivector_iter it=vals.begin();it!=vals.end();++it) {
            if ((col=*it)>0) {
                ia[++cnt]=row; ja[cnt]=col;
                ar[cnt]=1.0;
                ok=true;
            } else assert(col==-2);
        }
        assert(ok);
    }
    for (k=0;k<n;++k) {
        cn=cover_number[k];
        const vertex &v=G->node(k);
        for (j=0;j<ub;++j) {
            if (j<lb && k==initially_colored[j])
                continue;
            ++row;
            for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
                col=values[*it][j];
                if (col>0) {
                    ia[++cnt]=row; ja[cnt]=col;
                    ar[cnt]=1.0;
                }
            }
            if ((col=values[k][j])>0) {
                ia[++cnt]=row; ja[cnt]=col;
                ar[cnt]=cn;
            }
            if (j>=lb) {
                col=1+nxcols+j-lb;
                ia[++cnt]=row; ja[cnt]=col;
                ar[cnt]=-cn;
            }
        }
    }
    for (i=0;i<n;++i) {
        if (iscliq[i])
            continue;
        ++row;
        ivector &vals=values[i];
        for (ivector_iter it=vals.begin();it!=vals.end();++it) {
            j=it-vals.begin();
            if ((col=*it)>0) {
                ia[++cnt]=row; ja[cnt]=col;
                ar[cnt]=j+1;
            }
        }
        for (col=nxcols+1;col<=ncols;++col) {
            ia[++cnt]=row; ja[cnt]=col;
            ar[cnt]=-1.0;
        }
    }
    assert(cnt<=nonzeros && row==nrows);
    /* assign the constraint matrix to the MIP */
    glp_load_matrix(mip,cnt,ia,ja,ar);
    delete[] ia; delete[] ja;
    delete[] ar;
    for (i=0;i<nrows;++i) {
        glp_set_row_stat(mip,i+1,GLP_BS);
    }
}

void graphe::painter::callback(glp_tree *tree,void *info) {
    painter *pt=static_cast<painter*>(info);
    int j;
    switch (glp_ios_reason(tree)) {
    case GLP_IBRANCH:
        /* select the branching variable */
        if ((j=pt->select_branching_variable(tree))>0)
            glp_ios_branch_upon(tree,j,GLP_DN_BRNCH);
        break;
    case GLP_IHEUR:
        /* obtain a heuristic solution */
        pt->heur_solution(tree);
        break;
    case GLP_IROWGEN:
        /* generate clique and block color inequalities */
        pt->generate_rows(tree);
        break;
    default:
        /* ignore call for other reasons */
        break;
    }
}

/* generate constraints violated by the current subproblem relaxation solution */
void graphe::painter::generate_rows(glp_tree *tree) {
    glp_prob *prob=glp_ios_get_prob(tree);
    int n=G->node_count(),col,w,len,r=glp_ios_curr_node(tree),best_len=-1;
    int *data=(int*)glp_ios_node_data(tree,r);
    if (*data>maxiter)
        return;
    (*data)+=1;
    double rh,lh,h,maxh=0;
    /* block color cuts */
    for (int c=ub;c-->lb;) {
        w=nxcols+c-lb+1;
        for (int i=0;i<n;++i) {
            if (iscliq[i])
                continue;
            rh=0.0;
            len=0;
            for (int j=c;j<ub;++j) {
                col=values[i][j];
                if (col<0)
                    rh-=col+2;
                else {
                    row_indices[++len]=col;
                    row_coeffs[len]=1.0;
                }
            }
            if (len==0)
                continue;
            row_indices[++len]=w;
            row_coeffs[len]=-1.0;
            lh=0;
            for (int k=0;k<len;++k) {
                lh+=row_coeffs[k+1]*glp_get_col_prim(prob,row_indices[k+1]);
            }
            h=lh-rh;
            //h/=len;
            if (h>maxh) {
                maxh=h;
                memcpy(best_indices+1,row_indices+1,len*sizeof(int));
                memcpy(best_coeffs+1,row_coeffs+1,len*sizeof(double));
                best_len=len;
            }
        }
    }
    if (best_len>0)
        add_row(prob,best_len,best_indices,best_coeffs,rh);
    /* clique cuts */
    if (generate_clique_cuts) {
        const ivectors &cover=G->maximal_cliques();
        double lim=30.0/double(cover.size());
        int nc=0;
        for (int c=ub;c-->lb;) {
            w=nxcols+c-lb+1;
            for (ivectors_iter it=cover.begin();it!=cover.end();++it) {
                if (G->rand_uniform()>=lim)
                    continue;
                const ivector &cliq=*it;
                rh=0.0;
                len=0;
                for (ivector_iter jt=cliq.begin();jt!=cliq.end();++jt) {
                    col=values[*jt][c];
                    if (col<0)
                        rh-=col+2;
                    else {
                        row_indices[++len]=col;
                        row_coeffs[len]=1.0;
                    }
                }
                if (len==0)
                    continue;
                row_indices[++len]=w;
                row_coeffs[len]=-1.0;
                lh=0;
                for (int k=0;k<len;++k) {
                    lh+=row_coeffs[k+1]*glp_get_col_prim(prob,row_indices[k+1]);
                }
                h=lh-rh;
                //h/=len;
                if (h>0) {
                    add_row(prob,len,row_indices,row_coeffs,rh);
                    ++nc;
                }
            }
        }
    }
}

void graphe::painter::add_row(glp_prob *prob,int len,int *indices,double *coeffs,double rh) {
    int r=glp_add_rows(prob,1);
    glp_set_row_bnds(prob,r,GLP_UP,-DBL_MAX,rh);
    glp_set_mat_row(prob,r,len,indices,coeffs);
}

/* color the nodes for which the colors are known */
void graphe::painter::fixed_coloring(glp_tree *tree) {
    glp_prob *prob=glp_ios_get_prob(tree); // current subproblem
    G->uncolor_all_nodes();
    for (ivector_iter it=initially_colored.begin();it!=initially_colored.end();++it) {
        G->set_node_color(*it,int(it-initially_colored.begin())+1);
    }
    for (int j=nxcols;j-->0;) {
        ipair &ij=col2ij[j];
        if (!glp_ios_can_branch(tree,j+1) && glp_get_col_prim(prob,j+1)==1)
            G->set_node_color(ij.first,lb+ij.second+1);
    }
}

/* fast construction of a heuristic solution using greedy coloring */
void graphe::painter::heur_solution(glp_tree *tree) {
    if (maxiter==0) {
        fixed_coloring(tree);
        /* dsatur coloring, quadratic time */
        G->dsatur();
        assign_heur(tree);
    }
    int iter_count=0;
    while (++iter_count<=maxiter) {
        /* greedy coloring, linear time */
        fixed_coloring(tree);
        int n=G->node_count(),i,j;
        ordering=G->rand_permu(n);
        for (ivector_iter ot=ordering.begin();ot!=ordering.end();++ot) {
            i=*ot;
            used_colors.clear();
            if (!G->adjacent_colors(i,used_colors))
                continue;
            j=0;
            std::set<int>::const_iterator it=used_colors.begin();
            for (;it!=used_colors.end();++it) {
                if (*it!=++j) {
                    G->set_node_color(i,j);
                    break;
                }
            }
            if (it==used_colors.end())
                G->set_node_color(i,j+1);
        }
        if (assign_heur(tree)==0)
            break;
    }
}

/* offer heuristic solution to MIP solver */
int graphe::painter::assign_heur(glp_tree *tree) {
    G->get_node_colors(temp_colors);
    int nc=G->color_count(),i,j;
    if (nc>ub)
        return 1;
    nc-=lb;
    for (int col=1;col<=nxcols;++col) {
        const ipair &p=col2ij[col-1];
        i=p.first;
        j=p.second;
        *(heur+col)=temp_colors[i]==j+1?1.0:0.0;
    }
    for (i=0;i<ub-lb;++i) {
        *(heur+nxcols+1+i)=i<nc?1.0:0.0;
    }
    return glp_ios_heur_sol(tree,heur);
}

/* the custom branching rule for MVCP (Brelaz): a fractionable vertex with
* the largest number of different adjacent colors is chosen, ties are broken by
* choosing the vertex with the maximal degree in the uncolored subgraph */
int graphe::painter::select_branching_variable(glp_tree *tree) {
    glp_prob *prob=glp_ios_get_prob(tree); // current subproblem
    std::fill(branch_candidates.begin(),branch_candidates.end(),-1);
    fixed_coloring(tree);
    double fr;
    for (int j=nxcols;j-->0;) {
        ipair &ij=col2ij[j];
        if (glp_ios_can_branch(tree,j+1)) {
            int &bc=branch_candidates[ij.first];
            fr=std::abs(glp_get_col_prim(prob,j+1)-0.5);
            if (bc<0 || fr<fracts[ij.first]) {
                bc=ij.second;
                fracts[ij.first]=fr;
            }
        }
    }
    int i,j=0;
    ipair nc,nc_max=make_pair(-1,-1);
    for (ivector_iter it=branch_candidates.begin();it!=branch_candidates.end();++it) {
        if (*it<0)
            continue;
        i=it-branch_candidates.begin();
        nc=G->adjacent_color_count(i);
        if (nc>nc_max) {
            nc_max=nc;
            j=values[i][*it];
        }
    }
    return j;
}

int graphe::painter::color_vertices(ivector &colors,const ivector &icol,int max_colors) {
    compute_bounds(icol,max_colors);
    if (ub<lb)
        return 0;
    int n=G->node_count();
    make_values();
    formulate_mip();
    glp_smcp lparm;
    glp_init_smcp(&lparm);
    lparm.msg_lev=GLP_MSG_OFF;
    glp_simplex(mip,&lparm);
    glp_iocp parm;
    glp_init_iocp(&parm);
    parm.msg_lev=GLP_MSG_OFF;
    parm.gmi_cuts=GLP_OFF;
    parm.mir_cuts=GLP_OFF;
    parm.clq_cuts=GLP_ON;
    parm.cov_cuts=GLP_ON;
    parm.presolve=GLP_OFF;
    parm.bt_tech=GLP_BT_DFS;
    parm.cb_size=sizeof(int);
    parm.cb_func=&callback;
    parm.cb_info=static_cast<void*>(this);
    branch_candidates.resize(n);
    fracts.resize(n);
    maxiter=std::ceil(15.0*std::exp(-std::pow(n,2)*0.000321887582487));
    heur=new double[nxcols+ub-lb+1];
    row_indices=new int[nxcols+ub-lb+1];
    row_coeffs=new double[nxcols+ub-lb+1];
    best_indices=new int[nxcols+ub-lb+1];
    best_coeffs=new double[nxcols+ub-lb+1];
    int ncolors=0,res=glp_intopt(mip,&parm);
    if (res==0) {
        switch (glp_get_status(mip)) { // solution status:
        case GLP_UNDEF:
            G->message("Error: MIP solution undefined");
            break;
        case GLP_FEAS: // feasible but not necessarily optimal
            G->message("Warning: optimality of the solution is not guaranteed");
        case GLP_OPT: // optimal
            ncolors=lb+glp_mip_obj_val(mip);
            /* color the vertices */
            for (int k=0;k<nxcols;++k) {
                ipair &p=col2ij[k];
                if (glp_mip_col_val(mip,k+1)>0)
                    G->set_node_color(p.first,p.second+1);
            }
            G->get_node_colors(colors);
            break;
        case GLP_NOFEAS: // not feasible
            G->message("Error: MIP has no feasible solutions");
            break;
        }
    } // else G->message("Error: MIP solver failure");
    glp_delete_prob(mip);
    delete[] heur;
    delete[] row_indices;
    delete[] row_coeffs;
    delete[] best_indices;
    delete[] best_coeffs;
    return ncolors;
}
#endif

/*
* end of painter class
*/

/* find optimal vertex coloring using an exact algorithm, requires GLPK */
int graphe::exact_vertex_coloring(int max_colors) {
    int ncolors=0;
#ifndef HAVE_LIBGLPK
    message("Error: GLPK library is required for exact minimal graph coloring");
#else
    painter pt(this);
    ivector colors,clique;
    ostergard ost(this,5.0);
    ost.maxclique(clique);
    ncolors=pt.color_vertices(colors,clique,max_colors);
    if (ncolors>0 && find(colors.begin(),colors.end(),0)!=colors.end()) {
        uncolor_all_nodes();
        ncolors=0;
    }
#endif
    return ncolors;
}

/* color the edges of this graph using D or D+1 colors,
* return 1 in the former case, 2 in the latter, 0 on error */
int graphe::exact_edge_coloring(ivector &colors,int *numcol) {
    graphe L(ctx);
    ipairs E;
    line_graph(L,E);
    /* find the vertex with maximum degree in G (this graph) */
    int m=E.size(),maxdeg=0,deg,i,j,k;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if ((deg=it->neighbors().size())>maxdeg) {
            maxdeg=deg;
            i=it-nodes.begin();
        }
    }
    ivector icol(maxdeg);
    k=0;
    for (j=0;j<m;++j) {
        const ipair &e=E[j];
        if (e.first==i || e.second==i)
            icol[k++]=j;
    }
    assert(k==maxdeg);
    painter pt(&L);
    int ncolors=pt.color_vertices(colors,icol,maxdeg+1);
    for (k=0;k<maxdeg;++k) {
        colors[icol[k]]=k+1;
    }
    if (ncolors==0 || find(colors.begin(),colors.end(),0)!=colors.end())
        return 0;
    for (j=0;j<m;++j) {
        const ipair &e=E[j];
        set_edge_attribute(e.first,e.second,_GT_ATTRIB_COLOR,colors[j]);
    }
    if (numcol!=NULL)
        *numcol=ncolors;
    return ncolors-maxdeg+1;
}

/* returns true iff there is a clique cover of order not larger than k and finds that cover */
bool graphe::clique_cover(ivectors &cover,int k) {
    if (is_triangle_free()) {
        /* clique cover consists of matched edges and singleton vertex sets */
        ipairs matching;
        edmonds maximizer(this);
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
    graphe C(ctx);
    complement(C);
    int ncliques=C.exact_vertex_coloring();
    if (ncliques==0 || (k>0 && ncliques>k))
        return false;
    cover.clear();
    cover.resize(ncliques);
    for (int i=node_count();i-->0;) {
        const vertex &v=C.node(i);
        cover[v.color()-1].push_back(i);
    }
    return true;
}

/* return true iff the graph is complete (i.e., a clique) */
bool graphe::is_clique(int sg) const {
    assert(!is_directed());
    int i,j,n=node_count();
    for (i=0;i<n;++i) {
        if (sg>=0 && node(i).subgraph()!=sg)
            continue;
        for (j=i+1;j<n;++j) {
            if (sg>=0 && node(j).subgraph()!=sg)
                continue;
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
    if (is_directed()) {
        graphe G(ctx);
        underlying(G);
        return G.is_triangle_free();
    }
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
    assert(node_stack.empty());
    nodes.erase(nodes.begin()+i);
    int j;
    stack<attrib> attr;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
            node_stack.push(j>i?j-1:j);
            attr.push(it->neighbor_attributes(j));
        }
        it->clear_neighbors();
        while (!node_stack.empty()) {
            it->add_neighbor(node_stack.top(),attr.top());
            node_stack.pop();
            attr.pop();
        }
    }
}

/* contract the edge {i,j}, leaving j-th node isolated (not connected to any other node) */
void graphe::contract_edge(int i,int j) {
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
    vertex &v=node(i),&w=node(j);
    point p,q,r;
    if (get_node_position(v.attributes(),p) && get_node_position(w.attributes(),q) && p.size()==q.size()) {
        r.resize(p.size());
        copy_point(p,r);
        add_point(r,q);
        scale_point(r,0.5);
        v.set_attribute(_GT_ATTRIB_POSITION,point2gen(r));
    }
}

/* subdivide the edge e with r new vertices */
void graphe::subdivide_edge(const ipair &e,int n,int &label) {
    remove_edge(e);
    int i=e.first,j,nv=node_count();
    for (int k=0;k<n;++k) {
        j=add_node(++label);
        add_edge(i,j);
        i=j;
    }
    add_edge(i,e.second);
    vertex &v=node(e.first),&w=node(e.second);
    point p,q,r;
    if (get_node_position(v.attributes(),p) && get_node_position(w.attributes(),q) && p.size()==q.size()) {
        r.resize(p.size());
        copy_point(q,r);
        subtract_point(r,p);
        scale_point(r,1.0/double(n+1));
        for (i=nv;i<node_count();++i) {
            add_point(p,r);
            node(i).set_attribute(_GT_ATTRIB_POSITION,point2gen(p));
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
        int i,j;
        for (ivector_iter it=V.begin();it!=V.end();++it) {
            i=*it;
            vertex &v=node(i);
            for (ivector_iter jt=v.neighbors().begin();jt!=v.neighbors().end();++jt) {
                j=*jt;
                E.insert(make_pair(i<j?i:j,i<j?j:i));
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
    do {
        sort(D.begin(),D.end());
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
    assert(node_queue.empty());
    int g=RAND_MAX,h,i,j;
    bool hascycle=false;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        unvisit_all_nodes(sg);
        i=it-nodes.begin();
        vertex &v=node(i);
        if (sg>=0 && v.subgraph()!=sg)
            continue;
        v.unset_ancestor();
        v.set_disc(0);
        node_queue.push(i);
        while (!node_queue.empty()) {
            j=node_queue.front();
            node_queue.pop();
            vertex &w=node(j);
            w.set_visited(true);
            for (ivector_iter jt=w.neighbors().begin();jt!=w.neighbors().end();++jt) {
                vertex &u=node(*jt);
                if (*jt==w.ancestor() || (sg>=0 && u.subgraph()!=sg))
                    continue;
                if (!u.is_visited()) {
                    u.set_ancestor(j);
                    u.set_disc(w.disc()+1);
                    node_queue.push(*jt);
                } else {
                    h=w.disc()+u.disc()+1;
                    if (h<g && (!odd || h%2!=0)) {
                        hascycle=true;
                        g=h;
                    }
                }
            }
        }
    }
    return hascycle?g:-1;
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
void graphe::make_sierpinski_graph(int n,int k,bool triangle) {
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
        /* remove all non-clique edges to obtain Sierpinski triangle graph */
        map<int,int> m;
        clique_stats(m,true);
        ivector isolated_nodes;
        for (map<int,int>::const_iterator it=m.begin();it!=m.end();++it) {
            int v=it->first,w=it->second;
            contract_edge(v,w);
            isolated_nodes.push_back(w);
        }
        sort(isolated_nodes.begin(),isolated_nodes.end());
        for (unsigned i=isolated_nodes.size();i-->0;) {
            remove_isolated_node(isolated_nodes[i]);
        }
        vecteur labels;
        make_default_labels(labels,node_count());
        relabel_nodes(labels);
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
void graphe::make_complete_multipartite_graph(const ivector &partition_sizes,layout *x) {
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
    if (x!=NULL && partition_sizes.size()==2 && partition_sizes.front()==1 && partition_sizes.back()>2) {
        /* create the star layout */
        int n=partition_sizes.back();
        ivector hull(n);
        for (int i=0;i<n;++i) hull[i]=i+1;
        make_circular_layout(*x,hull);
    }
}

/* create generalized Petersen graph G(n,k) using Watkins' notation */
void graphe::make_petersen_graph(int n,int k,layout *x) {
    vecteur V;
    make_default_labels(V,2*n);
    add_nodes(V);
    /* add the outer cycle first */
    for (int i=0;i<n;++i) {
        add_edge(i,(i+1)%n);
    }
    /* add the inner edges */
    for (int i=0;i<n;++i) {
        add_edge(i,i+n);
        add_edge(i+n,(i+k)%n+n);
    }
    if (x!=NULL) {
        /* layout the graph */
        ivector hull(n);
        for (int i=0;i<n;++i) hull[i]=i;
        make_circular_layout(*x,hull,2.5);
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
void graphe::make_web_graph(int n,int m,layout *x) {
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
    if (x!=NULL) {
        /* layout the graph */
        ivector hull(n);
        for (int i=0;i<n;++i) hull[i]=m*i;
        make_circular_layout(*x,hull);
    }
}

/* create wheel graph with n+1 vertices */
void graphe::make_wheel_graph(int n,layout *x) {
    vecteur V;
    make_default_labels(V,n,0,1);
    add_nodes(V);
    make_cycle_graph();
    int i=add_node(0);
    for (int j=0;j<n;++j) {
        add_edge(i,j);
    }
    if (x!=NULL) {
        /* layout the graph */
        ivector hull(n);
        for (int i=0;i<n;++i) hull[i]=i;
        make_circular_layout(*x,hull);
    }
}

/* create antiprism graph of order n (with 2n vertices and 4n edges) */
void graphe::make_antiprism_graph(int n,layout *x) {
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
    if (x!=NULL) {
        /* layout the graph */
        ivector hull(n);
        for (int i=0;i<n;++i) hull[i]=2*i;
        make_circular_layout(*x,hull,-1.0);
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

/* return the number of connected components in this graph */
int graphe::connected_components_count(int sg) {
    unvisit_all_nodes(sg);
    unset_all_ancestors(sg);
    disc_time=0;
    int count=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if ((sg<0 || it->subgraph()==sg) && !it->is_visited()) {
            dfs(it-nodes.begin(),false,false,NULL,sg);
            ++count;
        }
    }
    return count;
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
        /* output a strongly connected component */
        components.resize(components.size()+1);
        ivector &component=components.back();
        int w;
        do {
            w=node_stack.top();
            node_stack.pop();
            component.push_back(w);
            node(w).set_on_stack(false);
        } while (w!=i);
    }
}

/* find all strongly connected components in directed graph using Tarjan's algorithm */
void graphe::strongly_connected_components(ivectors &components,int sg) {
    assert(node_stack.empty());
    unvisit_all_nodes(sg);
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->set_on_stack(false);
    }
    disc_time=0;
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

void graphe::find_cut_vertices_dfs(int i,std::set<int> &ap,int sg) {
    vertex &v=node(i);
    v.set_visited(true);
    ++disc_time;
    v.set_disc(disc_time);
    v.set_low(disc_time);
    int children=0;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        int j=*it;
        vertex &w=node(j);
        if (sg>=0 && w.subgraph()!=sg)
            continue;
        if (!w.is_visited()) {
            ++children;
            w.set_ancestor(i);
            find_cut_vertices_dfs(j,ap,sg);
            if (v.ancestor()<0) {
                if (children==2)
                    ap.insert(i);
            } else {
                v.set_low(std::min(v.low(),w.low()));
                if (w.low()>=v.disc())
                    ap.insert(i);
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
    std::set<int> ap;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if ((sg<0 || it->subgraph()==sg) && !it->is_visited())
            find_cut_vertices_dfs(it-nodes.begin(),ap,sg);
    }
    for (std::set<int>::const_iterator it=ap.begin();it!=ap.end();++it) {
        articulation_points.push_back(*it);
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
                /* output biconnected component to 'blocks' */
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

/* return true iff the graph is (semi-)eulerian */


/* Fleury's algorithm for eulerian trail, time complexity O(m^2) */
bool graphe::fleury(int start,ivector &path) {
    /* assuming that the graph is undirected and (semi-)eulerian */
    int m=edge_count(),i=start,j;
    path.resize(m+1);
    path.back()=i;
    ipairs bridges;
    ipair edge;
    while (m>0) {
        vertex &v=node(i);
        find_bridges(bridges);
        for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
            j=*it;
            edge=make_pair(i<j?i:j,i<j?j:i);
            if (find(bridges.begin(),bridges.end(),edge)==bridges.end())
                break;
        }
        remove_edge(edge);
        path[--m]=j;
        i=j;
    }
    return true;
}

/* Hierholzer algorithm for eulerian cycle, time complexity O(m) */
void graphe::hierholzer(ivector &path) {
    /* assuming that the graph is eulerian */
    int i,j,pos=path.size();
    ivector_iter it;
    while (pos>0) {
        --pos;
        /* find a closed trail starting in path[pos] */
        j=i=path[pos];
        do {
            vertex &v=node(j);
            for (it=v.neighbors().begin();it!=v.neighbors().end();++it) {
                if (!is_edge_visited(j,*it))
                    break;
            }
            if (it==v.neighbors().end())
                break;
            path.insert(path.begin()+(++pos),*it);
            set_edge_visited(j,*it);
            j=*it;
        } while (i!=j);
    }
}

/* return the node in which an eulerian trail may begin, or -1 if the graph has no such trail */
int graphe::eulerian_path_start(bool &iscycle) const {
    /* assuming that the graph is connected */
    int n=node_count(),count=0,start_node=0;
    for (int i=0;i<n;++i) {
        if (degree(i)%2!=0) {
            ++count;
            start_node=i;
        }
    }
    if (count!=0 && count!=2)
        return -1;
    iscycle=count==0;
    return start_node;
}

/* attempt to find an eulerian trail in this graph, return true iff one exists */
bool graphe::find_eulerian_path(ivector &path) {
    int start;
    bool iscycle;
    if (!is_connected() || (start=eulerian_path_start(iscycle))<0)
        return false;
    /* use Hierholzer's algorithm */
    assert(visited_edges.empty());
    path.clear();
    path.reserve(edge_count()+1);
    if (!iscycle) {
        /* find a path between two odd-degree nodes and delete it */
        dfs(start,false);
        int j,k=-1;
        for (int i=node_count();i-->0;) {
            if (i!=start && degree(i)%2!=0) {
                k=i;
                break;
            }
        }
        assert(k>=0);
        while (k!=start) {
            path.push_back(k);
            const vertex &v=node(k);
            j=v.ancestor();
            set_edge_visited(j,k);
            k=j;
        }
    }
    path.push_back(start);
    hierholzer(path);
    unvisit_all_edges();
    return true;
}

int graphe::find_cycle_dfs(int i,int sg) {
    vertex &v=node(i);
    v.set_visited(true);
    node_stack.push(i);
    int j,k;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        j=*it;
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
    assert(node_stack.empty());
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
        clear_node_stack();
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
    assert(node_stack.empty());
    unvisit_all_nodes(sg);
    if (find_path_dfs(i,j,sg,skip_embedded)) {
        path.resize(node_stack.size());
        while (!node_stack.empty()) {
            path[node_stack.size()-1]=node_stack.top();
            node_stack.pop();
        }
        return true;
    }
    clear_node_stack();
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
void graphe::unembed_all_nodes() {
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->set_embedded(false);
    }
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
    std::set<int> contact_nodes;
    unembed_all_nodes();
    /* adding two initial faces, obtained by finding a cycle in graph */
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
                    /* create a bridge consisting only of edge (i,j) */
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
            for (std::set<int>::const_iterator it=contact_nodes.begin();it!=contact_nodes.end();++it) {
                bridge[i++]=*it;
            }
        }
        if (bridges.empty())
            break;
        /* for each bridge, find all faces in which it can
     * be drawn (these faces are called 'admissible') */
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
        /* select the first bridge with the smallest number of admissible faces */
        n=RAND_MAX;
        for (ipairs_iter it=admissible_faces.begin();it!=admissible_faces.end();++it) {
            if (it->first<n) {
                k=it-admissible_faces.begin();
                if ((n=it->first)==1)
                    break;
            }
        }
        ivector &bridge=bridges[k];
        /* draw a path between the first two connecting vertices
     * to the admissible face, splitting it */
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
        arc_path(i,j,face,face1);
        arc_path(j,i,face,face2);
        if (path.size()>0) {
            face1.insert(face1.end(),path.begin(),path.end());
            face2.insert(face2.end(),path.rbegin(),path.rend());
        }
        faces.erase(faces.begin()+f);
        faces.push_back(face1);
        faces.push_back(face2);
        bridges.erase(bridges.begin()+k); // we're done with this bridge
    }
    /* we shuld have had found exactly ec-nc+2 faces (by Euler's theorem) */
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
    /* block structure: [is_visited (0 or 1), parent_index, connecting_vertex, ap_1, ap_2, ..., ap_n] */
    ivector &parent=blocks[i];
    parent.front()=1; // mark block as visited
    for (int j=0;j<n;++j) {
        /* find children */
        ivector &block=blocks[j];
        if (i==j || block.front()==1 || (c=common_element(parent,block,3))==-1)
            continue;
        /* block is a child connected in the vertex c */
        block.erase(find(block.begin()+3,block.end(),c));
        block[1]=i;
        block[2]=c;
        build_block_tree(j,blocks);
    }
}

/* choose a face to embed among those containing the vertex v */
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
    /* find all children of the i-th block */
    ivector children;
    for (ivectors_iter it=block_tree.begin();it!=block_tree.end();++it) {
        if (it->at(1)==i)
            children.push_back(it-block_tree.begin());
    }
    if (children.empty()) // this is a leaf node in the tree
        return;
    /* embed each child to the largest available face of the parent */
    ivectors &parent_faces=blocks_faces[i];
    int c,pf,cf,k,n;
    ivector::iterator vt;
    ivector::reverse_iterator wt;
    for (ivector_iter it=children.begin();it!=children.end();++it) {
        embed_children_blocks(*it,block_tree,blocks_faces);
        ivectors &child_faces=blocks_faces[*it];
        c=block_tree[*it][2]; // articulation vertex connecting the child with its parent
        /* find the suitable parent and child faces that contain c */
        pf=choose_embedding_face(parent_faces,c);
        cf=choose_embedding_face(child_faces,c);
        assert(pf>=0 && cf>=0);
        n=parent_faces.size();
        parent_faces.resize(n+child_faces.size());
        /* Add all child faces different than child_face to parent_faces,
     * add a new triangular face obtained by connecting neighbors of c in child and parent,
     * and modify the parent_face by adding child_face edges and removing two edges. */
        k=n;
        for (ivectors::iterator ft=child_faces.begin();ft!=child_faces.end();++ft) {
            if (int(ft-child_faces.begin())==cf)
                continue;
            parent_faces[k++].swap(*ft);
        }
        ivector &parent_face=parent_faces[pf],&child_face=child_faces[cf],&new_face=parent_faces.back();
        /* get v, w as neighbors of c in parent resp. child face */
        vt=find(parent_face.begin(),parent_face.end(),c)+1;
        if (vt==parent_face.end())
            vt=parent_face.begin();
        wt=find(child_face.rbegin(),child_face.rend(),c)+1;
        if (wt==child_face.rend())
            wt=child_face.rbegin();
        assert(*vt!=*wt);
        add_temporary_edge(*vt,*wt);
        /* construct the new face */
        new_face.resize(3);
        new_face[0]=*wt;
        new_face[1]=c;
        new_face[2]=*vt;
        /* modify the parent_face */
        ivector path(child_face.size()-1);
        k=0;
        while (*wt!=c) {
            path[k++]=*(wt++);
            if (wt==child_face.rend())
                wt=child_face.rbegin();
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
    /* split graph to blocks */
    vector<ipairs> blocks;
    find_blocks(blocks);
    if (blocks.size()==1) {
        if (!demoucron(faces))
            return -1;
    } else {
        /* there exist at least one articulation point */
        vector<ivectors> blocks_faces(blocks.size());
        ivector cv;
        find_cut_vertices(cv);
        int i=0,nf;
        for (vector<ipairs>::const_iterator it=blocks.begin();it!=blocks.end();++it) {
            /* test each block separately */
            graphe G(ctx);
            subgraph(*it,G,false);
            ivectors &block_faces=blocks_faces[i++];
            if (G.node_count()>2) {
                /* block has three or more vertices */
                if (int(it->size())+6>3*G.node_count() || !G.demoucron(block_faces))
                    return -1;
            } else {
                /* block contains only one edge  */
                ivector bin_face(2,0);
                bin_face[1]=1;
                block_faces.push_back(bin_face);
            }
            /* push back a vector of articulation points which belong to this component */
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
     * Now blocks are embedded into each other, starting from peripheral blocks,
     * by adding temporary edges. */
        /* make a tree of blocks */
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
        /* embed all blocks to the root block by climbing up the tree recursively */
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

/* compute the periphericity of the vertices with respect to
* the outer face by applying BFS (starting in each of the outer vertices) */
void graphe::periphericity(const ivector &outer_face,ivector &p) {
    assert(node_queue.empty());
    int level,i,j;
    std::fill(p.begin(),p.end(),RAND_MAX);
    for (ivector_iter jt=outer_face.begin();jt!=outer_face.end();++jt) {
        p[*jt]=0;
    }
    for (ivector_iter it=outer_face.begin();it!=outer_face.end();++it) {
        unvisit_all_nodes();
        node_queue.push(*it);
        level=1;
        while (!node_queue.empty()) {
            i=node_queue.front();
            vertex &v=node(i);
            for (ivector_iter jt=v.neighbors().begin();jt!=v.neighbors().end();++jt) {
                j=*jt;
                vertex &w=node(j);
                if (w.is_visited() || p[j]==0) // skip the outer vertices
                    continue;
                if (level<p[j])
                    p[j]=level;
                node_queue.push(j);
                w.set_visited(true);
            }
            ++level;
            node_queue.pop();
        }
    }
}

/*
* TREE NODE POSITIONING ALGORITHM
*/

/* traverse the tree from top to bottom using DFS */
void graphe::walker::walk(int i,int pass,int level,double modsum) {
    vertex &v=G->node(i);
    v.set_visited(true);
    if (pass==1)
        v.clear_children();
    double m=0;
    if (pass==3) {
        point &p=x->at(i);
        p.resize(2);
        p[0]=v.prelim()+modsum;
        p[1]=-level*vsep;
        m=v.modifier();
    }
    int j;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        j=*it;
        vertex &w=G->node(j);
        if (w.is_visited())
            continue;
        if (pass==1) {
            w.set_ancestor(i);
            v.inc_children();
        }
        walk(j,pass,level+1,modsum+m);
    }
    if (pass==1) {
        ++node_counters[level];
        depth=std::max(depth,level+1);
    } else if (pass==2) {
        if (node_counters[level]>0 && G->node(levels[level][node_counters[level]-1]).ancestor()!=v.ancestor())
            ++gap_counters[level];
        v.set_position(node_counters[level]++);
        levels[level][v.position()]=i;
        if (!v.is_leaf()) {
            v.set_gaps(gap_counters[level]);
            gap_counters[level]=0;
        }
    }
}

/* set prelim and modifier for each node in V (they are on the same level) */
void graphe::walker::process_level(int i) {
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
void graphe::walker::positioning(int apex) {
    walk(apex,1); // first walk: determine tree depth and set node ancestors
    /* allocate memory for level lists */
    levels.resize(depth);
    gap_counters.resize(depth,0);
    for (int i=0;i<depth;++i) {
        levels[i].resize(node_counters[i]);
        node_counters[i]=0;
    }
    G->unvisit_all_nodes();
    walk(apex,2); // second walk: create levels
    /* do node positioning for each level (except the top one) in a single sweep */
    for (int level=depth;level-->1;) {
        process_level(level);
    }
    G->unvisit_all_nodes();
    walk(apex,3); // third walk: sum up the modifiers (i.e. move subtrees)
}

graphe::walker::walker(graphe *gr,layout *ly,double hs,double vs) {
    G=gr;
    x=ly;
    hsep=hs;
    vsep=vs;
    depth=0;
    int n=G->node_count();
    node_counters.resize(n,0);
}

/*
* END OF TREE NODE POSITIONING ALGORITHM
*/

/* calculate node positions and store them to the specified layout */
void graphe::make_tree_layout(layout &x,double sep,int apex) {
    int n=node_count();
    x.resize(n);
    if (n==0) return;
    unset_all_ancestors();
    unvisit_all_nodes();
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->set_prelim(-1);
        it->set_modifier(0);
    }
    /* layout the tree */
    double hsep=sep,vsep=sep*std::pow(PLASTIC_NUMBER,2);
    walker P(this,&x,hsep,vsep);
    P.positioning(apex);
}

/* create a random tree with n vertices and degree not larger than maxd */
void graphe::make_random_tree(const vecteur &V,int maxd,bool addnodes) {
    if (addnodes) {
        this->clear();
        add_nodes(V);
    }
    vecteur src,labels=*_randperm(V,ctx)._VECTptr;
    src.push_back(labels.back());
    labels.pop_back();
    gen v,w;
    while (!labels.empty()) {
        v=_rand(src,ctx);
        w=labels.back();
        labels.pop_back();
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
        vertex &w=node(j);
        if (!w.is_visited())
            tree_height_dfs(j,level+1,depth);
    }
}

/* return the height of this tree (no check is made) */
int graphe::tree_height(int root) {
    /* assuming that this is a tree */
    if (node_count()==1)
        return 0;
    unvisit_all_nodes();
    int depth=0;
    tree_height_dfs(root,0,depth);
    return depth;
}


/* create a random biconnected planar graph with n vertices */
void graphe::make_random_planar(double p,int connectivity) {
    /* connectivity is 0 (any), 1 (connected), 2 (biconnected) or 3 (triconnected) */
    assert(p>=0 && p<1 && connectivity>=0 && connectivity<4);
    set_directed(false);
    /* start with a random maximal planar graph with n vertices */
    int n=node_count();
    if (n==1)
        return;
    if (n==2) {
        add_edge(0,1);
        return;
    }
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
    /* make random triangulated graph with n vertices */
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
    if (p==0)
        return;
    int mindeg=std::max(2,connectivity+1);
    ipairs E;
    get_edges_as_pairs(E);
    ivector indices=rand_permu(E.size());
    /* try to remove each edge with probability 0<p<1 */
    for (ivector_iter it=indices.begin();it!=indices.end();++it) {
        ipair &e=E[*it];
        if (degree(e.first)<mindeg || degree(e.second)<mindeg)
            continue;
        if (rand_uniform()<p)
            remove_edge(e);
        switch (connectivity) {
        case 0:
            break;
        case 1:
            if (!is_connected())
                add_edge(e);
            break;
        case 2:
            if (!is_biconnected())
                add_edge(e);
            break;
        case 3:
            if (!is_triconnected())
                add_edge(e);
            break;
        default:
            assert(false); // unreachable
        }
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

/* create a random graph with the given degree sequence d which must be graphical */
void graphe::make_random_sequential(const ivector &d,const vecteur &labels) {
    assert(d.size()==labels.size() && d.size()>0);
    int s=0;
    for (ivector_iter it=d.begin();it!=d.end();++it) {
        s+=*it;
    }
    assert((s%2)==0);
    int m=s/2,n=d.size(),sz,cnt;
    double p,r;
    map<ipair,bool> used;
    map<int,pair<ipair,double> > emap;
    ivector D;
    ipair e;
    do {
        D=d;
        used.clear();
        cnt=0;
        while(true) {
            p=0;
            sz=0;
            int &i=e.first,&j=e.second;
            for (i=0;i<n;++i) {
                for (j=i+1;j<n;++j) {
                    if (used[e])
                        continue;
                    p+=D[i]*D[j]*(1-d[i]*d[j]/(4*m));
                    emap[sz++]=make_pair(e,p);
                }
            }
            if (p==0)
                break;
            r=rand_uniform()*p;
            for (int k=0;k<sz;++k) {
                pair<ipair,double> &elm=emap[k];
                if (r<=elm.second) {
                    const ipair &edge=elm.first;
                    used[edge]=true;
                    ++cnt;
                    --D[edge.first];
                    --D[edge.second];
                    break;
                }
            }
        }
    } while (cnt<m);
    add_nodes(labels);
    for (map<ipair,bool>::const_iterator it=used.begin();it!=used.end();++it) {
        if (it->second)
            add_edge(it->first);
    }
}

/* create a random bipartite graph with two groups of vertices V and W */
void graphe::make_random_bipartite(const vecteur &V,const vecteur &W,double p) {
    this->clear();
    set_directed(false);
    int a=V.size(),b=W.size(),m=std::floor(p),k;
    add_nodes(mergevecteur(V,W));
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
    int prob_total,k,dd;
    double r;
    ipair edge;
    add_nodes(V);
    do {
        if (connected)
            make_random_tree(V,d);
        else {
            for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
                it->clear_neighbors();
            }
        }
        for (int i=0;i<n;++i) {
            degrees[i]=degree(i);
        }
        E.clear();
        for (int i=0;i<n;++i) {
            if ((dd=degrees[i])<d) {
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
                prob_total+=(d-degrees[it->first])*(d-degrees[it->second]);
                prob[it-E.begin()]=prob_total;
            }
            k=0;
            if (prob_total>0) {
                r=rand_uniform()*prob_total;
                for (ivector_iter it=prob.begin();it!=prob.end();++it) {
                    if (r<=*it)
                        break;
                    ++k;
                }
            }
            edge=E[k];
            E.erase(E.begin()+k);
            ++degrees[edge.first];
            ++degrees[edge.second];
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
    int deg=d;
    for (int i=0;i<n;++i) {
        if (deg<0)
            deg=degree(i);
        else if (degree(i)!=deg)
            return false;
    }
    return true;
}

/* create a random flow network */
void graphe::make_random_flow_network(const vecteur &V,int capacity) {
    assert(!V.empty());
    add_nodes(V);
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
    get_edges_as_pairs(E1);
    G.get_edges_as_pairs(E2);
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
bool graphe::embed_rectangles(vector<rectangle> &rectangles,double maxheight) {
    bool embedding_changed=false;
    double old_x,old_y;
    dpair old_anchor;
    rectangle *old_rect;
    int lock_type;
    for (vector<rectangle>::iterator it=rectangles.begin()+1;it!=rectangles.end();++it) {
        rectangle &r=*it;
        old_x=r.x();
        old_y=r.y();
        r.set_anchor(DBL_MAX,-1);
        old_rect=NULL;
        for (vector<rectangle>::iterator jt=rectangles.begin();jt!=it;++jt) {
            rectangle &s=*jt;
            if (s.x()>r.x())
                continue;
            old_anchor.first=r.x();
            old_anchor.second=r.y();
            if (!s.is_locked_above()) {
                r.set_anchor(s.x(),s.y()+s.height());
                if (r.y()+r.height()<=maxheight && !r.intersects(rectangles.begin(),it)) {
                    s.set_locked_above(true);
                    if (old_rect!=NULL) {
                        if (lock_type==1)
                            old_rect->set_locked_above(false);
                        else if (lock_type==2)
                            old_rect->set_locked_right(false);
                    }
                    old_rect=&s;
                    lock_type=1;
                    continue;
                }
            }
            if (!s.is_locked_right()) {
                r.set_anchor(s.x()+s.width(),s.y());
                if (r.x()<old_anchor.first && r.y()+r.height()<=maxheight && !r.intersects(rectangles.begin(),it)) {
                    s.set_locked_right(true);
                    if (old_rect!=NULL) {
                        if (lock_type==1)
                            old_rect->set_locked_above(false);
                        else if (lock_type==2)
                            old_rect->set_locked_right(false);
                    }
                    old_rect=&s;
                    lock_type=2;
                    continue;
                }
            }
            r.set_anchor(old_anchor.first,old_anchor.second);
        }
        assert(r.y()>=0);
        if (old_x!=r.x() || old_y!=r.y())
            embedding_changed=true;
    }
    return embedding_changed;
}

/* pack rectangles (sorted by height) to an enclosing rectangle with minimal perimeter and wasted space */
void graphe::pack_rectangles(vector<rectangle> &rectangles) {
    /* compute total area occupied by the rectangles */
    double total_area=0;
    for (vector<rectangle>::const_iterator it=rectangles.begin();it!=rectangles.end();++it) {
        total_area+=it->width()*it->height();
    }
    /* step = the length of a negligible part of the shorter side of the smallest rectangle */
    double maxwidth=0,minwidth=DBL_MAX,minheight=rectangles.back().height();
    for (vector<rectangle>::const_iterator it=rectangles.begin();it!=rectangles.end();++it) {
        if (it->width()>maxwidth)
            maxwidth=it->width();
        if (it->width()<minwidth)
            minwidth=it->width();
    }
    double step=std::min(minwidth,minheight)*MARGIN_FACTOR;
    double bw=DBL_MAX,bh=rectangles.front().height(); // initial enclosing rectangle has an "unlimited" width
    double perim,best_perim=DBL_MAX,d;
    double xpos=0;
    dpairs best_embedding(rectangles.size());
    for (vector<rectangle>::iterator it=rectangles.begin();it!=rectangles.end();++it) {
        it->set_anchor(xpos,0);
        xpos+=it->width();
    }
    if (rectangles.size()<2)
        return;
    bool firstpass=true;
    clock_t start=clock();
    while (bw>maxwidth+step) { // loop breaks after a stacked embedding is obtained
        for (vector<rectangle>::iterator it=rectangles.begin();it!=rectangles.end();++it) {
            it->set_locked_above(false);
            it->set_locked_right(false);
        }
        if (firstpass || embed_rectangles(rectangles,bh)) {
            bw=bh=0;
            /* find the smallest enclosing rectangle containing the embedding */
            for (vector<rectangle>::const_iterator it=rectangles.begin();it!=rectangles.end();++it) {
                if ((d=it->x()+it->width())>bw)
                    bw=d;
                if ((d=it->y()+it->height())>bh)
                    bh=d;
            }
            /* find the embedding with the smallest perimeter (when scaled by the wasted ratio) */
            if ((perim=(bw+PLASTIC_NUMBER_SQUARED*bh)*std::sqrt(bw*bh/total_area))<best_perim) {
                best_perim=perim;
                for (vector<rectangle>::const_iterator it=rectangles.begin();it!=rectangles.end();++it) {
                    dpair &p=best_embedding[it-rectangles.begin()];
                    p.first=it->x();
                    p.second=it->y();
                }
            }
        }
        /* increase enclosing rectangle height (rectangles will end up stacked eventually) */
        bh+=step;
        firstpass=false;
        if (double(clock()-start)/CLOCKS_PER_SEC>1.5)
            break;
    }
    for (vector<rectangle>::iterator it=rectangles.begin();it!=rectangles.end();++it) {
        dpair &p=best_embedding[it-rectangles.begin()];
        it->set_anchor(p.first,p.second);
    }
}

/* return true iff an isomorphic copy with vertices permuted according to sigma is constructed */
bool graphe::isomorphic_copy(graphe &G,const ivector &sigma) {
    int n=node_count();
    assert(int(sigma.size())==n);
    G.set_name(name());
    G.register_user_tags(user_tags);
    G.set_graph_attributes(attributes);
    /* add vertices */
    G.reserve_nodes(n);
    for (ivector_iter it=sigma.begin();it!=sigma.end();++it) {
        G.add_node(node_label(*it),nodes[*it].attributes());
    }
    if (G.node_count()!=n)
        return false;
    /* add edges */
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
            /* edges intersect each other, obtain the intersection point */
            scale_point(rr,t);
            add_point(pp,rr);
            crossing.resize(2);
            crossing.front()=pp[0];
            crossing.back()=pp[1];
            return true;
        }
    }
    /* edges are non-intersecting */
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
int graphe::largest_integer_label() const {
    int n,m=array_start(ctx)-1;
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
* IMPLEMENTATION OF THE DISJOINT SET DATA STRUCTURE
*/

bool graphe::unionfind::is_stored(int id) {
    assert(id>=0 && id<int(elements.size()));
    for (vector<element>::const_iterator it=elements.begin();it!=elements.end();++it) {
        if (it->id==id)
            return true;
    }
    return false;
}

void graphe::unionfind::make_set(int id) {
    if (is_stored(id))
        return;
    element &e=elements[id];
    e.id=id;
    e.parent=id;
    e.rank=1;
}

int graphe::unionfind::find(int id) {
    assert(id>=0 && id<int(elements.size()));
    element &e=elements[id];
    assert(e.id>=0);
    if (e.parent!=id)
        e.parent=find(e.parent);
    return e.parent;
}

void graphe::unionfind::unite(int id1,int id2) {
    int p1=find(id1),p2=find(id2);
    element &x=elements[p1],&y=elements[p2];
    if (x.rank>y.rank)
        y.parent=x.id;
    else if (x.rank<y.rank)
        x.parent=y.id;
    else {
        y.parent=x.id;
        ++x.rank;
    }
}

/*
* END OF DISJOINT_SET CLASS
*/

/* make planar layout */
bool graphe::make_planar_layout(layout &x) {
    int n=node_count(),of,m;
    ivectors faces;
    faces.clear();
    /* create the faces (adding temporary edges if necessary),
 * return the index of the outer face */
    of=planar_embedding(faces);
    if (of<0)
        return false; // the graph is not planar
    /* subdivide the concave faces */
    augment(faces,of,false);
    ivector &outer_face=faces[of];
    m=outer_face.size();
    /* create a fake outer face */
    ivector fake_outer_face(m);
    int label=largest_integer_label();
    for (ivector_iter it=outer_face.begin();it!=outer_face.end();++it) {
        add_edge(*it,fake_outer_face[it-outer_face.begin()]=add_node(++label));
    }
    make_tutte_layout(x,fake_outer_face); // create the layout
    /* remove temporary vertices */
    while (node_count()>n) {
        remove_node(node_count()-1);
    }
    x.resize(n);
    remove_temporary_edges();
    return true;
}

/* get the convex hull of 2D graph layout */
void graphe::convex_hull(const layout &x,layout &hull) {
    vecteur v(x.size());
    for (iterateur it=v.begin();it!=v.end();++it) {
        const point &p=x[it-v.begin()];
        *it=makecomplex(p.front(),p.back());
    }
    vecteur h=*_convexhull(v,ctx)._VECTptr;
    hull.resize(h.size());
    for (const_iterateur it=h.begin();it!=h.end();++it) {
        point &p=hull[it-h.begin()];
        p.resize(2);
        p.front()=it->_CPLXptr->DOUBLE_val();
        p.back()=(it->_CPLXptr+1)->DOUBLE_val();
    }
}

/* rotate layout x such that left to right symmetry is exposed */
void graphe::layout_best_rotation(layout &x) {
    /* center layout in the origin */
    point center=layout_center(x);
    scale_point(center,-1);
    translate_layout(x,center);
    layout hull;
    convex_hull(x,hull);
    int n=hull.size();
    /* compute the hull perimeter */
    double perim=0,score,maxscore=-1;
    point tmp(2),mp(2),rp(2),rq(2),origin(2,0.0),proj(2);
    for (int i=0;i<n;++i) {
        const point &p=hull[i],&q=hull[(i+1)%n];
        perim+=point_distance(p,q,tmp);
    }
    double a,b,c,d,tol,angle=0,half_perim=perim/2.0;
    vector<bool> matched(hull.size());
    for (int i=0;i<n;++i) {
        const point &p=hull[i];
        for (int j=i+1;j<n;++j) {
            const point &q=hull[j];
            if (!point2segment_projection(origin,p,q,proj))
                continue;
            /* obtain the perpendicular bisector ax+by+c=0 of the segment pq */
            copy_point(p,mp);
            add_point(mp,q);
            scale_point(mp,0.5);
            if (p.front()==q.front()) {
                a=1;
                c=-mp.front();
                b=0;
            } else {
                a=(p.back()-q.back())/(p.front()-q.front());
                b=-1;
                c=mp.back()-a*mp.front();
            }
            /* try to match hull edges that are reflections of each other */
            score=0;
            std::fill(matched.begin(),matched.end(),false);
            for (layout_iter it=hull.begin();it!=hull.end();++it) {
                if (matched[it-hull.begin()])
                    continue;
                const point &p1=*it,&q1=it+1==hull.end()?hull.front():*(it+1);
                tol=(d=point_distance(p1,q1,tmp))*MARGIN_FACTOR/2.0;
                point_mirror(a,b,c,p1,rp);
                point_mirror(a,b,c,q1,rq);
                for (layout_iter jt=it;jt!=hull.end();++jt) {
                    if (matched[jt-hull.begin()])
                        continue;
                    const point &p2=*jt,&q2=jt+1==hull.end()?hull.front():*(jt+1);
                    if ((points_coincide(rp,p2,tol) && points_coincide(rq,q2,tol)) ||
                            (points_coincide(rp,q2,tol) && points_coincide(rq,p2,tol))) {
                        score+=d;
                        matched[it-hull.begin()]=true;
                        matched[jt-hull.begin()]=true;
                    }
                }
            }
            score/=half_perim;
            double bigpart=std::pow(PLASTIC_NUMBER,4); // hull symmetry is more important
            score*=bigpart;
            score+=1.0-point_distance(proj,mp,tmp)/point_distance(p,q,tmp);
            score/=bigpart+1;
            if (score>maxscore) {
                maxscore=score;
                angle=a==0?M_PI_2:std::atan(b/a);
            }
        }
    }
    rotate_layout(x,M_PI_2-angle);
    rectangle rect=layout_bounding_rect(x);
    if (rect.y()+rect.height()/2.0<0)
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

/* customize the Giac display */
gen customize_display(int options) {
    return symbolic(at_equal,makesequence(at_display,change_subtype(options,_INT_COLOR)));
}

/* append the line segment [p,q] to vecteur v */
void graphe::append_segment(vecteur &drawing,const point &p,const point &q,int color,int width,int style,bool arrow) const {
    vecteur attributs(1,color | width | style);
    vecteur seg=p.size()==2?
                makevecteur(makecomplex(p[0],p[1]),makecomplex(q[0],q[1]))
            : makevecteur(gen(makevecteur(p[0],p[1],p[2]),_POINT__VECT),gen(makevecteur(q[0],q[1],q[2]),_POINT__VECT));
    drawing.push_back(pnt_attrib(gen(seg,arrow?_VECTOR__VECT:_GROUP__VECT),attributs,ctx));
}

/* append the vertex (as a circle) to vecteur v */
void graphe::append_node(vecteur &drawing,const point &p,int color,int width,int shape) const {
    gen P=point2gen(p,true),args;
    args=makesequence(P,customize_display(color | width | shape));
    drawing.push_back(_point(args,ctx));
}

/* append label to vecteur v at the specified quadrant */
void graphe::append_label(vecteur &drawing,const point &p,const gen &label,int quadrant,int color) const {
    gen P=point2gen(p),args=makesequence(P,label,customize_display(quadrant | color));
    drawing.push_back(_legende(args,ctx));
}

/* convert gen to point coordinates */
bool graphe::gen2point(const gen &g,point &p) {
    if (g.type==_VECT || g.is_symb_of_sommet(at_point)) {
        vecteur &v=g.type==_VECT?*g._VECTptr:*g._SYMBptr->feuille._VECTptr;
        p.resize(v.size());
        for (const_iterateur it=v.begin();it!=v.end();++it) {
            if (!is_real_number(*it))
                return false;
            p[it-v.begin()]=it->DOUBLE_val();
        }
    } else { /* assuming that pos is a complex number */
        p.resize(2);
        if (g.type==_CPLX) {
            gen &real=*g._CPLXptr,&imag=*(g._CPLXptr+1);
            if (!is_real_number(real) || !is_real_number(imag))
                return false;
            p.front()=real.DOUBLE_val();
            p.back()=imag.DOUBLE_val();
        } else {
            if (!is_real_number(g))
                return false;
            p.front()=g.DOUBLE_val();
            p.back()=0;
        }
    }
    return true;
}

/* extract position attribute from attr */
bool graphe::get_node_position(const attrib &attr,point &p) {
    attrib_iter it=attr.find(_GT_ATTRIB_POSITION);
    if (it==attr.end())
        return false;
    return gen2point(it->second,p);
}

/* append line segments representing edges (arcs) of the graph to vecteur v */
void graphe::draw_edges(vecteur &drawing,const layout &x) {
    if (x.empty())
        return;
    int i,j,color,width,style;
    bool isdir=is_directed();
    double d;
    point r(x.front().size());
    attrib_iter ait;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        i=it-nodes.begin();
        const point &p=x[i];
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            j=*jt;
            if (!isdir && j<i)
                continue;
            const point &q=x[j];
            const attrib &attr=it->neighbor_attributes(j);
            color=default_edge_color;
            if ((ait=attr.find(_GT_ATTRIB_COLOR))!=attr.end() && ait->second.is_integer()) {
                color=ait->second.val;
                if (color==7)
                    color=0; /* draw white (invisible) edges as black */
            } else if ((ait=attr.find(_GT_ATTRIB_TEMPORARY))!=attr.end() && is_one(ait->second))
                color=29; // gray
            style=0;
            width=default_edge_width;
            if ((ait=attr.find(_GT_ATTRIB_STYLE))!=attr.end()) {
                const gen &val=ait->second;
                string s=val.type==_STRNG?genstring2str(val):gen2str(val);
                if (s=="dashed")
                    style=_DASH_LINE;
                else if (s=="dotted")
                    style=_DOT_LINE;
                else if (s=="bold")
                    width=bold_edge_width;
            }
            if (isdir) {
                assert((ait=attr.find(_GT_ATTRIB_POSITION))!=attr.end());
                d=ait->second.DOUBLE_val();
                point_lincomb(p,q,d,1-d,r);
                append_segment(drawing,p,r,color,width,style,true);
                append_segment(drawing,r,q,color,width,style,false);
            } else
                append_segment(drawing,p,q,color,style,width);
        }
    }
}

/* append points representing vertices of the graph to vecteur v */
void graphe::draw_nodes(vecteur &drawing,const layout &x) const {
    if (x.empty())
        return;
    int color,width,n=node_count();
    if (n<=30)
        width=_POINT_WIDTH_4;
    else if (n<=130)
        width=_POINT_WIDTH_3;
    else if (n<=330)
        width=_POINT_WIDTH_2;
    else
        width=_POINT_WIDTH_1;
    int shape;
    attrib_iter ait;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        const attrib &attr=it->attributes();
        const point &p=x[it-nodes.begin()];
        color=default_vertex_color;
        if ((ait=attr.find(_GT_ATTRIB_COLOR))!=attr.end() && ait->second.is_integer()) {
            color=ait->second.val;
            if (color==7)
                color=0; // draw white (invisible) nodes as black
        }
        shape=_POINT_POINT;
        if ((ait=attr.find(_GT_ATTRIB_SHAPE))!=attr.end()) {
            const gen &val=ait->second;
            string s=val.type==_STRNG?genstring2str(val):gen2str(val);
            if (s=="box" || s=="square")
                shape=_POINT_CARRE;
            else if (s=="triangle")
                shape=_POINT_TRIANGLE;
            else if (s=="diamond")
                shape=_POINT_LOSANGE;
            else if (s=="star")
                shape=_POINT_ETOILE;
            else if (s=="plus")
                shape=_POINT_PLUS;
        }
        append_node(drawing,p,color,width,shape);
    }
}

/* return the best quadrant for the placement of the i-th vertex label
* (minimize the collision with the adjacent edges) */
int graphe::best_quadrant(const point &p,const layout &adj) const {
    int bestq,n=adj.size();
    if (n==0 || p.size()!=2)
        return _QUADRANT1;
    layout quad(4);
    quad[0]=make_point(0.7071,0.7071);
    quad[1]=make_point(-0.7071,0.7071);
    quad[2]=make_point(-0.7071,-0.7071);
    quad[3]=make_point(0.7071,-0.7071);
    vector<double> min_angular_dist(4,M_PI);
    point u(2);
    double a;
    for (int i=0;i<4;++i) {
        const point &q=quad[i];
        double &mindist=min_angular_dist[i];
        for (layout_iter it=adj.begin();it!=adj.end();++it) {
            scale_point(u,1/point_distance(p,*it,u));
            a=std::acos(point_dotprod(u,q));
            if (a<mindist)
                mindist=a;
        }
    }
    a=-1;
    for (int i=0;i<4;++i) {
        const double &dist=min_angular_dist[i];
        if (a<0 || dist>a) {
            a=dist;
            bestq=i;
        }
    }
    switch (bestq) {
    case 0: return _QUADRANT1;
    case 1: return _QUADRANT2;
    case 2: return _QUADRANT3;
    case 3: return _QUADRANT4;
    default: break;
    }
    /* unreachable */
    return -1;
}

/* append labels of the nodes of this graph to vecteur v */
void graphe::draw_labels(vecteur &drawing,const layout &x) const {
    if (is_empty())
        return;
    assert(!x.empty());
    ivector adjv;
    layout adj(2);
    attrib_iter ait;
    double d;
    int color,n=node_count();
    point r(x.front().size());
    bool isdirected=is_directed(),isweighted=is_weighted();
    if (isweighted && x.front().size()==2) {
        /* draw weight labels */
        for (int i=0;i<n;++i) {
            const point &p=x[i];
            const vertex &v=node(i);
            for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
                if (!isdirected && *it<i)
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
                append_label(drawing,r,ait->second,best_quadrant(r,adj),color);
            }
        }
    }
    /* draw vertex labels */
    for (int i=0;i<n;++i) {
        const point &p=x[i];
        adjacent_nodes(i,adjv);
        adj.resize(adjv.size());
        for (ivector_iter it=adjv.begin();it!=adjv.end();++it) {
            adj[it-adjv.begin()]=x[*it];
        }
        append_label(drawing,p,nodes[i].label(),best_quadrant(p,adj));
    }
}

void graphe::rdfs(int i,ivector &d,bool rec,int sg,bool skip_embedded) {
    vertex &v=node(i);
    v.set_visited(true);
    v.set_disc(++disc_time);
    v.set_low(v.disc());
    v.clear_children();
    if (rec)
        d.push_back(i);
    int j;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        j=*it;
        vertex &w=node(j);
        if ((sg>=0 && w.subgraph()!=sg) || (skip_embedded && w.is_embedded()))
            continue;
        if (!w.is_visited()) {
            w.set_ancestor(i);
            v.inc_children();
            rdfs(j,d,rec,sg,skip_embedded);
            v.set_low(std::min(v.low(),w.low()));
        } else if (j!=v.ancestor())
            v.set_low(std::min(v.low(),w.disc()));
    }
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
    assert(sg<0 || node(root).subgraph()==sg);
    rdfs(root,d,rec,sg,skip_embedded);
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
    assert(node_queue.empty());
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
    assert(!is_empty());
    if (is_directed()) {
        graphe G(ctx);
        underlying(G);
        return G.is_connected(sg);
    }
    int i=sg<0?0:first_vertex_from_subgraph(sg);
    assert(i>=0);
    dfs(i,false,true,NULL,sg);
    for (node_iter it=nodes.begin()+i;it!=nodes.end();++it) {
        if ((sg<0 || it->subgraph()==sg) && !it->is_visited())
            return false;
    }
    return true;
}

/* return true iff the graph is biconnected */
bool graphe::is_biconnected(int sg) {
    assert(!is_empty());
    if (is_directed()) {
        graphe G(ctx);
        underlying(G);
        return G.is_biconnected(sg);
    }
    return is_connected(sg) && !has_cut_vertex(sg);
}

/* return true iff the graph is triconnected, using a simple O(n*(n+m)) algorithm */
bool graphe::is_triconnected(int sg) {
    assert(!is_empty());
    if (is_directed()) {
        graphe G(ctx);
        underlying(G);
        return G.is_triconnected(sg);
    }
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
bool graphe::is_tournament() const {
    int n=node_count();
    if (!is_directed() || edge_count()!=n*(n-1)/2)
        return false;
    for (int i=0;i<n;++i) {
        for (int j=0;j<n;++j) {
            if (i==j)
                continue;
            if (has_edge(i,j)==has_edge(j,i))
                return false;
        }
    }
    return true;
}

/* return true iff the graph is planar */
bool graphe::is_planar() {
    ivectors components,faces;
    int m;
    connected_components(components);
    for (ivectors_iter it=components.begin();it!=components.end();++it) {
        if (it->size()<5)
            continue;
        graphe G(ctx);
        induce_subgraph(*it,G,false);
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
    P.reserve_nodes(n*m);
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
    G.reserve_nodes(node_count());
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        G.add_node(it->label(),it->attributes());
    }
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        for (ivector_iter jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
            G.add_edge(*jt,it-nodes.begin(),it->neighbor_attributes(*jt));
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
    unionfind ds(node_count());
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
    subgraph(res,T,true);
}

/* Tarjan's offline algorithm for the lowest common ancestor, time complexity O(n) */
void graphe::lca_recursion(int u,const ipairs &p,ivector &lca,unionfind &ds) {
    ds.make_set(u);
    vertex &U=node(u);
    U.set_ancestor(u);
    U.set_visited(true);
    int v;
    for (ivector_iter it=U.neighbors().begin();it!=U.neighbors().end();++it) {
        v=*it;
        vertex &V=node(v);
        if (V.is_visited())
            continue;
        lca_recursion(v,p,lca,ds);
        ds.unite(u,v);
        node(ds.find(u)).set_ancestor(u);
    }
    U.set_color(1); // black
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        v=it-nodes.begin();
        if (u==v)
            continue;
        for (ipairs_iter jt=p.begin();jt!=p.end();++jt) {
            if ((jt->first==u && jt->second==v) || (jt->first==v && jt->second==u)) {
                if (it->color()==1)
                    lca[jt-p.begin()]=node(ds.find(v)).ancestor();
                break;
            }
        }
    }
}

/* find the lowest common ancestors for all pairs of vertices in p (this must be a tree) */
void graphe::lowest_common_ancestors(int root,const ipairs &p,ivector &lca) {
    unvisit_all_nodes();
    unset_all_ancestors();
    uncolor_all_nodes();
    lca.resize(p.size(),-1);
    unionfind ds(node_count());
    lca_recursion(root,p,lca,ds);
    assert(find(lca.begin(),lca.end(),-1)==lca.end());
}

/* return the lowest common ancestor of i-th node and j-th node of this tree */
int graphe::lowest_common_ancestor(int i,int j,int root) {
    ipairs p;
    p.push_back(make_pair(i,j));
    ivector lca;
    lowest_common_ancestors(root,p,lca);
    return lca.front();
}

/* return true iff v is a descendant of anc wrt the last dfs traversal */
bool graphe::is_descendant(int v,int anc) const {
    int i=v;
    while ((i=node(i).ancestor())>=0) {
        if (i==anc)
            return true;
    }
    return false;
}

/* find a path from v to other visited vertex w along unvisited edges */
void graphe::pathfinder(int i,ivector &path) {
    const vertex &v=node(i);
    path.clear();
    /* find an unvisited edge from v */
    int j=-1,k;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        if (!is_edge_visited(i,*it)) {
            j=*it;
            break;
        }
    }
    if (j<0)
        return;
    vertex &w=node(j);
    int mode=0;
    if (v.ancestor()!=j && w.ancestor()!=i) {
        if (is_descendant(i,j))
            mode=1;
        else if (is_descendant(j,i))
            mode=2;
    } else if (w.ancestor()==i)
        mode=3;
    if (mode>0) {
        set_edge_visited(i,j);
        path.push_back(i);
        path.push_back(j);
    }
    if (mode>1) {
        while (true) {
            vertex &u=node(j);
            if (u.is_visited())
                break;
            k=-1;
            for (ivector_iter it=u.neighbors().begin();it!=u.neighbors().end();++it) {
                vertex &t=node(*it);
                if (!is_edge_visited(j,*it) && ((mode==2 && u.ancestor()==*it) ||
                                                (mode==3 && (t.disc()==u.low() || t.low()==u.low())))) {
                    k=*it;
                    break;
                }
            }
            assert(k>=0);
            u.set_visited(true);
            set_edge_visited(j,k);
            path.push_back(k);
            j=k;
        }
    }
}

/* compute the ST numbering for this graph using Even-Tarjan algorithm, time complexity O(n+m) */
void graphe::compute_st_numbering(int s,int t) {
    /* assuming that the graph is biconnected */
    assert(has_edge(s,t) && node_stack.empty());
    vertex &v=node(s),&w=node(t);
    v.move_neighbor(t,-1);
    dfs(s,false);
    unvisit_all_nodes();
    unvisit_all_edges();
    v.set_visited(true);
    w.set_visited(true);
    set_edge_visited(make_pair(s,t));
    node_stack.push(t);
    node_stack.push(s);
    int n=0,i;
    ivector path;
    while (!node_stack.empty()) {
        i=node_stack.top();
        vertex &u=node(i);
        node_stack.pop();
        pathfinder(i,path);
        if (path.empty()) {
            u.set_number(++n);
        } else {
            path.pop_back();
            while (!path.empty()) {
                node_stack.push(path.back());
                path.pop_back();
            }
        }
    }
}

/* return st-numbering in form of vecteur */
vecteur graphe::get_st_numbering() const {
    /* assuming that st numbering has been computed */
    vecteur st(node_count());
    int ofs=1-array_start(ctx);
    for (iterateur it=st.begin();it!=st.end();++it) {
        *it=node(it-st.begin()).number()-ofs;
    }
    return st;
}

/* assign directions to edges according to the st-numbering */
void graphe::assign_edge_directions_from_st() {
    set_directed(true);
    int j;
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        vertex &v=*it;
        for (int i=v.neighbors().size();i-->0;) {
            j=v.neighbors().at(i);
            const vertex &w=node(j);
            if (v.number()>w.number())
                v.remove_neighbor(j);
        }
    }
}

/* greedy vertex coloring algorithm due to Biggs, also records vertex inclusion time */
void graphe::greedy_vertex_coloring_biggs(ivector &ordering) {
    uncolor_all_nodes();
    int n=node_count(),k=0,i,maxdeg,d,col=0;
    ordering.resize(n);
    ivector_iter jt;
    while (k<n) {
        ++col;
        do {
            maxdeg=-1;
            i=-1;
            for (node_iter it=nodes.begin();it!=nodes.end();++it) {
                if (it->color()>0)
                    continue;
                for (jt=it->neighbors().begin();jt!=it->neighbors().end();++jt) {
                    if (node(*jt).color()==col)
                        break;
                }
                if (jt==it->neighbors().end() && (maxdeg<0 || (d=it->neighbors().size())>maxdeg)) {
                    maxdeg=d;
                    i=it-nodes.begin();
                }
            }
            if (i>=0) {
                node(i).set_color(col);
                ordering[k++]=i;
            }
        } while (i>=0);
    }
}

/* classical greedy vertex coloring algorithm, time complexity O(n+m) */
int graphe::greedy_vertex_coloring(const ivector &p) {
    if (is_directed()) {
        graphe G(ctx);
        underlying(G);
        return G.greedy_vertex_coloring(p);
    }
    uncolor_all_nodes();
    int c=0,k;
    std::set<int> used;
    for (ivector_iter it=p.begin();it!=p.end();++it) {
        vertex &v=node(*it);
        if (c==0) {
            v.set_color(++c);
            continue;
        }
        used.clear();
        for (ivector_iter jt=v.neighbors().begin();jt!=v.neighbors().end();++jt) {
            vertex &v=node(*jt);
            if (v.color()>0)
                used.insert(v.color());
        }
        k=1;
        for (std::set<int>::const_iterator jt=used.begin();jt!=used.end();++jt) {
            if (*jt>k)
                break;
            ++k;
        }
        v.set_color(k);
        if (k>c)
            c=k;
    }
    return c;
}

/* extract colors of the vertices and return them in order */
void graphe::get_node_colors(ivector &colors) {
    colors.resize(node_count());
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        colors[it-nodes.begin()]=it->color();
    }
}

/* return true iff the graph is bipartite, time complexity O(n+m) */
bool graphe::is_bipartite(ivector &V1,ivector &V2,int sg) {
    assert(node_queue.empty());
    if (is_directed()) {
        graphe G(ctx);
        underlying(G);
        return G.is_bipartite(V1,V2,sg);
    }
    uncolor_all_nodes(-1,sg);
    node(0).set_color(1);
    node_iter nt=nodes.begin();
    for (;nt!=nodes.end();++nt) {
        if (sg<0 || nt->subgraph()==sg)
            break;
    }
    assert(nt!=nodes.end());
    node_queue.push(nt-nodes.begin());
    int i;
    while (!node_queue.empty()) {
        i=node_queue.front();
        node_queue.pop();
        vertex &v=node(i);
        for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
            vertex &w=node(*it);
            if (sg>=0 && w.subgraph()!=sg)
                continue;
            if (w.color()==-1) {
                w.set_color(1-v.color());
                node_queue.push(*it);
            } else if (w.color()==v.color()) {
                clear_node_queue();
                return false;
            }
        }
    }
    V1.clear();
    V2.clear();
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (sg>=0 && it->subgraph()!=sg)
            continue;
        if (it->color()==1)
            V1.push_back(it-nodes.begin());
        else V2.push_back(it-nodes.begin());
    }
    return true;
}

/* place vertices from partitions v1 and v2 on two parallel lines */
void graphe::make_bipartite_layout(layout &x,const ivector &p1,const ivector &p2) {
    double aspect_ratio;
    int n1=p1.size(),n2=p2.size(),n=std::max(n1,n2);
    assert(n1>1 && n2>1);
    if (n<3)
        aspect_ratio=1.0;
    else if (n<5)
        aspect_ratio=PLASTIC_NUMBER;
    else if (n<8)
        aspect_ratio=PLASTIC_NUMBER_SQUARED;
    else
        aspect_ratio=PLASTIC_NUMBER_CUBED;
    double step1=aspect_ratio/(double)(n1-1),step2=aspect_ratio/(double)(n2-1),xpos=0.0,ypos=1.0;
    x.resize(node_count());
    for (ivector_iter it=p1.begin();it!=p1.end();++it) {
        point &p=x[*it];
        p.resize(2);
        p.front()=xpos;
        p.back()=ypos;
        xpos+=step1;
    }
    xpos=ypos=0.0;
    for (ivector_iter it=p2.begin();it!=p2.end();++it) {
        point &p=x[*it];
        p.resize(2);
        p.front()=xpos;
        p.back()=ypos;
        xpos+=step2;
    }
}

/* construct the plane dual of a planar graph with the given faces with time complexity O(n),
* each face must be a list of vertex indices */
void graphe::make_plane_dual(const ivectors &faces) {
    this->clear();
    set_directed(false);
    vecteur labels;
    make_default_labels(labels,faces.size());
    add_nodes(labels);
    int nc=0;
    for (ivectors_iter it=faces.begin();it!=faces.end();++it) {
        for (ivector_iter jt=it->begin();jt!=it->end();++jt) {
            if (*jt>nc)
                nc=*jt;
        }
    }
    edgemap emap(++nc);
    map<int,int>::const_iterator et;
    int i,j,v,w,f,n;
    for (ivectors_iter it=faces.begin();it!=faces.end();++it) {
        f=it-faces.begin();
        const ivector &face=*it;
        n=face.size();
        for (int k=0;k<n;++k) {
            i=face[k];
            j=face[(k+1)%n];
            v=i<j?i:j;
            w=i<j?j:i;
            map<int,int> &m=emap[v];
            if ((et=m.find(w))==m.end())
                m[w]=f;
            else add_edge(f,et->second);
        }
    }
}

/* return the number of different colors adjacent to the i-th vertex in a partially colored graph */
int graphe::saturation_degree(const vertex &v,std::set<int> &colors) const {
    int i,c;
    colors.clear();
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        i=*it;
        const vertex &w=node(i);
        if ((c=w.color())>0)
            colors.insert(c);
    }
    return colors.size();
}

/* return the degree of v in the uncolored subgraph of this graph */
int graphe::uncolored_degree(const vertex &v) const {
    int deg=0;
    for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
        if (node(*it).color()==0)
            ++deg;
    }
    return deg;
}

/* return true iff this graph has at least one uncolored (white) vertex */
bool graphe::is_partially_colored() const {
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->color()==0)
            return true;
    }
    return false;
}

/* a heuristic algorithm by D.Bre1az for nearly optimal vertex coloring (time complexity O(n*m)),
* operates on a partially colored graph, returns the number of colors */
void graphe::dsatur() {
    int col,i,sat,maxsat,deg,maxdeg=0;
    std::set<int> colors,maxcolors;
    ivector indices;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->color()==0)
            indices.push_back(it-nodes.begin());
    }
    if (indices.empty())
        return;
    ivector::iterator pos;
    do {
        maxsat=-1;
        i=-1;
        for (ivector::iterator it=indices.begin();it!=indices.end();++it) {
            const vertex &v=node(*it);
            if ((sat=saturation_degree(v,colors))>maxsat ||
                    (sat==maxsat && (deg=uncolored_degree(v))>maxdeg)) {
                if (sat==maxsat)
                    maxdeg=deg;
                maxsat=sat;
                maxcolors=colors;
                i=*it;
                pos=it;
            }
        }
        if (i>=0) {
            col=1;
            for (std::set<int>::const_iterator it=maxcolors.begin();it!=maxcolors.end();++it) {
                if (*it==col)
                    ++col;
                else break;
            }
            node(i).set_color(col);
            indices.erase(pos);
        }
    } while (i>=0);
}

/* return the total number of different nonzero vertex colors in this graph */
int graphe::color_count() const {
    std::set<int> colors;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        colors.insert(it->color());
    }
    return colors.size();
}

/* return true iff the vertices can be colored using at most k different colors,
* the vertices will be colored after the function returns */
bool graphe::is_vertex_colorable(int k) {
    assert(k>=0);
    if (k==0) {
        uncolor_all_nodes();
        return true;
    }
    if (k>node_count()) {
        message("Warning: there are more colors than vertices");
        return false;
    }
    /* try greedy coloring first (linear time), use random order of vertices */
    ivector sigma=rand_permu(node_count());
    if (greedy_vertex_coloring(sigma)<=k)
        return true;
    /* next try dsatur algorithm (quadratic time) */
    uncolor_all_nodes();
    dsatur();
    if (color_count()<=k)
        return true;
    /* finally resort to solving MIP problem */
    return exact_vertex_coloring(k)!=0;
}

/* return the lower and the upper bound for chromatic number (inclusive) */
graphe::ipair graphe::chromatic_number_bounds() {
    /* the lower bound is the size of maximum clique */
    ivector clique;
    ostergard ost(this,3.0); // with timeout
    int lb=ost.maxclique(clique);
    /* the upper bound is given by heuristic dsatur algorithm */
    uncolor_all_nodes();
    for (ivector_iter it=clique.begin();it!=clique.end();++it) {
        node(*it).set_color(it-clique.begin()+1);
    }
    dsatur();
    int ub=color_count();
    return make_pair(lb,ub);
}

/* store custom vertex coordinates */
void graphe::store_layout(const layout &x) {
    assert(int(x.size())>=node_count());
    for (layout_iter it=x.begin();it!=x.end();++it) {
        vertex &v=node(it-x.begin());
        v.set_attribute(_GT_ATTRIB_POSITION,point2gen(*it));
    }
}

/* retrieve the previously stored layout, return true iff successful */
bool graphe::has_stored_layout(layout &x) const {
    x.resize(node_count());
    attrib_iter ait;
    int dim=0;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        const attrib &attr=it->attributes();
        point &p=x[it-nodes.begin()];
        if ((ait=attr.find(_GT_ATTRIB_POSITION))==attr.end() ||
                !gen2point(ait->second,p) || (dim>0 && int(p.size())!=dim))
            return false;
        if (dim==0)
            dim=p.size();
    }
    return true;
}

bool graphe::bipartite_matching_bfs(ivector &dist) {
    assert(node_queue.empty());
    int u,v;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->color()!=1)
            continue;
        u=it-nodes.begin();
        if (it->number()==0) {
            dist[u+1]=0;
            node_queue.push(u);
        } else dist[u+1]=RAND_MAX;
    }
    dist.front()=RAND_MAX;
    while (!node_queue.empty()) {
        u=node_queue.front();
        node_queue.pop();
        if (dist[u+1]<dist.front()) {
            vertex &U=node(u);
            for (ivector_iter it=U.neighbors().begin();it!=U.neighbors().end();++it) {
                v=*it;
                vertex &V=node(v);
                if (dist[V.number()]==RAND_MAX) {
                    dist[V.number()]=dist[u+1]+1;
                    node_queue.push(V.number()-1);
                }
            }
        }
    }
    return dist.front()!=RAND_MAX;
}

bool graphe::bipartite_matching_dfs(int u,ivector &dist) {
    if (u>0) {
        int v;
        vertex &U=node(u-1);
        for (ivector_iter it=U.neighbors().begin();it!=U.neighbors().end();++it) {
            v=*it;
            vertex &V=node(v);
            if (dist[V.number()]==dist[u]+1) {
                if (bipartite_matching_dfs(V.number(),dist)) {
                    V.set_number(u);
                    U.set_number(v+1);
                    return true;
                }
            }
        }
        dist[u]=RAND_MAX;
        return false;
    }
    return true;
}

/* obtain maximum matching in bipartite graph using Hopcroft-Karp algorithm */
int graphe::bipartite_matching(const ivector &p1,const ivector &p2,ipairs &matching) {
    for (vector<vertex>::iterator it=nodes.begin();it!=nodes.end();++it) {
        it->set_number(0);
    }
    for (ivector_iter it=p1.begin();it!=p1.end();++it) {
        set_node_color(*it,1);
    }
    for (ivector_iter it=p2.begin();it!=p2.end();++it) {
        set_node_color(*it,2);
    }
    ivector dist(node_count()+1);
    int count=0,u,v;
    while (bipartite_matching_bfs(dist)) {
        for (node_iter it=nodes.begin();it!=nodes.end();++it) {
            if (it->color()!=1)
                continue;
            u=it-nodes.begin();
            if (it->number()==0 && bipartite_matching_dfs(u+1,dist))
                ++count;
        }
    }
    /* extract matching */
    matching.clear();
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if (it->color()!=1)
            continue;
        u=it-nodes.begin();
        v=it->number();
        if (v>0)
            matching.push_back(make_pair(std::min(u,v-1),std::max(u,v-1)));
    }
    return count; // size of the matching
}

/* construct the line graph of this graph */
void graphe::line_graph(graphe &G,ipairs &E) const {
    if (E.empty())
        get_edges_as_pairs(E);
    G.clear();
    vecteur labels;
    gen label;
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        label=_cat(makesequence(node_label(it->first),str2gen("-",true),node_label(it->second)),ctx);
        labels.push_back(label);
    }
    G.add_nodes(labels);
    int i,j;
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        i=it-E.begin();
        for (ipairs_iter jt=it+1;jt!=E.end();++jt) {
            j=jt-E.begin();
            if (edges_incident(*it,*jt))
                G.add_edge(i,j);
        }
    }
}

/* construct the transitive closure of this graph */
void graphe::transitive_closure(graphe &G,bool weighted) {
    int n=node_count();
    bool isdir=is_directed(),iswei=is_weighted();
    G.clear();
    G.set_directed(isdir);
    G.set_weighted(weighted);
    G.add_nodes(vertices());
    if (weighted) {
        matrice m;
        gen wgh;
        ivector dist,J(n-1);
        int cnt;
        if (iswei)
            allpairs_distance(m);
        for (int i=0;i<n;++i) {
            if (!iswei) {
                cnt=0;
                for (int k=0;k<n;++k) {
                    if (k!=i)
                        J[cnt++]=k;
                }
                distance(i,J,dist);
            }
            for (int j=isdir?0:i+1;j<n;++j) {
                if (i==j)
                    continue;
                if ((iswei && !is_inf(wgh=m[i][j])) ||
                        (!iswei && is_positive(wgh=dist[j<i?j:j-1],ctx)))
                    G.add_edge(i,j,wgh);
            }
        }
    } else {
        for (int i=0;i<n;++i) {
            dfs(i,false);
            for (int j=isdir?0:i+1;j<n;++j) {
                if (i==j)
                    continue;
                if (node(j).is_visited())
                    G.add_edge(i,j);
            }
        }
    }
}

/* return true iff this graph is isomorphic to other, also obtain an isomorphism */
int graphe::is_isomorphic(const graphe &other,map<int,int> &isom) const {
#if defined HAVE_LIBNAUTY && defined HAVE_NAUTY_NAUTUTIL_H
    assert(is_directed()==other.is_directed());
    int n=node_count();
    if (other.node_count()!=n)
        return 0;
    int *adj1=to_array();
    int *adj2=other.to_array();
    int *sigma=new int[n];
    bool res=nautywrapper_is_isomorphic(is_directed()?1:0,n,adj1,adj2,sigma)!=0;
    if (res) {
        /* obtain the isomorphism */
        isom.clear();
        for (int i=0;i<n;++i) {
            isom[i]=sigma[i];
        }
    }
    delete[] adj1;
    delete[] adj2;
    delete[] sigma;
    return res;
#else
    message("Error: nauty library is required for finding graph isomorphism");
    return -1;
#endif
}

/* convert a sequence of nd digits to nonnegative integer */
int digits2int(char *digits,int nd) {
    int n=1,res=0;
    for (int i=nd;i-->0;) {
        res+=*(digits+i)*n;
        n*=10;
    }
    return res;
}

/* return the set of generators of the automorphism group of this graph */
gen graphe::aut_generators() const {
#if defined HAVE_LIBNAUTY && defined HAVE_NAUTY_NAUTUTIL_H
    int n=node_count(),ofs=array_start(ctx);
    vecteur out(0);
    if (n>0) {
        int *adj=to_array();
        FILE *f=tmpfile();
        if (f==NULL) {
            message ("Error: failed to create temporary file");
            return undef;
        }
        nautywrapper_aut_generators(is_directed()?1:0,n,adj,f);
        /* parse the generators and output them as a sequence of
     * permutations, each in form of a list of disjoint cycles */
        int sz=ftell(f);
        rewind(f);
        int i=0,nd=0;
        char c;
        vecteur perm,cycle;
        char digits[32];
        for (int cnt=0;cnt<sz;++cnt) {
            c=fgetc(f);
            if (c=='(')
                cycle.clear();
            else if (c==')') {
                cycle.push_back(ofs+digits2int(digits,nd));
                perm.push_back(cycle);
                nd=0;
            } else if (c=='\n') {
                out.push_back(gen(perm,_LIST__VECT));
                perm.clear();
            } else if (c>47 && c<58)
                digits[nd++]=c-48;
            else if (c==32) {
                cycle.push_back(ofs+digits2int(digits,nd));
                nd=0;
            }
            ++i;
        }
        fclose(f);
        delete[] adj;
    }
    return gen(out,_SET__VECT);
#else
    message("Error: nauty library is required for finding graph automorphisms");
    return undef;
#endif
}

/* return the canonical labeling of this graph as a permutation */
bool graphe::canonical_labeling(ivector &lab) const {
#if defined HAVE_LIBNAUTY && defined HAVE_NAUTY_NAUTUTIL_H
    int n=node_count();
    if (n==0)
        return false;
    int *adj=to_array();
    int *clab=new int[n];
    nautywrapper_canonical(is_directed()?1:0,n,adj,clab);
    lab.resize(n);
    for (int i=0;i<n;++i) {
        lab[i]=clab[i];
    }
    delete[] adj;
    delete[] clab;
    return true;
#else
    message("Error: nauty library is required for canonical labeling");
    return false;
#endif
}

/* return 1 if the graph is Hamiltonian, i.e. if it has a Hamiltonian circuit,
* else return 0 or -1 of not conclusive */
int graphe::is_hamiltonian(bool conclusive) {
    assert(!is_directed());
    if (!is_biconnected())
        return 0;
    int mindeg=RAND_MAX,deg;
    for (node_iter it=nodes.begin();it!=nodes.end();++it) {
        if ((deg=it->neighbors().size())<mindeg)
            mindeg=deg;
    }
    int n=node_count();
    if (2*mindeg>=n)
        return 1;
    ivector v;
    if (2*maximum_independent_set(v)>n)
        return 0;
    v.clear();
    double cost;
    return conclusive?find_hamiltonian_cycle(v,cost):-1;
}

/*
* TSP CLASS IMPLEMENTATION
*/

#ifdef HAVE_LIBGLPK

/* MINIMAL CUT implementation, adapted from glpk/examples/tsp (maxflow.c, mincut.c and main.c)
 * Originally written by Andrew Makhorin <mao@gnu.org>
 */

/* compute maximal flow */
int graphe::tsp::max_flow(int nn,int nedg,const ivector &beg,
        const ivector &end,const ivector &cap,int s,int t,
        ivector &x) {
    int k;
    assert(nn>=2);
    assert(nedg>=0);
    assert(1<=s && s<=nn);
    assert(1<=t && t<=nn);
    assert(s!=t);
    for (k=0; k<nedg; k++) {
        assert(1<=beg[k] && beg[k]<end[k] && end[k]<=nn && cap[k]>0);
    }
    glp_prob *lp;
    glp_smcp smcp;
    int i,nz,flow,*rn,*cn;
    double temp,*aa;
    lp=glp_create_prob();
    glp_add_rows(lp,nn);
    for (i=1;i<=nn;++i) {
        glp_set_row_bnds(lp,i,GLP_FX,0.0,0.0);
    }
    glp_add_cols(lp,nedg+1);
    for (k=0; k<nedg; k++) {
        glp_set_col_bnds(lp,k+1,GLP_DB,-cap[k],+cap[k]);
    }
    glp_set_col_bnds(lp,nedg+1,GLP_FR,0.0,0.0);
    rn=new int[1+2*(nedg+1)];
    cn=new int[1+2*(nedg+1)];
    aa=new double[1+2*(nedg+1)];
    nz=0;
    for (k=0;k<nedg;k++) {
        rn[++nz]=beg[k]; cn[nz]=k+1; aa[nz]=-1.0;
        rn[++nz]=end[k]; cn[nz]=k+1; aa[nz]=+1.0;
    }
    rn[++nz]=t; cn[nz]=nedg+1; aa[nz]=-1.0;
    rn[++nz]=s; cn[nz]=nedg+1; aa[nz]=+1.0;
    assert(nz==2*(nedg+1));
    glp_load_matrix(lp,nz,rn,cn,aa);
    delete[] rn;
    delete[] cn;
    delete[] aa;
    glp_set_obj_dir(lp,GLP_MAX);
    glp_set_obj_coef(lp,nedg+1,1.0);
    glp_term_out(0);
    glp_adv_basis(lp,0);
    glp_term_out(1);
    glp_init_smcp(&smcp);
    smcp.msg_lev=GLP_MSG_OFF;
    assert(glp_simplex(lp,&smcp)==0);
    assert(glp_get_status(lp)==GLP_OPT);
    for (k=0;k<nedg;k++) {
        temp=glp_get_col_prim(lp,k+1);
        x[k]=(int)floor(temp+.5);
        assert(std::abs(x[k]-temp)<=1e-6);
    }
    temp=glp_get_col_prim(lp,nedg+1);
    flow=(int)floor(temp+.5);
    assert(std::abs(flow-temp)<=1e-6);
    glp_delete_prob(lp);
    return flow;
}

/* find min (s,t)-cut for known max flow */
int graphe::tsp::min_st_cut(int nn,int nedg,const ivector &beg,
        const ivector &end,const ivector &cap,int s,int t,
        const ivector &x,ivector &cut) {
    int i,j,k,p,q,temp;
    ivector &head1=mincut_data[18]; if (int(head1.size())<nn) head1.resize(nn);
    ivector &head2=mincut_data[19]; if (int(head2.size())<nn) head2.resize(nn);
    ivector &next1=mincut_data[20]; if (int(next1.size())<nn) next1.resize(nn);
    ivector &next2=mincut_data[21]; if (int(next2.size())<nn) next2.resize(nn);
    for (i=0;i<nn;i++) head1[i]=head2[i]=0;
    for (k=0;k<nedg;k++) {
        i=beg[k]-1; next1[k]=head1[i]; head1[i]=k+1;
        j=end[k]-1; next2[k]=head2[j]; head2[j]=k+1;
    }
    ivector &list=mincut_data[4]; list.resize(nn);
    for (i=0;i<nn;i++) cut[i]=0;
    p=q=0; list[0]=s; cut[s-1]=1;
    while (p<=q) {
        i=list[p++];
        for (k=head1[i-1];k!=0;k=next1[k-1]) {
            j=end[k-1];
            assert(beg[k-1]==i);
            if (cut[j-1]==0 && x[k-1]<+cap[k-1]) {
                list[++q]=j; cut[j-1]=1;
            }
        }
        for (k=head2[i-1];k!=0;k=next2[k-1]) {
            j=beg[k-1];
            assert(end[k-1]==i);
            if (cut[j-1]==0 && x[k-1]>-cap[k-1]) {
                list[++q]=j; cut[j-1]=1;
            }
        }
    }
    assert(!cut[t-1]);
    temp=0;
    for (k=0;k<nedg;k++) {
        i=beg[k],j=end[k];
        if ((cut[i-1] && !cut[j-1]) || (!cut[i-1] && cut[j-1]))
            temp+=cap[k];
    }
    return temp;
}

/* find min cut with Stoer and Wagner algorithm */
int graphe::tsp::minimal_cut(int nn,int nedg,const ivector &beg,
                             const ivector &end,const ivector &cap,ivector &cut) {
    int k;
    assert(nn>=2);
    assert(nedg>=0);
    for (k=0;k<nedg;k++) {
        assert(1<=beg[k] && beg[k]<end[k] && end[k]<=nn);
        assert(cap[k]>0);
    }
    int i,j,min_cut,flow,temp,I,J,K,S,T,DEG,NV,NE;
    ivector &head1=mincut_data[0]; if (int(head1.size())<nn) head1.resize(nn);
    ivector &head2=mincut_data[1]; if (int(head2.size())<nn) head2.resize(nn);
    ivector &next1=mincut_data[2]; if (int(next1.size())<nedg) next1.resize(nedg);
    ivector &next2=mincut_data[3]; if (int(next2.size())<nedg) next2.resize(nedg);
    for (i=0;i<nn;i++) head1[i]=head2[i]=0;
    for (k=0;k<nedg;k++) {
        i=beg[k]; next1[k]=head1[i-1]; head1[i-1]=k+1;
        j=end[k]; next2[k]=head2[j-1]; head2[j-1]=k+1;
    }
    NV=nn;
    ivector &HEAD=mincut_data[4]; if (int(HEAD.size())<nn) HEAD.resize(nn);
    ivector &NEXT=mincut_data[5]; if (int(NEXT.size())<nn) NEXT.resize(nn);
    ivector &NUMB=mincut_data[6]; if (int(NUMB.size())<nn) NUMB.resize(nn);
    for (i=0;i<nn;i++) {
        HEAD[i]=i+1; NEXT[i]=0; NUMB[i]=i+1;
    }
    ivector &BEG=mincut_data[7]; if (int(BEG.size())<nedg) BEG.resize(nedg);
    ivector &END=mincut_data[8]; if (int(END.size())<nedg) END.resize(nedg);
    ivector &CAP=mincut_data[9]; if (int(CAP.size())<nedg) CAP.resize(nedg);
    ivector &X=mincut_data[10]; if (int(X.size())<nedg) X.resize(nedg);
    ivector &ADJ=mincut_data[11]; if (int(ADJ.size())<nn) ADJ.resize(nn);
    ivector &SUM=mincut_data[12]; if (int(SUM.size())<nn) SUM.resize(nn);
    ivector &CUT=mincut_data[13]; if (int(CUT.size())<nn) CUT.resize(nn);
    min_cut=INT_MAX;
    while (NV>1) {
        for (I=0;I<NV;I++) SUM[I]=0.0;
        for (I=1;I<=NV;I++) {
            DEG=0;
            for (i=HEAD[I-1];i!=0;i=NEXT[i-1]) {
                for (k=head1[i-1];k!=0;k=next1[k-1]) {
                    j=end[k-1];
                    J=NUMB[j-1];
                    if (I>=J)
                        continue;
                    if (SUM[J-1]==0.0)
                        ADJ[DEG++]=J;
                    assert(cap[k-1]>0.0);
                    SUM[J-1]+=cap[k-1];
                }
                for (k=head2[i-1];k!=0;k=next2[k-1]) {
                    j=beg[k-1];
                    J=NUMB[j-1];
                    if (I>=J)
                        continue;
                    if (SUM[J-1]==0.0)
                        ADJ[DEG++]=J;
                    assert(cap[k-1]>0.0);
                    SUM[J-1]+=cap[k-1];
                }
            }
            NE=0;
            for (K=0;K<DEG;K++) {
                assert(NE<nedg);
                J=ADJ[K];
                BEG[NE]=I; END[NE]=J; CAP[NE]=SUM[J-1];
                SUM[J-1]=0.0;
                ++NE;
            }
        }
        S=1; T=NV;
        flow=max_flow(NV,NE,BEG,END,CAP,S,T,X);
        if (min_cut>flow)
        {  min_cut=flow;
            temp=min_st_cut(NV,NE,BEG,END,CAP,S,T,X,CUT);
            assert(flow==temp);
            for (i=0;i<nn;i++) cut[i]=CUT[NUMB[i]-1];
            if (min_cut==0)
                break;
        }
        assert(T==NV);
        for (i=HEAD[T-1];i!=0;i=NEXT[i-1]) NUMB[i-1]=S;
        i=HEAD[S-1];
        assert(i!=0);
        while (NEXT[i-1]!=0)
            i=NEXT[i-1];
        NEXT[i-1]=HEAD[T-1];
        NV--;
    }
    return min_cut;
}

/* TSP constructor */
graphe::tsp::tsp(graphe *gr) {
    G=gr;
    sg=-1;
    isdirected=G->is_directed();
    isweighted=G->is_weighted();
    ipairs E;
    G->get_edges_as_pairs(E);
    edges_comparator comp(G);
    sort(E.begin(),E.end(),comp); // sort edges by weight (ascending)
    nv=G->node_count();
    ne=E.size();
    tour.reserve(nv);
    obj.resize(ne);
    xev.resize(ne);
    can_branch.resize(ne);
    coeff=new double[ne+1];
    indices=new int[ne+1];
    visited=new bool[nv];
    arcs=new arc[ne];
    sg_vertices=new int[nv];
    sg_edges=new int[ne];
    int i;
    for (ipairs_iter it=E.begin();it!=E.end();++it) {
        i=it-E.begin();
        arc &a=arcs[i];
        a.head=it->second;
        a.tail=it->first;
        loc_map[a.tail][a.head]=i;
        a.sg_index=-1;
        weight_map[a.tail][a.head]=_evalf(G->weight(*it),G->giac_context()).DOUBLE_val();
    }
    is_undir_weighted=!isdirected && isweighted;
}

/* TSP destructor */
graphe::tsp::~tsp() {
    delete[] coeff;
    delete[] indices;
    delete[] visited;
    delete[] arcs;
    delete[] sg_vertices;
    delete[] sg_edges;
}

/* create an edge or arc */
graphe::ipair graphe::tsp::make_edge(int i,int j) const {
    if (isdirected)
        return make_pair(i,j);
    return make_pair(i<j?i:j,i<j?j:i);
}

/* return the subgraph index of edge e */
int graphe::tsp::edge_index(const ipair &e) {
    int i=loc_map[e.first][e.second];
    return sg<0?i:arcs[i].sg_index;
}

/* return the subgraph index of vertex i */
int graphe::tsp::vertex_index(int i) {
    if (sg<0)
        return i;
    for (int j=0;j<sg_nv;++j) {
        if (sg_vertices[j]==i)
            return j;
    }
    return -1;
}

/* collect indices of all edges in the subgraph of G with index sg */
void graphe::tsp::make_sg_edges() {
    int j=0;
    for (int i=0;i<ne;++i) {
        arc &a=arcs[i];
        if (sg<0 || (G->node(a.head).subgraph()==sg && G->node(a.tail).subgraph()==sg)) {
            a.sg_index=j;
            sg_edges[j++]=i;
        } else a.sg_index=-1;
    }
    sg_ne=j;
}

/* formulate TSP as MIP, initially without any subtour elimination constraints */
void graphe::tsp::formulate_mip() {
    ivectors rows;
    int n=sg<0?nv:sg_nv,m=sg<0?ne:sg_ne;
    int nonzeros=0,i,j,k,l,cnt=0,nrows,ncols,nrows0;
    for (j=0;j<m;++j) {
        const arc &a=arcs[sg<0?j:sg_edges[j]];
        obj[j]=isweighted?weight(a.tail,a.head):1.0;
    }
    for (j=0;j<n;++j) {
        rows.resize(rows.size()+1);
        ivector &row=rows.back();
        l=sg<0?j:sg_vertices[j];
        row.push_back(l);
        for (i=0;i<n;++i) {
            k=sg<0?i:sg_vertices[i];
            if (i!=j && G->has_edge(k,l)) {
                row.push_back(k);
                ++nonzeros;
            }
        }
    }
    if (isdirected) {
        nrows0=rows.size();
        for (i=0;i<n;++i) {
            rows.resize(rows.size()+1);
            ivector &row=rows.back();
            k=sg<0?i:sg_vertices[i];
            row.push_back(k);
            for (j=0;j<n;++j) {
                l=sg<0?j:sg_vertices[j];
                if (i!=j && G->has_edge(k,l)) {
                    row.push_back(l);
                    ++nonzeros;
                }
            }
        }
    }
    ncols=m;
    nrows=rows.size();
    if (is_undir_weighted)
        nonzeros+=m;
    int *ia=new int[nonzeros+1];
    int *ja=new int[nonzeros+1];
    double *ar=new double[nonzeros+1];
    for (ivectors_iter it=rows.begin();it!=rows.end();++it) {
        const ivector &row=*it;
        k=it-rows.begin();
        i=row.front();
        for (ivector_iter rt=row.begin()+1;rt!=row.end();++rt) {
            j=*rt;
            l=edge_index(isdirected?(k<nrows0?make_pair(j,i):make_pair(i,j)):make_edge(i,j));
            assert(l<m);
            ia[++cnt]=k+1; ja[cnt]=l+1;
            ar[cnt]=1.0;
        }
    }
    /* build MIP */
    glp_add_rows(mip,nrows);
    double rh=isdirected?1.0:2.0;
    for (i=0;i<nrows;++i) glp_set_row_bnds(mip,i+1,GLP_FX,rh,rh);
    if (is_undir_weighted) {
        int r=glp_add_rows(mip,1);
        double lb=lower_bound();
        glp_set_row_bnds(mip,r,GLP_LO,lb,DBL_MAX);
        for (i=0;i<m;++i) {
            ia[++cnt]=r; ja[cnt]=i+1;
            ar[cnt]=obj[i];
        }
    }
    glp_add_cols(mip,ncols);
    for (j=0;j<m;++j) {
        glp_set_col_kind(mip,j+1,GLP_BV);
        glp_set_obj_coef(mip,j+1,obj[j]);
    }
    assert(cnt<=nonzeros);
    glp_load_matrix(mip,cnt,ia,ja,ar);
    delete[] ia; delete[] ja;
    delete[] ar;
}

/* compute lower bound for the objective function using minimal spanning tree */
double graphe::tsp::lower_bound() {
    double cost,maxcost=0;
    int i;
    for (int k=0;k<(sg<0?nv:sg_nv);++k) {
        i=sg<0?k:sg_vertices[k];
        vertex &v=G->node(i);
        if (sg<0) {
            G->unset_subgraphs(0);
            v.set_subgraph(1);
        } else v.set_subgraph(-1);
        graphe T(G->giac_context());
        G->minimal_spanning_tree(T,sg);
        ipairs E;
        cost=0;
        T.get_edges_as_pairs(E);
        for (ipairs_iter it=E.begin();it!=E.end();++it) {
            cost+=_evalf(T.weight(*it),G->giac_context()).DOUBLE_val();
        }
        if (sg<0)
            G->unset_subgraphs();
        else v.set_subgraph(sg);
        dvector c;
        for (ivector_iter it=v.neighbors().begin();it!=v.neighbors().end();++it) {
            if (sg>=0 && G->node(*it).subgraph()!=sg)
                continue;
            c.push_back(weight(i,*it));
        }
        assert(c.size()>1);
        sort(c.begin(),c.end());
        cost+=c[0];
        cost+=c[1];
        if (cost>maxcost)
            maxcost=cost;
    }
    return maxcost;
}

/* find the subtours (sv) in the subgraph induced by vertices in v */
bool graphe::tsp::find_subgraph_subtours(ivectors &sv,solution_status &status) {
    glp_erase_prob(mip);
    glp_set_obj_dir(mip,GLP_MIN);
    formulate_mip();
    glp_smcp lparm;             // LP solver settings
    glp_init_smcp(&lparm);
    lparm.msg_lev=GLP_MSG_OFF;  // do not output any messages
    glp_iocp parm;              // MIP solver settings
    glp_init_iocp(&parm);
    parm.msg_lev=GLP_MSG_OFF;   // do not output any messages
    parm.gmi_cuts=GLP_ON;       // generate Gomory cuts
    parm.br_tech=GLP_BR_MFV;    // choose the most fractional variable (the fallback branching rule)
    parm.fp_heur=GLP_ON;        // enable feasibility pump heuristic
    parm.sr_heur=GLP_OFF;       // disable simple rounding heuristic (according to A. Makhorin)
    parm.cb_func=&callback;     // MIP callback
    parm.cb_info=static_cast<void*>(this);
    parm.tm_lim=sg>=0?5000:RAND_MAX;            // time limit
    parm.bt_tech=sg<0?GLP_BT_BPH:GLP_BT_BLB;    // backtracking technique
    parm.mip_gap=sg<0?0.0:1e-4;  // MIP gap relative tolerance
    bool retval=true;
    int iter_count=0,res,lres,stat;
    is_symmetric_tsp=is_undir_weighted && G->is_clique(sg);
    heur_type=is_symmetric_tsp?_GT_TSP_CHRISTOFIDES_SA:
                               (is_undir_weighted?_GT_TSP_FARTHEST_INSERTION_HEUR:
                                                  _GT_TSP_NO_HEUR);
    do {
        ++iter_count;
        /* append subtour elimination constraints */
        for (set<ivector>::const_iterator it=subtours.begin();it!=subtours.end();++it) {
            append_sce(*it);
        }
        subtours.clear();
        if ((lres=glp_simplex(mip,&lparm))==0 && glp_get_status(mip)==GLP_OPT)
            res=glp_intopt(mip,&parm);
        else res=-1;
        stat=glp_get_status(mip);
        if (res==0 && stat==GLP_OPT)
            status=_GT_TSP_OPTIMAL;
        else if (lres==0 && stat==GLP_NOFEAS) {
            status=_GT_TSP_NOT_HAMILTONIAN;
            break;
        } else {
            status=_GT_TSP_ERROR;
            retval=false;
            break;
        }
        get_subtours();
        if (sg>=0) {
            lift_subtours(sv);
            if (iter_count>25)
                break;
        }
    } while (subtours.size()>1);
    ++num_nodes;
    if (sg<0 && (status==_GT_TSP_OPTIMAL))
        lift_subtours(sv);
    return retval;
}

/* lift subtours to cycles in G */
void graphe::tsp::lift_subtours(ivectors &sv) const {
    for (set<ivector>::const_iterator it=subtours.begin();it!=subtours.end();++it) {
        const ivector &subtour=*it;
        if (sg<0)
            sv.push_back(subtour);
        else {
            sv.resize(sv.size()+1);
            ivector &lifted_subtour=sv.back();
            lifted_subtour.resize(subtour.size());
            for (ivector_iter jt=subtour.begin();jt!=subtour.end();++jt) {
                lifted_subtour[jt-subtour.begin()]=sg_edges[*jt];
            }
        }
    }
}

/* append subtour elimination constraint to MIP */
void graphe::tsp::append_sce(const ivector &subtour) {
    int i,j,r=glp_add_rows(mip,1),cnt=0,sz=subtour.size();
    int m=sg<0?ne:sg_ne;
    if (!isdirected && 3*sz>2*(sg<0?nv:sg_nv)+1) {
        for (int k=0;k<m;++k) {
            if (find(subtour.begin(),subtour.end(),k)!=subtour.end())
                continue;
            i=sg<0?k:sg_edges[k];
            const arc &a1=arcs[i];
            for (ivector_iter jt=subtour.begin();jt!=subtour.end();++jt) {
                j=sg<0?*jt:sg_edges[*jt];
                assert(i!=j);
                const arc &a2=arcs[j];
                if (a1.head==a2.head || a1.tail==a2.tail ||
                        a1.head==a2.tail || a1.tail==a2.head) {
                    indices[++cnt]=k+1;
                    coeff[cnt]=1.0;
                    break;
                }
            }
        }
        glp_set_row_bnds(mip,r,GLP_LO,2,DBL_MAX);
    } else {
        for (ivector_iter it=subtour.begin();it!=subtour.end();++it) {
            indices[++cnt]=*it+1;
            coeff[cnt]=1.0;
        }
        glp_set_row_bnds(mip,r,GLP_UP,0.0,sz-1);
    }
    glp_set_mat_row(mip,r,cnt,indices,coeff);
}

/* append subtours from sv, converting them to conform sg_edges */
void graphe::tsp::add_subtours(const ivectors &sv) {
    ivector subtour;
    for (ivectors_iter it=sv.begin();it!=sv.end();++it) {
        if (sg<0)
            subtours.insert(canonical_subtour(*it));
        else {
            subtour.resize(it->size());
            for (ivector_iter jt=it->begin();jt!=it->end();++jt) {
                subtour[jt-it->begin()]=arcs[*jt].sg_index;
            }
            subtours.insert(canonical_subtour(subtour));
        }
    }
}

/* collect subtours from the current best MIP integer solution */
void graphe::tsp::get_subtours() {
    ivector sel;
    int m=glp_get_num_cols(mip),i,j;
    for (i=0;i<m;++i) {
        if (glp_mip_col_val(mip,i+1)!=0)
            sel.push_back(i);
    }
    vector<bool> vst(sel.size(),false);
    ivector subtour;
    arc a;
    for (ivector_iter st=sel.begin();st!=sel.end();++st) {
        const arc &fa=arcs[sg<0?*st:sg_edges[*st]];
        i=st-sel.begin();
        if (vst[i])
            continue;
        subtour.clear();
        subtour.push_back(*st);
        vst[i]=true;
        a=fa;
        ivector_iter it;
        do {
            for (it=sel.begin();it!=sel.end();++it) {
                j=it-sel.begin();
                const arc &aa=arcs[sg<0?*it:sg_edges[*it]];
                if (i==j || vst[j])
                    continue;
                if (aa.tail==a.head || (!isdirected && (aa.tail==a.tail || aa.head==a.head || aa.head==a.tail))) {
                    vst[j]=true;
                    subtour.push_back(*it);
                    a.head=aa.head;
                    a.tail=aa.tail;
                    i=j;
                    break;
                }
            }
        } while (it!=sel.end());
        subtours.insert(canonical_subtour(subtour));
    }
}

/* construct hierarhical clustering forest */
void graphe::tsp::make_hierarhical_clustering_forest() {
    int k;
    int u=std::floor(nv*4.0/std::log2(nv)); // upper bound on the cluster cardinality
    hierarhical_clustering_forest.clear();
    /* create leaf nodes */
    for (int i=0;i<nv;++i) {
        ivector node(4,-1);
        node.back()=i;
        hierarhical_clustering_forest.push_back(node);
    }
    ipair child;
    /* add edges one by one, creating tree nodes from bottom up */
    for (int i=0;i<ne;++i) {
        arc &a=arcs[i];
        k=hierarhical_clustering_forest.size();
        for (ivectors_iter it=hierarhical_clustering_forest.begin();it!=hierarhical_clustering_forest.end();++it) {
            if (it->front()<0 && find(it->begin()+3,it->end(),a.tail)!=it->end())
                child.first=it-hierarhical_clustering_forest.begin();
            if (it->front()<0 && find(it->begin()+3,it->end(),a.head)!=it->end())
                child.second=it-hierarhical_clustering_forest.begin();
        }
        if (child.first==child.second) // e does not connect two separated components
            continue;
        ivector &c1=hierarhical_clustering_forest[child.first],&c2=hierarhical_clustering_forest[child.second];
        if (int(c1.size()+c2.size())>u+6) // forbid creating components with more than u vertices
            continue;
        /* merge components to parent node */
        c1.front()=c2.front()=k; // set parent to children
        ivector p(3,-1);
        p[1]=child.first;
        p[2]=child.second;
        /* merge the children vertex sets and sort the resulting list */
        p.insert(p.end(),c1.begin()+3,c1.end());
        p.insert(p.end(),c2.begin()+3,c2.end());
        sort(p.begin()+3,p.end());
        hierarhical_clustering_forest.push_back(p);
    }
}

/* return canonical representation of the subtour (starting with the lowest edge index) */
graphe::ivector graphe::tsp::canonical_subtour(const ivector &subtour) {
    int minv=-1,pos;
    for (ivector_iter it=subtour.begin();it!=subtour.end();++it) {
        if (minv<0 || *it<minv) {
            minv=*it;
            pos=it-subtour.begin();
        }
    }
    ivector res(subtour.begin()+pos,subtour.end());
    res.insert(res.end(),subtour.begin(),subtour.begin()+pos);
    if (!isdirected && res.size()>2 && res[1]>res.back())
        std::reverse(res.begin()+1,res.end()); // canonical orientation
    return res;
}

/* traverse the hierarhical clustering forest and collect all relevant SEC */
void graphe::tsp::hierarhical_clustering_dfs(int i,ivectors &considered_sec,ivectors &relevant_sec) {
    if (i<0)
        return;
    const ivector &node=hierarhical_clustering_forest[i];
    assert(node.size()>3);
    int sz=node.size()-3,left_child=node[1],right_child=node[2];
    if (sz<3) // do not search for subtours in nodes with less than 3 vertices
        return;
    /* process the children nodes to obtain considered and relevant SEC */
    ivectors left_cons,right_cons,left_sec,right_sec,sv,cons;
    hierarhical_clustering_dfs(left_child,left_cons,left_sec);
    hierarhical_clustering_dfs(right_child,right_cons,right_sec);
    relevant_sec.insert(relevant_sec.end(),left_sec.begin(),left_sec.end());
    relevant_sec.insert(relevant_sec.end(),right_sec.begin(),right_sec.end());
    cons.insert(cons.end(),left_cons.begin(),left_cons.end());
    cons.insert(cons.end(),right_cons.begin(),right_cons.end());
    /* process this node */
    solution_status status;
    sg=G->max_subgraph_index()+1;
    G->set_subgraph(ivector(node.begin()+3,node.end()),sg);
    if (!G->is_biconnected(sg)) {
        considered_sec=cons;
        return;
    }
    make_sg_edges();
    ivector V;
    G->get_subgraph(sg,V);
    sg_nv=V.size();
    for (ivector_iter it=V.begin();it!=V.end();++it) {
        sg_vertices[it-V.begin()]=*it;
    }
    subtours.clear();
    add_subtours(left_sec);
    add_subtours(right_sec);
    find_subgraph_subtours(sv,status);
    for (ivectors::iterator it=cons.begin();it!=cons.end();++it) {
        *it=canonical_subtour(*it);
    }
    ivector canon_s;
    ivectors_iter jt;
    for (ivectors_iter it=sv.begin();it!=sv.end();++it) {
        canon_s=canonical_subtour(*it);
        for (jt=cons.begin();jt!=cons.end();++jt) {
            if (canon_s==*jt) {
                relevant_sec.push_back(*it);
                break;
            }
        }
        if (jt==cons.end())
            considered_sec.push_back(*it);
    }
}

/* solve the original problem */
int graphe::tsp::solve(ivector &hc,double &cost) {
    make_hierarhical_clustering_forest();
    G->unset_subgraphs();
    ivectors cons,relevant,sec,sv;
    mip=glp_create_prob();
    num_nodes=0;
    for (ivectors_iter it=hierarhical_clustering_forest.begin();it!=hierarhical_clustering_forest.end();++it) {
        if (it->front()<0) { // root node of a tree in the forest
            hierarhical_clustering_dfs(it-hierarhical_clustering_forest.begin(),cons,relevant);
            sec.insert(sec.end(),cons.begin(),cons.end());
            sec.insert(sec.end(),relevant.begin(),relevant.end());
        }
    }
    sg=-1;
    G->unset_subgraphs();
    add_subtours(sec);
    solution_status status;
    if (!find_subgraph_subtours(sv,status))
        return -1;
    int retval;
    switch (status) {
    case _GT_TSP_OPTIMAL:
        if (sv.size()==1) {
            const ivector &t=sv.front();
            int n=G->node_count();
            assert(int(t.size())==n);
            hc.clear();
            const arc &first=arcs[t.front()],&last=arcs[t.back()];
            int i=first.tail,j=first.head,k;
            if (i==last.tail || i==last.head) {
                hc.push_back(i);
                hc.push_back(k=j);
            } else {
                hc.push_back(j);
                hc.push_back(k=i);
            }
            for (ivector_iter it=t.begin()+1;it!=t.end();++it) {
                const arc &a=arcs[*it];
                if (a.tail==k) hc.push_back(k=a.head);
                else {
                    assert(a.head==k);
                    hc.push_back(k=a.tail);
                }
            }
            straighten(hc);
            cost=tour_cost(hc);
            retval=1; // success
        } else retval=0;
        break;
    case _GT_TSP_NOT_HAMILTONIAN:
        retval=0; // fail
        break;
    case _GT_TSP_ERROR:
        retval=-1; // error
        break;
    }
    glp_delete_prob(mip);
    return retval;
}

/* return weight of edge (i,j) */
double graphe::tsp::weight(int i,int j) {
    ipair e=make_edge(i,j);
    return weight_map[e.first][e.second];
}

double graphe::tsp::weight(const ipair &e) {
    return weight(e.first,e.second);
}

/* return the cost of the tour */
double graphe::tsp::tour_cost(const ivector &hc) {
    int n=hc.size()-1,v,w;
    if (!isweighted)
        return n;
    double cost=0;
    for (int i=0;i<n;++i) {
        v=hc[i];
        w=hc[i+1];
        assert(G->has_edge(v,w));
        cost+=weight(v,w);
    }
    return cost;
}

/* apply heuristics */
void graphe::tsp::heur(glp_tree *tree) {
    if (heur_type==_GT_TSP_NO_HEUR)
        return;
    int n=sg<0?nv:sg_nv,m=sg<0?ne:sg_ne,i,j,k;
    if (heur_type==_GT_TSP_CHRISTOFIDES_SA) { // symmetric TSP
        christofides(tour);
        heur_type=_GT_TSP_FARTHEST_INSERTION_RANDOM;
    } else { // weighted undirected TSP
        /* choose the initial arc a by random such that weight(e) >= median weight,
         * in the first pass try to construct a tour starting from heaviest edge */
        int index=heur_type==_GT_TSP_FARTHEST_INSERTION_RANDOM?(m+1)/2+G->rand_integer(m/2):m-1;
        farthest_insertion(index,tour);
        heur_type=_GT_TSP_FARTHEST_INSERTION_RANDOM;
        if (int(tour.size())<=n)
            return;
    }
    assert(int(tour.size())==n+1);
    /* optimize the tour */
    //double old=tour_cost(tour);
    //cout << "Improving the tour... ";
    lin_kernighan(tour);
    //cout << "Done. Ratio: " << (old-tour_cost(tour))/old*100.0 << "%" << endl;
    /* construct the heuristic solution and pass it to the MIP solver */
    for (i=0;i<m;++i) coeff[i+1]=0.0;
    for (i=0;i<n;++i) {
        j=i+1;
        k=edge_index(make_edge(tour[i],tour[j]));
        coeff[k+1]=1.0;
    }
    glp_ios_heur_sol(tree,coeff);
}

/* farthest insertion heuristic for obtaining a Hamiltonian tour */
void graphe::tsp::farthest_insertion(int index,ivector &hc) {
    int n=sg<0?nv:sg_nv,i,j,k,f;
    const arc &a=arcs[index];
    i=a.tail;
    j=a.head;
    hc.clear();
    hc.push_back(i);
    hc.push_back(j);
    hc.push_back(i);
    ivector_iter jt;
    for (int vis=0;vis<nv;++vis) visited[vis]=false;
    visited[i]=visited[j]=true;
    dvector W;
    double w,maxw,minw;
    /* proceed with farthest insertion heuristic */
    do {
        maxw=0;
        f=-1;
        for (int l=0;l<n;++l) {
            k=sg<0?l:sg_vertices[l];
            if (visited[k])
                continue;
            W.clear();
            for (ivector_iter it=hc.begin();it!=hc.end();++it) {
                if (G->has_edge(k,*it))
                    W.push_back(weight(k,*it));
            }
            minw=DBL_MAX;
            if (W.size()<2)
                continue;
            else for (dvector_iter wt=W.begin();wt!=W.end();++wt) {
                if (*wt<minw)
                    minw=*wt;
            }
            if (minw>maxw) {
                maxw=minw;
                f=k;
            }
        }
        if ((k=f)<0)
            break;
        i=-1;
        minw=DBL_MAX;
        for (ivector_iter it=hc.begin();it!=hc.end();++it) {
            jt=it+1;
            if (jt!=hc.end() && G->has_edge(*it,k) && G->has_edge(k,*jt) &&
                    minw>(w=weight(*it,k)+weight(k,*jt)-weight(*it,*jt))) {
                minw=w;
                i=it-hc.begin();
            }
        }
        if (i<0)
            break;
        hc.insert(hc.begin()+i+1,k);
        visited[k]=true;
    } while (int(hc.size())<=n);
}

/* try to lower the cost of the tour hc */
void graphe::tsp::perform_3opt_moves(ivector &hc) {
    int n=hc.size()-1,b1,e1,b2,e2,b3,e3,i1,j1,i2,j2,i3,j3,i,j,k,var,iter_count=0,moves_count=0;
    double opt_timeout=5.0+25.0*std::exp(-std::pow(std::max(0,1000-n),2)/2e5);
    vector<bool> visited(nv);
    ivectors opt_moves;
    double sw,save,maxsave,wb1e1,wb2e2,wb3e3,wb1b2,wb1e2,we1e2;
    bool hb1e2,hb1b2,he1e2;
    /* improve the tour by making a limited number of 3-opt moves */
    clock_t start_time=clock();
    ivector ijk(3);
    bool triplet_found=false;
    bool timed_out=false;
    do {
        ++iter_count;
        opt_moves.clear();
        std::fill(visited.begin(),visited.end(),false);
        for (i=0;i<n;++i) {
            triplet_found=false;
            b1=hc[i]; e1=hc[i+1];
            if (visited[b1] || visited[e1])
                continue;
            wb1e1=weight(b1,e1);
            for (j=i+2;j<n;++j) {
                if (double(clock()-start_time)/CLOCKS_PER_SEC>opt_timeout) {
                    timed_out=true;
                    break;
                }
                b2=hc[j]; e2=hc[j+1];
                if (visited[b2] || visited[e2])
                    continue;
                wb2e2=weight(b2,e2);
                hb1e2=G->has_edge(b1,e2);
                hb1b2=G->has_edge(b1,b2);
                he1e2=G->has_edge(e1,e2);
                if (hb1b2) wb1b2=weight(b1,b2);
                if (hb1e2) wb1e2=weight(b1,e2);
                if (he1e2) we1e2=weight(e1,e2);
                for (k=j+2;k<n;++k) {
                    b3=hc[k]; e3=hc[k+1];
                    if (visited[b3] || visited[e3])
                        continue;
                    wb3e3=weight(b3,e3);
                    sw=wb1e1+wb2e2+wb3e3;
                    if ((hb1e2 && G->has_edge(b3,e1) && G->has_edge(b2,e3) && sw>wb1e2+weight(b3,e1)+weight(b2,e3)) ||
                            (hb1e2 && G->has_edge(b3,b2) && G->has_edge(e1,e3) && sw>wb1e2+weight(b3,b2)+weight(e1,e3)) ||
                            (hb1b2 && G->has_edge(b3,e1) && G->has_edge(e2,e3) && sw>wb1b2+weight(b3,e1)+weight(e2,e3)) ||
                            (G->has_edge(b1,b3) && he1e2 && G->has_edge(b2,e3) && sw>weight(b1,b3)+we1e2+weight(b2,e3)))
                    {
                        ivector opt_move(6);
                        opt_move[0]=b1; opt_move[1]=b2; opt_move[2]=b3;
                        opt_move[3]=e1; opt_move[4]=e2; opt_move[5]=e3;
                        opt_moves.push_back(opt_move);
                        visited[b1]=visited[b2]=visited[b3]=true;
                        visited[e1]=visited[e2]=visited[e3]=true;
                        triplet_found=true;
                        break;
                    }
                }
                if (triplet_found)
                    break;
            }
            if (timed_out)
                break;
        }
        if (opt_moves.empty()) break;
        for (ivectors_iter it=opt_moves.begin(); it!=opt_moves.end();++it) {
            const ivector &opt_move=*it;
            i1=find(hc.begin(),hc.end(),opt_move[0])-hc.begin();
            i2=find(hc.begin(),hc.end(),opt_move[1])-hc.begin();
            i3=find(hc.begin(),hc.end(),opt_move[2])-hc.begin();
            j1=find(hc.begin(),hc.end(),opt_move[3])-hc.begin();
            j2=find(hc.begin(),hc.end(),opt_move[4])-hc.begin();
            j3=find(hc.begin(),hc.end(),opt_move[5])-hc.begin();
            if (std::abs(i1-j1)!=1) {
                if (i1==0) i1=n;
                else if (j1==0) j1=n;
                else assert(false);
            }
            if (std::abs(i2-j2)!=1) {
                if (i2==0) i2=n;
                else if (j2==0) j2=n;
                else assert(false);
            }
            if (std::abs(i3-j3)!=1) {
                if (i3==0) i3=n;
                else if (j3==0) j3=n;
                else assert(false);
            }
            assert(std::abs(i1-j1)==1 && std::abs(i2-j2)==1 && std::abs(i3-j3)==1);
            ijk[0]=std::min(i1,j1);
            ijk[1]=std::min(i2,j2);
            ijk[2]=std::min(i3,j3);
            sort(ijk.begin(),ijk.end());
            i=ijk[0]; j=ijk[1]; k=ijk[2];
            b1=hc[i]; b2=hc[j]; b3=hc[k];
            e1=hc[i+1]; e2=hc[j+1]; e3=hc[k+1];
            sw=weight(b1,e1)+weight(b2,e2)+weight(b3,e3);
            maxsave=0.0;
            if (G->has_edge(b1,e2) && G->has_edge(b3,e1) && G->has_edge(b2,e3) &&
                    (save=sw-weight(b1,e2)-weight(b3,e1)-weight(b2,e3))>maxsave) {
                var=0; maxsave=save;
            }
            if (G->has_edge(b1,e2) && G->has_edge(b3,b2) && G->has_edge(e1,e3) &&
                  (save=sw-weight(b1,e2)-weight(b3,b2)-weight(e1,e3))>maxsave) {
                var=1; maxsave=save;
            }
            if (G->has_edge(b1,b2) && G->has_edge(b3,e1) && G->has_edge(e2,e3) &&
                  (save=sw-weight(b1,b2)-weight(b3,e1)-weight(e2,e3))>maxsave) {
                var=2; maxsave=save;
            }
            if (G->has_edge(b1,b3) && G->has_edge(e1,e2) && G->has_edge(b2,e3) &&
                  (save=sw-weight(b1,b3)-weight(e1,e2)-weight(b2,e3))>maxsave) {
                var=3; maxsave=save;
            }
            if (maxsave>.001) {
                ivector part(hc.begin()+j+1,hc.begin()+k+1);
                switch (var) {
                case 1:
                    std::reverse(hc.begin()+i+1,hc.begin()+j+1);
                case 3:
                    if (var==3) std::reverse(part.begin(),part.end());
                case 0:
                    hc.erase(hc.begin()+j+1,hc.begin()+k+1);
                    hc.insert(hc.begin()+i+1,part.begin(),part.end());
                    break;
                case 2:
                    std::reverse(hc.begin()+i+1,hc.begin()+j+1);
                    std::reverse(hc.begin()+j+1,hc.begin()+k+1);
                    break;
                default:
                    assert(false);
                }
                ++moves_count;
            }
        }
    } while (!timed_out);
}

void graphe::tsp::straighten(ivector &hc) {
    ivectors opt_moves;
    int i,j,k,l,b1,b2,e1,e2,n=hc.size()-1,iter_count=0,moves_count=0;
    double w0;
    vector<bool> visited(n+1);
    ivector opt_move(4);
    while (true) {
        ++iter_count;
        opt_moves.clear();
        std::fill(visited.begin(),visited.end(),false);
        for (int i=0;i<n;++i) {
            if (visited[i] && visited[i+1]) continue;
            b1=hc[i]; e1=hc[i+1];
            w0=weight(b1,e1);
            for (int j=i+2;j<n;++j) {
                if ((i==0 && j==n-1) || (visited[j] && visited[j+1])) continue;
                b2=hc[j]; e2=hc[j+1];
                if (G->has_edge(b1,b2) && G->has_edge(e1,e2) &&
                        w0+weight(b2,e2)-weight(b1,b2)-weight(e1,e2)>.001) {
                    opt_move[0]=b1; opt_move[1]=b2; opt_move[2]=e1; opt_move[3]=e2;
                    opt_moves.push_back(opt_move);
                    visited[i]=visited[i+1]=visited[j]=visited[j+1]=true;
                    break;
                }
            }
        }
        if (opt_moves.empty()) break;
        for (ivectors_iter it=opt_moves.begin();it!=opt_moves.end();++it) {
            const ivector &opt_move=*it;
            b1=opt_move[0]; b2=opt_move[1]; e1=opt_move[2]; e2=opt_move[3];
            i=find(hc.begin(),hc.end(),b1)-hc.begin(); j=find(hc.begin(),hc.end(),e1)-hc.begin();
            k=find(hc.begin(),hc.end(),b2)-hc.begin(); l=find(hc.begin(),hc.end(),e2)-hc.begin();
            if (std::abs(i-j)!=1) {
                if (i==0) i=n;
                else if (j==0) j=n;
                else assert(false);
            }
            if (std::abs(k-l)!=1) {
                if (k==0) k=n;
                else if (l==0) l=n;
                else assert(false);
            }
            assert(std::abs(i-j)==1 && std::abs(k-l)==1);
            i=std::min(i,j);
            j=std::min(k,l);
            std::reverse(hc.begin()+(i<j?i:j)+1,hc.begin()+(i<j?j:i)+1);
            ++moves_count;
        }
    }
}

/* Helsgaun criterion for k-opt move feasibility */
bool graphe::tsp::is_move_feasible(int k,const ivector &t,const ipairs &x) {
    ivector incl(2*k+1,0);
    int i,j,pi,count;
    for (int c=0;c<k;++c) {
        i=2*c+1;
        j=(i+1)%(2*k);
        incl[i+1]=j+1;
        incl[j+1]=i+1;
    }
    ivector p,q;
    ipairs xs(x.begin()+1,x.begin()+k+1);
    sort(xs.begin(),xs.end());
    for (ipairs_iter it=xs.begin();it!=xs.end();++it) {
        p.push_back(find(t.begin()+1,t.begin()+2*k+1,it->first)-t.begin());
    }
    for (i=k;i-->0;) {
        p.insert(p.begin()+i+1,((pi=p[i])%2)==0?pi-1:pi+1);
    }
    q.resize(p.size()+1);
    for (i=p.size();i-->0;) {
        assert((pi=p[i])<=2*k);
        q[pi]=i+1;
    }
    for (i=2*k,count=1;(i=q[incl[p[i-1]]]^1)!=0;count++);
    return count==k;
}

/* Lin-Kernighan backtracking k-opt heuristic for tour improvement */
void graphe::tsp::lin_kernighan(ivector &hc) {
    int n=hc.size()-1;
    int i,choice,j,j0,j_next,pos,ypos,t1,t2,t3,t4,t5,t2i,t2ip,dir;
    static ipairs x,y,y_tmp;
    static dvector wx,wy;
    static ivector t,t1_alt,y1_alt,y2_alt,cheaper_hc;
    x.resize(n+1); y.resize(n+1);
    wx.resize(n+1); wy.resize(n+1);
    t.resize(2*n+1);
    cheaper_hc.resize(n+1);
    t1_alt.clear();
    double w,wi;
    for (int k=0;k<n;++k) t1_alt.push_back(k); // initialize alternatives for t1
lk2:
    /* Let i:=1. Choose t[1] and let x1:=(t[1],t[2]=succ(t[1])) in hc.
     * Initialize alternatives for y1. */
    i=1;
    if (t1_alt.empty()) return; // no more alternatives for t[1]
    choice=t1_alt.size()==1?0:G->rand_integer(t1_alt.size());
    t1=t[1]=t1_alt[choice]; t1_alt.erase(t1_alt.begin()+choice);
    x[1]=make_pair(t1,t2=t[2]=(t1+1)%n);
    w=wx[1]=weight(hc[t1],hc[t2]);
    y1_alt.clear();
    for (int k=0;k<n;++k) {
        if (k!=t1 && k!=t2 && k!=(t1-1+n)%n && k!=(t2+1)%n &&
                G->has_edge(hc[t2],hc[k]) && w>weight(hc[t2],hc[k]))
            y1_alt.push_back(k);
    }
lk4:
    /* Choose y1:=(t[2],t[3]) not in hc such that G1>0. Else, go to lk2. */
    if (y1_alt.empty()) goto lk2;
    choice=y1_alt.size()==1?0:G->rand_integer(y1_alt.size());
    t3=t[3]=y1_alt[choice]; y1_alt.erase(y1_alt.begin()+choice);
    y[1]=make_pair(t2=t[2],t3);
    wy[1]=weight(hc[t2],hc[t3]);
lk5:
    /* Let i:=i+1 and let xi=(t[2i-1],t[2i]) in hc.
     * Update the tour if possible. If i=1 initialize alternatives for y2. */
    ++i;
    t2ip=t[2*i-1];
    x[i]=make_pair(t2ip,t2i=t[2*i]=(t2ip+1)%n);
    wx[i]=weight(hc[t2ip],hc[t2i]);
    w=0.0;
    for (int k=1;k<=i;++k) w+=wx[k]-(k==i?0.0:wy[k]);
    if (t2i!=(t1-1+n)%n && G->has_edge(hc[t2i],hc[t1]) &&
            w-weight(hc[t2i],hc[t1])>.001 && is_move_feasible(i,t,x)) {
        /* update hc with the new cheaper tour starting from t[2] */
        y[i]=make_pair(t2i,t1); // temporary yi
        y_tmp=ipairs(y.begin()+1,y.begin()+i+1);
        j=j0=t[2]; j_next=t[3];
        pos=ypos=dir=0;
        while (true) {
            if (ypos>=0)
                y_tmp.erase(y_tmp.begin()+ypos);
            cheaper_hc[pos++]=hc[j];
            j=j_next;
            if (j==j0) break;
            ypos=-1;
            for (int k=y_tmp.size();k-->0;) {
                const ipair &yk=y_tmp[k];
                if (yk.first==j || yk.second==j) {
                    j_next=yk.first==j?yk.second:yk.first;
                    ypos=k;
                    dir=0;
                    break;
                }
            }
            if (ypos<0) {
                if (dir==0)
                    dir=find(x.begin()+1,x.begin()+i+1,make_pair(j,(j+1)%n))==x.begin()+i+1?1:-1;
                j_next=(j+dir+n)%n;
            }
        }
        assert(pos==n);
        cheaper_hc.back()=hc[j0];
        hc=cheaper_hc;
        t1_alt.clear();
        for (int k=0;k<n;++k) t1_alt.push_back(k); // initialize alternatives for t1
        goto lk2;
    } else if (i==2) { // initialize alternatives for y2
        y2_alt.clear();
        t1=t[1]; t4=t[4];
        for (int k=0;k<n;++k) {
            if (find(t.begin()+1,t.begin()+5,k)==t.begin()+5 &&
                    find(t.begin()+1,t.begin()+5,(k+1)%n)==t.begin()+5 &&
                    G->has_edge(hc[t4],hc[k]) && w-weight(hc[t4],hc[k])>.001)
                y2_alt.push_back(k);
        }
    }
lk7:
    /* Choose yi:=(t[2i],t[2i+1]) not in hc such that Gi>0. If successful, go to lk5. */
    if (i==2) {
        if (y2_alt.empty()) { i=1; goto lk4; }
        choice=y2_alt.size()==1?0:G->rand_integer(y2_alt.size());
        y[2]=make_pair(t4=t[4],t5=t[5]=y2_alt[choice]); y2_alt.erase(y2_alt.begin()+choice);
        wy[2]=weight(hc[t4],hc[t5]);
        goto lk5;
    } else {
        int k;
        t2i=t[2*i]; t1=t[1];
        for (k=0;k<n;++k) {
            if (find(t.begin()+1,t.begin()+2*i+1,k)==t.begin()+2*i+1 &&
                    find(t.begin()+1,t.begin()+2*i+1,(k+1)%n)==t.begin()+2*i+1 &&
                    G->has_edge(hc[t2i],hc[k]) && w-(wi=weight(hc[t2i],hc[k]))>.001) {
                y[i]=make_pair(t2i,t[2*i+1]=k);
                wy[i]=wi;
                break;
            }
        }
        if (k<n) goto lk5;
    }
    i=2;
    goto lk7;
}

/* find minimum weight perfect matching in an undirected bipartite graph with
 * even number of vertices, edge set E and edge weights W */
void graphe::tsp::min_weight_matching_bipartite(const ivector &eind,const dvector &weights,ivector &matched_arcs) {
    set<int> Vset; // set of vertices
    for (ivector_iter it=eind.begin();it!=eind.end();++it) {
        const arc &a=arcs[*it];
        Vset.insert(a.head);
        Vset.insert(a.tail);
    }
    ivector V;
    for (set<int>::const_iterator it=Vset.begin();it!=Vset.end();++it) {
        V.push_back(*it);
    }
    int m=eind.size(),n=V.size(),nz=2*m,cnt=0,v;
    assert((n%2)==0);
    glp_prob *wp=glp_create_prob();
    glp_add_rows(wp,n);
    glp_add_cols(wp,m);
    glp_set_obj_dir(wp,GLP_MIN);
    int *ia=new int[nz+1];
    int *ja=new int[nz+1];
    double *ar=new double[nz+1];
    for (int i=0;i<n;++i) {
        v=V[i];
        for (int j=0;j<m;++j) {
            const arc &a=arcs[eind[j]];
            if (a.head==v || a.tail==v) {
                ia[++cnt]=i+1; ja[cnt]=j+1;
                ar[cnt]=1.0;
            }
        }
    }
    assert(cnt==nz);
    for (int i=0;i<n;++i) glp_set_row_bnds(wp,i+1,GLP_FX,1.0,1.0);
    for (int j=0;j<m;++j) {
        glp_set_col_kind(wp,j+1,GLP_BV);
        glp_set_obj_coef(wp,j+1,weights[j]);
    }
    glp_load_matrix(wp,nz,ia,ja,ar);
    delete[] ia; delete[] ja;
    delete[] ar;
    glp_smcp lparm;
    glp_iocp parm;
    glp_init_smcp(&lparm);
    lparm.msg_lev=GLP_MSG_OFF;
    glp_init_iocp(&parm);
    parm.br_tech=GLP_BR_MFV;
    parm.bt_tech=GLP_BT_BLB;
    parm.gmi_cuts=GLP_ON;
    parm.mir_cuts=GLP_ON;
    parm.msg_lev=GLP_MSG_OFF;
    parm.fp_heur=GLP_ON;
    parm.sr_heur=GLP_OFF;
    parm.cb_func=&min_wpm_callback;
    pair<const ivector*,tsp*> info;
    info.first=&eind;
    info.second=this;
    parm.cb_info=static_cast<void*>(&info);
    assert(glp_simplex(wp,&lparm)==0 && glp_get_status(wp)==GLP_OPT);
    assert(glp_intopt(wp,&parm)==0 && glp_get_status(wp)==GLP_OPT);
    for (int j=0;j<m;++j) {
        if (glp_mip_col_val(wp,j+1)!=0)
            matched_arcs.push_back(j);
    }
    glp_delete_prob(wp);
}

void graphe::tsp::min_wpm_heur(glp_tree *tree,const ivector &eind) {
    vector<bool> vertex_matched(nv,false);
    vector<bool> arc_matched(eind.size(),false);
    set<int> V;
    int i,j,k,n=eind.size();
    glp_prob *lp=glp_ios_get_prob(tree);
    for (i=0;i<n;++i) {
        if (glp_ios_can_branch(tree,i+1)==0 && glp_get_col_prim(lp,i+1)>0.9) {
            const arc &a=arcs[eind[i]];
            if (vertex_matched[a.head] || vertex_matched[a.tail])
                continue;
            arc_matched[i]=true;
            vertex_matched[a.head]=vertex_matched[a.tail]=true;
        }
    }
    for (ivector_iter it=eind.begin();it!=eind.end();++it) {
        k=it-eind.begin();
        if (arc_matched[k])
            continue;
        const arc &a=arcs[*it];
        i=a.tail; j=a.head;
        V.insert(i); V.insert(j);
        if (vertex_matched[i] || vertex_matched[j])
            continue;
        arc_matched[k]=true;
        vertex_matched[i]=vertex_matched[j]=true;
    }
    int cnt=0;
    for (vector<bool>::const_iterator it=arc_matched.begin();it!=arc_matched.end();++it) {
        if (*it) ++cnt;
        coeff[it-arc_matched.begin()+1]=*it?1.0:0.0;
    }
    if (int(V.size())==2*cnt)
        glp_ios_heur_sol(tree,coeff);
}

/* construct a tour using the method of Christofides for symmetric TSP */
void graphe::tsp::christofides(ivector &hc) {
    hc.clear();
    int n=sg<0?nv:sg_nv,m=sg<0?ne:sg_ne,ei;
    /* create spanning tree T of the subgraph with index sg */
    graphe T(G->giac_context());
    G->minimal_spanning_tree(T,sg);
    ivector V;
    for (int i=0;i<n;++i) {
        if ((T.degree(i)%2)!=0)
            V.push_back(G->node_index(T.node_label(i)));
    }
    /* the number of odd degree vertices in T must be even */
    assert(V.size()>0 && (V.size()%2)==0);
    /* get edges in SG\T connecting the vertices from V,
     * these form a bipartite graph B(V,E) */
    ivector eind,matched_arcs;
    dvector weights;
    for (int k=0;k<m;++k) {
        ei=sg<0?k:sg_edges[k];
        const arc &a=arcs[ei];
        if (find(V.begin(),V.end(),a.head)!=V.end() &&
                find(V.begin(),V.end(),a.tail)!=V.end() &&
                !T.has_edge(T.node_index(G->node_label(a.tail)),
                            T.node_index(G->node_label(a.head)))) {
            eind.push_back(ei);
            weights.push_back(weight(a.tail,a.head));
        }
    }
    if (eind.size()==1)
        matched_arcs.resize(1,0);
    else min_weight_matching_bipartite(eind,weights,matched_arcs);
    assert(2*matched_arcs.size()==V.size());
    /* add matched edges to T and find Eulerian circuit,
     * such exists because T now has all vertex degrees even */
    for (ivector_iter it=matched_arcs.begin();it!=matched_arcs.end();++it) {
        const arc &a=arcs[eind[*it]];
        const attrib &attr=G->edge_attributes(a.tail,a.head);
        T.add_edge(G->node_label(a.tail),G->node_label(a.head),attr);
    }
    ivector etrail; // eulerian trail
    assert(T.find_eulerian_path(etrail) && etrail.front()==etrail.back());
    vector<bool> visited(n,false);
    for (ivector_iter it=etrail.begin();it!=etrail.end();++it) {
        if (visited[*it])
            continue;
        hc.push_back(G->node_index(T.node_label(*it)));
        visited[*it]=true;
    }
    hc.push_back(hc.front());
}

/* improve the tour hc by making k-opt moves */
void graphe::tsp::improve_tour(ivector &hc) {
    lin_kernighan(hc);
    perform_3opt_moves(hc);
    straighten(hc);
}

/* construct an approximate tour, G must be a complete graph */
double graphe::tsp::approx(ivector &hc) {
    assert(is_undir_weighted);
    sg=-1;
    christofides(hc);
    double old_cost=tour_cost(hc),imp_cost;
    improve_tour(hc);
    imp_cost=tour_cost(hc);
    return (1.5*imp_cost/old_cost);
}

/* compute the mean and the standard deviation of the given sample */
void graphe::tsp::sample_mean_stddev(const dvector &sample,double &mean,double &stddev) {
    assert(!sample.empty());
    /* compute the mean */
    double sum=0;
    for (dvector_iter it=sample.begin();it!=sample.end();++it) {
        sum+=*it;
    }
    mean=sum/double(sample.size());
    if (sample.size()==1)
        stddev=0;
    else {
        double d;
        sum=0;
        for (dvector_iter it=sample.begin();it!=sample.end();++it) {
            d=*it-mean;
            sum+=d*d;
        }
        stddev=std::sqrt(sum/double(sample.size()-1));
    }
}

/* branching variable selection technique proposed by Padberg & Rinaldi */
void graphe::tsp::select_branching_variable(glp_tree *tree) {
    glp_prob *lp=glp_ios_get_prob(tree);
    int n=glp_get_num_cols(lp),b=-1;
    double x0=0.0,x1=1.0,xe;
    double par=0.5,rho=3.5,bctol=0.05; // constants
    for (int i=0;i<n;++i) {
        can_branch[i]=glp_ios_can_branch(tree,i+1)!=0;
        xe=xev[i]=glp_get_col_prim(lp,i+1);
        if (xe-0.5>=0 && xe<x1) x1=xe;
        if (xe-0.5<=0 && xe>x0) x0=xe;
        obj[i]=glp_get_obj_coef(lp,i+1);
    }
    ivector I;
    for (int i=0;i<n;++i) {
        xe=xev[i];
        if (can_branch[i] && (1-par)*x0<=xe && xe<=x1+par*(1-x1))
            I.push_back(i);
    }
    assert(!I.empty());
    if (I.size()==1)
        b=I.front();
    else {
        int N=I.size();
        vector<pair<double,int> > sorted_by_obj_coeff;
        ivector ind(N);
        for (ivector_iter it=I.begin();it!=I.end();++it) {
            sorted_by_obj_coeff.push_back(make_pair(obj[*it],*it));
        }
        sort(sorted_by_obj_coeff.begin(),sorted_by_obj_coeff.end());
        dvector data2(N),data1(0);
        for (int i=0;i<N;++i) {
            ind[i]=sorted_by_obj_coeff[i].second;
            data2[i]=sorted_by_obj_coeff[i].first;
        }
        std::reverse(data2.begin(),data2.end());
        double mu1,mu2,sigma1,sigma2;
        int i=1;
        for (;i<N;++i) {
            data1.push_back(data2.back());
            data2.pop_back();
            sample_mean_stddev(data1,mu1,sigma1);
            sample_mean_stddev(data2,mu2,sigma2);
            if (mu1+rho*sigma1<mu2-rho*sigma2)
                break;
        }
        if (i==N) i=0;
        /* the set I2 starts from i in ind */
        ivector I2(ind.begin()+i,ind.end());
        assert(!I2.empty());
        N=I2.size();
        double dist,mindist=DBL_MAX;
        int j0;
        for (int j=0;j<N;++j) {
            if ((dist=std::abs(0.5-xev[I2[j]]))<mindist) {
                mindist=dist;
                j0=j;
            }
        }
        x0=xev[I2[j0]];
        double c,maxc=0.0;
        for (ivector_iter it=I2.begin();it!=I2.end();++it) {
            xe=xev[*it];
            if (xe>=x0-bctol && xe<=x0+bctol && (c=obj[*it])>maxc) {
                maxc=c;
                b=*it;
            }
        }
    }
    assert(b>=0);
    /* Padberg & Rinaldi suggest exploring the upper branch first */
    glp_ios_branch_upon(tree,b+1,GLP_UP_BRNCH);
}

/* row generation (originally written by Andrew Makhorin) */
void graphe::tsp::rowgen(glp_tree *tree) {
    if (mincut_data.empty())
        mincut_data.resize(22);
    glp_prob *prob=glp_ios_get_prob(tree);
    int i,j,k,m=sg<0?ne:sg_ne,n=sg<0?nv:sg_nv,nedg=0,nz;
    double sum,xk;
    for (k=0;k<m;k++) {
        if (glp_get_col_prim(prob,k+1)>=.001)
            nedg++;
    }
    /* build the capacitated network */
    assert(nedg<=m);
    ivector &beg=mincut_data[14]; if (int(beg.size())<nedg) beg.resize(nedg);
    ivector &end=mincut_data[15]; if (int(end.size())<nedg) end.resize(nedg);
    ivector &cap=mincut_data[16]; if (int(cap.size())<nedg) cap.resize(nedg);
    nz=0;
    for (k=0;k<m;k++) {
        if ((xk=glp_get_col_prim(prob,k+1))>=.001) {
            const arc &a=arcs[sg<0?k:sg_edges[k]];
            beg[nz]=vertex_index(a.tail)+1; end[nz]=vertex_index(a.head)+1;
            rlx_sol_map[a.tail][a.head]=xk;
            cap[nz++]=std::ceil(1000*xk); // scale edge capacities to make them integral
        }
    }
    assert(nz==nedg);
    ivector &cut=mincut_data[17]; if (int(cut.size())<n) cut.resize(n);
    minimal_cut(n,nedg,beg,end,cap,cut); // find minimal cut in the capacitated network
    nedg=0; sum=0;
    for (i=0;i<n;i++) {
        for (j=i+1;j<n;j++) {
            if ((cut[i] && !cut[j]) || (!cut[i] && cut[j])) {
                nedg++;
                sum+=rlx_sol_map[sg<0?i:sg_vertices[i]][sg<0?j:sg_vertices[j]];
            }
        }
    }
    /* if the (unscaled) capacity of min cut is less than 2, the
     * corresponding subtour elimination constraint is violated */
    if (sum<=1.999) {
        nz=0;
        for (i=0;i<n;i++) {
            for (j=i+1;j<n;j++) {
                if ((cut[i] && !cut[j]) || (!cut[i] && cut[j])) {
                    nz++;
                    assert(nz<=nedg);
                    indices[nz]=edge_index(make_pair(sg<0?i:sg_vertices[i],sg<0?j:sg_vertices[j]))+1;
                    coeff[nz]=1.0;
                }
            }
        }
        assert(nz==nedg);
        i=glp_add_rows(prob,1);
        glp_set_row_bnds(prob,i,GLP_LO,2.0,0.0);
        glp_set_mat_row(prob,i,nz,indices,coeff);
    }
}

/* MIP callback routine */
void graphe::tsp::callback(glp_tree *tree,void *info) {
    tsp *tsprob=static_cast<tsp*>(info);
    switch (glp_ios_reason(tree)) {
    case GLP_IHEUR:
        tsprob->heur(tree);
        break;
    case GLP_IBRANCH:
        if (tsprob->is_undir_weighted)
            tsprob->select_branching_variable(tree);
        break;
    case GLP_IROWGEN:
        if (tsprob->is_symmetric_tsp)
            tsprob->rowgen(tree);
        break;
    case GLP_IBINGO:
        if (tsprob->is_symmetric_tsp && tsprob->sg>=0)
            tsprob->get_subtours();
        break;
    default:
        break;
    }
}

/* min weight perfect matching solver callback routine */
void graphe::tsp::min_wpm_callback(glp_tree *tree,void *info) {
    pair<const ivector*,tsp*> *origin=static_cast<pair<const ivector*,tsp*>*>(info);
    const ivector *eind=origin->first;
    tsp *tsprob=origin->second;
    //glp_prob *lp=glp_ios_get_prob(tree);
    //int n=glp_get_num_cols(lp);
    switch (glp_ios_reason(tree)) {
    case GLP_IHEUR:
        tsprob->min_wpm_heur(tree,*eind);
        break;
    default:
        break;
    }
}

#endif
/*
* END OF TSP CLASS IMPLEMENTATION
*/

/* try to find Hamiltonian circuit, return 0 if the graph is not Hamiltonian,
* else store the circuit in h and return 1, if unable to solve return -1 */
int graphe::find_hamiltonian_cycle(ivector &h,double &cost,bool approximate) {
#ifdef HAVE_LIBGLPK
    tsp t(this);
    if (approximate) {
        double ratio=t.approx(h);
        message("The tour cost is within %d%% of the optimal value",
                std::round((ratio-1.0)*100.0));
        cost=t.tour_cost(h);
        return 1;
    }
    return t.solve(h,cost);
#else
    message("Error: GLPK library is required for solving traveling salesman problem");
    return -1;
#endif
}

/* for each edge of the graph, assign the distance between endpoints as a weight */
bool graphe::make_euclidean_distances() {
    assert(!is_weighted() && !is_directed());
    layout x;
    if (!has_stored_layout(x))
        return false;
    if (!x.empty()) {
        int n=node_count();
        set_weighted(true);
        point pq(x.front().size());
        for (int i=0;i<n;++i) {
            const point &p=x[i];
            for (int j=i+1;j<n;++j) {
                const point &q=x[j];
                set_edge_attribute(i,j,_GT_ATTRIB_WEIGHT,gen(point_distance(p,q,pq)));
            }
        }
    }
    return true;
}

#ifndef NO_NAMESPACE_GIAC
}
#endif // ndef NO_NAMESPACE_GIAC
