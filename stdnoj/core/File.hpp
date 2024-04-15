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
// Class File: Written to encapsulate all of those MS / ANSI stream differences.
// Also serves as a place to wrapper all of those FILE_FUN functions.
// 11/12/1998: Class written, R. Nagy.
// 04/02/1999: This thing could have only worked previously if file A/E and Name()
//    was FQ? --Now it handles the unknown much better, R. Nagy.
//       Fixed potential maintenance bugs by checking if FQFname was !NULL.
//       Rewrote Qualify().
//       Changed Create() from OPEN_EXISITING to CREATE_ALWAYS.
// 07/24/1999: Added Set/QueryScopeDestruct(), R. Nagy
// 10/16/1999: Finally unified the i/o streams by adding OpenReadWrite(), R. Nagy
// 12/13/1999: Added Resume(), R. Nagy
// 12/22/1999: Moved implementation over to File.cpp so as to remove bleed-throughs 
//             to items in "File_Fun", R. Nagy
// 12/27/1999: Went over to the "xFile.hpp" convention. Allows classes to be used
//             in external linkange, as well as inline (legacy), R. Nagy.
// 01/15/2000: NOTE: Remove() usage changed to now imply a "close", as first, R. Nagy
// 06/02/2001: Added IsNull(), R. Nagy
// 06/06/2001: Tested and fixed IsNull(), R. Nagy
// 11/22/2001: Added Time(time_t). Updated Time(void) to use same [Feliez de la estaci'on del gracias!], R. Nagy
// 06/11/2002: Time to add an operator ==(). Compares file NAMES, only (CASE SENSITIVE), R. Nagy
// 07/06/2002: Added QueryFileNode(), R. Nagy
// 07/22/2002: Added LoadTextFile(), R. Nagy
// 11/20/2002: Added LoadStreamBuffer(), R. Nagy
// 05/08/2003: Added CanRead() / CanWrite(), R. Nagy
// 10/30/2003: Added CopyTo(dir) and Query(dir), R. Nagy
// 10/30/2003: Added QueryNode() and QueryParent() [to complement Directory signature], R. Nagy
// 02/07/2008: Removed inline from standalone function definitions, R. Nagy.
// 11/13/2008: Lightly updated to support commmon usage, R. Nagy
// 11/24/2008: Re-arranging headers to better support G++, R. Nagy
// 05/25/2005: Added static LoadText(), R. Nagy
// 05/20/2005: Added SetNode() and SetParent(), R. Nagy
// 07/05/2005: Sped up the implementaiton of LoadText(). Converted to text file mode, too, R. Nagy
// 07/05/2005: Added LoadPureText(), R. Nagy
// 11/25/2006: Updated MkUniqueName() to use the default node folder instead of the pwd, R. Nagy
// 12/16/2006: Added MkUniqueName(sSuffix), R. Nagy
// 02/10/2009: Copied MkUniqueName(2) from FastJob, R. Nagy
//
#ifndef xzFile_Hpp
#define xzFile_Hpp

#include <ctime>             // time_t
#include <core/CommonObject.hpp>
#include <core/Directory.hpp>

using namespace std;

namespace stdnoj {
    class File;
    class Directory;
    class StdString;
    class ShareableObject;

    bool IsTextFile(const File&);
    bool IsTextFile(const StdString& sFileName);
    void MkLocalFileName(StdString& sFileNameCandidate, bool bSkipPathChar = false);

    class File : public ShareableObject {
    private:
        bool _bDestructorDeletes;

    public:

        enum TranslationModes {
            AT_TEXT,
            AT_BINARY,
            DEFAULT = AT_TEXT
        };

    protected:
        fstream *pIOStream;

        StdString sFQName;
        bool Qualify(StdString& sName);
        void _init(void);

        // Please use QueryPathTo, instead

        const char *PathTo(StdString& str) const {
            return QueryPathTo(str);
        }

        // Retired on  10/30/2003, R. Nagy
        const char *QueryPathTo(StdString& str) const; // ONLY the path to the file (no file name)
        const char *QueryFileNode(StdString& str) const; // ONLY the name of the file (no path is included)

