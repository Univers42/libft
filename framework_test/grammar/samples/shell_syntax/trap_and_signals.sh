#!/bin/sh
trap 'echo Trapped; exit 1' INT TERM
echo "Sleeping for 1 (send SIGINT to test)"
sleep 1
