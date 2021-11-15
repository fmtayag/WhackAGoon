@echo off

SET INCLUDE=C:\Dev\allSDL_x86\include
SET LIB=C:\Dev\allSDL_x86\lib
SET OBJDIR=obj\
SET OUTFILE=whackagoon-rew.exe
SET BUILDDIR=build/
SET OUTPATH=%BUILDDIR%%OUTFILE%

for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do     rem"') do (
  set "DEL=%%a"
)

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

:: Compile
g++ %OBJDIR%*.o -o %OUTPATH% -L%LIB% -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -Wall -Wextra -pedantic

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
