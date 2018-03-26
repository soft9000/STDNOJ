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
#include <core/Utilities.hpp>
#include <core/StdString.hpp>

#include <cstdio>            // sprintf
#include <cctype>            // toupper()/tolower()

#include <cstddef>           // size_t
#include <cstdlib>

#include <sstream>           // 01/03/2008 - Finally - a universally worthy string stream!


namespace stdnoj {

    StdString::~StdString(void) {
        delete [] psz_alloc;
    }

    void StdString::_init(void) {
        extra_bytes = MIN_GROW_SPACE;
        sz_alloc = extra_bytes + sizeof (NULL);
        psz_alloc = new char[sz_alloc];
        *psz_alloc = 0L;
        iomax(MAX_READ_DEFAULT);
    }

    size_t StdString::iomax(void) {
        return max_read;
    }

    void StdString::iomax(size_t sz) {
        if (sz < size_t(-1L)) {
            max_read = sz;
            return;
        }
        max_read = MAX_READ_DEFAULT;
    }

    size_t StdString::instance_growth_space(void) {
        return extra_bytes;
    }

    void StdString::instance_growth_space(size_t sz) {
        if (sz < size_t(-1L)) {
            extra_bytes = ((size_t(-1L)) - sizeof (NULL));
            return;
        }
        if (sz > MIN_GROW_SPACE) {
            extra_bytes = sz;
            return;
        };
    }


    // "end" is a offset after offset "pos" (inclusive);

    char *StdString::_substr(const char *psz, size_t start, size_t end) const {
        if (!psz)
            return NULL;
        size_t dest_len = ::strlen(psz);
        // Full bounds-checking doesn't cost too much these days;
        if (start > dest_len)
            return NULL;
        if (end > dest_len)
            end = dest_len;
        if (start >= end)
            return NULL; // <= cuz a memcpy(,,0) doesn't make much sense, either...

        // Do it && return an allocated buffer;
        char *pBuf = new char[end - start + sizeof (NULL)];
        ::memcpy(pBuf, &psz[start], end - start);
        pBuf[end - start] = 0;
        return pBuf;
    }

    // "end" is NPOS or a count of bytes to take from offset "start" (inclusive);

    size_t StdString::tally(const StdString& ch, int ignore_case) const {
        size_t res = 0L;
        size_t whence = find(ch, 0L, ignore_case);
        while (whence != NPOS) {
            res++;
            whence = find(ch, whence + 1, ignore_case);
        }
        return res;
    }

    StdString StdString::subpos(size_t start, size_t end) const {
        if (end > length())
            end = length();
        return substr(start, end - start);
    }

    // "end" is NPOS or a count of bytes to take from offset "start" (inclusive);

    StdString StdString::substr(size_t start, size_t end) const {
        char *pBuf;
        if (end == NPOS)
            pBuf = _substr(c_str(), start, length());
        else {
            size_t sz = start + end;
            if (sz > length())
                sz = length();
            pBuf = _substr(c_str(), start, sz);
        }

        StdString stackResult;
        if (pBuf == NULL)
            stackResult = "";
        else {
            stackResult = pBuf;
            delete []pBuf;
        }
        return stackResult;
    }

    const char *StdString::own(char *psz) {
        delete[] psz_alloc;
        psz_alloc = psz;
        sz_alloc = strlen(psz);
        return c_str();
    }

    const char *StdString::own(char *psz, size_t bytes) {
        delete[] psz_alloc;
        psz_alloc = psz;
        sz_alloc = bytes; // strlen(psz);
        return c_str();
    }

    StdString& StdString::assign(istream& is) {
        StdString str;
        while (is) {
            str = "";
            str.read_line(is);
            if (str.is_null() == false)
                append(str);
        }
        return *this;
    }

    StdString& StdString::assign(const char *psz, size_t start, size_t end) {
        if (!psz)
            return assign("");

        char *pBuf = _substr(psz, start, end);
        if (pBuf) {
            assign(pBuf);
            delete [] pBuf;
        } else
            assign("");
        return *this;
    }

    StdString& StdString::remove(size_t start) {
        if (start < length())
            psz_alloc[start] = 0;
        return *this;
    }

