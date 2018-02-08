#!/bin/bash

# Test cases:

make

echo "120x4"

./main 1 120 4 120x4_testCase.pbm 0
./main 2 120 4 120x4_testCase.pgm 0
./main 3 120 4 120x4_testCase.ppm 0


echo "4x120 and 6x120"

./main 1 4 120 4x120_testCase.pbm 0	# ** Doesn't work as expected
./main 2 4 120 4x120_testCase.pgm 0	# ** Doesn't work as expected
./main 3 6 120 6x120_testCase.ppm 0


echo "120x120 (Binary)"

./main 1 120 120 binary_120x120_testCase.pbm 1
./main 2 120 120 binary_120x120_testCase.pgm 1
./main 3 120 120 binary_120x120_testCase.ppm 1


echo "120x120 (Square)"

./main 1 120 120 120x120_testCase.pbm 0
./main 2 120 120 120x120_testCase.pgm 0
./main 3 120 120 120x120_testCase.ppm 0


echo "60x120 (Vertical)"

./main 1 60 120 60x120_testCase.pbm 0
./main 2 60 120 60x120_testCase.pgm 0
./main 3 60 120 60x120_testCase.ppm 0


echo "120x60 (Horizontal)"

./main 1 120 60 120x60_testCase.pbm 0
./main 2 120 60 120x60_testCase.pgm 0
./main 3 120 60 120x60_testCase.ppm 0


echo "4x4 and 6x4 (Small)"

./main 1 4 4 4x4_testCase.pbm 0
./main 2 4 4 4x4_testCase.pgm 0
./main 3 6 4 6x4_testCase.ppm 0


echo "1200x1200 (Large)"

./main 1 1200 1200 1200x1200_testCase.pbm 0
./main 2 1200 1200 1200x1200_testCase.pgm 0
./main 3 1200 1200 1200x1200_testCase.ppm 0


echo "Improper Format"

./main 1 120 120 
./main 0 120 120 image 0 
./main 4 120 120 image 0 
./main 1 125 120 image 1 
./main 1  -4 120 image 1 
./main 3 124 120 image 1 
./main 3  -4 120 image 1 
./main 1 120  -6 image 1 
./main 1 120 120 image 2 


make clean
