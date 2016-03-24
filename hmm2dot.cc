#include <iostream>
#include <fstream>
#include <iomanip>
#include <apertium/tagger_data_hmm.h>
#include <apertium/utf_converter.h>

using namespace std;

TaggerDataHMM tagger_data_hmm;

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
  FILE* fin = NULL;

  fin = fopen(probfile, "r");
  if (!fin) {
    cerr<<"Error: cannot open file '"<<probfile<<"'\n";
    exit(-1);
  }
  tagger_data_hmm.read(fin);
  fclose(fin);

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
    fmsm << "q" << (tagi - array_tags.begin()) << "[label=\"" << UtfConverter::toUtf8(*tagi) << "\"];\n";
  }
  // Edges
  int i, j;
  for (i=0;i<N;i++) {
    for (j=0;j<N;j++) {
      if (i == j) continue;
      double weight = a[i][j];
      if (weight < threshold) continue;
      fmsm << "q" << i << " -> " << "q" << j \
        << fixed << setw(5) << setprecision(5) \
        << " [penwidth=" << 0.5 + 4 * weight \
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
  fobs << "overlap=expandxy;\n";
  // Nodes & dummy edges
  fobs << "edge [style=invis, weight=50, len=.2]";
  fobs << "subgraph cluster_left {\n";
  fobs << "left [pos=\"-1,0!\", color=red, style=invis];\n";
  for (vector<wstring>::const_iterator tagi=array_tags.begin(); tagi!=array_tags.end(); tagi++) {
    i = tagi - array_tags.begin();
    fobs << "q" << i << "[label=\"" << UtfConverter::toUtf8(*tagi) << "\"];\n";
  }
  for (vector<wstring>::const_iterator tagi=array_tags.begin(); tagi!=array_tags.end(); tagi++) {
    i = tagi - array_tags.begin();
    fobs << "q" << i << " -- left;\n";
  }
  fobs << "}\n";
  fobs << "subgraph cluster_right {";
  fobs << "right [pos=\"1,0!\", color=blue, style=invis];";
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
    fobs << "}\"]\n";
  }
  for (i=0;i<M;i++) {
    fobs << "o" << i << " -- right;\n";
  }
  fobs << "}\n";
  fobs << "edge [style=\"\", weight=1, len=10]";
  // Edges
  for (i=0;i<N;i++) {
    for (j=0;j<M;j++) {
      double weight = a[i][j];
      if (weight < threshold) continue;
      fobs << "q" << i << " -- " << "o" << j \
        << fixed << setw(5) << setprecision(5) \
        << " [penwidth=" << 1 + 2 * weight \
        << " color=\"#000000" << hex << (int)(weight * 255) << dec << "\"" \
        << "];\n";
    }
  }
  fobs << "}\n";
  fobs.close();
}
