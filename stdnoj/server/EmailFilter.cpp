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
#include <Server/EmailFilter.hpp>

Proximity::Proximity(void) {
    Reset();
}

void Proximity::Reset(void) {
    which = (size_t) - 1L;
    sp = (long) - 1L;
}

bool Proximity::Found(void) {
    return (sp == (long) - 1L) ? true : false;
}

long Proximity::At(void) {
    return sp;
}

EMailFilter::EMailFilter(void) : mtMaxArticles(-1L), eSearchRange(EMailFilter::GET_ALL), sType(".TXT") {
}

EMailFilter::EMailFilter(const EMailFilter& ef) {
    *this = ef;
}

EMailFilter::~EMailFilter(void) {
}

EMailFilter& EMailFilter::operator=(const EMailFilter& ef) {
    if (this == &ef)
        return *this;
    eSearchRange = ef.eSearchRange;
    mtMaxArticles = ef.mtMaxArticles;
    sType = ef.sType;
    return (*this);
}

ostream& EMailFilter::write(ostream& os) {
    os.write((char *) & eSearchRange, sizeof (enum SearchRange));
    sType.write_stream(os);
    return os;
}

istream& EMailFilter::read(istream& is) {
    is.read((char *) & eSearchRange, sizeof (enum SearchRange));
    sType.read_stream(is);
    return is;
}

bool EMailFilter::Contains(const StdString& what, EMail& em, Proximity& result) {
    stringstream srm;
    em.write(srm);
    srm << ends;
    return Contains(what, result, srm);
}

bool EMailFilter::Contains(const StdString& w, Proximity& result, stringstream& srm) {
    StdString what = w;
    what.to_upper();

    StdString std = PRESTO(srm).c_str();
    std.to_upper();

    if (size_t(result.sp) == -1L)
        result.sp = (size_t) std.find(what.c_str());
    else
        result.sp = (size_t) std.find(what.c_str(), size_t(result.sp) + 1);
    if (size_t(result.sp) == -1L)
        return false;
    return true;
}

EMailAND::EMailAND(void) {
}

EMailAND::EMailAND(const EMailAND& ef) {
    *this = ef;
}

EMailAND::~EMailAND(void) {
}

void EMailAND::AND(const StdString& std) {
    if (!std.is_null())
        sAND[sAND.Nelem()] = std;
}

bool EMailAND::Qualifies(EMail& em) {
    Proximity where;
    for (size_t ss = NULL; ss < sAND.Nelem(); ss++) {
        if (Contains(sAND[ss], (EMail&) em, where) == false)
            return false;
    }
    return true;
}

EMailAND& EMailAND::operator=(const EMailAND& ef) {
    if (this == &ef)
        return *this;
    sAND = ef.sAND;
    return *this;
}

ostream& EMailAND::write(ostream& os) {
    sAND.write(os);
    return EMailFilter::write(os);
}

istream& EMailAND::read(istream& is) {
    sAND.read(is);
    return EMailFilter::read(is);
}

EMailOR::EMailOR(void) {
}

EMailOR::EMailOR(const EMailOR& ef) {
    *this = ef;
}

EMailOR::~EMailOR(void) {
}

void EMailOR::OR(const StdString& std) {
    if (!std.is_null())
        sOR[sOR.Nelem()] = std;
}

bool EMailOR::Qualifies(EMail& em) {
    Proximity where;
    for (size_t ss = NULL; ss < sOR.Nelem(); ss++) {
        if (Contains(sOR[ss], (EMail&) em, where) == true)
            return true;
    }
    return false;
}

EMailOR& EMailOR::operator=(const EMailOR& ef) {
    if (this == &ef)
        return *this;
    sOR = ef.sOR;
    return *this;
}

ostream& EMailOR::write(ostream& os) {
    sOR.write(os);
    return EMailFilter::write(os);
}

istream& EMailOR::read(istream& is) {
    sOR.read(is);
    return EMailFilter::read(is);
}

EMailAndOrNot::EMailAndOrNot(void) {
}

EMailAndOrNot::EMailAndOrNot(const EMailAndOrNot& ef) {
    *this = ef;
}

EMailAndOrNot::~EMailAndOrNot(void) {
}

bool EMailAndOrNot::Assign(StdString& stdstr) {
    return (aStrings[aStrings.Nelem()]).Assign(stdstr.c_str());
}

