ECHO OFF

REM Parameter 1: Build directory.
REM Parameter 2: Build configuration (debug, release)
REM Parameter 3: Project directory.

CD %3

REM create the deployment folders.

CALL "..\..\installer\installer_sw_windows\prepare_deployment.bat"    "..\..\installer\installer_sw_windows"

REM Copy the QT and MSVC runtimes.

CALL "..\..\installer\installer_sw_windows\copy_redistributables.bat" "..\..\installer\installer_sw_windows\deployment"

REM Copy the toolkit schema & sample to the destination folder.

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying ToolkitEditor's schema and sample

COPY ..\..\data\toolkit.xsd ..\..\installer\installer_sw_windows\deployment\bin\toolkit.xsd

COPY ..\..\samples\image_processing\sample_workflowmaker_files\image_processing_toolkit.xml ..\..\installer\installer_sw_windows\deployment\data_samples\image_processing_toolkit.xml

REM Copy the executable to destination folders.

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying executable.

COPY "%1\%2\*.exe"          ..\..\installer\installer_sw_windows\deployment\bin\*.* 

REM Try to generate the doxygen documentation. Only if doxygen is installed.

if exist "%ProgramFiles%\doxygen\bin\doxygen.exe" (
  doxygen ToolkitEditor.dox
  COPY .\resources\HTML_ToolkitEditor_logo.png .\doc_html\*.*
)
