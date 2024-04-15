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
// Class StdDateTime: Place to hang all of the STANDARD (portable) date-and-time
// conversions and conventions.
//
// 1999/05/10: Class created, Randall Nagy
// 2001/05/09: Growing into a replacement for TDate amd TTime, Randall Nagy
// 2001/12/05: Added IsNull() and expanded support for user_assigned to recent operators, Randall Nagy
// 2003/10/30: Added EncodeMMDDYY(const StdString& str) and EncodeHHMMSS(...), Randall Nagy
// 2003/11/03: Added IsSameDate(), Randall Nagy
// 2008/04/26: Added Import / ExportDottedEditable(), Randall Nagy
// 2008/10/11: Updated EncodeDate() and EncodeTime() to preserve any complementing date / time aspect, Randall Nagy
// 2008/10/14: Changed read() and write() to use an ASCII format by default. Legacy(binary) format detected and converted on read as required, Randall Nagy
// 2008/12/06: Added operators >= and <= to support S.P.A.W.N, Randall Nagy
//

#ifndef StdDateTime_Hpp
#define StdDateTime_Hpp

#include <stdnoj.hpp>
#include <ctime>

// SEE ALSO
// #include <core/PortTime.hpp>

namespace stdnoj {

    class StdDateTime {
    private:
        int user_assigned;

    protected:
        // Maybe some day we'll find a reason to make this more
        // efficient, but there is no reason to do so, now;
        struct tm ttm;

    public:
        StdDateTime(void);
        StdDateTime(const StdDateTime& ss);
        StdDateTime(time_t tt);
        virtual ~StdDateTime(void);

        int IsDefaultTime(void) {
            return (!user_assigned);
        }

        bool IsNull(void) const {
            return (!user_assigned);
        }

        virtual StdDateTime & operator=(const StdDateTime& ss);

        StdDateTime & operator=(struct tm& keep);
        StdDateTime & operator=(time_t tt);

        operator time_t(void);

        int operator==(const StdDateTime& sdt) const;
        int operator!=(const StdDateTime& sdt) const;
        StdDateTime & operator+=(int sec);
        StdDateTime & operator-=(int sec);
        int operator>(const StdDateTime& sdt) const;
        int operator<(const StdDateTime& sdt) const;
        int operator>=(const StdDateTime& sdt) const;
        int operator<=(const StdDateTime& sdt) const;

        bool IsSameDate(const StdDateTime& sdt) const;

        // Year is fully qualified, though year before 1970 is not supported;
        bool EncodeDate(unsigned yy, unsigned mm, unsigned dd, StdString& sError);

        bool EncodeDate(unsigned yy, unsigned mm, unsigned dd) {
            StdString sError;
            return EncodeDate(yy, mm, dd, sError);
        }
        bool DecodeDate(unsigned *yy, unsigned *mm, unsigned *dd) const;
        bool DecodeDate(unsigned &yy, unsigned &mm, unsigned &dd) const;

        bool EncodeJulian(unsigned yy, unsigned jDay) {
            if (yy < 1900)
                return false;
            struct tm ttm2;
            ::memset(&ttm2, 0, sizeof (struct tm));
            ttm2.tm_yday = jDay;
            if (jDay)
                ttm2.tm_yday--;
            ttm2.tm_year = yy - 1900;
            *this = ttm2;
            return true;
        }

        // Hour is in military time;
        bool EncodeTime(unsigned hh, unsigned mm, unsigned ss);
        bool DecodeTime(unsigned *hh, unsigned *mm, unsigned *ss) const;
        bool DecodeTime(unsigned &hh, unsigned &mm, unsigned &ss) const;

        bool EncodeMMDDYY(const StdString& str, StdString& sError);
        bool EncodeMMDDYYYY(const StdString& str, StdString& sError);
        // Hour is in military time;
        bool EncodeHHMMSS(const StdString& str, StdString& sError);

        int IsDaylightSavingsTime(void) {
            return ttm.tm_isdst;
        }

        int DayOfWeek(void) {
            return ttm.tm_wday + 1;
        } // (Sunday is the first day of the week)

        int JulianDay(void) {
            return ttm.tm_yday + 1;
        } // (January 1st is the first day of the year)

        int Month(void) {
            return (ttm.tm_mon + 1);
        } // months since January - [0,11]

        int DayOfMonth(void) {
            return ttm.tm_mday;
        } // day of the month - [1,31]

        int Year(void) {
            return (ttm.tm_year + 1900);
        } // years since 1900   

        int Hour(void) {
            return ttm.tm_hour;
        } // hours since midnight - [0,23]

        int Minute(void) {
            return ttm.tm_min;
        } // minutes after the hour - [0,59]

        void AddYear(void) {
            ttm.tm_year += 1;
        }

        void NextDay(void) {
            operator+=(86400);
        }

        void PreviousDay(void) {
            operator-=(86400);
        }

        ostream& write_bin(ostream& os) const;
        istream& read_bin(istream& is);

        ostream& write_text(ostream& os) const;
        istream& read_text(istream& is);

        ostream& write(ostream& os) const;
        istream& read(istream& is);
    };

    struct sdtMasks {

        static const char *AsHMS(void) {
            return "%02d:%02d:%02d";
        }

        static const char *AsMDY(void) {
            return "%02d/%02d/%02d";
        }

        static const char *AsMDYHMS(void) {
            return "%02d/%02d/%02d %02d:%02d:%02d";
        }
    };

    StdString AsHMS(time_t total_seconds, const char *pszMask = sdtMasks::AsHMS());
    StdString AsMDY(time_t total_seconds, const char *pszMask = sdtMasks::AsMDY());
    StdString AsMDYHMS(time_t total_seconds, const char *pszMask = sdtMasks::AsMDYHMS());

    ostream & operator<<(ostream& os, const StdDateTime& sdt);
    istream & operator>>(istream& is, StdDateTime& sdt);

    bool ExportDottedEditable(const StdDateTime& std, ostream& os);
    bool ImportDottedEditable(StdDateTime& std, istream& is);

    StdString ExportDottedEditable(const StdDateTime& sdt);
    bool ImportDottedEditable(StdDateTime& sdt, const StdString& str);
    bool IsLeapYear(unsigned uYear);

} // stdnoj

#endif


