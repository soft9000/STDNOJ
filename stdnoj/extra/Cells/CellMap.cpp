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
#include <Cells/CellMap.hpp>

using namespace Cells;

CellMap::CellMap(unsigned x, unsigned y)
{
CellLocation ref;
ref.X(x); ref.Y(y);
MatResize(ref);
}

CellMap::CellMap(CellLocation ref)
{
MatResize(ref);
}

unsigned CellMap::CellCount(void)
{
if(matDimension.x == undefined || matDimension.y == undefined)
   return undefined;
return (matDimension.x * matDimension.y);
}

bool CellMap::MatResize(CellLocation ref)
{
if(ref.X() == undefined || ref.Y() == undefined)
   return false;
if(ref.X() == 0 || ref.Y() == 0)
   return false;
if(matDimension.x == undefined)
   {
   matDimension.x = ref.X();
   matDimension.y = ref.Y();
   }
size_t ss = ref.X() * ref.Y();

matDimension.x = ref.X();
matDimension.y = ref.Y();
return true;
}

CellLocation CellMap::MatSize(void)
{
CellLocation loc;
loc.X(matDimension.x);
loc.Y(matDimension.y);
return loc;
}

CellLocation CellMap::Query(size_t query)
{
CellLocation result;
if(query < CellCount())
   {
   result.X(query % matDimension.x);
   result.Y(query / matDimension.y);
   }
return result;
}

size_t CellMap::Query(CellLocation& ref)
{
normalize(ref);
size_t ss = calcit(ref);
if(ss < CellCount())
   return ss;
return undefined;
}

bool CellMap::Normalize(CellLocation& ref)
{
CellLocation loc = ref;
normalize(ref);
if(ref == loc)
   return false;
return true;
}

bool CellMap::normalize(CellLocation& ref)
{
bool br = false;
// Probable cause: Decrementing past zero
if(ref.X() == undefined)
   {
   ref.X(0);
   br = true;
   }
// Probable cause: Decrementing past zero
if(ref.Y() == undefined)
   {
   ref.Y(0);
   br = true;
   }
// Probable cause: Incrementing past range
if(ref.X() >= matDimension.x)
   {
   ref.X(matDimension.x - 1);
   br = true;
   }
// Probable cause: Incrementing past range
if(ref.Y() >= matDimension.y)
   {
   ref.Y(matDimension.y - 1);
   br = true;
   }
return br;
}

size_t CellMap::calcit(CellLocation& ref)
{
if(ref.X() == undefined)
   return undefined;
if(ref.Y() == undefined)
   return undefined;
return ((ref.Y()) * matDimension.y) + (ref.X());
}

