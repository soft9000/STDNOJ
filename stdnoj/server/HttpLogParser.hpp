/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2009 R. A. Nagy 

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
// 10/10/2003: Classes created, R. Nagy
//
#ifndef _Http_Log_Parser
#define _Http_Log_Parser

#include <stdnoj.hpp>

/*
The Common Logfile Format

    remotehost rfc931 authuser [date] "request" status bytes

   remotehost
      Remote hostname (DNS hostname or IP address if not available.) 
   rfc931
      The remote logname of the user (if available). 
   authuser
      The authenticated username (if available). 
   [date]
      Date and time of the request, delimited by brackets. Always present.
   "request"
      The exact client request line, delimited by quotes. Use Normalize() to standardize it.
   status
      HTTP status code result.
   bytes
      Resulting content-length transferred. 
 */

using namespace stdnoj;

class HttpLogEntry {
public:
    StdString sRemoteHost;
    StdString sRemoteLogname;
    StdString sRemoveUser;
    StdString sDateTime;
    StdString sRequest;
    StdString sResultCode;
    StdString sBytesSent;

    // Convert to our standard date and time format
    bool QueryDate(StdDateTime&) const;
    // The web page or file name requested
    StdString FileName(void) const;
    // Attempts to remove odd characters and other common problems. False if unexpected data / format is encountered.
    bool Normalize(void);
    // Returns the parameters up to and including the parameter char        
    bool Parameters(StdString& str, char ch = '?') const;
    // The RFC (hundreds based) result code

    int ResultCode(void) const {
        return sResultCode.as_int();
    }

    int operator==(const HttpLogEntry& ref) const;
    int operator!=(const HttpLogEntry& ref) const;
    int operator>(const HttpLogEntry& ref) const;

    istream& log_import(istream& is); // HTTP (common) format
    ostream& log_export(ostream& os); // HTTP (common) format

    istream& read(istream& is); // proprietary format
    ostream& write(ostream& os); // proprietary format
};

class HttpLog {
public:
    Array<HttpLogEntry> aLines;

    HttpLog(void) : bNormalized(false) {
    }

    bool bNormalized; // Normalization occurs only when required
    bool Normalize(void); // Asserts bNormalized only when 100% pass.

    istream& log_import(istream& is); // HTTP (common) format
    ostream& log_export(ostream& os); // HTTP (common) format

    istream& read(istream& is); // proprietary format
    ostream& write(ostream& os); // proprietary format
};

class HttpLogParser {
public:
    StdString sError;

    bool Import(HttpLog&, File& fileIn);
    bool Export(HttpLog&, File& fileOut);
};

inline
long unsigned NumLines(File& file) {
    StdString str;
    long unsigned count = 0L;
    istream& is = file.OpenRead();
    while (is) {
        str.readline(is);
        count++;
    }
    file.Close();
    return count;
}

#endif
