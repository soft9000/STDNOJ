/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2024 Randall Nagy 

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

#ifndef TextReader_hpx
#define TextReader_hpx

#include <stdnoj.hpp>
using namespace stdnoj;

// A place to put some handy textual (mostly 7-bit) file input and output 
// routines.
//
// (Because Cookpedia.com and TheQuoteforToday exchange a lot of data, I
// thought that we would include a lot of our more "designed" efforts here.)
//
// Keywords: C++, cpp, portable, ANSI, template, patterns, abstract, concrete, X3J11, XJ311, POSIX, WIN32, DOS, Linux, Microsoft Windows, example, examples, sample, samples
//
#include "Linear.hpp"

class FileConversion::Linear;

namespace FileConversion
{

class TextReader {
   private:
      FileConversion::Linear *pReader;

   public:
      File      pwFile;
      StdString sLastError;

      TextReader() : pReader(0L)
         {
         }
      TextReader(File& file) : pwFile(file), pReader(0L)
         {
         }

      ~TextReader()
         {
         delete pReader;
         }

      bool ReadFirst(Array<StdString>& aResult, LinearType type);
      bool ReadNext(Array<StdString>& aResult);

      static bool Recall(size_t zbRecordId, const File& file, Array<StdString>& aResult, LinearType type = ltTab);
};

}

#endif
