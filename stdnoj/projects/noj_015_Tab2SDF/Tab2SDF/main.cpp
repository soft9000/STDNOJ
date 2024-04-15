/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2024 R. A. Nagy 

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

//! I cobbled this NOJ together while struggling with 
//! the need to expand a tab-delimited file into 
//! something more viewable under a command-line 
//! challenged OS.
//! 
//! (1) Create a funky tab file -
//! MYDIR> RTree # > REPORT.TAB
//! 
//! (2) Use Tab2SDF to view the content
//! MYDIR> Tab2SFD REPORT.TAB
//! 
//! 	Or
//! 
//! (2) Use Tab2SDF to view the content
//! MYDIR> Tab2SFD REPORT.TAB > REPORT.SDF
//! 
//! 	Or
//! 
//! (1) As a pipe fitting:
//! MYDIR> RTree # | sort | Tab2SDF ! > REPORT.SDF
//!
//! How it Works:
//! =============
//! This program scans the REPORT.TAB file twice.
//!    Once to determine the optimal fill for each field and
//!    Once to display file content to cout / stdout.
//! 
//! Simple yet handy, this C++ tool is also neat for people 
//! who work with legacy COBOL or other data file / dataset 
//! mainframe formats. The perfect NOJ!
//!

#include <stdnoj.hpp>
using namespace stdnoj;

#include <noj/TypeThunk.hpp>

class Tab2SDF
{
public:
   
  static bool SaveStream(const File& fff, istream& is)
   {
   StdString sLine;
   File file = fff;
   ostream& os = file.OpenWrite();
   while(is)
      {
      sLine.readline(is);
      if(is)
         os << sLine << endl;
      }
   return true; 
   }
  static bool Scan(const File& fff, ostream& os, Array<TypeThunk<size_t> >& aPos)
   {
   File file = fff;
   aPos.Empty();
   if(file.Exists() == false)
      {
      os << "Error: File " << file.Name() << " not found." << endl;
      return false;
      }
   istream& is = file.OpenRead();
   Array<StdString> aFields;
   StdString sLine;
   while(is)
      {
      sLine.readline(is);   
      aFields = sLine.slice('\t');
      for(size_t ss = 0L; ss < aFields.Nelem(); ss++)
         {
         size_t iPos = aFields[ss].length();
         iPos++;  // add a space - SDF requires at least one extra -
         if(aPos[ss] < iPos)
            aPos[ss] = iPos;
         }
      }
   return true;
   }
   
  static bool List(const File& fff, ostream& os, Array<TypeThunk<size_t> >& aPos)
   {
   File file = fff;
   if(file.Exists() == false)
      {
      os << "Error: File " << file.Name() << " not found." << endl;
      return false;
      }
   istream& is = file.OpenRead();
   Array<StdString> aFields;
   StdString sLine;
   while(is)
      {
      sLine.readline(is);   
      aFields = sLine.slice('\t');
      for(size_t ss = 0L; ss < aFields.Nelem(); ss++)
         {
         size_t iPos = aPos[ss];
         StdString& sRef = aFields[ss];
         size_t iLen = sRef.length();
         if(iLen >= iPos)
            os << sRef << " ";   // GIGO
         else
            {
            // SDF
            iPos -= iLen;
            os << sRef << StdString(' ', iPos);
            }
         }
      os << endl;
      }
   return true;
   }
   
  static Usage(ostream& os)
   {
   os << endl;
   os << "Version: " << __DATE__ << "(" << __TIME__ << ") " << endl;
   os << "Tab2SDF: List the content of a TAB delimited file in SDF format." << endl;
   os << endl;
   os << "   Tab2SDF [!] | [REPORT.TAB]" << endl;
   os << endl;
   os << " Examples:" << endl;
   os << "    RTREE C:\\MYDIR | sort > REPORT.TAB" << endl;
   os << "    Tab2SDF REPORT.TAB" << endl;
   os << " Or:" << endl;
   os << "    RTREE C:\\MYDIR | sort | Tab2SDF ! > REPORT.SDF" << endl;
   os << endl;
   }

  static bool Main(Array<StdString>& array, istream& is, ostream& os)
   {
   bool bRemove = false;
   if(array.Nelem() != 2)
      {
      Usage(os);
      return false;
      }
   File file;
   if(array[1] == "!")
      {
      if(SaveStream(file, is) == false)
         {
         os << "Error: Unable to save stream to " << file.Name();
         return false;      
         }
      bRemove = true;
      }
   else
      {
      if(file.Name(array[1]) == false)
         {
         Usage(os);
         return false;
         }
      }
   Array<TypeThunk<size_t> > aPos;
   bool br = Tab2SDF::Scan(file, os, aPos);
   if(br == false)
      return br;
      
   br = Tab2SDF::List(file, os, aPos);
   
   if(bRemove)
      file.Remove();
      
   return br;
   }

  static bool Main(int argc, char *argv[], istream& is, ostream& os)
   {
   Array<StdString> array;

   for(size_t ss = 0; ss < argc; ss++)
      array.Append(argv[ss]);

   return Main(array, is, os);
   }
   
};

int main(int argc, char *argv[])
   {
   int iResult = (int) true;
   if(Tab2SDF::Main(argc, argv, cin, cout) == false)
      {
      iResult = (int) false;
      cerr << "Failure." << endl;
      }
   return iResult;
   }
   
