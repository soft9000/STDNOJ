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
// Products work off of a COMMON set of INI definitions.
#include <extra/ini/IniFile.hpp>
using namespace ini;

class IniSet
{
public:

   static bool AppendEpop(const File& fileIni)
      {
      IniFile fileINI(fileIni);
      fileINI.Load(); // does NOT have to A/E ...

      fileINI.Put("EPOP", "POP_Server",     "pop.yourhost.com");
      fileINI.Put("EPOP", "POP_Port",       "110");
      fileINI.Put("EPOP", "POP_User",       "USER you@yourhost.com");
      fileINI.Put("EPOP", "POP_Password",   "PASS your_pop_password");
      fileINI.Put("EPOP", "POP_Folder",     "c:\\MailBot01\\YourAccountFolder");
      fileINI.Put("EPOP", "POP_Debug",      "false");

      return fileINI.Save();
      }

   // Venn - Operational bridge between EPop and PagePub - For now!
   static bool AppendRssBot(const File& fileIni, bool bEpop = true)
      {
      if(bEpop == true)
         {
         if(AppendEpop(fileIni) == false)
            return false;
         }

      IniFile fileINI(fileIni);
      fileINI.Load(); // does NOT have to A/E ...

      fileINI.Put("RSS_FEED", "FTP_Server",      "ftp.yourhost.com");
      // fileINI.Put("RSS_FEED", "FTP_Port",       "26");
      fileINI.Put("RSS_FEED", "FTP_User",        "you@yourhost.com");
      fileINI.Put("RSS_FEED", "FTP_Password",    "your_ftp_password");
      fileINI.Put("RSS_FEED", "FTP_DestDir",     "/RSSRobot");
      fileINI.Put("RSS_FEED", "FTP_Debug",       "false");
      fileINI.Put("RSS_FEED", "FEED_Server",     "yourhost.com");
      fileINI.Put("RSS_FEED", "FEED_Title",      "Latest Information");
      fileINI.Put("RSS_FEED", "FEED_Description","The latest update for my web site.");

      return fileINI.Save();
      }

   static bool GetFtpServer(const File& fileIni, StdString& sResult)
      {
      IniFile fileINI(fileIni);
      if(fileINI.Load() == false)
         return false;
      return fileINI.Get("RSS_FEED", "FTP_Server", sResult);
      }

   static bool GetFtpUser(const File& fileIni, StdString& sResult)
      {
      IniFile fileINI(fileIni);
      if(fileINI.Load() == false)
         return false;
      return fileINI.Get("RSS_FEED", "FTP_User", sResult);
      }

   static bool GetFtpPassword(const File& fileIni, StdString& sResult)
      {
      IniFile fileINI(fileIni);
      if(fileINI.Load() == false)
         return false;
      return fileINI.Get("RSS_FEED", "FTP_Password", sResult);
      }

   static bool AppendPagePub(const File& fileIni)
      {
      if(AppendRssBot(fileIni, false) == false)
         return false;

      IniFile fileINI(fileIni);
      fileINI.Load(); // does NOT have to A/E ...

      // TBD: 
      fileINI.Put("PagePub", "RSS_File",       "NojStyle.rss");   // (example only)

      return fileINI.Save();
      }

};