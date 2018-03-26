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

#ifdef MSDOS
#include <unistd.h> // unlink(DJGPP)
#endif

#include <sys/stat.h>
#include <core/Directory.hpp>


namespace stdnoj {
#define DEFAULT_FILE_NAME "default.file"

    void File::_init(void) {
        _bDestructorDeletes = false;
    }

    bool File::FromString(const StdString& str) {
        return Name(str);
    }

    const char *File::ToString(StdString& str) const {
        str = Name();
        return str.c_str();
    }

    File::File(void) : pIOStream(NULL) {
        _init();
        sFQName = DEFAULT_FILE_NAME;
        Qualify(sFQName);
    }

    File::File(const File& rf) : pIOStream(NULL) {
        _init();
        *this = rf;
    }

    File::File(const StdString& stdName) : pIOStream(NULL) {
        _init();
        Name(stdName);
    }

    File::~File(void) {
        if (pIOStream)
            Close();
        delete pIOStream;
        pIOStream = NULL;

        if (_bDestructorDeletes)
            Remove();
    }

    const char *File::QueryNode(StdString& str) const {
        FilePath fp;
        StdString sPath;
        fp.Chop(sFQName, sPath, str, fp.PathChar());
        return str.c_str();
    }

    StdString File::QueryNode(void) const {
        StdString str;
        QueryNode(str);
        return str;
    }

    const char *File::QueryParent(StdString& str) const {
        FilePath fp;
        StdString sNode;
        fp.Chop(sFQName, str, sNode, fp.PathChar());
        return str.c_str();
    }

    bool File::QueryParent(Directory& dir) const // Parent directory
    {
        if (dir.Name(QueryParent()))
            return dir.Exists();
        return false;
    }

    StdString File::QueryParent(void) const {
        StdString str;
        QueryParent(str);
        return str;
    }

    bool File::IsNull(void) const {
        if (sFQName.is_null())
            return true; // Not there is not there!
        if (Exists())
            return false; // Even the default name might exist, and => is not null.
        StdString str = sFQName;
        size_t whence = str.rfind('\\');
        if (whence != NPOS)
            str.remove_pos(0, whence + 1);
        if (str == DEFAULT_FILE_NAME)
            return true; // Un-assigned (what we are looking for)
        return false; // Even if the file does not exist, a prior name assignment means it is not null.
    }

    bool File::Launch(void) const {
        return Platform::ShellExec(Name());
    }

    bool File::Qualify(StdString& sName) {
#ifdef WIN32
        // Map any UNIX path separators to DOS;
        size_t sp = sName.find('/');
        while (sp != NPOS) {
            sName[sp] = '\\';
            sp = sName.find('/');
        }
        // Qualify it, if it needs it (default is pwd);
        if (sName.rfind('\\') == NPOS) {
            StdString sDir;
            if (Platform::GetPwd(sDir) == true) {
                sDir.append('\\');
                sDir.append(sName);
                sName = sDir;
            }
        }
        return Platform::IsValidDirName(sName);
#else
        return true;
#endif
    }

    bool File::Name(Directory& dir, const StdString& str) {
        StdString sTotal = dir.Name();
        FilePath fp;
        sTotal += fp.PathChar();
        sTotal += str;
        return Name(sTotal);
    }

    bool File::Name(const StdString& rStr) {
        // Cannot use FilePath::Open(rStr, *this) because files 
        // do not have to exist upon creation...
        StdString sTmp = rStr;
        if (Qualify(sTmp) == true) {
            Close();
            sFQName = sTmp;
            return true;
        }
        return false;
    }

    bool File::MkUniqueName(const StdString& sSuffix) {
        StdString str;
        File file;
        for (int ss = 0; ss < TMP_MAX; ss++) {
            file.MkUniqueName();
            str = file.Name();
            str.append(sSuffix);
            if (file.Name(str) == false)
                return false;
            if (file.Exists() == false) {
                *this = file;
                return true;
            }
        }
        return false;
    }

    bool File::MkUniqueName(void) {
        Directory dir;
        StdString sPwd = Query(dir);
        StdString str = tmpnam(NULL); // ANSI
        sPwd.append(PathChar());
        sPwd.append(str);
        return Name(sPwd);
    }

   bool File::MkUniqueName(const Directory& dir, const StdString& sSuffix)
      {
      StdString str;
      File file;
      for(int ss = 0; ss < TMP_MAX; ss++)
         {
         file.MkUniqueName(dir.Name());
         str = file.Name();
         str.append(sSuffix);
         if(file.Name(str) == false)
            return false;
         if(file.Exists() == false)
            {
            *this = file;
            return true;
            }
         }
      return false;
      }

    const char* File::Name(void) const {
        return sFQName.c_str();
    }

    const char* File::QueryFileNode(StdString& std) const {
        std = sFQName;
        size_t sp = sFQName.rfind('\\');
        if (sp != NPOS)
            std.remove_pos(0, sp + 1);
        return std.c_str();
    }

