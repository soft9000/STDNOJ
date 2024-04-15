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
// 10/24/1998, Ran through NOTAB, R. Nagy
//

#include "Email.hpp"

#define BEGIN {
#define END }

void ffMkLocalFileName(StdString& std, bool bSkipPathChar = false) {
    for (size_t ss = NULL; ss < std.length(); ss++) {
        switch (std[ss]) {
            case '\\':
                if (bSkipPathChar == true)
                    continue;
            case ':':
                if ((bSkipPathChar == true) && ss == 1)
                    continue;
            case '<':
            case '>':
            case '*':
            case '?':
            case '/':
            case '\"':
                std[ss] = '_';
            default:
                continue;
        }
    }
}

inline bool FileExists(const StdString& str) {
    File file;
    file.Name(str);
    return file.Exists();
}

inline bool CopyFile(const StdString& sSource, const StdString& sDest) {
    File file;
    file.Name(sSource);
    return file.CopyTo(sDest);
}

bool ARTRENAME(EMailHeader& em, const StdString& sExtension, bool bDeleteOriginalFile) {
    bool br = false;
    if (em._local_name.is_null())
        return br;

    StdString sDir;
    size_t sz = em._local_name.rfind('\\');
    if (sz != NPOS) {
        sDir = em._local_name;
        sDir.remove(sz + 1);
    }

    StdString sNextName;
    em.MkLocalName(sNextName, false);
    if (sNextName.is_null())
        return br;

    if (!sDir.is_null())
        sNextName.prepend(sDir);

    StdString sName2;
    unsigned long file_count = NULL;
    do {
        stringstream srm;
        srm << sNextName;
        if (file_count) {
            srm << '.';
            srm << file_count;
        }

        if (sExtension[0] != '.')
            srm << '.';
        srm << sExtension;

        sName2 = PRESTO(srm);

        file_count++;
        if (!file_count)
            return false;

        if ((em._local_name == sName2) && (bDeleteOriginalFile == false)) {
            // The idea here is that we are NOT merely DUPLICATING this file
            // out to a redundant ".000x"-name for itself! (eeeewwwwwh)
            return false;
        }

    } while (FileExists(sName2));

    if (CopyFile(em._local_name, sName2) == false)
        return br;

    br = true;
    if (bDeleteOriginalFile == true) {
        File file(em._local_name);
        br = file.Remove();
    }

    em._local_name = sName2;
    return br;
}

StdString ARTNAME(Directory& dir, EMail& em, const StdString& sExtension) {
    return ARTNAME(dir.Name(), em, sExtension);
}

StdString ARTNAME(const char *pszDir, EMail& em, const StdString& sExtension) {
    /*
    // NO! Re-calculate it each time! An prior Import() use could mess-us up
    // if we defer to it, and we are now re-naming (i.e: You just don't know)!
    StdString std = em.GetLocalName();
    if(!std.is_null())
       return std;
     */

    // MkLocalName(std) considers the message-ID first; We
    // want a SUBJECT based opportunity, so we check that, first;
    StdString std = em.Subject();
    if (std.is_null() || std.is_space())
        em.MkLocalName(std);
    else
        ffMkLocalFileName(std);

    StdString sRes;
    sRes.assign(pszDir);
    sRes.append("\\");
    sRes.append(std.c_str());
    sRes.append(sExtension.c_str());

    em.SetLocalName(sRes);
    std = em.GetLocalName(); // \:(
    return std; // \:(
}

void AssignOrAppend(StdString& subj, const char *psz) {
    StdString str2 = psz;
    Cleanup(str2);
    if (subj == "")
        subj.assign(str2);
    else {
        subj.append(TERMINATE);
        subj.append(str2.c_str());
    }
}

EMailHeader::EMailHeader(void) {
}

EMailHeader::EMailHeader(const EMailHeader& im) {
    (*this) = im;
}

bool EMailHeader::IsNull(void) const {
    if (_to.is_null() && _from.is_null() && _subject.is_null())
        return true;
    return false;
}

bool EMailHeader::IsAddressSeparator(char ch) const {
    switch (ch) {
        case '<':
        case '>':
        case ' ':
        case '\t':
        case '\n':
        case '\r':
        case ',':
        case ';': // Microsoft does this...
            return true;
    }
    return false;
}

