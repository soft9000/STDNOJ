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
#include <extra/CsvParser.hpp>
//#include <presto.hpp>

const char PUSH_COMMA = (char)0xFE;
const char PUSH_QUOTE = (char)0xFF;

#define SINGLE_QUOTE '\"'

#define BEGIN  {
#define END    }

CsvParser::CsvParser(void) : pIfs(NULL)
   {
   }


CsvParser::~CsvParser(void)
   {
   delete pIfs;
   }

void CsvParser::NORMALIZE_COMMAS(StdString& std, size_t sz_start)
   {
   // This is illegal english, and un-desirable in our processing of CSV;
   size_t ss = std.find(" ,", sz_start);
   while(ss != NPOS)
      {
      // Swap 'em;
      std[ss] = ',';
      std[ss+1] = ' ';
      ss = std.find(" ,", sz_start);
      }
   }

void CsvParser::PUSH_LITERAL_QUOTES(StdString& std, size_t sz_start)
   {
   NORMALIZE_COMMAS(std, sz_start);

   size_t ss = std.find(SINGLE_QUOTE, sz_start);

   while(ss != NPOS)
      {
/*
//Observed:
//"Nature","Orthodoxy","Gilbert Keith","Chesterton","The only words that ever satisfied me as describing Nature are the terms used in fairy books, ""charm"", ""spell"", ""enchantment"". They�express the arbitrariness of the fact and its mystery."

      // DETECTING THE ASSININE (above):
      if(std[ss+1] == SINGLE_QUOTE && (std[ss+2] != ','))
         {
         // ENCODE THE QUOTE PAIR, FROM NEXT ONE; 
         std[ss+1] = PUSH_QUOTE;
         ss+=2;
         // -TO-LAST;
         for(ss; ss <= std.length(); ss++)
            {
            if(std[ss] == SINGLE_QUOTE)
               {
               std[ss] = PUSH_QUOTE;
               break;
               }
            }
         goto LOOPwq;
         }
*/
      // DETECTING THE OPENING QUOTE;
      if(ss <= 2 && (std[ss-1] == ' ') && (std[ss-1] != ',') && (std[ss-2] != ','))
         {
         // ENCODE THE QUOTE PAIR, FROM FIRST; 
         std[ss] = PUSH_QUOTE;
         ss++;
         // -TO-LAST;
         for(ss; ss <= std.length(); ss++)
            {
            if(std[ss] == SINGLE_QUOTE)
               {
               std[ss] = PUSH_QUOTE;
               break;
               }
            }
         goto LOOPwq;
         }
      // DETECTING THE CLOSING QUOTE;
      if(std[ss+1] == ' ' && (std[ss-1] != ',') && (std[ss-2] != ','))
         {
         // ENCODE THE QUOTE PAIR, FROM LAST; 
         std[ss] = PUSH_QUOTE;
         // -TO-FIRST;
         if(ss)
            {
            while(--ss && (std[ss] != SINGLE_QUOTE) && (std[ss] != PUSH_QUOTE))
               ;
            }
         if(std[ss] == SINGLE_QUOTE)
            std[ss] = PUSH_QUOTE;
         goto LOOPwq;
         }
      LOOPwq:
      ss = std.find(SINGLE_QUOTE, ss+1);
      }
   }

void CsvParser::POP_LITERAL_QUOTES(StdString& std)
   {
   size_t pos = std.find(PUSH_QUOTE);
   while(pos != NPOS)
      {
      std[pos] = SINGLE_QUOTE;
      pos = std.find(PUSH_QUOTE);
      }
   }

void CsvParser::PUSH_LITERAL_COMMAS(StdString& std, size_t sz_start)
   {
   size_t pos        = std.find(", ", sz_start);
   size_t comma_pos  = pos;
   if(pos != NPOS)
      {
      // If it is NOT >=1, then we CANNOT "peek" behind-it; 
      if(pos)
         {
         // PEEK:
         pos--;
         // If preceeding is NOT a string-field, and NOT numeric, then PUSH it.
         if(std[pos] != SINGLE_QUOTE)
            {
            // Unusual, but not uncommon (skip imbedded white spaces);
            while(pos && (std[pos] == ' '))
               pos--;
            char ch = std[pos];
            // If it is a string field with a numeric-set-up, then "PUSH" the comma;
            if( ! ((ch >= 0x30) && (ch <= 0x39)) )
               std[comma_pos] = PUSH_COMMA;
            }
         else
            {
            // GetField() WILL HANDLE THIS CASE, LATTER ON;
            // --------------------------------------------
            // What can we say? The sequence of `", ` is a legal string field:
            // -even though the same is VERY common in ASCII text (quoting a
            // saying, then pausing, then commenting (eg)) However, without
            // more field data (size, expected type, etc), this is the best that 
            // we can do, so we pass it on unchanged, to be interpteted as a field!
            }
         }
      }
   if(comma_pos != NPOS)
      PUSH_LITERAL_COMMAS(std, comma_pos +1);
   }