    StdString& StdString::remove_bytes(size_t start, size_t end) {
        if (!end)
            return *this;
        size_t max = length();
        if (!max)
            return *this;
        if (start >= max)
            return *this;
        if (end >= max)
            end = max;
        if ((start + end) >= max)
            return remove(start);
        strcpy(&psz_alloc[start], &psz_alloc[start + end]);
        return *this;
    }

    StdString& StdString::remove_pos(size_t start, size_t end) {
        if (start >= end)
            return *this;
        size_t max = length();
        if (start >= max)
            return *this;
        if (end >= max)
            end = max;
        strcpy(&psz_alloc[start], &psz_alloc[end]);
        return *this;
    }

    int StdString::cmp(const StdString& zs, int ignore_case) const {
        return ci_strcmp(psz_alloc, zs.psz_alloc, ignore_case);
    }

    StdString& StdString::remove_prefix(const StdString& sPattern, int ignore_case) {
        char chL, chR;
        size_t ss = 0L;
        size_t max_bytes = (sPattern.length() < length()) ? sPattern.length() : length();
        while (1) {
            chL = sPattern[ss];
            chR = (*this)[ss];
            if (ignore_case) {
                chL = toupper(chL);
                chR = toupper(chR);
            }
            if (chL != chR)
                break;
            if (ss == max_bytes)
                break;
            ss++;
        }
        remove_bytes(0, ss);
        return *this;
    }

    StdString& StdString::assign(long ii) {
        char buf[100];
        ::sprintf(&buf[0], "%ld", ii);
        return assign(&buf[0]);
    }

    StdString& StdString::assign(int ii) {
        char buf[20];
        ::sprintf(&buf[0], "%d", ii);
        return assign(&buf[0]);
    }

    StdString& StdString::assign(const char ch) {
        char buf[5];
        ::sprintf(&buf[0], "%c", ch);
        return assign(&buf[0]);
    }

    StdString& StdString::append(const char ch) {
        char buf[5];
        ::sprintf(&buf[0], "%c", ch);
        return append(&buf[0]);
    }

    StdString& StdString::append(const char ch, size_t count) {
        char *pbuf = new char[count + 1];
        ::memset(pbuf, ch, count);
        pbuf[count] = 0;
        append(pbuf);
        delete[] pbuf;
        return *this;
    }

    StdString& StdString::prepend(const char ch) {
        char buf[5];
        ::sprintf(&buf[0], "%c", ch);
        return prepend(&buf[0]);
    }

    int StdString::operator!=(const StdString& ss) const {
        return (!(*this == ss));
    }

    StdString& StdString::operator=(const StdString& ss) {
        if (this == &ss)
            return *this;
        assign(ss.c_str());
        return (*this);
    }

    StdString& StdString::operator=(const char *ss) {
        assign(ss);
        return (*this);
    }

    StdString& StdString::operator=(const char ch) {
        char buf[5];
        ::sprintf(&buf[0], "%c", ch);
        assign(&buf[0]);
        return *this;
    }

    int StdString::operator>(const StdString& ss) const {
        int cmp = ci_strcmp(psz_alloc, ss.psz_alloc);
        switch (cmp) {
            case 0:
            case 1:
                return 1;
            default:
                return 0;
        }
    }

    int StdString::operator<(const StdString& ss) const {
        int cmp = ci_strcmp(psz_alloc, ss.psz_alloc);
        switch (cmp) {
            case 0:
            case 1:
                return 0;
            default:
                return 1;
        }
    }

    int StdString::equals(const StdString& ss, int ignore_case) const {
        if (this == &ss)
            return 1;
        if (ci_strcmp(psz_alloc, ss.psz_alloc, ignore_case) == 0)
            return 1;
        return 0;
    }

    int StdString::operator==(const StdString& ss) const {
        return equals(ss);
    }

    char& StdString::operator[](size_t ss) const {
        if (ss > sz_alloc)
            return psz_alloc[sz_alloc % ss]; // best chance... (no exception thrown)
        return psz_alloc[ss];
    }

