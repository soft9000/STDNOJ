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
#include <stdnoj.hpp>
#include <sstream>
using namespace std;

namespace stdnoj {

    ostream & operator<<(ostream& os, const StdDateTime& sdt) {
        StdDateTime tmp = sdt;
        os << ' ' << time_t(tmp) << ' ';
        return os;
    }

    istream & operator>>(istream& is, StdDateTime& sdt) {
        char ch;
        time_t tt = 0L;
        is.read(&ch, 1);
        is.read((char *) & tt, sizeof (time_t));
        is.read(&ch, 1);
        sdt = tt;
        return is;
    }

    StdDateTime::StdDateTime(void) {
        time_t tt = time(NULL);
        ::memcpy(&ttm, localtime(&tt), sizeof (struct tm));
        user_assigned = 0;
    }

    StdDateTime::StdDateTime(const StdDateTime& ss) {
        *this = ss;
    }

    StdDateTime::StdDateTime(time_t tt) {
        struct tm *pt = localtime(&tt);
        if (pt) {
            ::memcpy(&ttm, pt, sizeof (struct tm));
            user_assigned = 1;
        } else {
            time_t tt = time(NULL);
            ::memcpy(&ttm, localtime(&tt), sizeof (struct tm));
            user_assigned = 0;
        }
    }

    StdDateTime::~StdDateTime(void) {
    }

    bool StdDateTime::IsSameDate(const StdDateTime& sdt) const {
        if ((ttm.tm_mday == sdt.ttm.tm_mday) &&
                (ttm.tm_mon == sdt.ttm.tm_mon) &&
                (ttm.tm_year == sdt.ttm.tm_year))
            return true;
        return false;
    }

    StdDateTime& StdDateTime::operator=(time_t tt) {
        user_assigned = 1;
        StdDateTime sdt(tt);
        *this = sdt;
        return *this;
    }

    StdDateTime& StdDateTime::operator+=(int sec) {
        user_assigned = 1;
        time_t ttime = (time_t) * this;
        ttime += sec;
        ::memcpy((char *) & ttm, (char *) localtime(&ttime), sizeof (struct tm));
        return *this;
    }

    bool StdDateTime::EncodeMMDDYYYY(const StdString& str, StdString& sError) {
        int mm, dd, yy;
        mm = dd = yy = 0;
        if (::sscanf(str.c_str(), "%d/%d/%d", &mm, &dd, &yy) != 3) {
            sError = "Unsupported date format.";
            return false;
        }
        return EncodeDate(yy, mm, dd, sError);
    }

    bool StdDateTime::EncodeMMDDYY(const StdString& str, StdString& sError) {
        int mm, dd, yy;
        mm = dd = yy = 0;
        if (::sscanf(str.c_str(), "%d/%d/%d", &mm, &dd, &yy) != 3) {
            sError = "Unsupported date format.";
            return false;
        }
        if (yy < 70)
            yy += 2000;
        else
            yy += 1900;
        return EncodeDate(yy, mm, dd, sError);
    }

    bool StdDateTime::EncodeHHMMSS(const StdString& str, StdString& sError) {
        sError = "Unsupported time format.";
        int hh, mm, ss;
        hh = mm = ss = 0;
        if (::sscanf(str.c_str(), "%d:%d:%d", &hh, &mm, &ss) != 3)
            return false;
        if (EncodeTime(hh, mm, ss) == false)
            return false;
        sError = "";
        return true;
    }

    StdDateTime& StdDateTime::operator-=(int sec) {
        user_assigned = 1;
        time_t ttime = (time_t) * this;
        ttime -= sec;
        ::memcpy((char *) & ttm, (char *) localtime(&ttime), sizeof (struct tm));
        return *this;
    }

    StdDateTime& StdDateTime::operator=(const StdDateTime& ss) {
        if (this == &ss)
            return *this;
        ::memcpy(&ttm, &ss.ttm, sizeof (struct tm));
        user_assigned = ss.user_assigned;
        return *this;
    }

    StdDateTime& StdDateTime::operator=(struct tm& keep) {
        user_assigned = 1;
        ::memcpy((char *) & ttm, (char *) & keep, sizeof (struct tm));
        ::mktime(&ttm); // normalize it.
        return *this;
    }

