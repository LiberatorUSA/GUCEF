; Script for the GUCE Full Source SDK installer


; include functions for setting the environment settings
!include "MSWIN\PathEnvUtils.nsh"
!include "MSWIN\WriteEnvStr.nsh"

; Define your application name
!define APPNAME "GUCE Full Source SDK"
!define APPNAMEANDVERSION "GUCE Full Source SDK"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\VanvelzenSoftware\GUCE_Full_Source_SDK"
InstallDirRegKey HKLM "Software\VanvelzenSoftware\GUCE Full Source SDK" ""
OutFile "GUCE_FULL_SOURCE_SDK.exe"

; Modern interface settings
!include "MUI.nsh"

!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\..\..\..\docs\Licence_LGPL.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

Section "GUCE Full Source SDK" Section1

	; Set Section properties
	SetOverwrite on

	; Set Section Files and Shortcuts
	SetOutPath "$INSTDIR"
	File "GUCE_FULL_SOURCE.zip"
	File "..\..\..\..\tools\InfoZip\MSWIN\WIN32\unzip.exe"
	
	CreateDirectory "$SMPROGRAMS\GUCE Full Source SDK\GUCE\"
	CreateShortCut "$SMPROGRAMS\GUCE Full Source SDK\GUCE\API.lnk" "$INSTDIR\GUALL\GUCE\docs\API\html\index.html"
	CreateDirectory "$SMPROGRAMS\GUCE Full Source SDK\GUCEF\"
	CreateShortCut "$SMPROGRAMS\GUCE Full Source SDK\GUCEF\API.lnk" "$INSTDIR\GUALL\GUCEF\docs\API\html\index.html"
	CreateDirectory "$SMPROGRAMS\GUCE Full Source SDK\Ogre\"
	CreateShortCut "$SMPROGRAMS\GUCE Full Source SDK\Ogre\API.lnk" "$INSTDIR\GUALL\GUCE\dependency\Ogre\Docs\api\html\index.html"
	;CreateDirectory "$SMPROGRAMS\GUCE Full Source SDK\CEGUI\"
  ;CreateShortCut "$SMPROGRAMS\GUCE Full Source SDK\CEGUI\API.lnk" "$INSTDIR\GUALL\GUCE\dependency\CEGUI\Docs\api\html\index.html"
	CreateShortCut "$SMPROGRAMS\GUCE Full Source SDK\uninstall.lnk" "$INSTDIR\uninstall.exe"
	
SectionEnd

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"

	Push "GUCEF_HOME"
	Push "$INSTDIR\GUALL\GUCEF"
	Call WriteEnvStr

	Push "GUCE_HOME"
	Push "$INSTDIR\GUALL\GUCE"
	Call WriteEnvStr

	Push "OGRE_HOME"
	Push "$INSTDIR\GUALL\GUCE\dependency\Ogre"
	Call WriteEnvStr

	Push "CEGUI_HOME"
	Push "$INSTDIR\GUALL\GUCE\dependency\CEGUI"
	Call WriteEnvStr
	
	; Now we can extract the archive
	ExecWait "'$INSTDIR\unzip.exe' $INSTDIR\GUCE_FULL_SOURCE.zip -d $INSTDIR"
	
	;Delete "$INSTDIR\unzip.exe"
	;Delete "$INSTDIR\GUCE_FULL_SOURCE.zip"
	
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
	RMDir "$SMPROGRAMS\GUCE Full Source SDK\GUCE"
	RMDir "$SMPROGRAMS\GUCE Full Source SDK\GUCEF"
	RMDir "$SMPROGRAMS\GUCE Full Source SDK\OGRE"
	;RMDir "$SMPROGRAMS\GUCE Full Source SDK\CEGUI"
  RMDir "$SMPROGRAMS\GUCE Full Source SDK"
	
	; Remove remaining directories
	RMDir "$INSTDIR\GUALL"

	; Remove our environment settings
	Push "GUCEF_HOME"
	Push "$INSTDIR\GUALL\GUCEF"
	Call un.RemoveFromEnvVar

	Push "GUCE_HOME"
	Push "$INSTDIR\GUALL\GUCE"
	Call un.RemoveFromEnvVar
	
	Push "OGRE_HOME"
	Push "$INSTDIR\GUALL\GUCE\dependency\Ogre"
	Call un.RemoveFromEnvVar

	Push "CEGUI_HOME"
	Push "$INSTDIR\GUALL\GUCE\dependency\CEGUI"
	Call un.RemoveFromEnvVar
	
SectionEnd

; eof