    size_t StdString::length(void) const {
        return (::strlen(psz_alloc));
    }

    const char *StdString::c_str(void) const {
        return psz_alloc;
    }

    StdString& StdString::assign(const char *psz) {
        if (!psz)
            psz = (const char *) "";

        size_t sz2 = ::strlen(psz);

        // 01/22/2000: DEFINING AN ARBITRARY RE-ALLOCATION THRESHOLD
        // ---------------------------------------------------------
        // A simple speed-up: Hitting RAM re-allocations is a REAL performance killer, and
        // allowing more than a small buffer to "hang around" is a REAL space killer when
        // arrays of objects using strings are used. The following represents our compromise 
        // between SPACE and SPEED. We simply pick a "reasonable" `don't-release` thresh-hold;
        if ((sz_alloc < (extra_bytes + (extra_bytes / 4))) && sz2 < sz_alloc) {
            // when we DON'T mess with sz_alloc, && we'll realize some VERY non-trivial speed-ups:
            ::strcpy(psz_alloc, psz);
            return *this;
        }

        // If the buffer is larger than the arbitrary (small) buffer, then we hit the heap,
        // (or, hopefully, the compilers local-store, or sub-allocation subsystem) for more
        // memory. Traditionally, realloc() has been usefull here;
        sz_alloc = sz2 + extra_bytes + sizeof (NULL);
        char *psz_allocIFDUP = new char[sz_alloc]; // JUST IN CASE we are self-referencing
        ::strcpy(psz_allocIFDUP, psz);

        delete [] psz_alloc; // JUST IN CASE we are self-referencing
        psz_alloc = psz_allocIFDUP;
        return *this;
    }

    StdString& StdString::assign(const StdString& ss) {
        assign(ss.c_str());
        return *this;
    }

    StdString& StdString::append(const StdString& ss) {
        append(ss.c_str());
        return *this;
    }

    StdString& StdString::append(const char *psz, size_t start, size_t end) {
        if (end > length())
            end = length();

        char *pBuf = _substr(psz, start, end);
        if (!pBuf)
            return *this;
        append(pBuf);
        delete [] pBuf;
        return *this;
    }

    StdString& StdString::append(const char *psz) {
        // NOW checking 4 buffer under-use..
        if (!psz)
            return *this; // Ignore a NULL pointer...
        if (!(*psz))
            return *this; // Appending a NULL...

        if (sz_alloc < (::strlen(psz_alloc) + ::strlen(psz) + sizeof (NULL))) {
            StdString ssOld = psz_alloc;
            delete [] psz_alloc;
            sz_alloc = ssOld.length() + ::strlen(psz) + extra_bytes + sizeof (NULL);
            psz_alloc = new char[sz_alloc];
            ::strcpy(psz_alloc, ssOld.c_str());
        }
        ::strcat(psz_alloc, psz); // Zooom!
        return *this;
    }

    StdString& StdString::prepend(const StdString& ss) {
        prepend(ss.c_str());
        return *this;
    }

    StdString& StdString::prepend(const char *psz) {
        // NOW checking 4 buffer under-use..
        StdString ssOld(psz_alloc);
        if (sz_alloc < (ssOld.length() + ::strlen(psz) + sizeof (NULL))) {
            delete [] psz_alloc;
            sz_alloc = ssOld.length() + ::strlen(psz) + extra_bytes + sizeof (NULL);
            psz_alloc = new char[sz_alloc];
        }
        ::strcpy(psz_alloc, psz); // NO Zoom! (sigh)
        ::strcat(psz_alloc, ssOld.c_str());
        return *this;
    }

    void StdString::reverse(void) {
        strrev(psz_alloc);
    }

    void StdString::to_upper(void) {
        for (size_t sz = 0; sz < length(); sz++)
            (*this)[sz] = toupper((*this)[sz]);
    }

    void StdString::to_up_low(void) {
        to_lower();
        int next_up = 1;
        for (size_t sz = 0; sz < length(); sz++) {
            if (next_up) {
                next_up = 0;
                (*this)[sz] = toupper((*this)[sz]); // not going to hurt whitespace (etc)
            }
            if (is_white((*this)[sz]) == true)
                next_up = 1;
            if ((*this)[sz] == '-') // hypenation counts in most titles
                next_up = 1;
        }
    }

