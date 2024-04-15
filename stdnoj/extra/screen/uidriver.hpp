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
// 05/04/2005: Created, Randall Nagy
//
#ifndef uidriver_hpx
#define uidriver_hpx

#include "uichar.hpp"

namespace stdscreen
{
// The idea here is to keep this as simple as possible for maximum
// portability across a tui / gui family - Also to allow for doubble
// buffering so as to make for smoother screen transitions.

class uibuffer
   {
   public:
      virtual void cls(const ColorSet& ref) = 0;
      virtual bool putch(int x, int y, uichar& ref) = 0;
      virtual void querysize(int& x, int& y) = 0;
   };

class uiaction
   {
   public:
      virtual bool process(void);
   };
   
class uidriver
   {
   public:
      virtual bool iodisplay(uibuffer *buffer, uiaction *action) = 0;
      virtual uibuffer *create(int x = 80, int y = 25) = 0;
   };
   
} // namespace

#endif   
