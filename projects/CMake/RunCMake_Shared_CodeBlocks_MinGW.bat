CALL CMakeCommon.bat

ECHO *** Set CB MGW specifics and run CMake ***

SET CB_OUTPUTDIR="%OUTPUTDIR%\CBMGW"
CMake.exe -DBUILD_SHARED_LIBS=ON -G"CodeBlocks - MinGW Makefiles" -H"%SRCROOTDIR%" -B%CB_OUTPUTDIR%
PAUSE