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
#include <core/ProgramId.hpp>
#define STRICT
#include <windows.h>

#define BEGIN {
#define END   }

using namespace stdnoj;

class _pid32 {
   public:
      static bool GetTemp(Directory& dir)                ; // A place for temporary files
      static bool GetProgram(Directory& dir)             ; // File where the executable resides
      static bool GetProgram(File& file)                 ; // Fully qualified executable
      static bool GetSystem(Directory& dir)              ; // This is the WINDOWS (system) directory. On unix, /etc might be a good place. Must be an pre-exisitng operating system defined location, and non-transient.
// 12/03/2008 - Obsolete - Will remove later on, along with any other ground clutter
//    static bool PutInDirectory(const Directory& dir, File& file) ; // Place in the directory
//    static bool PutInDirectory(File& file)             ; // Place the file in the default (program) directory
//    static bool PutInDirectory(Directory& dir)         ; // Place the leaf in the default (program) directory
//    static bool GetTemp(File& file)                    ; // Create a unique temporary file
//    static bool PutInTemp(File& file)                  ; // Put the file into the temp folder
//    static bool PutInTemp(Directory& dir)              ; // Put the directory tree into the temp dir
//    static bool PutInSubDirectory(const Directory& dirSub, File& file) ;
   };


const char *FullName(StdString& str)
   {
   char *pszBuf = new char[MAX_PATH];
   ::GetModuleFileName(NULL, pszBuf, MAX_PATH);
   str = pszBuf;
   delete [] pszBuf;
   return str.c_str();
   }
const char *ProgramName(StdString& str)
   {
   char *pszBuf = new char[MAX_PATH];
   ::GetModuleFileName(NULL, pszBuf, MAX_PATH);
   str = pszBuf;
   delete [] pszBuf;
   size_t sz = str.rfind("\\");
   if(sz != npos)
      str.remove_pos(0, sz+1);
   return str.c_str();
   }
const char *PathTo(StdString& str)
   {
   char *pszBuf = new char[MAX_PATH];
   ::GetModuleFileName(NULL, pszBuf, MAX_PATH);
   str = pszBuf;
   delete [] pszBuf;
   size_t sz = str.rfind("\\");
   if(sz != npos)
      str.remove_pos(sz, str.length());
   return str.c_str();
   }
const char *PlaceInDirectory(StdString& str)
   {
   StdString str2;
   PathTo(str2);
   str2.append('\\');
   str2.append(str);
   str = str2;
   return str.c_str();
   }
bool SetProgramDirectory(void)
   {
   StdString sPath;
   if(::SetCurrentDirectory(PathTo(sPath)))
      return true;
   return false;
   }
const char *GetTempDirectory(StdString& str)
   {
   BEGIN
   char pBuf[MAX_PATH + 1];
   if(GetTempPath(MAX_PATH, pBuf) == NULL)
      FullName(str);    // What if we are on a network drive, or a CDROM?
   else
      str = pBuf;
   END

   // Now here is the directory that WE will use for THIS program's TEMP storage;
   BEGIN
   StdString str2;
   ProgramName(str2);
   str.append(str2);
   END
   str.append(".tmp");

   Directory dir;
   dir.Name(str);
   dir.CreateDirectoryTree(); // (sigh) Legacy - Assume it went okay...
   return str.c_str();
   }
const char *TempFileName(StdString& str)
   {
   GetTempDirectory(str);
   StdString sPwd;
   Directory dir1, dir2;
   dir1.Get();
   dir2.Set(str);
   /* Create a temporary filename for the current working directory: */
   StdString str2 = tmpnam(NULL);     // ANSI
   dir1.Set();
   str.append(str2);
   return str.c_str();
   }
const char *TempFileName(StdString& str, const StdString& sSuffix)
   {
   TempFileName(str);
   str.append(sSuffix);
   return str.c_str();
   }
const char *PlaceInTempDirectory(StdString& str)
   {
   StdString str2;
   GetTempDirectory(str2);
   str2.append('\\');
   str2.append(str);
   str = str2;
   return str.c_str();
   }
const char *PlaceInSubDirectory(const StdString& sSubdir, StdString& sItem)
   {
   StdString str2;
   PathTo(str2);
   str2.append('\\');
   str2.append(sSubdir);
   str2.append('\\');
   str2.append(sItem);
   sItem = str2;
   return sItem.c_str();
   }

bool QuerySystemDirectory(StdString& str)
   {
   char WinDir[144];
   GetWindowsDirectory(WinDir, sizeof(WinDir));
   str = &WinDir[0];
   return true;
   }


bool _pid32::GetTemp(Directory& dir)
   {
   StdString str;
   return dir.Name(GetTempDirectory(str));
   }
bool _pid32::GetProgram(Directory& dir)
   {
   StdString str;
   return dir.Name(PathTo(str));
   }
bool _pid32::GetProgram(File& file)
   {
   StdString str;
   return file.Name(FullName(str));
   }
bool _pid32::GetSystem(Directory& dir)
   {
   StdString str;
   QuerySystemDirectory(str);
   return dir.Name(str);
   }
/*
bool _pid32::PutInDirectory(File& file)
   {
   StdString str;
   file.QueryNode(str);
   return file.Name(PlaceInDirectory(str));
   }
bool _pid32::PutInDirectory(Directory& dir)
   {
   StdString str = dir.Name();
   return dir.Name(PlaceInDirectory(str));
   }
bool _pid32::PutInSubDirectory(const Directory& sSubdir, File& file)
   {
   StdString sName, sPath;
   FilePath::Chop(file.Name(), sPath, sName, '\\');
   sPath = sSubdir.Name();
   sPath += '\\';
   sPath += sName;
   return file.Name(sPath);
   }
bool _pid32::PutInTemp(File& file)
   {
   StdString str = file.Name();
   size_t whence = str.rfind('\\');
   if(whence != NPOS)
      str.remove_pos(0, whence+1);
   return file.Name(PlaceInTempDirectory(str));
   }
bool _pid32::PutInTemp(Directory& file)
   {
   StdString str = file.Name();
   size_t whence = str.rfind('\\');
   if(whence != NPOS)
      str.remove_pos(0, whence+1);
   return file.Name(PlaceInTempDirectory(str));
   }
bool _pid32::GetTemp(File& file)
   {
   StdString str;
   TempFileName(str);
   return file.Name(str);
   }
*/