void CsvParser::POP_LITERAL_COMMAS(StdString& std)
   {
   size_t pos = std.find(PUSH_COMMA);
   while(pos != NPOS)
      {
      std[pos] = ',';
      pos = std.find(PUSH_COMMA);
      }
   }

void CsvParser::ConvertToTight(StdString& result)
	{
   size_t ss;
   ss = result.find("\", \"");
   while(ss != NPOS)
      {
      result.remove_bytes(ss+2, 1);
      ss = result.find("\", \"");
      }
   }


istream& CsvParser::GetField(istream& in, StdString& result)
	{
   // BY THIS TIME, ANY LITERAL COMMAS AND QUOTES HAVE BEEN ENCODED;
   long top = in.tellg();

	result.read_to_delim(in, ',');

#if 1
   BEGIN
   // Is it a string with an embedded comma?
   size_t ss1, ss2;
   ss1 = ss2 = NULL;

   ss1 = result.find(SINGLE_QUOTE);
   if(ss1 != NPOS)
      ss2 = result.find(SINGLE_QUOTE, ss1+1);

   if(ss2 == NPOS)
      {
      // Yes: This field is a string with an embedded ENGLISH comma-field (numeric or otherwise).
      // Since both english quotes and commas are presumed to be encoded, we simply need to read
      // 'till we get a CLOSING QUOTE-and-comma;
      StdString suffix = "~~~~~";
      while(suffix[suffix.length()-1] != SINGLE_QUOTE)      //suffix.rfind(SINGLE_QUOTE) == NPOS)
         {
         if(!in)
            {
//          throw "CsvParser(ERROR: DATA MALFORMED ON IMPORT)";
            break;      // Malformed data-files are common, too...
            }
      	suffix.read_to_delim(in, ',');
         result.append(suffix);
         }
      }
   END
#endif

   // Remove the comma;
	result.remove(result.length());
   // Remove any odd-spaces;
	result.strip();
	return in;
	}


istream& CsvParser::ImportCsvData(istream& in, StdString& result)
	{
	result = "";

   // get the field (quoted (string) or numeric (no quotes))
   GetField(in, result);

   // Now the quotes will be in a very specific place. Remove them, if we have them;
   if(result[0] == SINGLE_QUOTE)
      result[0] = ' ';
   if(result[result.length()-1] == SINGLE_QUOTE)
      result[result.length()-1] = ' ';

   result.strip();

	return in;
	}


bool CsvParser::Open(const char *psz)
   {
   delete pIfs;
   pIfs = new ifstream(psz, ios::in);
   if(*pIfs)
      return true;
   return false;
   }


size_t CsvParser::GetLine(istream& ifs, Array<StdString>& result, CsvDataType type)
   {
   istream *isH = pIfs;
   pIfs = &ifs;
   size_t count = GetLine(result, type);
   pIfs = isH;
   return count;
   }

size_t CsvParser::GetLine(const StdString& sInput, Array<StdString>& result, CsvDataType type)
   {
   result.Empty();
   StdString str = sInput;
   if(type == StringsOnly)
      {
      str.strip();
      if(str[0] == SINGLE_QUOTE)
         str[0] = ' ';
      if(str[str.length()-1] == SINGLE_QUOTE)
         str[str.length()-1] = ' ';
      str.strip();

      ConvertToTight(str);

      size_t lastone = NULL;
      size_t ss = str.find("\",\"");
      while(ss != NPOS)
         {
         str[ss] = NULL;
         StdString sRes = &str[lastone];
         result[result.Nelem()] = sRes;

         str[ss] = SINGLE_QUOTE;
         lastone = ss+3;
         ss = str.find("\",\"", lastone);
         }
      // Don't forget the last one!
      StdString sRes = &str[lastone];
      result[result.Nelem()] = sRes;
      }
   else
      {
      PUSH_LITERAL_QUOTES(str);
      PUSH_LITERAL_COMMAS(str);

    // WARNING: Stream seeking operations fail under threads (ONLY) in C++Builder 5 & 6!
      stringstream srm;
      srm << str.c_str();
      srm << ends;
      srm.seekg(0, ios::beg);

      StdString std2 = str;
      while(!srm.eof())
         {
         ImportCsvData(srm, std2);
         POP_LITERAL_QUOTES(std2);
         POP_LITERAL_COMMAS(std2);
         result[result.Nelem()] = std2;
         }
      }
return result.Nelem();
}

size_t CsvParser::GetLine(Array<StdString>& result, CsvDataType type)
   {
   result.Empty();
   if(pIfs && *pIfs)
      {
      StdString str;
      str.read_line(*pIfs);
      if(pIfs->eof())
         return NULL;
      return GetLine(str, result, type);
      }
   return 0L;      
   }
size_t CsvParser::GetLineStrings(istream& ifs, Array<StdString>& result)
   {
   return GetLine(ifs, result, StringsOnly);
   }

size_t CsvParser::GetLineStrings(const StdString& std,Array<StdString>& result)
   {
   return GetLine(std, result, StringsOnly);
   }

size_t CsvParser::GetLineStrings(Array<StdString>& result)
   {
   return GetLine(result, StringsOnly);
   }
