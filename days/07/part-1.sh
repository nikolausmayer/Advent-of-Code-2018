#!/bin/bash

## Fasten ur seatbelts, bois (and girls)
set -eu;


## Make a list of all vertices in the graph (we'll just assume that all letters
## are used, but it really does not matter).
echo "A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z"  \
  | tr ',' '\n'                                             \
  > starters.txt

## Convert "Step C must be finished before step A can begin." into "AC".
constraints=`cat input.txt | cut -b6,37 | rev`;
## This document stores the current solution state of the graph.
printf "" > lists.txt;
## Convert lines "AC", "AB", "AH" into a sorted constraint "A BCH".
while read A; do
  B=`echo "$constraints" | grep "^$A" | cut -c 2 | sort | tr -d '\n'`;
  echo $A $B >> lists.txt;
done < starters.txt

## Now let's walk the graph.
result="";
while `:`; do
  finished=0;
  while read A B; do
    ## "Empty" source nodes have already been processed.
    if test 1 -eq `echo $A | wc -c`; then
      continue;
    fi
    ## "Empty" target nodes have no constraints and can be processed.
    if test 1 -eq `echo $B | wc -c`; then
      echo $A;
      result="${result}$A";
      ## Delete this node from all constraints.
      cat lists.txt | tr -d "$A" > tmp.txt;
      mv tmp.txt lists.txt;
      finished=1;
      ## Breaking the inner loop here guarantees a sorted solution.
      break;
    fi
  done < lists.txt

  if test $finished -eq 0; then
    echo "Result sequence:" $result;
    break;
  fi
done

