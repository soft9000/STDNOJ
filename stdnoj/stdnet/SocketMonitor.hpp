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
// 09/15/2006: Refactored StdSocket::MonitorConnection() into 
//             nojnet::SocketMonitor class .Still needs to be tested for 
//             possible regression, Randall Nagy
//
#ifndef SocketMonitor78_hpx
#define SocketMonitor78_hpx

#include <stdnet.hpp>

namespace stdnet {

    class SocketMonitor {
    protected:
        stdnet::Socket pwSocket;

        void LOG(ostream& cout, const char *pszBuf, int capture);
        void LOG_LINE(const char *pszBuf);
        void LOG_LINE(ostream& cout, const char *pszBuf, int capture);
        int GETLINE(istream& cin, ostream& cout, char *pszBuf, size_t szBuf, int& capture, int& converse);

    public:

        // This monitors an existing connection -
        void MonitorConnection(Socket& soc, ostream& cout, istream& cin);

    };


}

#endif
