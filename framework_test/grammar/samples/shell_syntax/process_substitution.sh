#!/bin/bash
# process substitution examples
diff <(echo a; sleep 0) <(printf 'a')
