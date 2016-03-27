void print_ambiguity_class(const vector<wstring> &array_tags, const set<TTag> &abgset)
{
  int j;
  set<TTag>::const_iterator abgseti;
  cout << "(size: " << abgset.size() << "; ";
  for (abgseti=abgset.begin(), j=0; abgseti!=abgset.end(); abgseti++, j++) {
    cout << *abgseti << ": ";
    cout << UtfConverter::toUtf8(array_tags[*abgseti]);
    if (j < abgset.size() - 1) {
      cout << ", ";
    }
  }
  cout << ")";
}

void print_tagger_data(TaggerData &td)
{
  int i;
  map<wstring, TTag, Ltstr> tag_index = td.getTagIndex();
  vector<wstring> array_tags = td.getArrayTags();
  cout << "Tagger level POS tags:\n";
  for (i=0;i<array_tags.size();i++) {
    cout << i << ": " << UtfConverter::toUtf8(array_tags[i]) << "\n";
  }
  Collection &output = td.getOutput();
  cout << "Ambiguity sets:\n";
  for (i=0;i<output.size();i++) {
    cout << i << ": ";
    print_ambiguity_class(array_tags, output[i]);
    cout << "\n";
  }

  set<TTag> open_class = td.getOpenClass();
  cout << "Open class:\n" << output[open_class] << ": ";
  print_ambiguity_class(array_tags, open_class);
  cout << "\n";
}
