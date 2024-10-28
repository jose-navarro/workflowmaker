ECHO OFF

REM Parameter 1: Build directory.
REM Parameter 2: Build configuration (debug, release)
REM Parameter 3: Project directory.

@ECHO STARTING THE POSTBUILD STEP FOR ADAFinderLib

CD %3

REM create the deployment folders.

CALL "..\..\installer\installer_sw_windows\prepare_deployment.bat" "..\..\installer\installer_sw_windows"

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

REM Copy all *.hpp files.

@ECHO Copying headers (*.hpp) to the headers folder...

COPY .\src\*.hpp .\headers\*.hpp

REM Copy library

@ECHO Copying the library (.lib) to the binaries folder...

COPY "%1\%2\*.lib" .\binaries\*.*

REM Copy the version file.

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying the version file to the binaries folder...

COPY .\data_to_bin_folder\workflowmaker_version.txt       ..\..\installer\installer_sw_windows\deployment\bin\*.* 

REM Copy the documentation

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying the docs (.pdf) to the docs folder.

COPY "..\..\docs\user guide\WorkflowMaker user guide.pdf" ..\..\installer\installer_sw_windows\deployment\docs\*.* 
