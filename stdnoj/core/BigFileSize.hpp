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
// BigFileSize
// 11/04/1999: Class create to handle same (originally for Node1.hpp), R. Nagy
//
#ifndef bigfilesize_hpp
#define bigfilesize_hpp

#include <stdnoj.hpp>

class BigFileSize {
protected:
    long unsigned ulSize;
    long unsigned ulSizePlus;

public:

    BigFileSize(void) : ulSize(0L), ulSizePlus(0L) {
    }

    BigFileSize(const BigFileSize& ffs) {
        *this = ffs;
    }

    void SetSize(long unsigned ul) {
        ulSize = ul;
    }

    long unsigned GetSize(void) {
        return ulSize;
    }

    void SetSizePlus(long unsigned ul) {
        ulSizePlus = ul;
    }

    long unsigned GetSizePlus(void) {
        return ulSizePlus;
    }

    BigFileSize & operator=(const BigFileSize& fz);
    int operator==(const BigFileSize& fz) const;

    ostream& write(ostream& os) const;
    istream& read(istream& is);
};

inline BigFileSize&
        BigFileSize::operator=(const BigFileSize& fz) {
    if (this == &fz)
        return *this;
    ulSize = fz.ulSize;
    ulSizePlus = fz.ulSizePlus;
    return *this;
}

inline int
BigFileSize::operator==(const BigFileSize& fz) const {
    return ( (ulSize == fz.ulSize) && (ulSizePlus == fz.ulSizePlus));
}

inline ostream&
BigFileSize::write(ostream& os) const {
    os << ' ' << ulSize << ' ' << ulSizePlus << ' ';
    return os;
}

inline istream&
BigFileSize::read(istream& is) {
    char ch;
    is.read(&ch, 1);
    is >> ulSize;
    is.read(&ch, 1);
    is >> ulSizePlus;
    is.read(&ch, 1);
    return is;
}
#endif
