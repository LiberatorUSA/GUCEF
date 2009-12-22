CALL CMakeCommon.bat

ECHO *** Set CB NIX specifics and run CMake ***

SET E_OUTPUTDIR="%OUTPUTDIR%\E_NIX"
CMake.exe -DBUILD_SHARED_LIBS=ON -G"Eclipse CDT4 - Unix Makefiles" -H"%SRCROOTDIR%" -B%E_OUTPUTDIR%
PAUSE