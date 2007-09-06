SET PATH=%DOXYGEN%;%PATH%
SET DOXFILEDIR=%GUCEF_HOME%\docs\doxygen
cd..
cd..
doxygen.exe "%DOXFILEDIR%\GUCEF_API.dox"
cd %DOXFILEDIR%