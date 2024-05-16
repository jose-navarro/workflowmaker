#!/bin/bash

# Access the passed QMAKE_TOOL variable (it's the first and unique argument)
# It should have been defined by build_all.sh

QMAKE_TOOL="$1"

# Check if QMAKE_TOOL is set (not empty)
if [[ -z "$QMAKE_TOOL" ]]; then
  echo "Error: QMAKE_TOOL variable is not set!"
  exit 1
fi

# OK, let's go!

echo "---------- Building project: ToolkitEditor ----------"
cd ToolkitEditor
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release ToolkitEditor.pro
make
rm Makefile
rm toolkiteditor_plugin_import.cpp
rm -rf Release

cd ..

echo "---------- Building project: WorkflowEditor ----------"
cd WorkflowEditor
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release WorkflowEditor.pro
make
rm Makefile
rm workfloweditor_plugin_import.cpp
rm -rf Release

cd ..

echo "---------- Building project: WorkflowLauncher ----------"
cd WorkflowLauncher
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release WorkflowLauncher.pro
make
rm Makefile
rm workflowlauncher_plugin_import.cpp
rm -rf Release

cd ..

