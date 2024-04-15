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


// Class StdSocket: This class is depricated. Use stdnet, instead.

#include <sockets/stdsocket.hpp>

void StdSocket::LOG(ostream& cout, const char *pszBuf, int capture) {
    if (!pTrace && capture) {
        pTrace = new StdLog("capture.dat");
        LOG(cout, pszBuf, capture);
        delete pTrace;
        pTrace = NULL;
        return;
    } else {
        if (pTrace)
            cout << pTrace->Format(pszBuf);
        else
            cout << pszBuf;
    }
}

void StdSocket::LOG_LINE(const char *pszBuf) {
    if (pTrace) {
        StdString str = pTrace->Format(pszBuf);
        cout << str << endl;
    }
}

void StdSocket::LOG_LINE(ostream& cout, const char *pszBuf, int capture) {
    if (!pTrace && capture) {
        pTrace = new StdLog("capture.dat");
        LOG_LINE(cout, pszBuf, capture);
        delete pTrace;
        pTrace = NULL;
        return;
    } else {
        if (pTrace) {
            StdString str;
            if (capture)
                str = pTrace->Format(pszBuf);
            else
                str = pTrace->PrintLine(pszBuf);
            cout << str << endl;
        } else
            cout << pszBuf << endl;
    }
}

int StdSocket::GETLINE(istream& cin, ostream& cout, char *pszBuf, size_t szBuf, int& capture, int& converse) {
    // <<<WIP>>> This needs to be character-oriented before we do anything commercial!!
    cin.getline(pszBuf, szBuf);

    // *****
    // CHECK FOR USER REQUESTS:
    switch (pszBuf[0]) {
        case '!':
            LOG_LINE(cout, "Monitor mode terminated", capture);
            return 0;
        case '@':
            LOG_LINE(cout, GetLastError(), capture);
            break;
        case '+':
            capture = !capture;
            if (capture)
                LOG_LINE(cout, "CAPUTRE ON", capture);
            else
                LOG_LINE(cout, "capture off", capture);
            break;
        case '~':
            converse = !converse;
            if (converse)
                LOG_LINE(cout, "CONVERSATIONAL MODE = OFF", converse);
            else
                LOG_LINE(cout, "CONVERSATIONAL MODE = ON ", converse);
            break;
        case '?':
            LOG_LINE(cout, "Enter \"!\" to stop", capture);
            LOG_LINE(cout, "Enter \"+\" to toggle capture", capture);
            LOG_LINE(cout, "Enter \"@\" to see error state", capture);
            LOG_LINE(cout, "Enter \"~\" to toggle listen-after-send (default = ON)", capture);
            LOG_LINE(cout, "Enter \"?\" to see this help message", capture);
            break;

        default:
            return 2;
    }
    return 1;
}

StdSocket::StdSocket(void) : connected(0), pTrace(NULL), timeout_ai(SOC_TIMEOUT) {
}

StdSocket::~StdSocket(void) {
    if (connected)
        Disconnect();
    delete pTrace;
}

bool StdSocket::Connect(const StdString& sHostName, u_short service_port, int af_which) {
    WORD wVersionRequested;
    WSADATA wsaData;

    wVersionRequested = MAKEWORD(1, 1); //2, 0 );

    int err = ::WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
        return false;
#if 0 
    /* Confirm that the WinSock DLL supports 2.0.        */
    /* Note that if the DLL supports versions greater    */
    /* than 2.0 in addition to 2.0, it will still return */
    /* 2.0 in wVersion since that is the version we      */
    /* requested.                                        */

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0) {
        /* Tell the user that we couldn't find a useable */
        /* WinSock DLL.                                  */
        Disconnect();
        return false;
    }
#endif 
    /* The WinSock DLL is acceptable. Proceed. */

    port = socket(af_which, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof (struct sockaddr_in));

    struct hostent *host = gethostbyname(sHostName.c_str());
    if (!host)
        return false;

    addr.sin_family = af_which;
    addr.sin_port = htons(service_port);
    addr.sin_addr.s_addr = *(unsigned long *) host->h_addr_list[0];
    err = ::connect(port, (struct sockaddr *) & addr, sizeof (addr));
    if (err != 0) {
        Disconnect();
        return false;
    }

    connected = 1;
    return true;
}

