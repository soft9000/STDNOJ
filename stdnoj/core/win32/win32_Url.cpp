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
/* TODO 1 -cUrl : Test this feature set. */

#define STRICT
#include <windows.h>
#include <shellapi.h>// shellexecute()

namespace stdnoj
   {
   bool Url::Launch(void) const
      {
      if(sUrl.is_null())
         return false;
      if(int(ShellExecute(NULL, "open", sUrl.c_str(), NULL, NULL, SW_SHOWNORMAL)) <= 32)
         return false;
      return true;
      }
   } // stdnoj


