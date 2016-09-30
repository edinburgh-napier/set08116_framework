echo off
where /q cmake
IF %ERRORLEVEL%==1 (
    Echo cmake not in path
	SET p86=C:\Program Files (x86)
	PATH=%path%;%ProgramFiles(x86)%\CMake\bin
	PATH=%path%;%ProgramFiles%\\CMake\bin
 )
where /q cmake
IF %ERRORLEVEL%==1 (
    Echo Couldn't find cmake.exe
) ELSE ( 
   echo Cmake building for VS2015 Win64
   cd .. 
	mkdir gfx_build
	cd gfx_build
   cmake -G "Visual Studio 14 2015 Win64" ../enu_graphics_framework
   echo cmake complete, open the sln in the build dir
 )
pause > nul