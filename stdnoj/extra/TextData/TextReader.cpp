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
#include "FileConversion.hpp"

using namespace FileConversion;

bool TextReader::ReadFirst(Array<StdString>& aResult, LinearType type)
   {
   delete pReader;
   pReader = 0L;

   switch(type)
      {
      case ltCsv:
         {
         pReader = new CsvLine();
         }
      break;

      case ltTab:
         {
         pReader = new TabbedLine();
         }
      break;

      case ltSdf: 
      // At some point we will auto-detect the parameters 
      // required for this one - but not now -
      default:
      break;
      }

   if(pReader)
      {
      if(pwFile.Exists() == false)
         {
         delete pReader;
         sLastError = StdString("Not found: ") + pwFile.Name();
         return false;
         }
      // At some point we should use the binary mode so 
      // as to make Linux <-> Win32 transitions easier -
      if(pwFile.OpenRead() == false)
         {
         delete pReader;
         sLastError = StdString("Cannot read: ") + pwFile.Name();
         return false;
         }
      }

   return ReadNext(aResult);
   }
bool TextReader::ReadNext(Array<StdString>& aResult)
   {
   if(!pReader)
      {
      sLastError = "Unsupported LinearType Reader.";
      return false;
      }
   istream& is = pwFile.Resume();
   bool br = pReader->readline(is, aResult);
   if(br == false)
      {
      sLastError = pReader->sLastError;
      pwFile.Close();
      delete pReader;
      pReader = 0L;
      }
   return br;
   }

bool TextReader::Recall(size_t zbRecordId, const File& fff, Array<StdString>& aResult, LinearType type)
   {
   bool br = false;

   File file = fff;

   TextReader reader(file);

   br = reader.ReadFirst(aResult, type);
   if(br == false)
      return br;

   size_t pwRec = 0L;
   if(zbRecordId)
      {
      while(pwRec != zbRecordId)
         {
         br = reader.ReadNext(aResult);
         if(br == false)
            break;
         pwRec++;
         }
      }
   return br;
   }
