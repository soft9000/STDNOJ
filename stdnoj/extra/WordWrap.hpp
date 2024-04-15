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
// TextFormatting: A place for simple text formatting tools.
//
// 08/10/2003: WordWrap Class Created. Competes with SimpleTextFlow (not part of stdnoj), Randall Nagy
// 01/08/2005: Ported WordWrap over to stdnoj::TextFormatting, Randall Nagy
//    Members renamed. Added a more conveniant interface.
//    Re-worked internal normalization functions for public re-use.
// 01/08/2005: Created the Words Class, Randall Nagy
// 01/08/2005: Created the LinearMarkup Class, Randall Nagy
// 12/26/2005: Split classes out - One class per file / header set, Randall Nagy
//    Added a few new Wrapping functions, Randall Nagy
// 05/01/2006: Changed a few legacy int-s to size_t-s, Randall Nagy
//
//
#ifndef noj_word_wrap_hpp
#define noj_word_wrap_hpp

#include <stdnoj.hpp>

namespace TextFormatting
{
using namespace stdnoj;


class WordWrap
{
private:
   // This funciton demands a simple (unix) formatted string.
   static StdString  WrapNormalized(const StdString& str, int cx = 0, int margin = 0, int maxLen = 55);

public:
   enum TextType
      {
      tt_dos,
      tt_unix,
      tt_other
      };

   // This function accepts text with any type of ASCII newline termination - DOS, UNIX, or Mac
   static Array<StdString> Wrap(const StdString& str, int cx = 0, int margin = 0, int maxLen = 55);

   // This functions takes anything - returns the format that you request -
   static StdString  Wrap(const StdString& str, TextType tt, int cx = 0, int margin = 0, int maxLen = 55);

   // This functions takes anything - returns SimpleString format -
   static StdString  WrapUnix(const StdString& str, int cx = 0, int margin = 0, int maxLen = 55);

   // This functions takes anything - returns DosString format -
   static StdString  WrapDos(const StdString& str, int cx = 0, int margin = 0, int maxLen = 55);

   // Convert conflicting newline conventions to a simple (unix normal) format. Returns the encoded pattern found.
   static TextType  AsSimpleString(StdString& str);
   static StdString AsSimpleString(const StdString& str)
      {
      StdString sResult = str;
      AsSimpleString(sResult);
      return sResult;
      }

   // Convert normalized (unix) newlines to Windows/DOS (\r\n) format
   static void      AsDosString(StdString& str);
   static StdString AsDosString(const StdString& str)
      {
      StdString sResult = str;
      AsDosString(sResult);
      return sResult;
      }

};


}

#endif
