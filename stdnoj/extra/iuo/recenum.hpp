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
// Created for DOC and NUMBERS: A FASTER way to get at the records, without
// storage. Great for when number of records is large, and you are "goal seeking"
// a record / set / dump.
//
// 06/23/1997, Class Created (bare essentials!), R. Nagy.
// 05/22/1998, Now using "StdString" rather than "StdString", R. Nagy.
//    Created BinRecEnum, R. Nagy.
//
#ifndef RecEnum_Hpp
#define RecEnum_Hpp

#ifdef LEGACY
#include <HANDY.H>
#else
#include <stdnoj.hpp>
using namespace stdnoj;
#endif


template <class T>
   class RecEnum
      {
      protected:
         StdString FileName;
         RecEnum(void) : FileName("Default.Dat") {}

      public:
         RecEnum(const char *pszFile) : FileName(pszFile) {}
         virtual long Enumerate(T& r, long sp = ios::beg);
      };


template <class T>
   class BinRecEnum : RecEnum<T>
      {
      public:
         virtual long Enumerate(T& r, long sp = ios::beg);
      };


template <class T>
long RecEnum<T>::Enumerate(T& r, long sp)
   {
   ifstream ifs(FileName.c_str(), ios::in);  // ios::nocreate;
   if(!ifs || ifs.bad())
      return ios::beg;

   ifs.seekg(sp);
   r.read(ifs);
   if(ifs.eof())
      return ios::beg;

   return ifs.tellg();
   }


template <class T>
long BinRecEnum<T>::Enumerate(T& r, long sp)
   {
   ifstream ifs(FileName.c_str(), ios::in | ios::binary | ios::nocreate);
   if(!ifs || ifs.bad())
      return ios::beg;

   ifs.seekg(sp);
   r.read(ifs);
   if(ifs.eof())
      return ios::beg;

   return ifs.tellg();
   }
#endif
