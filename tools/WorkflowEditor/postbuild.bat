ECHO OFF

REM Parameter 1: Build directory.
REM Parameter 2: Build configuration (debug, release)
REM Parameter 3: Project directory.

CD %3

REM create the deployment folders.

CALL "..\..\installer\installer_sw_windows\prepare_deployment.bat"    "..\..\installer\installer_sw_windows"

REM Copy the samples and docs.

CALL "..\..\installer\installer_sw_windows\copy_samples_and_docs.bat" "..\..\installer\installer_sw_windows\deployment"

REM Copy the QT and MSVC runtimes.

CALL "..\..\installer\installer_sw_windows\copy_redistributables.bat" "..\..\installer\installer_sw_windows\deployment"

REM Copy the toolkit schema & sample to the destination folder.

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying WorkflowEditor's schema and sample

COPY ..\..\data\workflow.xsd ..\..\installer\installer_sw_windows\deployment\bin\workflow.xsd
COPY ..\..\samples\image_processing\sample_workflowmaker_files\image_processing_workflow.xml ..\..\installer\installer_sw_windows\deployment\data_samples\image_processing_workflow.xml

REM Copy the executable to destination folders.

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying executable.

COPY "%1\%2\*.exe"          ..\..\installer\installer_sw_windows\deployment\bin\*.* 

REM Try to generate the doxygen documentation. Only if doxygen is installed.

if exist "%ProgramFiles%\doxygen\bin\doxygen.exe" (
  doxygen WorkflowEditor.dox
)
