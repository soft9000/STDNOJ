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
// ScreenSaver: Classes to support moving rectangles (etc) about as used in
// a screen-saver (etc).
//
// 07/03/2003: Class created by refactoring old OmniWind BubbleMotion class, R. Nagy
// 12/03/2006: ScreenSaver namespace created, R. Nagy
//
#ifndef BubMot_hpx
#define BubMot_hpx

namespace ScreenSaver
{
class BubbleMotion
   {
   protected:
      int maxrate;
      int minrate;
      int xinc, yinc;
      void Bump(int &X, int &Y);
      void Fixup(int &X, int &Y);
   public:
      enum movement
         {
         North,
         South,
         East,
         West,
         NorthWest,
         NorthEast,
         SouthWest,
         SouthEast,
         none
         } trend;

      BubbleMotion(void);
      // Returns TRUE if there has been a change in movement:
      bool Next(int &X, int &Y, int xWide, int yHigh, int maxX, int maxY);
      void MaxRate(int r)  {maxrate = r;}
      int  MaxRate(void)   {return maxrate;}
      void MinRate(int r)  {minrate = r;}
      int  MinRate(void)   {return minrate;}
   };
}

#endif
