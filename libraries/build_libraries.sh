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

echo "---------- Building project: WorkflowMakerLib ----------"
cd WorkflowMakerLib
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release WorkflowMakerLib.pro
make
rm Makefile
rm *.o
rm .qmake.stash
rm libWorkflowMakerLib.a

"$QMAKE_TOOL" -makefile CONFIG+=debug WorkflowMakerLib.pro
make
rm Makefile
rm *.o
rm .qmake.stash
rm libWorkflowMakerLibd.a

cd ..
