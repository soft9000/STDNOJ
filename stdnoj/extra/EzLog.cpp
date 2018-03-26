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

#include <extra/EzLog.hpp>
#include <extra/WordWrap.hpp>
#include <extra/Disclaimer.hpp>
#include "LogLineReader.hpp"



const char *lfn = ".ezlog";

// Why will NetBeans not SEE LogFormat?
#include <extra/stdlog.hpp>

inline StdString LogFormat(const StdString& msg, const StdString& sStrftime = "%m/%d/%Y %H:%M:%S - ") {
    size_t iMax = sStrftime.length() * 5;
    char *psz = new char[iMax];

    // Concoct the time prefix in the requested format
    time_t pwTime;
    tm *ptm;
    time(&pwTime);
    ptm = localtime(&pwTime);
    strftime(psz, iMax - 1, sStrftime.c_str(), ptm);

    // Encode Result
    StdString sResult;
    stringstream srm;
    srm << psz << msg;
    srm << ends;
    sResult = PRESTO(srm);

    return sResult;
}

struct HtmlLog {

    static void Start(ostream & os) {
        File file;
        ProgramId pid;
        pid.GetProgram(file);
        os << "<html>" << endl;
        os << "<title>" << file.Name() << "</title>";

        os << "<body LINK=\"#0000ff\" VLINK=\"#800080\" BGCOLOR=\"#00ffff\">" << endl;

        File file2;
        file2.Name("Soft9000.gif");
        pid.PutInDirectory(file2);
        os << "<a href=\"http://www.soft9000.com\"><img src=\"";
        os << file2.Name();
        os << "\" alt=\"About the author\" border=\"0\" width=\"309\" height=\"42\"></a>";
        os << endl;

        os << "<FONT SIZE=2 FACE=\"Courier New\" COLOR=\"#800000\">";
        os << "<br>" << endl;
        os << "<p><i>These data have been brought to you by </i>";
        os << file.Name();
        os << "<i>, which is part of the <b>Stdnoj Project</b>.</i>";
        os << "</p></FONT>" << endl;

        StdDateTime sdt;
        os << "<FONT SIZE=1 FACE=\"Courier New\"> <p>";
        os << "Logging report generated on: <i>" << AsMDYHMS(sdt) << "</i>" << endl;
        os << "<br>" << endl;
        os << "</p></FONT>" << endl;
    }

    static void Stop(ostream & os) {
        os << "<br><hr>" << endl;
        os << "</body>" << endl;
        os << "</html>" << endl;
    }

    static void Write(StdString& sTime, StdString& sPayload, ostream & os) {
        StdString str;
        os << "<hr>";
        os << "<b>" << sTime << "</b><br>";
        os << "<p><FONT SIZE=3 FACE=\"Courier New\" COLOR=\"#008000\">";
        os << sPayload;
        os << "</p></FONT>" << endl;
        os << "<br>";
        os << endl;
    }

};

EzLog::EzLog(void) {
    setDefault();
}

void EzLog::setDefault(void) {
    Directory dir;
    if (dir.Get() == true) {
        ProgramId pid;
        pid.GetProgram(dir);
        if (setDefaultLog(dir) == false)
            pwFile.Name(lfn);
    }
}

bool EzLog::setDefaultLog(Directory& dir) {
    stringstream srm;
    srm << dir.Name() << PathChar() << lfn << ends;
    return pwFile.Name(PRESTO(srm));
}

bool EzLog::setDerivedLog(File& file) {
    stringstream srm;
    srm << file.Name() << lfn << ends;
    return pwFile.Name(PRESTO(srm));
}

bool EzLog::hasDefaultLog(Directory& dir) {
    StdString str;
    Array<File> aFiles;
    dir.Query(aFiles);
    for (size_t ss = 0L; ss < aFiles.Nelem(); ss++) {
        str = aFiles[ss].QueryNode();
        if (str == lfn)
            return true;
    }
    return false;
}

void EzLog::locateDefaultLog(void) {
    Directory dir;
    if (dir.Get() == true) {
        if (hasDefaultLog(dir) == true) {
            setDefaultLog(dir);
            return;
        }

        StdString sDir = dir.QueryParent();
        while (1) {
            if (dir.Name(sDir) == true) {
                if (hasDefaultLog(dir)) {
                    setDefaultLog(dir);
                    return;
                }
            }
            StdString sHold = dir.QueryParent();
            if (sDir == sHold) {
                setDefault();
                return;
            }
            sDir = sHold;
        }
    }

    setDefault();
}

bool EzLog::writeLog(const StdString& message) {
    StdString sResult;
    return writeLog(message, sResult);
}

bool EzLog::writeLog(const StdString& message, StdString& sResult) {
    bool br = false;
    ostream& os = pwFile.OpenAppend();
    sResult = ::LogFormat(message);
    os << sResult << endl;
    if (os)
        br = true;
    pwFile.Close();
    return br;
}

