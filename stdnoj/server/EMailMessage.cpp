// 10/24/1998, Ran through NOTAB, Randall Nagy
//

#include <server/Email.hpp>
#include <server/EMailMessage.hpp>
#include <extra/WordWrap.hpp>
//using namespace BetterMail;

#define BEGIN {
#define END }

//#pragma message "BINARY STREAMS REQUIRED: SETMODE is not going to do anything!!"


bool EMailMessage::rfc_write(ostream& ofs) const
   {
   if(EMailHeader::rfc_write(ofs) == false)
      return false;

   ofs << TERMINATE;

   ofs << _text.c_str();
   ofs << TERMINATE;
   return true;
   }
bool EMailMessage::HasBody(void) const
   {
   if(_text.is_null())
      return false;
   return true;
   }
bool EMailMessage::ParseMessageBody(stringstream& ifs)
   {
   return rfc_message(ifs);
   }
bool EMailMessage::ParseMessageBody(ifstream& ifs)
   {
   bool br = rfc_message(ifs);
   return br;
   }
bool EMailMessage::ParseMessageBody(const File& fff)
   {
   File file = fff;
   bool br = rfc_message(file.OpenRead(File::AT_BINARY));
   file.Close();
   return br;
   }
bool EMailMessage::rfc_message(istream& ifs)
   {
   StdString std, text;
   _text = std;     // empty it out
   if(!ifs)
      return false;

   while((!ifs.eof()) && ifs && ifs.good())
      {
      std.readline(ifs);

      if(MailBlock::IsEndOfMessage(std, true))
         {
         // Usual terminal condition;
         // done = 1;
         break;
         }

      if(ifs)
         {
         MailBlock::Cleanup(std);         // This might wind-up being NULL
         text.append(TERMINATE);          // and this both RFC-joins && will put same, back.
         if(!text.is_null())
            text.append(std);             // (i.o will terminate)
         }
      }

   _text = text;     // VERY IMPORTANT TO RE-ASSIGN SO WE CAN RAM-FREE THE HUGE GROWTH SPACE.
   return true;
   }
bool EMailMessage::rfc_read(istream& ifs)
   {
   (*this) = EMailMessage();
   if(EMailHeader::rfc_read(ifs) == true)
      return rfc_message(ifs);
   return false;
   }
int EMailMessage::operator==(const EMailMessage& em) const
   {
   // Man, there ain't no reason to do a strcmp on a huge string
   // when we have headers;
   return EMailHeader::operator==((const EMailHeader&) em);
   }
EMailMessage& EMailMessage::operator=(const EMailMessage& em)
   {
   if(this == &em)
      return *this;
   _text.assign(em._text);
   EMailHeader::operator=(em);
   return *this;
   }
bool EMailMessage::IsNull(void)   const
   {
   if(_text.is_null() && EMailHeader::IsNull())
      return true;
   return false;
   }
ostream& EMailMessage::write_stream(ostream& os) const
   {
   EMailHeader::write_stream(os);
   _text.write_stream(os);
   return os;
   }
ostream& EMailMessage::write_searchable(ostream& os)
   {
   EMailHeader::write_stream(os);
#if 1
   if(IsMime() == false)
      _text.write(os);
#else
   if(IsMime() == false)
      _text.write(os);
   else
      {
      StdString sSearchable;
      size_t whence     = NULL;
      size_t sz_found   = NULL;
      size_t count      = NULL;
      size_t sanity     = NULL;
      size_t lines      = NULL;
      while(sz_found != NPOS)
         {
         sz_found = _text.find('\n', whence);
         // Detect a size_t overflow;
         if(sz_found < sanity)
            sz_found = NPOS;
         else
            sanity = sz_found;
         if(sz_found == NPOS)
            {
            os << TERMINATE;
            os << sSearchable.c_str();
            continue;
            }

         sSearchable = _text.substr(whence, sz_found - whence);
         whence = sz_found +1;
         Cleanup(sSearchable);
         if( (sSearchable.length() > 60) && (sSearchable.find(' ') == false) )
            {
            // Crude, but universal hex-encoded attachment detection;
            count++;
            if(count >= 15)
               {
               sz_found = NPOS;     // bye!
               continue;
               }
            }
         else
            count = NULL;

         // Write-out the string;
         lines++;
         os << TERMINATE;
         os << sSearchable.c_str();
         } // ewhile
      } // if mime

   os << TERMINATE;
#endif
   return os;
   }
istream& EMailMessage::read_stream(istream& is)
   {
   EMailHeader::read_stream(is);
   _text.read_stream(is);
   return is;
   }

bool EMailMessage::Load(const File& file)          
   {
   File fff = file; 
   istream& is = read_stream(fff.OpenRead(File::AT_BINARY));
   if(is)
      return true;
   return false;
   }
bool EMailMessage::Save(const File& file) const    
   {
   File fff = file; 
   ostream& os = write_stream(fff.OpenWrite(File::AT_BINARY));
   if(os)
      return true;
   return false;
   }

// RFC Format - one per file / stream
bool EMailMessage::Import(const File& file)        
   {
   File fff = file; 
   return rfc_read(fff.OpenRead(File::AT_BINARY));
   }
bool EMailMessage::Export(const File& file) const  
   {
   File fff = file; 
   return rfc_write(fff.OpenWrite(File::AT_BINARY));
   }
bool EMailMessage::Import(istream& is)             
   {
   return rfc_read(is);
   }
bool EMailMessage::Export(ostream& os ) const
   {
   return rfc_write(os);
   }

bool EMailMessage::EncodeHTML(EMailMessage& ref, const StdString& sHtmlMessage)
   {
   ref.ContentType("text/html");

   // This is a quick fix - Easy line folding - 2/10/2007, Randall Nagy
   StdString sMessage = sHtmlMessage;
   sMessage.replace("><", "> <");   // observed -  often need logical a place TO wrap!
   sMessage = TextFormatting::WordWrap::WrapDos(sMessage, 0, 0, 65);

   return ref.Body(sMessage);
   }

