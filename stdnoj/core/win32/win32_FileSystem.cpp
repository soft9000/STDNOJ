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
//#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <dirent.h>
//#include <fnmatch.h>
//
// 02/15/2008: Filled in a few of the more important member functions using common object implmentations, R. Nagy
//
/* TODO : Portions of FileSystem still need to be implemented. */

#define STRICT
#include <windows.h>

namespace stdnoj
   {

size_t FileSystem::Query(const StdString& path, Array<Node>& aResult)
   {
   aResult.Empty();
   Directory dir;
   dir.Push();
   if(dir.Set(path) == false)
      {
      dir.Pop();
      return aResult.Nelem();
      }
   aResult.AddZombies(Nelem(path));
   WIN32_FIND_DATA ffd;
   HANDLE hRes = FindFirstFile("*", &ffd);
   if(hRes == INVALID_HANDLE_VALUE)
      {
      // We should at LEAST have a "dot" and a "dot dot";
      return aResult.Nelem();
      }

   Node object;
   bool iDone = false;
   while(iDone == false)
      {
      StdString sNodeName = &ffd.cFileName[0];
      if((sNodeName != ".") && (sNodeName != ".."))
         {
         // normal assignment:
         StdString sNodeName = path;
         (sNodeName += '\\') += &ffd.cFileName[0];
         FilePath::Open(sNodeName, object);
         aResult[aResult.Nelem()] = object;
         }
      // Find the next file:
      if(FindNextFile(hRes, &ffd) == FALSE)
         {
         // normal exit
         FindClose(hRes);
         iDone = true;
         }
      }
   dir.Pop();
   return aResult.Nelem();
   }
size_t FileSystem::Query(const StdString& path, Array<File>& aResult)
   {
   aResult.Empty();
   Array<Node> ary;
   Query(path, ary);
   aResult.AddZombies(ary.Nelem() + 1);   // slight speed-up
   for(size_t ss = 0L; ss < ary.Nelem(); ss++)
      {
      if(ary[ss].IsDir() == false)
         {
         File file;
         FilePath::Open(ary[ss].GetFullName('\\'), file);
         aResult[aResult.Nelem()] = file;
         }
      }
   return aResult.Nelem();
   }
size_t FileSystem::Query(const StdString& path, Array<Directory>& aResult)
   {
   aResult.Empty();
   Array<Node> ary;
   Query(path, ary);
   aResult.AddZombies(ary.Nelem() + 1);   // slight speed-up
   for(size_t ss = 0L; ss < ary.Nelem(); ss++)
      {
      if(ary[ss].IsDir() == true)
         {
         Directory dir;
         FilePath::Open(ary[ss].GetFullName('\\'), dir);
         aResult[aResult.Nelem()] = dir;
         }
      }
   return aResult.Nelem();
   }
size_t FileSystem::Nelem(const StdString& path)
   {
   int iCount = 0L;
   Directory dir;
   dir.Push();
   if(dir.Set(path) == false)
      {
      dir.Pop();
      return iCount;
      }
   WIN32_FIND_DATA ffd;
   HANDLE hRes = FindFirstFile("*", &ffd);
   if(hRes == INVALID_HANDLE_VALUE)
      {
      // We should at LEAST have a "dot" and a "dot dot";
      return iCount;
      }

   bool iDone = false;
   while(iDone == false)
      {
      StdString sNodeName = &ffd.cFileName[0];
      if((sNodeName != ".") && (sNodeName != ".."))
         iCount++;
      // Find the next file:
      if(FindNextFile(hRes, &ffd) == FALSE)
         {
         // normal exit
         FindClose(hRes);
         iDone = true;
         }
      }
   dir.Pop();
   return iCount;
   }
bool FileSystem::Create(Node& node)
   {
   FilePath fp;
   if(node.IsFile())
      {
      File file;
      if(file.Name(node.GetFullName(fp.PathChar())) == false)
         return false;
      return Create(file);
      }
   else
      {
      Directory dir;
      if(dir.Name(node.GetFullName(fp.PathChar())) == false)
         return false;
      return Create(dir);
      }
   }
bool FileSystem::Create(File& file)
   {
   return file.Create();
   }
bool FileSystem::Create(Directory& dir)
   {
   return dir.Create();
   }
bool FileSystem::Delete(Node& node)
   {
   FilePath fp;
   if(node.IsFile())
      {
      File file;
      if(file.Name(node.GetFullName(fp.PathChar())) == false)
         return false;
      return Delete(file);
      }
   else
      {
      Directory dir;
      if(dir.Name(node.GetFullName(fp.PathChar())) == false)
         return false;
      return Delete(dir);
      }
   }
bool FileSystem::Delete(File& file)
   {
   return file.Remove();
   }
bool FileSystem::Delete(Directory& dir)
   {
   return dir.Remove();
   }
PortTime FileSystem::Query(const Node& node)
   {
/* <<<WIP>>> */
   PortTime pt;
   return pt;
   }
PortTime FileSystem::Query(const File& file)
   {
/* <<<WIP>>> */
   PortTime pt;
   return pt;
   }
PortTime FileSystem::Query(const Directory& dir)
   {
/* <<<WIP>>> */
   PortTime pt;
   return pt;
   }
bool FileSystem::Touch(Node& node, PortTime& pt)
   {
/* <<<WIP>>> */
   return false;
   }
bool FileSystem::Touch(File& file, PortTime& pt)
   {
/* <<<WIP>>> */
   return false;
   }
bool FileSystem::Touch(Directory& dir, PortTime& pt)
   {
/* <<<WIP>>> */
   return false;
   }

   } // stdnoj
