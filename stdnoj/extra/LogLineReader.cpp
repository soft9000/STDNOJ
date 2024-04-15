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

// 2010/03/03: Created.

#include "LogLineReader.hpp"


bool LogLineReader::Parse(const StdString& sLine, LogLine& result) {
   size_t pos = sLine.find("-");
   if(pos != npos)
      {
      result.sDate = sLine.subpos(0, pos);
      result.sPayload = sLine.subpos(pos + 1, NPOS);
      pos = result.sDate.find(" ");
      if(pos == npos) {
         goto done;
         }
      result.sTime = result.sDate.substr(pos, NPOS);
      result.sDate = result.sDate.substr(0, pos);
      if(result.sTime.tally(":") != 2) {
         goto done;
         }
      if(result.sDate.tally("/") != 2) {
         goto done;
         }
      result.trim();
      return true;
      }
   done:
      result.sDate = result.sTime = "";
      result.sPayload = sLine;
      std::cout << "poop!" << endl;
   return false;
}



