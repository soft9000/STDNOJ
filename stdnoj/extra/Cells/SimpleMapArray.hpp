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
//---------------------------------------------------------------------------

#ifndef SimpleMatArray_hpp
#define SimpleMatArray_hpp
//---------------------------------------------------------------------------

#include <Cells/CellMap.hpp>

using namespace Cells;

namespace Cells
{

// A SimpleMapArray is all of the cells layed out in a cartesian plain,
// with the origin (0, 0) in the upper right (CRT default). No offset
// origin, or page support. The basis for many array management
// classes.
template <class T>
class SimpleMapArray
{
protected:
   CellMap map;
public:
   Array<T> aCells;

   unsigned       CellCount(void)               {return map.CellCount();}
   CellLocation   MatSize(void)                 {return map.MatSize();}
   bool           MatResize(CellLocation cl);

   CellLocation   Query(size_t sz)              {return map.Query(sz);}
   size_t         Query(CellLocation& cl)       {return map.Query(cl);}
   bool           Normalize(CellLocation& cl)   {return map.Normalize(cl);}
};

// Scrolling map adds a scrollable origin to the SimplyMap signature. Nothing more.
template <class T>
class ScrollingMat
{
protected:
   Array<T>      simpleMap;
   CellLocation  offset;
public:
   ScrollingMat(CellLocation ref);            // Mat dimensions

   bool LineUp(unsigned sz = 1);
   bool LineDown(unsigned sz = 1);
   bool ComumnLeft(unsigned sz = 1);
   bool ColumnRight(unsigned sz = 1);
};

struct PointLocation
{
unsigned x;
unsigned y;
PointLocation(void)   : x(undefined), y(undefined) {}
};

template <class T>
class GraphicalMat
{
};

template <class T>
class SrcollingGraphicalMat
{
};



template <class T>
bool SimpleMapArray<T>::MatResize(CellLocation ref)
{
if(map.MatResize(ref) == false)
   return false;
if(map.CellCount() >= aCells.Nelem())
   {
   // expand the array
   T ref;
   aCells[ss] = ref;
   }
return true;
}

} // Namespace

#endif
