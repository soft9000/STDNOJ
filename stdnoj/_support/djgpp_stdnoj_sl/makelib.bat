@echo off
@echo Creating libnoj.a ...
@echo ...

cd ..\..\stdnoj\noj
del *.err 2> NUL
del *.o 2> NUL
rem -fpermissive
for %%i in (*.cpp) do gpp -Xpreprocessor -DMSDOS -I.. -Ic:/DJGPP/incude -c %%i 2> %%i.err

REM Reporting ...
del dj_report.txt
echo Compilation Errors Reported (none if empty) > dj_report.txt
echo =========================================== >> dj_report.txt
for %%i in (*.err) do grep " error: " %%i >> dj_report.txt
start notepad dj_report.txt
del *.err

@echo Linking libnoj.a ...
for %%i in (*.o) do ar -rc libnoj.a %%i
md ..\lib
md ..\lib\dj
move *.a ..\lib\dj
del *.o
cd ..\lib\dj
if exist libnoj.a echo "Success."