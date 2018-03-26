/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2009 R. A. Nagy 

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to 
deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, List, publish, distribute, sublicense, and/or 
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

//! This is a handy little noj that you can use to create a node-first listing
//! of a directory tree.
//! 
//! As a utility, there is a lot that we could yet add, test, share, and enjoy... 
//! (most notably the date feature!) Feel free to contribute.
//!

#include <stdnoj.hpp>
using namespace stdnoj;

class RTree
{
public:
   
  static bool List(const Directory& dir, ostream& os, bool bPrefixDate)
   {
   bool br = false;
   Array<File> files;
   dir.Query(files);
   for(size_t ss = 0L; ss < files.Nelem(); ss++)
      {
      br = true;
      File& file = files[ss];
      os << file.QueryNode() << "\t";
      if(bPrefixDate)
         {
         StdDateTime sdt(file.GetTime());
         unsigned int yy, mm, dd;
         sdt.DecodeDate(yy, mm, dd);
         os << yy << ":" << mm << ":" << dd << " - ";
         sdt.DecodeTime(yy, mm, dd);
         os << yy << ":" << mm << ":" << dd << "\t";
         }
      os << dir.Name() << endl;
      }
   Array<Directory> dirs;
   dir.Query(dirs);
   for(size_t ss = 0L; ss < dirs.Nelem(); ss++)
      {
      br = true;
      List(dirs[ss], os, bPrefixDate);
      }
      
   return br;
   }
   
  static Usage(ostream& os)
   {
   os << "Version: " << __DATE__ << "(" << __TIME__ << ") " << endl;
   os << "RTree: List the content of a directory tree in sortable order." << endl;
   os << "   RTree [directory] [#]" << endl;
   os << "     directory - Starting point (default is the pwd)" << endl;
   os << "     #         - Follow the node names with node's date" << endl;
   os << endl;
   os << " Example: RTree C:\\WINDOWS | sort | more" << endl;
   }

  static bool Main(Array<StdString>& array, ostream& os)
   {
   bool bUseDate = false;
   Directory dir;
   dir.Get();  // default is to use the pwd
   StdString str;
   if(array.Nelem() == 2)
      {
      str = array[1];
      if(str.find(PathChar()) != npos)
         {
         if(dir.Name(str) == false)
            {
            Usage(os);
            os << "Invalid directory name: " << array[1] << endl;
            return false;
            }
         }
      else
         bUseDate = true;
      
      return RTree::List(dir, os, bUseDate);
      }
   if(array.Nelem() == 3)
      {
      bUseDate = true;
      str = array[1];
      if(str.find(PathChar()) == npos)
         str = array[2];
      if(dir.Name(str) == false)
         {
         Usage(os);
         os << "Invalid directory name: " << array[1] << endl;
         return false;
         }
      return RTree::List(dir, os, bUseDate);
      }

   Usage(os);
   return false;
   }

  static bool Main(int argc, char *argv[], ostream& os)
   {
   Array<StdString> array;

   for(int ss = 0; ss < argc; ss++)
      array.Append(argv[ss]);

   return Main(array, os);
   }
   
};

int main(int argc, char *argv[])
   {
   int iResult = (int) true;
   if(RTree::Main(argc, argv, cout) == false)
      {
      iResult = (int) false;
      cerr << "Failure." << endl;
      }
   return iResult;
   }
