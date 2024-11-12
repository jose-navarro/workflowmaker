#!/bin/bash

#
# Function to create the documentation and samples folder structure and copy the
# standard files there.
#
# Call it from the different postbuild.sh files.
#

build_doc_samples_folder_structure() {

  # Set the default directory (it is tne unique function's parameter)

  cd $1

  # Create, if needed, the folders for the .deb package.

  mkdir -p ../../installer_linux
  mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_doc_and_samples
  mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_doc_and_samples/docs
  mkdir -p ../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_doc_and_samples/samples

  # Copy the complete set of user guides.

  for file in "../../docs"/*.pdf; do
    cp "$file" "../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_doc_and_samples/docs"
  done

  # Copy the samples (image processing toolkit)

  for file in "../../sample_workflowmaker_files"/*.xml; do
    cp "$file" "../../installer_linux/workflowmaker-image-processing-samples_x.y-zz_doc_and_samples/samples"
  done
}

# Export the function for use in other scripts

export -f build_doc_samples_folder_structure 
