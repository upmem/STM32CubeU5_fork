export commit="$(git log --format='%h' HEAD -n1)"
export DIRTY=$(git status -uno |  grep "modified:" |  wc -l)
if [ 0 -eq $DIRTY ]; then
    echo "uint8_t commit[]=\"0x$commit\";" >../../../NonSecure/Inc/commit.h
else
    echo "uint8_t commit[]=\"0x$commit dirty\";" >../../../NonSecure/Inc/commit.h
fi
