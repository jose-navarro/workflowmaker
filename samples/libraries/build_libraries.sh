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

echo "---------- Building project: simple_options_file_parser ----------"
cd simple_options_file_parser
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release simple_options_file_parser.pro
make
rm Makefile
rm .qmake.stash
rm libsimple_options_file_parser.a
rm *.o

"$QMAKE_TOOL" -makefile CONFIG+=debug simple_options_file_parser.pro
make
rm Makefile
rm .qmake.stash
rm libsimple_options_file_parserd.a
rm *.o

cd ..

