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
#include "BasicLineManager.hpp"

bool BasicLineManager::range(int top, int bottom, int pos)
   {
   view_stat hold = line;
   line.top = top;
   line.bottom = bottom;
   if(setLine(pos) != pos)
      {
      line = hold;
      return false;
      }
   return true;         
   }
bool BasicLineManager::isEnd(void) const     // Being at the end is a VALID condition.
   {
   if(line.pos == line.bottom)
      return true;
   return false;         
   }      
int BasicLineManager::setLastLine(void)   // The last line is the line BEFORE the end (bottom).
   {
   return setLine(line.bottom - 1);
   }      
int BasicLineManager::setFirstLine(void)  // Same as the Top.
   {
   return setLine(line.top);
   }      
int BasicLineManager::setLine(int pos)
   {
   if(pos < line.top)
      return -1;
   if(pos > line.bottom)
      return -1;
   line.pos = pos;
   return line.pos;
   }
int BasicLineManager::setLineUp(int nelem)
   {
   int pos = line.pos - nelem;
   if(pos < line.top)
      return -1;
   line.pos = pos;
   return line.pos;
   }
int BasicLineManager::setLineDown(int nelem)   // It is possible to line down to the EOF. Just show a blank page.
   {
   int pos = line.pos + nelem;
   if(pos > line.bottom)
      return -1;
   line.pos = pos;
   return line.pos;
   }
int BasicLineManager::rangeStat(void) const
   {
   return line.pos;
   }
// This returns the original EOF (N+1) value given.
int BasicLineManager::rangeBottom(void) const
   {
   return line.bottom;
   }
int BasicLineManager::rangeTop(void) const
   {
   return line.top;
   }
bool BasicLineManager::operator=(view_stat& vs)
   {
   return range(vs.top, vs.bottom, vs.pos);
   }

#ifdef TESTING_CASES

bool BasicLineManager::test(ostream& os) const
{
BasicLineManager lTest;
// Step: Simple pagination;
if(lTest.range(0, 100, 0) == false)
   {
   os << "lineRange (set) test fails." << endl;
   return false;
   }
if(lTest.setLineUp() != -1)
   {
   os << "setUp (undeflow) fails." << endl;
   return false;
   }
if(lTest.setLine(lTest.rangeBottom()+1) != -1)
   {
   os << "set (overflow) fails." << endl;
   return false;
   }
if(lTest.setLine(lTest.rangeBottom()) == -1)  // should always accept its previously given range...
   {
   os << "set (exact - bottom) fails." << endl;
   return false;
   }
if(lTest.setLine(lTest.rangeBottom() - 1) == -1)
   {
   os << "setLine(acceptable) fails." << endl;
   return false;
   }
if(lTest.setLineUp() != lTest.rangeBottom() - 2)
   {
   os << "setLineUp (acceptable) fails." << endl;
   return false;
   }
if(lTest.setLineUp(5) != lTest.rangeBottom() - 7)
   {
   os << "setLineDown (acceptable n) fails." << endl;
   return false;
   }
// last   
if(lTest.setLastLine() == -1)
   {
   os << "setLastLine fails." << endl;
   return false;
   }   
if(lTest.setLineDown() == -1)
   {
   os << "setLineLast & setLineDown fails." << endl;
   return false;
   }   
if(lTest.isEnd() == false)
   {
   os << "lineEnd detection fails." << endl;
   return false;
   }   
// top setting   
if(lTest.setLine(lTest.rangeTop()) == -1)  // should always accept its previously given range...
   {
   os << "set (exact - top) fails." << endl;
   return false;
   }
if(lTest.setFirstLine() == -1)              // Logical - always works.
   {
   os << "setLineFirst fails." << endl;
   return false;
   }
return true;   
}

#endif
