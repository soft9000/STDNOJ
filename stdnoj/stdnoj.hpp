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

// 06/21/2003: Need to modernize my huge legacy.
// 08/21/2008: Moved more headers from older to newer (cstdio, etc.)
// 11/13/2008: Added CommonObject. Applied parentage to File, Directory, and Node, Randall Nagy
// 11/24/2008: Re-compiling under MSDOS, Randall Nagy
// 07/15/2008: Re-compiling under WIN32, Randall Nagy
// 07/18/2008: Re-compiling under cygwin, Randall Nagy

#ifndef stdnoj1
#define stdnoj1

//#define StdString    zstring
#define StreamString n_stream

#include <sstream>
#include <core/StdString.hpp>
#include <core/CommonObject.hpp>
#include <core/Array.hpp>
//#include <core/CompArray.hpp>

using namespace std;

#define Max(a, b) (a > b) ? a : b;
#define Min(a, b) (a < b) ? a : b;

namespace stdnoj {
    /**
     * class StdString
     *
     * @author Randall Nagy
     * @comment Considering the gradual introduction of JAVA style documentation a-la CppDoc -
     */
    class StdString;

    /**
     * PRESTO:
     * @param srm A stream
     * @result A project string
     * @comment This function used to be the way to stop strstream memory leaks.
     * It was the very reason why StdString had an "own" capability. These days
     * it just converts between STL strings and the project string class.
     */
    StdString PRESTO(stringstream& srm);

    typedef int TROOL;
    const int tTRUE = (int) true;
    const int tFALSE = (int) false;
    const int tERROR = 9;

#ifndef WIN32
    typedef int HANDLE;
#endif
#ifndef MAX_PATH
    const int MAX_PATH = 256;
#endif
} // stdnoj;

#include <core/ByteBuffer.hpp>
#include <core/FileSystem.hpp>

#include <core/ProgramId.hpp>
#include <core/StdSeek.hpp>
#include <core/RecArray.hpp>
#include <core/dfo.hpp>
#include <core/Platform.hpp>
#include <core/Utilities.hpp>
#include <core/Url.hpp>
#include <core/StdPath.hpp>

#include <cstdlib>

// Select a pseudo random number between `min` and `max` (inclusive)

inline int RandRange(int min, int max) {
    return ((::rand() % (int) (((max) + 1) - (min))) + (min));
}

inline int Rnd(int max = 1000) {
    if (!max)
        max = 1;
    srand((unsigned int) time(NULL));
    return ((rand() >> 3) % max);
}



#endif
