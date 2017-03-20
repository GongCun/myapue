#!/bin/ksh

pwd=`dirname $0`
cd $pwd

infile=$1
trap '' 2 3 15

indent $infile $infile.out
mv $infile.out $infile