    void StdString::to_lower(void) {
        for (size_t sz = 0; sz < length(); sz++)
            (*this)[sz] = ::tolower((*this)[sz]);
    }

    bool StdString::is_alnum(size_t pos) const {
        if (pos > length())
            return false;
        //if(pos < 0)
        //   return false;
        char ch = (*this)[pos];
        return ( (isalnum(ch)) ? true : false);
    }

    bool StdString::is_alpha(size_t pos) const {
        if (pos > length())
            return false;
        //if(pos < 0)
        //   return false;
        char ch = (*this)[pos];
        bool br = (isalpha(ch)) ? true : false;
        return ( br);
    }

    bool StdString::is_white(char ch) const {
        /*
        // The result of the test condition for the iscntrl() function depends on the LC_CTYPE 
        // category setting of the current locale; see setlocale for more information;
        return ( ::iscntrl( ch ) );
         */
        // 03/10/2001: THIS IS OUR DEFINITION, AS PER OUR mkwhite();
        if (ch <= ' ')// 0x20
            return true;
        if (ch > '~') // 0x7E
            return true;
        return false;
    }

    bool StdString::is_white(size_t pos) const {
        if (pos >= length())
            return false;
        //if(pos < 0)
        //   return false;
        return is_white((*this)[pos]);
    }

    bool StdString::is_white(void) const {
        for (size_t ss = 0L; ss < length(); ss++) {
            if (!is_white((*this)[ss]))
                return false;
        }
        return true;
    }

    bool StdString::is_null(void) const {
        if (!length())
            return true;
        char ch = (*this)[0];
        if (ch)
            return false;
        return true;
    }

    istream& StdString::read_to_delim(istream& is, char delim) {
        return readline(is, delim);
    }

    istream& StdString::read_line(istream& is, char delim) {
        return readline(is, delim);
    }

    istream& StdString::read_string(istream& is, char delim) {
        return readline(is, delim);
    }

    int StdString::strip(strip_type st, char ch) {
        int res = 0;
        switch (st) {
            case Leading:
                res = Strip(*this, ch);
                break;
            case Trailing:
                res = Clip(*this, ch);
                break;
            case Both:
                res = Clip(*this, ch);
                res += Strip(*this, ch);
                break;
        }
        return res;
    }

    bool StdString::is_space(void) const {
        for (size_t ss = 0L; ss < length(); ss++) {
            if (!isspace((*this)[ss]))
                return false;
        }
        return true;
    }

    size_t StdString::seek_binary(int SkipControlChars) const {
        for (size_t ss = 0L; ss < length(); ss++) {
            char ch = (*this)[ss];
            if (ch > 0x7F)
                return ss;
            if (ch < ' ') {
                if (SkipControlChars) {
                    switch ((*this)[ss]) {
                        case '\n':
                        case '\t':
                        case '\r':
                        case '\v':
                            continue;
                        default:
                            return ss;
                    }
                } else
                    return ss;
            }
        }
        return NPOS;
    }

    bool StdString::is_ascii(void) const {
        for (size_t ss = 0L; ss < length(); ss++) {
            char ch = (*this)[ss];
            if (ch > 0x7F) // 127d
                return false;
            if (ch < 0x00) // signed / unsigned (portability)
                return false;
        }
        return true;
    }

    bool StdString::mkwhite(void) {
        bool res = false;
        for (size_t ss = 0L; ss < length(); ss++) {
            if (is_white((*this)[ss])) {
                // Inforce the normalized definition of what a white-space character is;
                (*this)[ss] = ' ';
                res = true;
            }
        }
        return res;
    }

    int StdString::snip(char ch) {
        size_t pos = find(ch);
        if (pos != npos) {
            remove_pos(0, pos);
            while ((*this)[0] == ch)
                remove_pos(0, 1);
            return 1;
        }
        return 0;
    }

    ostream& StdString::write(ostream& ofs) const {
        // NOTE: If the stream is !binary, the newlines will expand on Wintel!
        ofs << psz_alloc;
        return ofs;
    }

