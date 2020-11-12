#!/bin/bash
SRC_FOLDER="./zdrojaky"
echo

# flattening aktualniho adresare
function flattening () {
  local FILE=""
  local NFILE=""
  local FILES=`find ${F_FOLDER} -name '*' -type f`
  local err=""
  for FILE in $FILES; do
    NFILE=${SRC_FOLDER}/${FILE##*/}  # get the filename behind the last / in the path (or use basename) and add prefix "./"          
    if [ "$FILE" != "$NFILE" ]; then
      cp "$FILE" ${NFILE} 2>/dev/null
      echo "Copied: ($FILE -> $NFILE)"
    fi
  done
}

function delete_dirs () {
    rm -r testdir > /dev/null 2>&1 # Delete testdir
    rm -r $SRC_FOLDER > /dev/null 2>&1 # Remove src folder
}

delete_dirs
mkdir $SRC_FOLDER

F_FOLDER="./general"
flattening
F_FOLDER="./parser"
flattening
F_FOLDER="./scanner"
flattening

cp ifj20-compiler.c ${SRC_FOLDER}/ifj20-compiler.c

# Zips all files, runs is_it_ok.sh

ZIP="xkarav01.zip"
TO_ZIP="${SRC_FOLDER}/* Makefile rozdeleni dokumentace.pdf"

rm -r $ZIP  > /dev/null 2>&1 # Delete old zip

echo
echo "Created '${ZIP}' from:"
echo $TO_ZIP
echo

zip -j $ZIP $TO_ZIP > /dev/null 2>&1

echo "Running is_it_ok..."
./is_it_ok.sh $ZIP testdir

echo
echo "Deleting testdir, ${SRC_FOLDER}"

delete_dirs