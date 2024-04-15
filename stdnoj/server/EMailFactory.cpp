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
#include <server/EMailFactory.hpp>
#include <server/EMailLines.hpp>

void _CleanFileName(StdString& str, bool bAllowPathChars = false)
{
str.mkwhite();
str.strip();
for(size_t ss = NULL; ss < str.length(); ss++)
   {
   switch(str[ss])
      {
      case '\\':
         if(bAllowPathChars == true)
            continue;
      case ':':
         if((bAllowPathChars == true) && ss == 1)
            continue;
      case '<':
      case '>':
      case '*':
      case '?':
      case '/':
      case '\"':
         str[ss] = '_';
      default:
         continue;
      }
   }
}


bool EMailFactory::IsEMailFactoryFile(const File& file)
   {
   EMailHeader msgResult;
   return msgResult.Import((File&)file);
   }

bool EMailFactory::Load(const File& fff, EMailMessage& msgResult)
   {
   return msgResult.Load(fff);
   }

bool EMailFactory::Save(const File& fff, EMailMessage& msgResult)
   {
   return msgResult.Export(fff);
   }

bool EMailFactory::Export(const File& fff, EMailMessage& msgResult)       // binary format
   {
   return msgResult.Export(fff);
   }

bool EMailFactory::Import(const File& fff, EMailMessage& msgResult)       // binary format
   {
   return msgResult.Import(fff);
   }


bool EMailFactory::Import(const File &fff, EMailHeader& header)
   {
   File file = fff;
   if(!file.Exists())
      return false;

   bool br = header.rfc_read(file.OpenRead(File::AT_BINARY));

   // No matter what the result is, this is what we were asked to read;
   // header.SetLocalName(file.Name());
   return br;
   }

bool EMailFactory::Export(const File& fff, EMailHeader& header)
   {
   return header.Export((File&)fff);
   }
bool EMailFactory::RemoveTerminalDot(EMailMessage& emb)
   {
   StdString str = emb.Body();
   size_t pos = str.rfind("\r\n.");
   if(pos == str.length() - 4)
      {
      str.remove(pos);
      emb.Body(str);
      return true;
      }
   return false;
   }
void EMailFactory::EncodeDots(EMailMessage& emb)
   {
   StdString str = emb.Body();
   if(str.replace("\r\n.\r\n", "\r\n..\r\n"))
      emb.Body(str);
   }
void EMailFactory::DecodeDots(EMailMessage& emb)
   {
   StdString str = emb.Body();
   if(str.replace("\r\n..\r\n", "\r\n.\r\n"))
      emb.Body(str);
   }
bool EMailFactory::MkLogicalName(File& file, const EMailHeader& emh, const StdString& sExtension, bool TryMessageID)
   {
   StdString str;

   if(TryMessageID)
      {
      str = emh.MessageID();      // THIS IS OUR LEGACY. USED TO BE THE DEFAULT.
      str.mkwhite();              // OBSERVED
      str.strip();
      }

   // If there is NO message id (i.e. if it is outgoing EMailMessage)
   if(str.is_null() || (str.length() <= 2))
      {
      // If there is a single recipient, then try to use that as the root name
      str = emh.To();
      if(str.find(';') != NPOS || str.find(',') != NPOS)
         str = emh.Subject(); // else go with the subject -

      // If neither single user subject (very unusual!)
      if(str.is_null() || str.is_space())
         {
         StdString scmp;
         str = "Posted on ";
         scmp = emh.Date();
         if(scmp.is_null())
            str.append("unknown date");
         else
            str.append(scmp);
         str.append(" from ");
         scmp = emh.From();
         if(scmp.is_null())
            {
            // This may look nuts, but it happens!
            scmp = emh.InternetPath();
            if(scmp.is_null() || scmp.is_space())
               str.append("unknown source");
            else
               str.append(scmp);
            }
         else
            str.append(scmp);
         }
      }

   _CleanFileName(str);

   if(str.ends_with(sExtension) == false)
      str.append(sExtension);

   // Validate the NEW file name -
   Directory dir;
   file.Query(dir);
   if(file.Name(str) == false)
      return false;
   dir.Home(file);
   return true;
   }

// Like MkLocalName, but favors the string provided (emh.Subject(), etc.)
File EMailFactory::MkLocalName(const Directory& dir, const StdString& sNodeRoot, const StdString& sExtension)
   {
   StdString sFileRoot = sNodeRoot;
   sFileRoot += sExtension;
   _CleanFileName(sFileRoot);

   File file;
   dir.Home(file);

   if(sFileRoot.is_null())
      {
      file.MkUniqueName(dir, sExtension); // Failsafe works with the extension -
      return file;
      }

   if(file.Name((Directory&)dir, sFileRoot) == false)
      {
      file.MkUniqueName(dir, sExtension); // Failsafe works with the extension -
      return file;
      }

   // NO HEADER TO INSPECT
   // Rule here is that if what we have already exists, then we use a number series -
   return File::MkGenerationName(dir, file);
   }

