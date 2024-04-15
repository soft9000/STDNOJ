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
#include <stdnoj.hpp>

#include <cstring>     // memcpy()
using namespace std;

namespace stdnoj {

    ByteBuffer::ByteBuffer(void) : len(0L), pBuf(0L) {
    }

    ByteBuffer::ByteBuffer(const StdString& str) : len(0L), pBuf(0L) {
        *this = ByteBuffer(str.c_str(), str.length());
    }

    ByteBuffer::ByteBuffer(const char *buf, size_t cb) : len(cb), pBuf(0L) {
        pBuf = new char[len + 1];
        ::memset(pBuf, 0, len);
        ::memcpy(pBuf, buf, len);
    }

    ByteBuffer::ByteBuffer(size_t cb) : len(cb), pBuf(0L) {
        pBuf = new char[len + 1];
        ::memset(pBuf, 0, len);
    }

    ByteBuffer::ByteBuffer(const ByteBuffer& bb) : len(0L), pBuf(0L) {
        *this = bb;
    }

    ByteBuffer::~ByteBuffer(void) {
        delete [] pBuf;
        pBuf = NULL;
    }

    bool ByteBuffer::is_null(void) const {
        if (!len)
            return true;
        if (!pBuf)
            return true;
        return false;
    }

    void ByteBuffer::assign(const char *pin, size_t ss) {
        delete [] pBuf;
        len = ss;
        if (!len) {
            pBuf = NULL;
            return;
        }
        pBuf = new char [len + 1];
        ::memcpy((void *) pBuf, (void *) pin, len);
    }

    void ByteBuffer::fill(char ch) {
        ::memset(pBuf, ch, len);
    }

    void ByteBuffer::prepend(const ByteBuffer& by) {
        if (!by.len)
            return;
        char *pbytes = new char[len + by.len + 1];
        ::memcpy((void *) pbytes, (void *) by.pBuf, by.len);
        if (len)
            ::memcpy((void *) & pbytes[by.len], (void *) pBuf, len);
        delete [] pBuf;
        len += by.len;
        pBuf = pbytes;
    }

    void ByteBuffer::append(const ByteBuffer& by) {
        if (!by.len)
            return;
        char *pbytes = new char[len + by.len + 1];
        if (len) {
            ::memcpy((void *) pbytes, (void *) pBuf, len);
            ::memcpy((void *) & pbytes[len], (void *) by.pBuf, by.len);
        } else
            ::memcpy((void *) pbytes, (void *) by.pBuf, by.len);
        delete [] pBuf;
        len += by.len;
        pBuf = pbytes;
    }

    void ByteBuffer::pad_to_alignment(size_t mod) {
        if (!len)
            return;
        size_t len2 = len % mod;
        if (len2) {
            ByteBuffer by(len2); // constructor zeros it out
            append(by);
        }
    }

    ByteBuffer& ByteBuffer::operator=(const ByteBuffer& bb) {
        if (this == &bb)
            return *this;
        delete [] pBuf;
        len = bb.len;
        if (!len) {
            pBuf = NULL;
            return *this;
        }
        pBuf = new char[len + 1];
        pBuf[len] = 0;
        ::memcpy((void *) pBuf, (void *) bb.pBuf, len);
        return *this;
    }

    void ByteBuffer::append(void *psz, size_t nelem) {
        ByteBuffer by;
        by.assign((char *) psz, nelem);
        append(by);
    }

