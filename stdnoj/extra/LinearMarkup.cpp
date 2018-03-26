#include <extra/LinearMarkup.hpp>

using namespace TextFormatting;


StdString LinearMarkup::AsGmlPage(const StdString& str)
   {
   StdString sResult = WordWrap::AsSimpleString(str);
   sResult.replace("\n", "<br>");
   return sResult;
   }
StdString LinearMarkup::AsGmlPage(Array<StdString>& array)
   {
   StdString sResult;
      {
   stringstream srm;
   for(size_t ss = 0L; ss < array.Nelem(); ss++)
      srm << array[ss] << " ";
   sResult = PRESTO(srm);
      }
   sResult.replace("  ", ' ');
   return AsGmlPage(sResult);
   }
StdString LinearMarkup::RemoveHtmlTags(const StdString& sGmlLine)
   {
   bool bSkip = false;
   StdString sResult;
   for(size_t ss = 0L; ss < sGmlLine.length(); ss++)
      {
      switch(sGmlLine[ss])
         {
         case '<':
            bSkip = true;
         continue;
         case '>':
            bSkip = false;
         continue;
         default:
            if(bSkip == false)
               sResult.append(sGmlLine[ss]);
         break;
         }
      }
   sResult.replace("  ", ' ');
   return sResult;
   }
StdString LinearMarkup::AsTextPage(const StdString& sGmlLine)
   {
   StdString sResult = sGmlLine;
   sResult.strip();
   sResult.replace("  ", ' ');
   sResult.replace("\n", "<br>");
   return RemoveHtmlTags(sResult);
   }


