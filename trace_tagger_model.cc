#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <apertium/tagger_data_hmm.h>
#include <apertium/utf_converter.h>
#include "print_tagger_data.h"
#include "find_similar_ambiguity_class_repl.h"

using namespace std;

int main(int argc, char *argv[])
{
  if (argc < 2) {
    cerr<<"Usage: "<<argv[0]<<" <probfile>\n";
    exit(-1);
  }
  char* probfile = argv[1];

  // Read data
  TaggerDataHMM tagger_data_hmm;
  FILE* fin = fopen(probfile, "r");
  if (!fin) {
    cerr<<"Error: cannot open file '"<<probfile<<"'\n";
    exit(-1);
  }
  tagger_data_hmm.read(fin);
  fclose(fin);

  print_tagger_data(tagger_data_hmm);
  find_similar_ambiguity_class_repl(tagger_data_hmm);
}
