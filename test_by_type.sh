# Parameters given to this script will be propagated to 'testsuite.py'

LOGIN="xkarav01"
TMP_COMPILER_FOLDER="tmp_testing_folder"

# Create ZIP, run 'is_it_ok.sh', unzip
./zip_and_test.sh
./unzip.sh ${LOGIN} ${TMP_COMPILER_FOLDER}

# Inner folder named after 'LOGIN'
COMPILER_BUILD_FOLDER="./${TMP_COMPILER_FOLDER}/${LOGIN}"

# Build
cd ./${COMPILER_BUILD_FOLDER}
make
cd ./../../

COMPILER_PATH=${COMPILER_BUILD_FOLDER}/ifj20-compiler

# Run testsuite
cd ./ifj-testsuite

echo
echo "LEXICAL"
echo "--------------"
python2 testsuite.py --compiler ../${COMPILER_PATH} -s tests/lex --log-file lex_log.txt | grep "PASSED:\|FAILED:"
echo
echo "SEMANTIC"
echo "--------------"
python2 testsuite.py --compiler ../${COMPILER_PATH} -s tests/sem --log-file sem_log.txt | grep "PASSED:\|FAILED:"
echo
echo "SYNTAX"
echo "--------------"
python2 testsuite.py --compiler ../${COMPILER_PATH} -s tests/syn --log-file syn_log.txt | grep "PASSED:\|FAILED:"
echo
echo "EXECUTION"
echo "--------------"
python2 testsuite.py --compiler ../${COMPILER_PATH} -s tests/exe --log-file exe_log.txt | grep "PASSED:\|FAILED:"
