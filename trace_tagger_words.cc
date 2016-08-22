#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <apertium/tsx_reader.h>
#include <apertium/morpho_stream.h>

int main(int argc, char *argv[])
{
  if (argc < 2) {
    cerr<<"Usage: "<<argv[0]<<" <tsx file>\n";
    exit(-1);
  }

  // Read data
  TSXReader tsx_reader;
  tsx_reader.read(argv[1]);
  TaggerData& td = tsx_reader.getTaggerData();
  TaggerWord::setArrayTags(tsx_reader.getTaggerData().getArrayTags());

  MorphoStream morpho_stream(stdin, true, &td);
  const TTag keof = (td.getTagIndex())[L"TAG_kEOF"];
  while (1) {
    auto_ptr<TaggerWord> tw(morpho_stream.get_next_word());
    if (tw.get() == NULL) {
      break;
    }
    wcout << tw.get()->get_superficial_form() << L" -- "
          << tw.get()->get_string_tags() << L" -- ";

    set<TTag> tags = tw.get()->get_tags();
    set<TTag>::iterator ti = tags.begin();
    for(;ti!=tags.end();ti++) {
      TTag tag = *ti;
      wcout << tw.get()->get_lexical_form(tag, keof)
            << L", "
            << tw.get()->get_lexical_form_without_ignored_string(tag, keof);
    }
    wcout << endl;
  }
}
