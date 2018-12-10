#!/bin/bash

## Fasten ur seatbelts, bois
set -eu -o pipefail

MAX_GUARD_ID=3000;

## The scorecard is a big image. It keeps track of guards' sleep minutes by
## means of pixel grey values
convert -size 60x"$MAX_GUARD_ID" xc:black -depth 8 scorecard.pgm;

cat input.txt | sort > input-sorted.txt;

## Fill the scorecard
while read line; do
  echo "$line";
  minute=`echo "$line" | cut -d':' -f2 | cut -d']' -f1`;
  if   test 1 -eq `echo "$line" | grep -c "Guard" -`; then
    ID=`echo "$line" | cut -d'#' -f2 | cut -d' ' -f1;`
  elif test 1 -eq `echo "$line" | grep -c "asleep" -`; then
    ## Note sleep start
    SLEEP_START=$minute;
  else
    ## Draw on scorecard
    convert -size 60x"$MAX_GUARD_ID" xc:black -fill '#111' -draw "rectangle $SLEEP_START,$ID `expr $minute - 1`,$ID" tmp.pgm;
    convert scorecard.pgm tmp.pgm -compose Plus -composite scorecard.pgm;
  fi
done < input-sorted.txt;

## Find the guard that sleeps the most by summing up the non-black pixels in
## that guard's scorecard line
sleepiest_guard=0;
sleepiest_guard_sleep=0;
i=0;
while test $i -lt $MAX_GUARD_ID; do
  if test 0 -eq `expr $i % 100`; then
    echo $i;
  fi
  convert scorecard.pgm -crop 60x1+0+"$i" +repage tmp.pgm;
  slept=`convert tmp.pgm -format %c histogram:info:- \
         | tail -n +2                                \
         | cut -d':' -f1                             \
         | awk '{n+=1;sum+=$1*n}END{print sum}'`;
  if test $slept -gt $sleepiest_guard_sleep ; then
    sleepiest_guard=$i;
    sleepiest_guard_sleep=$slept;
  fi
  i=`expr $i + 1`;
done
printf "Guard #%s sleeps for %s minutes\n" $sleepiest_guard $sleepiest_guard_sleep;

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



