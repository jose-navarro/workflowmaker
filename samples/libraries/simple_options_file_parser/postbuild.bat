ECHO OFF

REM Parameter 1: The project's directory.
REM Parameter 2: The path to the build directory.

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

COPY "%2\*.lib" .\binaries\*.*
