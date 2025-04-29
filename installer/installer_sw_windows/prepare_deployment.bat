REM
REM Create the deployment folders.
REM Parameter #1: the path where the deployment folder must reside.
REM

REM create the deployment folders.
@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Creating if needed...

IF NOT EXIST "%1\deployment" (
  MKDIR "%1\deployment"
)
IF NOT EXIST "%1\deployment\bin" (
  MKDIR "%1\deployment\bin"
)
IF NOT EXIST "%1\deployment\bin\platforms" (
  MKDIR "%1\deployment\bin\platforms"
)
IF NOT EXIST "%1\deployment\data" (
  MKDIR "%1\deployment\data"
)
IF NOT EXIST "%1\deployment\data_samples" (
  MKDIR "%1\deployment\data_samples"
)
IF NOT EXIST "%1\deployment\docs" (
  MKDIR "%1\deployment\docs"
)
