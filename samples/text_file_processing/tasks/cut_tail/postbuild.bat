REM ECHO OFF

REM Parameter 1: The project's directory.
REM Parameter 2: The name of current project platform, for example, "Win32" or "x64"
REM Parameter 3: The name of the current project configuration, for example, "Debug".
REM Parameter 4: The base name of the primary output file for the build.

CD %1

@ECHO Creating (if needed) the binaries folder.

IF NOT EXIST "..\binaries" (
  MKDIR "..\binaries"
  MKDIR "..\binaries\%2"
)

REM Copy the executable to destination folders.

@ECHO Copying executable file.

COPY .\%2\%3\%4.exe  ..\binaries\%2\*.* 

