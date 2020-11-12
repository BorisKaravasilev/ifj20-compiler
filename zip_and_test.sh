#!/bin/bash

# Zips all files, runs is_it_ok.sh

ZIP="xkarav01.zip"
TO_ZIP="general parser scanner ifj20-compiler.c Makefile rozdeleni dokumentace.pdf"

rm -r $ZIP  > /dev/null 2>&1 # Delete old zip

echo
echo "Created '${ZIP}' from:"
echo $TO_ZIP
echo

zip -r $ZIP $TO_ZIP > /dev/null 2>&1

echo "Running is_it_ok..."
./is_it_ok.sh $ZIP testdir

rm -r testdir  > /dev/null 2>&1 # Delete testdir