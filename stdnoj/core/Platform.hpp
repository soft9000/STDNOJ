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
// 06/17/2003: Implemented a plausable DirectoryEmpty() for all platforms. Only tested on WIN32, R. Nagy
//
#ifndef zzPlatform_Hpp
#define zzPlatform_Hpp

#include <stdnoj.hpp>

namespace stdnoj {
    class StdString;

    class Platform {
    public:
        static bool ShellExec(const StdString& sUrl, const StdString& str);
        static bool ShellExec(const StdString& sUrl);

        static bool GetPwd(StdString& str);
        static bool PushPwd(StdString& std);
        static bool PopPwd(StdString& std);

        static bool DirectoryChange(const StdString& sDir);
        static bool DirectoryExists(const StdString& sDir);
        static bool IsValidDirName(const StdString& std, bool bAllowDriveSpecifier = true);

        static bool FileRename(const StdString& sFrom, const StdString& sTo);
        static bool FileHide(const StdString& sFileName);
        static bool FileShow(const StdString& sFileName);
        static bool IsFileHidden(const StdString& sFileName);
        static bool Touch(const StdString& sFileName);

        static bool FileCopy(const StdString&, const StdString&);
        static bool FileReadable(const StdString&);
        static bool FileWritable(const StdString&);
        static bool FileExists(const StdString&);

        static bool DirectoryRemove(const StdString&);
        static bool DirectoryEmpty(const StdString&); // Remove all files from a folder (ONLY). True on success.
        static bool DirectoryCreate(const StdString&);
    };

} // stdnoj

#endif
