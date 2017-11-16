#ifndef GETOPT
#define GETOPT

#include <iostream>
#include <getopt.h>

using namespace std;

void run_getopts(int argc, char* argv[]);
void validtype(string& mode_in);

static struct option longopts[] = {
  {"mode",  required_argument,  nullptr, 'm'},
  {"help",        no_argument,  nullptr, 'h'},
  {nullptr,                 0,  nullptr, '\0'}
};

void run_getopts(int argc, char* argv[], string& mode){
  int idx = 0;
  int c;
  string mode_in;

  //check for non-flags
  mode_in = argv[1];
  if(mode_in[0] != '-'){
    cerr << "Mode flag required\nPlease use -m or --mode" << endl;
    exit(1);
  }
  
  //run getopts on flags
  while((c = getopt_long(argc, argv, "m:h", longopts, &idx)) != -1){
    switch(c){
      case 'm':
        mode_in = optarg;
        validtype(mode_in);
        mode = mode_in;
        break;

      case 'h':
        cerr << "\n\nWelcome to the Zoo!" << endl;
        cerr << "This program will help you find the best route to\n";
        cerr << "feed the animals.\n";
        cerr << "Run program with '-m' or '--mode' with one of the\nfollowing:\n";
        cerr << "\n    MST - Find an optimal path to feed animals food\n";
        cerr << "    FASTTSP - Find a good path for water canal\n";
        cerr << "    OPTTSP - Find THE BEST path for water canal\n\n";
        cerr << "...followed by a list of cage coordinates\n";
        cerr << "Have fun!\n\n" << endl;
        break;

      default:
        cerr << "Invalid command line input " << c << endl;
        exit(1);
        break;
    }
  }
}

void validtype(string& mode_in){
  if(mode_in != "MST" && mode_in != "OPTTSP" && mode_in != "FASTTSP"){
    cerr << "Invalid mode " << mode_in;
    cerr << "\nProgram exited" << endl;
    exit(1);
  }
}

#endif //getopt