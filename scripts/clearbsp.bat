@echo off
cd /d %~dp0/..
if exist "*.uvguix.*"   del /a /f /q *.uvguix.*
if exist "rtconfig.pyc" del /a /f /q rtconfig.pyc
if exist "rtthread.map" del /a /f /q rtthread.map
if exist "rtthread.elf" del /a /f /q rtthread.elf
if exist ".sconsign.dblite" del /a /f /q .sconsign.dblite
if exist "project.uvoptx" del /a /f /q project.uvoptx
if exist "project.uvprojx" del /a /f /q project.uvprojx
if exist "cconfig.h" del /a /f /q cconfig.h
if exist "DebugConfig"  rmdir /S /Q DebugConfig
if exist "build"  rmdir /S /Q build
if exist ".vscode"  rmdir /S /Q .vscode
if exist "JLinkLog.txt" del /a /f /q JLinkLog.txt
if exist "JLinkSettings.ini" del /a /f /q JLinkSettings.ini
if exist ".config.old" del /a /f /q .config.old
if exist "EventRecorderStub.scvd" del /a /f /q EventRecorderStub.scvd
exit
