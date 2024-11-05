REM Define the location of qmake (replace it with your actual path)

SET QMAKE_TOOL="C:\Qt\6.2.4\msvc2019_64\bin\qmake.exe"

REM Build the libraries

@ECHO --------------------- BUILDING THE LIBRARIES ---------------------

cd libraries

call build_libraries.bat

cd ..

REM Build the image_processing toolkit

@ECHO --------------------- BUILDING THE IMAGE_PROCESSING TOOLKIT ---------------------

cd image_processing
cd tasks

call build_tools.bat

cd ..

REM Build the image_processing toolkit installer.

cd installer
REM cd installer_sw_windows

@ECHO --------------------- BUILDING THE IMAGE_PROCESSING INSTALLER ---------------------

makensis image_tools.nsi

REM That's all.

cd ..
cd ..

REM That's all.

echo All projects built


