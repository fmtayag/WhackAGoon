@echo off

SET INCLUDE=C:\Dev\allSDL_x86\include
SET LIB=C:\Dev\allSDL_x86\lib
SET OBJDIR=obj\
SET OUTFILE=whackagoon-rew.exe
SET BUILDDIR=build/
SET OUTPATH=%BUILDDIR%%OUTFILE%

if not exist %OBJDIR% (
	mkdir %OBJDIR%
)

if not exist %BUILDDIR% (
	mkdir %BUILDDIR%
)

@echo on
:: Link
g++ main.cpp -c -o %OBJDIR%main.o -I%INCLUDE%
g++ scenes.cpp -c -o %OBJDIR%scenes.o -I%INCLUDE%
g++ widgets.cpp -c -o %OBJDIR%widgets.o -I%INCLUDE%
g++ sprites.cpp -c -o %OBJDIR%sprites.o -I%INCLUDE%
g++ helpers.cpp -c -o %OBJDIR%helpers.o -I%INCLUDE%
g++ g_texture.cpp -c -o %OBJDIR%g_texture.o -I%INCLUDE%

:: Compile
g++ %OBJDIR%*.o -o %OUTPATH% -L%LIB% -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

@echo off

if %errorlevel% == 0 (
	call :colorEcho a0 "Compilation is a success."
	echo:

	:: run file
	cd %BUILDDIR%
	%OUTFILE%
) else (
	call :colorEcho 40 "Compilation has failed."
	echo:
)

pause
exit

:: for coloring echos
:colorEcho
echo off
<nul set /p ".=%DEL%" > "%~2"
findstr /v /a:%1 /R "^$" "%~2" nul
del "%~2" > nul 2>&1i
