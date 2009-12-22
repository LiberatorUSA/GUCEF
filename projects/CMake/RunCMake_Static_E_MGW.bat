CALL CMakeCommon.bat

ECHO *** Set CB MGW specifics and run CMake ***

SET E_OUTPUTDIR="%OUTPUTDIR%\E_MGW"
CMake.exe -DBUILD_SHARED_LIBS=ON -G"Eclipse CDT4 - MinGW Makefiles" -H"%SRCROOTDIR%" -B%E_OUTPUTDIR%
PAUSE