ECHO OFF

REM Parameter 1: The project's directory.
REM Parameter 2: The name of current project platform, for example, "Win32" or "x64"
REM Parameter 3: The name of the current project configuration, for example, "Debug".
REM Parameter 4: The base name of the primary output file for the build.

CD %1

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Creating if needed...

IF NOT EXIST "..\..\installer\deployment" (
  MKDIR "..\..\installer\deployment"
  MKDIR "..\..\installer\deployment\bin"
  MKDIR "..\..\installer\deployment\bin\x64"
  MKDIR "..\..\installer\deployment\bin\x64\platforms"
  MKDIR "..\..\installer\deployment\data_samples"
  MKDIR "..\..\installer\deployment\docs"
)

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying QT and VC runtimes...

REM Copy the QT runtimes.

COPY ..\..\QT_redistributables\%2\*.* ..\..\installer\deployment\bin\%2\*.*
COPY ..\..\QT_redistributables\%2\platforms\*.* ..\..\installer\deployment\bin\%2\platforms\*.*

REM Copy the VC runtimes.

COPY ..\..\VC_redistributables\%2\*.* ..\..\installer\deployment\bin\%2\*.*

@ECHO COPYING TOOLKIT SCHEMA AND SAMPLE.

REM Copy the workflow schema to the destination folder.

COPY ..\..\data\workflow.xsd ..\..\installer\deployment\bin\%2\workflow.xsd
COPY ..\..\data_samples\IN2PNG_workflow.xml ..\..\installer\deployment\data_samples\IN2PNG_workflow.xml

REM Copy the executable to destination folders.

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying executable.

COPY .\%2\%3\%4.exe  ..\..\installer\deployment\bin\%2\*.* 