    void ByteBuffer::xdump(ostream& os, bool address, int option_base) const {
        if (!pBuf) {
            os << "<NULL>";
            os << endl;
            return;
        }
        size_t which, ss2, ss1;
        which = ss2 = ss1 = 0L;
        char *pbuf = new char[100];
        for (ss1 = 0L; (ss1 * option_base) < len; ss1++) {
            // Address
            if (address) {
                ::sprintf(pbuf, "%04d: ", ss1 * option_base);
                os << pbuf;
            }
            // Hexdump
            for (ss2 = 0L; ss2 < option_base; ss2++) {
                which = (ss1 * option_base) + ss2;
                if (which <= len)
                    ::sprintf(pbuf, " %02x", pBuf[which]);
                else
                    ::strcpy(pbuf, "   ");
                os << pbuf;
            }
            // Translational (clear text)
            ::strcpy(pbuf, ":   ");
            os << pbuf;
            for (ss2 = 0L; ss2 < option_base; ss2++) {
                which = (ss1 * option_base) + ss2;
                if (which <= len) {
                    char bb = pBuf[which];
                    if ((bb >= ' ') && (bb <= '~'))
                        ::sprintf(pbuf, "%c", bb);
                    else
                        ::strcpy(pbuf, ".");
                } else
                    ::strcpy(pbuf, " ");
                os << pbuf;
            }
            os << endl;
        }
        delete [] pbuf;
    }

    void ByteBuffer::hexdump(ostream& os, bool address, int option_base) const {
        char *pbuf = new char[100];
        os << endl;
        sprintf(pbuf, "Block has %04d bytes:", len);
        os << pbuf;
        os << endl;
        delete [] pbuf;

        xdump(os, address, option_base);
        os << endl;
    }

    void ByteBuffer::hexline(StdString& std, bool address, int option_base) const {
        std = ""; // maybe free some mega-memory.
        stringstream srm;
        xdump(srm, address, option_base);
        std = PRESTO(srm);
    }

    int ByteBuffer::operator==(const ByteBuffer& bb) {
        if (len == bb.len) {
            int ires = ::memcmp(pBuf, bb.pBuf, len);
            if (!ires)
                return 1;
        }
        return 0;
    }

    ostream& ByteBuffer::write(ostream& os) const {
        os.write((char *) & len, sizeof (size_t));
        if (len)
            os.write((const char *) pBuf, len);
        return os;
    }

    istream& ByteBuffer::read(istream& os) {
        delete [] pBuf;
        pBuf = NULL;
        os.read((char *) & len, sizeof (size_t));
        if (len)
            read_bytes(os, len);
        return os;
    }

    ostream& ByteBuffer::write_stream(ostream& os) const {
        os << len << " ";
        if (len)
            os.write((const char *) pBuf, len);
        return os;
    }

    istream& ByteBuffer::read_stream(istream& is) {
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

            delete [] pBuf;
            len = sz2; // exact length
            pBuf = new char[len + 2];
            is.read(pBuf, len);
        } else {
rd_abort:
            ByteBuffer blanker;
            *this = blanker;
        }
        return is;
    }

    size_t ByteBuffer::read_file(istream& fs) {
        long sp1 = fs.tellg();
        fs.seekg(0L, ios::end);
        long sp2 = fs.tellg();
        len = sp2 - sp1;
        fs.seekg(sp1);
        return read_bytes(fs, len);
    }

    size_t ByteBuffer::read_bytes(istream& is, size_t sz) {
        len = sz;
        delete [] pBuf;
        pBuf = new char[len + 1];
        pBuf[len] = 0;
        size_t in = 0L;
        int ich;
        while (len != in) {
            ich = is.get();
            if (!is)
                break;
            pBuf[in] = (char) ich;
            in++;
        }
        len = in;
        return in;
    }

    void ByteBuffer::assign(const void *pin, size_t ss) {
        assign((char *) pin, ss);
    }

    void ByteBuffer::empty(void) {
        ByteBuffer bb;
        *this = bb;
    }

    void ByteBuffer::assign(const StdString& str) {
        assign(str.c_str(), str.length());
    }

    void ByteBuffer::append(char by) {
        // Expensive... Do someting faster later...
        ByteBuffer cp(len + 1);
        ::memcpy(cp.pBuf, pBuf, len);
        cp.pBuf[len] = by;
        *this = cp;
    }

    ByteBuffer& ByteBuffer::operator=(const StdString& str) {
        assign(str);
        return *this;
    }

}; // stdnoj