bool EMailAndOrNot::Assign(Array<StdString>& aStdString) {
    aStrings.Free();
    for (size_t ss = NULL; ss < aStdString.Nelem(); ss++) {
        StdString str = (aStdString[ss]).c_str();

        if ((aStrings[ss]).Assign(str.c_str()) == false)
            return false;
    }
    return true;
}

bool EMailAndOrNot::Qualifies(EMail& em) {
    HyperTextSearch01 search;

    size_t ss;
    for (ss = NULL; ss < aStrings.Nelem(); ss++) {
        (aStrings[ss]).Found(false);
        if (search.AddKey((aStrings[ss]).c_str(), ss) == false)
            return false;
    }

    stringstream srm;
    em.write_searchable(srm);
    srm << ends;

    int gotcha = NULL;
    size_t which = search.FindKey(PRESTO(srm).c_str());
    while (which != NPOS) {
        gotcha = 1;
        (aStrings[which]).Found(true);

        // We can process NOT's right away;
        if ((aStrings[which]).QueryType() == BabyBool::NOT)
            return false;

        which = search.FindNextKey();
    }
    if (!gotcha)
        return false;

    // NOT success-determination;
    // -------
    // By this point, we have;
    //    o Rejected any NOTs.

    bool or_result = false;
    bool and_result = false;
    for (ss = NULL; ss < aStrings.Nelem(); ss++) {
        // Did we find all of the "must finds" (AND)?
        if (aStrings[ss].QueryType() == BabyBool::AND) {
            if (aStrings[ss].Found() == false)
                return false; // Bail out now: Very important to venn-diagram for tallying final result.
            else
                and_result = true; // Successfull search will clear all ANDs rejections (below).
            continue;
        }

        // Did we find any "can contain" (OR)?
        if (aStrings[ss].QueryType() == BabyBool::OR) {
            if (aStrings[ss].Found() == true)
                or_result = true;
        }
    }

    // AND success-determination;
    // -------
    // By this point, we have either;
    //    o Found all ANDs (or have none to find).
    //    o Flagged a found-state at the existance of ALL good ANDs.
    // (i.e: If we get this far, then we will have encounterd ALL ands, if and_result is set)
    if (and_result == true)
        return and_result;

    // OR success-determination;
    // -------
    // By this point, we have;
    //    o Flagged a found-state if we found any OR's.
    return or_result;
}

EMailAndOrNot& EMailAndOrNot::operator=(const EMailAndOrNot& eff) {
    if (this == &eff)
        return *this;
    aStrings = eff.aStrings;
    return *this;
}

ostream& EMailAndOrNot::write(ostream& os) {
    aStrings.write(os);
    return EMailFilter::write(os);
}

istream& EMailAndOrNot::read(istream& is) {
    aStrings.read(is);
    return EMailFilter::read(is);
}

bool SearchDataManager::Assign(const char *psz) {
    if (eSearch.ImportControlString(psz) == false)
        return false;
    return true;
}

void SearchDataManager::MarkAsFound(size_t offset_base, size_t token) {
    size_t ss = token - offset_base;
    aHitResult[ss] = true;
    found = true;
}

bool SearchDataManager::CanWeContinue(void) {
    if (found && eSearch.MustNotContain() && (eSearch.OneOrMore() == true))
        return false;

    return true;
}

bool SearchDataManager::AppendYourTokens(HyperTextSearch01& search, size_t offset_base) {
    Array<StreamString> aWords;
    if (eSearch.GetWords(aWords) == false)
        return false;

    // Eeeeewwwwwhhh;
    aHitResult.Free();
    // Pre-allocate the zombies;
    aHitResult[aWords.Nelem() - 1];
    for (size_t ss = NULL; ss < aWords.Nelem(); ss++) {
        if (search.AddKey(aWords[ss].c_str(), offset_base + ss) == false)
            return false;
        aHitResult[ss] = false;
    }
    return true;
}

