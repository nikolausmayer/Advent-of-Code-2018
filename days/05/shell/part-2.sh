#!/bin/bash

## Fasten ur seatbelts, bois (and girls)
set -eu -o pipefail

original_polymer=`cat input.txt`;
polymer="";
polymer2="";

min_length=100000;
min_letter="a";

## Remove one letter from the original string
remove_unit_type () {
  polymer=`echo $original_polymer | tr -d "$1"`;
  polymer2="DUMMY";
}

## Do one elimination step
react () {
  polymer=`echo $polymer2 | sed 's/aA\|Aa\|bB\|Bb\|cC\|Cc\|dD\|Dd\|eE\|Ee\|fF\|Ff\|gG\|Gg\|hH\|Hh\|iI\|Ii\|jJ\|Jj\|kK\|Kk\|lL\|Ll\|mM\|Mm\|nN\|Nn\|oO\|Oo\|pP\|Pp\|qQ\|Qq\|rR\|Rr\|sS\|Ss\|tT\|Tt\|uU\|Uu\|vV\|Vv\|wW\|Ww\|xX\|Xx\|yY\|Yy\|zZ\|Zz//g'`;
}

## Try a letter
try_letter () {
  letter="$1";
  remove_unit_type "$letter";
  ## Iterate elimination steps until the string stops changing
  while test $polymer2 != $polymer; do
    polymer2=$polymer;
    react;
  done
  length=`echo $polymer | wc -c`;
  if test $length -lt $min_length; then
    min_length=$length;
    min_letter=$letter;
  fi
  echo $letter $length;
}

## This is dumb, but it works, so it's not dumb
try_letter "aA";
try_letter "bB";
try_letter "cC";
try_letter "dD";
try_letter "eE";
try_letter "fF";
try_letter "gG";
try_letter "hH";
try_letter "iI";
try_letter "jJ";
try_letter "kK";
try_letter "lL";
try_letter "mM";
try_letter "nN";
try_letter "oO";
try_letter "pP";
try_letter "qQ";
try_letter "rR";
try_letter "sS";
try_letter "tT";
try_letter "uU";
try_letter "vV";
try_letter "wW";
try_letter "xX";
try_letter "yY";
try_letter "zZ";

echo "";
echo $min_letter `expr $min_length - 1`;

