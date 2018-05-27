@echo off

start "" "..\..\..\tools\TextPad 5\TextPad.exe" bank0.inc bank1.inc bank2.inc bank3.inc bank4.inc bank5.inc bank6.inc bank7.inc ram.inc notes.txt

pause 0

:again

del !err.log
del !rc3.nes
del !rc3.hdr

echo assembling...

dasm bank0.nas -f3 -obank0.bin
dasm bank0.nas -f3 -l!rc3.lst  -obank0.bin >> !err.log

for %%f in (*.nas) do call :dodasm %%f > NUL

dasm hdr.nas -f3 -o!rc3.hdr > NUL

goto :cleanup

:dodasm
dasm %1 -f3 -o%~n1.bin
goto :eof

:cleanup
echo build...
copy /b bank0.bin+bank1.bin+bank2.bin+bank3.bin+bank4.bin+bank5.bin+bank6.bin+bank7.bin !rc3.prg > NUL
nesimage j !rc3 > NUL

echo cleanup...
for %%f in (*.bin) do del %%f

echo done.
pause 0

goto :again

:eof
