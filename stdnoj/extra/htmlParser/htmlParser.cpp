#include <htmlParser.hpp>


htmlParser::htmlParser(void) : sToken("<"), eToken(">")
{
}


bool htmlParser::snarfit(istream& is, StdString& sResult, long spStart, long spEnd)
   {
   sResult = "";

   if(spStart == (long) NPOS)
      return false;

   if(spEnd == (long) NPOS)
      return false;

   if(spEnd > spStart)
      {
      size_t szTotal = spEnd - spStart;
      is.seekg(spStart);
      char *buf = new char[szTotal+1];
      is.read(buf, szTotal);
      buf[szTotal] = NULL;
      sResult.own(buf);
      return true;
      }
   return false;
   }
   

bool htmlParser::StreamTo(const char ch, istream& is)
   {
   char ch0 = NULL;
   do
      {
      is.read((char *)&ch0, 1);
      } while(is && (ch0 != ch));
   if(is)
      {
      is.putback(ch0);
      return true;
      }
   return false;
   }


void htmlParser::DumpToAppend(const char ch, istream& is, StdString& std)
   {
   char ch0 = NULL;
   while(1)
      {
      is.read((char *)&ch0, 1);
      if(!is)
         return;
      if(ch0 == ch)
         return;
      std.append(ch0);
      }
   }


// MarkStartStreamPast returns the START position of sKey, but streams past it;
bool htmlParser::MarkStartStreamPast(istream& is, const StdString& sKey0, long& st1)
   {
   if(!is)
      return false;

   // Always upper-case;
   StdString sKey = sKey0;
   sKey.to_upper();

   long st0 = is.tellg();
   size_t count = NULL;

   st1 = NULL; char ch = NULL; char chcmp = NULL; 
   do
      {
      // Get the actual character;
      is.read((char *)&ch, 1);

      // Compare to upper-case;
      chcmp = ch;
      if(chcmp >= 'a' && (chcmp <= 'z'))
         chcmp = ::toupper(ch);     // BUGFIX 4/12/2003

      // Compare;
      if(sKey[count] == chcmp)
         {
         if(!count)
            st1 = (long)is.tellg() -1;
         count++;
         if(count == sKey.length())
         return true;
         }
      else
         count = 0;
      } while(is);

   // Nope, we got to the EOF and it was not there;
   return false;
   }


bool htmlParser::ExtractTag(const StdString& s, htmlTag& sResult)
   {
   stringstream srm;
   srm << s.c_str();
   srm << ends;
   return ExtractTag(srm, sResult);
   }


bool htmlParser::ExtractTag(istream& is, htmlTag& sResult)
   {
   BEGIN
   htmlTag blanker;
   sResult = blanker;
   END

   if(!is)
      return false;

   long sp = is.tellg();
   if(StreamTo(sToken[0], is) == false)
      {
      is.seekg(sp);
      return false;
      }

   sp = is.tellg();
   DumpToAppend(eToken[0], is, sResult.sStartTag);
   if(sResult.sStartTag.is_null())
      return false;
   sResult.stStart = sp;      

   int stop = 0;
   for(size_t ss = 1; ss < sResult.sStartTag.length(); ss++)
      {
      switch(sResult.sStartTag[ss])
         {
         case ' ':
         case '\t':
         case '\n':
         case '\r':
         case '>':
         case '<':
            stop = 1;
            break;
         default:
            sResult.sKey.append(sResult.sStartTag[ss]);
         }
      if(stop)
         break;
      }
   if(sResult.sKey.length())
      return true;

   return false;
   }


bool htmlParser::ExtractBlock(const StdString& sKey, const htmlBlock& sIn, htmlBlock& sResult)
   {
   /*
   TDS
   stringstream srm;
   srm << sIn.sBodyContent;
   srm << ends;
   return ExtractBlock(sKey, srm, sResult);
   */
   File  temp;
   temp.MkUniqueName();
   ostream& os = temp.OpenWrite(File::AT_BINARY);
   os << sIn.sBodyContent;
   temp.Close();
   bool br = ExtractBlock(sKey, temp.OpenRead(File::AT_BINARY), sResult);
   temp.Close();
   temp.Remove();
   return br;
   }


