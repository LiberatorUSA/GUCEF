CALL CMakeCommon.bat

ECHO *** Set MVC12 specifics and run CMake ***

SET MVC12_OUTPUTDIR="%OUTPUTDIR%\MVC12"
CMake.exe -DBUILD_SHARED_LIBS=ON -G"Visual Studio 12" -H"%SRCROOTDIR%" -B%MVC12_OUTPUTDIR%
PAUSE