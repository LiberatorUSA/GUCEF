CALL CMakeCommon.bat

ECHO *** Set VS2019 specifics and run CMake ***

SET MAINCMAKE=%SRCROOTDIR%\projects\CMake\targets\GUCEF_exe_awslambdahost
SET VS19_OUTPUTDIR="%OUTPUTDIR%\VS2019_x64_awslambdahost"

CMake.exe -DBUILD_SHARED_LIBS=ON -G"Visual Studio 16 2019" -A x64 -H"%MAINCMAKE%" -B%VS19_OUTPUTDIR%
PAUSE