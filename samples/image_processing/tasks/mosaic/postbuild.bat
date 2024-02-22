REM ECHO OFF

REM Parameter 1: The name of current project platform. It should always be "x64"
REM Parameter 2: The name of the current project configuration. It should always be "Release".
REM Parameter 3: The base name of the primary output file for the build.

If "%2"=="Debug" (@ECHO LEAVING POSTBUILD - Executable not copied to binary folder)
If "%2"=="Debug" (exit 0)

@ECHO Creating (if needed) the binaries folder.

IF NOT EXIST "..\binaries" (
  MKDIR "..\binaries"
  MKDIR "..\binaries\%1"
)

REM Copy the executable to destination folders.

@ECHO Copying executable file.

COPY .\%1\%2\%3.exe  ..\binaries\%1\*.* 

