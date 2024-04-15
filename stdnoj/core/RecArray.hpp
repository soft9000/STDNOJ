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
#ifndef RecArray_Hpp
#define RecArray_Hpp

// This class was originally written to create a index-file of `long`.
// It was generalized into a template, because it is easy to apply it to other
// fixed-length entities. -All that you need to provide is an apropriate
// remove() set for your object. RecArray is a fast indexing mechanism for things
// that can be read and written via `ifs.write((const char *)&T, sizeof(T))`.
//
// Note that your remove() can either delete the record && pack the
// file, or simply update() the same with a record marked as `deleted` (sp_npos,
// as in the default remove() (below));
// 
// Member functions don't cache anything, so you can code && reuse this with impunity.
//
// Class created 07/22/1997, by R. Nagy
//
// 12/06/2003: Updated to work with Visual Studio 6, R. Nagy
// 08/04/2008: Fixed the update to File usage after stdnoj port under VS.NET 2003, R. Nagy
//             Verified using _spi::test() routine.
// 01/05/2008: Updated seeking to use File logic, R. Nagy
// 08/25/2005: Added is_removed() and swap(). Removed commented-out FILE * logic, R. Nagy
// 08/26/2005: Replaced "const char *" with "const StdString&" in naming functions, R. Nagy
//
#include <cstdio>
#include <fstream>
#include <cassert>

namespace stdnoj {
    using namespace std; // <<imda>> Must be in the namespace block ...

    typedef long unsigned seek_t;
    const seek_t sp_npos = ((seek_t) - 1L);


    // THE RecArray CLASS WANTS AN OBJECT THAT RESPONDS TO THE SIZEOF() TO WRITE and READ.

    template <class T>
    class RecArray {
    protected:
        StdString iFile;

    public:
        RecArray(void);
        virtual ~RecArray(void);

        StdString name(void) {
            StdString str = iFile;
            return str;
        }

        void name(const StdString& psz) {
            iFile = psz;
        }

        // The sizeof-based number of elements in the file -
        virtual seek_t nelem(void);
        // Recall the CONTENT of the logical location -
        virtual bool read(seek_t logical, T& res);
        // Update the CONTENT of the logical locations - 
        virtual bool update(seek_t, T& sp);
        // Exchange the CONTENTS of the two logical locations - 
        virtual bool swap(seek_t sL, seek_t sR);
        // Append the data to the file
        virtual seek_t append(T& sp);

        // NOTE: WE ASSUME THAT THE SIZEOF(T) >= SIZEOF(seek_t) FOR DELETION PURPOSES!
        virtual bool remove(seek_t logical) = 0;
        virtual bool is_removed(seek_t which) = 0;
    };

    template <class T>
    RecArray<T>::RecArray(void) : iFile("default.idx") {
        // 08/04/2008 - Changed for backwards compatability 
        // - old (iostream.h) long was a long (4 bytes under wintel)
        // - new (iostream)   long is 24 bytes!!
        // assert(sizeof(T) >= sizeof(long)); 
        assert(sizeof (T) >= sizeof (seek_t));
    }

    template <class T>
    RecArray<T>::~RecArray(void) {
    }

    template <class T>
    seek_t RecArray<T>::nelem(void) {
        File file;
        if (file.Name(iFile) == false)
            return -1L;
        if (file.IsNull())
            return 0L;
        if (file.Exists() == false)
            file.Create();
        iostream &fs = file.OpenReadWrite(File::AT_BINARY);
        fs.seekp(0, ios::end); // prime the ftell pointer
        long tot = fs.tellp();
        if (tot == -1L)
            return 0L;
        file.Close();
        return ((seek_t) (tot / sizeof (T)));
    }

    template <class T>
    seek_t RecArray<T>::append(T& ref) {
        File file;
        if (file.Name(iFile) == false)
            return false;
        if (file.Exists() == false)
            file.Create();
        iostream &fs = file.OpenReadWrite(File::AT_BINARY);
        fs.seekp(0, ios::end); // prime the ftell pointer
        long tot = fs.tellp();
        if (tot == -1L)
            return 0L;
        fs.write((const char *) & ref, sizeof (T));
        file.Close();
        return ((seek_t) (tot / sizeof (T)));
    }

    template <class T>
    bool RecArray<T>::update(seek_t which, T& ref) {
        File file; // 12/06/2003: Converted to avoid Microsoft stream anomalies in VC 6, R. Nagy
        file.Name(iFile);
        iostream& fs = file.OpenReadWrite(File::AT_BINARY);
        if (!fs)
            return false;
        fs.seekp(which * sizeof (T)); // 12/06/2003: Changed seekg to seekp, R. Nagy
        if (fs.bad())
            return false;
        fs.write((const char *) & ref, sizeof (T));
        if (fs.bad())
            return false;
        return true;
    }

    template <class T>
    bool RecArray<T>::remove(seek_t which) {
        File file; // 12/06/2003: Converted to avoid Microsoft stream anomalies in VC 6, R. Nagy
        file.Name(iFile);
        iostream& fs = file.OpenReadWrite(File::AT_BINARY);
        if (!fs)
            return false;
        fs.seekp(which * sizeof (T)); // 12/06/2003: Changed seekg to seekp, R. Nagy
        if (fs.bad())
            return false;
        fs.write((const char *) & sp_npos, sizeof (T));
        if (fs.bad())
            return false;
        return true;
    }

    template <class T>
    bool RecArray<T>::is_removed(seek_t which) {
        T st;
        read(which, st);
        if (st == sp_npos)
            return true;
        return false;
    }

    template <class T>
    bool RecArray<T>::read(seek_t which, T& res) {
        File file; // 12/06/2003: Converted to avoid Microsoft stream anomalies in VC 6, R. Nagy
        file.Name(iFile);
        iostream& fs = file.OpenReadWrite(File::AT_BINARY);
        if (!fs)
            return false;
        fs.seekg(which * sizeof (T));
        if (fs.good()) {
            fs.read((char *) & res, sizeof (T));
            if (fs.good())
                return true;
        }
        return false;
    }

    template <class T>
    bool RecArray<T>::swap(seek_t sL, seek_t sR) {
        T tL, tR;
        if (read(sL, tL) == false)
            return false;
        if (read(sR, tR) == false)
            return false;
        if (update(sL, tR) == false)
            return false;
        if (update(sR, tL) == false)
            return false;
        return true;
    }


} // stdnoj
#endif
