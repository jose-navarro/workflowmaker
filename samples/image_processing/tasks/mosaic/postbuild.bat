ECHO OFF

REM Parameter 1: The project's directory.
REM Parameter 2: The path to the build directory.
REM Parameter 3: Name of the executable file to copy.

CD %1

@ECHO Creating (if needed) the binaries folder.

IF NOT EXIST "..\binaries" (
  MKDIR "..\binaries"
)

REM Copy the executable to destination folders.

@ECHO Copying executable file.

COPY "%2\%3.exe"  ..\binaries\*.* 