bool StdSocket::Send(const StdString& std, long timeout) {
    // If the user is defaulting, then we are free to use timeout_ai;
    if (timeout == SOC_TIMEOUT && (timeout_ai != SOC_TIMEOUT))
        timeout = timeout_ai;

    struct timeval tmv;
    memset(&tmv, 0, sizeof (timeval));

    tmv.tv_sec = timeout;
    tmv.tv_usec = NULL;
    switch (select(NULL, NULL, OneFd(), NULL, &tmv)) {
        case 0: // timeout: data not available.
        {
            if (timeout_ai < 100L)
                timeout_ai += 5L;

            stringstream srm;
            srm << "StdSocket::select(TIMEOUT) next timeout is " << timeout_ai;
            srm << ends;
            LOG_LINE(PRESTO(srm).c_str());
        }
            return false;
        case SOCKET_ERROR: // problem: check out error codes.
        {
            stringstream srm;
            srm << "StdSocket::select(ERROR) " << GetLastError();
            srm << ends;
            LOG_LINE(PRESTO(srm).c_str());
        }
            return false;
        default: // okay: data available to read.
            break;
    }

    int sz = send(port, std.c_str(), strlen(std.c_str()), 0);
    if (sz == SOCKET_ERROR) {
        stringstream srm;
        srm << "StdSocket::Send(ERROR) " << GetLastError();
        srm << ends;
        LOG_LINE(PRESTO(srm).c_str());
        return false;
    }
    stringstream srm;
    srm << "StdSocket::Send(RESULT) " << std.c_str();
    srm << ends;
    LOG_LINE(PRESTO(srm).c_str());
    return true;
}

bool StdSocket::Receive(StdString& std, size_t sz_alloc_to, long timeout) {
    // If the user is defaulting, the we are free to use timeout_ai;
    if (timeout == SOC_TIMEOUT && (timeout_ai != SOC_TIMEOUT))
        timeout = timeout_ai;

    struct timeval tmv;
    memset(&tmv, 0, sizeof (timeval));

    tmv.tv_sec = timeout;
    tmv.tv_usec = NULL;
    switch (select(NULL, OneFd(), NULL, NULL, &tmv)) {
        case 0: // timeout: data not available.
        {
            if (timeout_ai < 100L)
                timeout_ai += 5L;
            stringstream srm;
            srm << "StdSocket::select(TIMEOUT) next timeout is " << timeout_ai;
            srm << ends;
            LOG_LINE(PRESTO(srm).c_str());
        }
            return false;
        case SOCKET_ERROR: // problem: check out error codes.
        {
            stringstream srm;
            srm << "StdSocket::select(ERROR) " << GetLastError();
            srm << ends;
            LOG_LINE(PRESTO(srm).c_str());
        }
            return false;
        default: // okay: data available to read.
            break;
    }

    char *pbuf = new char[sz_alloc_to + sizeof (NULL)];
    int sz = recv(port, pbuf, sz_alloc_to, 0);
    if (sz == SOCKET_ERROR) {
        delete [] pbuf;
        stringstream srm;
        srm << "StdSocket::Receive(ERROR) " << GetLastError();
        srm << ends;
        LOG_LINE(PRESTO(srm).c_str());
        return false;
    }
    pbuf[sz] = NULL;
    std.assign(pbuf);
    delete [] pbuf;
    stringstream srm;
    srm << "StdSocket::Receive(RESULT) " << std.c_str();
    srm << ends;
    LOG_LINE(PRESTO(srm).c_str());
    return true;
}

bool StdSocket::TransactResponseContaining(const StdString& stdIn, StdString& stdOut, const char delim, long timeout) {
    if (Send(stdIn, timeout) == false)
        return false;
    if (Receive(stdOut, SOC_BUFFER_SZ, timeout) == false)
        return false;
    // This uniquely builds the line;
    while (stdOut.find(delim) == NPOS) {
        StdString std2;
        if (Receive(std2, SOC_BUFFER_SZ, timeout) == false)
            return false;
        stdOut.append(std2);
    }
    return true;
}

