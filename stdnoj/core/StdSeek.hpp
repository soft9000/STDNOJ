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
#ifndef StdSeek_Hpp
#define StdSeek_Hpp

// The StdSeek class is built for raw speed. It provides un-obstrusive (index is
// stored in a seperate file) indexing for a file containing variant size objects
// (read()/write() is assumed to be variant sizes: doesn't have to be, but is
// fully supported). Originally developed to support RawQuote and UserQuote
// collections of the "Quote" (FORTUNE) project.
//
// The updating of records is handled with index replacement: new data is appended,
// deleted space is lost in the data file, and reclaimed via packing. Why? Because
// typical logic mandates either the "deletion" of the record (via sp_npos
// over-write in index-file), replacement of object with <= sized object, or
// an entire file compaction and expansion. -Just what is best depends upon the
// nature of the data being stored, so, we'll leave further delete() and update()
// possibilities to our descendants. Note, however, that RecArray<T> has lots of
// support for these types of operations, so we promoted the definition of the
// same to the protected realm: be carefull how you implement sp_npos recovery
// (explination follows.)
//
// As presently implemented sp_npos, when returned from indexFile, means that
// a file error has occured. Depending how your descendant class implements update()
// or delete() however, that meaning might change. Once again, make sure your
// utilization of sp_npos is well planned if you choose to imbed the same into
// the index-file as an indication of a "deleted" status. -The index-file will
// simply give you gigo, so be sure you can differientiate between file-error
// and object-deleted contexts when the index-file op returns (hint: use nelem()).
//
// If the nature of your data is such that you realy need to care, use DataSet<T>,
// or FastFlatFile<T>: Those are more heavy-weight database classes. They provide 
// for a MUCH more accountable database implementation!
//
// 07/22/1997: Class Created, Randall Nagy
// 11/05/1997: Added AppendDataOnly(), Randall Nagy
// 03/19/1998: Realized that StdSeek<> can successfully track and append data to 
// ANYthing (an icon, for example): Resolved that the implementing class has the 
// requirement to control the desirability of this type of behavior: For example,
// you might choose to append your data onto a FileVersion object (etc). Randall Nagy
//
// 06/17/1999: Promoted a variant of Replace(QuoteDatabaseV1), then felt obliged 
//    to add CopyTo(), Rename(), and Pack(), Randall Nagy
// 10/28/1999: Externalized the "class B", so we can over-ride it for items like 
//    UserQuoteList, which needs a different type of index-file, Randall Nagy
// 11/26/2003: Updated to file object usage to avoid Microsoft VC6 streams 
//    usage problems, Randall Nagy
// 11/30/2003: Fix for the Microsoft tellp streams bug (VC 6), Randall Nagy
// 08/03/2008: Created spi::test and re-tested same under VS.NET 2003, Randall Nagy
// 08/04/2008: Changed long template parameter default to seek_t for backwards 
// compatability, Randall Nagy
// - old (iostream.h) long was a long (4 bytes under wintel)
// - new (iostream)   long is 24 bytes!!
// 01/05/2008: Updated seeking to use stdonj::File, Randall Nagy
// 08/26/2005: Modernized and simplified the public interface, Randall Nagy
// 07/16/2006: Re-introduced QueryFileName, Randall Nagy
//

#include <stdnoj.hpp>
#include <core/RecArray.hpp>

#ifdef UNIX
#include <unistd.h>  // unlink
#else
#include <io.h>      // unlink
#endif

#define BEGIN {
#define END   }

namespace stdnoj {
    using namespace std;

    template <class T>
    class _spi : public RecArray<T> {
    public:
        // Confirmed - The default implementaiton writes sp_npos at the item's index entry

        virtual bool remove(seek_t ss) {
            return RecArray<T>::remove(ss);
        }

        virtual bool is_removed(seek_t ss) {
            return RecArray<T>::is_removed(ss);
        }
    };

    class StreamposIndex : public _spi<seek_t> {
    };

