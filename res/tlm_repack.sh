for f in *.unp; do ./rc3enc "$f" "${f%.unp}.pak"; done
for f in *.pak; do cp "$f" ../tlm/"$f"; done
rm *.pak
