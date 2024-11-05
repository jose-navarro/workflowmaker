#!/bin/bash

# Parameter 1: The project's directory.
# Parameter 2: The path to the build directory.
# Parameter 3: Name of the executable file to copy.

#
# Change the default directory to that of the project, so we can use
# relative paths without problems.
#

cd $1

# Create, if needed, the folders for the .deb package.

mkdir -p ../../installer_linux
mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64
mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/DEBIAN
mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/usr
mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/usr/local
mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/usr/local/bin


# Copy the executable to destination folder

cp $2/$3  ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/usr/local/bin

# Copy the control file to the DEBIAN folder.

cp ../../installer_linux/control ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/DEBIAN