    public:
        File(void);
        File(const StdString& stdName);
        File(const File&);
        ~File(void);

        // CommonObject
        bool FromString(const StdString&);
        const char *ToString(StdString&) const;

        bool IsNull(void) const; // true if default file name assigned, AND default file name does not exist. Otherwise false.

        bool Launch(void) const; // This opens or views a file, if possible.

        // Removes the `pw File` when class goes out-of-scopy (ONLY);

        void SetScopeDestruct(bool br) {
            _bDestructorDeletes = br;
        }

        bool QueryScopeDestruct(void) {
            return _bDestructorDeletes;
        }

        // The fully-qualified name. Use file pah to parse.
        bool Name(const StdString&);
        bool Name(Directory& dir, const StdString&);
        const char *Name(void) const;

        bool Rename(const StdString&);
        bool MkUniqueName(void); // Create a temporary file
        bool MkUniqueName(const StdString& sSuffix); // Create a temporary file
        bool MkUniqueName(const Directory& dir, const StdString& sSuffix); // Create a temporary file, with the prefixed 'file type'
        static File MkGenerationName(const Directory& dir, const File& fff);  // Make a unique, number-encrusted name :)

        const char *Query(Directory& dir);

        const char *QueryNode(StdString& str) const; // ONLY the name of the file (no path is included)
        const char *QueryParent(StdString& str) const; // ONLY the path to the file (no file name)
        bool QueryParent(Directory& dir) const; // Parent directory

        bool SetNode(StdString& str); // ONLY the name of the file (no path is included)
        bool SetParent(StdString& str); // ONLY the path to the file (no file name)
        bool SetParent(Directory& dir); // ONLY the path to the file (no file name)

        StdString QueryNode(void) const; // ONLY the name of the file (no path is included)
        StdString QueryParent(void) const; // ONLY the path to the file (no file name)

        time_t GetTime(void) const; // Returns the time, in nearest seconds, of the referenced file
        bool Touch(void);
        bool Touch(time_t);

        bool Create(void);
        bool Exists(void) const;
        bool CanRead(void) const; // A file may exist, but we not have access to it (read  locked, security, etc).
        bool CanWrite(void) const; // A file may exist, but we not have access to it (write locked, security, etc).

        bool CopyTo(File&);
        bool CopyTo(Directory&);
        bool CopyTo(const StdString&);
        bool Remove(void);
        bool Empty(void);

        bool Hide(void);
        bool UnHide(void);
        bool IsHidden(void);

        char Drive(void);
        int Device(void);

        istream& OpenRead(TranslationModes at = AT_TEXT);

        istream& OpenRead(const StdString& sfn, TranslationModes at = AT_TEXT) {
            Name(sfn);
            return OpenRead(at);
        }
        ostream& OpenWrite(TranslationModes at = AT_TEXT);

        ostream& OpenWrite(const StdString& sfn, TranslationModes at = AT_TEXT) {
            Name(sfn);
            return OpenWrite(at);
        }
        ostream& OpenAppend(TranslationModes at = AT_TEXT); // Only use this to append. Do not seek!
        iostream& OpenReadWrite(TranslationModes at = AT_TEXT); // Okay to seek using this, except has ios::end seeking problems under Microsoft VS??
        iostream& Resume(void);
        void Close(void);

        operator ostream&() {
            if (pIOStream) return *pIOStream;
            return OpenWrite();
        } // 04/03/2001

        File & operator=(const File&);

        int operator==(const File& file) {
            return (sFQName == file.sFQName);
        }
        ostream& write(ostream& os);
        istream& read(istream& is);

        bool DumpContents(ostream& os, TranslationModes at = AT_TEXT);
        bool DumpContents(long spStartAt, long spEndBefore, ostream& osDumpTo, TranslationModes at);

        static bool LoadText(const File& file, StdString& sResult); // DOC: Loads a file into a string.
        static bool LoadPureText(const File& file, StdString& sResult, char chStart = '<', char chEnd = '>'); // DOC: Loads a file into a string.



    };

    inline bool LoadTextFile(StdString& sResult, const File& file) {
        return File::LoadText(file, sResult);
    }

} // namespace stdnoj


#endif