    istream& StdString::read(istream& is, size_t whence) {
        if (is) {
            char *pbuf = new char[whence + 1];
            ::memset(pbuf, (int) whence, 0); // might get less!
            is.read(pbuf, (int) whence);
            pbuf[whence] = 0; // REDUNDANT
            if (!is)
                pbuf[0] = 0;
            assign(pbuf);
            delete [] pbuf;
        } else
            assign("");
        return is;
    }

    istream& StdString::read(istream& is) {
        if (is) // 7/16/2001, R. Nagy
        {
            char *pbuf = new char[max_read];
            is >> pbuf; // 7/16/2001, R. Nagy [BUGFIX]!
            if (!is)
                pbuf[0] = 0;
            assign(pbuf);
            delete [] pbuf;
        } else
            assign("");
        return is;
    }

    StdString& StdString::operator=(int i) {
        char buf[100];
        sprintf(buf, "%d", i);
        *this = buf;
        return *this;
    }

    int StdString::as_int(void) const {
        return ::atoi(c_str());
    }

    long StdString::as_long(void) const {
        return (long) ::atol(c_str());
    }

    ostream& StdString::write_stream(ostream& ofs) const {
        // NOTE: If the stream is !binary, the newlines will expand on Wintel!
        ofs << length() << " ";
        if (is_null() == false)
            ofs << psz_alloc;
        return ofs;
    }

    istream& StdString::read_stream(istream& is) {
        if (is) {
            size_t sz2 = 0L;
            is >> sz2;
            if (!is)
                goto rd_abort;
#ifdef DEBUG
            if (sz2 > max_read)
                throw "Possibly a too-large StdString error!";
#endif
            char ch[5];
            is.read(ch, 1); // skip the ' '
            if (!is)
                goto rd_abort;

            delete [] psz_alloc;
            sz_alloc = sz2; // exact length
            sz2 = sz_alloc + extra_bytes + sizeof (NULL);
            psz_alloc = new char[sz2];
            is.read(psz_alloc, sz_alloc);
            psz_alloc[sz_alloc] = 0;
            sz_alloc = sz2; // grow length
        } else {
rd_abort:
            assign("");
        }
        return is;
    }

    ostream& StdString::write_string(ostream& ofs) const {
        // Binary compatability with borland -almost (imbedded NULL would mess us READ up!)
        // Borland code DOES insure that termination is a NULL on WRITE (I checked their READ RTL code).
        // Problem might be with write_string: could legacally-have an imbedded NULL, but
        // not sure how Borland handled the conversion of imbedded NULLs (if at all)! 
        // MY legacy stuff did not do imbed NULLS ('cept for crypto string), so this will do for now;
        ofs.write(psz_alloc, length());
        char sends = 0;
        ofs.write(&sends, 1);
        return ofs;
    }

    StdString StdString::get_token(const StdString& sTok) const {
        StdString sResult;
        size_t sz = find(sTok);
        if (sz != npos) {
            char *pszF = &(*this)[sz];
            char *pszL = &(*this)[sz];

            if (isspace(*pszF)) {
                sResult = *pszF;
                return sResult;
            }

            while ((pszF >= psz_alloc) && !isspace(*pszF))
                pszF--;
            while (*pszL && !isspace(*pszL))
                pszL++;

            if (pszF != psz_alloc)
                pszF++;

            char ch = *pszL;
            *pszL = 0;
            sResult = pszF;
            *pszL = ch;
        }
        return sResult;
    }

    StdString StdString::remove_token(const StdString& pattern) const {
        StdString sResult = *this;
        size_t ssHit = sResult.find(pattern, 0L);
        while (ssHit != NPOS) {
            sResult.remove_pos(ssHit, ssHit + pattern.length());
            ssHit = sResult.find(pattern, ssHit + 1);
        }
        return sResult;
    }

    StdString& StdString::remove(const StdString& str) {
        assign(remove_token(str));
        return *this;
    }

    istream& StdString::read_token(istream& is) {
        char *psz = new char[MAX_TOKEN]; // a large buffer!
        is >> psz;
        assign(psz);
        delete [] psz;
        return is;
    }

