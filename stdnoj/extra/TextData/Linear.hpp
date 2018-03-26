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

#ifndef Linear_hpx
#define Linear_hpx

#include <stdnoj.hpp>
using namespace stdnoj;

// A place to put some handy textual (mostly 7-bit) file input and output 
// routines.
//
// (Because Cookpedia.com and TheQuoteforToday exchange a lot of data, I
// thought that we would include a lot of our more "designed" efforts here.)
//
// These classes are great for importing / exporting exported excel spreadsheet 
// data and / or data from other user-editable spreadsheet / database programs.
//
// Keywords: C++, cpp, portable, ANSI, template, patterns, abstract, concrete, X3J11, XJ311, POSIX, WIN32, DOS, Linux, Microsoft Windows, example, examples, sample, samples, TextReader, TextWriter
//

namespace FileConversion
{
enum LinearType
   {
   ltCsv,            // Comma Seperated Variables (CSV)
   ltSdf,            // Space Delimited Format (SDF)
   ltTab,            // 'Ye olde TAB (or any other odd char) -delimited format (a personal favorite :)
   ltNone            // It happends in the best classes :->
   };

class Linear
   {
   protected:
      StdString readCooked(istream& is);
   public:
      StdString sLastError;

      virtual bool readline(istream& is, Array<StdString>& aResult) = 0;
      virtual bool writeline(ostream& os, Array<StdString>& aResult) = 0;
   };

class TabbedLine : public Linear
   {
   private:
      char chDelim;
   public:
      TabbedLine(const char cDelim = '\t') : chDelim(chDelim)
         {
         }
      bool readline(istream& is, Array<StdString>& aResult);
      bool writeline(ostream& os, Array<StdString>& aResult);
   };

class CsvLine : public Linear
   {
   public:
      bool readline(istream& is, Array<StdString>& aResult);
      bool writeline(ostream& os, Array<StdString>& aResult);
   };

class SdfLine : public Linear
   {
   private:
      SdfLine() {}
   protected:
      Array<int> aCols;
   public:
      SdfLine(const Array<int>& aPos)
         {
         aCols = aPos;
         }
      bool readline(istream& is, Array<StdString>& aResult);
      bool writeline(ostream& os, Array<StdString>& aResult);
   };
}

#endif
