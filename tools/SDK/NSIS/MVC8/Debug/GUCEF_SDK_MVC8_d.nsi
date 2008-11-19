; Script for the GUCEF SDK installer


; include functions for setting the environment settings
!include "..\..\MSWIN\PathEnvUtils.nsh"
!include "..\..\MSWIN\WriteEnvStr.nsh"

; Define your application name
!define APPNAME "GUCEF SDK for MVC8 (debug)"
!define APPNAMEANDVERSION "GUCEF SDK 0.0.0.0 (pre-release)"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\VanvelzenSoftware"
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
	SetOutPath "$INSTDIR\GUCEF\docs"
	File "..\..\..\..\..\docs\Licence_LGPL.txt"	
  SetOutPath "$INSTDIR\GUCEF\common\include"
	File "..\..\..\..\..\common\include\*.h"	
	SetOutPath "$INSTDIR\GUCEF\gucefCORE\bin"
	File "..\..\..\..\..\gucefCORE\bin\Debug_MVC8\*.dll"
	File "..\..\..\..\..\gucefCORE\bin\Debug_MVC8\*.lib"
	SetOutPath "$INSTDIR\GUCEF\gucefCORE\include"
	File "..\..\..\..\..\gucefCORE\include\*.h"
	SetOutPath "$INSTDIR\GUCEF\gucefVFS\bin"
	File "..\..\..\..\..\gucefVFS\bin\Debug_MVC8\*.dll"
	File "..\..\..\..\..\gucefVFS\bin\Debug_MVC8\*.lib"
	SetOutPath "$INSTDIR\GUCEF\gucefVFS\include"
	File "..\..\..\..\..\gucefVFS\include\*.h"
	SetOutPath "$INSTDIR\GUCEF\gucefCOMCORE\bin"
	File "..\..\..\..\..\gucefCOMCORE\bin\Debug_MVC8\*.dll"
	File "..\..\..\..\..\gucefCOMCORE\bin\Debug_MVC8\*.lib"
	SetOutPath "$INSTDIR\GUCEF\gucefCOMCORE\include"
	File "..\..\..\..\..\gucefCOMCORE\include\*.h"
	SetOutPath "$INSTDIR\GUCEF\gucefCOM\bin"
	File "..\..\..\..\..\gucefCOM\bin\Debug_MVC8\*.dll"
	File "..\..\..\..\..\gucefCOM\bin\Debug_MVC8\*.lib"
	SetOutPath "$INSTDIR\GUCEF\gucefCOM\include"
	File "..\..\..\..\..\gucefCOM\include\*.h"
	SetOutPath "$INSTDIR\GUCEF\gucefINPUT\bin"
	File "..\..\..\..\..\gucefINPUT\bin\Debug_MVC8\*.dll"
	File "..\..\..\..\..\gucefINPUT\bin\Debug_MVC8\*.lib"
	SetOutPath "$INSTDIR\GUCEF\gucefINPUT\include"
	File "..\..\..\..\..\gucefINPUT\include\*.h"
	SetOutPath "$INSTDIR\GUCEF\gucefMT\bin"
	File "..\..\..\..\..\gucefMT\bin\Debug_MVC8\*.dll"
	File "..\..\..\..\..\gucefMT\bin\Debug_MVC8\*.lib"
	SetOutPath "$INSTDIR\GUCEF\gucefMT\include"
	File "..\..\..\..\..\gucefMT\include\*.h"
	SetOutPath "$INSTDIR\GUCEF\gucefIMAGE\bin"
	File "..\..\..\..\..\gucefIMAGE\bin\Debug_MVC8\*.dll"
	File "..\..\..\..\..\gucefIMAGE\bin\Debug_MVC8\*.lib"
	SetOutPath "$INSTDIR\GUCEF\gucefIMAGE\include"
	File "..\..\..\..\..\gucefIMAGE\include\*.h"	
	SetOutPath "$INSTDIR\GUCEF\gucefPATCHER\bin"
	File /nonfatal "..\..\..\..\..\gucefPATCHER\bin\Debug_MVC8\*.dll"
	File /nonfatal "..\..\..\..\..\gucefPATCHER\bin\Debug_MVC8\*.lib"
	SetOutPath "$INSTDIR\GUCEF\gucefPATCHER\include"
	File /nonfatal "..\..\..\..\..\gucefPATCHER\include\*.h"
	SetOutPath "$INSTDIR\GUCEF\gucefDRN\bin"
	File /nonfatal "..\..\..\..\..\gucefDRN\bin\Debug_MVC8\*.dll"
	File /nonfatal "..\..\..\..\..\gucefDRN\bin\Debug_MVC8\*.lib"
	SetOutPath "$INSTDIR\GUCEF\gucefDRN\include"
	File /nonfatal "..\..\..\..\..\gucefDRN\include\*.h"
	SetOutPath "$INSTDIR\GUCEF\plugins\bin\Debug_MVC8"
	File /nonfatal "..\..\..\..\..\common\bin\Debug_MVC8\plugins\*.dll"
	
	CreateDirectory "$SMPROGRAMS\GUCEF SDK\MVC8\Debug"
	CreateShortCut "$SMPROGRAMS\GUCEF SDK\MVC8\Debug\Uninstall.lnk" "$INSTDIR\GUCEF\uninstall.exe"
	CreateShortCut "$SMPROGRAMS\GUCEF SDK\MVC8\Debug\licence.lnk" "$INSTDIR\GUCEF\docs\Licence_LGPL.txt"

SectionEnd

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\GUCEF\uninstall.exe"
	WriteUninstaller "$INSTDIR\GUCEF\uninstall.exe"

	Push "GUCEF_HOME"
	Push "$INSTDIR\GUCEF"
	Call WriteEnvStr

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
	Delete "$INSTDIR\GUCEF\uninstall.exe"

	; Delete Shortcuts
	Delete "$SMPROGRAMS\GUCEF SDK\MVC8\Debug\Uninstall.lnk"

	; Clean up GUCEF SDK
	Delete "$INSTDIR\GUCEF\docs\*.*"
	Delete "$INSTDIR\GUCEF\gucefCORE\bin\Debug_MVC8\*.*"
	Delete "$INSTDIR\GUCEF\gucefMT\bin\Debug_MVC8\*.*"
	Delete "$INSTDIR\GUCEF\gucefIMAGE\bin\Debug_MVC8\*.*"
	Delete "$INSTDIR\GUCEF\gucefVFS\bin\Debug_MVC8\*.*"
	Delete "$INSTDIR\GUCEF\gucefCOMCORE\bin\Debug_MVC8\*.*"
	Delete "$INSTDIR\GUCEF\gucefCOM\bin\Debug_MVC8\*.*"
	Delete "$INSTDIR\GUCEF\gucefPATCHER\bin\Debug_MVC8\*.*"
	Delete "$INSTDIR\GUCEF\gucefDRN\bin\Debug_MVC8\*.*"
	Delete "$INSTDIR\GUCEF\plugins\bin\Debug_MVC8\*.*"

	; Remove remaining directories
	RMDir "$SMPROGRAMS\GUCEF SDK\MVC8\Debug"
	RMDir "$INSTDIR\GUCEF"

	; Remove our environment settings
	Push "GUCEF_HOME"
	Push "$INSTDIR\GUCEF"
	Call un.RemoveFromEnvVar

SectionEnd

; eof