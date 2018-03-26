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
#include <stdnoj.hpp>

using namespace stdnoj;

Url::Url(void) : uType(url_none) {
}

bool Url::Name(const StdString& sParam) {
    uType = url_none;

    StdString str = sParam;
    size_t whence = str.find(':'); // Is this right? Always??
    if (whence == npos)
        return false;
    StdString sub = str.substr(0, whence);
    sub.to_lower();
    sub.strip();
    bool br = false;
    if (sub == "http") {
        br = true;
        uType = url_http;
    }
    if (sub == "https") {
        br = true;
        uType = url_https;
    }
    if (sub == "ftp") {
        br = true;
        uType = url_ftp;
    }
    if (sub == "file") {
        br = true;
        uType = url_file;
    }
    if (br == true)
        sUrl = sParam;
    return br;
}

bool Url::Name(const File& file) {
    StdString str = file.Name();
    str.replace(PathChar(), '/'); // Is this right? Always??
    str.prepend("file://");
    return Name(str);
}

bool Url::Name(const Directory& dir) {
    StdString str = dir.Name();
    str.replace(PathChar(), '/');
    str.prepend("file://");
    return Name(str);
}

bool Url::FromString(const StdString& str) {
    return Name(str);
}

const char *Url::ToString(StdString& str) const {
    str = Name();
    return str.c_str();
}

StdString Url::Site(void) const {
    StdString sDelim = "://";
    StdString str;
    if (uType == url_none)
        return str;
    str = Name();
    size_t pos = str.find(sDelim);
    if (pos != npos)
        str.remove_pos(0, pos + sDelim.length());
    pos = str.find('/');
    if (pos != npos)
        str = str.subpos(0, pos);
    return str;
}

StdString Url::Page(void) const {
    StdString sDelim = "://";
    StdString str;
    if (uType == url_none)
        return str;
    str = Name();
    size_t pos = str.find(sDelim);
    if (pos != npos)
        str.remove_pos(0, pos + sDelim.length());
    pos = str.find('/');
    if (pos != npos)
        str.remove_pos(0, pos);
    return str;
}


#ifdef UNIX
#include <core/unix/unix_Url.cpp>
#endif

#ifdef MSDOS
#include <core/msdos/msdos_Url.cpp>
#endif

#ifdef WIN32
#include <core/win32/win32_Url.cpp>
#endif


