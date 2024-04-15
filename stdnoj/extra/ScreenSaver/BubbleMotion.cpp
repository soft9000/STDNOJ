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

#include <noj/Screensaver.hpp>


using namespace ScreenSaver;

BubbleMotion::BubbleMotion(void)
{
   srand(time(NULL));
   xinc = yinc = 1;
   maxrate = 3;
   minrate = 1;
   trend = none;     // bugfix 09/14/03, R. Nagy
}

void BubbleMotion::Fixup(int& Left, int& Top)
{
   if(minrate < maxrate)
      {
      xinc = yinc = 0;
      while(xinc < minrate)
         xinc = rand() % maxrate;
      while(yinc < minrate)
         yinc = rand() % maxrate;
      }
   else
      xinc = yinc = maxrate;     // gigo

   int foo = int(trend);
   while(foo == int(trend))
      foo = rand() % 8;
   trend = movement(foo);
   Bump(Left, Top);
   Bump(Left, Top);
}

void BubbleMotion::Bump(int& Left, int& Top)
{
switch(trend)
   {
   case North:
      {
      Top  -= yinc;
      //Left -= xinc;
      }
   break;
   case South:
      {
      Top  += yinc;
      //Left -= xinc;
      }
   break;
   case East:
      {
      //Top  -= yinc;
      Left += xinc;
      }
   break;
   case West:
      {
      //Top  -= yinc;
      Left -= xinc;
      }
   break;
   case NorthWest:
      {
      Top  -= yinc;
      Left -= xinc;
      }
   break;
   case SouthEast:
      {
      Top  += yinc;
      Left += xinc;
      }
   break;
   case NorthEast:
      {
      Top  -= yinc;
      Left += xinc;
      }
   break;
   case SouthWest:
      {
      Top  += yinc;
      Left -= xinc;
      }
   break;
   }
}


bool BubbleMotion::Next(int &Left, int &Top, int Width, int Height, int maxX, int maxY)
{
bool br = false;
if(trend == none)
   trend = NorthWest;
if(Top + Height > maxY)
   {
   Top = maxY - Height;
   Fixup(Left, Top);
   br = true;
   }
if(Left + Width > maxX)
   {
   Left = maxX - Width;
   Fixup(Left, Top);
   br = true;
   }
if(Left <= 0)
   {
   Left = 1;
   Fixup(Left, Top);
   br = true;
   }
if(Top <= 0)
   {
   Top = 1;
   Fixup(Left, Top);
   br = true;
   }
Bump(Left, Top);
return br;
}

