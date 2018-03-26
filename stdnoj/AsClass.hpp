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
// AsClass<T>: Allow a base-type to participate in the Array<T> (etc).
// 03/25/2000: Class created, R. Nagy
// 08/03/2008: Ported over to stdnoj, R. Nagy
//
#ifndef AsClass1B_Hpp
#define AsClass1B_Hpp

template <class T>
class AsClass {
protected:
    T which;

public:

    AsClass(void) {
    }

    AsClass(const T& qt) {
        which = qt;
    }

    virtual ~AsClass(void) {
    }

    T & operator=(const T& qt) {
        which = qt;
        return which;
    }

    operator T(void) {
        return which;
    }

    T & operator *(void) {
        return which;
    }

    int operator==(T& qt) {
        return which == qt;
    }

    int operator!=(T& qt) {
        return which != qt;
    }

    int operator>=(T& qt) {
        return which >= qt;
    }

    int operator<=(T& qt) {
        return which <= qt;
    }

    int operator>(T& qt) {
        return which > qt;
    }

    int operator<(T& qt) {
        return which < qt;
    }

    ostream& write(ostream& os) {
        os << which << endl;
        return os;
    }

    istream& read(istream& is) {
        is >> which;
        return is;
    }
};

#endif
