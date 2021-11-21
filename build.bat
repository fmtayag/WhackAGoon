@echo off

SET PROJECT=WhackAGoon
SET ROOT=%cd%
SET DEBUGDIR=%cd%/build/debug

for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do     rem"') do (
  set "DEL=%%a"
)

cd build
msbuild %PROJECT%.sln -property:Configuration=Debug

if %errorlevel% == 0 (
    call :colorEcho a0 "Compilation is a success."
	echo:

    call :copyAssetsAndLibs

    cd %DEBUGDIR%
    %PROJECT%.exe  
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

:copyAssetsAndLibs
cd %ROOT%
xcopy /e /v /Y assets build\Debug\assets
xcopy /e /v /Y libraries build\Debug