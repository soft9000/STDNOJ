#ifndef EmailMessage_Hpx
#define EmailMessage_Hpx

#include <server/EMail.hpp>
//#include <server/MimeBlock.hpp>

class EMailMessage : public EMailHeader
   {
   private:
      bool   ParseMessageBody(ifstream& is);     // Rfc822 format (BODY ONLY)

   protected:
      StdString   _text;

      // If you dare to use these, you simply *MUST* make sure that your
      // stream is in BINARY MODE since \r\n is by definition what TERMINATE is!!
      ostream& write_stream(ostream& os) const; // NATIVE / BINARY
      istream& read_stream(istream& is);        // NATIVE / BINARY

      bool     rfc_write(ostream& ofs) const;   // RFC
      bool     rfc_read(istream& ifs);          // RFC

      bool     rfc_message(istream& ifs);      // BODY ONLY ..?

      ostream&  write_searchable(ostream& os);

   public:
      EMailMessage(void)                       {}
      EMailMessage(const EMailMessage& em)     {(*this) = em;}
      virtual ~EMailMessage(void)  {}

      bool   IsNull(void)   const;

      bool   ParseMessageBody(const File& file); // Now, only RFC ASCII... but later, who knowns?
      bool   ParseMessageBody(stringstream& is); // Rfc822 format (BODY ONLY)

      bool           HasBody(void) const;
      const char *   Body(void) const                {return _text.c_str();}
      bool           Body(const StdString& msg)      {_text.assign(msg); return true;}
//      bool           Decode(Array<MimeBlock>& array) {return MimeBlock::Decode(*this, array);}
//      bool           Encode(Array<MimeBlock>& array) {return MimeBlock::Encode(*this, array);}

      int            operator==(const EMailMessage& em) const;
      EMailMessage&  operator=(const EMailMessage& em);

      // Streamable (array compatible) format
      ostream& write(ostream& os) const    {return write_stream(os);}
      istream& read(istream& is)           {return read_stream(is);}

      // Streamable (array compatible) format
      bool Load(const File& file);
      bool Save(const File& file) const;

      // RFC Format - one per file / stream
      bool Import(const File& file);
      bool Export(const File& file) const;
      bool Import(istream& is);
      bool Export(ostream& os ) const;

      static bool EncodeHTML(EMailMessage& ref, const StdString& sHtmlMessage);

   friend class EMailAND;
   friend class EMailOR;
   friend class EMailFactory;
   friend class EMailModifier;
   friend class Pop3Server;
   friend class SmtpServer;
   friend class EMailAndOrNot;
   friend class EMailFilter;
   friend class EMailAndOrNotExtended;
   friend class MimeBlock;
   friend class EMimeMessage;
   };

#endif

