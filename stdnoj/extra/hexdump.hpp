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
//
// 11/30/2003: Changed hexdump to stop showing the NULL terminator, R. Nagy
//

#ifndef _hex_dumper1_
#define _hex_dumper1_

#include <stdnoj.hpp>

using namespace stdnoj;

inline
void hexdump(const StdString& str, ostream& os)
   {
   size_t which, ss2, ss1;
   which = ss2 = ss1 = NULL;
   char *pbuf = new char[100];
   for(ss1 = NULL; (ss1 * 16) < str.length(); ss1++)
      {
      ::sprintf(pbuf, "%04d: ", ss1 * 16);
      os << pbuf;
      for(ss2 = NULL; ss2 < 16; ss2++)
         {
         which = (ss1 * 16) + ss2;
         if(which <= str.length())
            ::sprintf(pbuf, " %02x", str[which]);
         else
            ::strcpy(pbuf, "   ");
         os << pbuf;
         }
      ::strcpy(pbuf, ":   ");
      os << pbuf;
      for(ss2 = NULL; ss2 < 16; ss2++)
         {
         which = (ss1 * 16) + ss2;
         if(which <= str.length())
            {
            byte_t bb = str[which];
            if((bb >= ' ') && (bb <= '~'))
               ::sprintf(pbuf, "%c", bb);
            else
               ::strcpy(pbuf, ".");
            }
         else
            ::strcpy(pbuf, " ");
         os << pbuf;
         }
      os << endl;
      }
   delete [] pbuf;
   }

inline
const char *hexdump(const StdString& str, StdString& sResult)
   {
   stringstream srm;
   hexdump(str, srm);
   sResult = PRESTO(srm);
   return sResult.c_str();
   }

#endif
