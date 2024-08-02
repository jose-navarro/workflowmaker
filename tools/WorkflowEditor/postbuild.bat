ECHO OFF

REM Parameter 1: Build directory.
REM Parameter 2: Build configuration (debug, release)
REM Parameter 3: Project directory.

CD %3

REM create the deployment folders.

CALL "..\..\prepare_deployment.bat" "..\..\installer"

REM Copy the QT and MSVC runtimes.

CALL "..\..\copy_redistributables.bat" "..\..\installer\deployment"

REM Copy the toolkit schema & sample to the destination folder.

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying WorkflowEditor's schema and sample

COPY ..\..\data\workflow.xsd ..\..\installer\deployment\bin\workflow.xsd
COPY ..\..\samples\image_processing\sample_workflowmaker_files\image_processing_workflow.xml ..\..\installer\deployment\data_samples\image_processing_workflow.xml

REM Copy the executable to destination folders.

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying executable.

COPY "%1\%2\*.exe"          ..\..\installer\deployment\bin\*.* 
