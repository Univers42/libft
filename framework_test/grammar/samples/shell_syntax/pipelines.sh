#!/bin/sh
# basic pipeline: stdout of each command feeds the next
echo -e "one\ntwo\nthree" | grep "o" | wc -l

# pipeline with redirection at the end
echo "a b c" | awk '{print $2}' > /tmp/_pipeline_field

# split a pipeline with tee so output goes to file and next stage
printf "alpha\nbeta\n" | tee /tmp/_pipeline_tee | sed -n '1p'

# pipeline producing stderr and stdout combined
( echo out; echo err 1>&2 ) |& sed -n '1,2p' 2>/dev/null  # use |& where supported (bash/ksh/zsh); safe fallback may vary

# capture exit status of pipeline (bash-specific behavior with PIPESTATUS array not POSIX)
# POSIX shells return status of last command in pipeline by default
false | true
echo "pipeline status (last command): $?"

# simple multi-stage pipeline in background
( echo "bg pipeline" | tr '[:lower:]' '[:upper:]' > /tmp/_bg_pipeline ) &
bg=$!
wait "$bg"
echo "bg pipeline pid=$bg done"
