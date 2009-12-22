CALL CMakeCommon.bat

ECHO *** Set MVC8 specifics and run CMake ***

SET MVC8_OUTPUTDIR="%OUTPUTDIR%\MVC8"
CMake.exe -DBUILD_SHARED_LIBS=OFF -G"Visual Studio 8 2005" -H"%SRCROOTDIR%" -B%MVC8_OUTPUTDIR%
PAUSE