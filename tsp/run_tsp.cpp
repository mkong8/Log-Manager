#include <iomanip>

#include "getopt.h"
#include "opttsp.h"

using namespace std;

int main(int argc, char* argv[]){
  string mode;

  ios_base::sync_with_stdio(false); //speed up text output
  cout << setprecision(2); //Always show 2 decimal places
  cout << fixed; //Disable scientific notation for large numbers

  run_getopts(argc, argv, mode);
  if(mode == "MST"){
  	MST_map mst_map(cin);
  	mst_map.prim_alg();
  	//mst_map.print_adjmat();
  	mst_map.print_path();
  }

  if(mode == "FASTTSP"){
    FAST_map fast(cin);
    fast.greedy_alg();
    fast.two_opt(1);
    fast.two_opt(5); //factor of 5 passes
    //fast.print_adjmat();
    fast.print_path();
    //fast.distance_check();
  }

  if(mode == "OPTTSP"){
    OPT_map opt(cin);
    opt.find_path();
    opt.print_path();
  }

  return 0;
}