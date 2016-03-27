#include <iostream>
#include <fstream>
#include <iomanip>
#include <apertium/tagger_data_hmm.h>
#include <apertium/utf_converter.h>

using namespace std;


int main(int argc, char *argv[])
{
  if (argc < 4) {
    cerr<<"Usage: "<<argv[0]<<" <threshold> <probfile> <output markov state machine dot> <output observeration dot>\n";
    exit(-1);
  }
  char* probfile = argv[1];
  double threshold = strtod(argv[2], NULL);
  char* msmdot = argv[3];
  char* obsdot = argv[4];

  FILE* fin = fopen(probfile, "r");
  TaggerDataHMM tagger_data_hmm;
  if (!fin) {
    cerr<<"Error: cannot open file '"<<probfile<<"'\n";
    exit(-1);
  }
  tagger_data_hmm.read(fin);
  fclose(fin);

  int i, j;
  vector<wstring> array_tags;
  double **a = tagger_data_hmm.getA();
  double **b = tagger_data_hmm.getB();
  int N = tagger_data_hmm.getN();
  int M = tagger_data_hmm.getM();
  array_tags = tagger_data_hmm.getArrayTags();
  Collection &output = tagger_data_hmm.getOutput();
  cerr << "POS tags: " << array_tags.size() << "\n";
  cerr << "HMM states: " << N << "\n";
  cerr << "Abiguity sets: " << M << "\n";

  // Hidden part of HMM
  ofstream fmsm;
  fmsm.open(msmdot);
  fmsm << "digraph g {\n";
  fmsm << "splines=true;\n";
  fmsm << "overlap=prism;\n";
  fmsm << "node [margin=\"0,0\"];\n";
  // Nodes
  for (vector<wstring>::const_iterator tagi=array_tags.begin(); tagi!=array_tags.end(); tagi++) {
    i = tagi - array_tags.begin();
    fmsm << "q" << (tagi - array_tags.begin())
      << "[label=\"" << UtfConverter::toUtf8(*tagi) << "\"";
    if (wcscmp((*tagi).c_str(), L"TAG_SENT") == 0) {
      fmsm << " style=filled fillcolor=red";
    } else if (i > N) {
      fmsm << " style=filled fillcolor=blue";
    }
    fmsm << "];\n";
  }
  // Edges
  for (i=0;i<N;i++) {
    for (j=0;j<N;j++) {
      if (i == j) continue;
      double weight = a[i][j];
      if (weight < threshold) continue;
      fmsm << "q" << i << " -> " << "q" << j \
        << fixed << setw(5) << setprecision(5) \
        << " [penwidth=" << 5 * weight \
        << " color=\"#000000" << hex << (int)(weight * 255) << dec << "\"" \
        << "];\n";
    }
  }
  fmsm << "}\n";
  fmsm.close();

  // Draw observations
  ofstream fobs;
  fobs.open(obsdot);
  fobs << "graph g {\n";
  fobs << "rankdir=LR;\n";
  fobs << "ranksep=10;\n";
  fmsm << "node [margin=\"0,0\"];\n";
  // Nodes
  for (vector<wstring>::const_iterator tagi=array_tags.begin(); tagi!=array_tags.end(); tagi++) {
    i = tagi - array_tags.begin();
    fobs << "q" << i << "[label=\"" << UtfConverter::toUtf8(*tagi) << "\"];\n";
  }
  for (i=0;i<M;i++) {
    fobs << "o" << i << "[label=\"{";
    set<TTag> abgset = output[i];
    j = 0;
    for (set<TTag>::const_iterator abgseti=abgset.begin(); abgseti!=abgset.end(); abgseti++, j++) {
      fobs << UtfConverter::toUtf8(array_tags[*abgseti]);
      if (j < abgset.size() - 1) {
        fobs << ", ";
      }
    }
    fobs << "}\"];\n";
  }
  // Edges
  int curedge = 0;
  for (i=0;i<N;i++) {
    for (j=0;j<M;j++) {
      double weight = b[i][j];
      curedge++;
      if (weight < threshold) continue;
      fobs << "q" << i << " -- " << "o" << j \
        << fixed << setw(5) << setprecision(5) \
        << " [penwidth=" << 0 + 20 * weight \
        << " color=\"" << (double)i / N << " 1 1\"" \
        << "];\n";
    }
  }
  fobs << "}\n";
  fobs.close();
}
