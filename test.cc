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
  graph_power_demo(&ct);
  return 0;
}
