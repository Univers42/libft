#!/bin/sh
# append using >>
printf 'line1\n' >> /tmp/_append_test
printf 'line2\n' >> /tmp/_append_test

# append both stdout and stderr
{
  echo "stdout append"
  ls /nonexistent 2>&1
} >> /tmp/_append_test 2>&1

# using tee to append (preserve stream and append to file)
echo "tee-append-line" | tee -a /tmp/_append_test >/dev/null
