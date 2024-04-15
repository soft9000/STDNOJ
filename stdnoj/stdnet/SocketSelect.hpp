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
// Class nojnet::SocketSelect: Tired of all of the #ifdef garbage? 
// Then here is a wrapper that you can use no matter whose "standard" sockets 
// you are running!
//
// While the model has been designed for portability, what I have to share at 
// the moment remains a decidely WIN32 implementation - Apologies - But there 
// are only so many free hours in the day - (regression testing is always the 
// issue ...!)
//
// *** but ***
// Try libwww - Not too sexy - It has a huge learning curve - but it works
// okay.
//
// *** note ***
// Windows developers will need to include a reference to ws2_32.lib in 
// their project (replaces the old wsock32.lib) to make the link work -
//
// 12/22/1997: Class created, Randall Nagy (WinSock2 Version)
// 12/24/1997: Added select(TIMEOUT) to IO to keep from blocking-forever, Randall Nagy.
//             Added MonitorConnection(), Randall Nagy.
// 01/21/1998: Added the Trace facility, Randall Nagy.
// 01/31/1998: Implemented the timeout_ai, Randall Nagy.
// 12/03/1998: Added timeout value to ::Transact(),   Randall Nagy.
// 09/23/2003: Ported to stdnoj, Randall Nagy
// 09/24/2003: Changed SOC_TIMEOUT from 100L to 15L,  Randall Nagy
// 09/15/2006: Coined the nojnet namespace            Randall Nagy
//             Refactored StdSocket into SocketSelect Randall Nagy 
//
#ifndef zSocketSelect_Hpp
#define zSocketSelect_Hpp

// NOTE: Borland uses the older version of windows sockets. Be sure to globally define
// _WINSOCKAPI_ to turn it off!

#include <stdnet.hpp>
#include <winsock2.h>

namespace stdnet {

    class SocketSelect {
    private:
        long timeout_ai; // This value is adjusted upward to accomidate service slowdowns, etc..
        int connected; // Please don't make the user worry about an "IsConnected()", okay?
        SOCKET port; // -the purpose of this class is to hide the gory details from the user,
        fd_set fdset; // no? -Enough to tell them that we're connected, or no, from the API result.

    protected:
        fd_set *OneFd(void);

    public:
        StdString sLastError;
        StdLog pwLog;

        SocketSelect(void);
        ~SocketSelect(void);

        bool IsConnected(void);

        // Notice that we default to INTERNET connections (AF_INET);
        bool Connect(const StdString& sHostName, u_short service_port = IPPORT_ECHO, int af_which = AF_INET);
        void Disconnect(void);

        bool Send(const StdString& std, long timeout = SOC_TIMEOUT);
        bool Receive(StdString&, size_t sz_largest_expected_from_service = SOC_BUFFER_SZ, long timeout = SOC_TIMEOUT);

        // Response will only contain what is available on-the-socket (i.e: might not be the ENTIRE host response (a polling / latency thing));
        bool Transact(const StdString& in, StdString& out, long timeout = SOC_TIMEOUT);

        // Response will contain `delim`, and perhaps a little more (depending on host response);
        bool TransactResponseContaining(const StdString& stdIn, StdString& stdOut, const char delim = '\n', long l_timeout = SOC_TIMEOUT);

        const char *GetLastError(void);
        int GetLastErrorCode(void);

        // This is useful for multiple operations on many sockets (i.e. select() that uses a passle of SocketSelects);

        operator SOCKET(void) {
            return port;
        }
    };
}

#endif
