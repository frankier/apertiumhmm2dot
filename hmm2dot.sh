./hmm2dot $1 0.001 msm.dot obs.dot
>&2 echo "Generating graphs"
neato -Tsvg msm.dot -o msm.svg
dot -Tsvg obs.dot -o obs.svg
