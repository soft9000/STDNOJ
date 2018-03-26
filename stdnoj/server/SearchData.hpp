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
//---------------------------------------------------------------------------
#ifndef SearchDataH
#define SearchDataH
//---------------------------------------------------------------------------

#include <stdnoj.hpp>

// 09/23/2003: Ported to stdnoj, R. Nagy

using namespace stdnoj;

class SearchData {
protected:
    int type;
    StdString sKeyString;
    bool bMustContain;

public:
    SearchData(void);
    virtual ~SearchData(void);

    virtual void Reset(void);

    // These must read and write your data in such a manner as to allow their
    // user-discernable display / parsing in a standard WINDOWS control.
    virtual bool ExportControlString(StdString& std);
    virtual bool ImportControlString(const StdString& std);

    virtual bool GetWords(Array<StreamString>&);

    bool MustContain(void) {
        return bMustContain;
    }

    bool MustNotContain(void) {
        return !bMustContain;
    }

    void MustContain(bool b) {
        bMustContain = b;
    }

    void MustNotContain(bool b) {
        bMustContain = !b;
    }
};

class EnhancedSearchData : public SearchData {
protected:
    bool bOneOrMore;
    bool bWholeWordsOnly;
public:
    EnhancedSearchData(void);
    virtual ~EnhancedSearchData(void);

    bool IsEnhancedSearch(void) {
        if (type == 3) return true;
        return false;
    }

    virtual void Reset(void);

    // These must read and write your data in such a manner as to allow their
    // user-discernable display / parsing in a standard WINDOWS control.
    virtual bool ExportControlString(StdString& std);
    virtual bool ImportControlString(const StdString& std);

    virtual bool GetWords(Array<StreamString>&);
    virtual bool PutWords(Array<StreamString>&);

    void OneOrMore(bool b) {
        bOneOrMore = b;
    }
    bool OneOrMore(void);

    bool MustHaveAll(void) {
        return (!bOneOrMore);
    }

    bool WholeWordsOnly(void);

    void WholeWordsOnly(bool b) {
        bWholeWordsOnly = b;
    }
};


#endif
