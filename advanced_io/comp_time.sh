#!/bin/bash
if [ $# -ne 2 ]
then
    echo "`basename $0` <count> <step>"
    exit 1
fi
count=$1
step=$2

echo | awk -v COUNT="$count" -v STEP="$step" '
function realtime(prog, count)
{
    cmd = ("/usr/bin/time -p " prog " " count " 2>&1")
    cmd | getline foo
    close(cmd)
    split(foo, ary, " ")
    return ary[2]
} {
    t1 = 0
    t2 = 0
    while (1) {
        t1 = realtime("./rdwrv6", COUNT)
        t2 = realtime("./rdwrv7", COUNT)
        print COUNT" "t1" "t2
        COUNT += STEP
    }
}'

