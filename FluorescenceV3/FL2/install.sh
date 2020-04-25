# Run this script to fetch all headers from parent directory and source code files from  ../Commons, /Src for Arduino compilation

# All headers
# cp -fr ../*.h ${PWD}

# Common source code & header files
cp -fr ../Commons/*.c ${PWD}
cp -fr ../Commons/*.h ${PWD}
# Platform specific source code files
# cp -fr ${PWD}/_Src/*.c ${PWD}
cp -fr ${PWD}/_Src/*.cpp ${PWD}

# Cleanup file
cp -fr ${PWD}/_Src/cleanup.sh ${PWD}