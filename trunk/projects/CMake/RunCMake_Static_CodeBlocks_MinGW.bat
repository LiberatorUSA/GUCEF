CALL CMakeCommon.bat

ECHO *** Set CB MGW specifics and run CMake ***

SET CB_OUTPUTDIR="%OUTPUTDIR%\CB_MGW"
CMake.exe -DBUILD_SHARED_LIBS=OFF -G"CodeBlocks - MinGW Makefiles" -H"%SRCROOTDIR%" -B%CB_OUTPUTDIR%
PAUSE