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
#include <stdnoj.hpp>
using namespace stdnoj;

#ifndef stdnet_url_hpx
#define stdnet_url_hpx

namespace stdnet {
    // URL syntax is Protocol://<user>:<password>@<host>:<port>/<url-path>

    struct URL {
        StdString sError;

        enum RfcProtocol {
            none,
            ftp, // File Transfer Protocol
            http, // Hypertext Transfer Protocol
            gopher, // The Gopher Protocol
            mailto, // Electronic mail address
            news, // USENET news
            nntp, // USENET news using NNTP access
            telnet, // Reference to interactive sessions
            wais, // Wide Area Information Servers
            file, // Host-specific file names
            prospero // Prospero Directory Service
        } protocol; // This is required

        StdString sServer; // This is required
        StdString sPath; // Technically optional, but used frequently.

        // The Specific portion tailors the URL to an access instance -

        struct Specific {
            int iPort;
            StdString sUserId;
            StdString sPassword; // RFC1738: "... there is no way to specify a password without specifying a user name."

            Specific(void) : iPort(0) {
            }

            bool IsNull(void);
        } opt;

        URL(void);

        int operator==(const URL & url) const;
        int operator!=(const URL & url) const;
        int operator>(const URL & url) const;
        int operator<(const URL & url) const;

        bool IsNull(void) const;
        void Reset(void);

        // RFC1738: Special User, Password, and Path characters must be encoded (sic)

        bool Encode(void) {
            return false;
        } // Not supported yet

        bool Decode(void) {
            return false;
        } // Not supported yet

        static bool Encode(StdString& sEncoded, const URL & upResult); // Factorize
        static bool Decode(const StdString& sEncoded, URL & upResult); // Serialize
        static bool Test(URL& url, ostream & cout); // Resize ;-)
    };

}

#endif
