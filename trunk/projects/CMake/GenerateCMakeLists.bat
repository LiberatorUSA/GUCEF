SET THEBATCHDIR=%CD%

cd..
cd..
cd common
cd bin
cd Debug_MVC9

SET GENERATORPATH=%CD%
SET PATH=%GENERATORPATH%;%PATH%

cd..
cd..
cd..

CMakeListGenerator_d.exe

cd %THEBATCHDIR%

IF NOT DEFINED NOPAUSE (
  PAUSE
)