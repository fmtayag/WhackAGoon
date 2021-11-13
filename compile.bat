:: note: add -mwindows to remove console output

@echo off

SET BUILDDIR=build\
SET OBJDIR=source\obj\
SET OUTFILE=whackagoon-vdebug.exe
SET OUTPATH=%BUILDDIR%%OUTFILE%
SET INCLUDE=C:\Dev\allSDL_x86\include
SET LIB=C:\Dev\allSDL_x86\lib
SET OBJFILE=source\whackagoon.o

for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do     rem"') do (
  set "DEL=%%a"
)
if not exist %OBJDIR% (
	mkdir %OBJDIR%
)

@echo on
:: Compile cpp files
g++ source\main.cpp -c -o %OBJDIR%main.o -I%INCLUDE% 
g++ source\sprites.cpp -c -o %OBJDIR%sprites.o -I%INCLUDE%
g++ source\scenes.cpp -c -o %OBJDIR%scenes.o -I%INCLUDE% 
g++ source\widgets.cpp -c -o %OBJDIR%widgets.o -I%INCLUDE% 
g++ source\utils.cpp -c -o %OBJDIR%utils.o -I%INCLUDE% 
g++ source\frames.cpp -c -o %OBJDIR%frames.o -I%INCLUDE% 

:: Link obj files and other libs
:: g++ %OBJDIR%*.o -o %OUTPATH% -L%LIB% -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -static-libgcc -static-libstdc++ -lwinpthread
:: g++ %OBJFILE% -o %OUTPATH% -L%LIB% -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
:: g++ source\*.cpp -o %OUTPATH% -I%INCLUDE% -L%LIB% -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -static -static-libgcc -static-libstdc++ 

g++ %OBJDIR%*.o -o %OUTPATH% -L%LIB% -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive

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
