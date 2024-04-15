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

#include <Server/Email.hpp>

#define BEGIN {
#define END }

//#pragma message "BINARY STREAMS REQUIRED: SETMODE is not going to do anything!!"

bool EMail::SaveBin(const StdString& sFileName) // binary format
{
    File file;
    if (file.Name(sFileName) == false)
        return false;
    return SaveBin(file);
}

bool EMail::SaveBin(const File& fff) // binary format
{
    File file = fff;
    ostream& os = file.OpenWrite(File::AT_BINARY);
    if (!os)
        return false;
    write(os);
    return true;
}

bool EMail::LoadBin(const StdString& sFileName) // binary format
{
    File file;
    if (file.Name(sFileName) == false)
        return false;
    return LoadBin(file);
}

bool EMail::LoadBin(const File& fff) // binary format
{
    File file = fff;
    istream& is = file.OpenRead(File::AT_BINARY);
    if (!is)
        return false;
    read(is);
    return true;
}

bool EMail::RemoveTerminalDot(void) {
    size_t last = NPOS;
    size_t whence = _text.find("\r\n."); // observed && guaranteed minimum signature.
    while (whence != NPOS) {
        last = whence;
        whence = _text.find("\r\n.", last + 1);
    }
    if (last != NPOS) {
        // We have a potential terminal (RFC) "dot";
        StdString std = _text.substr(last);
        std[2] = ' ';
        if (std.is_white()) {
            _text.remove(last);
            return true;
        }
    }
    return false;
}

bool EMail::FirstMessageLine(StdString& std) {
    _ff_marker = NULL;
    return NextMessageLine(std);
}
// RETURN an RFC-terminated portion of the article;

bool EMail::NextMessageLine(StdString& std) {
    if (_ff_marker >= _text.length())
        return false;

    size_t _ff = _text.find(TERMINATE, _ff_marker);
    if (_ff == NPOS)
        _ff = _text.length();
    else
        _ff += 2; // INCLUDE the NEWLINE

    std = _text.substr(_ff_marker, (_ff - _ff_marker));
    _ff_marker = _ff;
    return true;
}

void EMail::EncodeDots(void) {
    StdString std;
    bool br = FirstMessageLine(std);
    size_t start_pos = NULL;

    while (br == true) {
        if (std == ".") // The LAST ONE looks like this.
        {
            std = "..";
            _text.remove_pos(start_pos, _ff_marker + 1);
            _text.insert(start_pos, std);
            _ff_marker += start_pos + std.length();
            std = "";
        }
        if (std == ".\r\n") // An IMBEDDED ONE looks like this.
        {
            std = "..\r\n";
            _text.remove_pos(start_pos, _ff_marker + 1);
            _text.insert(start_pos, std);
            _ff_marker += start_pos + std.length();
            std = "";
        }
        start_pos = _ff_marker;
        br = NextMessageLine(std);
    }
}

void EMail::DecodeDots(void) {
    StdString std;
    bool br = FirstMessageLine(std);
    size_t start_pos = NULL;

    while (br == true) {
        if (std == "..") // The LAST ONE looks like this.
        {
            std = ".";
            _text.remove_pos(start_pos, _ff_marker + 1);
            _text.insert(start_pos, std);
            _ff_marker += start_pos + std.length();
            std = "";
        }
        if (std == "..\r\n") // An IMBEDDED ONE looks like this.
        {
            std = ".\r\n";
            _text.remove_pos(start_pos, _ff_marker + 1);
            _text.insert(start_pos, std);
            _ff_marker += start_pos + std.length();
            std = "";
        }
        start_pos = _ff_marker;
        br = NextMessageLine(std);
    }
}

bool EMail::rfc_write(ostream& ofs) const {
    if (EMailHeader::rfc_write(ofs) == false)
        return false;

    ofs << TERMINATE;

    ofs << _text.c_str();
    ofs << TERMINATE;
    return true;
}

