/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2009 R. A. Nagy 

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to 
deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
sell copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions: 

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software. 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
IN THE SOFTWARE. 

*/
#include "Linear.hpp"

using namespace FileConversion;


StdString Linear::readCooked(istream& is)
{
StdString sLine;
sLine.readline(is);
if(is)
   {
   size_t ssLast = sLine.length()-1;
   if(sLine[ssLast] == '\r')
      sLine[ssLast] = 0;
   }
return sLine;
}


bool TabbedLine::readline(istream& is, Array<StdString>& aResult)
{
bool br = false;
aResult.Empty();
StdString sLine = readCooked(is);
if(is)
   {
   br = true;
   aResult = sLine.slice('\t');
   }
else
   sLastError = "readline: no line read.";
return br;
}
bool TabbedLine::writeline(ostream& os, Array<StdString>& aResult)
{
if(aResult.Nelem())
   {
   sLastError = "writeline: Input array is empty.";
   return false;
   }
size_t ssLast = aResult.Nelem() - 1;
for(size_t ss = 0L; ss < aResult.Nelem(); ss++)
   {
   os << aResult[ss];
   if(ss != ssLast)
      os << "\t";
   }
os << endl;
return true;
}


bool CsvLine::readline(istream& is, Array<StdString>& aResult)
{
bool br = false;
aResult.Empty();
StdString sLine = readCooked(is);
if(is)
   {
   // Archaic usage would have numbers seperated by a comma, and strings quoted.
   // Observed modern covention seems to just quote them all. If your  mileage
   // varies, then feel free to use the <extra/CsvParser.hpp> class, instead.
   br = true;
   // VERY RARE that this would EVER cause a problem
   sLine.replace("\",\"", "\t");  
   aResult = sLine.slice('\t');
   }
else
   sLastError = "readline: no line read.";
return br;
}
bool CsvLine::writeline(ostream& os, Array<StdString>& aResult)
{
if(aResult.Nelem())
   {
   sLastError = "writeline: Input array is empty.";
   return false;
   }
size_t ssLast = aResult.Nelem() - 1;
for(size_t ss = 0L; ss < aResult.Nelem(); ss++)
   {
   os << aResult[ss];
   if(ss != ssLast)
      os << "\",\"";
   }
os << endl;
return true;
}


bool SdfLine::readline(istream& is, Array<StdString>& aResult)
{
sLastError = "work in progress";
return false;
}
bool SdfLine::writeline(ostream& os, Array<StdString>& aResult)
{
sLastError = "work in progress";
return false;
}
