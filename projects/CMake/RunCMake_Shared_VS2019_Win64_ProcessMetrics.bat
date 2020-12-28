CALL CMakeCommon.bat

ECHO *** Set Visual Studio specifics and run CMake ***

SET MAINCMAKE=%SRCROOTDIR%\projects\CMake\targets\GUCEF_exe_ProcessMetrics
SET VS19_OUTPUTDIR="%OUTPUTDIR%\VS2019_x64_ProcessMetrics"

CMake.exe -DBUILD_SHARED_LIBS=ON -G"Visual Studio 16 2019" -A x64 -H"%MAINCMAKE%" -B%VS19_OUTPUTDIR%
PAUSE