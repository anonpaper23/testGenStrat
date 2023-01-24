@echo off

call "setup_mssdk71.bat"

cd .

if "%1"=="" (nmake -f Altitude_Mode.mk all) else (nmake -f Altitude_Mode.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
An_error_occurred_during_the_call_to_make
