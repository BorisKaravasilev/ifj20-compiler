LOGIN=${1:-"xkarav01"} # cmd argument 1 or default
TEST_FOLDER=${2:-"unzipped_"$LOGIN} # cmd argument 2 or default

rm -r ${TEST_FOLDER} > /dev/null 2>&1
mkdir ${TEST_FOLDER}
unzip -d ./${TEST_FOLDER}/${LOGIN} ${LOGIN}.zip