    int StdDateTime::operator==(const StdDateTime& sdt) const {
        StdDateTime sampB = sdt;
        StdDateTime sampA = (*this);
        time_t tA = time_t(sampA);
        time_t tB = time_t(sampB);
        return (tA == tB);
    }

    int StdDateTime::operator!=(const StdDateTime& sdt) const {
        StdDateTime sampB = sdt;
        StdDateTime sampA = (*this);
        time_t tA = time_t(sampA);
        time_t tB = time_t(sampB);
        return (tA != tB);
    }

    int StdDateTime::operator>(const StdDateTime& sdt) const {
        StdDateTime sampB = sdt;
        StdDateTime sampA = (*this);
        time_t tA = time_t(sampA);
        time_t tB = time_t(sampB);
        return (tA > tB);
    }

    int StdDateTime::operator<(const StdDateTime& sdt) const {
        StdDateTime sampB = sdt;
        StdDateTime sampA = (*this);
        time_t tA = time_t(sampA);
        time_t tB = time_t(sampB);
        return (tA < tB);
    }

    int StdDateTime::operator>=(const StdDateTime& sdt) const {
        int ires = (*this == sdt);
        if (ires)
            return ires;
        return (*this > sdt);
    }

    int StdDateTime::operator<=(const StdDateTime& sdt) const {
        int ires = (*this == sdt);
        if (ires)
            return ires;
        return (*this < sdt);
    }

    StdDateTime::operator time_t(void) {
        /*
        The mktime function converts the supplied time structure
        (possibly incomplete) pointed to into a fully defined structure
        with normalized values and then converts it to a time_t calendar
        time value.
         */
        return ::mktime(&ttm);
    }

    bool StdDateTime::DecodeDate(unsigned *yy, unsigned *mm, unsigned *dd) const {
        *yy = (ttm.tm_year + 1900);
        *mm = (ttm.tm_mon + 1);
        *dd = ttm.tm_mday;
        return true;
    }

    bool StdDateTime::DecodeDate(unsigned &yy, unsigned &mm, unsigned &dd) const {
        yy = (ttm.tm_year + 1900);
        mm = (ttm.tm_mon + 1);
        dd = ttm.tm_mday;
        return true;
    }

    bool StdDateTime::EncodeDate(unsigned yy, unsigned mm, unsigned dd, StdString& sError) {
        struct tm ztm;
        ::memset(&ztm, 0, sizeof (struct tm));
        ztm.tm_hour = ttm.tm_hour;
        ztm.tm_min = ttm.tm_min;
        ztm.tm_sec = ttm.tm_sec;
        if (yy < 1970) {
            sError = "Year is less than 1970";
            return false;
        }
        ztm.tm_year = yy - 1900;
        if (!mm) {
            sError = "Month is ZERO.";
            return false;
        }
        if (mm > 12) {
            sError = "Month is greater than 12";
            return false;
        }
        ztm.tm_mon = mm - 1;
        if (!dd) {
            sError = "Day is ZERO.";
            return false;
        }
        if (dd > 31) {
            sError = "Day of month is greater than 31";
            return false;
        }
        ztm.tm_mday = dd;

        *this = StdDateTime(mktime(&ztm));

        user_assigned = 1;
        return true;
    }

    bool StdDateTime::EncodeTime(unsigned hh, unsigned mm, unsigned ss) {
        struct tm ztm;
        ::memset(&ztm, 0, sizeof (struct tm));

        ztm.tm_year = ttm.tm_year;
        ztm.tm_mon = ttm.tm_mon;
        ztm.tm_mday = ttm.tm_mday;

        ztm.tm_hour = hh;
        ztm.tm_min = mm;
        ztm.tm_sec = ss;

        *this = StdDateTime(mktime(&ztm));

        user_assigned = 1;
        return true;
    }

    bool StdDateTime::DecodeTime(unsigned *hh, unsigned *mm, unsigned *ss) const {
        *hh = ttm.tm_hour;
        *mm = ttm.tm_min;
        *ss = ttm.tm_sec;
        return true;
    }

    bool StdDateTime::DecodeTime(unsigned &hh, unsigned &mm, unsigned &ss) const {
        hh = ttm.tm_hour;
        mm = ttm.tm_min;
        ss = ttm.tm_sec;
        return true;
    }

