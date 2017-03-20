#!/bin/ksh
outputfile=$1
shift
for f in $*; do
    files="$files $f"
done
/opt/local/bin/ld -dynamic -arch x86_64 -macosx_version_min 10.8.5 -weak_reference_mismatches non-weak -o $outputfile -L/opt/local/lib/gcc47/gcc/x86_64-apple-darwin12/4.7.4 -L/opt/local/lib/gcc47/gcc/x86_64-apple-darwin12/4.7.4/../../.. $files -no_compact_unwind -lSystem -lgcc_ext.10.5 -lgcc -lSystem -v

