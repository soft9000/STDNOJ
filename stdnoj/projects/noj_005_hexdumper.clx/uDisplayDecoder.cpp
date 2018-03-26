//---------------------------------------------------------------------------
#pragma hdrstop

#include "uDisplayDecoder.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


void DisplayDecoder::DecodeText(const ByteBuffer& bb, Array<StdString>& aResult)
   {
   aResult.Empty();
   StdString sLine;
   for(int ss = 0L; ss < bb.length(); ss++)
      {
      byte_t ich = bb[ss];
      if(ich == '\n')
         {
         // ASCII line break
         aResult[aResult.Nelem()] = sLine;
         sLine = "";
         continue;
         }
      if(ich < 32 || ich > 126)
         {
         // Binary character
         sLine.append('_');
         continue;
         }
      // A normal character ...
      sLine.append(bb[ss]);
      }

   if(!sLine.is_null())
      {
      aResult[aResult.Nelem()] = sLine;
      sLine = "";
      }
   }

void DisplayDecoder::DecodeHex(const ByteBuffer& bb, Array<StdString>& aResult, int chars_per_line)
   {
   StdString sLine;
   aResult.Empty();
   ByteBuffer cx;
   for(size_t ss = 0L; ss < bb.length(); ss++)
      {
      if(ss && (ss % chars_per_line) == 0)
         {
         // convert the buffer to hex
         cx.hexline(sLine, false, chars_per_line);
         aResult[aResult.Nelem()] = sLine;
         cx.empty();
         }
      cx.append(bb[ss]);
      }
   if(cx.is_null() == false)
      {
      cx.hexline(sLine, false, chars_per_line);
      aResult[aResult.Nelem()] = sLine;
      }
   }
