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
//---------------------------------------------------------------------------

#ifndef CellMap_hpp
#define CellMap_hpp
//---------------------------------------------------------------------------

#include <Cells/CellLocation.hpp>

namespace Cells
{
// A CellMap is all of the cells layed out in a cartesian plain,
// with the origin (0, 0) in the upper right (CRT default). No offset
// origin, or page support. The basis for many linear array management
// classes.
class CellMap
{
protected:
   size_t calcit(CellLocation& ref);    // raw result. undefined on error.
   bool   normalize(CellLocation& ref); // true if it was normalized.
   struct keep
      {
      unsigned x;
      unsigned y;
      keep(void) : x(undefined), y(undefined) {}
      } matDimension;
public:
   CellMap(unsigned x, unsigned y);       // Mat dimensions
   CellMap(CellLocation ref);             // Mat dimensions

   unsigned       CellCount(void);        // Number of cells
   CellLocation   MatSize(void);          // Mat dimensions
   bool           MatResize(CellLocation);// Mat dimensions

   CellLocation   Query(size_t);             // Array position to cell
   size_t         Query(CellLocation&);      // Cell position to array. WILL NORMALIZE!
   bool           Normalize(CellLocation&);  // Normalize as required. True if it had to.
};

} // namespace Cells
#endif
