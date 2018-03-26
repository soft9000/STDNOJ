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

using namespace stdnet;

Socket::Socket(void) : connected(0), timeout_ai(SOC_TIMEOUT) {
    pwLog.Disable(); // start with the logging OFF -
}

Socket::~Socket(void) {
    if (connected)
        Disconnect();
}

bool Socket::Connect(const StdString& sHostName, u_short service_port, int af_which) {
    if (connected)
        Disconnect();

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

    /*
    htonl converts from host-to-network byte order operating on long unsigned_integers. 
    htons converts from host-to-network byte order operating on short unsigned_integers. 
    ntohl converts from network-to-host byte order operating on long unsigned_integers. 
    ntohs converts from network-to-host byte order operating on short unsigned_integers.
     */
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

bool Socket::Send(const StdString& std) {
    int sz = send(port, std.c_str(), strlen(std.c_str()), 0);
    if (sz == SOCKET_ERROR) {
        stringstream srm;
        srm << "Socket::Send(ERROR) " << GetLastError();
        srm << ends;
        pwLog.PrintLine(PRESTO(srm).c_str());
        return false;
    }
    stringstream srm;
    srm << "Socket::Send(RESULT) " << std.c_str();
    srm << ends;
    pwLog.PrintLine(PRESTO(srm).c_str());
    return true;
}

bool Socket::Receive(StdString& std) {
    char *pbuf = new char[SOC_BUFFER_SZ + sizeof (NULL)];
    int sz = recv(port, pbuf, SOC_BUFFER_SZ, MSG_WAITALL);
    if (sz == SOCKET_ERROR) {
        delete [] pbuf;
        stringstream srm;
        srm << "Socket::Receive(ERROR) " << GetLastError();
        srm << ends;
        pwLog.PrintLine(PRESTO(srm).c_str());
        return false;
    }
    pbuf[sz] = NULL;
    std.assign(pbuf);
    delete [] pbuf;
    stringstream srm;
    srm << "Socket::Receive(RESULT) " << std.c_str();
    srm << ends;
    pwLog.PrintLine(PRESTO(srm).c_str());
    return true;
}

bool Socket::Transact(const StdString& in, StdString& out) {
    if (Send(in) == true && (Receive(out) == true))
        return true;
    return false;
}

void Socket::Disconnect(void) {
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

int Socket::GetLastError(void) const {
    return WSAGetLastError();
}

const char *Socket::GetError(StdString& sLastError, int iCode) const {
    // A successful function call, or a call to WSAGetLastError, does not reset the 
    // error code. To reset the error code, use the WSASetLastError function call 
    // with iError set to zero.
    switch (iCode) {
        case 6:
            // This is one that I see, which is not defined any where else;
            sLastError = "Observed host and / or service name mis-match";
            break;
        case WSAEWOULDBLOCK:
            sLastError = "WSAEWOULDBLOCK";
            break;
        case WSAEINPROGRESS:
            sLastError = "WSAEINPROGRESS";
            break;
        case WSAEALREADY:
            sLastError = "WSAEALREADY";
            break;
        case WSAENOTSOCK:
            sLastError = "WSAENOTSOCK";
            break;
        case WSAEDESTADDRREQ:
            sLastError = "WSAEDESTADDRREQ";
            break;
        case WSAEMSGSIZE:
            sLastError = "WSAEMSGSIZE";
            break;
        case WSAEPROTOTYPE:
            sLastError = "WSAEPROTOTYPE";
            break;
        case WSAENOPROTOOPT:
            sLastError = "WSAENOPROTOOPT";
            break;
        case WSAEPROTONOSUPPORT:
            sLastError = "WSAEPROTONOSUPPORT";
            break;
        case WSAESOCKTNOSUPPORT:
            sLastError = "WSAESOCKTNOSUPPORT";
            break;
        case WSAEOPNOTSUPP:
            sLastError = "WSAEOPNOTSUPP";
            break;
        case WSAEPFNOSUPPORT:
            sLastError = "WSAEPFNOSUPPORT";
            break;
        case WSAEAFNOSUPPORT:
            sLastError = "WSAEAFNOSUPPORT";
            break;
        case WSAEADDRINUSE:
            sLastError = "WSAEADDRINUSE";
            break;
        case WSAEADDRNOTAVAIL:
            sLastError = "WSAEADDRNOTAVAIL";
            break;
        case WSAENETDOWN:
            sLastError = "WSAENETDOWN";
            break;
        case WSAENETUNREACH:
            sLastError = "WSAENETUNREACH";
            break;
        case WSAENETRESET:
            sLastError = "WSAENETRESET";
            break;
        case WSAECONNABORTED:
            sLastError = "WSAECONNABORTED";
            break;
        case WSAECONNRESET:
            sLastError = "WSAECONNRESET";
            break;
        case WSAENOBUFS:
            sLastError = "WSAENOBUFS";
            break;
        case WSAEISCONN:
            sLastError = "WSAEISCONN";
            break;
        case WSAENOTCONN:
            sLastError = "WSAENOTCONN";
            break;
        case WSAESHUTDOWN:
            sLastError = "WSAESHUTDOWN";
            break;
        case WSAETOOMANYREFS:
            sLastError = "WSAETOOMANYREFS";
            break;
        case WSAETIMEDOUT:
            sLastError = "WSAETIMEDOUT";
            break;
        case WSAECONNREFUSED:
            sLastError = "WSAECONNREFUSED";
            break;
        case WSAELOOP:
            sLastError = "WSAELOOP";
            break;
        case WSAENAMETOOLONG:
            sLastError = "WSAENAMETOOLONG";
            break;
        case WSAEHOSTDOWN:
            sLastError = "WSAEHOSTDOWN";
            break;
        case WSAEHOSTUNREACH:
            sLastError = "WSAEHOSTUNREACH";
            break;
        case WSAENOTEMPTY:
            sLastError = "WSAENOTEMPTY";
            break;
        case WSAEPROCLIM:
            sLastError = "WSAEPROCLIM";
            break;
        case WSAEUSERS:
            sLastError = "WSAEUSERS";
            break;
        case WSAEDQUOT:
            sLastError = "WSAEDQUOT";
            break;
        case WSAESTALE:
            sLastError = "WSAESTALE";
            break;
        case WSAEREMOTE:
            sLastError = "WSAEREMOTE";
            break;
        case WSAEDISCON:
            sLastError = "WSAEDISCON";
            break;
        case WSASYSNOTREADY:
            sLastError = "WSASYSNOTREADY";
            break;
        case WSAVERNOTSUPPORTED:
            sLastError = "WSAVERNOTSUPPORTED";
            break;
        case WSANOTINITIALISED:
            sLastError = "WSANOTINITIALISED";
            break;
        default:
            sLastError = "Unknown error";
            break;
    }
    return sLastError.c_str();
}

bool Socket::IsConnected(void) {
    if (connected)
        return true;
    return false;
}
