#!/bin/bash

##
# Nikolaus Mayer, 2018 (github.com/nikolausmayer)
##

## Fasten ur seatbelts, bois
set -eu -o pipefail

INPUT="input.txt";
LENGTH=`wc -l "$INPUT" | cut -d' ' -f1`;
FIRST_INDEX=1;

## Set IFS to empty to prevent collapsing the output of "tail"
## into a single line (for the loop input)
IFS=;

while test $FIRST_INDEX -lt $LENGTH; do
  printf "Checking %3d/%d\n" "$FIRST_INDEX" "$LENGTH";
  ## Just to be fancy, do not check mirror pairs
  TAIL_LENGTH=`expr $LENGTH - $FIRST_INDEX`;
  A=`head -n $FIRST_INDEX $INPUT | tail -n 1`;
  while read B; do
    ## This is the good part: the regex matches if two space-separated
    ## words differ by zero or one characters.
    if test "1" = `echo "$A $B" | grep -c -G "^\(.*\).\(.*\) \1.\2$"`; then
      printf "Got it!  %s  %s\n" "$A" "$B";
      printf "Common characters:\n";
      echo "$A $B" | sed 's/^\(.*\).\(.*\) \1.\2$/\1\2/';
      exit `:`;
    fi
  done <<< `tail -n $TAIL_LENGTH $INPUT`;
  FIRST_INDEX=`expr $FIRST_INDEX + 1`;
done