bool SearchDataManager::WillUserWantThisOne(void) {
    bool br = false;
    // NOT success-determination;
    // -------
    // By this point, we have;
    //    o Rejected any NOTs, or easy-outs by them using our "CanWeContinue()",
    //    but for the sake of maintenance, we'll check it one more time;
    if (CanWeContinue() == false)
        return br;

    if (eSearch.MustHaveAll()) {
        for (size_t ss = NULL; ss < aHitResult.Nelem(); ss++) {
            // IF we don't have ALL;
            if (aHitResult[ss] == false) {
                // IF we are doing a "NOT" for "ALL", then we CAN have SOME of the NOTs,
                // -just as long as they are LESS THAN "ALL";
                if (eSearch.MustNotContain() == true)
                    return true;
                // IF we are doing an "ALL", then we MUST HAVE ALL, so we must bail;
                return false;
            }
        }

        // Okay! We have them ALL: Is that what the user requested?
        if (eSearch.MustNotContain() == true)
            return false;
        return true;
    }
    if (eSearch.OneOrMore()) {
        for (size_t ss = NULL; ss < aHitResult.Nelem(); ss++) {
            // IF we have ONE;
            if (aHitResult[ss] == true) {
                // If it must not contain one of these, then our result is obvious;
                if (eSearch.MustNotContain())
                    return false;
                // Otherwise, we have found one, so the user will want to see it;
                return true;
            }
        }

        // Okay! We didn't have one. Is that what the user requested?
        if (eSearch.MustNotContain())
            return true;
        else
            return false;
    }
    // ?? (dung happened)
    return false;
}

bool SearchDataManager::_and_check(void) {
    bool br = false;
    for (size_t ss = NULL; ss < aHitResult.Nelem(); ss++) {
        if (aHitResult[ss] == false) {
            if (eSearch.MustHaveAll())
                return false;
        } else {
            if (eSearch.OneOrMore())
                return true;
            br = true;
        }
    }
    return br;
}

EMailAndOrNotExtended::EMailAndOrNotExtended(void) {
}

EMailAndOrNotExtended::EMailAndOrNotExtended(const EMailAndOrNotExtended& ef) {
    *this = ef;
}

EMailAndOrNotExtended::~EMailAndOrNotExtended(void) {
}

bool EMailAndOrNotExtended::Assign(StdString& stdstr) {
    return (aStrings[aStrings.Nelem()]).Assign(stdstr.c_str());
}

bool EMailAndOrNotExtended::Assign(Array<StdString>& aStdString) {
    aStrings.Free();
    for (size_t ss = NULL; ss < aStdString.Nelem(); ss++) {
        StdString str = (aStdString[ss]).c_str();

        if ((aStrings[ss]).Assign(str.c_str()) == false)
            return false;
    }
    return true;
}

bool EMailAndOrNotExtended::Qualifies(EMail& em) {
    HyperTextSearch01 search;

    size_t ss;
    for (ss = NULL; ss < aStrings.Nelem(); ss++) {
        (aStrings[ss]).found = false;
        if ((aStrings[ss]).AppendYourTokens(search, ss * 1000) == false)
            return false;
    }

    stringstream srm;
    em.write_searchable(srm);
    srm << ends;

    int gotcha = NULL;
    size_t which = search.FindKey(PRESTO(srm).c_str());
    while (which != NPOS) {
        // Reduce the token back into the base of the controller that it represents;
        size_t ss = which / 1000;
        gotcha = 1;
        (aStrings[ss]).MarkAsFound(ss * 1000, which);

        if ((aStrings[ss]).CanWeContinue() == false)
            return false;

        which = search.FindNextKey();
    }

    /*
    // Nope! This short-cut only works if we are AND-ing.
    // Since we are now NOT-ing too, it no longer can be assumed.
    if(!gotcha)
       return false;
     */


    // NOT success-determination;
    // -------
    // By this point, we have;
    //    o Rejected any NOTs, or (other exclusively-easy-to reject stuff);
    for (ss = NULL; ss < aStrings.Nelem(); ss++) {
        if (aStrings[ss].WillUserWantThisOne() == false)
            return false;
    }
    return true;
}

EMailAndOrNotExtended& EMailAndOrNotExtended::operator=(const EMailAndOrNotExtended& eff) {
    if (this == &eff)
        return *this;
    aStrings = eff.aStrings;
    return *this;
}

ostream& EMailAndOrNotExtended::write(ostream& os) {
    aStrings.write(os);
    return EMailFilter::write(os);
}

istream& EMailAndOrNotExtended::read(istream& is) {
    aStrings.read(is);
    return EMailFilter::read(is);
}


