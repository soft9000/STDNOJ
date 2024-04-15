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
#include <cstdio> // unlink()

#include <sys/stat.h>
#include <fstream>
#include <ctime>             // time_t

#ifdef UNIX
#include <unistd.h>
#endif

#ifdef WIN32
#define STRICT
#include <windows.h>
#include <shellapi.h>// shellexecute()
#endif

const int HUGE_PATH = 1024;

inline bool TOBOOL(BOOL a)
   {
   if(a == TRUE)
      return true;
   return false;
   }

namespace stdnoj
{

bool Platform::ShellExec(const StdString& sUrl, const StdString& str) 
{
if(int(ShellExecute(NULL, sUrl.c_str(), str.c_str(), NULL, NULL, SW_SHOWNORMAL)) <= 32)
   return false;
return true;
}

bool Platform::ShellExec(const StdString& sUrl) 
{
if(int(ShellExecute(NULL, "open", sUrl.c_str(), NULL, NULL, SW_SHOWNORMAL)) <= 32)
   return false;
return true;
}

bool Platform::GetPwd(StdString& str) 
{
   bool br = false;
   char *pBuf = new char[MAX_PATH + 1];
   if(GetCurrentDirectory(MAX_PATH, pBuf))
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
   if(GetCurrentDirectory(MAX_PATH, pBuf))
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
   if(::SetCurrentDirectory(sDir.c_str()))
      br = true;
   std.remove(sz);
   return br;
}

bool Platform::DirectoryChange(const StdString& sDir) 
{
   if(::SetCurrentDirectory(sDir.c_str()))
      return true;
   return false;
}

bool Platform::DirectoryExists(const StdString& sDir) 
{
   bool br = false;
   StdString std;
   PushPwd(std);
   if(::SetCurrentDirectory(sDir.c_str()))
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
   if(::MoveFile(pszFrom.c_str(), pszTo.c_str()))
      return true;
   return false;
}

bool Platform::FileHide(const StdString& sFileName)
{
   bool br = false;
   HANDLE hFile = ::CreateFile(sFileName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if(hFile == INVALID_HANDLE_VALUE)
      return br;

   DWORD dw = ::GetFileAttributes(sFileName.c_str());
   if(dw == 0xFFFFFFFF)
      return br;

   dw |= FILE_ATTRIBUTE_HIDDEN;

   br = false;
   if(::SetFileAttributes(sFileName.c_str(), dw))
      br = true;

   ::CloseHandle(hFile);
   return br;
}

bool Platform::FileShow(const StdString& sFileName)
{
   bool br = false;
   HANDLE hFile = ::CreateFile(sFileName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if(hFile == INVALID_HANDLE_VALUE)
      return br;

   DWORD dw = ::GetFileAttributes(sFileName.c_str());
   if(dw == 0xFFFFFFFF)
      return br;

   dw ^= FILE_ATTRIBUTE_HIDDEN;

   br = false;
   if(::SetFileAttributes(sFileName.c_str(), dw))
      br = true;

   ::CloseHandle(hFile);
   return br;
}

bool Platform::IsFileHidden(const StdString& sFileName)
{
   Node node;
   FilePath::Open(sFileName, node);
   return node.IsHidden();
}

bool Platform::Touch(const StdString& sFileName)
{
   SYSTEMTIME st;
   ::GetSystemTime(&st);
   FILETIME ft;
   if(::SystemTimeToFileTime(&st, &ft) == false)
      return false;

   HANDLE hFile = ::CreateFile(sFileName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
      NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if(hFile == INVALID_HANDLE_VALUE)
      return false;

   if(::SetFileTime(hFile, &ft, &ft, &ft) == false)
      {
      ::CloseHandle(hFile);
      return false;
      }
   ::CloseHandle(hFile);
   return true;
}

bool Platform::FileCopy(const StdString& strFrom, const StdString& strTo)
{
   // file is over written if it already exists
   if(::CopyFile(strFrom.c_str(), strTo.c_str(), FALSE))
      return true;
   return false;
}

bool Platform::FileReadable(const StdString& str)
{
   Node node;
   FilePath::Open(str, node);
   return node.IsRead();
}

bool Platform::FileWritable(const StdString& str)
{
   Node node;
   FilePath::Open(str, node);
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
   if(::RemoveDirectory(str.c_str()))
      return true;
   return false;
}

bool Platform::DirectoryEmpty(const StdString& sPwd) 
{
   bool br = true;
   Array<File> ary;
   FileSystem::Query(sPwd, ary);
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
// Copied this from my Archive class;
   bool br = false;
   char *pBuf = new char[MAX_PATH];
   if(::GetCurrentDirectory(MAX_PATH, pBuf))
      {
      StdString sDest = str;     // a StdString that we can "eat".
      long sp = sDest.find('\\');
      if(sDest[1] == ':')
         sp = sDest.find('\\');

      while(size_t(sp) != NPOS)
         {
         StdString sTrunk;
         sDest.copy_pos(sTrunk, 0, sp);

         if(::SetCurrentDirectory(sTrunk.c_str()))
            br = true;
         else
            br = false;

         if(br == false)
            {
            br = TOBOOL(::CreateDirectory(sTrunk.c_str(), NULL));
            }

         if(br == false)
            goto DONE_IT1333;     // Could only be too deep, or an access violation.
         sp = (size_t)sDest.find('\\', size_t(sp)+1);
         }
      br = TOBOOL(::CreateDirectory(sDest.c_str(), NULL));
      }
   DONE_IT1333:
   ::SetCurrentDirectory(pBuf);
   delete [] pBuf;
   return br;
}

} // stdnoj
