@echo off
setlocal

set "SOURCE=%~dp0..\opengl"
set "DESTINATION=%~1"

if "%DESTINATION%"=="" (
    echo Usage:
    echo   %~nx0 "C:\path\to\destination"
    exit /b 1
)

if not exist "%SOURCE%" (
    echo Source directory not found:
    echo   "%SOURCE%"
    exit /b 1
)

echo Copying OpenGL shaders...
echo From: "%SOURCE%"
echo To:   "%DESTINATION%"

robocopy "%SOURCE%" "%DESTINATION%" *.glsl /E /IS /IT

if %ERRORLEVEL% GEQ 8 (
    echo Shader copy failed.
    exit /b %ERRORLEVEL%
)

echo Shader copy completed successfully.
exit /b 0