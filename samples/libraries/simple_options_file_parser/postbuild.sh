#!/bin/bash

#
# Create the shapelib library.
#
# Parameter 1: Current configuration (Debug | Release)
# Parameter 2: Project directory.
#

#
# Set the current directory to our project's one. Then, we'll
# be able to reference some folders using relative paths.
#

cd $2

#
# Create the output directories. These may exist, but we do it just in case...
#

mkdir -p ./binaries
mkdir -p ./headers

# Copy all *.hpp files.

cp ./src/*.hpp   ./headers/

# Copy library

cp ./$1/*.a ./binaries/


