BASEDIR=$(dirname "$0")
"${BASEDIR}/hmm2dot" "$1" 0.001 msm.dot obs.dot
>&2 echo "Drawing msm graph"
neato -Tsvg msm.dot -o msm.svg
>&2 echo "Drawing obs graph"
dot -Tsvg obs.dot -o obs.svg
