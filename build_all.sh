#!/bin/bash

# Define the location of qmake (replace with your actual path)

#QMAKE_TOOL="/lib/qt6/bin/qmake"
QMAKE_TOOL="qmake6"

# Build the libraries.

cd libraries
chmod +x build_libraries.sh
./build_libraries.sh "$QMAKE_TOOL"
cd ..

# Build the tools
cd tools
chmod +x build_tools.sh
./build_tools.sh "$QMAKE_TOOL"
cd ..

# That's all

echo "All projects built"