void EzLog::Usage(ostream& os) {
    File file;
    ProgramId pid;
    pid.GetProgram(file);

    StdString str = file.QueryNode();
    os << endl;
    os << str << " - By rnagy@Soft9000.com, " << __DATE__ << endl;
    for (size_t ss = 0L; ss < str.length(); ss++)
        os << '-';
    os << endl << endl;

    os << "Time stamp or list log messages into either the present folder (.) or" << endl;
    os << "to a dynamically located parental log else " << EzLog::logDefaultName(str) << " (=) " << endl;
    os << endl;
    os << "Usage:" << endl;
    os << "-----" << endl;

    os << file.QueryNode() << " . message" << endl;
    os << "   -Log the message to an " << lfn << " in the current folder." << endl;

    os << endl << "or " << endl << endl;
    os << file.QueryNode() << " $ message" << endl;
    os << "   -Log the message to the default (EzLog.exe) program folder." << endl;

    os << endl << "or " << endl << endl;
    os << file.QueryNode() << " = message" << endl;
    os << "   -Log the message to an " << lfn << " in the ancestor OR default program folder." << endl;

    os << endl << "or " << endl << endl;
    os << file.QueryNode() << " @" << endl;
    os << "   -Format and display the log file." << endl;

    os << endl << "or " << endl << endl;
    os << file.QueryNode() << " #" << endl;
    os << "   -Export the log as a tab-delimited file." << endl;
    os << endl;
    Disclaimer::Header(os);
    os << endl;
}

bool EzLog::showLog(ostream& os) {
    return showLog(pwFile, os);
}

bool EzLog::exportLog(ostream& os) {
    return exportLog(pwFile, os);
}

bool EzLog::exportLog(const File& fff, ostream& os) {
    StdString sLine;

    File file = fff;
    istream& is = file.OpenRead();
    LogLineReader::LogLine result;
    while (is) {
        sLine.readline(is);
        if (!is)
            continue;
        LogLineReader::Parse(sLine, result);
        os << result.sDate << "\t" << result.sTime << "\t" << result.sPayload << endl;
    }
    file.Close();
    return true;
}

bool EzLog::showLog(const File& file, ostream& os) {
    bool br = showHtmlLog(file);
    if (br)
        return br;
    return showTextLog(file, os);
}

bool EzLog::showHtmlLog(const File& fff) {
    StdString sTime, sPayload, sLine;

    File fnOut = StdString(fff.Name()) + ".html";
    ostream& os = fnOut.OpenWrite();
    HtmlLog::Start(os);

    File file = fff;
    istream& is = file.OpenRead();
    while (is) {
        sLine.readline(is);
        if (!is)
            continue;
        size_t pos = sLine.find("-");
        if (pos == npos) {
            sTime = "";
            sPayload = sLine;
        } else {
            sTime = sLine.subpos(0, pos);
            sPayload = sLine.subpos(pos, NPOS);
        }
        HtmlLog::Write(sTime, sPayload, os);
    }

    HtmlLog::Stop(os);
    fnOut.Close();
    file.Close();
    return fnOut.Launch();
}

bool EzLog::showTextLog(const File& file, ostream& os) {
    File f2 = file;
    bool br = file.Exists();
    cout << file.Name() << endl;
    if (br) {
        Array<StdString> aLines;
        StdString sLine;
        istream& is = f2.OpenRead();
        while (is) {
            sLine.readline(is);
            if (!is)
                continue;
            aLines = TextFormatting::WordWrap::Wrap(sLine, 0, 0, 77);
            for (size_t ss = 0L; ss < aLines.Nelem(); ss++)
                os << aLines[ss] << endl;
            os << "-----" << endl;
        }
    }
    return br;
}

const char *EzLog::logDefaultName(StdString& str) {
    EzLog log;
    log.setDefault();
    str = log.logName();
    return str.c_str();
}

bool EzLog::Main(int argc, char *argv[], ostream& os) {
    StdString str;
    Array<StdString> array;
    for (size_t ss = 0L; ss < argc; ss++) {
        str = argv[ss];
        array.Append(str);
    }
    return Main(array, os);
}

bool EzLog::Main(Array<StdString>& array, ostream& os) {
    bool bHelp = true;

    EzLog log;

    if (array[1] == "@") {
        log.locateDefaultLog();
        return log.showLog(cout);
    }

    if (array[1] == "#") {
        log.locateDefaultLog();
        return log.exportLog(cout);
    }

    if (array[1] == ".") {
        Directory dir;
        dir.Get();
        if (log.setDefaultLog(dir) == false) {
            os << "Waring: Unable to set log file to " << dir.Name() << ". Using default." << endl;
            log.locateDefaultLog();
        }
        bHelp = false;
    }

    if (array[1] == "$") // default - as used
    {
        log.setDefault();
        bHelp = false;
    }

    if (array[1] == "=") // locate the default - in the tree or otherwise
    {
        log.locateDefaultLog();
        bHelp = false;
    }

    if (bHelp) {
        Usage(os);
        if (array.Nelem() <= 2)
            return true;
        return false;
    }

    // Adjust for WIN32 command shell oddities
    stringstream srm;
    for (size_t ss = 2; ss < array.Nelem(); ss++)
        srm << array[ss] << " " << endl;;
    StdString str = PRESTO(srm);
    str.strip();
    StdString sResult;
    bool br = log.writeLog(str, sResult);
    if (br == false)
        os << "Error: Unable to log message to file." << endl;
    else
        os << log.logName() << ":" << endl;
    os << sResult;
    return br;
}


