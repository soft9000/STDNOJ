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
#ifndef stdHttp_hpp
#define stdHttp_hpp

#include <stdnet.hpp>

namespace stdnet {

    class Http {
    protected:
        static bool Connect(Socket& soc, const Url& url);
        static size_t Size(Socket& soc, const Url& url, size_t szDefault = SOC_BUFFER_SZ);

    public:

        struct HttpResponse {
            HttpResponse();
            int iCode;
            Array<StdString> aHeaders;
            StdString sResponse;
            bool Parse(const StdString & sSocketString);
        };
        static size_t Size(const Url& url, size_t szDefault = SOC_BUFFER_SZ);
        static bool Download(const Url& url, const File& file, size_t szDefault = SOC_BUFFER_SZ);
    };

}

#endif
