@echo off
setlocal

set install_folder=%~dp0\cmake\conan\%1
conan install -if %install_folder% -s build_type=%1 .