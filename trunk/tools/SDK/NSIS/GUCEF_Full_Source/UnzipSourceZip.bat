
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

:: *** Set some environment variables ***

%ZIPTOOLDIR%\unzip.exe %ZIPOUTPUTDIR%\GUCEF_FULL_SOURCE.zip -d %ZIPOUTPUTDIR%