#!/bin/bash

#
# Function to create the deb package folder structure and copy the
# standard files there.
#
# Call it from the different postbuild.sh files.
#

build_deb_folder_structure() {

  # Set the default directory (it is the unique function's parameter)

  cd $1

  # Create, if needed, the folders for the .deb package.

  mkdir -p ../../installer_linux
  mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64
  mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/DEBIAN
  mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/usr
  mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/usr/local
  mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/usr/local/bin

  # Copy the files that must be stored in the DEBIAN folder. Set their permissions.

  cp ../../installer_linux/control  ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/DEBIAN
  chmod +x                          ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_amd64/DEBIAN/control
}

# Export the function for use in other scripts

export -f build_deb_folder_structure  
