CALL CMakeCommon.bat

ECHO *** Set MVC14 specifics and run CMake ***

SET MAINCMAKE=%SRCROOTDIR%\projects\CMake\targets\GUCEF_[tag]_platform
SET MVC14_OUTPUTDIR="%OUTPUTDIR%\MVC14_Platform"

CMake.exe -DBUILD_SHARED_LIBS=ON -G"Visual Studio 14 2015" -H"%MAINCMAKE%" -B%MVC14_OUTPUTDIR%
PAUSE