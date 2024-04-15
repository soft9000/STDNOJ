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
#include <Server/hypertext01.hpp>

class HyperTextKey01 {
private:
    StdString key;
    int tok;

protected:
    virtual char Normalize(const char ch);

public:

    HyperTextKey01(void) : tok(NULL) {
    }

    HyperTextKey01(const HyperTextKey01& hk) {
        *this = hk;
    }

    virtual ~HyperTextKey01(void) {
    }

    void Assign(const StdString& sss, int token) {
        key = sss;
        tok = token;
    }

    const char * c_str(void) {
        return key.c_str();
    }

    int Token(void) {
        return tok;
    }

    bool IsKeyAt(const StdString& str);

    HyperTextKey01 & operator=(const HyperTextKey01& hk) {
        if (this == &hk) return *this;
        key = hk.key;
        tok = hk.tok;
        return *this;
    }

    int operator==(const HyperTextKey01& hk) {
        return (key == hk.key);
    }

    ostream& write(ostream& os) {
        key.write(os);
        os.write((char *) & tok, sizeof (int));
        return os;
    }

    istream& read(istream& os) {
        key.read(os);
        os.read((char *) & tok, sizeof (int));
        return os;
    }
};

const char *BabyBool::QueryPhrase(SearchType st) {
    switch (st) {
        case AND:
            return " <must contain> ";
        case NOT:
            return " <must not contain> ";
        case OR:
            return " <can contain> ";
        case ALL:
            return " <match everything> ";
    }
    return " <ERROR> ";
}

const char *BabyBool::Query(StdString& str) {
    str = QueryPhrase();
    str.append(c_str());
    return str.c_str();
}

bool BabyBool::Create(SearchType stt, const char *sData, bool bFound) {
    st = stt;
    assign(sData);
    found = bFound;
    return true;
}

bool BabyBool::Assign(const StdString& psz) {
    StdString str = psz;
    size_t sz = str.find('>');
    if (sz == NPOS)
        return false;
    str.remove(sz);
    if (str.find("must not") != NPOS) {
        st = NOT;
        goto leap22;
    }
    if (str.find("must ") != NPOS) {
        st = AND;
        goto leap22;
    }
    if (str.find("can not") != NPOS) // LEGACY SUPPORT (News*NET (ETC))
    {
        st = NOT;
        goto leap22;
    }
    if (str.find("can ") != NPOS) {
        st = OR;
        goto leap22;
    }
    if (str.find(" everyt") != NPOS) {
        st = ALL;
        goto leap22;
    }
    return false;
leap22:
    // Assign the query-string;
    str = psz;
    assign(str.substr(sz + 2));
    return true;
}

ostream& BabyBool::write(ostream& os) {
    os.write((char *) & st, sizeof (int));
    os.write((char *) & found, sizeof (bool));
    write_stream(os);
    return os;
}

istream& BabyBool::read(istream& is) {
    is.read((char *) & st, sizeof (int));
    is.read((char *) & found, sizeof (bool));
    read_stream(is);
    return is;
}

bool HyperTextKey01::IsKeyAt(const StdString& str) {
    if (str.find(key) == npos)
        return false;
    return true;
}

HyperTextSearch01::HyperTextSearch01(void) : ssWip(0L) {
}

HyperTextSearch01::~HyperTextSearch01(void) {
}

bool HyperTextSearch01::AddKey(const StdString& str, int token) {
    if (token == NPOS)
        return false;

    StdString sB = str;
    sB.mkwhite();
    sB.replace("   ", ' ');
    sB.replace("  ", ' ');
    sB.strip();
    sB.to_lower();
    if (sB.is_null())
        return false;

    (aKeys[aKeys.Nelem()]).Assign(sB, token);
    return true;
}

const char *HyperTextSearch01::GetKey(StdString& str, int token) {
    for (size_t ss = NULL; ss < aKeys.Nelem(); ss++) {
        if ((aKeys[ss]).Token() == token) {
            str = (aKeys[ss]).c_str();
            return str.c_str();
        }
    }
    return NULL;
}

int HyperTextSearch01::FindKey(const StdString& sBuffer) {
    if (sBuffer.is_null())
        return NPOS;

    if (!aKeys.Nelem())
        return NPOS;

    ssWip = 0L;
    sSaveBuffer = sBuffer;
    sSaveBuffer.mkwhite();
    sSaveBuffer.replace("   ", ' ');
    sSaveBuffer.replace("  ", ' ');
    sSaveBuffer.strip();
    sSaveBuffer.to_lower();
    return NextKey();
}

int HyperTextSearch01::FindNextKey(void) {
    if (!aKeys.Nelem())
        return NPOS;
    ssWip++;
    if (ssWip > aKeys.Nelem())
        ssWip = 0L;
    return NextKey();
}

int HyperTextSearch01::NextKey(void) {
    if (!aKeys.Nelem())
        return NPOS;
    for (size_t ss = ssWip; ss < aKeys.Nelem(); ss++) {
        if ((aKeys[ss]).IsKeyAt(sSaveBuffer) == true) {
            ssWip = ss;
            return ((aKeys[ss]).Token());
        }
    }
    ssWip = 0L;
    return NPOS;
}
