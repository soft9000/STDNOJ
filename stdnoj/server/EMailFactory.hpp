/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2024 Randall Nagy

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
// 11/12/1998, Created OutboundEMail to support PostJOB1, Randall Nagy
// 11/16/1998, Added FileVersion support, Randall Nagy
// 05/17/1999, Now MUST use "Load()" and "Save()": Moved read() and write() into "protected", Randall Nagy
// 09/23/2003: Ported to FastJob, Randall Nagy
// 06/17/2004: Added Load and Save for file objects, Randall Nagy
// 01/10/2008: Updated Pop3Server::Get() to use subject name (new EMailFactory member MkLocalName() overload), Randall Nagy
// 02/11/2009: Copied from FastJob to stdnoj - Email* IO changed to be in native / RFC format (stdnoj / EPOP) ONLY, Randall Nagy
//
#ifndef EMailFactory_Hpx
#define EMailFactory_Hpx
#include <server/EMail.hpp>
#include <server/EMailMessage.hpp>
#include <Extra/FileVersion.hpp>

using namespace stdnoj;

/**
 * class EMailFactory
 *
 * @author Randall Nagy
 */
class EMailFactory
   {
   public:
      /**
       * Import:
       * @param file RFC-encoded file to load
       * @param header The result
       * @result True if imported
       * @comment In order to control newline cooking, importing and exporting are to files.
       */
      static bool Import(const File& file, EMailHeader& header);
      /**
       * Export:
       * @param file RFC-encoded file name to save to
       * @param header The subject
       * @result True if exported
       * @comment In order to control newline cooking, importing and exporting are to files.
       */
      static bool Export(const File& file, EMailHeader& header);

      /**
       * Import:
       * @param file RFC-encoded file to load
       * @param EMailMessage The result
       * @result True if imported
       * @comment In order to control newline cooking, importing and exporting are to files.
       */
      static bool Import(const File&, EMailMessage&  EMailMessage);
      /**
       * Export:
       * @param file RFC-encoded file name to save to
       * @param EMailMessage The subject
       * @result True if exported
       * @comment In order to control newline cooking, importing and exporting are to files.
       */
      static bool Export(const File&, EMailMessage&  EMailMessage);

      /**
       * IsEMailFactoryFile:
       * @param file RFC-encoded OR native-encoded header file
       * @result True if it contains either type of valid header
       * @comment Use OmniLoad to load either RFC or native file type.
       */
      static bool IsEMailFactoryFile(const File& file);
      /**
       * OmniLoad:
       * @param file RFC-encoded OR native-encoded header file
       * @param header result
       * @result True if it contains either type of valid header
       * @comment Use OmniLoad to load either RFC or native file type.
       */
      static bool OmniLoad(const File& file, EMailHeader& header);
      /**
       * OmniLoad:
       * @param file RFC-encoded OR native-encoded header file
       * @param message result
       * @result True if it contains either type of valid content
       * @comment Use OmniLoad to load either RFC or native file type.
       */
      static bool OmniLoad(const File& file, EMailMessage& emh);
      static bool Load(const File& file, EMailMessage&  EMailMessage);
      static bool Save(const File& file, EMailMessage&  EMailMessage);

      // List the lines in the message as if we were going to send them to the server
      static bool List(const EMailMessage& EMailMessage, Array<StdString>& aLines);
      static bool ListBody(const EMailMessage& EMailMessage, Array<StdString>& aLines);

      // Mostly used by servers to encode / decode an EMailMessage message -
      static bool RemoveTerminalDot(EMailMessage& emb);
      static void EncodeDots(EMailMessage& emb);
      static void DecodeDots(EMailMessage& emb);

      // Logical names are NOT checked for physical exstance - You might want to engineer
      // in an "X-NNS-LocalName: " in the header for use later, but it's not important at
      // the moment;
      static bool MkLogicalName(File& file, const EMailHeader& emh, const StdString& sExtension, bool TryMessageID);

      // This version checks to see if the enumerated file exists in the folder.
      // If it does and headers are the same, then the same file name is returned.
      // This means that if the file returned exists, then the content has been
      // previously saved to the folder specified.
      static File MkLocalName(const Directory& dir, EMailHeader& em, const StdString& sExtension);
      // Like MkLocalName(above), but favors the string provided (emh.Subject(), etc.)
      // Does NOT check file content. Guaranteed unique file name returned, only.
      static File MkLocalName(const Directory& dir, const StdString& sNodeRoot, const StdString& sExtension);

      // Reduces a single address to just the '@' part;
      static bool AbsEmailAddress(StdString& std);
      static bool AbsEmailAddress(const StdString& sIn, StdString& sOut);
      static bool ContainsAbsEmailAddress(const StdString& sIn);
      // Same as AbsEmailAddress(), but returns result with the required <>'s;
      static bool FormatAsAddress(StdString& std);
      static bool IsAddressSeparator(char ch);
   };
#endif
