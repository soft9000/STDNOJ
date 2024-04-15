/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2024 R. A. Nagy 

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
// Class nojnet::Socket: Tired of all of the #ifdef garbage? 
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
// 09/15/2006: Refactored SocketSelect into Socket,    R. Nagy 
//
#ifndef zSocket78_Hpp
#define zSocket78_Hpp

// NOTE: Borland uses the older version of windows sockets. Be sure to globally define
// _WINSOCKAPI_ to turn it off!

#include <extra/stdlog.hpp>
using namespace stdnoj;

#include <stdnet/URL.hpp>
#include <stdnet/CommBase.hpp>
#include <winsock2.h>

// msdn.microsoft.com recv() doc SAYS that they define it in winsock2.h - But they do not -
#ifndef MSG_WAITALL
//#define MSG_WAITALL 0x8 - // Modern (2005) = Constant from MS SDK - STILL DOES NOT WORK!
#define MSG_WAITALL 0       // Classic - WINSOCK, WINSOCK2, and now WS2_36 !
#endif

// A micro-smurff extension (winsoc2) -
#define SOC_BUFFER_SZ  SO_MAX_MSG_SIZE

namespace stdnet {

    class Socket : public CommBase {
    private:
        long timeout_ai; // This value is adjusted upward to accomidate service slowdowns, etc..
        int connected; // Please don't make the user worry about an "IsConnected()", okay?
        SOCKET port; // -the purpose of this class is to hide the gory details from the user,
        fd_set fdset; // no? -Enough to tell them that we're connected, or no, from the API result.

    public:
        StdLog pwLog;

        Socket(void);
        virtual ~Socket(void);

        bool IsConnected(void);

        bool Connect(const StdString& sHostName, u_short service_port = IPPORT_ECHO, int af_which = AF_INET);
        void Disconnect(void);

        bool Send(const StdString& str);
        bool Receive(StdString&);
        bool Transact(const StdString& in, StdString& out);

        const char *GetError(StdString& sResult, int iCode) const;
        int GetLastError(void) const;

        // This is useful for management by external routines;

        operator SOCKET(void) {
            return port;
        }
    };
}

#endif
