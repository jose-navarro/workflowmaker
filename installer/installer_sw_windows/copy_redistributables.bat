REM
REM Copy Qt and MSVC redistributables.
REM Parameter #1: path to the deployment folder.
REM

@ECHO INSTALLER'S DEPLOYMENT FOLDERS: Copying Qt and MSVC redistributables...

COPY "..\..\QT_redistributables\*.*"           "%1\bin\*.*"
COPY "..\..\QT_redistributables\platforms\*.*" "%1\bin\platforms\*.*
COPY "..\..\VC_redistributables\*.exe"         "%1\bin\*.exe"