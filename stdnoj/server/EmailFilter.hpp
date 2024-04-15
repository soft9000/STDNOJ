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
// Class EMailFilter: Allows filtering of its namesake for desirability. Originally
// written to "grep" rfc977 items (news) for HyperRfc.
//
// 01/16/1998: Classes EMailFilter and EMailAND written, Randall Nagy.
// 01/17/1998: Added class EMailOR, Randall Nagy
// 12/26/1998: Added EMailAndOrNotExtended, Randall Nagy
// 09/23/2003: Ported to stdnoj, Randall Nagy
//
#ifndef zEMailFiltre_Hpp
#define zEMailFiltre_Hpp

#include <stdnoj.hpp>

#include <Server/Email.hpp>

#include <Server/HyperText01.hpp>

#include <Server/SearchData.hpp>

using namespace stdnoj;

class Proximity;

class Proximity {
protected:
    size_t which;
    long sp;

public:
    Proximity(void);

    void Reset(void);

    bool Found(void);
    long At(void);

    friend class EMailFilter;
};

typedef long unsigned mail_t;

class EMailFilter {
public:

    enum SearchRange {
        SEARCH_NEW,
        DEFAULT = SEARCH_NEW,
        GET_NEW,
        xSEARCH_ALL, // used by News*NET2
        GET_ALL,
        SEARCH_RANGE,
        GET_RANGE,
        LOCAL_SEARCH,
        TOO_BIG
    };
private:
    enum SearchRange eSearchRange;
    StdString sType;

protected:
    mail_t mtMaxArticles;
    bool Contains(const StdString&, EMail&, Proximity&);
    bool Contains(const StdString&, Proximity&, stringstream& srm);

public:

    enum SearchType {
        st_DEFAULT,
        st_AND,
        st_OR,
        st_ANDORNOT,
        st_ANDORNOTEX
    };

    EMailFilter(void);
    EMailFilter(const EMailFilter& ef);
    virtual ~EMailFilter(void);

    virtual bool Assign(enum SearchRange sr) {
        eSearchRange = sr;
        return true;
    }

    enum SearchRange Range(void) const {
        return eSearchRange;
    }

    void EmailExtension(const StdString& s) {
        sType = s;
    }

    const char *EmailExtension(void) {
        return sType.c_str();
    }

    mail_t MaxArticles(void) {
        return mtMaxArticles;
    }

    void MaxArticles(mail_t ma) {
        mtMaxArticles = ma;
    }

    virtual enum SearchType Type(void) {
        return st_DEFAULT;
    }

    virtual bool Qualifies(EMail& em) {
        return true;
    }

    virtual EMailFilter & operator=(const EMailFilter& ef);

    virtual ostream& write(ostream& os);
    virtual istream& read(istream& is);
};

// Used to see if EMail contains ALL of a series (logical AND);

class EMailAND : public EMailFilter {
protected:
    Array<StdString> sAND;

public:
    EMailAND(void);
    EMailAND(const EMailAND& ef);
    virtual ~EMailAND(void);

    virtual enum SearchType Type(void) {
        return st_AND;
    }

    virtual void AND(const StdString&);

    virtual bool Qualifies(EMail& em);

    virtual EMailAND & operator=(const EMailAND& ef);

    virtual ostream& write(ostream& os);
    virtual istream& read(istream& is);
};


// Used to see if EMail contains ONE of a series (logical OR);

class EMailOR : public EMailFilter {
protected:
    Array<StdString> sOR;

public:
    EMailOR(void);
    EMailOR(const EMailOR& ef);
    virtual ~EMailOR(void);

    virtual enum SearchType Type(void) {
        return st_OR;
    }

    virtual void OR(const StdString&);

    virtual bool Qualifies(EMail& em);

    virtual EMailOR & operator=(const EMailOR& ef);

    virtual ostream& write(ostream& os);
    virtual istream& read(istream& is);
};

class EMailAndOrNot : public EMailFilter {
protected:
    Array<BabyBool> aStrings;

public:
    EMailAndOrNot(void);
    EMailAndOrNot(const EMailAndOrNot& ef);
    virtual ~EMailAndOrNot(void);

    virtual SearchType Type(void) {
        return st_ANDORNOT;
    }

    // Returns false is string is not supported;
    virtual bool Assign(Array<StdString>&);
    virtual bool Assign(StdString&);

    virtual bool Qualifies(EMail& em);

    virtual EMailAndOrNot & operator=(const EMailAndOrNot&);

    virtual ostream& write(ostream& os);
    virtual istream& read(istream& is);
};

class SearchDataManager {
private:
    bool found;
    Array<bool> aHitResult;
    bool _and_check(void);

protected:
    EnhancedSearchData eSearch;
    bool AppendYourTokens(HyperTextSearch01&, size_t offset_base);
    void MarkAsFound(size_t offset_base, size_t token);
    bool CanWeContinue(void);
    bool WillUserWantThisOne(void);

public:

    SearchDataManager(void) : found(false) {
    }

    virtual ~SearchDataManager(void) {
    }

    virtual bool Assign(const char *psz);

    // "hit" storage areas;

    bool Found(void) {
        return found;
    }

    ostream& write(ostream& os) {
        return os;
    }

    istream& read(istream& is) {
        return is;
    }

    friend class EMailAndOrNotExtended;
};

class EMailAndOrNotExtended : public EMailFilter {
protected:
    Array<SearchDataManager> aStrings;

public:
    EMailAndOrNotExtended(void);
    EMailAndOrNotExtended(const EMailAndOrNotExtended& ef);
    virtual ~EMailAndOrNotExtended(void);

    // Returns false if string is not supported;
    virtual bool Assign(Array<StdString>&);
    virtual bool Assign(StdString&);

    virtual bool Assign(enum SearchRange sr) {
        return EMailFilter::Assign(sr);
    }

    virtual SearchType Type(void) {
        return st_ANDORNOTEX;
    }

    virtual bool Qualifies(EMail& em);

    virtual EMailAndOrNotExtended & operator=(const EMailAndOrNotExtended&);

    virtual ostream& write(ostream& os);
    virtual istream& read(istream& is);
};

#endif
