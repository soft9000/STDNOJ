
echo "Creating stdnoj.a ..."

cd ../../../../stdnoj/core
rm -f *.err > /dev/null
rm -f *.o > /dev/null

for i in *.cpp; do g++ -Xpreprocessor -DUNIX -I.. -c $i 2> $i.err; done;

echo Reporting ...
rm -f gcc_report.txt
echo "Compilation Errors Reported  - none if empty" > gcc_report.txt
echo "===========================================" >> gcc_report.txt
for i in *.err; do grep " error: " $i >> gcc_report.txt; done;
cat gcc_report.txt
rm -f *.err


echo "Linking stdnoj.a ..."
for i in *.o; do ar -rc stdnoj.a $i; done;
mkdir ../lib
mkdir ../lib/GCC_WIN32
mv -f *.a ../lib/GCC_WIN32
rm -f *.o
cd ../lib/GCC_WIN32
if test -e "stdnoj.a"; then echo "Success."; else echo "bummer!"; fi;


