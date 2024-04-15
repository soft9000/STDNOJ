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


#include <stdnoj.hpp>
#include <core/ProgramId.hpp>

#include <cstdlib>   // getenv()

#ifdef WIN32
// This is a special layer designed to encapsulate the WIN32 API.
// It is required to support our legacy on this platform
// (i.e. We did not have to call SetProgram() to query system defaults)
#include <core/win32/win32_ProgramId.cpp>
#endif


namespace stdnoj {

    bool ProgramId::SetProgram(const StdString &sFQFileName) {
        File file;
        if (file.Name(sFQFileName) == false)
            return false;
        if (file.Exists() == false)
            return false;
        return fProgramOverride.Name(sFQFileName);
    }

    bool ProgramId::PutInDirectory(const Directory& dir, File& file) const {
        StdString str, str1;
        str = dir.Name();
        str.append(PathSep());
        str.append(file.QueryNode(str1));
        return file.Name(str);
    }

    bool ProgramId::PutInDirectory(File& file) const {
        Directory dir;
        if (GetProgram(dir) == false)
            return false;
        StdString str = dir.Name();
        str += PathSep();
        str += file.QueryNode();
        return file.Name(str);
    }

    bool ProgramId::PutInDirectory(Directory& dirSub) const {
        Directory dir;
        if (GetProgram(dir) == false)
            return false;
        StdString str = dir.Name();
        str += PathSep();
        str += dirSub.QueryNode();
        return dirSub.Name(str);
    }

    bool ProgramId::PutInSubDirectory(const Directory& sSubdir, File& file) const {
        StdString sName, sPath;
        FilePath::Chop(file.Name(), sPath, sName, '\\');
        sPath = sSubdir.Name();
        sPath += '\\';
        sPath += sName;
        return file.Name(sPath);
    }

    StdString ProgramId::Concat(const StdString& sSubdir, const StdString& sItem, char sep) const {
        StdString sResult = sSubdir;
        sResult.append(sep);
        sResult.append(sItem);
        return sResult;
    }

    StdString ProgramId::Concat(const StdString& sSubdir, const StdString& sItem) const {
        return Concat(sSubdir, sItem, PathSep());
    }

    bool ProgramId::PutInTemp(File& file) const {
        Directory dir;
        if (GetTemp(dir) == false)
            return false;
        StdString str = dir.Name();
        str += PathSep();
        str += file.QueryNode();
        return file.Name(str);
    }

    bool ProgramId::PutInTemp(Directory& file) const {
        Directory dir;
        if (GetTemp(dir) == false)
            return false;
        StdString str = dir.Name();
        str += PathSep();
        str += file.QueryNode();
        return file.Name(str);
    }

    bool ProgramId::GetTemp(File& file) const {
        Directory dir;
        if (GetTemp(dir) == false)
            return false;
        StdString str = dir.Name();

        Directory dHold;
        dHold.Get(); // push

        // Create a temporary filename within a current working directory...
        dir.Set();
        StdString sTemp = ::tmpnam(NULL); // ANSI

        dHold.Set(); // pop

        str += PathSep();
        str += sTemp;
        return file.Name(str);
    }

    bool ProgramId::GetTemp(Directory& dir) const {
        StdString str = ::getenv("TEMP");
        // First we check the environment usual variable
        if (str.is_null() == false) {
            if (dir.Name(str) == true)
                return true;
        }
        // Next, any program override
        if (fProgramOverride.IsNull() == false) {
            StdString str = fProgramOverride.Name();
            size_t whence = str.rfind(PathSep());
            if (whence != npos) {
                str.remove(whence);
                if (dir.Name(str) == true) {
                    whence = str.find(PathSep()); // might already be "C:\" or "/" ...
                    if (whence != npos)
                        str.remove(whence); // if not, make it so
                    str += PathSep(); // and put it on the root of the default device ...
                    str += "TMP";
                    return dir.Name(str); // (should always work)
                }
            }
        }
#ifdef WIN32
        // Our WIN32 legacy is not required to call SetProgram - it used the WIN32 API ...
        return _pid32::GetTemp(dir);
#else   
        return false;
#endif
    }

    bool ProgramId::GetProgram(Directory& dir) const {
        if (fProgramOverride.IsNull() == false) {
            StdString str = fProgramOverride.Name();
            size_t whence = str.rfind(PathSep());
            if (whence != npos) {
                str.remove(whence);
                if (dir.Name(str) == true)
                    return dir.Exists();
            }

        }
#ifdef WIN32
        // Our WIN32 legacy is not required to call SetProgram - it used the WIN32 API ...
        return _pid32::GetProgram(dir);
#else   
        return false;
#endif
    }

    bool ProgramId::GetProgram(File& file) const {
        bool br = false;
        if (fProgramOverride.IsNull() == false)
            br = file.Name(fProgramOverride.Name());
#ifdef WIN32
        // Our WIN32 legacy is not required to call SetProgram - it used the WIN32 API ...
        br = _pid32::GetProgram(file);
#endif
        return br;
    }

    bool ProgramId::GetSystem(Directory& dir) const {
#ifdef WIN32
        // Our WIN32 legacy is not required to call SetProgram - it used the WIN32 API ...
        return _pid32::GetSystem(dir);
#else
        // This will usually work for both DOS and UNIX (a permission thing if it fails on POSIX) -
        StdString str = PathSep();
        str += "etc";
        return dir.Create();
#endif
    }

    // Place the file in the program's home (start-up) location

    bool ProgramId::Home(File& file) const {
        Directory dir;
        if (GetProgram(dir) == true)
            return dir.Home(file);
        return false;
    }

    // Place the file in the program's home (start-up) location

    bool ProgramId::Home(Directory& file) const {
        Directory dir;
        if (GetProgram(dir) == true)
            return dir.Home(file);
        return false;
    }

    // Place the file in the program's home (start-up) + sub dir location

    StdString ProgramId::Home(const StdString sSubDir, const StdString& sNode, char cSep) const {
        if (!cSep)
            cSep = PathSep();
        Directory dir;
        StdString sResult;
        if (GetProgram(dir) == true) {
            sResult = Concat(sSubDir, sNode, cSep);
            sResult = Concat(dir.Name(), sResult, cSep);
        }
        return sResult;
    }


}

namespace stdnoj {
    ProgramId pid;
}


