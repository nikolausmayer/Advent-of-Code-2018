#!/bin/bash

## Fasten ur seatbelts, bois
set -eu -o pipefail

echo "Please run part-1.sh before (we need the results)."
if test ! -f "scorecard.pgm"; then
  exit `false`;
fi

MAX_GUARD_ID=3000;

## Find the line with the brightest pixel in the scorecard "image"
i=0;
sleepiest_guard=0;
maxval=0;
while test $i -lt $MAX_GUARD_ID; do
  if test 0 -eq `expr $i % 100`; then
    echo $i;
  fi
  convert scorecard.pgm -crop 60x1+0+"$i" +repage tmp.pgm;
  val=`convert tmp.pgm -format %c histogram:info:- \
       | tail -n 2                                 \
       | head -n 1                                 \
       | cut -d'(' -f2                             \
       | cut -d',' -f1`;
  if test $val -gt $maxval; then
    sleepiest_guard=$i;
    maxval=$val;
  fi
  i=`expr $i + 1`;
done

## Same as in part 1: 
## In the sleepiest guard's scorecard line, find the pixel that, if removed,
## changes the brightest color in the histogram. This pixel is the most-slept
## minute.
convert scorecard.pgm -crop 60x1+0+"$sleepiest_guard" +repage tmp.pgm;
maxcol=`convert tmp.pgm -format %c histogram:info:- \
        | tail -n 2                                 \
        | head -n 1                                 \
        | cut -d'(' -f2                             \
        | cut -d')' -f1`;
i=0;
while test $i -lt 60; do
  convert tmp.pgm -crop `expr 60 - $i`x1+$i+0 +repage tmp2.pgm;
  if test `convert tmp2.pgm -format %c histogram:info:- | grep -c "$maxcol"` -eq 0; then
    themin=`expr $i - 1`;
    printf "Max sleep minute is %s\n" $themin;
    printf "Answer is %s * %s = %s" $sleepiest_guard $themin `expr $sleepiest_guard \* $themin`;
    exit `true`;
  fi
  i=`expr $i + 1`;
done



