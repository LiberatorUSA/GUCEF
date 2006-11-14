; Script for the GUCEF SDK installer


; include functions for setting the environment settings
!include "..\..\MSWIN\PathEnvUtils.nsh"

; Define your application name
!define APPNAME "GUCEF SDK for MVC8 (debug)"
!define APPNAMEANDVERSION "GUCEF SDK 0.0.0.0 (pre-release)"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\GUCEF SDK\MVC8\Debug"
InstallDirRegKey HKLM "Software\VanvelzenSoftware\GUCEF SDK\MVC8\Debug" ""
OutFile "GUCEF_SDK_MVC8_d.exe"

; Modern interface settings
!include "MUI.nsh"

!define MUI_ABORTWARNING

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\..\..\..\..\docs\Licence_LGPL.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Set languages (first is default language)
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

Section "GUCEF SDK" Section1

	; Set Section properties
	SetOverwrite on

	; Set Section Files and Shortcuts
	SetOutPath "$INSTDIR\bin"
	File "..\..\..\..\..\common\bin\Debug_MVC8\*.dll"
	SetOutPath "$INSTDIR\bin\plugins\"
	File "..\..\..\..\..\common\bin\Debug_MVC8\plugins\*.dll"
	SetOutPath "$INSTDIR\lib"
	File /nonfatal "..\..\..\..\..\common\bin\Debug_MVC8\*.lib"
	SetOutPath "$INSTDIR\lib\plugins"
	File /nonfatal "..\..\..\..\..\common\bin\Debug_MVC8\plugins\*.lib"
	SetOutPath "$INSTDIR\include"
;	File "..\..\..\..\..\common\bin\Debug_MVC8\*.lib"		
	CreateDirectory "$SMPROGRAMS\GUCEF SDK\MVC8\Debug"
	CreateShortCut "$SMPROGRAMS\GUCEF SDK\MVC8\Debug\Uninstall.lnk" "$INSTDIR\uninstall.exe"

SectionEnd

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"

	; Write the environment settings
	Push "$INSTDIR\bin"
  Call AddToPath
	Push "INCLUDE"
	Push "$INSTDIR"
	Call AddToEnvVar
	Push "LIB"
	Push "$INSTDIR"
	Call AddToEnvVar

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
	Delete "$SMPROGRAMS\GUCEF SDK\MVC8\Debug\Uninstall.lnk"

	; Clean up GUCEF SDK
	Delete "$INSTDIR\bin\*.dll"
	Delete "$INSTDIR\bin\plugins\*.dll"

	; Remove remaining directories
	RMDir "$SMPROGRAMS\GUCEF SDK\MVC8\Debug"
	RMDir "$INSTDIR\bin\plugins\"
	RMDir "$INSTDIR\bin"
	RMDir "$INSTDIR\lib"
	RMDir "$INSTDIR\include"	
	RMDir "$INSTDIR\"

	; Remove our environment settings
	Push "$INSTDIR\bin"
	Call un.RemoveFromPath	
	Push "INCLUDE"
	Push "$INSTDIR\include"
	Call un.RemoveFromEnvVar
	Push "LIB"
	Push "$INSTDIR\lib"
	Call un.RemoveFromEnvVar


SectionEnd

; eof