!include "FileFunc.nsh"
!include "EnvVarUpdate.nsh"

!define ALL_USERS

RequestExecutionLevel admin ;Require admin rights on NT6+ (When UAC is turned on)

; Some definitions used later on. Change HERE its values tto customize the
; installer.

!define InstallFrom        ".\deployment\bin"
!define defCompanyShortDir "cttc"
!define defVersion         "1.1.0"
!define defProductName     "WorkflowMaker"
!define defPartNumber      "${defProductName}-${defVersion}"
!define defPublisher       "Geomatics RU, CTTC"
!define UNINST_KEY         "Software\Microsoft\Windows\CurrentVersion\Uninstall\${defProductName}"
!define INST_KEY           "Software\${defProductName}"

; Full path to the company's folder, where everything from such company
; will be installed.

Var /GLOBAL CompanyDir

; Macros

!macro setOwnVariables

  ; The default installation & company directories.
  
  StrCpy $INSTDIR "$PROGRAMFILES64\${defCompanyShortDir}\${defProductName}"
  StrCpy $CompanyDir "$PROGRAMFILES64\${defCompanyShortDir}"

!macroend

!macro BIMAGE IMAGE PARMS
	Push $0
	GetTempFileName $0
	File /oname=$0 "${IMAGE}"
	SetBrandingImage ${PARMS} $0
	Delete $0
	Pop $0
!macroend

!define ENV_SETTINGS 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment'

; Installer settings
;---------------------------------

; The name of the installer
Name "${defProductName} ${defVersion}"

; The caption for the installer.
Caption "${defProductName} ${defVersion} installer"

LicenseData license.txt

; Installer icon
Icon "./icons/WorkflowMaker.ico"

; Branding image
AddBrandingImage left 62

; Uninstaller icon
UninstallIcon "./icons/WorkflowMaker.ico"

; The file to write (the installer itself)
OutFile "${defProductName}-${defVersion}-setup.exe"

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
;InstallDirRegKey HKLM "Software\${defProductName}" "Install_Dir"

;--------------------------------

; Pages

Page license brandImage
Page components brandImage
Page directory brandImage
Page instfiles brandImage

UninstPage uninstConfirm un.brandImage
UninstPage instfiles un.brandImage

;--------------------------------

; Functions

; The next two functions do exactly the same: put a brand image
; in the installer pages. The first one is to be used with
; the INSTALLER, while the second one, with the UNINSTALLER.
; Note that the uninstaller functions must begin with "un."
; while installer's must not. Therefore, the very same function
; is repeated with a different name to make it available for
; both the installer and uninstaller... :-(

Function brandImage
	!insertmacro BIMAGE ".\images\net_brand.bmp" /RESIZETOFIT
FunctionEnd

Function un.brandImage
	!insertmacro BIMAGE ".\images\net_brand.bmp" /RESIZETOFIT
FunctionEnd

; The next .onInit version has been provided and programmed to show a splash
; screen

Function .onInit

  # Make sure that our own variables are set.
  
  !insertmacro setOwnVariables

  # Prevent multiple copies of installer from being open ------------------------------------
  
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "TEMPLATE_SETUP") i .r1 ?e'
  Pop $R0
  StrCmp $R0 0 +3
  MessageBox MB_OK|MB_ICONEXCLAMATION "The installer is already running."
  Abort
  
  # End of multiple copies prevention -------------------------------------------------------
  
  # Check if the user trying to install the software has administrative rights or not -------

  ClearErrors
  
  UserInfo::GetName
  IfErrors Win9x
  
  Pop $0
  
  UserInfo::GetAccountType
  Pop $1
  ${If} $1 != "admin" ; Reguire admin rights on NT6+ (when UAC is turned on)
    MessageBox mb_iconstop 'You are logged as user$\n$\n "$0" $\n$\nwhich is ** NOT ** an administrator. $\n$\nYou must have administrator privileges to proceed.'
    SetErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
    Abort
  ${Endif}
  
  goto AdminDone

Win9x:
  # This one means you don't need to care about admin or
  # not admin because Windows 9x doesn't either
  
  MessageBox MB_OK "Error! Windows 9x not supported"
  Abort

AdminDone:

  # End of administrator check ---------------------------------------------------------------
  
  # Splash screen ----------------------------------------------------------------------------

  # the plugins dir is automatically deleted when the installer exits
  
  InitPluginsDir
  File /oname=$PLUGINSDIR\net_splash.bmp ".\images\net_splash.bmp"
	
  splash::show 4000 $PLUGINSDIR\net_splash

  Pop $0 ; $0 has '1' if the user closed the splash screen early,
		; '0' if everything closed normal, and '-1' if some error occured.
		
  # End of splash screen ---------------------------------------------------------------------
			

  # Check if user really wants to install ----------------------------------------------------
  
  MessageBox MB_YESNO "This will install ${defProductName} ${defVersion}. $\nDo you wish to continue?" IDYES gogogo
  Abort
  gogogo:
  # End of check about how sure the user is ---------------------------------------------------
  
