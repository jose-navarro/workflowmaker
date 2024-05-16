ECHO OFF

REM Parameter 1: The project's directory.
REM Parameter 2: The name of current project platform, for example, "Win32" or "x64"
REM Parameter 3: The name of the current project configuration, for example, "Debug".
REM Parameter 4: The base name of the primary output file for the build.

CD %1

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

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying QT and VC runtimes...

REM Copy the QT runtime and VC redistributable installer.

COPY ..\..\QT_redistributables\%2\*.* ..\..\installer\deployment\bin\%2\*.*
COPY ..\..\QT_redistributables\%2\platforms\*.* ..\..\installer\deployment\bin\%2\platforms\*.*

COPY ..\..\VC_redistributables\%2\*.* ..\..\installer\deployment\bin\%2\*.*

REM Copy the toolkit schema & sample to the destination folder.

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying ToolkitEditor's schema and sample

COPY ..\..\data\toolkit.xsd ..\..\installer\deployment\bin\%2\toolkit.xsd
COPY ..\..\samples\image_processing\sample_workflowmaker_files\image_processing_toolkit.xml ..\..\installer\deployment\data_samples\image_processing_toolkit.xml

REM Copy the executable to destination folders.

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying executable.

COPY .\%2\%3\%4.exe  ..\..\installer\deployment\bin\%2\*.* 


