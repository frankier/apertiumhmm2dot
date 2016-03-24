all:
	g++ -I /usr/local/include/apertium-3.4 -I /usr/local/include/lttoolbox-3.3/ hmm2dot.cc -llttoolbox3 -lapertium3 -o hmm2dot
