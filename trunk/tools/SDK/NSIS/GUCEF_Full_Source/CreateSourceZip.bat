
:: *** Get rid of an existing package ***

del GUCEF_FULL_SOURCE.zip

:: *** Set some environment variables ***

SET ZIPOUTPUTDIR=%CD%
cd..
cd..
cd..
cd InfoZip
cd MSWIN
cd WIN32
SET ZIPTOOLDIR=%CD%
cd..
cd..
cd..
cd..
SET ARCHIVEROOTDIR=%CD%

:: *** Get rid of data we dont need in the GUCEF dirs ***

:: - Disabled in favor of a removal after zip creation
::   it is a better idea to mess with the zip then with the archive ;)

:: *** Create the zip file using the GUCEF archive + dependencies ***

%ZIPTOOLDIR%\zip.exe -v -r %ZIPOUTPUTDIR%\GUCEF_FULL_SOURCE.zip %ARCHIVEROOTDIR% -x@%ZIPOUTPUTDIR%\ExcludeFromZip.txt

:: *** Get rid of data we dont need in the zip GUCEF dirs ***

%ZIPTOOLDIR%\zip.exe -d %ZIPOUTPUTDIR%\GUCEF_FULL_SOURCE.zip "GUALL/GUCEF/gucefCORE/bin\*" "GUALL/GUCEF/gucefMT/bin\*" "GUALL/GUCEF/gucefAUDIO/bin\*" "GUALL/GUCEF/gucefCOM/bin\*" "GUALL/GUCEF/gucefCOM_TestApp/bin\*" "GUALL/GUCEF/gucefCOMCORE/bin\*" "GUALL/GUCEF/gucefCOMCORE_TestApp/bin\*" "GUALL/GUCEF/gucefCORE_TestApp/bin\*"  "GUALL/GUCEF/gucefDRN/bin\*" "GUALL/GUCEF/gucefDRN_TestApp/bin\*" "GUALL/GUCEF/gucefGUI/bin\*" "GUALL/GUCEF/gucefIMAGE/bin\*" "GUALL/GUCEF/gucefIMAGE_TestApp/bin\*" "GUALL/GUCEF/gucefINPUT/bin\*" "GUALL/GUCEF/gucefINPUT_TestApp/bin\*" "GUALL/GUCEF/gucefMATH/bin\*" "GUALL/GUCEF/gucefMT_TestApp/bin\*" "GUALL/GUCEF/gucefPATCHER/bin\*" "GUALL/GUCEF/gucefVFS/bin\*" "GUALL/GUCEF/gucefVFS_TestApp/bin\*" "GUALL/GUCEF/common/bin/Debug*" "GUALL/GUCEF/common/bin/Release*" "GUALL/GUCEF/docs/API\*" "GUALL/GUCEF/tools/PatchSetGenerator/bin\*" "GUALL/GUCEF/tools/PatcherGUI/MFCPatcherGUI/bin\*" "GUALL/GUCEF/tools/DVPPackTool/bin\*"

REM *** FINISHED!!! ***