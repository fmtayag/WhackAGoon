:: note: add -mwindows to remove console output

@echo off
cd C:\Users\Francis\Documents\_gameprojects\Whack-A-Goon\src
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do     rem"') do (
  set "DEL=%%a"
)
@echo on	

g++ internal\*.cpp main.cpp -IC:\Dev\allSDL_x86\include -LC:\Dev\allSDL_x86\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o dwhack.exe
@echo off

if EXIST dwhack.exe (
	:: run output file
	call :colorEcho a0 "Compilation is a success."
	echo:
	dwhack.exe
	
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
