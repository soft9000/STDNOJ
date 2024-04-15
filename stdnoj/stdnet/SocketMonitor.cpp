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
#include <stdnet.hpp>

using namespace stdnet;

void SocketMonitor::LOG(ostream& cout, const char *pszBuf, int capture) {
    if (pwSocket.pwLog.IsLogging()) {
        LOG(cout, pszBuf, capture);
        return;
    } else
        cout << pszBuf;
}

void SocketMonitor::LOG_LINE(const char *pszBuf) {
    if (pwSocket.pwLog.IsLogging()) {
        StdString str = pwSocket.pwLog.Format(pszBuf);
        cout << str << endl;
    }
}

void SocketMonitor::LOG_LINE(ostream& cout, const char *pszBuf, int capture) {
    if (pwSocket.pwLog.IsLogging() && capture) {
        LOG_LINE(cout, pszBuf, capture);
        return;
    } else {
        if (pwSocket.pwLog.IsLogging()) {
            StdString str;
            if (capture)
                str = pwSocket.pwLog.Format(pszBuf);
            else
                str = pwSocket.pwLog.PrintLine(pszBuf);
            cout << str << endl;
        } else
            cout << pszBuf << endl;
    }
}

int SocketMonitor::GETLINE(istream& cin, ostream& cout, char *pszBuf, size_t szBuf, int& capture, int& converse) {
    // <<<WIP>>> This needs to be character-oriented before we do anything commercial!!
    cin.getline(pszBuf, szBuf);

    StdString sBuf;

    // *****
    // CHECK FOR USER REQUESTS:
    switch (pszBuf[0]) {
        case '!':
            LOG_LINE(cout, "Monitor mode terminated", capture);
            return 0;
        case '@':
            LOG_LINE(cout, pwSocket.GetError(sBuf, GetLastError()), capture);
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

void SocketMonitor::MonitorConnection(Socket& soc, ostream& cout, istream& cin) {
    pwSocket = soc;

    int capture = 0;
    int converse = 1;
    StdString str;

    LOG_LINE(cout, "MONITOR MODE", capture);
    LOG_LINE(cout, "~~~~~~~~~~~~", capture);

    if (pwSocket.IsConnected() == false) {
        LOG_LINE(cout, "ERROR: No connection established. Cannot monitor socket.", capture);
        return;
    }

    LOG_LINE(cout, "Enter \"?\" for help, capture", capture);

    char *pszBuf = new char[1024];
    *pszBuf = NULL;
    int running = 1;
    while (running) {
        str = "";
        // *****
        // PROMPT && BUFFER-KEY-IN:
        if (capture)
            str.append("$ ");
        str.append("Monitor: ");

        LOG(cout, str.c_str(), capture);
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
            str.assign(" \r\n");
        else {
            str.assign(pszBuf);
            str.append("\r\n");
        }

        LOG_LINE(cout, str.c_str(), capture);

        // *****
        // TRANSACTION ROUND-TRIP;
        if (pwSocket.Send(str) == false)
            str.assign("Socket: SEND FAILED / TIMEDOUT");
        else
            if (converse && pwSocket.Receive(str) == false)
            str.assign("Socket: RECEIVE FAILED / TIMEDOUT");

        LOG_LINE(cout, str.c_str(), capture);
    }
    delete [] pszBuf;
}

