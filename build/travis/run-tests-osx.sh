#!/usr/bin/env sh

mkdir bin
cd bin
cmake -DSEARCH_GDAL_PATH=${HOME}/.gdal -DSEARCH_GDAL_PATH=${HOME}/.gdal -DSEARCH_GEOS_PATH=/usr/local -DMYSQL_MAIN_DIR=${HOME}/.mysql -DSEARCH_WXPDFDOCUMENT_PATH=${HOME}/.wxpdfdoc -DSEARCH_GEOS=1 -DSEARCH_GDAL=1 -DUSE_UNITTEST=1 -DCXXTEST_INCLUDE_DIR=${HOME}/cxxtest-4.3 -DCXXTEST_PYTHON_TESTGEN_EXECUTABLE=${HOME}/cxxtest-4.3/bin/cxxtestgen ..
make -j4
