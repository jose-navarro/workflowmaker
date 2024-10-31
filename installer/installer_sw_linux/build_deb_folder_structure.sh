#!/bin/bash

#
# Function to create the deb package folder structure and copy the
# standard files there.
#
# Call it from the different postbuild.sh files.
#

build_deb_folder_structure() {

  # Set the default directory (it is tne unique function's parameter)

  cd $1

  # Create, if needed, the folders for the .deb package.

  mkdir -p ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64
  mkdir -p ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/debian
  mkdir -p ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/DEBIAN
  mkdir -p ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/usr
  mkdir -p ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/usr/local
  mkdir -p ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/usr/local/bin
  mkdir -p ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/usr/share
  mkdir -p ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/usr/share/workflowmaker

  # Copy the control file to the debian folder. It will be used to find dependencies.

  cp ../../installer/installer_sw_linux/control  ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/debian/

  # Copy the files that must be stored in the DEBIAN folder. Set their permissions.

  cp ../../installer/installer_sw_linux/control  ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/DEBIAN/
  cp ../../installer/installer_sw_linux/postinst ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/DEBIAN/
  cp ../../installer/installer_sw_linux/prerm    ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/DEBIAN/

  chmod +x ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/DEBIAN/control
  chmod +x ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/DEBIAN/postinst
  chmod +x ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/DEBIAN/prerm

  # Copy the files that must be stored in the installer folder. Set their permissions

  cp ../../installer/installer_sw_linux/find_dependencies.sh ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/

  chmod +x ../../installer/installer_sw_linux/workflowmaker_x.y-zz_amd64/find_dependencies.sh
}

# Export the function for use in other scripts

export -f build_deb_folder_structure  
