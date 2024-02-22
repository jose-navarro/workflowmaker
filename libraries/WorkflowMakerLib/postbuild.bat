ECHO OFF

REM Parameter 1: Path to the directory where THIS file is stored.
REM Parameter 2: The name of current project platform, for example, "Win32" or "x64"
REM Parameter 3: The name of the current project configuration, for example, "Debug".
REM Parameter 4: The base name of the primary output file for the build.

@ECHO STARTING THE POSTBUILD STEP FOR WorkflowMakerLib

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

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Creating if needed...

IF NOT EXIST "..\..\installer\deployment" (
  MKDIR "..\..\installer\deployment"
  MKDIR "..\..\installer\deployment\bin"
  MKDIR "..\..\installer\deployment\bin\x64"
  MKDIR "..\..\installer\deployment\bin\x64\platforms"
  MKDIR "..\..\installer\deployment\data_samples"
  MKDIR "..\..\installer\deployment\docs"
)

REM Copy all *.hpp files.

@ECHO COPYING HEADERS TO THE headers FOLDER...

COPY .\src\*.hpp .\headers\*.hpp

REM Copy library

@ECHO COPYING THE LIBRARY TO THE binaries FOLDER...

COPY .\%2\%3\%4.lib .\binaries\*.*

REM Copy the version file.

@ECHO COPYING THE VERSION FILE TO THE binaries FOLDER...

COPY .\data_to_bin_folder\workflowmaker_version.txt ..\..\installer\deployment\bin\%2\*.* 
