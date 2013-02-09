CALL Premake4Common.bat

ECHO *** Set MVC10 specifics and run Pemake 4 ***

SET MVC10_OUTPUTDIR="%OUTPUTDIR%\MVC10"
premake4.exe -DBUILD_SHARED_LIBS=ON -G"Visual Studio 10" -H"%SRCROOTDIR%" -B%MVC10_OUTPUTDIR%
PAUSE