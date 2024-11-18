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

  mkdir -p ../../installer/installer_sw_linux/workflowmaker_x.y-zz_doc_and_samples
  mkdir -p ../../installer/installer_sw_linux/workflowmaker_x.y-zz_doc_and_samples/docs
  mkdir -p ../../installer/installer_sw_linux/workflowmaker_x.y-zz_doc_and_samples/samples

  # Copy the complete set of user guides (WorkflowMaker only)

  for file in "../../docs/user guide"/*.pdf; do
    cp "$file" "../../installer/installer_sw_linux/workflowmaker_x.y-zz_doc_and_samples/docs"
  done

  #
  # Copy the complete set of documents related to the samples and the
  # simple_options_file_parser_library.
  #

  for file in "../../samples/image_processing/docs"/*.pdf; do
    cp "$file" "../../installer/installer_sw_linux/workflowmaker_x.y-zz_doc_and_samples/docs"
  done

  for file in "../../samples/libraries/simple_options_file_parser/docs"/*.pdf; do
    cp "$file" "../../installer/installer_sw_linux/workflowmaker_x.y-zz_doc_and_samples/docs"
  done

  # Copy the samples (image processing toolkit)

  for file in "../../samples/image_processing/sample_workflowmaker_files"/*.xml; do
    cp "$file" "../../installer/installer_sw_linux/workflowmaker_x.y-zz_doc_and_samples/samples"
  done
}

# Export the function for use in other scripts

export -f build_doc_samples_folder_structure 
