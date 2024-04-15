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
#include <extra/WordWrap.hpp>

using namespace TextFormatting;


WordWrap::TextType WordWrap::AsSimpleString(StdString& str)
   {
   char ZZ_NEWLINE  = char(0xfe);

   TextType iFlag = tt_unix;
   if(str.find("\r\n") != NPOS)
      {
      iFlag = tt_dos;  // Probable: Map back to MS-DOS style newlines
      str.replace("\r\n", ZZ_NEWLINE);
      }
   // Expected (unix) mapping:
   str.replace('\n', ZZ_NEWLINE);
   if(str.find('\r') != NPOS)
      {
      iFlag = tt_other;  // Unexpected (TANDY / ERROR) mapping
      str.replace('\r', ZZ_NEWLINE);
      }
   str.replace(ZZ_NEWLINE, '\n');
   int ict = str.replace(" \n", '\n');
   while(ict)
      ict = str.replace(" \n", '\n');
   return iFlag;
   }


void  WordWrap::AsDosString(StdString& str)
   {
   str.replace('\n', "\r\n");
   }


Array<StdString>  WordWrap::Wrap(const StdString& str, int cx, int margin, int max)
   {
   StdString sResult = str;
   AsSimpleString(sResult);
   sResult = WrapNormalized(str, cx, margin, max);
   return sResult.as_array('\n');
   }


StdString  WordWrap::Wrap(const StdString& str, TextType tt, int cx, int margin, int maxLen)
   {
   StdString sContent = str;
   AsSimpleString(sContent);
   sContent = WrapNormalized(sContent, cx, margin, maxLen);
   if(tt != tt_unix)
      AsDosString(sContent);
   return sContent;
   }


StdString  WordWrap::WrapUnix(const StdString& str, int cx, int margin, int maxLen)
   {
   return Wrap(str, tt_unix, cx, margin, maxLen);
   }


StdString  WordWrap::WrapDos(const StdString& str, int cx, int margin, int maxLen)
   {
   return Wrap(str, tt_dos, cx, margin, maxLen);
   }


StdString  WordWrap::WrapNormalized(const StdString& str, int cx, int margin, int max)
   {
   StdString sQuote = str;
   if(cx < max)
      cx = max;

   if(cx < margin)
      margin = cx / 4;

   int ignoring_leading_newlines_and_spaces = 1;

   size_t last_sp = -1L;
   for(size_t ss = 0L, offset = 0L; ss < (int)sQuote.length(); ss++, offset++)
      {
      switch(sQuote[ss])
         {
         case '\n':
            {
            // If multiple '\n' sequence, then just pass-it-thru;
            if(sQuote[ss+1] == '\n')
               ignoring_leading_newlines_and_spaces = 1;

            if(ignoring_leading_newlines_and_spaces)
               {
               offset  = 0L;   // Position 0 on pwLine
               last_sp = -1L;    // NO SPACE ON pwLine
               continue;
               }

            size_t underflow = cx - margin;
            // Is there an UNDERflow?
            if(ss && (offset < underflow))
               {
               // Map the '\n' over to a space. Moving onward without
               // changing our offset now makes us a candidate for an
               // OVERflow conditon (DESIRED);
               sQuote[ss] = ' ';
               last_sp = ss-1;
               }
            else
               {
               // No need to re-format anything.
               // Move on toward to the next line;
               offset  = 0L;   // Position 0 on pwLine
               last_sp = -1L;    // NO SPACE ON pwLine
               }
            }
         continue;

         case ' ':
         case '\t':
            last_sp = ss;
         continue;

         default:
            {
            ignoring_leading_newlines_and_spaces = 0;

            if(offset > cx)
               {
               if(last_sp && (last_sp != -1L))
                  {
                  // BREAK TYPE ONE (TYPICAL)
                  // ========================
                  if(sQuote[ss+1] == '\n')
                     continue;

                  // Break the line at the last white-space;
                  sQuote.insert(last_sp+1, '\n');
                  // re-start parsing AFTER the last '\n' (IMPORTANT FOR UNDERFLOW DETECTION);
                  ss = sQuote.find('\n', last_sp); // MAINTENANCE: USE THIS METHOD. DO NOT RISK A FUTURE UNDERFLOW TO SAVE A FEW PICA-SECONDS!
                  ss++;
                  }
               else
                  {
                  // BREAK TYPE TWO (ATYPICAL)
                  // =========================
                  // Too strange a format calls for too strange a result;
                  // (i.e: precision break-up of a long series of "===="'s (etc);
                  sQuote.insert(ss, '\n');
                  }
               offset  = 0L;   // Position 0 on pwLine
               last_sp = -1L;    // NO SPACE ON pwLine
               }
            }
         continue;
         }
      }
   return sQuote;
}




