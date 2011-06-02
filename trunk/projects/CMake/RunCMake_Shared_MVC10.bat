CALL CMakeCommon.bat

ECHO *** Set MVC10 specifics and run CMake ***

SET MVC10_OUTPUTDIR="%OUTPUTDIR%\MVC10"
CMake.exe -DBUILD_SHARED_LIBS=ON -G"Visual Studio 10" -H"%SRCROOTDIR%" -B%MVC10_OUTPUTDIR%
PAUSE