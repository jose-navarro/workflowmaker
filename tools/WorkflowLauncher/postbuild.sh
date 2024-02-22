#!/bin/bash

# Parameter 1: The project's directory.
# Parameter 2: The name of the current project configuration, for example, "Debug".
# Parameter 3: Name of the executable file to copy.

#
# Change the default directory to that of the project, so we can use
# relative paths without problems.
#

cd $1

# Create, if needed, the folders for the .deb package.

mkdir -p ../../installer/workflowmaker_x.y-zz_amd64
mkdir -p ../../installer/workflowmaker_x.y-zz_amd64/DEBIAN
mkdir -p ../../installer/workflowmaker_x.y-zz_amd64/usr
mkdir -p ../../installer/workflowmaker_x.y-zz_amd64/usr/local
mkdir -p ../../installer/workflowmaker_x.y-zz_amd64/usr/local/bin


# Copy the executable to destination folder

cp ./$2/$3  ../../installer/workflowmaker_x.y-zz_amd64/usr/local/bin/

# Copy the toolkit schema to the destination folder.

cp ../../data/launcher.xsd ../../installer/workflowmaker_x.y-zz_amd64/usr/local/bin

