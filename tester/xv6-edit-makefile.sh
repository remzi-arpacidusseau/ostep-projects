#! /bin/bash

infile=$1
testnames=$2

gawk -vtestnames=$testnames '
($1 == "_mkdir\\") {
  n = split(testnames, x, ",");
  for (i = 1; i <= n; i++) {
    printf("\t_%s\\\n", x[i]);
  }
} 

{
  print $0;
}' $infile 
