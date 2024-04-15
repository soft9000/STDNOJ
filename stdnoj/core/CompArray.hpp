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
// A BETTER sort for an array that uses several different facets.
//
// 11/09/2001: Added Swap(), Sort(), and Comparitor (Don't know why I didn't to it earlier!), Randall Nagy
// 11/19/2001: Broke out into seperate header, with inheritance, for backwards-compatability and maintenance reasons.
// 05/16/2005: Added QSort(void), Randall Nagy
// ****************************************************************************
#ifndef _CompArray_HPP
#define _CompArray_HPP

#include <core/Array.hpp>

namespace stdnoj {

    template <class T>
    class MemberComp {
    public:
        // Return 1 if greater, 0 if equal, -1 if less...

        virtual int Comp(T& l, T& r) {
            return (l > r);
        }
    };

    template <class T, class SZ = size_t>
    class CompArray : public Array<T, SZ> {
    public:
        CompArray(void);
        CompArray(SZ usz);
        CompArray(const CompArray<T, SZ> & old);
        virtual ~CompArray(void);

        void Swap(SZ ss1, SZ ss2); // 11/09/2001: Randall Nagy
        virtual void QSort(void); // 05/16/2005: Randall Nagy
        virtual void Sort(MemberComp<T>& cmp); // 11/09/2001: Randall Nagy

        virtual CompArray<T> & operator=(const Array<T>& ref) {
            Array<T>::operator=(ref);
            return *this;
        }
    };

    template <class T, class SZ>
    CompArray<T, SZ>::CompArray(void) {
    }

    template <class T, class SZ>
    CompArray<T, SZ>::CompArray(SZ usz) {
        AddZombies(usz);
    }

    template <class T, class SZ>
    CompArray<T, SZ>::CompArray(const CompArray<T, SZ> & old) {
        *this = old;
    }

    template <class T, class SZ>
    CompArray<T, SZ>::~CompArray() {
    }

    template <class T, class SZ>
    void CompArray<T, SZ>::Swap(SZ ss1, SZ ss2) {
        T& a = operator[](ss1);
        T& b = operator[](ss2);
        T hold;
        hold = a;
        a = b;
        b = hold;
    }

    template <class T, class SZ>
    void CompArray<T, SZ>::QSort(void) {
        QuickSort<T, SZ> qs;
        qs.Sort(*this);
    }

    template <class T, class SZ>
    void CompArray<T, SZ>::Sort(MemberComp<T>& cmp) {
        // Simple bubble should suffice for the task at hand;
        for (SZ ss1 = 0L; ss1 < Array<T, SZ>::Nelem(); ss1++) {
            T& a = operator[](ss1);
            for (SZ ss2 = 0L; ss2 < Array<T, SZ>::Nelem(); ss2++) {
                T& b = operator[](ss2);
                if (cmp.Comp(a, b) != 1)
                    Swap(ss2, ss1);
            }
        }
    }

} // namespace stdnoj
#endif

