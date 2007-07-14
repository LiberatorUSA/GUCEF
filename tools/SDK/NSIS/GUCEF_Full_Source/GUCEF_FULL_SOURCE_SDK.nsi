; Script for the GUCE Full Source SDK installer


; include functions for setting the environment settings
!include "MSWIN\PathEnvUtils.nsh"
!include "MSWIN\WriteEnvStr.nsh"

; Define your application name
!define APPNAME "GUCEF Full Source SDK"
!define APPNAMEANDVERSION "GUCEF Full Source SDK"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\VanvelzenSoftware\GUCEF_Full_Source_SDK"
InstallDirRegKey HKLM "Software\VanvelzenSoftware\GUCEF Full Source SDK" ""
OutFile "GUCEF_FULL_SOURCE_SDK.exe"

; Modern interface settings
!include "MUI.nsh"

!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\..\..\..\docs\License_LGPL.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

Section "GUCEF Full Source SDK" Section1

	; Set Section properties
	SetOverwrite on

	; Set Section Files and Shortcuts
	SetOutPath "$INSTDIR"
	File "GUCEF_FULL_SOURCE.zip"
	File "..\..\..\..\tools\InfoZip\MSWIN\WIN32\unzip.exe"
	
	CreateDirectory "$SMPROGRAMS\GUCEF Full Source SDK\GUCEF\"
	CreateShortCut "$SMPROGRAMS\GUCEF Full Source SDK\GUCEF\API.lnk" "$INSTDIR\GUALL\GUCEF\docs\API\html\index.html"
	CreateShortCut "$SMPROGRAMS\GUCEF Full Source SDK\uninstall.lnk" "$INSTDIR\uninstall.exe"
	
SectionEnd

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"

	;Push "GUCEF_HOME"
	;Push "$INSTDIR\GUALL\GUCEF"
	;Call WriteEnvStr
	
	; Now we can extract the archive
	ExecWait "'$INSTDIR\unzip.exe' $INSTDIR\GUCEF_FULL_SOURCE.zip -d $INSTDIR"
	Delete "$INSTDIR\unzip.exe"
	Delete "$INSTDIR\GUCEF_FULL_SOURCE.zip"
	
	; Now we can generate the API docs
	
SectionEnd

; Modern install component descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
	!insertmacro MUI_DESCRIPTION_TEXT ${Section1} ""
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;Uninstall section
Section Uninstall

	;Remove from registry...
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
	DeleteRegKey HKLM "SOFTWARE\${APPNAME}"

	; Delete self
	Delete "$INSTDIR\uninstall.exe"

	; Delete Shortcuts
	RMDir "$SMPROGRAMS\GUCE Full Source SDK\GUCEF"
  RMDir "$SMPROGRAMS\GUCE Full Source SDK"
	
	; Remove remaining directories
	RMDir "$INSTDIR\GUALL\GUCEF"

	; Remove our environment settings
	Push "GUCEF_HOME"
	Push "$INSTDIR\GUALL\GUCEF"
	Call un.RemoveFromEnvVar
	
SectionEnd

; eof