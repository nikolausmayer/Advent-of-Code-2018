#!/bin/bash

## Fasten ur seatbelts, bois (and girls)
set -eu 

convert -size 512x512 xc:black canvas.pgm;
i=0;
while read line; do
  XC=`echo "$line" | cut -d',' -f1`;
  YC=`echo "$line" | cut -d',' -f2`;
  echo "`expr $i + 1`/50";
  convert -size 512x512 xc:black -fx "XX=abs(i-${XC})/65535; YY=abs(j-${YC})/65535; XX+YY" ${i}.pgm;
  convert canvas.pgm ${i}.pgm -compose Plus -composite tmp.pgm;
  mv tmp.pgm canvas.pgm;
  i=`expr $i + 1`;
done < input.txt;

pixels_under_10k=0;
convert canvas.pgm -format %c histogram:info:- \
| tail -n+1                                    \
| while read line; do
    number_of_pixels=`echo $line | cut -d':' -f1`;
    acc_distance=`echo $line | cut -d'(' -f2 | cut -d',' -f1`;
    if test $acc_distance -ge 10000; then
      echo "pixels with distance under 10k: $pixels_under_10k";
      break;
    fi
    pixels_under_10k=`expr $pixels_under_10k + $number_of_pixels`;
  done

