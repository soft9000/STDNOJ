#include <stdnoj.hpp>

namespace stdnoj {

    PortTime::PortTime(void) {
        ::memset(&goo, 0, sizeof (goo));
    }

    PortTime::~PortTime(void) {
    }

    int PortTime::Day(void) const {
        return goo.DD;
    }

    int PortTime::Month(void) const {
        return goo.MM;
    }

    int PortTime::Year(void) const {
        return goo.YY;
    }

    int PortTime::Hour24(void) const {
        return goo.HH;
    }

    int PortTime::Minute(void) const {
        return goo.Mn;
    }

    int PortTime::Second(void) const {
        return goo.SS;
    }

    void PortTime::DateYMD(int yy, int mm, int dd) {
        goo.MM = mm;
        goo.DD = dd;
        goo.YY = yy;
    }

    void PortTime::TimeHMS(int hh, int mm, int ss) {
        goo.HH = hh;
        goo.Mn = mm;
        goo.SS = ss;
    }

    void PortTime::EncodeDate(int yy, int mm, int dd) {
        DateYMD(yy, mm, dd);
    }
    // DO NOT USE REFERENCES HERE: Causes problem under C++ Builder 5.x;

    void PortTime::DecodeDate(int *yy, int *mm, int *dd) {
        *yy = goo.YY;
        *mm = goo.MM;
        *dd = goo.DD;
    }

    void PortTime::EncodeTime(int hh, int mm, int ss) {
        TimeHMS(hh, mm, ss);
    }
    // DO NOT USE REFERENCES HERE: Causes problem under C++ Builder 5.x;

    void PortTime::DecodeTime(int *hh, int *mm, int *ss) {
        *hh = goo.HH;
        *mm = goo.Mn;
        *ss = goo.SS;
    }

    PortTime& PortTime::operator =(const PortTime& pt) {
        if (this == &pt)
            return *this;
        goo.MM = pt.goo.MM;
        goo.DD = pt.goo.DD;
        goo.YY = pt.goo.YY;
        goo.HH = pt.goo.HH;
        goo.Mn = pt.goo.Mn;
        goo.SS = pt.goo.SS;
        return *this;
    }

    int PortTime::operator ==(const PortTime& pt) const {
        // Could also just do a memcmp() here, but this would make an assumption
        // about byte packing and portability that I no longer wish to infer ....
        if ((goo.MM == pt.goo.MM) && (goo.DD == pt.goo.DD) && (goo.YY == pt.goo.YY) &&
                (goo.HH == pt.goo.HH) && (goo.Mn == pt.goo.Mn) && (goo.SS == pt.goo.SS))
            return 1;
        return 0;
    }

    int PortTime::operator !=(const PortTime& pt) const {
        return (!(*this == pt));
    }

    int PortTime::operator>(const PortTime& pt) const {
        if (goo.YY != pt.goo.YY)
            return (goo.YY > pt.goo.YY);
        if (goo.MM != pt.goo.MM)
            return (goo.MM > pt.goo.MM);
        if (goo.DD != pt.goo.DD)
            return (goo.DD > pt.goo.DD);
        if (goo.HH != pt.goo.HH)
            return (goo.HH > pt.goo.HH);
        if (goo.Mn != pt.goo.Mn)
            return (goo.Mn > pt.goo.Mn);
        if (goo.SS != pt.goo.SS)
            return (goo.SS > pt.goo.SS);
        return 0; // they are EQUAL
    }

    int PortTime::operator<(const PortTime& pt) const {
        if (goo.YY != pt.goo.YY)
            return (goo.YY < pt.goo.YY);
        if (goo.MM != pt.goo.MM)
            return (goo.MM < pt.goo.MM);
        if (goo.DD != pt.goo.DD)
            return (goo.DD < pt.goo.DD);
        if (goo.HH != pt.goo.HH)
            return (goo.HH < pt.goo.HH);
        if (goo.Mn != pt.goo.Mn)
            return (goo.Mn < pt.goo.Mn);
        if (goo.SS != pt.goo.SS)
            return (goo.SS < pt.goo.SS);
        return 0; // they are EQUAL
    }

    const char *PortTime::Format(StdString& str, const char *pFmt) const {
        if (!pFmt)
            pFmt = sdtMasks::AsMDY();
        char *pbuf = new char[80];
        sprintf(pbuf, pFmt, goo.MM, goo.DD, goo.YY, goo.HH, goo.Mn, goo.SS);
        str.assign(pbuf);
        delete [] pbuf;
        return str.c_str();
    }

    ostream& PortTime::write(ostream& os) {
        os << " ";
        os << goo.MM << " ";
        os << goo.DD << " ";
        os << goo.YY << " ";
        os << goo.HH << " ";
        os << goo.Mn << " ";
        os << goo.SS << " ";
        return os;
    }

    istream& PortTime::read(istream& is) {
        int by;
        is.get((char *) & by, 1);
        is >> goo.MM;
        is >> goo.DD;
        is >> goo.YY;
        is >> goo.HH;
        is >> goo.Mn;
        is >> goo.SS;
        is.get((char *) & by, 1);
        return is;
    }

    time_t PortTime::Extract(struct tm& rtm) {
        rtm.tm_sec = goo.SS; /* Seconds */
        rtm.tm_min = goo.Mn; /* Minutes */
        rtm.tm_hour = goo.HH; /* Hour (0--23) */
        rtm.tm_mday = goo.DD; /* Day of month (1--31) */
        rtm.tm_mon = goo.MM - 1; /* Month (0--11) */
        rtm.tm_year = goo.YY - 1900; /* Year (calendar year minus 1900) */
        //rtm.tm_wday;           /* Weekday (0--6; Sunday = 0) */
        //rtm.tm_yday;           /* Day of year (0--365) */
        //rtm.tm_isdst;          /* 0 if daylight savings time is not in effect) */
        return ::mktime(&rtm);
    }

    time_t PortTime::Extract(void) {
        struct tm rtm;
        ::memset(&rtm, 0, sizeof (struct tm));
        return Extract(rtm);
    }

} // stdnoj
