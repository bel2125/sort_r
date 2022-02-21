#/bin/bash

[ ! -e test.out ] || rm test.out
gcc test.c ../src/sort_r.c -o test.out -O3
./test.out
