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

#include <stdnoj.hpp>

namespace stdnoj
   {

FileInformation::FileInformation(void)
   {
//   ::memset(&ofData, 0, sizeof(BY_HANDLE_FILE_INFORMATION));
   sFileName.assign("?");
   }

FileInformation::FileInformation(const FileInformation& fi)
   {
//   ::memset(&ofData, 0, sizeof(BY_HANDLE_FILE_INFORMATION));
   sFileName.assign("?");
   *this = fi;
   }

bool FileInformation::IsDirectory(void) const
   {
//   if(ofData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//      return true;
   return false;
   }

FileInformation& FileInformation::operator=(const FileInformation& fi)
   {
   if(this == &fi)
      return *this;
   sFileName = fi.sFileName;
//   memcpy((BYTE *)&ofData, (BYTE *)&fi.ofData, sizeof(BY_HANDLE_FILE_INFORMATION));
   return *this;
   }

FileInformation::~FileInformation(void)
   {
   }

const char *FileInformation::Name(void) const
   {
   return sFileName.c_str();
   }

bool FileInformation::Update(void)
   {
   StdString sHold = Name();     // so `self` mess won't happen...
   return Get(sHold);
   }

bool FileInformation::Get(const StdString& std)
   {
/*
   // 5/22/2001: Changed to accept read-only, as well as other type of files, R. Nagy
   HANDLE hFile = ::CreateFile(std.c_str(),
      0,    // 5/8/2003 BUGFIX: WE ONLY WANT INODE ACCESS => DO NOT USE "GENERIC_READ" or "GENRRIC_WRITE". FAILS ON IN-USE FILES.
      FILE_SHARE_READ,
      NULL,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_READONLY, // FILE_ATTRIBUTE_NORMAL (only valid when used alone)
      NULL);
   if(hFile == INVALID_HANDLE_VALUE)
      {
#if 0
      StdString str;
      GetLastErrorString(str);
#endif
      return false;
      }

   if(::GetFileInformationByHandle(hFile, &ofData) == false)
      {
      ::CloseHandle(hFile);
      return false;
      }

   ::CloseHandle(hFile);
*/
   sFileName = std;
   return true;
   }

bool FileInformation::Put(void)
   {
   return Put(sFileName);
   }

bool FileInformation::Put(const StdString& std)
   {
/*
// <<<WIP>>> For some crazy reason, this fails on OTHER people's machine! I can't get it to fail om my
// <<<WIP>>> machine: Reports indicate that is is a problem with ATTRIBUTES, rather than FILETIMES, since
// <<<WIP>>> later seem to come out okay when run remotely (don't know about former)...
   HANDLE hFile = ::CreateFile(std.c_str(),
      GENERIC_READ      | GENERIC_WRITE,
      FILE_SHARE_READ   | FILE_SHARE_WRITE,
      NULL,
      OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL,
      NULL);
   if(hFile == INVALID_HANDLE_VALUE)
      return false;

   if(::SetFileTime(hFile, &ofData.ftCreationTime, &ofData.ftLastAccessTime, &ofData.ftLastWriteTime) == false)
      {
      ::CloseHandle(hFile);
      return false;
      }
   ::CloseHandle(hFile);

   // ::GetFileAttributes()
   if(::SetFileAttributes(sFileName.c_str(), ofData.dwFileAttributes) == false)
      return false;
*/
   return true;
   }

ostream& FileInformation::write(ostream& os)
   {
   if(!os)
      return os;
   sFileName.write_stream(os);
//   os.write((BYTE *)&ofData, sizeof(BY_HANDLE_FILE_INFORMATION));
   return os;
   }

istream& FileInformation::read(istream& is)
   {
   if(!is)
      return is;
   sFileName.read_stream(is);
//   is.read((BYTE *)&ofData, sizeof(BY_HANDLE_FILE_INFORMATION));
   return is;
   }


const char *FileInformation::CreationTime(StdString& str)
   {
   return 0L;//FormatTime(ofData.ftCreationTime, str);
   }


const char *FileInformation::WriteTime(StdString& str)
   {
   return 0L;//FormatTime(ofData.ftLastWriteTime, str);
   }


const char *FileInformation::ReadTime(StdString& str)
   {
   return 0L;//FormatTime(ofData.ftLastAccessTime, str);
   }

#if 0

const char *FileInformation::FormatTime(const FILETIME& ff, StdString& sResult)
   {
   sResult = "File time conversion error";
/*
   FILETIME ft;
   if(::FileTimeToLocalFileTime(&ff, &ft) == false)
      return sResult.c_str();
   SYSTEMTIME st;
   if(::FileTimeToSystemTime(&ft, &st) == false)
      return sResult.c_str();
   char buf[40];
   sprintf(buf, "%04d/%02d/%02d, %02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
*/
   sResult = buf;
   return sResult.c_str();
   }
#endif

bool   FileInformation::IsFileHuge(void)
   {
/*
   if(ofData.nFileSizeHigh)
      return true;
*/
   return false;
   }

size_t FileInformation::FileSize(void)
   {
/*
   return size_t(ofData.nFileSizeLow);
*/
   return 0L;
   }

   } // stdnoj
