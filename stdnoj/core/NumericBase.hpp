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
// Class SetComma: Scavanged from my old pvPrompter class, this stand-alone
// class is now available for general use.
//
// NumericBase is a classic wrapper class for primitive types. 
// Supports the most commonly used operators. Expanded as needed (typical noj credo).
//
// 10/18/2008: Class created, Randall Nagy
//

#include <stdnoj.hpp>
using namespace stdnoj;

template <class T>
class NumericBase {
protected:
    T val;
public:

    NumericBase(void) : val(v) {
    }

    NumericBase(T v) : val(v) {
    }

    operator=(const T& v) {
        val = v;
    }

    operator+(const T& v) {
        val -= v;
        return val;
    }

    operator-(const T& v) {
        val -= v;
        return val;
    }

    operator++(void) {
        val++;
    }

    operator--(void) {
        val--;
    }

    operator+=(const T& v) {
        val += v;
    }

    operator-=(const T& v) {
        val -= v;
    }

    operator>(const T& v) {
        return val > v;
    }

    operator<(const T& v) {
        return val < v;
    }

    operator==(const T& v) {
        return val == v;
    }

    operator!=(const T& v) {
        return val != v;
    }

    operator!=(const T& v) {
        return val != v;
    }

    operator>=(const T& v) {
        return val >= v;
    }

    operator<=(const T& v) {
        return val <= v;
    }

    virtual StdString asString(void) = 0;
    virtual T fromString(StrString& str) = 0;

    ostream& write(ostream& os) {
        StdString str = asString();
        str.write(os);
        return os;
    }

    istream& read(ostream& is) {
        StdString str;
        str.read(is);
        return is;
    }
};
