REM Check that QMAKE_TOOL has been defined by build_all.bat

if not defined QMAKE_TOOL (
  echo Error: QMAKE_TOOL variable is not set! Please set it in build_all.bat.
  exit /b 1
)

REM Go on!

SET CURRENT_DIR=%~dp0%

echo "---------- Building project: WorkflowMakerLib ----------"

cd WorkflowMakerLib
SET WORKING_DIR=%CURRENT_DIR%WorkflowMakerLib

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release WorkflowMakerLib.pro
nmake
call postbuild.bat "%WORKING_DIR%" x64 Release WorkflowMakerLib_x64_mtd

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"
rmdir /S /Q "x64"

REM Debug.

"%QMAKE_TOOL%" -makefile CONFIG+=debug WorkflowMakerLib.pro
nmake
call postbuild.bat "%WORKING_DIR%" x64 Debug WorkflowMakerLib_x64_dmtd

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"
rmdir /S /Q "x64"

cd ..