File EMailFactory::MkLocalName(const Directory& dir, EMailHeader& em, const StdString& sExtension)
   {
   File file;
   dir.Home(file);
   // First try to make a meaningfull name from the message header -
   // Try the recipient if only one - else the subject - first -
   if(MkLogicalName(file, em, sExtension, false) == false)
      {
      // Next, try using that nasty 'ol message id -
      if(MkLogicalName(file, em, sExtension, true) == false)
         {
         // Finally, try the file's own posix-based ability -
         dir.Home(file);
         if(file.MkUniqueName(dir, sExtension) == true)
            return file;
         }
      }

   // MkLogicalName gave us a unique name - or not - Go with what we have ...

   // Does what we have already exist?
   unsigned iCount = 0;
   File filePre = file; // the base name
   while(file.Exists())
      {
      // STEP: We have a file with that name - Is it the same message?
      EMailHeader emh;
      bool br = emh.Import(file);
      if(br == true)
         {
         // IF the header is the same, then the content is the same .... SO
         // IF the header is the same, then the file name should be the same, too -
         if(em == emh)
            return file;
         }

      // STEP: Not the same message - Lets keep going with the standard prefixes -
      iCount++;
      if(!iCount) // wrap - not likley - but all the same - could happen - some day -
         {
         dir.Home(file);
         file.MkUniqueName(dir, sExtension); // Failsafe works with the extension -
         return file;
         }
      stringstream srm;
      srm << iCount << "." << filePre.QueryNode();
      file.Name(PRESTO(srm));
      dir.Home(file);
      }
   return file;
   }
bool EMailFactory::IsAddressSeparator(char ch)
   {
   switch(ch)
      {
      case '<':
      case '>':
      case ' ':
      case '\t':
      case '\n':
      case '\r':
      case ',':
      case ';':   // Microsoft does this...
         return true;
      }
   return false;
   }
bool EMailFactory::AbsEmailAddress(StdString& str)
   {
   // It happens that people have newlines and other unspeakable
   // nasties in their EMailMessage addresses. Our job is simply to make
   // the best guess here;
   str.mkwhite();

   // Okay: Now we can assume a somewhat tangendental reality;
   size_t whence = str.find('@');
   if(whence == NPOS)
      return false;

   size_t start = whence;
   while(start)
      {
      if(IsAddressSeparator(str[start]) == true)
         {
         start++;
         break;
         }
      start--;
      }
   str = &str[start];   // save start point

   size_t end = str.find('@');
   while(end < str.length())
      {
      if(IsAddressSeparator(str[end]) == true)
         {
         str.remove(end);     // terminate (atypical)
         break;
         }
      end++;
      }
   return true;
   }
bool EMailFactory::FormatAsAddress(StdString& str)
   {
   StdString sHold = str;
   if(AbsEmailAddress(str) == false)
      {
      str = sHold;
      return false;
      }
   if(str.find('<') == NPOS)     // maintenance
      {
      str.prepend('<');
      str.append('>');
      }
   return true;
   }
bool EMailFactory::ContainsAbsEmailAddress(const StdString& sIn)
   {
   StdString sOut = sIn;
   return AbsEmailAddress(sOut);
   }
bool EMailFactory::AbsEmailAddress(const StdString& sIn, StdString& sOut)
   {
   sOut = sIn;
   return AbsEmailAddress(sOut);
   /*
   sOut = "";
   StdString str = sIn;
   str.mkwhite();
   str.strip();
   size_t which = str.rfind(' ');
   if(which != NPOS)
      {
      str = &str[which+1];
      str.strip();
      }

   sOut = str;
   if(sOut.find('@') == NPOS)
      return false;
   if(sOut.find('.') == NPOS)
      return false;

   return true;
   */
   }
/*
bool EMailFactory::List(const EMailMessage& emm, Array<StdString>& aLines)
   {
   aLines.Empty();
   EMailLines eml;
   eml.Assign(emm);
   StdString str;
   bool br = eml.FirstLine(str);
   while(br)
      {
      aLines.Append(str);
      br = eml.NextLine(str);
      }
   return true;
   }
bool EMailFactory::ListBody(const EMailMessage& EMailMessage, Array<StdString>& aLines)
   {
   aLines.Empty();
   EMailLines eml;
   eml.Assign(EMailMessage.Body());
   StdString str;
   bool br = eml.FirstLine(str);
   while(br)
      {
      aLines.Append(str);
      br = eml.NextLine(str);
      }
   return true;
   }
*/
bool EMailFactory::OmniLoad(const File& file, EMailHeader& emx)
   {
   EMailHeader emh;
   if(emh.Import((File&)file) == false)
      return false;
   emx = emh;
   return true;
   }
bool EMailFactory::OmniLoad(const File& file, EMailMessage& emx)
   {
   EMailMessage emh;
   if(emh.Load(file) == false && emh.Import(file) == false)
      return false;
   emx = emh;
   return true;
   }

