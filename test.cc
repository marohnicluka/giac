#include "graphtheory.h"
#include <giac/giac.h>
#include <sstream>

using namespace std;
using namespace giac;

int main(){
  context ct;
  gen g;
  stringstream ss;
  ss << "maple_mode(1)";
  ss >> g;
  _eval(g,&ct);
  gen spec=graphe::str2gen("[1,2,3,4,5,6,7,8,9,10,11,12]");
  graphe G(&ct);
  G.make_random_flow_network(*spec._VECTptr,100);
  cout << _draw_graph(G.to_gen(),&ct) << endl;
  return 0;
}