bool EMailHeader::AbsEmailAddress(StdString& std) const {
    // It happens that people have newlines and other unspeakable
    // nasties in their email addresses. Our job is simply to make 
    // the best guess here;
    std.mkwhite();

    // Okay: Now we can assume a somewhat tangendental reality;
    size_t whence = std.find('@');
    if (whence == NPOS)
        return false;

    size_t start = whence;
    while (start) {
        if (IsAddressSeparator(std[start]) == true) {
            start++;
            break;
        }
        start--;
    }
    std = &std[start]; // save start point

    size_t end = std.find('@');
    while (end < std.length()) {
        if (IsAddressSeparator(std[end]) == true) {
            std.remove(end); // terminate (atypical)
            break;
        }
        end++;
    }
    return true;
}

bool EMailHeader::ReturnAddress(StdString& std) const {
    // Example;
    //    "From: Rusty Alderson <cessna@phoenix.net>"
    std = From();
    return AbsEmailAddress(std);
}

bool EMailHeader::FormatAsAddress(StdString& std) const {
    StdString sHold = std;
    if (AbsEmailAddress(std) == false) {
        std = sHold;
        return false;
    }
    if (std.find('<') == NPOS) // maintenance
    {
        std.prepend('<');
        std.append('>');
    }
    return true;
}

bool EMailHeader::ContainsAbsEmailAddress(const StdString& sIn) {
    StdString sOut = sIn;
    return AbsEmailAddress(sOut);
}

bool EMailHeader::AbsEmailAddress(const StdString& sIn, StdString& sOut) const {
    sOut = sIn;
    return AbsEmailAddress(sOut);
    /*
    sOut = "";
    StdString std = sIn;
    std.mkwhite();
    std.strip();
    size_t which = std.rfind(' ');
    if(which != NPOS)
       {
       std = &std[which+1];
       std.strip();
       }

    sOut = std;
    if(sOut.find('@') == NPOS)
       return false;
    if(sOut.find('.') == NPOS)
       return false;

    return true;
     */
}

bool EMailHeader::IsMessageFile(void) const {
    if (_from.is_null() && _subject.is_null() && _date.is_null())
        return false;
    return true;
}

// If there is a pre-existing file name, we will over-write it with the contents
// of "EMail". Else, we will create a temp file-name, and write to that, instead.

bool EMailHeader::Destroy(void) {
    // STEP: Blow-away any local file;
    StdString sFileName = GetLocalName();
    if (!sFileName.is_null()) {
        File file;
        if (file.Name(sFileName) == false)
            return false;
        if (file.Remove() == false)
            return false;
    }

    // STEP: Blank us out;
    EMailHeader emh;
    *this = emh;
    return true;
}

// If there is a pre-existing file name, we will over-write it with the contents
// of "EMail". Else, we will create a temp file-name, and write to that, instead.

bool EMailHeader::RfcOverwrite(EMail& email) {
    StdString sFileName = email.GetLocalName();
    File file;
    if (file.MkUniqueName() == false)
        return false;
    if (file.Remove() == false)
        return false;
    return Export(file.Name());
}

bool EMailHeader::Import(const StdString& str) {
    File file;
    if (file.Name(str) == false)
        return false;
    return Import(file);
}

bool EMailHeader::Import(File &file) {
    if (!file.Exists())
        return false;

    bool br = rfc_read(file.OpenRead(File::AT_BINARY));

    // No matter what the result is, this is what we were asked to read;
    SetLocalName(file.Name());
    return br;
}

bool EMailHeader::Export(const StdString& str) {
    File file;
    if (file.Name(str) == false)
        return false;
    return Export(file);
}

bool EMailHeader::Export(File& file) {
    /*
 Path: ix.netcom.com!ix.netcom.com!ixnews1.ix.netcom.com!howland.reston.ans.net!newsfeed.internetmci.com!news.sprintlink.net!gryphon.phoenix.net!usenet
 From: Rusty Alderson <cessna@phoenix.net>
 Newsgroups: misc.jobs.offerred,misc.jobs.offered,fl.jobs,comp.lang.c++
 Subject: Re: $$$ C++ $$$ C++ $$$ C++ $$$ C++ $$$ C++ $$$ C++
 Date: Wed, 24 Apr 96 14:47:03 PDT
     */
    if (file.Exists() == true)
        return false;

    SetLocalName(file.Name());

    return rfc_write(file.OpenWrite(File::AT_BINARY));
}

