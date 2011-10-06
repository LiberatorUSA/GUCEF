
SET CREATEPROJECT_THEBATCHDIR=%~dp0

cd ..\..\..\..\dependencies\android-sdk-windows\tools
PATH=%PATH%;%CD%
cd %CREATEPROJECT_THEBATCHDIR%

android.bat create project -t "android-9" -n GalaxyUnlimitedPlatform -k com.vanvelzensoftware.guplatform -a NativeActivity -p %CREATEPROJECT_THEBATCHDIR%

PAUSE