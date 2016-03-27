What?
=====

This program draws Apertium's HMM models in a way inspired by
https://en.wikipedia.org/wiki/Hidden_Markov_model#Inference

Requirements
------------

* Apertium's library files and header files.
* An Apertium pair with trained HMM tagger data.

Usage
-----

    $ make
    $ ./hmm2dot.sh /path/to/an/apertium/tagger/model.prob

Then resulting files are:

1. msm.svg: the internal Markov model. Nodes are "coarse" part-of-speech tags.
   Edge weights reflect the probability of one part of speech following
   another.
2. obs.svg: a bipartite graph reflecting the probability a certain ambiguity
   set will be observed given a particular actual part of speech tag.

I have tested with apertium-en-ca/en-ca.prob.

If you're using Debian or Ubuntu's Graphviz, layout will be poor due to being
compiled without a triangulation library. It is (slightly) better with one.
Install libgts-dev and then compile GraphViz yourself for a better layout. If
anyone can coax Graphviz into producing more readable layouts, a pull request
would be very welcome!

Also included
-------------

* trace-tagger-model - dump some info about a hmm tagger model
* trace-tagger-spec - unfinished

Caveats/TODO
------------

* Versions and paths are hardcoded and may need to be changed in the Makefile
  manually at the moment.
* Should parse command line options better.
* Shouldn't put implementation in header files.
* Different Graphviz options could possibly produce better graph layout.
