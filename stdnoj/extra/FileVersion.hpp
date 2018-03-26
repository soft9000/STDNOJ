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
// Class FileVersion: A way to validate and uniquely identify the version(s) of a data file (saving room
// for backwards-compatibilities). On read, the class first checks for its own header, then reads it's data,
// if the file-header properly identifies it as being able to do so. Since file read() always properly 
// resets to pre-read position upon failure, file-version testing can be iterative: Use Read() to
// get the result of the header-read attempt (1 means all went okay).
//
// THE PRODUCT NAME MUST BE THE SAME ACROSS ALL VERSIONS: Suggest that you use a 'REGISTER CLASS' 
// NAME: Once the class insure that the header and product identifiers are okay, then the rest is 
// up to your program to determine if it can read the specific file-version (etc).
//
// The ProductName() is the *ONLY* thing that operator= is concerned with!! Use Read() to 
// check if the last read() detected a valid FileVersion header, the operator= with a default
// FileVersion to see if the products match up. See EzInstall::Query() and QueryImport() for 
// an example of how this can be easily implemented using polymorphism.
//
// 03/18/1998: Class created, R. Nagy
// 11/16/1998: Added ProgramId in default constructor to replace "undefined", R. Nagy
//             Virtualized Read() and read()/write() to allow class to be "grown", R. Nagy
//
#ifndef FILE_VERSIONx_HPP
#define FILE_VERSIONx_HPP

#include <stdnoj.hpp>

using namespace stdnoj;

class FileVersion
   {
   private:
      char cReadOkay;

   protected:
      double      version;          // the FILE-FORMAT VERSION: default is 0.0
      StdString   sVersionName;     // e.g: "EzInstall 4.6": default is last date of compilation
      StdString   sProduct;         // default is EXE name
      char        key[6];
   public:
      FileVersion(void);
      virtual ~FileVersion(void);

      // THE PRODUCT NAME MUST BE THE SAME ACROSS ALL VERSIONS: Suggest that you use the REGISTER CLASS NAME.
      // ProductName is the *ONLY* thing that operator= is concerned with!!
      void        ProductName(const char *psz);
      const char *ProductName(void) const;
      // This can be anything, e.g: "EzInstall 4.6";
      void        VersionName(const char *psz);
      const char *VersionName(void) const;
      // This can be anything. It identifies the FILE-FORMAT VERSION!;
      void        VersionNumber(double fv);
      double      VersionNumber(void) const;

      // ProductName is the *ONLY* thing that operator= is concerned with!!
      int operator==(const FileVersion& fv)     {return (sProduct == fv.sProduct);}
      int operator!=(const FileVersion& fv)     {return (!(sProduct == fv.sProduct));}

      // Use this to get a successfull read indication; 1 means "okay";
      // (file read() always properly resets to pre-read position upon failure,
      // so file-version testing can be iterative);
      virtual int Read(istream& is);

      virtual ostream& write(ostream& os);
      virtual istream& read(istream& is);
   };
   
#endif

