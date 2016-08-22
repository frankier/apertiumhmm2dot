#include <iostream>
#include <locale>
#include <apertium/analysis.h>
#include <apertium/basic_tagger.h>
#include <apertium/stream.h>

using namespace Apertium;

int main(int argc, char *argv[])
{
  std::locale::global(std::locale(""));
  basic_Tagger::Flags flags;
  Stream stream(flags);
  while (true) {
    StreamedType bit = stream.get();
    std::wcout << L"\"" << bit.TheString << L"\"";

    if (!bit.TheLexicalUnit) {
      std::wcout << L"No LexicalUnit";
      if (!stream.flush_())
        break;

      std::wcout << std::flush;
      continue;
    }

    std::wcout << L": ";
    std::vector<Analysis> &analyses = bit.TheLexicalUnit->TheAnalyses;
    std::wcout << bit.TheLexicalUnit->TheSurfaceForm;
    std::vector<Analysis>::const_iterator ai = analyses.begin();
    for (;ai!=analyses.end();ai++) {
      std::vector<Morpheme>::const_iterator mi = ai->TheMorphemes.begin();
      std::wcout << L"[";
      for (;mi!=ai->TheMorphemes.end();mi++) {
        std::wcout << mi->TheLemma;
        std::wcout << L"(";
        std::vector<Tag>::const_iterator ti = mi->TheTags.begin();
        for (;ti!=mi->TheTags.end();ti++) {
          std::wcout << L" / ";
          std::wcout << ti->TheTag;
        }
        std::wcout << L") ";
      }
      std::wcout << L"] ";
    }
    std::wcout << std::endl;

    if (stream.flush_())
      std::wcout << std::flush;
  }
}

