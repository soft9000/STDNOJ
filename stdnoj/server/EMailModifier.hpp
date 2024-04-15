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

#ifndef z_EMailModifier
#define z_EMailModifier

#include <stdnoj.hpp>
using namespace stdnoj;

class EMailModifier {
protected:
    EMail *pEm;
public:

    EMailModifier(void) : pEm(NULL) {
    }

    virtual ~EMailModifier(void) {
    }

    void Assign(EMail *pem) {
        pEm = pem;
    }

    // Outbound modifiers;

    bool To(const StdString& sto) {
        if (!pEm) return false;
        pEm->_to = sto;
        return true;
    }

    bool Newsgroups(const StdString& sto) {
        if (!pEm) return false;
        pEm->_newsg = sto;
        return true;
    }

    bool From(const StdString& sto) {
        if (!pEm) return false;
        pEm->_from = sto;
        return true;
    }

    bool Subject(const StdString& sto) {
        if (!pEm) return false;
        pEm->_subject = sto;
        return true;
    }

    bool Message(const StdString& sto) {
        if (!pEm) return false;
        pEm->_text = sto;
        return true;
    }

    // Other modifiers <<<WIP>>>

    bool Forward(const StdString& ToList) {
        return false;
    }

    bool ReplyAll(void) {
        return false;
    }

    bool ReplySender(void) {
        return false;
    }
};

#endif

