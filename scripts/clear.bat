@echo off
cd /d %~dp0/..
if exist "*.uvguix.*"   del /a /f /q *.uvguix.*
if exist "rtconfig.pyc" del /a /f /q rtconfig.pyc
if exist "rtthread.map" del /a /f /q rtthread.map
if exist "rtthread.elf" del /a /f /q rtthread.elf
if exist "DebugConfig"  rmdir /S /Q DebugConfig
if exist "EventRecorderStub.scvd" del /a /f /q EventRecorderStub.scvd
exit
