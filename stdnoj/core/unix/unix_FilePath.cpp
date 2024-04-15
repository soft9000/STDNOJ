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
/* TODO 1 -cFilePath : Test this feature set. */
// Ref: <noj/Node.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace stdnoj
   {
char FilePath::PathChar(void)
   {
   return '/';
   }
bool FilePath::Open(const StdString& sPath, Node& result)
   {
   Node blanker;
   result = blanker;
   struct stat info;
   if(::stat(sPath.c_str(), &info) != 0)
      return false;
   result.IsOnline(true);
   result.FullName(sPath);
   if(S_ISDIR(info.st_mode))
      result.IsDir(true);
   else
      result.IsDir(false);
   if(info.st_mode & S_IREAD)
      result.IsRead(true);    // owner relative, only
   if(info.st_mode & S_IWRITE)
      result.IsWrite(true);   // owner relative, only
   if(info.st_mode & S_IEXEC)
      result.IsExec(true);    // owner relative, only
   result.SetCreateDateTime(info.st_ctime);
   result.SetModDateTime(info.st_mtime);
   result.SetAccessDateTime(info.st_atime);
   result.file_size.SetSize(info.st_size);  // off_t
   return true;
   }
bool FilePath::Open(const StdString& sPath, File& result)
   {
   Node basic;
   if(Open(sPath, basic) == false)
      return false;
   if(basic.IsDir() == true)
      return false;
   if(result.Name(sPath) == false)
      return false;
   if(result.Exists() == false)
      return false;
   return true;
   }
bool FilePath::Open(const StdString& sPath, Directory& result)
   {
   Node basic;
   if(Open(sPath, basic) == false)
      return false;
   if(basic.IsDir() == false)
      return false;
   if(result.Name(sPath) == false)
      return false;
   if(result.Exists() == false)
      return false;
   return true;
   }

   } // stdnoj

 
