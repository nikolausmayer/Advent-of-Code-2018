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


echo "default: A B C D E F G H I J K L M N O P Q R S T U V W X Y Z" > Makefile;
echo ""                                                            >> Makefile;

echo "clean:"                                                      >> Makefile;
echo "	rm A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"    >> Makefile;
echo ""                                                            >> Makefile;

echo ".SILENT:"                                                    >> Makefile;
echo ""                                                            >> Makefile;

## Convert lines "AC", "AB", "AH" into a sorted constraint "A BCH".
i=61;
while read A; do
  B=`echo "$constraints" | grep "^$A" | cut -c 2 | sort | tr '\n' ' '`;
  echo "$A: $B"                   >> Makefile;
  echo "	sleep $i && touch \$@"  >> Makefile;
  echo ""                         >> Makefile;
  i=`expr $i + 1`;
done < starters.txt

seconds=`/usr/bin/time -f'%e' make -j5 2>&1 >/dev/null`;
seconds=`echo $seconds | cut -d'.' -f1`;
echo "Processing took $seconds seconds."
make clean