bool htmlParser::ExtractBlock(const StdString& sKey, istream& is, htmlBlock& sResult)
   {
#if 1
   // 09/16/2003: <<<WIP>>> Broke this today when we removed the inheritance
   // dependancy for htmlParser on htmlTag to hold the START position. Should
   // be as easy as mapping the tagStart and tagEnd over to where we need to.
   //
   // Need to fix this some other time, but I do not feel up to it at the moment...
   //
   return false;
#else
   bool br = false;
   StdString std;

   BEGIN
   htmlBlock hBlank;
   sResult = hBlank;
   END

   if(!is)
      return false;

   sResult.sKey = sKey;

   // *****
   // Locate the HTML opener (start-tag);
   sResult.sStartTag = sToken;
   sResult.sStartTag.append(sKey);
   if(MarkStartStreamPast(is, sResult.sStartTag, sResult.stStart) == false)
      return br;


   if(!is)
      return false;

   // *****
   // Finish-up the rest of the start-tag;
   DumpToAppend(eToken[0], is, sResult.sStartTag);


   if(!is)
      return false;

   // *****
   // Locate the HTML closer (end-tag);
   sResult.sEndTag = sToken;
   sResult.sEndTag.append("/");
   sResult.sEndTag.append(sKey);
   sResult.sEndTag.append(eToken);
   if(MarkStartStreamPast(is, sResult.sEndTag, sResult.stEnd) == false)
      return br;

   if(!is)
      return false;

   // Fetch, ubu, fetch;
   return 
      snarfit(is, sResult.sBodyContent, (sResult.stStart += (long)sResult.sStartTag.length()), (long)sResult.stEnd -1);
#endif
   }


bool htmlParser::ExtractBlocks(const StdString& sKey, const htmlBlock& sIn, Array<htmlBlock>& ary)
   {
   File  temp;
   temp.MkUniqueName();
   ostream& os = temp.OpenWrite(File::AT_BINARY);
   os << sIn.sBodyContent;
   temp.Close();

   bool br = false;
   bool bOk= false;
   htmlBlock hResult;
   istream& is = temp.OpenRead(File::AT_BINARY);
   do
      {
      br = ExtractBlock(sKey, is, hResult);
      if(br == true)
         {
         ary[ary.Nelem()] = hResult;
         bOk = true;
         }
      } while(is && (br == true));

   temp.Close();
   temp.Remove();
   return bOk;
   }



bool ConvertToStrings(const StdString& sPat, const StdString& sBlock, Array<StdString>& aRes)
{
StdString sWip = sBlock;
size_t sp = NULL;
while (sp != NPOS)
   {
   sp = sWip.find(sPat);
   if(sp != NPOS)
      {
      aRes[aRes.Nelem()] = sWip.substr(0, sp);
      sWip.remove_bytes(0, sp + sPat.length());
      }
   }
if(!sWip.is_null())
   aRes[aRes.Nelem()] = sWip;
return true;
}

StdString _ZapHtml(const StdString& sBlock, const char ch1, const char ch2)
{
bool bSkip = false;
StdString sResult;
for(size_t ss = NULL; ss < sBlock.length(); ss++)
   {
   if(sBlock[ss] == ch1)
      {
      bSkip = true;
      continue;
      }
   if(sBlock[ss] == ch2)
      {
      bSkip = false;
      continue;
      }
   if(bSkip == false)
      {
      sResult.append(sBlock[ss]);
      continue;
      }
   }
return sResult;
}

StdString HtmlTo7Bit(const StdString& str)
{
/* TODO 1 -oYo -cHTML :
Need to finally create *the definitive* set of HTML character codes
for 8-bit (ONLY). Only do symbolic. Will do the decimal encodings
later */
return _ZapHtml(str, '&', ';');      // <<<WIP>>>
}

StdString ZapHtml(const StdString& sBlock, const char ch1, const char ch2)
{
StdString sResult = _ZapHtml(sBlock, ch1, ch2);
sResult.mkwhite();
sResult = HtmlTo7Bit(sResult);
return sResult;
}
