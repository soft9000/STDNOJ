/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2024 R. A. Nagy 

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
// 12/20/1998: Created class for QUOTEZ scripture data conversion, R. Nagy
// 01/06/2000: Added stand-alone usage of GetLine(), R. Nagy
//
#ifndef Csv_Hpp
#define Csv_Hpp

#include <stdnoj.hpp>
using namespace stdnoj;

enum CsvDataType
   {
   StringsOnly,
   NumericOnly,
   Both
   };

class CsvParser
   {
   private:
      void NORMALIZE_COMMAS(StdString& std, size_t sz_start = NULL);

      void PUSH_LITERAL_QUOTES(StdString& std, size_t sz_start = NULL);
      void  POP_LITERAL_QUOTES(StdString& std);

      void PUSH_LITERAL_COMMAS(StdString& std, size_t sz_start = NULL);
      void  POP_LITERAL_COMMAS(StdString& std);

   protected:
      istream *pIfs;

      void     ConvertToTight(StdString& result);

      istream& GetField(istream& in, StdString& result);
      istream& ImportCsvData(istream& in, StdString& result);

   public:
      CsvParser(void);
      virtual ~CsvParser(void);

      // usage one;
      bool   Open(const char *psz);
      size_t GetLine(Array<StdString>& result, CsvDataType = Both);
      // Stand-alone usage;
      size_t GetLine(istream& ifs, Array<StdString>& result, CsvDataType = Both);
      size_t GetLine(const StdString& sInput, Array<StdString>& result, CsvDataType = Both);

      // This usage assumes that the line ONLY contains strings delimited
      // by quotations;
      size_t GetLineStrings(const StdString&,Array<StdString>& result);
      size_t GetLineStrings(Array<StdString>& result);
      size_t GetLineStrings(istream& inf, Array<StdString>& result);
   };

#endif
