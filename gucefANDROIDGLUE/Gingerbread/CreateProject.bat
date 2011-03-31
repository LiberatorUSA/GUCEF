
SET CREATEPROJECT_THEBATCHDIR=%~dp0

cd ..\..\dependencies\android-sdk-windows\tools
PATH=%PATH%;%CD%
cd %CREATEPROJECT_THEBATCHDIR%

android.bat create project --target 9 --name 

PAUSE