bool StdSocket::Transact(const StdString& in, StdString& out, long timeout) {
    if (Send(in, timeout) == true && (Receive(out, SOC_BUFFER_SZ, timeout) == true))
        return true;
    return false;
}

void StdSocket::Disconnect(void) {
    if (!connected)
        return;
    /*
 Any sockets open when WSACleanup is called are reset and automatically deallocated as if 
 closesocket was called; sockets which have been closed with closesocket but which still 
 have pending data to be sent may be affected--the pending data may be lost if the Windows 
 Sockets DLL is unloaded from memory as the application exits. To insure that all pending 
 data is sent an application should use shutdown to close the connection, then wait until 
 the close completes before calling closesocket and WSACleanup. All resources and internal 
 state, such as queued un-posted messages, must be deallocated so as to be available to the 
 next user.

 There must be a call to WSACleanup for every successful call to WSAStartup made by a task. 
 Only the final WSACleanup for that task does the actual cleanup; the preceding calls simply 
 decrement an internal reference count in the Windows Sockets DLL.
     */
    closesocket(port);

    ::WSACleanup();
    /*
 Note that shutdown does not close the socket, and resources attached to the socket 
 will not be freed until closesocket is invoked. To assure that all data is sent and
 received on a connected socket before it is closed, an application should use shutdown
 to close connection before calling closesocket. For example, to initiate a graceful 
 disconnect, an application could:

 call WSAAsyncSelect to register for FD_CLOSE notification
 call shutdown with how=SD_SEND

 when FD_CLOSE received, call recv until 0 returned, or SOCKET_ERROR
 call closesocket
     */
    connected = 0;
}

int StdSocket::GetLastErrorCode(void) {
    return WSAGetLastError();
}

fd_set *StdSocket::OneFd(void) {
    fdset.fd_count = 1; /* how many are SET? */
    fdset.fd_array[0] = port; /* an array of SOCKETs */
    return &fdset;
}

void StdSocket::MonitorConnection(ostream& cout, istream& cin) {
    int capture = 0;
    int converse = 1;
    StdString std;

    LOG_LINE(cout, "MONITOR MODE", capture);
    LOG_LINE(cout, "~~~~~~~~~~~~", capture);

    if (!connected) {
        LOG_LINE(cout, "ERROR: No connection established. Cannot monitor socket.", capture);
        return;
    }

    LOG_LINE(cout, "Enter \"?\" for help, capture", capture);

    char *pszBuf = new char[1024];
    *pszBuf = NULL;
    int running = 1;
    while (running) {
        std = "";
        // *****
        // PROMPT && BUFFER-KEY-IN:
        if (capture)
            std.append("$ ");
        std.append("Monitor: ");

        LOG(cout, std.c_str(), capture);
        pszBuf[0] = NULL;

        // *****
        // GET USER RESPONSE & PROCESS ANY USER MACROS:
        running = GETLINE(cin, cout, pszBuf, 1024, capture, converse);
        switch (running) {
            case 0:
            case 1:
                continue;

            case 2:
            default:
                break;
        }

        // *****
        // PROCESS INTERNET-FORMATTED COMMAND SEQUENCE (i.e: use <CR><LF>)
        size_t sz = ::strlen(pszBuf);
        pszBuf[sz] = NULL;

        if (!(*pszBuf))
            std.assign(" \r\n");
        else {
            std.assign(pszBuf);
            std.append("\r\n");
        }

        LOG_LINE(cout, std.c_str(), capture);

        // *****
        // TRANSACTION ROUND-TRIP;
        if (Send(std) == false)
            std.assign("Socket: SEND FAILED / TIMEDOUT");
        else
            if (converse && Receive(std) == false)
            std.assign("Socket: RECEIVE FAILED / TIMEDOUT");

        LOG_LINE(cout, std.c_str(), capture);
    }
    delete [] pszBuf;
}

