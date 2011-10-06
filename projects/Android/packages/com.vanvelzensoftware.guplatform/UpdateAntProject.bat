
SET UPDATEPROJECT_THEBATCHDIR=%~dp0

cd ..\..\dependencies\android-sdk-windows\tools
PATH=%PATH%;%CD%
cd %UPDATEPROJECT_THEBATCHDIR%

android.bat update project -t "android-9" -n GalaxyUnlimitedPlatform -p %UPDATEPROJECT_THEBATCHDIR%

PAUSE