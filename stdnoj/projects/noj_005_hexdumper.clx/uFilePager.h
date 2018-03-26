//---------------------------------------------------------------------------

#ifndef uFilePagerH
#define uFilePagerH
//---------------------------------------------------------------------------

#include <stdnoj.hpp>

using namespace stdnoj;

class FilePager
{
private:
   bool bTextFile;
   streampos spOffset;
   int pwPage;
   int block_size;
   int iLast;

protected:
   File pwFile;
   int Get(ByteBuffer& buff, int ss);

public:
   FilePager(void);
   bool Open(File& file);

   bool ReadAll(Array<StdString>& array);


   int PageNumber(void)    {return pwPage;}
   streampos Offset(void)  {return spOffset;}

   // Returns next block number, else -1 on error
   void  GetPresent(ByteBuffer& buf);
   void  GetNext(ByteBuffer& buf);
   void  GetPrev(ByteBuffer& buf);
};

#endif
