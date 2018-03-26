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
#include <stdnoj.hpp>

using namespace stdnoj;

namespace stdnoj {

    // "Liberates" a name by removing any drive designations
    // and platform specific path separations.

    StdString FilePath::LiberatedName(const StdString& strIn, char chSep) {
        StdString str = strIn;
        if (str[1] == ':')
            str.remove_pos(0, 1);
        if (chSep != '\\')
            str.replace('\\', chSep);
        if (chSep != '/')
            str.replace('/', chSep);
        return str;
    }

    // Always a nagging problem to solve...

    StdString FilePath::Glue(const StdString& sFolder, const StdString& sNode, char chSep) {
        StdString sResult = sFolder;
        sResult.append(chSep);
        sResult.append(sNode);
        return sResult;
    }

    void FilePath::Chop(const StdString& sName, StdString& sPath, StdString& sFile, char chSep) {
        sPath = sFile = "";
        size_t whence = sName.rfind(chSep);
        if (whence == NPOS) {
            sPath = sName;
            return;
        }
        sPath = sName.substr(0, whence);
        sFile = sName.substr(whence + 1, sName.length());
    }

} // stdnoj

#ifdef UNIX
#include <core/unix/unix_FilePath.cpp>
#endif

#ifdef MSDOS
#include <core/msdos/msdos_FilePath.cpp>
#endif

#ifdef WIN32
#include <core/win32/win32_FilePath.cpp>
#endif