bool EMailHeader::Dump(ofstream& ofs) const {
    int omode = SETMODE(ofs, bin_mode);
    bool br = rfc_write(ofs);
    SETMODE(ofs, omode);
    return br;
}

bool EMailHeader::write_mime_headers(ostream& os) const {
    // When using MIME, we cannot write the header until
    // we know how many PARTS we have, attachment type (etc).
    return rfc_write2(os, 0);
}

bool EMailHeader::rfc_write(ostream& ofs) const {
    return rfc_write2(ofs, 1);
}

bool EMailHeader::rfc_write2(ostream& ofs, int use_subject) const {
    // Write-out the article:
    StdString std;
    std = InternetPath();
    if (!std.is_null())
        ofs << "Path: " << std << TERMINATE;
    std = From();
    if (!std.is_null())
        ofs << "From: " << std << TERMINATE;
    std = To();
    if (!std.is_null())
        ofs << "To: " << std << TERMINATE;
    std = Newsgroups();
    if (!std.is_null())
        ofs << "Newsgroups: " << std << TERMINATE;
    if (use_subject) {
        std = Subject();
        if (!std.is_null())
            ofs << "Subject: " << std << TERMINATE;
    }
    std = Date();
    if (!std.is_null())
        ofs << "Date: " << std << TERMINATE;
    std = MessageID();
    if (!std.is_null())
        ofs << "Message-ID: " << std << TERMINATE;
    std = ContentType();
    if (!std.is_null())
        ofs << "Content-Type: " << std << TERMINATE;
    std = MimeVersion();
    if (!std.is_null())
        ofs << "MIME-Version: " << std << TERMINATE;

    if (!_unknown_headers.is_null())
        ofs << _unknown_headers.c_str() << TERMINATE;

    return true;
}

#ifdef __BCPLUSPLUS__
bool EMailHeader::ParseBinaryONLY(ifstream& ifs)
#else

bool EMailHeader::Parse(ifstream& ifs)
#endif
{
    int omode = SETMODE(ifs, bin_mode);
    bool br = rfc_read(ifs);
    SETMODE(ifs, omode);
    return br;
}

bool EMailHeader::rfc_read(istream& ifs) {
    BEGIN
    EMailHeader emh;
    *this = emh;
    END
    if (!ifs.good())
        return false;

    StdString *pLastWrite = &_unknown_headers;

    int parsing = NULL;
    StdString strLine;

    int done = 0;
    int times = NULL;

    while ((!ifs.eof()) && ifs && ifs.good()) {
        if (done)
            break;

        // Get the line;
        strLine = "";
        strLine.readline(ifs);

        // Finally, BINARY MODE IS GUARANTEED, so we can do THIS;
        if (IsEndOfHeader(strLine)) {
            // Usual terminal condition;
            done = 1;
            continue;
        }
        if (!ifs || ifs.eof()) {
            // UNusual terminal condition, but possible (eg: HEADER-ONLY PARSING)
            done = 1;
            continue;
        }
        if (strLine.is_null()) // This is what the STREAMS BUG in VC++ looks like.
        { //  You need to re-install BOTH Visual C++ 5.0 and NTSP 4.0.
            // UNusual terminal condition;
            if (parsing)
                done = 1;
            continue;
        }
        if (strLine[0] == '\t') {
            AssignOrAppend(*pLastWrite, &strLine[1]);
            continue;
        }

        size_t whence = strLine.find(':');
        if (whence == NPOS) {
            // Ensure that ";" parameters (such as boundary=)
            // are parsed properly... For example, for _ctype
            // Microsoft Outlook prefixes same with \r\n\t
            if (strLine.find("boundary=") != npos) {
                if (!_ctype.is_null()) {
                    strLine.mkwhite();
                    strLine.strip();
                    _ctype.mkwhite();
                    _ctype.strip();
                    if (_ctype.find(';') != _ctype.length() - 1)
                        _ctype.append(';');
                    _ctype.append(strLine);
                    continue;
                }
            }
            // Microsoft ignores RFC's;
            AssignOrAppend(_unknown_headers, &strLine[0]);
            continue;
        } else {
            // Parse the headers;
            StdString str = strLine.substr(0, whence + 1);
            if (str == "To:") {
                AssignOrAppend(_to, &strLine[whence + 1]);
                pLastWrite = &_to;
                parsing = 1;
                continue;
            }
            if (_date == "" && (str == "Date:")) {
                AssignOrAppend(_date, &strLine[whence + 1]);
                pLastWrite = &_date;
                parsing = 1;
                continue;
            }
            if (_from == "" && (str == "From:")) {
                AssignOrAppend(_from, &strLine[whence + 1]);
                pLastWrite = &_from;
                parsing = 1;
                continue;
            }
            if (_subject == "" && (str == "Subject:")) {
                AssignOrAppend(_subject, &strLine[whence + 1]);
                pLastWrite = &_subject;
                parsing = 1;
                continue;
            }
            if (_epath == "" && (str == "Path:")) {
                AssignOrAppend(_epath, &strLine[whence + 1]);
                pLastWrite = &_epath;
                parsing = 1;
                continue;
            }
            if (_newsg == "" && (str == "Newsgroups:")) {
                AssignOrAppend(_newsg, &strLine[whence + 1]);
                pLastWrite = &_newsg;
                parsing = 1;
                continue;
            }
            if (_messageid == "" && (str == "Message-ID:")) {
                AssignOrAppend(_messageid, &strLine[whence + 1]);
                pLastWrite = &_messageid;
                parsing = 1;
                continue;
            }
            if (_ctype == "" && (str == "Content-Type:")) {
                AssignOrAppend(_ctype, &strLine[whence + 1]);
                pLastWrite = &_ctype;
                parsing = 1;
                continue;
            }
            if (_mime_ver == "" && (str == "Mime-Version:")) {
                AssignOrAppend(_mime_ver, &strLine[whence + 1]);
                pLastWrite = &_mime_ver;
                parsing = 1;
                continue;
            }
            if (_mime_ver == "" && (str == "MIME-Version:")) {
                AssignOrAppend(_mime_ver, &strLine[whence + 1]);
                pLastWrite = &_mime_ver;
                parsing = 1;
                continue;
            }
        }

        // DEFAULT:
        // We are not interested in the header, so we just put it into;
        AssignOrAppend(_unknown_headers, &strLine[0]);
        pLastWrite = &_unknown_headers;
    } // ewhile

    if (!parsing)
        return false;

    return true;
}

