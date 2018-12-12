#!/bin/bash

## Fasten ur seatbelts, bois (and girls)
set -eu -o pipefail

polymer=`cat input.txt`;
polymer2="DUMMY";

## Do one elimination step
react () {
  polymer=`echo $polymer2 | sed 's/aA\|Aa\|bB\|Bb\|cC\|Cc\|dD\|Dd\|eE\|Ee\|fF\|Ff\|gG\|Gg\|hH\|Hh\|iI\|Ii\|jJ\|Jj\|kK\|Kk\|lL\|Ll\|mM\|Mm\|nN\|Nn\|oO\|Oo\|pP\|Pp\|qQ\|Qq\|rR\|Rr\|sS\|Ss\|tT\|Tt\|uU\|Uu\|vV\|Vv\|wW\|Ww\|xX\|Xx\|yY\|Yy\|zZ\|Zz//g'`;
}

## Iterate elimination steps until the string stops changing
i=0;
while test $polymer2 != $polymer; do
  polymer2=$polymer;
  react;

  i=`expr $i + 1`;
done

echo "finished after $i reduction steps";
echo "answer: " `echo $polymer | wc -c` " (-1 because newline)";

