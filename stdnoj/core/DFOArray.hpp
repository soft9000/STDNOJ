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
// 08/20/2006: Relocated DFOArray from DFO.HPP to DFOArray.HPP, Randall Nagy
//
#ifndef DFO_Array_Happ
#define DFO_Array_Happ

#include "core/StdString.hpp"
#include "core/CompArray.hpp"
#include "core/File.hpp"

namespace stdnoj {
    using namespace std;

    class StdString;

    template <class T, class SZ = size_t> class DFO_Array {
    protected:
        CompArray<T, SZ> array;

        // 04/01/2005: Acestrialized & retired, Randall Nagy
        //    virtual bool _load(T& rRec, istream& is);
        //    virtual bool _save(T& rRec, ostream& os);

        virtual void _load_header(istream& is) {
        }

        virtual void _save_header(ostream& os) {
        }

    public:

        DFO_Array(void) {
        }

        virtual ~DFO_Array(void) {
        }

        void New(void) {
            array.Empty();
        }

        void AddZombies(size_t ss) {
            array.AddZombies(ss);
        }

        void Delete(size_t ss) {
            array.Remove(ss);
        }

        void Empty(void) {
            array.Empty();
        }

        istream& read_heap(istream& is);
        ostream& write_heap(ostream& os);

        istream& read(istream& is);
        ostream& write(ostream& os);

        size_t Nelem(void) const {
            return array.Nelem();
        }

        T & operator[](size_t ss) {
            return array[ss];
        }

                /*
        // Borland 5.x no likee these;
              int      operator==(const DFO_Array<T>&);
              DFO_Array<T>
                       operator=(const DFO_Array<T>&);
              int      operator==(const CompArray<T>&);
              DFO_Array<T>
                       operator=(const CompArray<T>&);
         */
        bool Append(T& rec) {
            array[array.Nelem()] = rec;
            return true;
        }

        bool Update(T& rec, size_t ss) {
            array[ss] = rec;
            return true;
        }

        void Use(CompArray<T>& ary) {
            array = ary;
        }

        void Query(CompArray<T>& ary) {
            ary = array;
        }

        void Append(CompArray<T>& ary) {
            for (size_t ss = 0L; ss < ary.Nelem(); ss++) Append(ary[ss]);
        }

        void Sort(MemberComp<T>& cmp) {
            array.Sort(cmp);
        }

        void QSort(void) {
            array.QSort();
        }
        bool Swap(size_t ss1, size_t ss2);
    };

    template <class T, class SZ>
    istream& DFO_Array<T, SZ>::read_heap(istream& is) {
        _load_header(is);
        T ref;
        // WARNING - This is massively slow - See DFO::read() for the pre-allocation version -
        while (is) {
            T ref;
            ref.read(is);
            if (is)
                array.Append(ref);
        }
        return is;
    }

    template <class T, class SZ>
    ostream& DFO_Array<T, SZ>::write_heap(ostream& os) {
        _save_header(os);
        for (size_t ss = 0L; ss < array.Nelem(); ss++)
            array[ss].write(os);
        return os;
    }

    template <class T, class SZ>
    istream& DFO_Array<T, SZ>::read(istream& is) {
        _load_header(is);
        array.read(is);
        return is;
    }

    template <class T, class SZ>
    ostream& DFO_Array<T, SZ>::write(ostream& os) {
        _save_header(os);
        array.write(os);
        return os;
    }

    /*
    template <class T, class SZ>
    bool DFO_Array<T, SZ>::_load(T& rRec, istream& is)
       {
       rRec.read(is);
       if(is)
          return true;
       return false;
       }
    template <class T, class SZ>
    bool DFO_Array<T, SZ>::_save(T& rRec, ostream& os)
       {
       rRec.write(os);
       return true;
       }
    template <class T, class SZ>
    int DFO_Array<T, SZ>::operator==(const DFO_Array<T, SZ>& ref)
       {
       if(sFile == ref.sFile)
          return (array == ref.array);
       return 0;
       }
    template <class T, class SZ>
    DFO_Array<T, SZ> DFO_Array<T, SZ>::operator=(const DFO_Array<T, SZ>& ref)
       {
       sFile = ref.sFile;
       array = ref.array;
       return *this;
       }
    template <class T, class SZ>
    int DFO_Array<T, SZ>::operator==(const CompArray<T, SZ>& ary)
       {
       return (array == ary);
       }
    template <class T, class SZ>
    DFO_Array<T, SZ> DFO_Array<T, SZ>::operator=(const CompArray<T, SZ>& ary)
       {
       array = ref.ary;
       return *this;
       }
     */

    // ****************************************************************************
    // These will write the records in array format. Adequate for most applicatons.
    //

    template <class T, class SZ>
    bool DFO_Array<T, SZ>::Swap(size_t ss1, size_t ss2) {
        if (ss2 >= Nelem())
            return false;
        if (ss1 >= Nelem())
            return false;
        T rec = array[ss2];
        array[ss2] = array[ss1];
        array[ss1] = rec;
        return true;
    }

} // stdnoj
#endif
