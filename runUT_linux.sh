env:
C_COMPILER=gcc-8
CXX_COMPILER=g++-8

mkdir build
cd build
env CC=${C_COMPILER} CXX=${CXX_COMPILER}
cmake .. -DENABLE_COVERAGE:BOOL=TRUE -DCMAKE_BUILD_TYPE=DEBUG
make
ctest -V
#ctest -V | grep -i "OK" | wc -l
