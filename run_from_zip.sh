LOGIN="xkarav01"
INPUT_FILE="../input_files/example1.txt"

unzip -d ./${LOGIN} ${LOGIN}.zip
cd ./${LOGIN}
make
cat ${INPUT_FILE} | make run
cd ..
rm -r xkarav01
