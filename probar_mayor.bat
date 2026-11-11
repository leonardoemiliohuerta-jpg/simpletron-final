@echo off
rem Compila y corre el programa de ejemplo mayor.simple (Windows). Requiere haber compilado antes con probar.bat
if not exist simple.exe call probar.bat <nul >nul 2>&1
echo ^> simple --compile programas\simple\mayor.simple programas\sml\mayor.sml
simple.exe --compile programas\simple\mayor.simple programas\sml\mayor.sml
echo.
echo ^> simple --run programas\sml\mayor.sml   (escribe dos numeros)
simple.exe --run programas\sml\mayor.sml