const char *StdSocket::GetLastError(void) {
    /*
    A successful function call, or a call to WSAGetLastError, does not reset the 
    error code. To reset the error code, use the WSASetLastError function call 
    with iError set to zero.
     */
    char *pszErr = NULL;
    int err = WSAGetLastError();
    switch (err) {
        case 6:
            // This is one that I see, which is not defined any where else;
            pszErr = "Observed host and / or service name mis-match";
            break;
        case WSAEWOULDBLOCK:
            pszErr = "WSAEWOULDBLOCK";
            break;
        case WSAEINPROGRESS:
            pszErr = "WSAEINPROGRESS";
            break;
        case WSAEALREADY:
            pszErr = "WSAEALREADY";
            break;
        case WSAENOTSOCK:
            pszErr = "WSAENOTSOCK";
            break;
        case WSAEDESTADDRREQ:
            pszErr = "WSAEDESTADDRREQ";
            break;
        case WSAEMSGSIZE:
            pszErr = "WSAEMSGSIZE";
            break;
        case WSAEPROTOTYPE:
            pszErr = "WSAEPROTOTYPE";
            break;
        case WSAENOPROTOOPT:
            pszErr = "WSAENOPROTOOPT";
            break;
        case WSAEPROTONOSUPPORT:
            pszErr = "WSAEPROTONOSUPPORT";
            break;
        case WSAESOCKTNOSUPPORT:
            pszErr = "WSAESOCKTNOSUPPORT";
            break;
        case WSAEOPNOTSUPP:
            pszErr = "WSAEOPNOTSUPP";
            break;
        case WSAEPFNOSUPPORT:
            pszErr = "WSAEPFNOSUPPORT";
            break;
        case WSAEAFNOSUPPORT:
            pszErr = "WSAEAFNOSUPPORT";
            break;
        case WSAEADDRINUSE:
            pszErr = "WSAEADDRINUSE";
            break;
        case WSAEADDRNOTAVAIL:
            pszErr = "WSAEADDRNOTAVAIL";
            break;
        case WSAENETDOWN:
            pszErr = "WSAENETDOWN";
            break;
        case WSAENETUNREACH:
            pszErr = "WSAENETUNREACH";
            break;
        case WSAENETRESET:
            pszErr = "WSAENETRESET";
            break;
        case WSAECONNABORTED:
            pszErr = "WSAECONNABORTED";
            break;
        case WSAECONNRESET:
            pszErr = "WSAECONNRESET";
            break;
        case WSAENOBUFS:
            pszErr = "WSAENOBUFS";
            break;
        case WSAEISCONN:
            pszErr = "WSAEISCONN";
            break;
        case WSAENOTCONN:
            pszErr = "WSAENOTCONN";
            break;
        case WSAESHUTDOWN:
            pszErr = "WSAESHUTDOWN";
            break;
        case WSAETOOMANYREFS:
            pszErr = "WSAETOOMANYREFS";
            break;
        case WSAETIMEDOUT:
            pszErr = "WSAETIMEDOUT";
            break;
        case WSAECONNREFUSED:
            pszErr = "WSAECONNREFUSED";
            break;
        case WSAELOOP:
            pszErr = "WSAELOOP";
            break;
        case WSAENAMETOOLONG:
            pszErr = "WSAENAMETOOLONG";
            break;
        case WSAEHOSTDOWN:
            pszErr = "WSAEHOSTDOWN";
            break;
        case WSAEHOSTUNREACH:
            pszErr = "WSAEHOSTUNREACH";
            break;
        case WSAENOTEMPTY:
            pszErr = "WSAENOTEMPTY";
            break;
        case WSAEPROCLIM:
            pszErr = "WSAEPROCLIM";
            break;
        case WSAEUSERS:
            pszErr = "WSAEUSERS";
            break;
        case WSAEDQUOT:
            pszErr = "WSAEDQUOT";
            break;
        case WSAESTALE:
            pszErr = "WSAESTALE";
            break;
        case WSAEREMOTE:
            pszErr = "WSAEREMOTE";
            break;
        case WSAEDISCON:
            pszErr = "WSAEDISCON";
            break;
        case WSASYSNOTREADY:
            pszErr = "WSASYSNOTREADY";
            break;
        case WSAVERNOTSUPPORTED:
            pszErr = "WSAVERNOTSUPPORTED";
            break;
        case WSANOTINITIALISED:
            pszErr = "WSANOTINITIALISED";
            break;
        default:
            pszErr = "Unknown error";
            break;
    }
    return pszErr;
}
