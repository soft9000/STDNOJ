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

#ifndef z_RoutingSlip
#define z_RoutingSlip

#include <stdnoj.hpp>
using namespace stdnoj;

class RoutingSlip {
private:
    StdString _to;
    StdString _from;
    StdString _subject;

public:

    RoutingSlip(void) {
    }

    RoutingSlip(const RoutingSlip& rs) {
        *this = rs;
    }

    virtual ~RoutingSlip(void) {
    }

    bool IsNull(void) {
        if (_to.is_null() && _from.is_null() && _subject.is_null()) return true;
        return false;
    }

    void From(const StdString& ss) {
        _from = ss;
    }

    void To(const StdString& ss) {
        _to = ss;
    }

    void Subject(const StdString& ss) {
        _subject = ss;
    }

    const char *To(void) const {
        return _to.c_str();
    }

    const char *From(void) const {
        return _from.c_str();
    }

    const char *Subject(void) const {
        return _subject.c_str();
    }

    RoutingSlip & operator=(const RoutingSlip& rs) {
        if (this == &rs) return *this;
        _from = rs._from;
        _to = rs._to;
        _subject = rs._subject;
        return *this;
    }
};

#endif

