CALL CMakeCommon.bat

ECHO *** Set VS2019 specifics and run CMake ***

SET MAINCMAKE=%SRCROOTDIR%\projects\CMake\targets\GUCEF_exe_gucefVFS_TestApp
SET MVC14_OUTPUTDIR="%OUTPUTDIR%\MVC14_gucefVFS_TestApp"

CMake.exe -DBUILD_SHARED_LIBS=ON -G"Visual Studio 14 2015" -A x64 -H"%MAINCMAKE%" -B%MVC14_OUTPUTDIR%
PAUSE
