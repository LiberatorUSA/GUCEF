@echo off

SET LISTDEVICES_THEBATCHDIR=%~dp0

cd ..\..\dependencies\android-sdk-windows\platform-tools
PATH=%PATH%;%CD%
cd %LISTDEVICES_THEBATCHDIR%

@echo on

adb devices

@echo off
PAUSE