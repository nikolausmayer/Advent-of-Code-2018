#!/bin/bash

## Fasten ur seatbelts, bois (and girls)
set -eu 

convert -size 512x512 xc:white canvas.pfm;
i=0;
while read line; do
  XC=`echo "$line" | cut -d',' -f1`;
  YC=`echo "$line" | cut -d',' -f2`;
  #XC=`expr $XC \* 50`;
  #YC=`expr $YC \* 50`;
  echo $i $XC $YC;
  convert -size 512x512 xc:black -fx "XX=abs(i-${XC})/1000; YY=abs(j-${YC})/1000; XX+YY" ${i}.pfm;
  convert canvas.pfm ${i}.pfm -compose Darken -composite tmp.pfm;
  mv tmp.pfm canvas.pfm;
  i=`expr $i + 1`;
done < input.txt;

convert -size 512x512 xc:black canvas2.pfm;

i=0;
while read line; do
  echo $i;
  convert canvas.pfm ${i}.pfm -fx "u == v ? 0.01 : 0" ${i}-2.pfm;
  convert canvas2.pfm ${i}-2.pfm -compose Plus -composite tmp.pfm;
  mv tmp.pfm canvas2.pfm;
  i=`expr $i + 1`;
done < input.txt;

i=0;
while read line; do
  echo $i;
  convert ${i}-2.pfm -compose minus_dst \( canvas2.pfm -evaluate Multiply 2 \) -composite ${i}-3.pfm;
  i=`expr $i + 1`;
done < input.txt;

convert -size 512x512 xc:black canvas3.pfm;
i=0;
while read line; do
  echo $i;
  convert ${i}-3.pfm -fx "intensity <= 0.01 ? ( $i + 1 ) / 100 : 0" ${i}-4.pfm;
  convert canvas3.pfm ${i}-4.pfm -compose Plus -composite tmp.pfm;
  mv tmp.pfm canvas3.pfm;
  i=`expr $i + 1`;
done < input.txt;


## Colors touching the canvas boundaries are infinite
## TODO is this a heuristic?

check_pixels () {
  convert border.pfm -format %c histogram:info:- \
  | tail -n+2                                    \
  | tr -d '%'                                    \
  | grep -o -E '[1-9]?[0-9]?[0-9])$'             \
  | cut -d')' -f1                                \
  | tr '\n' ':';
}
convert canvas3.pfm -crop 512x1+0+0 border.pfm;
infinite_colors=":`check_pixels`";
convert canvas3.pfm -crop 512x1+0+511 border.pfm;
infinite_colors="${infinite_colors}`check_pixels`";
convert canvas3.pfm -crop 1x512+0+0 border.pfm;
infinite_colors="${infinite_colors}`check_pixels`";
convert canvas3.pfm -crop 1x512+511+0 border.pfm;
infinite_colors="${infinite_colors}`check_pixels`";

echo $infinite_colors;
max_pixels=0;
max_index=0;
convert canvas3.pfm -format %c histogram:info:- \
| tail -n+2                                     \
| while read line; do
  pixels=`echo $line | cut -d':' -f1`;
  index=`echo $line | tr -d '%' | grep -o -E '[1-9]?[0-9]?[0-9])$' | cut -d')' -f1`;
  if test `echo $infinite_colors | grep -c ":${index}:"` -gt 0; then
    echo "$index is infinite";
    continue;
  fi
  if test $pixels -gt $max_pixels; then
    max_pixels=$pixels;
    max_index=$index;
  fi
  echo "current maximum: $max_index with $max_pixels pixels";
done




