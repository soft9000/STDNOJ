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
// ****************************************************************************
// Class DFO: Data File Operations.
// --------------------------------
// Puropse is to model the operations that we usually want to perform on a 
// homogeneous collection of records. The goal is to easily allow the pattern 
// to be readily re-used between record types.
//
// PATTERN: This class reads and writes records in the Array<T> format with an 
// optional header record. Overload the Load(File) and Save(File) if that is not 
// what you need.
//
// 03/17/2002: Class created by retro-verifying and extracting operations in
// Phones application, Randall Nagy
// 06/10/2002: By adding the Tally() member and using it to pre-allocate, we
// reduced the load time of a 777 PHONES entry file from around 7 seconds to
// sub-second, Randall Nagy
// 06/11/2002: Eliminated #define: Split DFO into DFO_Array and DFO, Randall Nagy
// 07/06/2002: Extracted and publicized read() and write(), Randall Nagy
// 08/01/2002: Specified array operators =() and ==() for better support, Randall Nagy
// 09/02/2002: Added Swap(), Randall Nagy
// 09/06/2002: Fixed bug in Swap() and removed !is.eof(), as it was causing strange problems, Randall Nagy
// 05/01/2005: Added Query(array), Append(array), better File support, and re-arranged the model, Randall Nagy
// 05/16/2005: Added QSort(void), Randall Nagy
// 07/19/2005: Added read/write_heap functions - Legacy code uses both fast pre-allocated AND slower heap-rec-io strategies, Randall Nagy
// 07/23/2006: Now requiring File::TranslationModes mode on all Load and Save operations, Randall Nagy
// 08/20/2006: Added the AsArray parameter for better backwards-compatability (DFO_BLOCK_WRITE support), Randall Nagy
//             Fixed a few bugs in Tally(), Randall Nagy
// 09/09/2006: Fixed a few AsArray oversights in ... everything, Randall Nagy
//
#if 0

#ifndef DFO_Happ
#define DFO_Happ

#include "noj/DFOArray.hpp"

// DFO_BLOCK_WRITE - or pre-processor style serialization in Array<T> format - is
// no longer needed. Instead, specify AsArray = true ...

namespace stdnoj {
    using namespace std;

    class StdString;

    template <class T, class SZ = size_t>
    class DFO : public DFO_Array<T, SZ> {
    protected:
        File pwFile;

    public:

        DFO(void) {
        }

        virtual ~DFO(void) {
        } // old habits die hard ...

        StdString FileName(void) {
            return pwFile.Name();
        }

        bool FileName(const StdString& str) {
            return pwFile.Name(str);
        }

        bool FileName(const File& file) {
            return pwFile.Name(file);
        }

        bool Name(const File& file) {
            return pwFile.Name(file.Name());
        }

        bool Create(File& file, CompArray<T>& data, File::TranslationModes mode, bool AsArray = false);

        bool Load(File::TranslationModes mode, bool AsArray = false);
        bool Save(File::TranslationModes mode, bool AsArray = false);

        bool Load(const StdString& FilePath, File::TranslationModes mode, bool AsArray = false);
        bool Save(const StdString& FilePath, File::TranslationModes mode, bool AsArray = false);

        // Everything i/o gets down to this usage at some point -
        virtual bool Load(File& file, File::TranslationModes mode = File::AT_BINARY, bool AsArray = false);
        virtual bool Save(File& file, File::TranslationModes mode = File::AT_BINARY, bool AsArray = false);

        bool SaveAs(const StdString& FilePath, File::TranslationModes mode, bool AsArray = false);
        bool SaveAs(File& FilePath, File::TranslationModes mode, bool AsArray = false);

        // NPOS on ERROR, else number of objects in the file.
        size_t Tally(const StdString& sFileName, File::TranslationModes mode, bool AsArray = false);
    };

    template <class T, class SZ>
    bool DFO<T, SZ>::Create(File& file, CompArray<T>& data, File::TranslationModes mode, bool AsArray) {
        DFO_Array<T, SZ>::array = data;
        return Save(file, mode, AsArray);
    }

    template <class T, class SZ>
    bool DFO<T, SZ>::Load(File::TranslationModes mode, bool AsArray) {
        return Load(pwFile.Name(), mode, AsArray);
    }

    template <class T, class SZ>
    bool DFO<T, SZ>::Save(File::TranslationModes mode, bool AsArray) {
        return Save(pwFile, mode, AsArray);
    }

    template <class T, class SZ>
    bool DFO<T, SZ>::Load(const StdString& FilePath, File::TranslationModes mode, bool AsArray) {
        File file;
        if (file.Name(FilePath) == false)
            return false;
        return Load(file, mode, AsArray);
    }

    template <class T, class SZ>
    bool DFO<T, SZ>::Save(const StdString& FilePath, File::TranslationModes mode, bool AsArray) {
        File file;
        if (file.Name(FilePath) == false)
            return false;
        return Save(file, mode, AsArray);
    }

    template <class T, class SZ>
    bool DFO<T, SZ>::SaveAs(const StdString& FilePath, File::TranslationModes mode, bool AsArray) {
        File file;
        if (file.Name(FilePath) == false)
            return false;
        return Save(file, mode, AsArray);
    }

    template <class T, class SZ>
    bool DFO<T, SZ>::SaveAs(File& file, File::TranslationModes mode, bool AsArray) {
        return Save(file, mode, AsArray);
    }

    template <class T, class SZ>
    bool DFO<T, SZ>::Load(File& file, File::TranslationModes mode, bool AsArray) {
        if (pwFile.Name(file.Name()) == false)
            return false;
        if (AsArray) {
            bool br = false;
            istream& is = pwFile.OpenRead(mode);
            DFO_Array<T, SZ>::_load_header(is);
            DFO_Array<T, SZ>::array.read(is);
            if (is)
                br = true;
            pwFile.Close();
            return br;
        }
        return FileObjectHeap<T, SZ>::Load(array, pwFile, mode);
    }

    template <class T, class SZ>
    bool DFO<T, SZ>::Save(File& file, File::TranslationModes mode, bool AsArray) {
        if (pwFile.Name(file.Name()) == false)
            return false;
        if (AsArray) {
            bool br = false;
            ostream& os = pwFile.OpenWrite(mode);
            DFO_Array<T, SZ>::_save_header(os);
            DFO_Array<T, SZ>::array.write(os);
            if (os)
                br = true;
            pwFile.Close();
            return br;
        }
        return FileObjectHeap<T, SZ>::Save(array, pwFile, mode);
    }

    template <class T, class SZ>
    size_t DFO<T, SZ>::Tally(const StdString& FilePath, File::TranslationModes mode, bool AsArray) {
        File file;
        if (file.Name(FilePath) == false)
            return NPOS;
        size_t ss = NULL;
        istream& is = file.OpenRead(mode);
        if (!is)
            return NPOS;
        DFO_Array<T, SZ>::_load_header(is);
        if (!is)
            return NPOS;
        if (AsArray) {
            Array<T> array;
            array.read(is);
            ss = array.Nelem();
            file.Close();
            return ss;
        } else {
            while (is) {
                T rRec;
                if (_load(rRec, is) == true) {
                    if (is.fail())
                        return NPOS;
                    if (is)
                        ss++;
                }
            }
            file.Close();
            return ss;
        }
    }
} // stdnoj
#endif

#endif	// if 0
