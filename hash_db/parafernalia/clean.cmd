@echo off
setlocal
del *.obj>NUL
del *.pdb>NUL
del *.ilk>NUL
del *.exp>NUL

if [%1] == [] goto exit

del *.exe>NUL
del *.lib>NUL
del *.dll>NUL

:exit