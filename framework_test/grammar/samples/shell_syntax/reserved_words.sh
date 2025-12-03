#!/bin/sh
# if/then/else/fi
if [ -n "$1" ]; then
  echo "arg present"
else
  echo "no arg"
fi

# for/do/done
for i in 1 2 3; do
  echo "i=$i"
done

# case/esac
case "$1" in
  start) echo started ;;
  stop) echo stopped ;;
  *) echo default ;;
esac

# while/done
count=0
while [ $count -lt 2 ]; do
  count=$((count + 1))
  echo "count=$count"
done