    template <class T, class B = seek_t >
    class StdSeek {
    private:
        StdString sName;

    protected:
        _spi<B> indexFile;

        // Obsolete - sorry -
        //void        FileName(const StdString& str)     {sName.assign(psz);}
        //const char *FileName(void)          		     {return sName.c_str();}

        // Normally appending a record mandates a corresponding index entry. There are times,
        // however, when you will want to put data into a file without indexing it. Examples
        // include "out of line" objects, index-updated-and-data-is-appended, and
        // index-removed-and-data-is-appended operations.
        virtual bool AppendDataOnly(const T& ref, B& spResult);

    public:

        static StdString Suffix(void) {
            StdString str = ".std";
            return str;
        }

        StdSeek(void) {
            Name(StdString("default") + Suffix());
        }

        virtual ~StdSeek(void) {
        }

        virtual bool Rename(const StdString& str); // Renames last "Name()", Then does a "use"

        bool Name(const File& file) {
            return Name(file.Name());
        }
        virtual bool Name(const StdString& str); // Works like "use"

        StdString Name(void) {
            StdString str = sName;
            return str;
        }

        const char * QueryFileName(StdString& str) {
            str = sName;
            return str.c_str();
        }

        bool Set(File& file) {
            return Name(file.Name());
        }

        bool Query(File& file) {
            return file.Name(Name());
        }

        void IndexName(const StdString& str);
        StdString IndexName(void);

        bool SetIndex(File& file) {
            IndexName(file.Name());
            return true;
        }

        bool QueryIndex(File& file) {
            return file.Name(IndexName());
        }

        virtual bool Create(void); // Create data-file. Chance to put-in any header (etc) before we start adding data.
        virtual bool Remove(void); // Delete data-file and index.

        virtual bool Pack(void); // Removed dead-space created by "Replace()".
        virtual bool CopyTo(const char *psz); // Copies file to file name, removing dead-space.

        seek_t Nelem(void);
        StdSeek<T, B> & operator=(const StdSeek<T, B>&);

        virtual bool Recall(seek_t ss, seek_t& ref); // NOTE: Indexing is ZERO based.

        // The handling of updating and deleted records is beyond the scope of this
        // class. Typical logic mandates either the "deletion" of the record via sp_npos
        // over-write in index-file, replacement of object with <= sized object, or
        // an entire file compaction and expansion. -Just what is best depends upon the
        // nature of the data being stored, so, we'll leave the decision another implementation;
        virtual bool Recall(seek_t ss, T& ref); // NOTE: Indexing is ZERO based.
        virtual bool Append(const T& ref);

        // A "replace" of an object preserves it's index position. Note, however, that the
        // object's PHYSICAL position in the data-file is always at the end. The net 
        // result of this operation can be a gradual slow-down of disk operations:
        // File-packing should occur at some point to keep files lean and responsive.
        virtual bool Replace(seek_t ss, T& uq);

        // Exchange the order in which the two objects occur in the physical index.
        virtual bool SwapObjectIndex(seek_t ss1, seek_t ss2);

        // This "deletes" an item by removing it's entry in the index-file. Pack() will
        // reclaim the object's physical space -
        virtual bool ReleaseObjectIndex(seek_t _lastSelected);
        // See if ReleaseObjectIndex has been called - (i.e. see if index entry has been set to sp_npos.)
        virtual bool IsObjectIndexReleased(seek_t _lastSelected);

        // Using sp_npos in the index is OUR deletion criteria. You might also have another?

        virtual bool IsRecordDeleted(seek_t rT) {
            return IsObjectIndexReleased(rT);
        }
    };

    template <class T, class B>
    bool StdSeek<T, B>::Name(const StdString& psz) {
        StdString sr2 = psz;
        sName = psz;
        size_t ss = sr2.rfind('.');
        if ((ss < sName.length()) && ss != 0L)
            sr2[ss] = NULL;
        sr2.append(".idx");
        IndexName(sr2.c_str());
        return true;
    }

