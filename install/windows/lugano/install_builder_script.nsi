; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "ToolMap"
;!define PRODUCT_VERSION '2.0.475' (removed for batch script)
!define PRODUCT_VERSION_LONG "2.0 Codename `Geneva`"
!define PRODUCT_PUBLISHER "CREALP"
!define PRODUCT_WEB_SITE "http://www.crealp.ch"
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_KEY "Software\ToolMap2"
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_VALUENAME "Install_Mode"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY "Software\ToolMap2"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_VALUENAME "Install_Dir"
!define MULTIUSER_INSTALLMODE_INSTDIR "ToolMap2"


; MUI 1.67 compatible ------
!define MULTIUSER_EXECUTIONLEVEL Highest
!define MULTIUSER_MUI
!define MULTIUSER_INSTALLMODE_COMMANDLINE
!include MultiUser.nsh
;!include MUI2.nsh



;!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\box-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\box-uninstall.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange-uninstall.bmp"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MULTIUSER_PAGE_INSTALLMODE
;!insertmacro MUI_PAGE_LICENSE "c:\dossier\vers\license\VotreLicenseLogicielle.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
;!define MUI_FINISHPAGE_RUN "$INSTDIR\ToolMap2.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

!insertmacro GetParameters
!insertmacro GetOptions
Var cmdLineInstallDir
 
 Function .onInit
	Call CheckToolMapRuning
	
  ${GetParameters} $0
  ClearErrors
  ${GetOptions} '$0' "/INSTDIR=" $1
  IfErrors +2
  StrCpy $cmdLineInstallDir $1
  ClearErrors

 !insertmacro MULTIUSER_INIT

  StrCmp $cmdLineInstallDir "" +2
  StrCpy $INSTDIR $cmdLineInstallDir
 FunctionEnd
 
 
Function CheckToolMapRuning
   
  StrCpy $0 "${PRODUCT_NAME}2.exe"
  KillProc::FindProcesses
  IntCmp $0 1 0 notruning 
	IfSilent +2
	MessageBox MB_ICONSTOP|MB_RETRYCANCEL "Toolmap is running. Close it and try again." IDRETRY -4
	Abort
  
  notruning: 
FunctionEnd

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "InstallToolMap.exe"
ShowInstDetails show
ShowUninstDetails hide

Section "SectionPrincipale" SEC01
  Call CheckToolMapRuning
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "..\..\..\..\bin\win32_VS2008\Debug\ToolMap2.exe"
  File "..\..\..\..\bin\win32_VS2008\Debug\ToolMap2.pdb"
  File "D:\LS\PROGRAMATION\ToolBasView\bin\Debug\ToolBasView.exe"
  CreateDirectory "$SMPROGRAMS\ToolMap 2"
  CreateShortCut "$SMPROGRAMS\ToolMap 2\ToolMap 2.lnk" "$INSTDIR\ToolMap2.exe"
  CreateShortCut "$SMPROGRAMS\ToolMap 2\ToolBasView.lnk" "$INSTDIR\ToolBasView.exe"
  CreateShortCut "$DESKTOP\ToolMap 2.lnk" "$INSTDIR\ToolMap2.exe"
  File "..\..\..\..\bin\win32_VS2008\Debug\gdal16.dll"
  File "..\..\..\..\bin\win32_VS2008\Debug\geos_c.dll"
  File "..\..\..\..\bin\win32_VS2008\Debug\libmysqld.dll"
  File "..\..\..\..\bin\win32_VS2008\Debug\libcurld.dll"
  CreateDirectory "$INSTDIR\mysql"
  SetOutPath "$INSTDIR\mysql"
  File "..\..\..\..\bin\win32_VS2008\Debug\mysql\errmsg.sys"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$SMPROGRAMS\ToolMap 2\Website.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\ToolMap 2\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr SHCTX "Software\ToolMap2" ${MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_VALUENAME}  "$INSTDIR"
  WriteRegStr SHCTX "Software\ToolMap2" ${MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_VALUENAME}  "$MultiUser.InstallMode"
 
 ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ToolMap2" "DisplayName" "$(^Name)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ToolMap2" "DisplayIcon" "$INSTDIR\ToolMap2.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ToolMap2" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ToolMap2" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ToolMap2" "Publisher" "${PRODUCT_PUBLISHER}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ToolMap2" "UninstallString" "$INSTDIR\uninst.exe"

  ;WriteRegStr ${MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  ;WriteRegStr ${MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  ;WriteRegStr ${MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\ToolMap2.exe"
  ;WriteRegStr ${MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  ;WriteRegStr ${MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  ;WriteRegStr ${MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  IfSilent +3
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) a �t� d�sinstall� avec succ�s de votre ordinateur."
FunctionEnd

Function un.onInit
  !insertmacro MULTIUSER_UNINIT
  IfSilent +3
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "�tes-vous certains de vouloir d�sinstaller totalement $(^Name) et tous ses composants ?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ToolMap2"
  DeleteRegKey SHCTX "Software\ToolMap2"
  
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\mysql\errmsg.sys"
  Delete "$INSTDIR\libmysqld.dll"
  Delete "$INSTDIR\gdal16.dll"
  Delete "$INSTDIR\geos_c.dll"
  Delete "$INSTDIR\libcurld.dll"
  Delete "$INSTDIR\ToolMap2.exe"
  Delete "$INSTDIR\ToolMap2.pdb"
  Delete "$INSTDIR\ToolBasView.exe"

  Delete "$SMPROGRAMS\ToolMap 2\Uninstall.lnk"
  Delete "$SMPROGRAMS\ToolMap 2\ToolBasView.lnk"
  Delete "$SMPROGRAMS\ToolMap 2\Website.url"
  Delete "$DESKTOP\ToolMap 2.lnk"
  Delete "$SMPROGRAMS\ToolMap 2\ToolMap 2.lnk"

  RMDir "$SMPROGRAMS\ToolMap 2"
  RMDir "$INSTDIR\mysql"
  RMDir "$INSTDIR"

  
  SetAutoClose true
SectionEnd