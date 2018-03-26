
echo "Creating libnoj.a ..."

cd ../../../../stdnoj/noj
rm -f *.err > /dev/null
rm -f *.o > /dev/null

for i in *.cpp; do g++ -Xpreprocessor -DUNIX -I.. -c $i 2> $i.err; done;
for i in *.Cpp; do g++ -Xpreprocessor -DUNIX -I.. -c $i 2> $i.err; done;

echo Reporting ...
rm -f osx_report.txt
echo "Compilation Errors Reported  - none if empty" > osx_report.txt
echo "===========================================" >> osx_report.txt
for i in *.err; do grep " error: " $i >> osx_report.txt; done;
cat osx_report.txt | more
rm -f *.err


echo "Linking libnoj.a ..."
for i in *.o; do ar -rc libnoj.a $i; done;
mkdir ../lib
mkdir ../lib/OSX
mv *.a ../lib/OSX
rm -f *.o
cd ../lib/OSX
if test -e "libnoj.a"; then echo "Success."; else echo "bummer!"; fi;