FunctionEnd

Function .onInstSuccess

  
  MessageBox MB_OK 'Thank you very much for installing ${defProductName} ${defVersion}'
  
  ; Check if we have to reboot...
  
  IfRebootFlag 0 noreboot
    MessageBox MB_YESNO "A reboot is required to finish the installation. Do you wish to reboot now?" IDNO noreboot
    Reboot
  noreboot:
FunctionEnd

Function un.onInit
  # Make sure that our own variables are set.
  !insertmacro setOwnVariables
FunctionEnd

Function un.onUninstSuccess

  ; Check if we have to reboot...
  
  IfRebootFlag 0 noreboot
    MessageBox MB_YESNO "A reboot is required to finish the deinstallation. Do you wish to reboot now?" IDNO noreboot
    Reboot
  noreboot:
FunctionEnd


Function .onRebootFailed
   MessageBox MB_OK|MB_ICONSTOP "Reboot failed. Please reboot manually." /SD IDOK
FunctionEnd

Function un.onRebootFailed
   MessageBox MB_OK|MB_ICONSTOP "Reboot failed. Please reboot manually." /SD IDOK
FunctionEnd

; Function that deletes a directory but only when it's empty.
; To use it: put the directory to delete in $0 and call the function.
; Example:
; StrCpy $0 "$SMPROGRAMS\PwStore"
; Call un.DeleteDirIfEmpty

Function un.DeleteDirIfEmpty
  FindFirst $R0 $R1 "$0\*.*"
  strcmp $R1 "." 0 NoDelete
   FindNext $R0 $R1
   strcmp $R1 ".." 0 NoDelete
    ClearErrors
    FindNext $R0 $R1
    IfErrors 0 NoDelete
     FindClose $R0
     Sleep 1000
     RMDir "$0"
  NoDelete:
   FindClose $R0
FunctionEnd


;--------------------------------

; The stuff to install

Section "WorkflowMaker applications"

  SectionIn RO
  
  ;
  ; FIRST COPY the executables included in this section...
  ;
  
  ; Set output path to the installation\bin directory.
  SetOutPath $INSTDIR\bin
  
  ;
  ; Copy the output files from the proper directory, taking
  ; into account whether we're installing for 32 or 64 bit.
  ;
  
  ; INSTALLING FOR 64-BIT ONLY.
    
  ; Copy runtimes (QT + VC) and any other DLL files.
  File ${InstallFrom}\*.dll

  ; Copy the executables, including the external tools.

  File ${InstallFrom}\*.exe
  
  ; Copy the XML schemas.

  File ${InstallFrom}\*.xsd
  
  ; Copy the version file

  File ${InstallFrom}\*.txt

  ; Set output path to the installation\platforms directory.
  SetOutPath $INSTDIR\bin\platforms

  ; Copy QT's platform files.
  File ${InstallFrom}\platforms\*.*
  
  ; Create shortcuts.
  SetShellVarContext all
  
  CreateDirectory "$SMPROGRAMS\${defProductName}"
  CreateShortCut  "$SMPROGRAMS\${defProductName}\ToolkitEditor.lnk" "$INSTDIR\bin\ToolkitEditor.exe" "" "$INSTDIR\bin\ToolkitEditor.exe" 0
  CreateShortCut  "$SMPROGRAMS\${defProductName}\WorkflowEditor.lnk" "$INSTDIR\bin\WorkflowEditor.exe" "" "$INSTDIR\bin\WorkflowEditor.exe" 0
  CreateShortCut  "$SMPROGRAMS\${defProductName}\WorkflowLauncher.lnk" "$INSTDIR\bin\WorkflowLauncher.exe" "" "$INSTDIR\bin\WorkflowLauncher.exe" 0
  
  ;
  ; NOW copy the license file.
  ;

  ; Set output path to the installation\docs directory.

  SetOutPath $INSTDIR

  ; Copy the whole set of documents in the docs folder

  File "license.txt"

 ; Create a specific shortcut for the license file.

  CreateShortCut "$SMPROGRAMS\${defProductName}\license.lnk" "$INSTDIR\license.txt" "" "$INSTDIR\license.txt" 0

SectionEnd

Section "User guides"

  ; Set output path to the installation\docs directory.

  SetOutPath $INSTDIR\docs
  
  ; Copy the whole set of documents in the docs folder

  File /r ".\deployment\docs\*.*"

  ; Create a specific shortcut for the docs folder.

  SetShellVarContext all
  CreateShortCut "$SMPROGRAMS\${defProductName}\${defProductName} documentation folder.lnk" "$INSTDIR\docs" "" "$INSTDIR\docs" 0

SectionEnd

