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
#include <core/Directory.hpp>
#include <core/FilePath.hpp>

namespace stdnoj {

    Directory::Directory(void) {
        //Get();    No! We need a NULL directory ability! (5/27/2002)
    }

    Directory::Directory(const Directory& rDir) {
        *this = rDir;
    }

    Directory::~Directory(void) {
    }

    bool Directory::IsEmpty(void) {
        {
            Array<File> af;
            Query(af);
            if (af.Nelem())
                return false;
        }
        Array<Directory> ad;
        Query(ad);
        if (ad.Nelem())
            return false;
        return true;
    }

    bool Directory::FromString(const StdString& str) {
        return Name(str);
    }

    const char *Directory::ToString(StdString& str) const {
        str = Name();
        return str.c_str();
    }

    bool Directory::Home(File& file) const {
        FilePath fp;
        StdString sPath, sNode;
        fp.Chop(file.Name(), sPath, sNode, fp.PathChar());
        sPath = fp.Glue(Name(), sNode, fp.PathChar());
        return file.Name(sPath);
    }

    bool Directory::Home(Directory& dir) const {
        FilePath fp;
        StdString sPath, sNode;
        fp.Chop(dir.Name(), sPath, sNode, fp.PathChar());
        sPath = fp.Glue(Name(), sNode, fp.PathChar());
        return dir.Name(sPath);
    }

    StdString Directory::Home(const StdString& sNode) const {
        FilePath fp;
        return fp.Glue(Name(), sNode, fp.PathChar());
    }

    bool Directory::Exists(void) const {
        return Platform::DirectoryExists(sPwd);
    }

    bool Directory::Name(const StdString& sName) {
        // Cannot use FilePath::Open(rStr, *this) because folders 
        // do not have to exist upon creation...
        if (Platform::IsValidDirName(sName) == true) {
            FilePath fp;
            char ch = fp.PathChar();
            size_t whence = sName.find(ch);
            if (whence != npos) // 06/17/2008, Randall Nagy
            {
#ifndef UNIX
                if (whence == 2 && sName[1] == ':')
#else
                if (whence == 0)
#endif
                {
                    // Assign the absolute path name & return
                    sPwd = sName;
                    return true;
                }
                // It is NOT an absolute path name - we need to prefix it...            
            }
            // Prefix it with the current default directory
            Get();
            sPwd.append(ch);
            sPwd.append(sName);
            return true;
        }
        return false;
    }

    const char *Directory::Name(void) const {
        return sPwd.c_str();
    }

    const char *Directory::QueryNode(StdString& str) const {
        FilePath fp;
        StdString sPath;
        fp.Chop(sPwd, sPath, str, fp.PathChar());
        return str.c_str();
    }

    StdString Directory::QueryNode(void) const {
        StdString str;
        QueryNode(str);
        return str;
    }

    const char *Directory::QueryParent(StdString& str) const {
        FilePath fp;
        StdString sNode;
        fp.Chop(sPwd, str, sNode, fp.PathChar());
        return str.c_str();
    }

    bool Directory::QueryParent(Directory& dir) const // Parent directory
    {
        if (dir.Name(QueryParent()))
            return dir.Exists();
        return false;
    }

    StdString Directory::QueryParent(void) const {
        StdString str;
        QueryParent(str);
        return str;
    }

    bool Directory::Create(void) {
        if (Platform::DirectoryCreate(sPwd))
            return true;
        return false;
    }

    bool Directory::Remove(void) {
        if (Platform::DirectoryRemove(sPwd.c_str()))
            return true;
        return false;
    }

    bool Directory::Empty(void) {
        return Platform::DirectoryEmpty(sPwd);
    }

    bool Directory::Get(void) {
        return Platform::GetPwd(sPwd);
    }

    bool Directory::Set(void) {
        return Platform::DirectoryChange(sPwd);
    }

    bool Directory::Set(const StdString& sDir) {
        if (Name(sDir) == true) {
            StdString sHold = sPwd;
            sPwd = sDir;
            if (Set() == false) {
                sPwd = sHold;
                return false;
            }
            return true;
        }
        return false;
    }

    bool Directory::Push(void) {
        return Platform::PushPwd(sStack);
    }

    bool Directory::Pop(void) {
        if (Platform::PopPwd(sStack) == true) {
            Get();
            return true;
        }
        return false;
    }

    void Directory::EmptyStack(void) {
        sStack = "";
    }

    bool Directory::CreateDirectoryTree(void) {
        return Create();
    }

    Directory& Directory::operator=(const Directory& rDir) {
        if (this == &rDir)
            return *this;
        sStack = rDir.sStack;
        sPwd = rDir.sPwd;
        return *this;
    }

    ostream& Directory::write(ostream& os) {
        sStack.write_stream(os);
        sPwd.write_stream(os);
        return os;
    }

    istream& Directory::read(istream& is) {
        sStack.read_stream(is);
        sPwd.read_stream(is);
        return is;
    }

    bool Directory::CreateTreeFor(File& file) {
        if (file.Exists())
            return true;

        StdString sHold;
        sHold = sPwd;
        file.QueryParent(sPwd);

        bool br = Create();
        sPwd = sHold;
        return br;
    }

    //#include <File_Fun.hpp>

    bool Directory::GoTo(void) {
        return Platform::DirectoryChange(sPwd.c_str());
    }

    void Directory::Query(Array<File>& ary) const {
        FileSystem::Query(sPwd, ary);
    }

    void Directory::Query(Array<Directory>& ary) const {
        FileSystem::Query(sPwd, ary);
    }

    bool Directory::Launch(void) {
        File file;
        if (file.Name(Name()) == true)
            return file.Launch();
        return false;
    }

    void Directory::QueryTree(Array<Directory>& aFileNames) const {
        Array<Directory> aDirs;
        Query(aDirs);

        for (size_t ssD = 0L; ssD < aDirs.Nelem(); ssD++)
            aDirs[ssD].QueryTree(aFileNames);

        aFileNames.Append(aDirs);
    }

    void Directory::QueryTree(Array<File>& aFileNames) const {
        Array<Directory> aDirs;
        Query(aDirs);

        for (size_t ssD = 0L; ssD < aDirs.Nelem(); ssD++)
            aDirs[ssD].QueryTree(aFileNames);

        Array<File> files;
        Query(files);
        aFileNames.Append(files);
    }


} // stdnoj
