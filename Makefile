all: hmm2dot testambiguityclass

hmm2dot: hmm2dot.cc
	g++ -Wall -Wextra -Wno-sign-compare -I /usr/local/include/apertium-3.4 -I /usr/local/include/lttoolbox-3.3/ hmm2dot.cc -llttoolbox3 -lapertium3 -o hmm2dot

trace-tagger-model: trace_tagger_model.cc find_similar_ambiguity_class_repl.h  print_tagger_data.h
	g++ -Wall -Wextra -Wno-sign-compare -I /usr/local/include/apertium-3.4 -I /usr/local/include/lttoolbox-3.3/ trace_tagger_model.cc -llttoolbox3 -lapertium3 -o trace-tagger-model

trace-tagger-spec: trace_tagger_spec.cc find_similar_ambiguity_class_repl.h  print_tagger_data.h
	g++ -Wall -Wextra -Wno-sign-compare -I  /usr/include/libxml2 -I /usr/local/include/apertium-3.4 -I /usr/local/include/lttoolbox-3.3/ trace_tagger_spec.cc -llttoolbox3 -lapertium3 -o trace-tagger-spec
