#!/bin/bash

# Parameter 1: The project's directory.
# Parameter 2: The path to the build directory.
# Parameter 3: Name of the executable file to copy.

#
# Change the default directory to that of the project, so we can use
# relative paths without problems.
#

cd $1

# Create the deb package structure, and copy there the files that
# must be stored in the DEBIAN FOLDER. Pass the default directory
# as parameter, so the called script knows where to set its default
# folder.

source ../../installer_linux/build_deb_folder_structure.sh
build_deb_folder_structure "$1"

# Create the docs & samples folders and copy the documentation
# and example files to these directories.

source ../../installer_linux/build_doc_samples_folder_structure.sh
build_doc_samples_folder_structure "$1"

# Copy the executable to destination folder

cp $2/$3  ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/usr/local/bin

