//---------------------------------------------------------------------------


#pragma hdrstop

#include "uFilePager.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


FilePager::FilePager(void) : pwPage(0), spOffset(-1), block_size(16 * 20), iLast(0), bTextFile(true)
   {
   }
bool FilePager::Open(File& file)
   {
   bool br = false;
   if(file.Exists())
      {
      pwPage = -1;      // mark first page
      spOffset = 0L;
      pwFile = file;
      br = true;
      }
   bTextFile = IsTextFile(pwFile);
   return br;
   }
bool FilePager::ReadAll(Array<StdString>& array)
   {
   bool br = false;
   array.Empty();
   istream& is = pwFile.OpenRead(File::AT_BINARY);
   StdString sLine;
   while(is)
      {
      sLine.readline(is);
      if(!is)
         continue;
      sLine.replace('\r', ' ');
      array[array.Nelem()] = sLine;
      br = true;
      }
   return br;      
   }
// -1 on error
void FilePager::GetNext(ByteBuffer& buf)
   {
   if(iLast == -1)
      return;  // cannnot advance - last buffer!

   if(pwPage < 0)
      {
      pwPage = 0;  // the first page
      return GetPresent(buf);
      }

   // Subsequent pages
   pwPage++;
   iLast = Get(buf, pwPage);
   if(iLast == 0)
      pwPage--;
   }
void FilePager::GetPrev(ByteBuffer& buf)
   {
   if(pwPage < 0)
      {
      pwPage = 0;  // the first page
      return GetPresent(buf);
      }

   pwPage--;
   iLast = Get(buf, pwPage);
   if(iLast == 0)
      pwPage++;
   }
void  FilePager::GetPresent(ByteBuffer& buf)
   {
   if(pwPage < 0)
      pwPage = 0;  // the first page
   iLast = Get(buf, pwPage);
   }
int  FilePager::Get(ByteBuffer& buf, int ss)
   {
   if(ss < 0)
      return 0;

   istream& ios = pwFile.OpenRead(File::AT_BINARY);
   spOffset = ss * block_size;
   ios.seekg(spOffset);
   if(ios)
      {
      size_t count = buf.read_bytes(ios, block_size);
      pwFile.Close();
      if(!count)
         return 0;   // Got nothing
      if(count == block_size)
         return 1;   // complete
      if(count < block_size)
         return -1;  // partial
      }
   return 0;
   }
   
