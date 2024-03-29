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
ulimit -u 120
python2 testsuite.py --compiler ../${COMPILER_PATH} ${@}
