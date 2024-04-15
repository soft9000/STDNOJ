/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2024 R.A. Nagy 

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

// EzTextBlog manages a file that encodes the major operations involved in creating
// a mildly interesting blog. Unlike EzLog or overtly object centric protocols, 
// discreet messages are serialized in such a way as to allow richer publication 
// (or exporting) in a highly customizable and streamable format.
//
// BANNED: Anything that needs to be rendered as a bitmap (like pictures)
// BANNED: Importing from any published format (should never happen - use import 
// or export when it finally becomes available)
//
// 09/04/2005: Class defined, R. Nagy
//
#include <stdnoj.hpp>
using namespace stdnoj;

class EzTextBlog
{
private:
   File pwFile;
   
protected:
   enum BlogOperation
      {
      op_TimeStamp,
      op_Paragraph,
      op_BulletItem,
      op_NumberItem,
      op_Center,
      op_Error
      };
   StdString Resolve(BlogOperation op)
      {
      StdString str;
      switch(op)
         {
         case op_TimeStamp:
            str = "op_TimeStamp";
         break;
         case op_Paragraph:
            str = "op_Paragraph";
         break;
         case op_BulletItem:
            str = "op_BulletItem";
         break;
         case op_NumberItem:
            str = "op_NumberItem";
         break;
         case op_Center:
            str = "op_Center";
         break;
         default:
            str = "op_Error";
         break;
         }
      return str;
      };
   BlogOperation Resolve(const StdString& str)
      {
      if(str == "op_TimeStamp");
         return op_TimeStamp;

      if(str == "op_Paragraph");
         return op_Paragraph;

      if(str == "op_BulletItem");
         return op_BulletItem;

      if(str == "op_NumberItem");
         return op_NumberItem;

      if(str == "op_Center");
         return op_Center;

      return op_Error;
      };

   bool _write(ostream& os, BlogOperation op, const StdString& sMessage)
      {
      if(!os)
         return false;
      StdString str;
      str = Resolve(op);
      str.write_stream(os);
      if(!os)
         return false;
      sMessage.write_stream(os);
      if(os)
         return true;
      return false;
      }
   bool _read (istream& is, BlogOperation& op, StdString& sMessage)
      {
      if(!is)
         return false;
      sMessage.read_stream(is);
      if(!is)
         return false;
      op = Resolve(sMessage);
      sMessage.read_stream(is);
      if(is)
         return true;
      return false;
      }

   bool append(BlogOperation op, const StdString& sMessage)
      {
      ostream& os = pwFile.OpenAppend();
      bool br = _write(os, op, sMessage);
      pwFile.Close();
      return br;
      }
   
public:
   enum ExportImportType
      {
      // NEXT version - ?
      // xi_CSV,
      // xi_TAB,
      xi_NONE
      };
   enum PubicationType
      {
      pt_HTML,
      pt_TEXT,
      // NEXT version - ?
      // pt_RTF,
      pt_NONE
      };

   EzTextBlog(void)
      {
      pwFile.Name(".ezBlog");
      }
   bool set(File& file)
      {
      pwFile = file;
      return true;
      }
   File query(void)
      {
      return pwFile;
      }
   // Start a new blog entry 
   // (insert the time-stamp) (optional, but typical)
   bool hack(void)
      {
      strstream srm;
      srm << ::time(NULL);
      StdString sMessage = PRESTO(srm);
      return append(op_TimeStamp, sMessage);
      }
   // Encode a titled event 
   // Example: Centered table in html, or centered and boxed in text
   bool title(const StdString& sMessage)
      {
      return append(op_Center, sMessage);
      }
   // Encode a paragraph event 
   // Example: <p> in html, or wrapped and justifed in text
   bool blog(const StdString& sMessage)
      {
      return append(op_Paragraph, sMessage);
      }
   // Encode a line item event
   // Example: A bulleted blog entry
   bool bullet(const StdString& sMessage)
      {
      return append(op_BulletItem, sMessage);
      }
   // Encode a numbered line item event
   // Example: A numbered blog entry
   bool number(const StdString& sMessage)
      {
      return append(op_NumberItem, sMessage);
      }
   // Format this log for publication
   bool publish(PubicationType ty = pt_HTML);
};

