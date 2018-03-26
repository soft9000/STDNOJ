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
#include <stdnoj.hpp>

#include <fstream>
#include <ctime>             // time_t

#include <sys/stat.h>
#include <unistd.h>

const int HUGE_PATH = 1024;

/* TODO : Need to implement several POSIX platform equivalents here. */

namespace stdnoj
{

bool Platform::ShellExec(const StdString& sUrl, const StdString& str)
{
return false;
}

bool Platform::ShellExec(const StdString& sUrl)
{
return false;
}

bool Platform::GetPwd(StdString& str)
{
   bool br = false;
   char *pBuf = new char[MAX_PATH + 1];
   if(::getcwd(pBuf, MAX_PATH) != NULL)
      {
      str = pBuf;
      br = true;
      }
   delete [] pBuf;
   return br;
}

bool Platform::PushPwd(StdString& std)
{
   bool br = false;
   char *pBuf = new char[MAX_PATH + 1];
   if(::getcwd(pBuf, MAX_PATH))
      {
      std.append(";");
      std.append(pBuf);
      br = true;
      }
   delete [] pBuf;
   return br;
}

bool Platform::PopPwd(StdString& std) 
{
   bool br = false;
   size_t sz = std.rfind(";");
   if(sz == NPOS)
      return false;
   StdString sDir;
   std.copy_pos(sDir, sz+1, std.length());
   if(::chdir(sDir.c_str()))
      br = true;
   std.remove(sz);
   return br;
}

bool Platform::DirectoryChange(const StdString& sDir) 
{
   if(::chdir(sDir.c_str()))
      return true;
   return false;
}

bool Platform::DirectoryExists(const StdString& sDir) 
{
   bool br = false;
   StdString std;
   PushPwd(std);
   if(::chdir(sDir.c_str()))
      br = true;
   PopPwd(std);
   return br;
}

bool Platform::IsValidDirName(const StdString& std, bool bAllowDriveSpecifier)
{
   if(std.is_null())
      return false;

   if(std.find('/') != NPOS)
      return false;
   if(std.find('*') != NPOS)
      return false;
   if(std.find('?') != NPOS)
      return false;
   if(std.find('\"') != NPOS)
      return false;
   if(std.find('<') != NPOS)
      return false;
   if(std.find('>') != NPOS)
      return false;
   if(std.find('|') != NPOS)
      return false;

   if(bAllowDriveSpecifier == true)
      {
      size_t ss = std.find(':');
      if(ss != NPOS)
         {
         if(ss != 1)
            return false;
         return IsValidDirName(&std[ss+1], false);
         }
      }
   else
      {
      if(std.find(':') != NPOS)
         return false;
      }
   return true;
}

bool Platform::FileRename(const StdString& pszFrom, const StdString& pszTo)
{
   if(::rename(pszFrom.c_str(), pszTo.c_str()) == 0)
      return true;
   return false;
}

bool Platform::FileHide(const StdString& sFileName)
{
   bool br = false;
   return br;
}

bool Platform::FileShow(const StdString& sFileName)
{
   bool br = false;
   return br;
}

bool Platform::IsFileHidden(const StdString& sFileName)
{
   File node;
   FilePath fs;
   fs.Open(sFileName, node);
   return node.IsHidden();
}

bool Platform::Touch(const StdString& sFileName)
{
   bool br = false;
   return br;
}

bool Platform::FileCopy(const StdString& strFrom, const StdString& strTo)
{
   bool br = false;
   return br;
}

bool Platform::FileReadable(const StdString& str)
{
   Node node;
   FilePath fs;
   fs.Open(str, node);
   return node.IsRead();
}

bool Platform::FileWritable(const StdString& str)
{
   Node node;
   FilePath fs;
   fs.Open(str, node);
   return node.IsWrite();
}

bool Platform::FileExists(const StdString& str)
{
   struct stat info;
   if(::stat(str.c_str(), &info) != 0)
      return false;
   return true;
}

bool Platform::DirectoryRemove(const StdString& str)
{
   bool br = false;
   if(::rmdir(str.c_str()) == 0)
      br = true;
   return br;
}

bool Platform::DirectoryEmpty(const StdString& sPwd)
{
   bool br = true;
   Array<File> ary;
   FileSystem fs;
   fs.Query(sPwd, ary);
   for(size_t ss = 0L; ss < ary.Nelem(); ss++)
      {
      // We will give it our best try...
      if(ary[ss].Remove() == false)
         br = false;
      }
   return br;
}

bool Platform::DirectoryCreate(const StdString& str)
{
   bool br = false;
   if(::mkdir(str.c_str(), S_IRWXU) == 0)
      br = true;
   return br;
}

} // stdnoj
