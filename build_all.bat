REM Define the location of qmake (replace it with your actual path)

SET QMAKE_TOOL="C:\Qt\5.15.2\msvc2019_64\bin\qmake.exe"

REM Build the libraries

cd libraries

call build_libraries.bat

REM Build the tools

cd ..
cd tools

call build_tools.bat

REM That's all.

cd ..

echo All projects built