bool EMail::MessagePayload(stringstream& ifs) {
    return rfc_message(ifs);
}
#ifdef __BCPLUSPLUS__
bool EMail::MessagePayloadBinaryONLY(ifstream& ifs)
#else

bool EMail::MessagePayload(ifstream& ifs)
#endif
{
    int omode = SETMODE(ifs, bin_mode);
    bool br = rfc_message(ifs);
    SETMODE(ifs, omode);
    return br;
}

bool EMail::MessagePayload(const File& fff) {
    File file = fff;
    bool br = rfc_message(file.OpenRead(File::AT_BINARY));
    file.Close();
    return br;
}

bool EMail::HasMessage(void) const {
    if (_text.is_null())
        return false;
    return true;
}

bool EMail::rfc_message(istream& ifs) {
    StdString std, text;
    _text = std; // empty it out
    if (!ifs)
        return false;

    while ((!ifs.eof()) && ifs && ifs.good()) {
        std.readline(ifs);

        if (IsEndOfMessage(std)) {
            // Usual terminal condition;
            // done = 1;
            break;
        }

        if (ifs) {
            Cleanup(std); // This might wind-up being NULL
            text.append(TERMINATE); // and this both RFC-joins && will put same, back.
            if (!text.is_null())
                text.append(std); // (i.o will terminate)
        }
    }

    _text = text; // VERY IMPORTANT TO RE-ASSIGN SO WE CAN RAM-FREE THE HUGE GROWTH SPACE.
    return true;
}

bool EMail::rfc_read(istream& ifs) {
    BEGIN
    EMail emh;
    *this = emh;
    END
    if (EMailHeader::rfc_read(ifs) == true)
        return rfc_message(ifs);
    return false;
}

int EMail::operator==(const EMail& em) const {
    // Man, there ain't no reason to do a strcmp on a huge string
    // when we have headers;
    return EMailHeader::operator==((const EMailHeader&) em);
}

EMail& EMail::operator=(const EMail& em) {
    if (this == &em)
        return *this;
    _pop3_server_id = em._pop3_server_id;
    _text.assign(em._text);
    EMailHeader::operator=(em);
    return *this;
}

bool EMail::IsNull(void) const {
    if (_text.is_null() && EMailHeader::IsNull())
        return true;
    return false;
}

ostream& EMail::write(ostream& os) {
    EMailHeader::write(os);
    _text.write_stream(os);
    return os;
}

ostream& EMail::write_searchable(ostream& os) {
    EMailHeader::write(os);
#if 1
    if (IsMime() == false)
        _text.write(os);
#else
    if (IsMime() == false)
        _text.write(os);
    else {
        StdString sSearchable;
        size_t whence = NULL;
        size_t sz_found = NULL;
        size_t count = NULL;
        size_t sanity = NULL;
        size_t lines = NULL;
        while (sz_found != NPOS) {
            sz_found = _text.find('\n', whence);
            // Detect a size_t overflow;
            if (sz_found < sanity)
                sz_found = NPOS;
            else
                sanity = sz_found;
            if (sz_found == NPOS) {
                os << TERMINATE;
                os << sSearchable.c_str();
                continue;
            }

            sSearchable = _text.substr(whence, sz_found - whence);
            whence = sz_found + 1;
            Cleanup(sSearchable);
            if ((sSearchable.length() > 60) && (sSearchable.find(' ') == false)) {
                // Crude, but universal hex-encoded attachment detection;
                count++;
                if (count >= 15) {
                    sz_found = NPOS; // bye!
                    continue;
                }
            } else
                count = NULL;

            // Write-out the string;
            lines++;
            os << TERMINATE;
            os << sSearchable.c_str();
        } // ewhile
    } // if mime

    os << TERMINATE;
#endif
    return os;
}

istream& EMail::read(istream& is) {
    EMailHeader::read(is);
    _text.read_stream(is);
    return is;
}

