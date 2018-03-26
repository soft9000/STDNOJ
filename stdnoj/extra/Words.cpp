#include <extra/Words.hpp>

using namespace TextFormatting;

// Formats the string as a flowing block of text (paragraph)
Array<StdString> Words::AsParagraph(const StdString& sBlock, int cx, int margin, int maxLen)
   {
   StdString str = sBlock;
   WordWrap::AsSimpleString(str);
   str.replace("\n", ' ');
   return WordWrap::Wrap(str, cx, margin, maxLen);
   }

// Formats the string as a flowing block of text (paragraph)
Array<StdString> Words::AsPage(const StdString& sBlock, int cx, int margin, int maxLen)
   {
   StdString str = sBlock;
   WordWrap::AsSimpleString(str);
   str.replace("\n\n", (char)0xfe);
   str.replace("\n", ' ');
   Array<StdString> sArray = str.as_array((char)0xfe);
   Array<StdString> array;
   array.AddZombies(500);
   for(size_t ss = 0L; ss < sArray.Nelem(); ss++)
      {
      array.Append(AsParagraph(sArray[ss]));
      array.Append("\n\n");
      }
   return array;
   }

// Formats the block into a "record" (sentance) format.
Array<StdString> Words::AsSentances(const StdString& sBlock)
   {
   StdString str = sBlock;
   str.replace('\r', ' ');
   str.replace('\n', ' ');
   str.replace("  ", ' ');
   size_t last_ss = 0L;
   StdString sLine;
   Array<StdString> array;
   size_t ss = 0L;

   str.replace("...", '.');
   str.replace("??", '?');
   str.replace("!!", '!');

   for(ss = 0L; ss < str.length(); ss++)
      {
      switch(str[ss])
         {
         case '?':
         case ':':
         case '!':
         case ';':
         case '.':
            {
            sLine = str.subpos(last_ss, ss + 1);
            sLine.strip();
            array.Append(sLine);
            last_ss = ss + 1;
            }
         break;
         default:
         break;
         }
      }
   if(last_ss != ss)
      array.Append(str.subpos(last_ss, NPOS));
   return array;
   }

StdString Words::AsBlock(const Array<StdString>& array)
   {
   StdString str;
   stringstream srm;
   for(size_t ss = 0L; ss < array.Nelem(); ss++)
      {
      array.At(ss, str);
      srm << str;
      srm << '\n';
      }
   str = WordWrap::AsSimpleString(PRESTO(srm));
   str.replace('\n', ' ');
   return str;
   }


