#!/bin/bash

## Fasten ur seatbelts, bois
set -eu -o pipefail

echo "Please run part-1.sh before (we need the results)."
if test ! -f "canvas.pgm"; then
  exit `false`;
fi

W=1000;
H=1000;

## Unclaimed pixels on full-claim canvas
zero_pixels=`convert canvas.pgm -format %c histogram:info:- | head -n 1 | cut -d':' -f1`;

## Draw claims
while read line; do
  echo "$line";
  w=`echo "$line" | cut -d' ' -f4 | cut -d'x' -f1`;
  h=`echo "$line" | cut -d' ' -f4 | cut -d'x' -f2`;
  x=`echo "$line" | cut -d' ' -f3 | cut -d',' -f1`;
  y=`echo "$line" | cut -d' ' -f3 | cut -d',' -f2 | cut -d':' -f1`;
  claim_size=`expr $w \* $h`;
  ## Draw claim
  convert -size "$W"x"$H" xc:black -fill '#111' -draw "rectangle $x,$y `expr $x + $w - 1`,`expr $y + $h - 1`" tmp.pgm;
  ## Subtract this single claim from the full-claim canvas
  convert canvas.pgm tmp.pgm -compose subtract -composite canvas2.pgm;
  ## If the number of new unclaimed pixels is equal to the size of this claim, then this claim does not overlap with any other claim
  new_zero_pixels=`convert canvas2.pgm -format %c histogram:info:- | head -n 1 | cut -d':' -f1`;
  if test $new_zero_pixels -eq `expr $zero_pixels + $claim_size`; then
    echo "That's the one!";
    exit `:`;
  fi
done < input.txt;