Section "Data samples"
;
; Set output path to the installation\data_samples directory.

  SetOutPath $INSTDIR\data_samples

; Copy the whole set of documents in the data_samples folder

  File /r ".\deployment\data_samples\*.*"

; Create a specific shortcut for the data_samples folder.

  SetShellVarContext all
  CreateShortCut "$SMPROGRAMS\${defProductName}\${defProductName} Data samples folder.lnk" "$INSTDIR\data_samples" "" "$INSTDIR\data_samples" 0

SectionEnd

;Section "Software samples"
;
; Set output path to the installation\software_samples directory.

;  SetOutPath $INSTDIR\software_samples

; Copy the whole set of documents in the data_samples folder

;  File /r "..\deployment\software_samples\*.*"

; Create a specific shortcut for the data_samples folder.

;  SetShellVarContext all
;  CreateShortCut "$SMPROGRAMS\${defProductName}\${defProductName} Software samples folder.lnk" "$INSTDIR\software_samples" "" "$INSTDIR\software_samples" 0

;SectionEnd

Section "Register toolset and create uninstaller"

  SectionIn RO

  ; Add our binary path to the PATH environment variable.
  
  ${EnvVarUpdate} $0 "PATH" "A" "HKLM" $INSTDIR\bin ; Append
  
  ; Set output path to the installation\bin directory.
  SetOutPath $INSTDIR\bin
  
  ; Copy the icon for the whole toolset.
  
  File ".\icons\WorkflowMaker.ico"
  
  ; Compute the size of the toolset. The size is stored in $0
  
  ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
  IntFmt $0 "0x%08X" $0

  ; Write the installation path into the registry
  WriteRegStr HKLM "${INST_KEY}" "Install_Dir" "$INSTDIR"

  ; Write the current version into the registry.
  WriteRegStr HKLM "${INST_KEY}" "Version" "${defVersion}"

  ; Write the uninstall keys for Windows (icon, name, version, publisher, uninstallation path, modify & repair flags).

  WriteRegStr   HKLM "${UNINST_KEY}" "DisplayIcon"     "$INSTDIR\bin\WorkflowMaker.ico"
  WriteRegStr   HKLM "${UNINST_KEY}" "DisplayName"     "${defProductName} ${defVersion}"
  WriteRegStr   HKLM "${UNINST_KEY}" "DisplayVersion"  "${defVersion}"
  WriteRegStr   HKLM "${UNINST_KEY}" "Publisher"       "${defPublisher}"
  WriteRegStr   HKLM "${UNINST_KEY}" "UninstallString" "$INSTDIR\uninstall.exe"
  WriteRegDWORD HKLM "${UNINST_KEY}" "EstimatedSize"   "$0"
  WriteRegDWORD HKLM "${UNINST_KEY}" "NoModify"        1
  WriteRegDWORD HKLM "${UNINST_KEY}" "NoRepair"        1
  
  ; Write the uninstaller.

  WriteUninstaller "uninstall.exe"
  
  ; Create shortcuts.

  CreateShortCut "$SMPROGRAMS\${defProductName}\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0

SectionEnd

Section "Microsoft Visual Studio Runtime"

  SectionIn RO

  ; Add our binary path to the PATH environment variable.

  ${EnvVarUpdate} $0 "PATH" "A" "HKLM" $INSTDIR\bin ; Append

  ; Set output path to the installation\bin directory.
  SetOutPath $INSTDIR\bin

  ; Copy the MSVC redistributable installer.
  File "${InstallFrom}\VC_redist.x64.exe"

  ; Launch the installer
  ExecWait "$INSTDIR\bin\VC_redist.x64.exe"

  ; Remove the installer (we need it no more in our destination binary folder).
  Delete "$INSTDIR\bin\VC_redist.x64.exe"

SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"

  ; Remove our binary path from the PATH environment variable.
  
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKLM" $INSTDIR\bin

  ; Remove registry keys
  DeleteRegKey HKLM "${UNINST_KEY}"
  DeleteRegKey HKLM "${INST_KEY}"
  
  SetShellVarContext all
  SetOutPath "$SMPROGRAMS"
  
  ; Remove shortcuts and shortcuts directories.
  
  Delete "$SMPROGRAMS\${defProductName}\*.*"
  RMDir /r  "$SMPROGRAMS\${defProductName}"
  
  ; Remove the company shortcut folder, but only if it's empty
  
  ;StrCpy $0 "$SMPROGRAMS\cttc"
  ;Call un.DeleteDirIfEmpty
  
  ; Remove the installation directory completely.
  
  SetOutPath "$INSTDIR"
  SetOutPath ".."
  RMDir /r $INSTDIR

  ; Remove the company directory, but only when it's empty, since other products
  ; may be installed there
  
  SetOutPath "$CompanyDir"
  SetOutPath ".."
  
  StrCpy $0 "$CompanyDir"
  Call un.DeleteDirIfEmpty

SectionEnd
