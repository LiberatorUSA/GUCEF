CALL CMakeCommon.bat

ECHO *** Set VS2015 specifics and run CMake ***

SET MAINCMAKE=%SRCROOTDIR%\projects\CMake\targets\GUCEF_exe_pubsub2pubsub
SET VS19_OUTPUTDIR="%OUTPUTDIR%\MVC14_x64_pubsub2pubsub"

CMake.exe -DBUILD_SHARED_LIBS=ON -G"Visual Studio 14 2015" -A x64 -H"%MAINCMAKE%" -B%VS19_OUTPUTDIR%
PAUSE