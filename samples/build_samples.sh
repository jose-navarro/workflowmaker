#!/bin/bash

# Define the location of qmake (replace with your actual path)

QMAKE_TOOL="/usr/bin/qmake6"

# Build the libraries.

cd libraries
chmod +x build_libraries.sh
./build_libraries.sh "$QMAKE_TOOL"
cd ..

# Build the image_processing toolkit.

cd image_processing
cd tasks
chmod +x build_tools.sh
./build_tools.sh "$QMAKE_TOOL"
cd ..
cd ..

# That's all

echo "All projects built"