    ostream& StdDateTime::write(ostream& os) const {
        return write_text(os);
    }

    istream& StdDateTime::read(istream& is) {
        char ch;
        is.get(ch);
        is.unget();
        if (ch == '+')
            return read_bin(is); // Convert the older format -
        return read_text(is);
    }

    ostream& StdDateTime::write_bin(ostream& os) const {
        os.write("+2", 2);
        os.write((const char *) & ttm, sizeof (struct tm));
        os.write((char *) & user_assigned, sizeof (int));
        return os;
    }

    istream& StdDateTime::read_bin(istream& is) {
        char version[5];
        is.read((char *) & version, 2);
        is.read((char *) & ttm, sizeof (struct tm));
        if (version[1] == '1')
            user_assigned = 0;
        else
            is.read((char *) & user_assigned, sizeof (int));
        return is;
    }

    ostream& StdDateTime::write_text(ostream& os) const {
        os << ExportDottedEditable(*this);
        return os;
    }

    istream& StdDateTime::read_text(istream& is) {
        StdString str;
        str.readline(is, '$');
        str.append('$');
        ImportDottedEditable(*this, str);
        return is;
    }

    StdString AsHMS(time_t total_seconds, const char *pszMask) {
        StdDateTime sdt(total_seconds);
        unsigned h, m, s;
        sdt.DecodeTime(&h, &m, &s);
        char buf[80];
        ::sprintf(buf, pszMask, h, m, s);
        StdString str = buf;
        return str;
    }

    StdString AsMDY(time_t total_seconds, const char *pszMask) {
        StdDateTime sdt(total_seconds);
        unsigned y, m, d;
        sdt.DecodeDate(&y, &m, &d);
        char buf[80];
        ::sprintf(buf, pszMask, m, d, y);
        StdString str = buf;
        return str;
    }

    StdString AsMDYHMS(time_t total_seconds, const char *pszMask) {
        StdDateTime sdt(total_seconds);
        unsigned y, m, d, h, min, s;
        sdt.DecodeDate(&y, &m, &d);
        sdt.DecodeTime(&h, &min, &s);
        char buf[80];
        ::sprintf(buf, pszMask, m, d, y, h, min, s);
        StdString str = buf;
        return str;
    }

    StdString ExportDottedEditable(const StdDateTime& sdt) {
        StdString sResult;
        stringstream srm;
        if (ExportDottedEditable(sdt, srm) == true)
            sResult = PRESTO(srm);
        return sResult;
    }

    bool ExportDottedEditable(const StdDateTime& sdt, ostream& os) {
        unsigned int a, b, c;
        if (sdt.DecodeDate(&a, &b, &c) == false)
            return false;
        unsigned int d, e, f;
        if (sdt.DecodeTime(&d, &e, &f) == false)
            return false;
        os << "Date[" << a << "." << b << "." << c << "]+";
        os << "Time[" << d << "." << e << "." << f << "]$";
        return true;
    }

    bool ImportDottedEditable(StdDateTime& sdt, istream& is) {
        StdString str;
        str.readline(is, '$');
        if (!is)
            return false;
        return ImportDottedEditable(sdt, str);
    }

    bool ImportDottedEditable(StdDateTime& sdt, const StdString& sss) {
        StdString str = sss;
        int a, b, c, d, e, f;
        str.to_upper();
        if (::sscanf(str.c_str(), "DATE[%d.%d.%d]+TIME[%d.%d.%d]", &a, &b, &c, &d, &e, &f) != 6)
            return false;
        if (sdt.EncodeDate(a, b, c, str) == false)
            return false;
        if (sdt.EncodeTime(d, e, f) == false)
            return false;
        return true;
    }

    bool IsLeapYear(unsigned uYear) {
        if ((uYear % 4) == 0) {
            // A leap year is divisible by 4 ...
            if (uYear % 100 == 0) {
                // ... unless it's divisible by 100 in which case it isn't ...
                if (uYear % 400 == 0)
                    // ... unless it's divisible by 400!
                    return true;
                else
                    return false;
            }
            return true;
        }
        return false;
    }


} // stdnoj
