@echo off
rem Compila el proyecto y corre el programa de ejemplo suma.simple (Windows)
set "PATH=%PATH%;C:\Program Files (x86)\Embarcadero\Dev-Cpp\TDM-GCC-64\bin"
echo ^> gcc src\simpletron.c -o simple.exe -lm
gcc src\simpletron.c -o simple.exe -lm
if errorlevel 1 (
  echo No se pudo compilar: revisa que gcc este instalado.
  exit /b 1
)
echo Compilado sin errores.
echo.
echo ^> simple --compile programas\simple\suma.simple programas\sml\suma.sml
simple.exe --compile programas\simple\suma.simple programas\sml\suma.sml
echo.
echo ^> simple --run programas\sml\suma.sml   (escribe dos numeros)
simple.exe --run programas\sml\suma.sml
