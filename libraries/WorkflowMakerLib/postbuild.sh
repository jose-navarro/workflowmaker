#!/bin/bash

#
# Create the WorkflowMakerLib library.
#
# Parameter 1: Build directory.
# Parameter 2: Project directory.
#

#
# Set the current directory to our project's one. Then, we'll
# be able to reference some folders using relative paths.
#

cd $2

# Create the deb package structure, and copy there the files that
# must be stored in the debian and DEBIAN FOLDERS. Pass the
# default directory as parameter, so the called script knows
# where to set the default folder.

source ../../installer/installer_sw_linux/build_deb_folder_structure.sh
build_deb_folder_structure "$2"

#
# Create the output directories. These may exist, but we do it just in case...
#

mkdir -p ./binaries
mkdir -p ./headers

# Copy all *.hpp files.

cp ./src/*.hpp ./headers/

# Copy library

cp $1/*.a ./binaries/

# Copy the version file to the .deb folder.

cp ./data_to_bin_folder/workflowmaker_version.txt ../../installer/workflowmaker_x.y-zz_amd64/usr/local/bin

