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

## Split input lines like zo:
# +4   -->  + 4
# -12  -->  - 12
# (because `expr` wants "1 + 4" instead of "1+4")
BIGSTRING="0 "`cat input.txt | while read line; do
  echo $line | sed 's/\(.\)\(.*\)/ \1 \2/';
done`

printf "Answer: %s\n" `expr ${BIGSTRING}`;

