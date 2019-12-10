
SET ANDROIDHELP_THEBATCHDIR=%~dp0

cd ..\..\dependencies\android-sdk-windows\tools
PATH=%PATH%;%CD%
cd %ANDROIDHELP_THEBATCHDIR%

android.bat --help
pause