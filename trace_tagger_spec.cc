#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <apertium/tsx_reader.h>
#include <apertium/utf_converter.h>
#include "print_tagger_data.h"
#include "find_similar_ambiguity_class_repl.h"

using namespace std;

int main(int argc, char *argv[])
{
  if (argc < 2) {
    cerr<<"Usage: "<<argv[0]<<" <tsx file>\n";
    exit(-1);
  }

  // Read data
  TSXReader tsx_reader;
  tsx_reader.read(argv[1]);
  TaggerData& tagger_data = tsx_reader.getTaggerData();

  print_tagger_data(tagger_data);
  find_similar_ambiguity_class_repl(tagger_data);
}
