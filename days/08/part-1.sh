#!/bin/bash

## Fasten ur seatbelts, bois (and girls)
set -eu;


cat input.txt | tr ' ' '\n' > input_with_newlines.txt

mkdir tree;
cd tree;
depth=2;
state="header-children";
while read n; do
  case $state in
    "header-children")
      echo $n > children-left.txt;
      state="header-metas";
      ;;
    "header-metas")
      echo $n > metas-left.txt;
      c=`cat children-left.txt`;
      m=`cat metas-left.txt`;
      if test $c -gt 0; then
        echo `expr $c - 1` > children-left.txt;
        mkdir "c$c";
        cd "c$c";
        depth=`expr $depth + 1`;
        state="header-children";
      elif test $m -gt 0; then
        state="body-metas";
      else
        cd ..;
        depth=`expr $depth - 1`;
        up=1;
        while test $up -eq 1; do
          up=0;
          c=`cat children-left.txt`;
          m=`cat metas-left.txt`;
          if test $c -gt 0; then
            echo `expr $c - 1` > children-left.txt;
            mkdir "c$c";
            cd "c$c";
            depth=`expr $depth + 1`;
            state="header-children";
          elif test $m -gt 0; then
            state="body-metas";
          else
            cd ..;
            depth=`expr $depth - 1`;
            state="header-children";
            up=1;
          fi
        done
      fi
      ;;
    "body-metas")
      m=`cat metas-left.txt`;
      if test $m -gt 0; then
        echo `expr $m - 1` > metas-left.txt;
        echo $n > "m$m";
        echo "meta $n";
      fi
      m=`cat metas-left.txt`;
      if test $m -eq 0; then
        cd ..;
        depth=`expr $depth - 1`;
        if test $depth -le 1; then
          break;
        fi
        up=1;
        while test $up -eq 1; do
          up=0;
          c=`cat children-left.txt`;
          m=`cat metas-left.txt`;
          if test $c -gt 0; then
            echo `expr $c - 1` > children-left.txt;
            mkdir "c$c";
            cd "c$c";
            depth=`expr $depth + 1`;
            state="header-children";
          elif test $m -gt 0; then
            state="body-metas";
          else
            cd ..;
            depth=`expr $depth - 1`;
            state="header-children";
            up=1;
          fi
        done
      fi
      ;;
  esac
done < ../input_with_newlines.txt;

sum=`find tree -type f ! -name "*.txt" -exec cat {} \; \
       | awk '{sum+=$1}END{print sum}'`;
echo "Answer: $sum";
if test -d tree; then
  rm tree -r;
fi

