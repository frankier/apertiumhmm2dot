#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cwchar>
#include <apertium/tagger_data_hmm.h>
#include <apertium/utf_converter.h>

void print_ambiguity_class(const vector<wstring> &array_tags, const set<TTag> &abgset)
{
  int j;
  set<TTag>::const_iterator abgseti;
  wcout << L"(size: " << abgset.size() << L"; ";
  for (abgseti=abgset.begin(), j=0; abgseti!=abgset.end(); abgseti++, j++) {
    wcout << *abgseti << L": ";
    wcout << array_tags[*abgseti];
    if (j < abgset.size() - 1) {
      wcout << L", ";
    }
  }
  wcout << L")";
}

void print_tagger_data(TaggerData &td)
{
  int i;
  map<wstring, TTag, Ltstr> tag_index = td.getTagIndex();
  vector<wstring> array_tags = td.getArrayTags();
  wcout << "Tagger level POS tags:\n";
  for (i=0;i<array_tags.size();i++) {
    wcout << i << L": " << array_tags[i] << L"\n";
  }
  Collection &output = td.getOutput();
  wcout << L"Ambiguity sets:\n";
  for (i=0;i<output.size();i++) {
    wcout << i << L": ";
    print_ambiguity_class(array_tags, output[i]);
    wcout << L"\n";
  }

  set<TTag> open_class = td.getOpenClass();
  wcout << L"Open class:\n" << output[open_class] << L": ";
  print_ambiguity_class(array_tags, open_class);
  wcout << "\n";

  PatternList &pl = td.getPatternList();
  wcout << L"Fine grain tags:\n";
  const Alphabet &alpha = pl.getAlphabet();
  int size = alpha.size();
  for (int i=1;i<=size;i++) {
    wcout << i << L": " << flush;
    alpha.writeSymbol(-i, stdout);
    wcout << flush << L"\n";
  }
}
