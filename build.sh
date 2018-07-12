cd nas

echo assemble...

for f in *.nas; do ./dasm $f -f3 -o"${f%.nas}.bin"; done

echo build...

mv hdr.bin !rc3.hdr
mv chr.bin !rc3.chr

cat bank0.bin bank1.bin bank2.bin bank3.bin bank4.bin bank5.bin bank6.bin bank7.bin > !rc3.prg
cat !rc3.hdr !rc3.prg !rc3.chr > !rc3.nes

echo cleanup...

rm *.bin
rm !rc3.hdr !rc3.prg !rc3.chr

cp !rc3.nes ../!rc3.nes

echo done.
