ECHO OFF

REM Parameter 1: Path to the directory where THIS file is stored.
REM Parameter 2: The name of current project platform, for example, "Win32" or "x64"
REM Parameter 3: The name of the current project configuration, for example, "Debug".
REM Parameter 4: The base name of the primary output file for the build.

@ECHO STARTING THE POSTBUILD STEP FOR WorkflowMakerLib

REM Create the header folder if it does not exist.

CD %1

@ECHO Creating the headers and binaries folders if these do not exist yet...

IF NOT EXIST ".\headers" (
  @ECHO CREATING THE headers FOLDER...
  MKDIR ".\headers"
)

REM Create the binary folder if it does not exist.

IF NOT EXIST ".\binaries" (
  @ECHO CREATING THE binaries FOLDER...
  MKDIR ".\binaries"
)

REM
REM Create the installer's deployment directories. Note that
REM we check for the existence of every individual sub-folder
REM instead of just checking for the existence of the root one.
REM In this way, if the user deletes any of the sub-folders,
REM we'll still detect it and re-create it.
REM

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Creating if these do not exist yet...

IF NOT EXIST "..\..\installer\deployment" (
  MKDIR "..\..\installer\deployment"
)

IF NOT EXIST "..\..\installer\deployment\bin" (
  MKDIR "..\..\installer\deployment\bin"
)

IF NOT EXIST "..\..\installer\deployment\bin\x64" (
  MKDIR "..\..\installer\deployment\bin\x64"
)

IF NOT EXIST "..\..\installer\deployment\bin\x64\platforms" (
  MKDIR "..\..\installer\deployment\bin\x64\platforms"
)

IF NOT EXIST "..\..\installer\deployment\data_samples" (
  MKDIR "..\..\installer\deployment\data_samples"
)

IF NOT EXIST "..\..\installer\deployment\docs" (
  MKDIR "..\..\installer\deployment\docs"
)


REM Copy all *.hpp files.

@ECHO Copying headers (.h,.hpp) to the headers folder...

COPY .\src\*.hpp .\headers\*.hpp

REM Copy library

@ECHO Copying the library (.lib) to the binaries folder...

COPY .\%2\%3\%4.lib .\binaries\*.*

REM Copy the version file.

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying the version file to the binaries folder...

COPY .\data_to_bin_folder\workflowmaker_version.txt ..\..\installer\deployment\bin\%2\*.* 

REM Copy the documentation

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying the docs (.pdf) to the docs folder.

COPY "..\..\docs\user guide\WorkflowMaker user guide.pdf" ..\..\installer\deployment\docs\*.* 
