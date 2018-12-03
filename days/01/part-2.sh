#!/bin/bash

##
# Nikolaus Mayer, 2018 (github.com/nikolausmayer)
##

#######################
## Safety mechanisms ##
#######################
## Fail if any command fails (use "|| true" if a command is ok to fail)
set -e;
## Treat unset variables as error
set -u;
## Fail if any pipeline command fails
set -o pipefail;


FREQUENCY=0;
SEEN_FREQUENCIES=" 0 ";

## Split input lines like zo:
# +4   -->  + 4
# -12  -->  - 12
# (because `expr` wants "1 + 4" instead of "1+4")

SPLIT_LINES=`cat input.txt | while read line; do 
  echo $line | sed 's/\(.\)\(.*\)/ \1 \2/';     
done`

while `true`; do
  while read line; do
    ## "|| true" because the return code of "expr" is FAILURE if the result is 
    #  not positive, and that would be caught by "set -e"!
    FREQUENCY=`expr $FREQUENCY $line` || `true`;
    #echo "current: $FREQUENCY";
    #echo "seen: #${SEEN_FREQUENCIES}#";
    if test 1 -eq `echo "$SEEN_FREQUENCIES" | grep -c " $FREQUENCY " -`; then
      printf "Double frequency: %s\n" $FREQUENCY;
      exit `:`;
    fi
    SEEN_FREQUENCIES="$SEEN_FREQUENCIES $FREQUENCY ";
  done <<< "$SPLIT_LINES";
done

