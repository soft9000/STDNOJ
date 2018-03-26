/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2009 R. A. Nagy 

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
// The BasicPageManager allows a "frame" (page) to be imposed upon a line enumeration.
// In general, the page manager returns the line number, not the page, of each operation.
//
// 08/17/2008: Class created, R. Nagy
// 08/18/2008: Lightly tested. Basic functionality assured, but more on the way, R. Nagy
//
#ifndef BasicLineManager_hppx
#define BasicLineManager_hppx

#include "BasicLineManager.hpp"

// GUI pagination is a strange beastie for a number of reasons. First of all, users 
// expect to see the last item on the page become the first line on a new page.
// So beware - this class always honors the user's expectations. In other words, 
// starting at the top and doing two page downs is equivalent to going to page 2 
// (the zero-based third page).
//
class BasicPageManager : public BasicLineManager
{
private:
   int linesPerPage;
public:
   BasicPageManager(void);

   bool pageSize(int num_lines);                      // Set number of lines per page
   int  pageSize(void) const {return linesPerPage;}   // Get number of lines per page
   
   int  pageSet(int page_number);// Set page (absolute). Zero based.
   int  pageStat(void) const;    // Query where we are in page terms

   int  pageUp(int num = 1);     // Page up (relative)
   int  pageDown(int num = 1);   // Page down (relative)

   int  pageFirst(void);         // Go to the first page
   int  pageLast(void);          // Go to the last page
   
#ifdef TESTING_CASES
   bool test(ostream& os) const;
#endif

};

#endif
