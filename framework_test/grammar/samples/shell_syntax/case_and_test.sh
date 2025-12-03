#!/bin/sh
case "$1" in
  start) echo start ;;
  stop) echo stop ;;
  *) echo unknown ;;
esac

if test -f /etc/passwd; then
  echo "passwd exists"
fi