    template <class T, class B>
    bool StdSeek<T, B>::Rename(const StdString& psz) {
        StdSeek<T, B> newname;
        if (newname.Name(psz) == false)
            return false;

        File file;
        if (file.Name(IndexName()) == false)
            return false;
        if (file.Rename(newname.IndexName()) == false)
            return false;

        if (file.Name(Name()) == false)
            return false;
        if (file.Rename(newname.Name()) == false)
            return false;

        return Name(psz);
    }

    template <class T, class B>
    StdSeek<T, B>& StdSeek<T, B>::operator=(const StdSeek<T, B>& ss) {
        if (this == &ss)
            return *this;
        Rename(ss.Name()); // used to be FileName()
        IndexName(ss.IndexName());
        return *this;
    }

    template <class T, class B>
    bool StdSeek<T, B>::Create(void) {
        // 11/26/2003: Implementation used to simply return true. 
        // Invlaid with Microsoft stream implementaiton on VC6.0, Randall Nagy
        if (Remove() == false)
            return false;
        File file;
        file.Name(IndexName());
        file.Create();
        if (file.Exists() == false)
            return false;
        file.Name(Name());
        file.Create();
        if (file.Exists() == false)
            return false;
        return true;
    }

    template <class T, class B>
    bool StdSeek<T, B>::Remove(void) {
        /*
                int r1 = ::unlink(IndexName());
           int r2 = ::unlink(FileName());
           return ((r1 == r2 == 0) ? true : false);
         */
        bool br1, br2;
        File file;
        file.Name(IndexName());
        if (file.Exists())
            br1 = file.Remove();
        else
            br1 = true;

        file.Name(Name());
        if (file.Exists())
            br2 = file.Remove();
        else
            br2 = true;

        if (br1 == false)
            return false;
        if (br2 == false)
            return false;

        return true;
    }

    template <class T, class B>
    bool StdSeek<T, B>::Recall(seek_t ss, seek_t& ref) {
        return indexFile.read(ss, ref);
    }

    template <class T, class B>
    bool StdSeek<T, B>::Recall(seek_t sssssss, T& ref) {
        // First, we get the index relative locaiton of the record to read
        B sp;
        bool br = indexFile.read(sssssss, sp);
        // Either an index read failure, or the index entry has been marked as deleted;
        if (sp == sp_npos)
            return false;
        if (br == true) {
            // Now we read the actual record
            File file;
            file.Name(sName.c_str());
            istream& fs = file.OpenRead(File::AT_BINARY);
            if (!fs)
                return false;
            fs.seekg(sp);
            if (!fs) // 12/06/2003: Changed from .bad testing to any failure, Randall Nagy
                return false;
            ref.read(fs);
            if (fs) // 12/06/2003: Changed from .eof testing to any failure, Randall Nagy
                return true;
        }
        return false;

    }

    template <class T, class B>
    bool StdSeek<T, B>::Append(const T& ref) {
        B sp;
        if (AppendDataOnly(ref, sp) == true) {
            if (indexFile.append(sp) == sp_npos) // 11/26/2003, Randall Nagy
                return false;
            return true;
        }
        return false;
    }

    template <class T, class B>
    bool StdSeek<T, B>::AppendDataOnly(const T& ref, B& sp) {
        File file;
        if (file.Name(Name()) == false)
            return false;
        if (file.Exists() == false)
            file.Create();
        iostream &fs = file.OpenReadWrite(File::AT_BINARY);
        fs.seekp(0, ios::end); // prime the ftell pointer
        sp = fs.tellp();
        if (sp == -1L)
            return false;
        ((T&) ref).write(fs);
        file.Close();
        return true;
        /*
        // Just in case you need to fall back to a acceptable legacy usage:
        // Here is a cross-vendor implementaiton using the Standard C Library.
           sp = -1L;
           FILE *fp = ::fopen(FileName(), "r+b");
           if(!fp)
              fp = ::fopen(FileName(), "w+b");
           if(!fp)
              return false;
           if(::fseek(fp, 0, SEEK_END) != 0) // be sure to give ftell it's value ...
              return false;
           sp = ::ftell(fp);                 // ... and be sure to get it now.
           if(sp == -1L)
              return false;
           stringstream srm;             // This is a kluge - required because of modern streams anomolies between vendors
           ((T&)ref).write(srm);
           StdString str = PRESTO(srm);
           if(str.is_null())
              {
              // nothing to write... sp is accurate... just let them have it!
              ::fclose(fp);
              return true;
              }
           if(::fwrite((const void *)str.c_str(), str.length(), 1, fp) != 1)
              {
              ::fclose(fp);
              return false;
              }
           ::fflush(fp);  // of debatable value in this context ...
           ::fclose(fp);
                return true;
         */
    }

