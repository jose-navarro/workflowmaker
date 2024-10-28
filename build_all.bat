REM Define the location of qmake (replace it with your actual path)

SET QMAKE_TOOL="C:\Qt\6.2.4\msvc2019_64\bin\qmake.exe"

REM Build the libraries

cd libraries
call build_libraries.bat
cd ..

REM Build the tools

cd tools
call build_tools.bat
cd ..

REM Build the installer.

cd installer
cd installer_sw_windows

@ECHO --------------------- BUILDING THE INSTALLER ---------------------

makensis WorkflowMaker.nsi

cd ..
cd ..

REM That's all.

echo All projects and installer built