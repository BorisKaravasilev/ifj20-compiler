LOGIN="xkarav01"
TEST_FOLDER="tmp_test_build"

rm -r ${TEST_FOLDER} > /dev/null 2>&1
mkdir ${TEST_FOLDER}
unzip -d ./${TEST_FOLDER}/${LOGIN} ${LOGIN}.zip
