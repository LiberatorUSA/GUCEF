@echo off

SET CALLBUIDUSINGNDK_THEBATCHDIR=%~dp0

ECHO.
ECHO *** Invoking build script ***
ECHO.

CALL %CALLBUIDUSINGNDK_THEBATCHDIR%..\..\BuildUsingNDK.bat

ECHO.
ECHO *** Invoking binary copy script ***
ECHO.

CALL %CALLBUIDUSINGNDK_THEBATCHDIR%CopyPackageBinaries.bat