#include "graphtheory.h"
#include <giac/giac.h>
#include <sstream>

using namespace std;
using namespace giac;

int main(){
  gen g;
  context ct;
  stringstream ss;
  ss << "maple_mode(1)";
  ss >> g;
  _eval(g,&ct);
  //g=_lcf_graph(makesequence(makevecteur(-13,-9,7,-7,9,13),5),&ct);
  //g=_import_graph(graphe::str2gen("graph-test2",true),&ct);
  //g=_cycle_graph(20,&ct);
  //g=_seidel_switch(makesequence(g,makevecteur(2,3,5,7)),&ct);
  //g=_sierpinski_graph(makesequence(3,3,at_triangle),&ct);
  g=_import_graph(graphe::str2gen("testgraph",true),&ct);
  string disp;
  if (is_graphe(g,disp,&ct))
      cout << disp << endl;
  else cout << "not a graph: " << g << endl;
  cout << "Cut vertices: " << _articulation_points(g,&ct) << endl;
  cout << _draw_graph(g,&ct) << endl;
  //gen v=_maximum_matching(g,&ct);
  //cout << v << endl;
  return 0;
}
