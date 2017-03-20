#!/bin/ksh

dir=`dirname $0`
cd $dir

exec 3>&-
exec 3>> test.log

date >&3 2>&3
#echo >&3
RC=$?

exec 3>&-
exit $RC

