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
#include "extra/FileVersion.hpp"

FileVersion::FileVersion(void) : version(0.0), cReadOkay(0)
   {
   ProgramId pid;
   File file;
   pid.GetProgram(file);       // default 4this USED TO BE __DATE__: Now using the 'short'.EXE name.
   file.QueryNode(sProduct);   // the short name
   sVersionName = __DATE__;    // default 4this USED TO BE "<<undefined>>"
   key[0] = (char)0xfe;
   key[1] = 'n';
   key[2] = 'A';
   key[3] = 'U';
   key[4] = 'z';
   key[5] = (char)0xff;
   }
FileVersion::~FileVersion(void)
   {
   }
void FileVersion::ProductName(const char *psz)
   {
   sProduct = psz;
   }
const char *FileVersion::ProductName(void) const                            
   {
   return sProduct.c_str();
   }
void FileVersion::VersionName(const char *psz)
   {
   sVersionName = psz;
   }
const char *FileVersion::VersionName(void) const
   {
   return sVersionName.c_str();
   }
void FileVersion::VersionNumber(double fv)
   {
   version = fv;
   }
double FileVersion::VersionNumber(void) const
   {
   return version;
   }
int FileVersion::Read(istream& is)
   {
   read(is);
   return cReadOkay;
   }
ostream& FileVersion::write(ostream& os)
   {
   os.write((const char *)&key[0], 6);
   sVersionName.write_stream(os);
   sProduct.write_stream(os);
   os.write((char *)&version, sizeof(double));
   return os;
   }
istream& FileVersion::read(istream& is)
   {
   cReadOkay = 0;
   long sp = is.tellg();
   char buf[10];
   is.read((char *)&buf[0], 6);
   if(::memcmp(&buf[0], &key[0], 6) == 0)
      {
      cReadOkay = 1;
      sVersionName.read_stream(is);
      sProduct.read_stream(is);
      is.read((char *)&version, sizeof(double));
      }
   else
      is.seekg(sp);
   return is;
   }
