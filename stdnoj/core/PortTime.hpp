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
// Class PortTime: Use this when you just want to store a human-readable time.
// 01/08/1997: Class created (Document project, 32bit update), R. Nagy
//    Interfaced same with Node1 and GetFileTime classes.
// 11/02/1999: Added Encode/DecodeDate() and Encode/DecodeTime(), R. Nagy
// 03/29/2001: Fixed operator < and >, R. Nagy
// 12/05/2001: Added Extract(struct tm), R. Nagy
// 11/24/2008: Extracted implementation into it's own file, R. Nagy
//
#ifndef PORT_TIME_HPP
#define PORT_TIME_HPP

#include <stdnoj.hpp>
#include <ctime>         // mktime() and time_t

// SEE ALSO;
#include <core/StdDateTime.hpp>   // for Masks..

namespace stdnoj {

    class PortTime {
    protected:

        struct goomey {
            int YY;
            int DD;
            int MM;
            int SS;
            int Mn;
            int HH;
        } goo;
    public:
        PortTime(void);
        virtual ~PortTime(void);

        int Day(void) const;
        int Month(void) const;
        int Year(void) const;

        int Hour24(void) const;
        int Minute(void) const;
        int Second(void) const;

        void DateYMD(int yy, int mm, int dd);
        void TimeHMS(int hh, int mm, int ss);

        void EncodeDate(int yy, int mm, int dd);
        // DO NOT USE REFERENCES HERE: Causes problem under C++ Builder 5.x;
        void DecodeDate(int *yy, int *mm, int *dd);

        void EncodeTime(int hh, int mm, int ss);
        // DO NOT USE REFERENCES HERE: Causes problem under C++ Builder 5.x;
        void DecodeTime(int *hh, int *mm, int *ss);

        PortTime & operator =(const PortTime& pt);

        int operator ==(const PortTime& pt) const;

        int operator !=(const PortTime& pt) const;

        int operator>(const PortTime& pt) const;

        int operator<(const PortTime& pt) const;

        // Default date format is StdDateTime::sdtMasks::AsMDY()
        const char *Format(StdString& str, const char *pFmt = 0L) const;

        ostream& write(ostream& os);
        istream& read(istream& is);
        time_t Extract(struct tm& rtm);
        time_t Extract(void);
    };

} // stdnoj
#endif
