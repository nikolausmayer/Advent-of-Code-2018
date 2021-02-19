#!/bin/bash

## Fasten ur seatbelts, bois (and girls)
set -eu;


cat input.txt | tr ' ' '\n' > input_with_newlines.txt

if test ! -d tree; then
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
          echo $n > "mm$m";
          #echo "meta $n";
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
fi


echo ".SILENT:"            >  Makefile;
echo "default: tree/value" >> Makefile;
echo "clean:"              >> Makefile;
echo "	-rm tree -r;"         >> Makefile;

printf "tree/value:" >> Makefile;

find tree -maxdepth 1 -mindepth 1 -type d -name "c*" | while read child; do
  printf " $child/value" >> Makefile;
done
echo "" >> Makefile;
list_of_children=`find tree -maxdepth 1 -mindepth 1 -type d -name "c*"`;
number_of_children=`find tree -maxdepth 1 -mindepth 1 -type d -name "c*" | wc -l`;
#printf "	sum=0" >> Makefile;
printf "	expr 0" >> Makefile;
find tree -maxdepth 1 -mindepth 1 -type f -name "mm*" | while read child; do
  child_id=`cat $child`;
  if test $child_id -le $number_of_children; then
    child_id=`expr $number_of_children - $child_id + 1`;
    #printf " && tmp=\`cat tree/c${child_id}/value\` && sum=\`expr \$\$sum + \$\$tmp\`" >> Makefile;
    printf " + \`cat tree/c${child_id}/value\`" >> Makefile;
  fi
done
#echo " && printf \$\$sum > \$@" >> Makefile;
echo " > \$@ && cat \$@" >> Makefile;
echo "" >> Makefile;


find tree -type d -name "c*" | while read node; do
  if test `find $node -maxdepth 1 -mindepth 1 -type d -name "c*" | wc -l` -eq 0; then
    echo "${node}/value:" >> Makefile;
    echo "	cat ${node}/mm* | awk '{sum+=\$\$1}END{print sum}' > \$@" >> Makefile
  else
    rule="${node}/value:"`find $node -maxdepth 1 -mindepth 1 -type d -name "c*" \
                            | while read child; do                              \
                            printf " $child/value";                             \
                          done`;
    echo $rule >> Makefile;
    list_of_children=`find $node -maxdepth 1 -mindepth 1 -type d -name "c*"`;
    number_of_children=`find $node -maxdepth 1 -mindepth 1 -type d -name "c*" | wc -l`;
    printf "	sum=0" >> Makefile;
    find $node -maxdepth 1 -mindepth 1 -type f -name "mm*" | while read child; do
      child_id=`cat $child`;
      if test $child_id -le $number_of_children; then
        child_id=`expr $number_of_children - $child_id + 1`;
        printf " && tmp=\`cat ${node}/c${child_id}/value\` && sum=\`expr \$\$sum + \$\$tmp\`" >> Makefile;
      fi
    done
    echo " && printf \$\$sum > \$@" >> Makefile;
  fi
  echo "" >> Makefile;
done

make;
make clean;

