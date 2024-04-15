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
// Class Directory: Created to remove and add a name-space to all of theos functions that
// we are building up in the "FILE_FUN.HPP". Like File.hpp, only for directories.
//
// 12/22/1999: Class created, Randall Nagy
// 11/02/2002: Added GoTo and File / Directory Query capabilities, Randall Nagy
// 10/30/2003: Added QueryNode() and QueryParent() [to complement File signature], Randall Nagy
// 06/17/2008: Updated Name() to prefix with pwd for non-absolute path names, Randall Nagy
// 11/13/2008: Lightly updated to support commmon usage, Randall Nagy
// 02/28/2006: Added Launch(), Randall Nagy
// 08/28/2006: Added QueryTree(recursive) for File and Directory, Randall Nagy
//

#ifndef Directory_Hpp
#define Directory_Hpp

#include <core/StdString.hpp>  // includes Array<T>
#include <core/File.hpp>

namespace stdnoj {
    class File;
    class StdString;

    class Directory : public ShareableObject {
    protected:
        StdString sStack;
        StdString sPwd;

    public:
        Directory(void);
        Directory(const Directory& r);
        ~Directory(void);

        bool Name(const StdString& sName);
        const char *Name(void) const;

        // CommonObject
        bool FromString(const StdString&);
        const char *ToString(StdString&) const;

        const char *QueryNode(StdString& str) const; // ONLY the name of the directory (no path is included)
        const char *QueryParent(StdString& str) const; // ONLY the path to the directory (no node name)

        StdString QueryNode(void) const; // ONLY the name of the directory (no path is included)
        StdString QueryParent(void) const; // ONLY the path to the directory (no node name)
        bool QueryParent(Directory& dir) const; // Parent directory

        bool IsNull(void) const {
            if (sPwd.is_null()) return true;
            return false;
        }

        bool Exists(void) const;

        bool Home(File&) const; // place file   (leaf node) in the directory
        bool Home(Directory&) const; // place folder (leaf node) in the directory
        StdString Home(const StdString&) const; // place string (entire)    in the directory

        bool Create(void);
        bool Remove(void);
        bool Empty(void);

        bool Set(void);
        bool Set(const StdString& sDir);

        bool Get(void);

        bool Push(void);
        bool Pop(void);
        void EmptyStack(void);

        bool CreateDirectoryTree(void);

        bool CreateTreeFor(File& file);

        int operator==(const Directory& rDir) {
            return (sPwd == rDir.sPwd);
        }
        Directory & operator=(const Directory& rDir);

        ostream& write(ostream& os);
        istream& read(istream& is);

        bool GoTo(void);

        void Query(Array<File>&) const;
        void Query(Array<Directory>&) const;

        void QueryTree(Array<Directory>& aDirs) const; // Return a list of folders / directories (recursive)
        void QueryTree(Array<File>& aFiles) const; // Return a list of files (recursive)

        bool IsEmpty(void);

        bool Launch(void);
    };

} // stdnoj
#endif


