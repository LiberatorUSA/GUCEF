CALL CMakeCommon.bat

ECHO *** Set CB NIX specifics and run CMake ***

SET CB_OUTPUTDIR="%OUTPUTDIR%\CB_NIX"
CMake.exe -DBUILD_SHARED_LIBS=ON -G"CodeBlocks - Unix Makefiles" -H"%SRCROOTDIR%" -B%CB_OUTPUTDIR%
PAUSE