    template <class T, class B>
    bool StdSeek<T, B>::Replace(seek_t ss, T& uq) {
        // Validate that there is an object at ss;
        B sp;
        if (indexFile.read(ss, sp) == false)
            return false;

        bool br = AppendDataOnly(uq, sp);
        if (br == true)
            return indexFile.update(ss, sp);
        return br;
    }

    template <class T, class B>
    bool StdSeek<T, B>::CopyTo(const char *pszFileName) {
        if (sName == pszFileName)
            return false;

        // INGORING ANY HEADERS is okay with the original (it's our modus operandi);
        StdSeek<T, B> originalCopy;
        originalCopy.Name(Name());

        // Now, we use THIS to re-write the NEW copy;
        if (Name(pszFileName) == false)
            return false;
        if (Remove() == false) // jic it already exists...
            return false;
        if (Create() == false) // make sure it's started out 'fresh' (create header, etc)
            return false;

        T rT;
        for (seek_t ss = NULL; ss < originalCopy.Nelem(); ss++) {
            if (IsRecordDeleted(ss) == true)
                continue;
            if (originalCopy.Recall(ss, rT) == true)
                Append(rT);
        }

        return Name(originalCopy.Name());
    }

    template <class T, class B>
    bool StdSeek<T, B>::Pack(void) {
        StdString sOriginalName = sName;

        // Create the name of the NEXT NEW (holding) copy;
        StdString sNewNext = sName;
        sNewNext.append(".pak.STD");

        // Next, we save-out a copy of THIS file to the HOLDING (NEXT NEW) location;
        if (CopyTo(sNewNext.c_str()) == false)
            return false;

        // Now, we rename THIS file, for a BACKUP;
        StdString sBackup = sName;
        sBackup.append(".bak.STD");
        BEGIN
        // Remove any old backup files, if they already exist;
        StdSeek<T, B> backupCopy;
        if (backupCopy.Name(sBackup) == false)
            return false;
        backupCopy.Remove();
        END
        if (Rename(sBackup.c_str()) == false)
            return false;

        // Finaly, we rename the HOLDING copy AS the original name;
        Name(sNewNext.c_str());
        return (Rename(sOriginalName.c_str()));
    }

    template <class T, class B>
    void StdSeek<T, B>::IndexName(const StdString& str) {
        indexFile.name(str);
    }

    template <class T, class B>
    StdString StdSeek<T, B>::IndexName(void) {
        return indexFile.name();
    }

    template <class T, class B>
    seek_t StdSeek<T, B>::Nelem(void) {
        return indexFile.nelem();
    }

    template <class T, class B>
    bool StdSeek<T, B>::ReleaseObjectIndex(seek_t _lastSelected) {
        return indexFile.remove(_lastSelected);
    }

    template <class T, class B>
    bool StdSeek<T, B>::IsObjectIndexReleased(seek_t _lastSelected) {
        return indexFile.is_removed(_lastSelected);
    }

    template <class T, class B>
    bool StdSeek<T, B>::SwapObjectIndex(seek_t ssL, seek_t ssR) {
        return indexFile.swap(ssL, ssR);
    }

} // stdnoj

#endif
