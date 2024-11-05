REM Check that QMAKE_TOOL has been defined by build_all.bat

if not defined QMAKE_TOOL (
  echo Error: QMAKE_TOOL variable is not set! Please set it in build_all.bat.
  exit /b 1
)

REM Go on!

SET CURRENT_DIR=%~dp0%

echo "---------- Building project: simple_options_file_parser ----------"

cd simple_options_file_parser
SET WORKING_DIR=%CURRENT_DIR%simple_options_file_parser

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release simple_options_file_parser.pro
nmake
call postbuild.bat "%WORKING_DIR%" "%WORKING_DIR%\release"

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

REM Debug

"%QMAKE_TOOL%" -makefile CONFIG+=debug simple_options_file_parser.pro
nmake
call postbuild.bat "%WORKING_DIR%" "%WORKING_DIR%\debug"

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..

echo "----------Warning about the ImageMagick library ----------"

echo WARNING - THE ImageMagick library must be built manually.
echo           Proceed as described in:
echo               https://github.com/ImageMagick/ImageMagick-Windows
echo           and then organize the headers and libraries as stated
echo           in the documentation.