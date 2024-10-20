@echo off
echo Compiling ...
cl /EHsc TokenApp.cpp

if %errorlevel% neq 0 (
    echo Compile failed.
    exit /b %errorlevel%
)
