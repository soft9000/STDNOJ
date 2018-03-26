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
// Class StdPath: We need a common path storage mechanism, as well as
// a way to associate relative paths to the htree type om "mount
// points" (Directory, File, and Url)
//
// 08/25/2005: Created, R. Nagy
//
#ifndef STD_PATH_HPP
#define STD_PATH_HPP

#include <core/File.hpp>
#include <core/Directory.hpp>

namespace stdnoj {

    class StdPath {
    private:
        bool _relate(const StdString& sFullPath, StdString& sResult);
    protected:
        bool _resolve(const StdString& sMount, const StdString& sNode, StdString& sResult);
    public:

        class Path {
        protected:
            StdString sPath;
        public:
            Path(const Path& ref);
            Path(const StdString& ref);

            Path & operator=(const StdString& str);
            operator StdString(void);

            ostream& write(ostream& os);
            istream& read(istream& is);
        };

        // Existance is not verified.
        static bool Mount(const Path&, const Url&, Url& uResult);
        static bool Mount(const Path&, const Url&, File& uResult);
        static bool Mount(const Path&, const Url&, Directory& uResult);

        static bool Mount(const Path&, const File&, File& uResult);
        static bool Mount(const Path&, const File&, Url& uResult);

        static bool Mount(const Path&, const Directory&, Directory& uResult);
        static bool Mount(const Path&, const Directory&, Url& uResult);
    };

} // stdnoj
#endif