ostream& EMailHeader::write(ostream& os) {
    _epath.write_stream(os);
    _newsg.write_stream(os);
    _messageid.write_stream(os);
    _to.write_stream(os);
    _from.write_stream(os);
    _subject.write_stream(os);
    _date.write_stream(os);
    _unknown_headers.write_stream(os);
    return os;
}

istream& EMailHeader::read(istream& is) {
    _epath.read_stream(is);
    _newsg.read_stream(is);
    _messageid.read_stream(is);
    _to.read_stream(is);
    _from.read_stream(is);
    _subject.read_stream(is);
    _date.read_stream(is);
    _unknown_headers.read_stream(is);
    return is;
}

int EMailHeader::operator==(const EMailHeader& im) const {
    if ((im._date == _date) &&
            (im._from == _from) &&
            (im._subject == _subject) &&
            (im._to == _to))
        return 1;
    return 0;
}

EMailHeader& EMailHeader::operator=(const EMailHeader& im) {
    if (this == &im)
        return *this;
    _local_name = im._local_name;
    _epath = im._epath;
    _from = im._from;
    _newsg = im._newsg;
    _messageid = im._messageid;
    _subject = im._subject;
    _date = im._date;
    _to = im._to;
    _ctype = im._ctype;
    _mime_ver = im._mime_ver;
    _unknown_headers = im._unknown_headers; // 5/30/2002 (yikes!)
    return *this;
}

const char *EMailHeader::MkLocalName(StdString& std, bool TryMessageID) const {
    std = "";

    if (TryMessageID)
        std = MessageID(); // THIS IS OUR LEGACY. USED TO BE THE DEFAULT.

    if (std.is_null() || (std.length() <= 2)) {
        std = Subject();
        if (std.is_null() || std.is_space()) {
            StdString scmp;
            std = "Posetd on ";
            scmp = Date();
            if (scmp.is_null())
                std.append("unknown date");
            else
                std.append(scmp);
            std.append(" from ");
            scmp = From();
            if (scmp.is_null()) {
                // This may look nuts, but it happens!
                scmp = InternetPath();
                if (scmp.is_null() || scmp.is_space())
                    std.append("unknown source");
                else
                    std.append(scmp);
            } else
                std.append(scmp);
        }
    }

    ffMkLocalFileName(std);

    return std.c_str();
}

