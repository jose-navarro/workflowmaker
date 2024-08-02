REM Check that QMAKE_TOOL has been defined by build_all.bat

if not defined QMAKE_TOOL (
  echo Error: QMAKE_TOOL variable is not set! Please set it in build_all.bat.
  exit /b 1
)

REM Go on!

SET CURRENT_DIR=%~dp0%

echo "---------- Building project: ToolkitEditor ----------"

cd ToolkitEditor
SET WORKING_DIR=%CURRENT_DIR%ToolkitEditor

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release ToolkitEditor.pro
nmake
call postbuild.bat "%WORKING_DIR%" release "%WORKING_DIR%"

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..

echo "---------- Building project: WorkflowEditor ----------"

cd WorkflowEditor
SET WORKING_DIR=%CURRENT_DIR%WorkflowEditor

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release WorkflowEditor.pro
nmake
call postbuild.bat "%WORKING_DIR%" release "%WORKING_DIR%"

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..

echo "---------- Building project: WorkflowLauncher ----------"

cd WorkflowLauncher
SET WORKING_DIR=%CURRENT_DIR%WorkflowLauncher

REM Release.

"%QMAKE_TOOL%" -makefile CONFIG+=release WorkflowLauncher.pro
nmake
call postbuild.bat "%WORKING_DIR%" release "%WORKING_DIR%"

del .qmake.stash
del Makefile
del Makefile.Debug
del Makefile.Release

rmdir /S /Q "release"
rmdir /S /Q "debug"

cd ..

