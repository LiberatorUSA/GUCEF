
SET TARGETS_THEBATCHDIR=%~dp0

cd ..\..\dependencies\android-sdk-windows\tools
PATH=%PATH%;%CD%
cd %TARGETS_THEBATCHDIR%

android.bat list targets
pause