    const char* File::QueryPathTo(StdString& std) const {
        std = sFQName;
        size_t sp = sFQName.rfind('\\');
        if (sp != NPOS)
            std[sp] = 0;
        return std.c_str();
    }

    bool File::Rename(const StdString& std) {
        if (sFQName == std)
            return true;
        if (Platform::FileExists(sFQName) == true) {
            if (Platform::FileRename(sFQName.c_str(), std.c_str()) == false)
                return false;
        }
        return Name(std);
    }

    const char *File::Query(Directory& dir) {
        StdString str;
        QueryParent(str);
        dir.Name(str);
        return dir.Name();
    }

    bool File::CopyTo(File& file) {
        return CopyTo(file.Name());
    }

    bool File::CopyTo(Directory& dir) {
        StdString str;
        if (dir.Exists() == false)
            dir.Create();
        return CopyTo(dir.Home(QueryNode(str)));
    }

    bool File::CopyTo(const StdString& str) {
        if (sFQName.is_null())
            return false;
        Close(); // 11/09/2003, R. Nagy
        // This was a "file_fun" call;
        if (sFQName == str)
            return true;
        if (Platform::FileCopy(sFQName.c_str(), str.c_str()))
            return true;
        return false;
    }

    time_t File::GetTime(void) const {
        struct stat info;
        StdString bugfix = sFQName;
        if (::stat(bugfix.c_str(), &info) == -1)
            return 0L;
        return info.st_ctime;
    }

    bool File::Touch(void) {
        time_t timex = ::time(NULL);
        return Touch(timex);
    }

    bool File::Touch(time_t timex) {
        if (sFQName.is_null())
            return false;
        return false;
    }

    bool File::Create(void) {
        ofstream ofs(sFQName.c_str());
        if (ofs)
            return true;
        return false;
    }

    bool File::Hide(void) {
        return Platform::FileHide(sFQName);
    }

    bool File::UnHide(void) {
        return Platform::FileShow(sFQName);
    }

    bool File::IsHidden(void) {
        if (sFQName.is_null())
            return false;
        return Platform::IsFileHidden(sFQName);
    }

    bool File::Exists(void) const {
        if (sFQName.is_null())
            return false;
        return Platform::FileExists(sFQName.c_str());
    }

    bool File::CanRead(void) const {
        if (sFQName.is_null())
            return false;
        return Platform::FileReadable(sFQName.c_str());
    }

    bool File::CanWrite(void) const {
        if (sFQName.is_null())
            return false;
        return Platform::FileWritable(sFQName.c_str());
    }

    bool File::Remove(void) {
        Close();
        if (sFQName.is_null())
            return false;
        if (::unlink(sFQName.c_str()))
            return false;
        return true;
    }

    bool File::Empty(void) {
        return Create();
    }

    bool
    File::DumpContents(ostream& os, TranslationModes at) {
        istream& is = OpenRead(at);
        if (!is)
            return false;

        char ch;
        while (is) {
            is.read((char *) & ch, 1);
            if (is)
                os.write((char *) & ch, 1);
        }
        Close();

        return true;
    }

    bool
    File::DumpContents(long spStart, long spEnd, ostream& os, TranslationModes at) {
        istream& is = OpenRead(at);
        if (!is)
            return false;

        is.seekg(spStart);
        char ch;
        while (is) {
            if ((spEnd != (long) - 1L) && (is.tellg() >= spEnd))
                break;

            is.read((char *) & ch, 1);
            if (is)
                os.write((char *) & ch, 1);
        }
        Close();
        return true;
    }

    char File::Drive(void) {
        if (sFQName.is_null())
            return '?';
        if (sFQName[1] == ':') {
            StdString std = sFQName[0];
            std.to_upper();
            return std[0];
        }
        return '?';
    }

    int File::Device(void) {
        char ch = Drive();
        if (ch == '?')
            return -1;
        return (ch - 'A');
    }

    File& File::operator=(const File& rf) {
        if (this == &rf)
            return *this;
        sFQName = rf.sFQName;
        return *this;
    }

    ostream& File::write(ostream& os) {
        sFQName.write_stream(os);
        return os;
    }

    istream& File::read(istream& is) {
        sFQName.read_stream(is);
        return is;
    }

    istream& File::OpenRead(TranslationModes at) {
        Close();
        pIOStream = new fstream;

        if (at == AT_BINARY)
            pIOStream->open(sFQName.c_str(), ios::in | ios::binary);
        else
            pIOStream->open(sFQName.c_str(), ios::in);
        return *pIOStream;
    }

    ostream& File::OpenWrite(TranslationModes at) {
        Close();
        Remove(); // For the benefit of Microsoft's legacy.
        pIOStream = NULL;
        pIOStream = new fstream;

        if (at == AT_BINARY)
            pIOStream->open(sFQName.c_str(), ios::out | ios::binary);
        else
            pIOStream->open(sFQName.c_str(), ios::out);
        return *pIOStream;
    }

