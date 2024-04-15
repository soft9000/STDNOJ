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
//
// This project will read a pipe fitting looking for file names. 
// Once validated, an input source file will be updated so as to
// replace obsolete header usages. This updating includes things
// like changing "stdio.h" to "cstdio", "strstrea.h" to "strsteam",
// and "strstream.h" to "strstream". An appropriate "using namespace std;"
// statement is also added where, and as, required by the conversion.
//
// Input files are only updated as needed. Backup file is created / overwritten.
//
// Under Linux, use the "find" command:
//    find /usr/code -type f -name "*.h" -print | HeaderUpdate
//            or 
//    find /usr/code -type f -name "*.h" -print "{}\;" | HeaderUpdate
//
// Under DOS, use the "attrib" command:
//    attrib *.h /s | HeaderUpdate
// 
// **********************************************************
// WORKS FINE ON MY MACHINE, BUT BACKUP YOUR FILES BEFORE USE.
// **********************************************************
//
#include <stdnoj.hpp>

using namespace stdnoj;


class shazam
{
protected:
   Array<StdString> lines;

   long lReplaced;
   bool bChanged;
   int iLastLine;

public:
   shazam(void) : lReplaced(0L), bChanged(false), iLastLine(0)
      {
      // Pre-allocate some space in the array
      lines.AddZombies(10000);
      }

   long Results(void)
      {
      return lReplaced;
      }

   bool Load(File& file)
      {
      iLastLine = 0;
      lines.Empty();
      bChanged = false;
      if(file.Exists() == false)
         return false;

      istream& is = file.OpenRead();
      StdString sline;
      while(is)
         {
         sline.readline(is);
         if(is)
            lines[lines.Nelem()] = sline;
         }

      file.Close();

      if(!lines.Nelem())
         return false;

      return true;
      }
   bool Save(File& file)
      {
      if(bChanged == false)
         return true;   // no need to save...

      // Create a fallback position (backup)
      File copy;
      StdString str = file.Name();
      str += ".backup";
      if(copy.Name(str) == false)
         return false;

      copy.Remove();

      if(file.CopyTo(copy) == false)
         return false;

      // Add a using clause, if required
      if(iLastLine)
         {
         lines.Insert("using namespace std;", iLastLine);
         }

      // Save the update
      ostream& os = file.OpenWrite();
      for(size_t ss = 0L; ss < lines.Nelem(); ss++)
         {
         os << lines[ss] << endl;
         }
      file.Close();

      return file.Exists();
      }
   bool Changed(void)
      {
      return bChanged;
      }
   void SwapPair(const StdString& sPat1)
      {
      StdString sPat2 = sPat1;
      sPat2.append(".h");
      SwapPair(sPat1, sPat2, true);
      }
   void SwapPair(const StdString& sNew, const StdString& sOld, bool bStl = false)
      {
      StdString sBuf;
      for(size_t ss = 0L; ss < lines.Nelem(); ss++)
         {
         sBuf = lines[ss];
         sBuf.to_lower();
         if(sBuf.find("#include") != npos)
            {
            size_t pos = sBuf.find(sOld);
            if(pos != npos)
               {
               char ch = sBuf[pos-1];
               if(ch != '<' && ch != '"')
                  continue;
               ch = sBuf[pos+sOld.length()];
               if(ch != '>' && ch != '"')
                  continue;

               sBuf.remove_pos(pos, pos + sOld.length());
               sBuf.insert(pos, sNew);
               lines[ss] = sBuf;
               lReplaced++;
               bChanged = true;
               if(bStl)
                  {
                  int inc = ss + 1;
                  if(iLastLine < inc)
                     iLastLine = inc;
                  }
               }
            }
         }
      }
};


void Fix(shazam& fix, File& file)
{
   if(fix.Load(file) == false)
      {
      cerr << "Unable to load " << file.Name() << endl;
      return;
      }

   fix.SwapPair("cassert", "assert.h"); 
   fix.SwapPair("cctype", "ctype.h"); 
   fix.SwapPair("cerrno", "errno.h"); 
   fix.SwapPair("cfloat", "float.h"); 
   fix.SwapPair("ciso646", "iso646.h"); 
   fix.SwapPair("climits", "limits.h"); 
   fix.SwapPair("clocale", "locale.h"); 
   fix.SwapPair("cmath", "math.h"); 
   fix.SwapPair("csetjmp", "setjmp.h"); 
   fix.SwapPair("csignal", "signal.h"); 
   fix.SwapPair("cstdarg", "stdarg.h"); 
   fix.SwapPair("cstddef", "stddef.h"); 
   fix.SwapPair("cstdio", "stdio.h"); 
   fix.SwapPair("cstdlib", "stdlib.h"); 
   fix.SwapPair("cstring", "string.h"); 
   fix.SwapPair("ctime", "time.h"); 
   fix.SwapPair("cwchar", "wchar.h"); 
   fix.SwapPair("cwtype", "wtype.h");
 
   fix.SwapPair("algorithm");   
   fix.SwapPair("bitset");   
   fix.SwapPair("deque");   
   fix.SwapPair("exception");   
   fix.SwapPair("fstream");   
   fix.SwapPair("functional");   
   fix.SwapPair("iomanip");   
   fix.SwapPair("ios");   
   fix.SwapPair("iosfwd");   
   fix.SwapPair("iostream");   
   fix.SwapPair("istream");   
   fix.SwapPair("iterator");   
   fix.SwapPair("limits");   
   fix.SwapPair("list");   
   fix.SwapPair("locale");   
   fix.SwapPair("map");   
   fix.SwapPair("memory");   
   fix.SwapPair("new");   
   fix.SwapPair("numeric");   
   fix.SwapPair("ostream");   
   fix.SwapPair("queue");   
   fix.SwapPair("set");   
   fix.SwapPair("sstream");   
   fix.SwapPair("stack");   
   fix.SwapPair("stdexcept");   
   fix.SwapPair("streambuf");   
   fix.SwapPair("string");   
   fix.SwapPair("typeinfo");   
   fix.SwapPair("utility");   
   fix.SwapPair("valarray");   
   fix.SwapPair("vector");

   fix.SwapPair("strstream");                     // Hummmmm....
   fix.SwapPair("strstream", "strstrea.h", true); // and again...

   if(fix.Changed() == false)
      {
      cout << "Bypassed " << file.Name() << "." << endl;
      return;
      }

   if(fix.Save(file) == false)
      {
      cerr << "Unable to save " << file.Name() << endl;
      return;
      }
   cout << "Updated " << file.Name() << "." << endl;
}

void debug_main(int argc, char *argv[]) {
   shazam fix;
   File file;
   file.Name("X.TXT");
   ostream& os = file.OpenWrite();
   os << "#" << "include <stdio.h>     // hello!"  << endl; // (common)
   os << "// This should remain." << endl;
   os << "#" << "include <strstrea.h>  // again!!" << endl; // (8.3)
   os << "// This should remain, too" << endl;
   file.Close();
   Fix(fix, file);
   file.Remove();
}

int main(int argc, char *argv[]) {
#if 0
   debug_main(argc, argv);
#else
   shazam fix;
   File file;
   StdString str;
   while(cin)
      {
      str.readline(cin);
      size_t pos = str.find(':');
      if(pos == npos)
         continue;
      str.remove_pos(0, pos - 1);
      str.strip();
      if(file.Name(str) == false)
         {
         cerr << "Unble to open file " << str << endl;
         continue;
         }
      Fix(fix, file);
      }

   cout << "Replaced " << fix.Results() << " items." << endl;
   return 1;
#endif
}
