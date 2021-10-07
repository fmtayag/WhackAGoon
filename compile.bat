:: note: add -mwindows to remove console output

@echo off

SET BUILDDIR=build\
SET OUTFILE=db_whack.exe
SET OUTPATH=%BUILDDIR%%OUTFILE%
SET INCLUDE=C:\Dev\allSDL_x86\include
SET LIB=C:\Dev\allSDL_x86\lib

for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do     rem"') do (
  set "DEL=%%a"
)
@echo on	

g++ source\*.cpp -I%INCLUDE% -L%LIB% -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o %OUTPATH% 
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
