@echo off

rem start "" "..\..\..\tools\TextPad\TextPad.exe" ram.inc macro.inc bank7.inc bank6.inc bank5.inc bank4.inc bank3.inc bank2.inc bank1.inc bank0.inc

pause 0

:again

del !rc3prev.nes
rename !rc3.nes !rc3prev.nes

cd nas

dasm bank0.nas -f3 -obank0.bin

echo log/lst generate...
dasm bank0.nas -f3 -l!rc3.lst -obank0.bin > !err.log

echo assemble...
for %%f in (*.nas) do call :dodasm %%f > NUL

goto :build

:dodasm
dasm %1 -f3 -o%~n1.bin
goto :eof

:build
echo build...

rename hdr.bin !rc3.hdr > NUL
rename chr.bin !rc3.chr > NUL

copy /b bank0.bin+bank1.bin+bank2.bin+bank3.bin+bank4.bin+bank5.bin+bank6.bin+bank7.bin !rc3.prg > NUL

nesimage j !rc3 > NUL

echo cleanup...

for %%f in (*.bin) do del %%f
del !rc3.prg
del !rc3.chr
del !rc3.hdr

copy /b !rc3.nes ..\!rc3.nes > NUL

del !rc3.nes

cd ..

echo ===OLD ROM===
if exist !rc3prev.nes nas\m3checksum !rc3prev.nes
echo ===NEW ROM===
if exist !rc3.nes nas\m3checksum !rc3.nes

echo done.
pause 0

goto :again

:eof
