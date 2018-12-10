#!/bin/bash

## Fasten ur seatbelts, bois
set -eu -o pipefail

W=1000;
H=1000;

## We will draw claims onto a persistent canvas
convert -size "$W"x"$H" xc:black canvas.pgm;

## Draw claims
while read line; do
  echo "$line";
  w=`echo "$line" | cut -d' ' -f4 | cut -d'x' -f1`;
  h=`echo "$line" | cut -d' ' -f4 | cut -d'x' -f2`;
  x=`echo "$line" | cut -d' ' -f3 | cut -d',' -f1`;
  y=`echo "$line" | cut -d' ' -f3 | cut -d',' -f2 | cut -d':' -f1`;
  ## Draw claim
  convert -size "$W"x"$H" xc:black -fill '#111' -draw "rectangle $x,$y `expr $x + $w - 1`,`expr $y + $h - 1`" tmp.pgm;
  ## Add new claim image to canvas
  convert canvas.pgm tmp.pgm -compose Plus -composite canvas.pgm;
done < input.txt;

## Histogram of image to count pixels
## First line is unclaimed pixels (we assume there is at least 1 unclaimed pixel in the image)
unclaimed=`convert canvas.pgm -format %c histogram:info:- | head -n 1 | cut -d':' -f1`;
## Second line is pixels on a single claim (we assume there is at least 1 such pixel)
one_claimed=`convert canvas.pgm -format %c histogram:info:- | head -n 2 | tail -n 1 | cut -d':' -f1`;
two_or_moreclaimed=`expr \( $W \* $H \) - $unclaimed - $one_claimed`;
printf "%s pixels are claimed by >=2\n" "$two_or_moreclaimed";


