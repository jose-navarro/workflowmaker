REM
REM Copy the complete set of samples and user guides.
REM

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying user guides and data samples...

COPY "..\..\docs\user guide\*.pdf"                                      "%1\docs\*.*"
COPY "..\..\samples\libraries\simple_options_file_parser\docs\*.pdf"    "%1\docs\*.*"
COPY "..\..\samples\image_processing\docs\*.pdf"                        "%1\docs\*.*"
COPY "..\..\samples\image_processing\sample_workflowmaker_files\*.xml"  "%1\data_samples\*.*"
