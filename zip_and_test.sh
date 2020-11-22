#!/bin/bash
TMP_SRC_FOLDER="./zdrojaky"
echo

# flattening aktualniho adresare
function flattening () {
  local FILE=""
  local NFILE=""
  local FILES=`find ${F_FOLDER} -name '*' -type f`
  local err=""
  for FILE in $FILES; do
    NFILE=${TMP_SRC_FOLDER}/${FILE##*/}  # get the filename behind the last / in the path (or use basename) and add prefix "./"          
    if [ "$FILE" != "$NFILE" ]; then
      cp "$FILE" ${NFILE} 2>/dev/null
      echo "Copied: ($FILE -> $NFILE)"
    fi
  done
}

function delete_dirs () {
    rm -r testdir > /dev/null 2>&1 # Delete testdir
    rm -r $TMP_SRC_FOLDER > /dev/null 2>&1 # Remove src folder
}

delete_dirs
mkdir $TMP_SRC_FOLDER

F_FOLDER="./general"
flattening
F_FOLDER="./parser"
flattening
F_FOLDER="./scanner"
flattening
F_FOLDER="./generator"
flattening

cp ifj20-compiler.c ${TMP_SRC_FOLDER}/ifj20-compiler.c

# Zips all files, runs is_it_ok.sh

ZIP="xkarav01.zip"
TO_ZIP="${TMP_SRC_FOLDER}/* Makefile rozdeleni dokumentace.pdf"

rm -r $ZIP  > /dev/null 2>&1 # Delete old zip

echo
echo "Created '${ZIP}' from:"
echo $TO_ZIP
echo

zip -j $ZIP $TO_ZIP > /dev/null 2>&1

echo "Running is_it_ok..."
./is_it_ok.sh $ZIP testdir

echo
echo "Deleting testdir, ${TMP_SRC_FOLDER}"

delete_dirs
