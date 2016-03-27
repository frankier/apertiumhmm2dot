#include <stdlib.h>

set<TTag> find_similar_ambiguity_class(TaggerData &td, set<TTag> &c)
{
  int size_ret = -1;
  set<TTag> ret=td.getOpenClass(); //Se devolverá si no encontramos ninguna clase mejor
  bool skeep_class;
  Collection &output = td.getOutput();

  for(int k=0; k<output.size(); k++) {
    if ((((int)output[k].size())>((int)size_ret)) && (((int)output[k].size())<((int)c.size()))) {
      skeep_class=false;
      // Test if output[k] is a subset of class
      for(set<TTag>::const_iterator it=output[k].begin(); it!=output[k].end(); it++) {
        if (c.find(*it)==c.end()) { 
	   skeep_class=true; //output[k] is not a subset of class
	   break;
	}
      }
      if (!skeep_class) {
        size_ret = output[k].size();
	     ret = output[k];
      }
    }
  }
  return ret;
}

void find_similar_ambiguity_class_repl(TaggerData &td)
{
  vector<wstring> array_tags = td.getArrayTags();
  string line = "";
  while (true) {
    cout << "> ";
    if (!getline(cin, line, '\n')) {
      break;
    }
    stringstream lineStream(line);
    set<TTag> ambiguity_class;
    TTag ttag;
    while (lineStream >> ttag) {
      if (ttag < 0 || ttag > array_tags.size()) {
        cerr << "Out of range!\n";
        exit(EXIT_FAILURE);
      }
      ambiguity_class.insert(ttag);
    }
    print_ambiguity_class(array_tags, ambiguity_class);
    set<TTag> similar_ambiguity_class = find_similar_ambiguity_class(td, ambiguity_class);
    cout << " -> ";
    print_ambiguity_class(array_tags, similar_ambiguity_class);
    cout << "\n";
  }
}
