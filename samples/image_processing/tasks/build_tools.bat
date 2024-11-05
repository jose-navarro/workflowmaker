REM Check that QMAKE_TOOL has been defined by build_all.bat

if not defined QMAKE_TOOL (
  echo Error: QMAKE_TOOL variable is not set! Please set it in build_all.bat.
  exit /b 1
)

REM Go on!

SET CURRENT_DIR=%~dp0%

echo "---------- Building project: blur ----------"

cd blur
SET WORKING_DIR=%CURRENT_DIR%blur

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release blur.pro
nmake
call postbuild.bat "%WORKING_DIR%" "%WORKING_DIR%\release" blur

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..

echo "---------- Building project: charcoal ----------"

cd charcoal
SET WORKING_DIR=%CURRENT_DIR%charcoal

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release charcoal.pro
nmake
call postbuild.bat "%WORKING_DIR%" "%WORKING_DIR%\release" charcoal

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..

echo "---------- Building project: grayscale ----------"

cd grayscale
SET WORKING_DIR=%CURRENT_DIR%grayscale

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release grayscale.pro
nmake
call postbuild.bat "%WORKING_DIR%" "%WORKING_DIR%\release" grayscale

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..

echo "---------- Building project: jpg2png ----------"

cd jpg2png
SET WORKING_DIR=%CURRENT_DIR%jpg2png

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release jpg2png.pro
nmake
call postbuild.bat "%WORKING_DIR%" "%WORKING_DIR%\release" jpg2png

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..

echo "---------- Building project: mirror ----------"

cd mirror
SET WORKING_DIR=%CURRENT_DIR%mirror

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release mirror.pro
nmake
call postbuild.bat "%WORKING_DIR%" "%WORKING_DIR%\release" mirror

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..

echo "---------- Building project: mosaic ----------"

cd mosaic
SET WORKING_DIR=%CURRENT_DIR%mosaic

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release mosaic.pro
nmake
call postbuild.bat "%WORKING_DIR%" "%WORKING_DIR%\release" mosaic

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..

echo "---------- Building project: oilpaint ----------"

cd oilpaint
SET WORKING_DIR=%CURRENT_DIR%oilpaint

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release oilpaint.pro
nmake
call postbuild.bat "%WORKING_DIR%" "%WORKING_DIR%\release" oilpaint

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..

echo "---------- Building project: png2jpg ----------"

cd png2jpg
SET WORKING_DIR=%CURRENT_DIR%png2jpg

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release png2jpg.pro
nmake
call postbuild.bat "%WORKING_DIR%" "%WORKING_DIR%\release" png2jpg

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..

echo "---------- Building project: rotate ----------"

cd rotate
SET WORKING_DIR=%CURRENT_DIR%rotate

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release rotate.pro
nmake
call postbuild.bat "%WORKING_DIR%" "%WORKING_DIR%\release" rotate

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..