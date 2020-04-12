# Run this script to fetch all headers from parent directory and source code files from  ../Commons, /Src for Arduino compilation

# All headers
rm ${PWD}/*.h

# Common source code files
rm ${PWD}/*.c
rm ${PWD}/*.cpp

rm ${PWD}/cleanup.sh 