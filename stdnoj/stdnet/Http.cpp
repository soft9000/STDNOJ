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
#include <stdnet.hpp>
#include <sstream>
using namespace std;

using namespace stdnet;

Http::HttpResponse::HttpResponse(void) : iCode(0) {
}

bool Http::HttpResponse::Parse(const StdString& sSocketString) {
    StdString sBuf = sSocketString;
    sBuf.replace("\r\r", "\r"); // Adjust if a "\r\n" was saved as text on DOS -

    iCode = 0;
    aHeaders.Empty();
    sResponse = "";
    size_t szHeader = sBuf.find("\r\n\r\n"); // 4
    if (szHeader == npos)
        return false;
    sBuf[szHeader] = NULL;
    sBuf.replace("\r\n", "\n");
    aHeaders = sBuf.slice('\n');
    if (!aHeaders.Nelem())
        return false;

    szHeader += 4;
    sResponse = sSocketString.substr(szHeader, NPOS);

    sBuf = aHeaders[0];
    Array<StdString> array = sBuf.slice(' ');

    iCode = array[1].as_int();
    if (!iCode)
        return false;

    return true;
}

bool Http::Connect(Socket& soc, const Url& url) {
    if (url.Type() != url_http)
        return false;

    if (soc.Connect(url.Site(), 80) == false)
        return false;

    return true;
}

size_t Http::Size(const Url& url, size_t szDefault) {
    Socket soc;

    if (Connect(soc, url) == false)
        return npos;

    stringstream srm;
    srm << "HEAD " << url.Page() << " HTTP/1.0\n\n";

    StdString sRequest = srm.str().c_str();
    StdString sResult;

    bool br = soc.Transact(sRequest, sResult);
    soc.Disconnect();
    if (br == false)
        return npos;

    StdString sPattern = "Content-Length:";
    size_t pos = sResult.find(sPattern);
    if (pos == npos)
        return szDefault;

    sResult.remove_pos(0, pos + sPattern.length());
    pos = sResult.find('\n');
    if (pos == npos)
        return npos;

    sResult.remove(pos);
    sResult.strip();

    return (size_t) sResult.as_long();
}

bool Http::Download(const Url& url, const File& fff, size_t szDefault) {
    /*
       size_t pos = Size(url, szDefault);
       if(pos == npos)
          return false;
     */
    stringstream srm;
    srm << "GET " << url.Page() << " HTTP/1.0\n\n";
    StdString sRequest = srm.str().c_str();

    Socket soc;
    if (Connect(soc, url) == false)
        return npos;

    if (soc.Send(sRequest) == false)
        return false;

    StdString sResult, sBuffer;
    while (soc.Receive(sBuffer) == true) {
        if (sBuffer.is_null())
            break;
        if (sResult.is_null()) {
            HttpResponse resp;
            if (resp.Parse(sBuffer) == true) {
                // Only want to examine the headers -
                if (resp.iCode != 200)
                    return false;
                // Do not want the headers ...
                sBuffer = resp.sResponse;
            }
        }
        sResult += sBuffer;
    }

    if (sResult.is_null())
        return false;

    File file = fff;
    ostream& os = file.OpenWrite();
    os << sResult;
    return true;
}

