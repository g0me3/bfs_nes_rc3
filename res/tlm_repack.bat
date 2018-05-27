for %%f in (tlm*.unp) do rc3enc.exe %%f %%~nf.pak
copy /b tlm*.pak ..\tlm\tlm*.pak
del tlm*.pak