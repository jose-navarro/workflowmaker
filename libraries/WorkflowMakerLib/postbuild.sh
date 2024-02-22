#!/bin/bash

#
# Create the WorkflowMakerLib library.
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

# Create, if needed, the folders for the .deb package.

mkdir -p ../../installer/workflowmaker_x.y-zz_amd64
mkdir -p ../../installer/workflowmaker_x.y-zz_amd64/DEBIAN
mkdir -p ../../installer/workflowmaker_x.y-zz_amd64/usr
mkdir -p ../../installer/workflowmaker_x.y-zz_amd64/usr/local
mkdir -p ../../installer/workflowmaker_x.y-zz_amd64/usr/local/bin

# Copy all *.hpp files.

cp ./src/*.hpp ./headers/

# Copy library

cp ./$1/*.a ./binaries/

# Copy the version file to the .deb folder.

cp ./data_to_bin_folder/workflowmaker_version.txt ../../installer/workflowmaker_x.y-zz_amd64/usr/local/bin


