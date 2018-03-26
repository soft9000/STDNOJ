/* The MIT License (Open Source Approved)

Copyright (c) 1999 - 2009 R. A. Nagy 

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
// Class DosMatch: Expand DOS-type wildcard characters to match patterns in strings.
// NOTE: The name might say `DOS`, but the implementation is PLATFORM INDEPENDANT!!
// ====  Use where ever C++ is sold!!
//
// Class created 12/18/1999, R. Nagy
// 12/27/1999: Added test cases to intercept a few bugs that came up in EzSearch, R. Nagy.
//             Dumped the silly "Bool" usage, as it broke Borland (sigh).
// 08/09/2005: Ported over to stdnoj, R. Nagy
//
#ifndef DosMatch_Hpp
#define DosMatch_Hpp


#include <stdnoj.hpp>
using namespace stdnoj;


// Note: ?  matches any SINGLE character ('?' == '1', but '?' != '123') except NULL: '?' != ""
//       *  matches any pattern                                         except NULL: '*' != ""
//
// Match chars and patterns can be arbitrarily combined to create masks, eg:
//       "*.EXE", "T?.*", "F*.*", "D*.??M", (etc.): (See test program for more information).
//
// See the test cases for a good demonstration of what this class is capeable of doing for you.
//

class DosMatch
{
protected:
   size_t      MatchMaskChars(const StdString& sString, const StdString& sMask);
   const char *MaskPortion(const StdString& sString, StdString& sMask);
public:
   bool        Match(const StdString& sPattern, const StdString& sString);
   bool        HasWildcard(const StdString&);
};

#endif
