LOGIN="xkarav01"
TEST_FOLDER="tmp_test_build" # set to "../tmp_test_build" to be super safe
INPUT_FILE="../input_files/example1_no_underline.txt"

rm -r ${TEST_FOLDER} > /dev/null 2>&1
mkdir ${TEST_FOLDER}
unzip -d ./${TEST_FOLDER}/${LOGIN} ${LOGIN}.zip
cd ./${TEST_FOLDER}/${LOGIN}
make
cat ${INPUT_FILE} | make run
cd ./../../
rm -r ${TEST_FOLDER} > /dev/null 2>&1
