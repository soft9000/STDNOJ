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
// 11/13/2008: Converted to static usage, R. Nagy
//
#ifndef FilePath_Hpp
#define FilePath_Hpp

#include <stdnoj.hpp>
#include <core/Node.hpp>
#include <core/File.hpp>
#include <core/Directory.hpp>

namespace stdnoj {
    class File;

    class FilePath {
    public:
        char PathChar(void); // "/" on UNIX, "\" on DOS, ":" on PICK (etc.)

        static bool Open(const StdString& sPath, Node& result);
        static bool Open(const StdString& sPath, File& result);
        static bool Open(const StdString& sPath, Directory& result);

        static void Chop(const StdString& sPath, StdString& sFolder, StdString& sNode, char chSep);
        static StdString Glue(const StdString& sFolder, const StdString& sNode, char chSep);
        static StdString LiberatedName(const StdString&, char chSep = '|'); // "Liberates" a name by removing and drive designations & platform specific path separations.
    };
    // Always a nagging problem to solve...

    inline char PathChar(void) {
        FilePath fp;
        return fp.PathChar();
    }
    inline StdString Glue(const StdString& sFolder, const StdString& sNode, char chSep);

} // stdnoj

#endif   
