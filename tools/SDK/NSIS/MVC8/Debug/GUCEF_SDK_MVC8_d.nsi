; Script generated with the Venis Install Wizard

!define ALL_USERS
!include "..\..\MSWIN\WriteEnvStr.nsh"

; Define your application name
!define APPNAME "GUCEF SDK for MVC8 (debug)"
!define APPNAMEANDVERSION "GUCEF SDK 0.1 (pre-release)"

; Main Install settings
Name "${APPNAMEANDVERSION}"
InstallDir "$PROGRAMFILES\GUCEF SDK MVC8d"
InstallDirRegKey HKLM "Software\VanvelzenSoftware\GUCEF SDK MVC8d" ""
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
	SetOutPath "$INSTDIR\"
	File "..\..\..\..\..\common\bin\Debug_MVC8\gucefCOMCORE_MVC8_d.dll"
	File "..\..\..\..\..\common\bin\Debug_MVC8\gucefCOM_MVC8_d.dll"
	File "..\..\..\..\..\common\bin\Debug_MVC8\gucefCORE_MVC8_d.dll"
	File "..\..\..\..\..\common\bin\Debug_MVC8\gucefIMAGE_MVC8_d.dll"
	File "..\..\..\..\..\common\bin\Debug_MVC8\gucefINPUT_MVC8_d.dll"
	File "..\..\..\..\..\common\bin\Debug_MVC8\gucefMT_MVC8_d.dll"
	File "..\..\..\..\..\common\bin\Debug_MVC8\gucefPATCHER_MVC8_d.dll"
	File "..\..\..\..\..\common\bin\Debug_MVC8\gucefVFS_MVC8_d.dll"
	SetOutPath "$INSTDIR\plugins\"
	File "..\..\..\..\..\common\bin\Debug_MVC8\plugins\dstorepluginPARSIFALXML.dll"
	File "..\..\..\..\..\common\bin\Debug_MVC8\plugins\inputdriverDIRECTINPUT8.dll"
	File "..\..\..\..\..\common\bin\Debug_MVC8\plugins\inputdriverMSWINMSG.dll"
	File "..\..\..\..\..\common\bin\Debug_MVC8\plugins\vfspluginDVP.dll"
	File "..\..\..\..\..\common\bin\Debug_MVC8\plugins\vfspluginVP.dll"
	CreateDirectory "$SMPROGRAMS\GUCEF SDK"
	CreateShortCut "$SMPROGRAMS\GUCEF SDK\Uninstall.lnk" "$INSTDIR\uninstall.exe"

SectionEnd

Section -FinishSection

	WriteRegStr HKLM "Software\${APPNAME}" "" "$INSTDIR"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$INSTDIR\uninstall.exe"
	WriteUninstaller "$INSTDIR\uninstall.exe"

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
	Delete "$SMPROGRAMS\GUCEF SDK\Uninstall.lnk"

	; Clean up GUCEF SDK
	Delete "$INSTDIR\gucefCOMCORE_MVC8_d.dll"
	Delete "$INSTDIR\gucefCOM_MVC8_d.dll"
	Delete "$INSTDIR\gucefCORE_MVC8_d.dll"
	Delete "$INSTDIR\gucefIMAGE_MVC8_d.dll"
	Delete "$INSTDIR\gucefINPUT_MVC8_d.dll"
	Delete "$INSTDIR\gucefMT_MVC8_d.dll"
	Delete "$INSTDIR\gucefPATCHER_MVC8_d.dll"
	Delete "$INSTDIR\gucefVFS_MVC8_d.dll"
	Delete "$INSTDIR\plugins\dstorepluginPARSIFALXML.dll"
	Delete "$INSTDIR\plugins\inputdriverDIRECTINPUT8.dll"
	Delete "$INSTDIR\plugins\inputdriverMSWINMSG.dll"
	Delete "$INSTDIR\plugins\vfspluginDVP.dll"
	Delete "$INSTDIR\plugins\vfspluginVP.dll"

	; Remove remaining directories
	RMDir "$SMPROGRAMS\GUCEF SDK"
	RMDir "$INSTDIR\plugins\"
	RMDir "$INSTDIR\"

SectionEnd

; eof