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
#ifdef BORLAND_C
#include <vcl.h>
#pragma package(smart_init)
#else
#include <windows.h>
#endif

#pragma hdrstop

#include "searchdata.hpp"

//---------------------------------------------------------------------------

SearchData::SearchData(void) {
    Reset();
}

SearchData::~SearchData(void) {
}

void SearchData::Reset(void) {
    type = 0;
    sKeyString = "";
    bMustContain = false;
}

bool SearchData::ExportControlString(StdString& std) {
    if (sKeyString.is_null())
        return false;
    std = sKeyString;
    return true;
}

bool SearchData::ImportControlString(const StdString& std) {
    bool br = false;

    StdString std2 = std;
    size_t whence = std2.find('>');
    if (whence == NPOS) {
        type = 1;
        sKeyString = std;
        return true;
    }

    type = 1;
    std2[whence] = NULL;
    if (std2.find(" not ") == NPOS)
        MustContain(true);
    else
        MustContain(false);

    sKeyString = std;
    return true;
}

EnhancedSearchData::EnhancedSearchData(void) {
    Reset();
}

EnhancedSearchData::~EnhancedSearchData(void) {
}

bool SearchData::GetWords(Array<StreamString>& aWords) {
    aWords.Free();
    if (sKeyString.is_null())
        return false;

    size_t whence = sKeyString.find('>');
    if (whence != NPOS) {
        // NOTE THAT WE ADD 2 TO SKIP BOTH THE TOKEN AND THE TRAILING SPACE!
        aWords[aWords.Nelem()] = &sKeyString[whence + 2];
        return true;
    }

    aWords[aWords.Nelem()] = sKeyString.c_str();
    return true;
}

bool EnhancedSearchData::PutWords(Array<StreamString>& aWords) {
    bool br = true;
    stringstream srm;

    // *****
    // First, we give the user something to read;
    srm << "ENHANCE(";
    for (size_t ss = NULL; ss < aWords.Nelem(); ss++) {
        if (ss)
            srm << ", ";
        srm << aWords[ss].c_str();
    }

    // *****
    // Now here is the stuff that WE read;
    srm << ")                             \t";
    aWords.write(srm);

    // STORE a unique (non GUI) "seek to" marker;
    srm << '\v';
    // This a bninary choice, so lets us a "grep-able" character;;
    if (bOneOrMore)
        srm << '+';
    else
        srm << '-';

    // This a bninary choice, so lets use a "grep-able" character;;
    if (bWholeWordsOnly)
        srm << '+';
    else
        srm << '-';

    srm << ends;
    sKeyString = PRESTO(srm).c_str();

    return br;
}

bool EnhancedSearchData::GetWords(Array<StreamString>& aWords) {
    bool br = false;
    aWords.Free();
    size_t where = sKeyString.find('\t');
    if (where == NPOS)
        return SearchData::GetWords(aWords);
    stringstream srm;
    srm << &(sKeyString[where + 1]);
    srm << ends;
    srm.seekg(0, ios::beg);
    aWords.read(srm);
    if (aWords.Nelem())
        br = true;
    return br;
}

bool EnhancedSearchData::ImportControlString(const StdString& std) {
    Reset();
    if (SearchData::ImportControlString(std) == false)
        return false;

    size_t where = sKeyString.find('\v');
    if (where != NPOS) {
        type = 3;
        if (std[where + 1] == '+')
            OneOrMore(true);
        else
            OneOrMore(false);

        if (std[where + 2] == '+')
            WholeWordsOnly(true);
        else
            WholeWordsOnly(false);
    }
    return true;
}

void EnhancedSearchData::Reset(void) {
    SearchData::Reset();
    bOneOrMore = true;
    bWholeWordsOnly = false;
}

bool EnhancedSearchData::OneOrMore(void) {
    return bOneOrMore;
}

bool EnhancedSearchData::WholeWordsOnly(void) {
    return bWholeWordsOnly;
}

bool EnhancedSearchData::ExportControlString(StdString& std) {
    return SearchData::ExportControlString(std);
}
