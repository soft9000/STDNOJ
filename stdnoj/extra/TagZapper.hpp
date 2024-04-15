// Class TagZapper: Removed from htmlParser and encapsulated to create a new class
// for the NOJ Library.
//
// 04/13/2008: Class created by wrappering some functionality from my htmlParser classes, Randall Nagy
//
#ifndef _TagZapper_Hpp
#define _TagZapper_Hpp

#include <stdnoj.hpp>

using namespace stdnoj;

class TagZapper
   {
   protected:
      static StdString 
         _ZapHtml(const StdString& sBlock, const char ch1, const char ch2);

   public:
      static StdString 
         HtmlTo7Bit(const StdString& str);

      static StdString 
         ZapHtml(const StdString& sBlock, const char ch1 = '<', const char ch2 = '>');
   };

inline
bool RemoveHtml(File& fileIn, File& fileOut)
   {
   stringstream srm;
   if(fileIn.DumpContents(srm) == false)
      return false;

   StdString sBlock = PRESTO(srm);
   StdString sResult = TagZapper::ZapHtml(sBlock);

   ostream& os = fileOut.OpenWrite();
   sResult.write(os);
   fileOut.Close();
   return fileOut.Exists();
   }


#endif
