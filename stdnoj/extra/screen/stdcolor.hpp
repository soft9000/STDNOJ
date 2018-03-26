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
// Color8
// 05/03/2005: Header created to handle same (cognative extraction from my ctoos operating system), R. Nagy
//
#ifndef StdColor_hpx
#define StdColor_hpx

#include <stdnoj.hpp>
using namespace stdnoj;

namespace stdscreen
{

// This color encoding scheme will represent 8 colors across a variety of platforms.
// Even if your platform does not have color, you can use this across the stdnoj api.
struct Color8
{
bool red, green, blue;

Color8(void) :red(false), green(false), blue(false)
   {
   }
Color8(bool bred, bool bgreen, bool bblue) : red(bred), green(bgreen), blue(bblue)
   {
   }
void Set(bool bred, bool bgreen, bool bblue)
   {
   red   = bred;
   green = bgreen;
   blue  = bblue;
   }
StdString Encode(void) const
   {
   StdString sResult;
   if(red)
      sResult.append("r");
   if(green)
      sResult.append("g");
   if(blue)
      sResult.append("b");
   return sResult;
   }
StdString Decode(const StdString& code)
   {
   StdString sResult = code;
   sResult.to_lower();
   red = blue = green = false;
   if(sResult.find('r') != npos)
      red = true;
   if(sResult.find('g') != npos)
      green = true;
   if(sResult.find('b') != npos)
      blue = true;
   }
static Color8 FromString(const StdString& str)
   {
   Color8 color;
   color.Decode(str);
   return color;
   }
static StdString ToString(const Color8& color)
   {
   return color.Encode();
   }
};

struct ColorSet
	{
	Color8 foreground;
	Color8 background;
	ColorSet(void)
	   {
	   Reset();
	   }
	void Reset(void)
	   {
	   foreground.red = foreground.green = foreground.blue = true;
	   background.red = background.green = background.blue = false;
	   }
	};


/*
 r g b
 = = =
 + + + white
 - - - black
 + - - red
 - + - green
 - - + blue
 + + - yellow
 - + + lt blue
 + - + lt red
*/

inline Color8 White8(void)
   {
   Color8 result;
   result.Set(true, true, true);
   return result;
   }
inline Color8 Black8(void)
   {
   Color8 result;
   result.Set(false, false, false);
   return result;
   }
inline Color8 Red8(void)
   {
   Color8 result;
   result.Set(true, false, false);
   return result;
   }
inline Color8 Green8(void)
   {
   Color8 result;
   result.Set(false, true, false);
   return result;
   }
inline Color8 Blue8(void)
   {
   Color8 result;
   result.Set(false, false, true);
   return result;
   }
inline Color8 Yellow8(void)
   {
   Color8 result;
   result.Set(true, true, false);
   return result;
   }
inline Color8 LightBlue8(void)
   {
   Color8 result;
   result.Set(false, true, true);
   return result;
   }
inline Color8 LightRed8(void)
   {
   Color8 result;
   result.Set(true, false, true);
   return result;
   }

} // namespace

#endif