    istream& StdString::readline(istream& is, const StdString& delim) {
        *this = StdString();

        size_t sPos = 0L;
        stringstream srm;
        bool bHitting = false;
        char ch = 0;
        while (is) {
            is.read(&ch, 1);
            if (!is)
                continue;
            if (ch == delim[sPos]) {
                sPos++;
                bHitting = true;
                if (sPos == delim.length()) {
                    // Found -
                    assign(srm.str().c_str());
                    return is;
                }
                continue;
            }
            if (bHitting) {
                bHitting = false;
                srm << delim.subpos(0, sPos);
                sPos = 0L;
            }
            srm << ch;
        }
        assign(srm.str().c_str()); // Assign whatever we found -
        return is;
    }

    istream& StdString::readline(istream& is, char delim) {
        // The better part of valor: I'd rather do it myself
        // than trust to the compiler's auto type conversion -
        StdString str;
        str.assign(delim);
        return readline(is, str);
    }

    StdString StdString::enumerate(char ch) {
        StdString sResult;
        size_t whence = find(ch);
        if (whence == npos) {
            sResult = c_str();
            assign("");
        } else {
            sResult = substr(0, whence);
            remove_pos(0, whence + 1);
        }
        return sResult;
    }

    int StdString::remove_pattern(const StdString& pattern, int ignore_case) {
        size_t numHits = 0L;
        size_t ssHit = find(pattern, 0L, ignore_case);
        while (ssHit != NPOS) {
            numHits++;
            remove_pos(ssHit, ssHit + pattern.length());
            ssHit = find(pattern.c_str(), ssHit, ignore_case);
        }
        return numHits;
    }

    int StdString::replace(const StdString& pattern, const StdString& replace, int ignore_case) {
        int numHits = 0;
        size_t ssHit = find(pattern, 0, ignore_case);
        while (ssHit != NPOS) {
            numHits++;

            remove_pos(ssHit, ssHit + pattern.length());
            insert(ssHit, replace.c_str());

            ssHit = find(pattern.c_str(), ssHit + replace.length(), ignore_case);
        }
        return numHits;
    }

    size_t StdString::find(char ch, size_t pos, int ignore_case) const {
        if (pos >= ::strlen(psz_alloc))
            return npos;
        if (pos == npos)
            pos = 0L;
        if (pos > ::strlen(psz_alloc))
            return npos;

        char *psz = ci_strchr(&psz_alloc[pos], ch, ignore_case);
        if (psz) {
            char h = *psz;
            *psz = 0;
            // Offset index is relative to the ENTIRE StdString;
            size_t offs = ::strlen(psz_alloc);
            *psz = h;
            return offs;
        }
        return npos;
    }

    size_t StdString::rfind(char ch, size_t pos, int ignore_case) const {
        strrev(psz_alloc);
        size_t res = find(ch, pos, ignore_case);
        strrev(psz_alloc);
        if (res == npos)
            return res;
        return (length() - res - 1);
    }

    size_t StdString::find_any(const StdString& str, size_t pos) const {
        return (find(str, pos, 1));
    }

    size_t StdString::rfind_any(const StdString& str, size_t pos) const {
        return (rfind(str, pos, 1));
    }

    size_t StdString::find_any(const char ch, size_t pos) const {
        return (find(ch, pos, 1));
    }

    size_t StdString::rfind_any(const char ch, size_t pos) const {
        return (rfind(ch, pos, 1));
    }

    size_t StdString::find(const char *pch, size_t pos, int ignore_case) const {
        if (pos >= ::strlen(psz_alloc))
            return npos;
        if (pos == npos)
            pos = 0L;

        char *psz = ci_strstr(&psz_alloc[pos], pch, ignore_case);
        if (psz) {
            char h = *psz;
            *psz = 0;
            // Offset index is relative to the ENTIRE StdString;
            size_t offs = ::strlen(psz_alloc);
            *psz = h;
            return offs;
        }
        return npos;
    }

    size_t StdString::find_last(const char ch, size_t pos, int ignore_case) const {
        StdString sz = ch;
        return find_last(sz.c_str(), pos, ignore_case);
    }

