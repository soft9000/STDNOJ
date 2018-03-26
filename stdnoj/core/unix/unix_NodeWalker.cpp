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

THIS CLASS IS OBSOLETE. USE IT TO COMPLETE THE FILE AND DIRECTORY CLASSES, THEN DELETE IT.

#include <stdnoj.hpp>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <fnmatch.h>
namespace stdnoj
   {

bool NodeWalker::GetDirectory(StdString& sDir)
   {
   char buf[PATH_MAX+2];
   if(::getcwd(&buf[0], PATH_MAX) != 0)
      {
      sDir = &buf[0];
      return true;
      }
   return false;
   }
bool NodeWalker::SetDirectory(const StdString& sDir)
   {
   if(::chdir(sDir.c_str()) == 0)
      return true;
   return false;
   }
bool NodeWalker::PushPwd(StdString& std)
   {
   bool br = false;
   StdString sBuf;
   if(GetDirectory(sBuf))
      {
      std.append(";");
      std.append(sBuf);
      br = true;
      }
   return br;
   }
bool NodeWalker::PopPwd(StdString& std)
   {
   bool br = false;
   size_t sz = std.rfind(";");
   if(sz == NPOS)
      return false;
   StdString sDir;
   std.copy_pos(sDir, sz+1, std.length());
   if(SetDirectory(sDir.c_str()))
      br = true;
   std.remove(sz);
   return br;
   }
bool NodeWalker::FirstNode(const StdString& sDir, Node& node)
   {
   pHandle = (void *)NULL;

   // STEP: Open the directory
   DIR *pHandle   = ::opendir(sDir.c_str());
   if(!pHandle)
      return false;
   pHandle = pHandle;

   return NextNode(node);
   }
bool NodeWalker::NextNode(Node& node)
   {
   // STEP: Get the name of a node
   struct dirent *pEnt = readdir((DIR *)pHandle);
   if(!pEnt)
      return false;

   // STEP: Open the node
   StdString sNode = pEnt->d_name;
   FilePath fp;
   if(fp.Open(sNode, node) == false)
      return false;
   return true;
   }
void NodeWalker::LastNode(void)
   {
   ::closedir((DIR *)pHandle);
   }

   } // stdnoj
