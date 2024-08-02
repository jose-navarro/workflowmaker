#!/bin/bash

# Define the location of qmake (replace with your actual path)

QMAKE_TOOL="/lib/qt6/bin/qmake"

# Build the libraries.

cd libraries
./build_libraries.sh "$QMAKE_TOOL"
cd ..

# Build the tools
cd tools
./build_tools.sh "$QMAKE_TOOL"
cd ..

# That's all

echo "All projects built"