    size_t StdString::find_last(const char *pch, size_t pos, int ignore_case) const {
        size_t ssRes = find(pch, pos, ignore_case);
        if (ssRes == NPOS)
            return ssRes;
        while (1) {
            size_t ss = find(pch, ssRes + 1, ignore_case);
            if (ss == NPOS)
                break;
            ssRes = ss;
        }
        return ssRes;
    }

    bool StdString::ends_with(const StdString& sToken) const {
        size_t pos = rfind(sToken);
        if (pos == length() - sToken.length())
            return true;
        return false;
    }

    size_t StdString::rfind(const char *pch, size_t pos, int ignore_case) const {
        StdString str = c_str();
        str.reverse();
        StdString spat = pch;
        spat.reverse();
        size_t whence = str.find(spat, ignore_case);
        if (whence == npos)
            return whence;
        return (length() - spat.length() - whence);
    }

    size_t StdString::find(const StdString& pch, size_t pos, int ignore_case) const {
        return find(pch.c_str(), pos, ignore_case);
    }

    size_t StdString::rfind(const StdString& pch, size_t pos, int ignore_case) const {
        return rfind(pch.c_str(), pos, ignore_case);
    }

    const char *StdString::substr(StdString& str, size_t start) const {
        size_t max = length();
        if (start >= max)
            str = "";
        else
            str = &operator[](start);
        return str.c_str();
    }

    const char *StdString::copy_bytes(StdString& sRes, size_t start, size_t end) const {
        size_t max = ::strlen(psz_alloc);
        if (start >= max)
            sRes = "";
        else {
            sRes = &operator[](start);
            if ((start + end) < max)
                sRes.remove(end);
        }
        return sRes.c_str();
    }

    const char *StdString::copy_pos(StdString& str, size_t start, size_t end) const {
        size_t max = ::strlen(psz_alloc);
        if (start >= max)
            str = "";
        else {
            str = &operator[](start);
            if (end < max)
                str.remove(abs(long(end - start)));
        }
        return str.c_str();
    }

    void StdString::insert(size_t ss, const StdString& str) {
        insert(ss, str.c_str());
    }

    void StdString::insert(size_t ss, const char *psz) {
        // usage:: If you insert something at 0, then you'll have a new 0!
        StdString tmp;
        substr(tmp, ss);
        remove(ss);
        append(psz);
        append(tmp.c_str());
    }

    char *StdString::strrev(char *psz) const {
        if (*psz) {
            size_t len = ::strlen(psz);
            char *psz1 = psz;
            char *psz2 = &psz[len - 1];

            char ch;
            len /= 2; // No need to swap the "odd man out" (think about it!)
            while (len) {
                ch = *psz1;
                *psz1 = *psz2;
                *psz2 = ch;
                psz1++;
                psz2--;
                len--;
            }
        }
        return psz;
    }

    char *StdString::ci_strstr(const char *str1, const char *str2, int ignore_case) const {
        char *cp = (char *) str1;
        char *s1, *s2;
        char mklow = 'A' - 'a';

        if (!*str2)
            return NULL;

        while (*cp) {
            s1 = cp;
            s2 = (char *) str2;

            if (ignore_case) {
                char ch1, ch2;
                while (*s1 && *s2) {
                    ch1 = *s1;
                    ch2 = *s2;
                    if (ch1 >= 'A' && ch1 <= 'Z')
                        ch1 -= mklow;
                    if (ch2 >= 'A' && ch2 <= 'Z')
                        ch2 -= mklow;
                    if ((ch1 - ch2))
                        break;
                    s1++, s2++;
                }
            } else {
                while (*s1 && *s2 && !(*s1 - *s2))
                    s1++, s2++;
            }

            if (!*s2)
                return cp;

            cp++;
        }
        return NULL;
    }

