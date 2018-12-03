#!/bin/bash

##
# Nikolaus Mayer, 2018 (github.com/nikolausmayer)
##

## Fasten ur seatbelts, bois
set -eu -o pipefail

## Counters
LINES_WITH_DUBS=0;
LINES_WITH_TRIPS=0;

## Iterate over input lines
while read line; do
  ## 1. split line into one-line-per-character
  ## 2. sort (1.)
  ## 3. count unique lines in (2.)
  REPS=`echo "$line" | sed 's/\(.\)/\1\n/g' | sort | uniq -c -w1`;

  ## 4. check if (3.) contains "2" (double) or "3" (triple)
  DUBS=`echo "$REPS" | grep -c 2` || true;
  TRIPS=`echo "$REPS" | grep -c 3` || true;

  if test "$DUBS" -ge 1; then
    LINES_WITH_DUBS=`expr $LINES_WITH_DUBS + 1`;
    #echo "line with DOUBLE";
  fi
  if test "$TRIPS" -ge 1; then
    LINES_WITH_TRIPS=`expr $LINES_WITH_TRIPS + 1`;
    #echo "line with TRIPLE";
  fi
done < input.txt;

printf "DUBS * TRIPS = $LINES_WITH_DUBS * $LINES_WITH_TRIPS = `expr $LINES_WITH_DUBS \* $LINES_WITH_TRIPS`\n";



