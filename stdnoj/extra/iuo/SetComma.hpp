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
// Class SetComma: Scavanged from my old pvPrompter class, this stand-alone
// class is now available for general use.
//
// 06/18/2001: Class created, Randall Nagy
// 07/15/2003: Moved under stdnoj, Randall Nagy
//             Got rid of stringstream usage due to bugs in same under threaded operations.
// 09/06/2003: Changed usage to static, renamed class. Randall Nagy
//
#ifndef SetComma_Hpp
#define SetComma_Hpp

#include <stdnoj.hpp>

using namespace stdnoj;

class _SetComma
{
private:
   static void _Format(char *buf);

protected:
   static void _Format(StdString& str);

public:
   static const char *Format(long unsigned uldata, StdString& buf);
   static StdString   Format(long unsigned uldata);
};

inline const char *CommaFormat(long unsigned ss, StdString& str)  // 06/04/2002
   {
   _SetComma::Format(ss, str);
   }

inline
void _SetComma::_Format(StdString& str)
   {
   char *psz = new char[str.length() * 2];   // massive room for comma-growth (len%3 + 1 ain't worth the cpu)
   strcpy(psz, str.c_str());
   _Format(psz);
   str = psz;
   delete [] psz;
   }

inline
void _SetComma::_Format(char *buf)
   {
   char hold[30], *pc;
   int i = 0;

   pc = buf;

   ::strrev(pc);

   strcpy(&hold[0], buf);

   while(hold[i])
      {
      if ((i) && (!(i % 3)))
         {
         *buf = ',';
         buf++;
         }
      *buf = hold[i];
      buf++;
      i++;
      }
   *buf = NULL;

   ::strrev(pc);
   } // _Format()

inline
const char *_SetComma::Format(long unsigned uldata, StdString& str)
   {
   char buf[25];
   ::ltoa(uldata, buf, 10);
   str = buf;
   _Format(str);
   return str.c_str();
   } // Format()

inline
StdString _SetComma::Format(long unsigned uldata)
   {
   StdString str;
   Format(uldata, str);
   return str;
   } // Format()

inline
StdString SetComma(long unsigned lu)
{
return (_SetComma::Format(lu));
}

inline
StdString SetZerosLeft(long unsigned lu, int len = 6)
{
StdString sPat = "%0";
if(len < 98)
   {
   char buf[100];
   StdString s2;
   s2.assign(len);
   sPat.append(s2);
   sPat.append("d");
   sprintf(&buf[0], sPat.c_str(), lu);
   sPat = buf;
   }
return sPat;
}
#endif