    int StdString::ci_strcmp(const char *str1, const char *str2, int ignore_case) const {
        char mklow = 'A' - 'a';
        char ch1, ch2;
        int ret = 0;

        if ((!(*str1)) && (!(*str2)))
            return ret;
        if (!(*str1))
            return 1;
        if (!(*str2))
            return -1;

        while (*str1 && *str2) {
            ch1 = *str1;
            ch2 = *str2;
            if (ignore_case) {
                if (ch1 >= 'A' && ch1 <= 'Z')
                    ch1 -= mklow;
                if (ch2 >= 'A' && ch2 <= 'Z')
                    ch2 -= mklow;
            }
            ret = ch1 - ch2;
            if (ret)
                break;

            ++str1;
            ++str2;
        }
        if (!ret) {
            // This keeps "fl.jobs" from equaling "fl.jobs.www" (and vice versa);
            if (*str1)
                return -1;
            if (*str2)
                return 1;
        }
        if (ret < 0)
            ret = -1;
        else if (ret > 0)
            ret = 1;
        return ret;
    }

    char *StdString::ci_strchr(const char *str1, char ch, int ignore_case) const {
        char mklow = 'A' - 'a';
        char ch2 = ch;
        if (ignore_case) {
            if (ch2 >= 'A' && ch2 <= 'Z')
                ch2 -= mklow;
        }
        while (*str1) {
            char ch1 = *str1;
            if (ignore_case) {
                if (ch1 >= 'A' && ch1 <= 'Z')
                    ch1 -= mklow;
            }
            if (ch1 == ch2)
                return ((char *) str1);
            str1++;
        }
        return NULL;
    }

    Array<StdString> StdString::slice(const StdString& ch) const {
        Array<StdString> array;
        PipeArray::Parse(array, *this, ch);
        return array;
    }

    Array<StdString> StdString::slice(char ch) const {
        Array<StdString> aRes22;
        PipeArray::Parse(aRes22, *this, ch);
        return aRes22;
    }

    StdString & operator >>(StdString& str, size_t& sz) {
        sz = ::atoi(str.c_str());
        return str;
    }

    StdString & operator <<(StdString& str, size_t sz) {
        char buf[120];
        ::sprintf(buf, "%d", sz);
        str.append(buf);
        return str;
    }

    StdString & operator <<(StdString& str, const StdString& str2) {
        str.append(str2);
        return str;
    }

    StdString & operator <<(StdString& str, const char *psz) {
        str.append(psz);
        return str;
    }

    istream & operator >>(istream& is, StdString& ss) {
        ss.read(is);
        return is;
    }

    ostream & operator <<(ostream& os, const StdString& ss) {
        os << ss.c_str();
        return os;
    }

    /*

    ifstream& operator >> (ifstream& is, StdString& ss)
    {
       ss.read_stream(is);
       return is;
    }

    ofstream& operator << (ofstream& os, const StdString& ss)
    {
       ss.write_stream(os);
       return os;
    }
     */



    size_t Strip(StdString& str, const char ch) {
        int sp = 0;
        if (str.is_null())
            return sp;
        while (str[sp] == ch)
            sp = sp + 1;
        if (sp)
            str.remove_pos(0, sp);
        return sp;
    }

    size_t Clip(StdString& str, const char ch) {
        size_t sp = str.length();
        size_t count = 0L;
        if (!sp)
            return count;
        while (str[sp - 1] == ch) {
            count = count + 1;
            if (!sp) {
                count = str.length();
                str = "";
                return count;
            }
            sp = sp - 1;
        }
        if (count)
            str.remove_pos(sp, str.length());
        return count;
    }

    size_t Snip(const StdString& str, StdString& sLeft, StdString& sRight, const char ch) {
        StdString sss = str;
        size_t sp = sss.find(ch);
        if (sp != NPOS) {
            sLeft = str.substr(0, sp);
            sRight = str.substr(sp + 1, str.length());
        }
        return sp;
    }

    int getline(istream& is, StdString& str) {
        str.read_line(is);
        if (is.good())
            return 1;
        return 0;
    }

    StdString
    operator +(const StdString& str, const StdString& cstr) {
        StdString sResult = str;
        sResult.append(cstr);
        return sResult;
    }

    StdString &
            operator +=(StdString& str, const StdString& cstr) {
        str.append(cstr);
        return str;
    }


} // stdnoj
