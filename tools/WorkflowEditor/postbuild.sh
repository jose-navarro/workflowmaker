#!/bin/bash

# Parameter 1: Project directory.
# Parameter 2: Build directory.
# Parameter 3: Name of the executable file to copy.

#
# Set the current directory to our project's one. Then, we'll
# be able to reference some folders using relative paths.
#

cd $1

# Create the deb package structure, and copy there the files that
# must be stored in the debian and DEBIAN FOLDERS. Pass the
# default directory as parameter, so the called script knows
# where to set the default folder.

source ../../installer/installer_sw_linux/build_deb_folder_structure.sh
build_deb_folder_structure "$1"

# Copy the executable to destination folder

cp $2/$3                   ../../installer/workflowmaker_x.y-zz_amd64/usr/local/bin/

# Copy the workflow schema to the destination folder.

cp ../../data/workflow.xsd ../../installer/workflowmaker_x.y-zz_amd64/usr/local/bin

# Copy the icons and desktop files to the destination share folder

cp ./linux_menu/*.png      ../../installer/workflowmaker_x.y-zz_amd64/usr/share/workflowmaker/
cp ./linux_menu/*.desktop  ../../installer/workflowmaker_x.y-zz_amd64/usr/share/workflowmaker/

