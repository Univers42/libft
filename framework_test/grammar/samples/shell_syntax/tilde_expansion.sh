#!/bin/sh
# tilde expansion examples (may depend on system)
echo "home via ~: ~ expands to: $(echo ~)"
echo "current dir short: ~+ -> $(echo ~+)"
echo "previous dir short: ~- -> $(echo ~-)"
# user home (may not resolve in some minimal test environments)
# echo "root home: ~root -> $(echo ~root)"
