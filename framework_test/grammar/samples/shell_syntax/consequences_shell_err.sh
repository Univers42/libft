#!/bin/bash
# Show pipeline and subshell exit-status consequences (bash features)

echo "without pipefail: pipeline exit is last command"
false | true
echo "pipeline status (no pipefail): $?"

set -o pipefail
echo "with pipefail: pipeline fails if any element fails"
false | true
echo "pipeline status (with pipefail): $?"

# subshell exit code
( false )
echo "subshell status: $?"

# && and || control flow examples
false && echo "this won't run"
false || echo "this runs because previous failed"
