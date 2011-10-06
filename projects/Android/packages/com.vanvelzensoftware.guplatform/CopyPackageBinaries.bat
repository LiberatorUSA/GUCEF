@echo off

SET COPYPACKAGEBINARIES_THEBATCHDIR=%~dp0
CALL :COPY_PACKAGE_BINARIES 
CALL :COPY_PACKAGE_BINARIES_AS_ASSETS
GOTO :END

REM -----------------------------------------------------

:COPY_PACKAGE_BINARIES

SET DSTDIR=%COPYPACKAGEBINARIES_THEBATCHDIR%libs\armeabi
SET SRCDIR=%COPYPACKAGEBINARIES_THEBATCHDIR%..\..\libs\armeabi

ECHO.
ECHO *** Copying package binary files ***
ECHO.
ECHO From: %SRCDIR%
ECHO.
ECHO To: %SRCDIR%
ECHO.

xcopy %SRCDIR%\libmain.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefLOADER.so %DSTDIR% /Y /I /F

GOTO :eof

REM -----------------------------------------------------

:COPY_PACKAGE_BINARIES_AS_ASSETS

SET DSTDIR=%COPYPACKAGEBINARIES_THEBATCHDIR%assets\libs\armeabi
SET SRCDIR=%COPYPACKAGEBINARIES_THEBATCHDIR%..\..\libs\armeabi

ECHO.
ECHO *** Copying package binary files as assets ***
ECHO.
ECHO From: %SRCDIR%
ECHO.
ECHO To: %SRCDIR%
ECHO.

ECHO * Copy main platform modules

xcopy %SRCDIR%\libgucefLOADER.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefCORE.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefMT.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefVFS.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefGUI.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefIMAGE.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefMATH.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefDRN.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefSMDRN.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefPATCHER.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefMULTIPLAY.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefINPUT.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefCOM.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefCOMCORE.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libgucefPRODMAN.so %DSTDIR% /Y /I /F

ECHO * Copy plugins

xcopy %SRCDIR%\libdstorepluginPARSIFALXML.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libGucefLogServiceClientPlugin.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libvfspluginDVP.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libvfspluginVP.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libvfspluginZIP.so %DSTDIR% /Y /I /F

ECHO * Copy plugin dependencies

xcopy %SRCDIR%\libDVPACKSYS.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libGucefLogServiceLib.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libArchiveDiffLib.so %DSTDIR% /Y /I /F

ECHO * Copy 3rd party plugin dependencies

xcopy %SRCDIR%\libzziplib.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libfreetype.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\liblibparsifal.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libminiupnpc.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libRocketControls.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libRocketCore.so %DSTDIR% /Y /I /F
xcopy %SRCDIR%\libRocketDebugger.so %DSTDIR% /Y /I /F

GOTO :eof

REM -----------------------------------------------------

:END

IF NOT DEFINED NOPAUSE (
  PAUSE
)
