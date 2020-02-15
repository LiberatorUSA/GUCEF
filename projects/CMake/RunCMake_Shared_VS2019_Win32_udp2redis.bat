CALL CMakeCommon.bat

ECHO *** Set MVC14 specifics and run CMake ***

SET MAINCMAKE=%SRCROOTDIR%\projects\CMake\targets\GUCEF_[exe]_udp2redis
SET MVC14_OUTPUTDIR="%OUTPUTDIR%\VS2019_udp2redis"

CMake.exe -DBUILD_SHARED_LIBS=ON -G"Visual Studio 16 2019" -A Win32 -H"%MAINCMAKE%" -B%MVC14_OUTPUTDIR%
PAUSE