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
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <fnmatch.h>

/* TODO : FileSystem needs implemented.  WIN32 version is more complete. Can borrow much from there... */

class GetNodes : public stdnoj::NodeReport
   {
   };

namespace stdnoj
   {

size_t FileSystem::Query(const StdString& path, Array<Node>& aResult)
   {
   NodeFinder ff;
   return 0L;
   }
size_t FileSystem::Query(const StdString& path, Array<File>& aResult)
   {
   return 0L;
   }
size_t FileSystem::Query(const StdString& path, Array<Directory>& aResult)
   {
   return 0L;
   }
size_t FileSystem::Nelem(const StdString& path)
   {
   return 0L;
   }
bool FileSystem::Create(Node& node)
   {
   return false;
   }
bool FileSystem::Create(File& file)
   {
   return false;
   }
bool FileSystem::Create(Directory& dir)
   {
   return false;
   }
bool FileSystem::Delete(Node& node)
   {
   return false;
   }
bool FileSystem::Delete(File& file)
   {
   return false;
   }
bool FileSystem::Delete(Directory& dir)
   {
   return false;
   }
PortTime FileSystem::Query(const Node& node)
   {
   PortTime pt;
   return pt;
   }
PortTime FileSystem::Query(const File& file)
   {
   PortTime pt;
   return pt;
   }
PortTime FileSystem::Query(const Directory& dir)
   {
   PortTime pt;
   return pt;
   }
bool FileSystem::Touch(Node& node, PortTime& pt)
   {
   return false;
   }
bool FileSystem::Touch(File& file, PortTime& pt)
   {
   return false;
   }
bool FileSystem::Touch(Directory& dir, PortTime& pt)
   {
   return false;
   }

   } // stdnoj
