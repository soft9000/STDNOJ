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
// These classes are used to quickly find multiple keys in a ascii-z text.
//
// Classes created 12/28/1998, R. Nagy
// 10/16/1999: Added capability to search binary-buffers, R. Nagy
// 09/23/2003: Ported to stdnoj, R. Nagy
//
#ifndef zHyperText_Hpp
#define zHyperText_Hpp

#include <stdnoj.hpp>

using namespace stdnoj;

class BabyBool : public StdString {
public:

    enum SearchType {
        NONE, AND, OR, NOT, ALL
    };
private:
    int st;
    bool found;
public:

    BabyBool(void) : st(NONE), found(false) {
    }

    virtual ~BabyBool(void) {
    }

    void SetType(SearchType stt) {
        st = stt;
    }

    SearchType QueryType(void) {
        return (SearchType) st;
    }
    const char *QueryPhrase(SearchType);

    const char *QueryPhrase(void) {
        return QueryPhrase((SearchType) st);
    }

    virtual bool Create(SearchType st, const char *pRawData, bool bFound = false);
    virtual bool Assign(const StdString& sQueryFormat);
    virtual const char * Query(StdString& sResult);

    bool Found(void) {
        return found;
    }

    void Found(bool b) {
        found = b;
    }

    BabyBool & operator=(const StdString& str) {
        Assign(str.c_str());
        return *this;
    }

    BabyBool & operator=(const BabyBool& str) {
        if (this == &str) return *this;
        Create((SearchType) str.st, str.c_str(), str.found);
        return *this;
    }


    ostream& write(ostream& os);
    istream& read(istream& is);
};


class HyperTextKey01;

class HyperTextSearch01 {
private:
    StdString sSaveBuffer;
    size_t ssWip;

protected:
    Array<HyperTextKey01> aKeys;

    int NextKey(void);

public:
    HyperTextSearch01(void);
    virtual ~HyperTextSearch01(void);

    bool AddKey(const StdString& sKey, int token);
    const char *GetKey(StdString& str, int token);

    int FindKey(const StdString& sBuffer);
    int FindNextKey(void);
};
#endif
