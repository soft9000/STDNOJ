/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2024 R.A. Nagy 

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
// This little dutty demonstrates how to dump a file. Tool is usefull for
// doing things like inserting license agreements on top of files without 
// using the POSIX `cat` or DOS `COPY /B` commands. Example of use:
//
// merge mit_licenst.txt myfile.c
// 
// or 
//
// merge ! mit_licenst.txt myfile.c
//
// 11/09/2003: Program Created, R. Nagy
//
#include <stdnoj.hpp>

using namespace stdnoj;

void Usage(ostream& cerr, StdString& pName)
{
   cerr << "\tUsage: " << pName << " [!] header_file body_file" << endl;
   cerr << "\tPlaces header_file before body_file (stdout)." << endl;
   cerr << "\tUse ! to overwrite body file with combined content." << endl;
   cerr << "\t(Use of ! option creates a file named \"body_file.old\".)" << endl;
}


void main(int argc, char *argv[])
{
StdString pName = argv[0];
pName.to_upper();
if(argc < 3)
   {
   Usage(cerr, pName);
   return;
   }
bool bUpdate = false;
StdString dtA, dtB;  // Remove the arguments from the DTA (MSDOS)
if(argv[1][0] == '!')
   {
   if(argc != 4)
      {
      Usage(cerr, pName);
      return;
      }
   dtA = argv[2];
   dtB = argv[3];
   File fileSafe;
   if(fileSafe.Name(dtB) == false)
      {
      Usage(cerr, pName);
      return;
      }
   if(fileSafe.Exists() == false)
      {
      Usage(cerr, pName);
      return;
      }
   if(fileSafe.CopyTo(dtB + ".old") == false)
      {
      cerr << "Unable to create file " << fileSafe.Name() << endl;
      return;
      }
   bUpdate = true;
   }
else
   {
   dtA = argv[1];
   dtB = argv[2];
   }

cerr << pName << ": Merge the content of two files together." << endl;

File fileIn;
if(fileIn.Name(dtA) == false)
   {
   cerr << "ERROR: " << dtA << " is not a valid file name (terminated)." << endl;
   return;
   }
File fileOut;
fileOut.MkUniqueName();
ostream& os = fileOut.OpenWrite();
fileIn.DumpContents(os);
if(fileIn.Name(dtB) == false)
   {
   cerr << "ERROR: " << dtB << " is not a valid file name (terminated)." << endl;
   return;
   }
fileIn.DumpContents(os);

if(!bUpdate)
   fileOut.DumpContents(cout);
else
   {
   if(fileOut.CopyTo(fileIn) == false)
      cout << "ERROR: File update failed!" << endl;
   else
      cout << "File content updated." << endl;
   }
fileOut.Remove();
}