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
  maximum_clique_demo(&ct);
  return 0;
}
