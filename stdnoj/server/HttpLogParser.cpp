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
#include <Server/HttpLogParser.hpp>

StdString HttpLogEntry::FileName(void) const {
    StdString str = sRequest;
    size_t whence = str.find(' ');
    str.remove_pos(0, whence + 1);
    whence = str.rfind(' '); // well, why not?
    str.remove(whence);
    return str;
}

bool HttpLogEntry::QueryDate(StdDateTime& sdt) const {
    // strftime 
    // strptime(sDateTime.c_str(), "%d/%b/%Y:%T", &rtm);
    char buf[100];
    int iDat = 0;
    int iYear = 0;
    int iHour = 0;
    int iMun = 0;
    int iSec = 0;

    StdString str = sDateTime.c_str();
    str.replace('[', ' ');
    str.replace(']', ' ');
    str.replace(':', ' ');
    str.replace('/', ' ');
    int iFields = ::sscanf(str.c_str(), " %2d %s %4d %d %d %d ", &iDat, buf, &iYear, &iHour, &iMun, &iSec);
    if (iFields != 6)
        return false;
    StdString sMonth = buf;
    sMonth.to_lower();
    int iMonth = 0;
    if (sMonth.find("jan") == 0L) {
        iMonth = 1;
        goto trer_NEXT;
    }
    if (sMonth.find("feb") == 0L) {
        iMonth = 2;
        goto trer_NEXT;
    }
    if (sMonth.find("mar") == 0L) {
        iMonth = 3;
        goto trer_NEXT;
    }
    if (sMonth.find("apr") == 0L) {
        iMonth = 4;
        goto trer_NEXT;
    }
    if (sMonth.find("may") == 0L) {
        iMonth = 5;
        goto trer_NEXT;
    }
    if (sMonth.find("jun") == 0L) {
        iMonth = 6;
        goto trer_NEXT;
    }
    if (sMonth.find("jul") == 0L) {
        iMonth = 7;
        goto trer_NEXT;
    }
    if (sMonth.find("aug") == 0L) {
        iMonth = 8;
        goto trer_NEXT;
    }
    if (sMonth.find("sep") == 0L) {
        iMonth = 9;
        goto trer_NEXT;
    }
    if (sMonth.find("oct") == 0L) {
        iMonth = 10;
        goto trer_NEXT;
    }
    if (sMonth.find("nov") == 0L) {
        iMonth = 11;
        goto trer_NEXT;
    }
    if (sMonth.find("dec") == 0L) {
        iMonth = 12;
        goto trer_NEXT;
    }
    if (!iMonth)
        return false;

trer_NEXT:

    StdString sError;
    sdt.EncodeDate(iYear, iMonth, iDat, sError);
    sdt.EncodeTime(iHour, iMun, iSec);
    return !(sError.is_null());
}

bool HttpLogEntry::Parameters(StdString& str, char ch) const {
    size_t whence = sRequest.rfind(ch);
    if (whence == NPOS)
        return false;
    str = sRequest.substr(whence, NPOS);
    return true;
}
// Attempts to remove odd characters and other common problems. 
// false if unexpected data / format is encountered.

bool HttpLogEntry::Normalize(void) {
    sRequest.mkwhite();
    sRequest.strip();
    while (sRequest.replace("  ", " "))
        ;
    return true;
}

int HttpLogEntry::operator==(const HttpLogEntry& ref) const {
    return (sRequest == ref.sRequest);
}

int HttpLogEntry::operator!=(const HttpLogEntry& ref) const {
    return (sRequest != ref.sRequest);
}

int HttpLogEntry::operator>(const HttpLogEntry& ref) const {
    return (sRequest > ref.sRequest);
}

istream& HttpLogEntry::log_import(istream& is) {
    StdString str;
    str.readline(is);
    if (!is)
        return is;

    stringstream srm;
    srm << str << ends;
    srm.seekg(0, ios::beg);

    srm >> sRemoteHost;
    srm >> sRemoteLogname;
    srm >> sRemoveUser;
    sDateTime.readline(srm, '"');
    sDateTime.remove_pattern("\"");
    sRequest.readline(srm, '\"');
    sRequest.remove_pattern("\"");
    srm >> sResultCode;
    srm >> sBytesSent;
    return is;
}

ostream& HttpLogEntry::log_export(ostream& os) {
    os << sRemoteHost << ' ';
    os << sRemoteLogname << ' ';
    os << sRemoveUser << ' ';
    os << sDateTime << " \"";
    os << sRequest << "\" ";
    os << sResultCode << ' ';
    os << sBytesSent << endl;
    return os;
}

istream& HttpLogEntry::read(istream& is) {
    Array<StreamString> ary;
    ary.read(is);
    sRemoteHost = ary[0];
    sRemoteLogname = ary[1];
    sRemoveUser = ary[2];
    sDateTime = ary[3];
    sRequest = ary[4];
    sResultCode = ary[5];
    sBytesSent = ary[6];
    return is;
}

ostream& HttpLogEntry::write(ostream& os) {
    Array<StreamString> ary;
    ary.AddZombies(7);
    ary[ary.Nelem()] = sRemoteHost;
    ary[ary.Nelem()] = sRemoteLogname;
    ary[ary.Nelem()] = sRemoveUser;
    ary[ary.Nelem()] = sDateTime;
    ary[ary.Nelem()] = sRequest;
    ary[ary.Nelem()] = sResultCode;
    ary[ary.Nelem()] = sBytesSent;
    return ary.write(os);
}

istream& HttpLog::log_import(istream& is) {
    HttpLogEntry line;
    while (is) {
        line.log_import(is);
        if (is)
            aLines[aLines.Nelem()] = line;
    }
    return is;
}

ostream& HttpLog::log_export(ostream& os) {
    HttpLogEntry line;
    for (size_t ss = 0L; ss < aLines.Nelem(); ss++)
        line.log_export(os);
    return os;
}

bool HttpLog::Normalize(void) {
    if (bNormalized == true)
        return bNormalized;
    bool br = true;
    for (size_t ss = 0L; ss < aLines.Nelem(); ss++) {
        if (aLines[ss].Normalize() == false)
            br = false;
    }
    bNormalized = br;
    return br;
}

istream& HttpLog::read(istream& is) {
    aLines.read(is);
    return is;
}

ostream& HttpLog::write(ostream& os) {
    aLines.write(os);
    return os;
}

bool HttpLogParser::Import(HttpLog& hLog, File& file) {
    sError = "";
    if (file.Exists() == false) {
        sError = "File not found.";
        return false;
    }

    hLog.aLines.AddZombies(NumLines(file)); // this is first open attempt

    istream& is = file.OpenRead();
    if (!is) {
        sError = "File is empty";
        return false;
    }

    hLog.log_import(is);
    file.Close();
    return true;
}

bool HttpLogParser::Export(HttpLog& hLog, File& file) {
    bool br = true;
    sError = "";
    ostream& os = file.OpenWrite();
    if (!os) {
        sError = "File creation error";
        br = false;
    }
    hLog.log_export(os);
    if (!os) {
        sError = "File exportation error";
        br = false;
    }
    file.Close();
    return br;
}
