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
// Class ByteBuffer: Used to manipulate arrays of same, with allignemnt.
//
// 03/03/1998, Class created, Randall Nagy
// 03/18/1998, Added read(is), write(os), and operator==(), Randall Nagy
// 04/28/1999, Added hexline(StdString&), Randall Nagy
// 05/19/1999, Added assign(const void *, size_t), Randall Nagy
// 05/21/1999, Fixed a classic "uni-buffer-freed" self-assignemnt bug, Randall Nagy
// 12/02/2000, Added read_file(), read_bytes() and more, and fixed a bug or two, Randall Nagy
// 11/30/2003: Renamed _hexdump xdump and made it public, Randall Nagy
// 04/26/2003: Changed is_null() to boolean usage, Randall Nagy.
// 11/21/2008: Added a few members to integrate better into the stdnoj paradigm, Randall Nagy
// 11/08/2006: Added read_stream and write_stream - Exact same IO as StdString, Randall Nagy
// 11/12/2006: Removed byte_t - Most have figgured that one out by now, Randall Nagy

#ifndef ByteBuffer_Hpp
#define ByteBuffer_Hpp

#include <stdnoj.hpp>

namespace stdnoj {

    class ByteBuffer {
    private:
        size_t len;
        char *pBuf;

    public:
        ByteBuffer(void);
        ByteBuffer(const StdString& str);
        ByteBuffer(const char *buf, size_t cb);
        ByteBuffer(size_t cb);
        ByteBuffer(const ByteBuffer& bb);
        ~ByteBuffer(void);

        void empty(void);

        bool is_null(void) const; // 1 == true

        void hexline(StdString&, bool address = true, int option_base = 16) const;
        void hexdump(ostream&, bool address = true, int option_base = 16) const;
        void xdump(ostream& os, bool address = true, int option_base = 16) const;

        const char * bytes(void) const {
            return pBuf;
        }

        size_t length(void) const {
            return len;
        }

        void assign(const char *pin, size_t ss);
        void assign(const void *pin, size_t ss);
        void assign(const StdString& str);

        void fill(char ch = NULL);
        void prepend(const ByteBuffer& by);

        void append(const ByteBuffer& by);
        void append(char by);
        void append(void *psz, size_t nelem);

        void pad_to_alignment(size_t mod);

        ByteBuffer & operator=(const ByteBuffer& bb);
        ByteBuffer & operator=(const StdString& str);
        int operator==(const ByteBuffer& bb);

        char operator[](size_t ss) const {
            return pBuf[ss];
        }

        ostream& write(ostream& os) const;
        istream& read(istream& is);
        size_t read_file(istream& fs);
        size_t read_bytes(istream& is, size_t);

        // Common stream format - as shared with StdString -
        ostream& write_stream(ostream& os) const;
        istream& read_stream(istream& is);
    };

} // stdnoj
#endif
