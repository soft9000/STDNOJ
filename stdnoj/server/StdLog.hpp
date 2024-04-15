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
// Class StdLogger: Class to allow for the creation of a fully-qualified log-file. Need to keep the log
// files from floatin' round! Also needed to get access to the Format() for GUI / non-stream based logging.
//
// 01/17/1998: StdLog class created, Randall Nagy.
// 01/21/1998: Broke-out StdLogger to return formatted string (Better for GUI, etc), Randall Nagy
//             Added the Print/PrintLine(os, psz, 1) idea form the StdSocket LOG capability.
// 09/19/2003: Ported StdLogger over to stdnoj/extra/StdLog.hpp, Randall Nagy
//             Added a few functions & changed poor constructor init logic.
//
#ifndef zStdLogger_Hpp
#define zStdLogger_Hpp

#include <stdnoj.hpp>

using namespace stdnoj;

#define DEFAULT_LOG_NAME "StdNojLog2.Log"

class StdLog {
protected:
    StdString sLogFileName;
    StdString sLine;

public:

    StdLog(void) : sLogFileName(DEFAULT_LOG_NAME) {
    }

    StdLog(const StdString& rLogFile) : sLogFileName(rLogFile) {
    }

    virtual ~StdLog(void) {
    }

    virtual bool Name(const StdString& cstrName);

    const char * Name(void) const {
        return sLogFileName.c_str();
    }

    bool Assign(const File& file);
    bool Query(File& file);

    bool Delete(void);

    const char *Format(const StdString& cstr);

    const char *Print(const StdString& cstr);
    const char *PrintLine(const StdString& cstr);

    virtual const char *Print(ostream& os, const StdString& psz, int LogCapture = 1);
    virtual const char *PrintLine(ostream& os, const StdString& psz, int LogCapture = 1);

    ostream& write(ostream& os) {
        sLogFileName.write_stream(os);
        return os;
    }

    istream& read(istream& is) {
        sLogFileName.read_stream(is);
        return is;
    }
};


#endif
