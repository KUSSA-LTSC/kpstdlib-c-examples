@echo off

chcp 65001

call msvc.bat

echo === 汇编 ===
nasm -f win64 launcher.asm -o launcher.obj
if errorlevel 1 goto :fail

nasm -f win64 kpstdlib.asm -o kpstdlib.obj
if errorlevel 1 goto :fail

echo === 编译 C ===
cl /c utfc.c /Fo:utfc.obj /utf-8 /GS- /Gs9999999 /W4
if errorlevel 1 goto :fail

echo === 链接 ===
link /OUT:utfc.exe launcher.obj utfc.obj kpstdlib.obj kernel32.lib user32.lib /SUBSYSTEM:WINDOWS /ENTRY:duck /NODEFAULTLIB
if errorlevel 1 goto :fail

echo === 成功 ===
pause
goto :eof

:fail
echo === 失败 ===
pause
exit /b 1