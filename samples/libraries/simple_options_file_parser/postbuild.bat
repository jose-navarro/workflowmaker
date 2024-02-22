REM Parameter 1: Path to the directory where THIS file is stored.
REM Parameter 2: The name of current project platform, for example, "Win32" or "x64"
REM Parameter 3: The name of the current project configuration, for example, "Debug".
REM Parameter 4: The base name of the primary output file for the build.

ECHO OFF

@ECHO STARTING THE POSTBUILD STEP FOR THE SIMPLE_OPTIONS_FILE_PARSER library

REM Create the header folder if it does not exist.

CD %1

IF NOT EXIST ".\headers" (
  @ECHO CREATING THE headers FOLDER...
  MKDIR ".\headers"
)

REM Create the binary folder if it does not exist.

IF NOT EXIST ".\binaries" (
  @ECHO CREATING THE binaries FOLDER...
  MKDIR ".\binaries"
)

REM Copy all *.hpp files.

@ECHO COPYING HEADERS TO THE headers FOLDER...

COPY .\src\*.hpp .\headers\*.hpp

REM Copy library

@ECHO COPYING THE LIBRARY TO THE binaries FOLDER...

COPY .\%2\%3\%4.lib .\binaries\*.*
