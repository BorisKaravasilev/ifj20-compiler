# RUN LIKE THIS:
# ./run_from_zip.sh ./input_files/exampleX.txt

LOGIN="xkarav01"
TEST_FOLDER="tmp_test_build" # set to "../tmp_test_build" to be super safe
INPUT_FILE="../../${1:-"input_files/example1.txt"}" # cmd argument 1 or default

rm -r ${TEST_FOLDER} > /dev/null 2>&1
mkdir ${TEST_FOLDER}
unzip -d ./${TEST_FOLDER}/${LOGIN} ${LOGIN}.zip
cd ./${TEST_FOLDER}/${LOGIN}
make
./ifj20-compiler < ${INPUT_FILE}
cd ./../../
rm -r ${TEST_FOLDER} > /dev/null 2>&1
