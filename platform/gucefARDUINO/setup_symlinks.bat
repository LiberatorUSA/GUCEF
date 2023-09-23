@echo off
SET GUCEF_THEBATCHDIR=%~dp0

echo --- Setting up gucefARDUINO library symlink
echo ---
cmd /c mklink /d /J "%UserProfile%\Documents\Arduino\libraries\gucefARDUINO" "%GUCEF_THEBATCHDIR%"
echo ---

echo --- Setting up gucefARDUINO library fake subdir symlinks
echo ---
mkdir "%GUCEF_THEBATCHDIR%\src\common"
cmd /c mklink /d /J "%GUCEF_THEBATCHDIR%\src\common\include" "%GUCEF_THEBATCHDIR%\..\..\common\include"
cmd /c mklink "%GUCEF_THEBATCHDIR%\src\gucef.h" "%GUCEF_THEBATCHDIR%\..\..\common\include\gucef.h"
cmd /c mklink /d /J "%GUCEF_THEBATCHDIR%\src\gucefMT" "%GUCEF_THEBATCHDIR%\..\gucefMT"
cmd /c mklink /d /J "%GUCEF_THEBATCHDIR%\src\gucefCORE" "%GUCEF_THEBATCHDIR%\..\gucefCORE"
echo ---