    void File::Close(void) {
        if (pIOStream)
            pIOStream->close();
    }

    ostream& File::OpenAppend(TranslationModes at) {
        Close();
        delete pIOStream;
        pIOStream = NULL;
        pIOStream = new fstream;

        if (at == AT_BINARY)
            pIOStream->open(sFQName.c_str(), ios::out | ios::app | ios::binary);
        else
            pIOStream->open(sFQName.c_str(), ios::out | ios::app);
        return *pIOStream;
    }

    iostream& File::OpenReadWrite(TranslationModes at) {
        Close();
        delete pIOStream;
        pIOStream = NULL;
        pIOStream = new fstream;

        if (at == AT_BINARY)
            pIOStream->open(sFQName.c_str(), ios::in | ios::out | ios::binary);
        else
            pIOStream->open(sFQName.c_str(), ios::in | ios::out);
        return *pIOStream;
    }

    iostream& File::Resume(void) {
        if (!pIOStream)
            pIOStream = new fstream;
        return *pIOStream;
    }

    bool File::LoadText(const File& fff, StdString& sResult) {
        File file = fff;
        bool br = false;
        StdString sLine;
        sResult = sLine; // clear it out ...
        istream& is = file.OpenRead();
        while (sLine.read_line(is)) {
            sResult += sLine;
            sResult += '\n';
            br = true;
        }
        return br;
    }

    bool File::LoadPureText(const File& fff, StdString& sResult, char chStart, char chEnd) {
        File file = fff;
        bool br = false;
        StdString sLine;
        sResult = sLine; // clear it out ...
        istream& is = file.OpenRead();
        while (sLine.read_line(is)) {
            sResult += tag_remove(sLine, chStart, chEnd);
            sResult += '\n';
            br = true;
        }
        return br;
    }


#ifdef FileInformation_Hpp

    bool LoadTextFile(StdString& sResult, File& file) {
        FileInformation info;
        if (file.Get(info) == false)
            return false;
        if (info.IsFileHuge())
            return false;
        sResult.read(file.OpenRead(File::AT_TEXT), info.FileSize());
        if (sResult.length())
            return true;
        return false;
    }

    bool LoadBytes(File& file, ByteBuffer& bytes) {
        FileInformation info;
        if (file.Get(info) == false)
            return false;
        if (info.IsFileHuge())
            return false;
        ifstream ifs(file.Name());
        char *pBuf = new char[info.FileSize() + 1];
        ifs.read(pBuf, info.FileSize());
        bytes.assign(pBuf, info.FileSize());
        return true;
    }
#endif

    bool IsTextFile(const File& file) {
        File f = file;
        istream& is = f.OpenRead(File::AT_BINARY);
        char ich = 0;
        while (is) {
            is.read(&ich, 1); // Let the OS buffer it...
            if ((ich >= ' ') && ich <= '~') // 0x20 to 0x7e
                continue;

            if ((ich >= 0x09) && ich <= 0x0d) // ASCII control chars
                continue;

            if (ich == 0x1a) // Necessary evil on DOS..
                continue;

            return false;
        }
        f.Close();
        return true;
    }

    void MkLocalFileName(StdString& std, bool bSkipPathChar) {
        for (size_t ss = 0L; ss < std.length(); ss++) {
            switch (std[ss]) {
                case '\\':
                    if (bSkipPathChar == true)
                        continue;
                case ':':
                    if ((bSkipPathChar == true) && ss == 1)
                        continue;
                case '<':
                case '>':
                case '*':
                case '?':
                case '/':
                case '\"':
                    std[ss] = '_';
                default:
                    continue;
            }
        }
    }

    bool File::SetNode(StdString& str) {
        bool br = false;
        Directory dir;
        if (dir.Name(QueryParent()) == true) {
            if (Name(str))
                br = dir.Home(*this);
        }
        return br;
    }

    bool File::SetParent(StdString& sDir) {
        StdString str = QueryNode();
        return Name(FilePath::Glue(sDir, str, PathChar()));
    }

    bool File::SetParent(Directory& dir) {
        StdString str;
        str = dir.Name();
        return SetParent(str);
    }


   File File::MkGenerationName(const Directory& dir, const File& fff)
      {
      File file = fff;
      if(dir.Home(file) == false)
         {} // =:-0

      unsigned long lCount = 0L;
      while(file.Exists())
         {
         lCount++;
         if(!lCount) // wrap - not likley - but all the same - could happen - some day -
            {
            file.MkUniqueName(dir.Name()); // Failsafe
            return file;
            }
         stringstream srm;
         srm << lCount << "." << fff.QueryNode();
         file.Name(srm.str().c_str());
         }
      return file;
      }

} // stdnoj
