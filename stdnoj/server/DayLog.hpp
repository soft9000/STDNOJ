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
// Class DayLog: One log file a day, to keep same from becomming huge.
// Default is NOT to auto-wrap around every year, so as to not loose valuable log info.
//
// 06/18/2001: Class created, R. Nagy
// 09/23/2003: Ported to stdnoj, R. Nagy
//
#ifndef zDayLog_Hpp
#define zDayLog_Hpp

#include <Extra/StdLog.hpp>
#include <Extra/JulianDay.hpp>

class DayLog : public StdLog {
private:
    bool bAutoWrap;
    int jdate;
    JulianDay today;
    StdString sRootName;

public:

    DayLog(void) : bAutoWrap(false) {
        Name("jday.txt");
    }

    DayLog(const StdString& sfn) : bAutoWrap(false) {
        Name(sfn.c_str());
    }

    virtual ~DayLog(void) {
    }

    virtual bool Name(const StdString& sName);
    virtual bool Write(const StdString& sMessage);

    bool IsOneYearOnly(void) {
        return bAutoWrap;
    }

    bool OneYearOnly(bool b = true) {
        bAutoWrap = b;
        return true;
    }
};

inline
bool DayLog::Name(const StdString& pszName) {
    sRootName = pszName;

    StdString str;
    jdate = today.Today();
    today.Today(str);
    if (sRootName[0] != '.')
        str.append('.');
    str.append(sRootName);

    StdString str2;
    if (bAutoWrap == false) // our default, this is.
    {
        str2 = today.LocalYear();
        str2.append('.');
    }
    str2.append(str);
    return StdLog::Name(str2);
}

inline
bool DayLog::Write(const StdString& psz) {
    if (today.Today() != jdate)
        Name(sRootName);
    StdLog::PrintLine(psz);
    return true;
}

#endif
