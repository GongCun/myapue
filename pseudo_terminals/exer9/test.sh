#!/bin/ksh
stty -echo
pty -e -i <data tty.sh
stty echo
