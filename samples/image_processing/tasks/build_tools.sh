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

echo "---------- Building project: blur ----------"
cd blur
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release blur.pro
make
rm Makefile
rm .qmake.stash
rm blur
rm *.o

cd ..

echo "---------- Building project: charcoal ----------"
cd charcoal
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release charcoal.pro
make
rm Makefile
rm .qmake.stash
rm charcoal
rm *.o

cd ..

echo "---------- Building project: grayscale ----------"
cd grayscale
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release grayscale.pro
make
rm Makefile
rm .qmake.stash
rm grayscale
rm *.o

cd ..

echo "---------- Building project: jpg2png----------"
cd jpg2png
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release jpg2png.pro
make
rm Makefile
rm .qmake.stash
rm jpg2png
rm *.o

cd ..

echo "---------- Building project: mirror ----------"
cd mirror
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release mirror.pro
make
rm Makefile
rm .qmake.stash
rm mirror
rm *.o

cd ..

echo "---------- Building project: mosaic ----------"
cd mosaic
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release mosaic.pro
make
rm Makefile
rm .qmake.stash
rm mosaic
rm *.o

cd ..

echo "---------- Building project: oilpaint ----------"
cd oilpaint
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release oilpaint.pro
make
rm Makefile
rm .qmake.stash
rm oilpaint
rm *.o

cd ..

echo "---------- Building project: png2jpg ----------"
cd png2jpg
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release png2jpg.pro
make
rm Makefile
rm .qmake.stash
rm png2jpg
rm *.o

cd ..

echo "---------- Building project: rotate ----------"
cd rotate
chmod +x postbuild.sh
"$QMAKE_TOOL" -makefile CONFIG+=release rotate.pro
make
rm Makefile
rm .qmake.stash
rm rotate
rm *.o

cd ..

