
SET UPDATEPROJECT_THEBATCHDIR=%~dp0

cd ..\..\dependencies\android-sdk-windows\tools
PATH=%PATH%;%CD%
cd %UPDATEPROJECT_THEBATCHDIR%

android.bat update project --target "android-8" --path %UPDATEPROJECT_THEBATCHDIR%

PAUSE