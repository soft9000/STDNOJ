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
#ifndef CellLocation_Hpp
#define CellLocation_Hpp

#include <stdnoj.hpp>


namespace Cells
{
using namespace stdnoj;

const unsigned undefined = -1;

// Minimum cell locaitons is (0,0).
// Default cell location is (undefined, undefined).
struct CellLocation
{
protected:
   unsigned x;
   unsigned y;
public:
   CellLocation(void)   : x(undefined), y(undefined)  {}
   CellLocation(unsigned x, unsigned y)               {X(x); Y(y);}

   bool X(unsigned i)  {x = i; return true;}
   bool Y(unsigned i)  {y = i; return true;}
   unsigned X(void)    {return x;}
   unsigned Y(void)    {return y;}

   int operator==(const CellLocation& ref) {if((x == ref.x) && (y == ref.y)) return 1; return 0;}
};

} // Namespace

#endif

