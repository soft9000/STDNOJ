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
// ****************************************************************************
// A Template-based array class that stores objects into a file / header set.
// -The only requirement for the object to be stored is that it be streamable,
// and operator= works as expected (4object duplication when array expands).
//
// 11/22/94, R. Nagy.
// Took my ARRAY soluton from the ADVANCEDC++ class, fixed numerous bugs, and grew into
// the class you see here.
//
// 04/29/94, R. Nagy
// Renamed a lot of members. Added the Insert() and Remove().
//
// 02/10/96, R. Nagy
// Fixed bug in expandActive(), operator==() calls object== (rather than !=), added Nelem().
// TOOK MEMSET OUT OF ARRAY! IT'S EVIL!! CAN REALY SCREW-UP AN ARRAY OF OBJECTS!!
//
// 02/11/96, R. Nagy
// Re-tested and integrated the 'numZombie' feature.  numZombie allows for the array
// to dynamically contract (create trailing "zombies") THEN expandActive (res-use same)
// so as to bypass expensive node re-allocations.  Also added and tested Empty().
//
// ////////////////////////////////////////////////////////////////////////////
// CAREFULL when writing under machines without garbage collection (DOS)!
// If you blow-up on a "new" where trivial operations are occuring && the
// object looks sane, suspect garbage collection && not having enough contiguous
// memory for `new T[]` to succeed: Try storing `T *` and manage instanciations
// yourself to keep memory fragmentation to a mimum under primitive memory-managment
// machines (i.e: Use Baggie<T>).
// ////////////////////////////////////////////////////////////////////////////
//
// 02/20/96: Fixed a bug I introduced last time: expandActive(0) went to += 0, R. Nagy.
// Split features of Empty() between itself and new `Free()` member (4 BETTER
// SPEED DURING RE-USE, STILL USE Empty()!).
//
// 05/31/97: Fixed bugs in Insert(), R. Nagy.
//
// ////////////////////////////////////////////////////////////////////////////
// SPEED, SPEED, SPEED!  This thing must be written for SPEED!! We can use Baggie<T>
// when we don't need  R-A-W  S-P-E-E-D ! !
// ////////////////////////////////////////////////////////////////////////////
//
// 11/26/1997: Warnings removed, finally re-coded operator >>, R. Nagy
// 01/02/1998: Added read() and write(), R. Nagy
// 01/06/1998: Publicised an AddZombies() to support pre-allocation (HyperRFC1), R. Nagy
// 01/09/1998: Re-wrote the AddZombies() operation to do both "expandActive" (IUO)
//    and AddZombies() (public). -Latter expands array, but sets it to zombie space.
//    Former expands the array to active space. R. Nagy.
// 01/20/1998: Fixed bug in operator=() when that.numActive == 0 got new T[0] (boom! on free), R. Nagy.
// 03/13/1998: Had operator ==(), saw no harm in adding Contains(T&), R. Nagy.
// 07/23/1998: Fixed a bug in operator=(): Needed to zero-out numZombie, R. Nagy.
// 05/10/1999: Added stream checking on Array<T, SZ>::read(), R. Nagy
// 08/11/1999: Added At(), R. Nagy.
// 10/09/1999: Added Append(const Array<T>&), R. Nagy
// 01/14/2000: Added SZ as second template parameter, R. Nagy
// 01/19/2000: Added read(istream, FileVersion&), R. Nagy
// 04/14/2001: Added Append(const T&) and operator +(const T&), R. Nagy
// 08/17/2001: Fixed potential bug in ___xMqFY() that occurred when had no elements and no zombies, R. Nagy
// 11/19/2001: Eliminated the potential of the dangling zombie object feature on erroneous array restoration (chalk one up for the learning curve (yeesh)), R. Nagy
// 05/05/2002: Added AddZombies() to ::read() to help speed things up, R. Nagy
// 05/26/2002: Ran through NOTAB (new era!), R. Nagy
// 11/02/2004: Moved QuickSort<T, SZ> into this class, R. Nagy
//    In a futile speed-up jesture, increased NZ_BLOCK_SIZE from 20 to 100, R. Nagy
//    Ran through regression testing suite today. Fixed some bugs that (fortunately) were not part of my typical usage pattern. Required updates as indicated, R. Nagy
//    Fixed bug in AddZombie, R. Nagy
// 06/17/2004: Added Load/Save and moved Array underneath the stdnoj namespace, R. Nagy
// 11/02/2004: Added QuickSort, R. Nagy
// 11/24/2004: Removed File and FileVersion support to better support G++, R. Nagy
// 01/02/2004: Added Dump(), R. Nagy
// 04/22/2005: Improved the Append(Array<T>) operation - Speed and portability enhancements, R. Nagy
// 04/02/2006: Added operator+(Array) and operator+=(Array), R. Nagy
// 07/07/2006: Added T& At(ss) (const) to return a reference to avoid assignment copying (a speed improvement), R. Nagy
// 09/08/2006: Changed write() to const by using At() rather than the never-can-be-const ("autogrow") operator[] - R. Nagy
// 10/01/2006: Extraced ObjecThunk into its own file - We do not use it very often, R. Nagy
// ****************************************************************************
#ifndef _Array2_HPP
#define _Array2_HPP

#include <iostream>
#include <cassert>

using namespace std;

#include <core/ObjectThunk.hpp>


namespace stdnoj {
    template <class T, class SZ = size_t> class QuickSort;

#ifndef NZ_BLOCK_SIZE
#ifdef DOS
#define NZ_BLOCK_SIZE 30      // larger blocks decrease memory fragmentation
#else
#define NZ_BLOCK_SIZE 100     // smaller is okay
#endif
#endif

    // Class Array<T> does *NOT* assume ownership of the Object;

#define SZ_ERROR (-1L)

    template <class T, class SZ = size_t>
    class Array {
    private:
        char hyper_safe; // parinoid array re-allocation check.
        SZ numActive; // (one based) Objects in the array.
        SZ numZombie; // (one based) "things" left over at the end of the array after a Remove() (expandActive() or Insert() fodder).
        T * ObjAry;
        const unsigned int instance;
        static unsigned int instances;

        void _cpary(const Array<T> & a);

        int ___xMqFY(SZ nelem);
        void expandActive(SZ nelem);

    protected:
        //   T    refNull;

    public:
        Array(void); // Makes a zombie array of 1.    Nelem() == 0.
        Array(SZ usz); // Makes a zombie array of usz.  Nelem() == 0.
        Array(const Array<T> & old);
        virtual ~Array(void);

        bool IsNull(void) {
            if (Nelem()) return false;
            return true;
        }

        SZ
        Maximum(void) const {
            return SZ(-1L);
        } // Maximum array capacity (SAME AS SZ_ERROR)

        SZ
        Nelem(void) const {
            return numActive;
        } // Number in array

        SZ
        NumZombies(void) const {
            return numZombie;
        } // was "ZombieCount"
        void
        AddZombies(SZ nelem); // This function GROWS the array by the number specified.
        // Normally, AddZombies() is not required, as operator[Nelem()] auto-expands the ACTIVE array-set
        // for you. However, since the internal array is contiguous, using AddZombies() to pre-size can speed
        // things up quite a bit, as the array is allocated and re-allocated as a single unit when more space
        // is needed.
        // From the coders viewpoint, using AddZombies() is more formal than merley growing an array by
        // referencing off-of the port-bow: Formally calling AddZombies() announces to everyone what you
        // are up to.

        unsigned int
        Instance(void) {
            return instance;
        }

        unsigned int
        Instances(void) {
            return instances;
        }

        Array<T> &
                operator=(const Array<T> & rhs);
        Array<T> &
        operator+(const Array<T> & rhs);
        Array<T> &
                operator+=(const Array<T> & rhs);
        unsigned
        operator ==(const Array<T> & rval) const;
        unsigned
        operator !=(const Array<T> & rval) const;

        void
        operator +(const T& rval) {
            Append(rval);
        }
        //   void
        //      operator +  (unsigned int)         {}
        T &
        operator [](SZ ss); // THIS, OF COURSE, IS ZERO BASED (AUTO EXPANDS TO FILL REQUEST).
        bool
        At(SZ ss, T&) const; // ALLOWS ACCESS TO AN OBJECT IN A CONST ARRAY (NO AUTO_EXPANDING).
        const T&
        At(SZ ss) const; // RETURNS DEFAULT OBJECT OR ERROR: ALLOWS ACCESS TO AN OBJECT IN A CONST ARRAY (NO AUTO_EXPANDING).
        SZ
        Contains(const T&) const; // SZ_ERROR if !found, else FIRST ZERO-BASED index of hit.
        void
        Insert(const T&, SZ ss); // THIS IS ZERO BASED, LIKE OPERATOR []!
        void
        Remove(SZ ss); // THIS IS ZERO BASED, LIKE OPERATOR []!
        void
        Empty(void); // Keeps [] as zombie space, but makes Nelem == 0.
        void
        Free(void); // Deletes [], then makes a single [] with a zombie.
        void
        Append(const Array<T>&);

        void
        Append(const T& ref) {
            Insert(ref, Nelem());
        }
        void
        Dump(ostream& os);

        ostream& write(ostream& os) const;
        istream& read(istream& is);

        friend class QuickSort<T, SZ>;

        //Lets keep it public access, only;
        //friend ostream & operator << (ostream & os,  const Array<T> & a);
        //friend istream & operator >> (istream & is,        Array<T> & a);
    };


    template <class T, class SZ>
    unsigned int Array<T, SZ>::instances = 0;

    template <class T, class SZ>
    Array<T, SZ>::Array(void) :
    numActive(0L),
    numZombie(0L),
    ObjAry(0L),
    hyper_safe(0),
    instance(instances++) {
        ObjAry = new T[1];
        numZombie = 1;
    }

    template <class T, class SZ>
    Array<T, SZ>::Array(SZ usz) :
    numActive(0L),
    numZombie(usz),
    ObjAry(new T[usz]),
    hyper_safe(0),
    instance(instances++) {
    }

    template <class T, class SZ>
    Array<T, SZ>::Array(const Array<T> & old) :
    numActive(old.numActive),
    numZombie(0L),
    ObjAry(new T[old.numActive]),
    hyper_safe(0),
    instance(instances++) {
        _cpary(old);
    }

    template <class T, class SZ>
    Array<T, SZ>::~Array() {
        delete [] ObjAry;
    }

    template <class T, class SZ>
    void Array<T, SZ>::Empty(void) {
#ifdef ARRAY_EMPTY_OKAY
        // Better still (minimize memory fragmentation);
        numZombie += numActive;
        numActive = (SZ) 0L;
#else
        Free(); // Problems with memory-overcommits on NT made me hard-code in this
        // Free(): After all, most popular OS' in the 90's FINALLY have
        // garbage collection!
#endif
    }

    template <class T, class SZ>
    void Array<T, SZ>::Free(void) {
        delete [] ObjAry;
        ObjAry = 0L;
        numZombie = numActive = (SZ) 0L;
        // ^^ jic we throw during allocation vv;
        ObjAry = new T[1];
        numZombie = 1;
    }

    template <class T, class SZ>
    void Array<T, SZ>::Append(const Array<T>& aAry) {
        AddZombies(aAry.Nelem() + 1); // 04/22/2005 - Speed up, R. Nagy
        T ref;
        for (SZ ss = 0L; ss < aAry.Nelem(); ss++) {
            aAry.At(ss, ref); // 04/22/2005 - Easier for compilers to "see", R. Nagy
            Append(ref);
        }
    }

    template <class T, class SZ>
    void Array<T, SZ>::_cpary(const Array<T> & a) {
        // By this time, we assume old array has been deleted, and the arrays are
        // now excatly the same size (refer to operator = for details).
        for (SZ i = 0; i < numActive; i++)
            ObjAry[i] = a.ObjAry[i];
    }

    template <class T, class SZ>
    void Array<T, SZ>::Insert(const T& ins, SZ ss) // THIS IS ZERO BASED, LIKE OPERATOR []!
    {
        // It this realy an insert?
        if (ss >= numActive) {
            // No, It's an append: do the overflow thing;
            operator[](ss) = ins;
            return;
        }
        // Is there room for one more? (2make an overflow (insert) area?)
        if (numZombie <= 1) {
            // No, set-up a new block && create a zombie so we have room for the insert/expandActive;
            SZ osz = numActive;

            expandActive(NZ_BLOCK_SIZE);

            // The following number resets are not trivial: expandActive() marks the entire expandActive
            // amount as active. expandActive() also consumes all zombies. We need to perform the
            // following adjustments to reset our state to accomidate the upcomming expansion:
            numZombie = NZ_BLOCK_SIZE - 1; // DEACTIVATE WHAT expandActive() ACTIVATED
            numActive = osz + 1; // OVER-RIDE (reduce) THE expandActive() ACTIVATED BLOCK
        } else {
            // YES (F-A-S-T-E-R): Just use a zombie && expandActive the array;
            numZombie--;
            numActive++;
        }
        // DOWNTO copy loop:
        for (SZ i = numActive; i > ss; i--)
            ObjAry[i] = ObjAry[i - 1];
        ObjAry[ss] = ins;
    }

    template <class T, class SZ>
    void Array<T, SZ>::Remove(SZ ss) // THIS IS ZERO BASED, LIKE OPERATOR []!
    {
        if (ss >= numActive)
            return;

        numActive--; // n + 1
        for (SZ i = ss; i < numActive; i++)
            ObjAry[i] = ObjAry[i + 1];

        // We've removed one from the numActive set;
        numZombie++;
        // LET'S ASSUME A SANE USER. I.E: Let THEM PUT THE PAYMENT WHERE IT BELONGS...
        // THIS THING IS WRITTEN FOR  R-A-W  S-P-E-E-D ! !;
        T DefaultVal;
        ObjAry[numActive] = DefaultVal; // Just in case we grow without an expandActive (it happens!)
    }

    template <class T, class SZ>
    int Array<T, SZ>::___xMqFY(SZ nelem) {
        // assert(hyper_safe);
        // *******************************************************************************
        // WARNING!! THIS IS A MASSIVELY HAZZARDOUS OPERATION! IS IS ONLY TO BE CALLED VIA
        // AddZombies() OR expandActive()!! USE THOSE FUNCTIONS TO ACCESS THIS OPERATION!
        // *******************************************************************************

        // If we have enough "Zombie" space to expandActive into, then use it;
        if (numZombie && (numZombie >= nelem)) // 8/17/01 bug avoidance, R. Nagy
        {
            if (nelem == 0L)
                nelem = 1;
            numZombie -= nelem;
            numActive += nelem;
            return 0;
        }
        // Else we throw it ALL away && swap-in a new array;
        T * newObjAry = new T[numActive + nelem];

        for (SZ i = 0; i < numActive; i++)
            newObjAry[i] = ObjAry[i];

        delete [] ObjAry;
        ObjAry = newObjAry;

        // Have thrown it all away, ergo, no mo zombies!
        // --Calling function determine zombie context;
        numZombie = 0L;

        return 1;
    }

    template <class T, class SZ>
    void Array<T, SZ>::expandActive(SZ nelem) {
        nelem++; // 05/26/2002 (moved it here, rather than isolated to operator[])
        hyper_safe = 1;
        // In the past, expanzion was only on-demand. For this reason,
        // we always expandActive to "active", as it made sense in the context of
        // the page-fault (it was user-generated, so we assumed they knew what
        // they wanted, even if there might be default-initialized array members).
        if (___xMqFY(nelem)) {
            // "expandActive to active"
            // (zombie count stays zero)
            numActive += nelem;
        }
        hyper_safe = 0;
    }

    template <class T, class SZ>
    void Array<T, SZ>::AddZombies(SZ nelem) {
        // "Add" is somewhat of a misnomer: It translate to "add enough Zombies to accomidate this size":
        if (numZombie >= nelem)
            return;
        hyper_safe = 1;
        // Now we let user "snap the waste-line", themselves: This is mainly a performance,
        // pre-sizing, operation. It occurs under the auspices of a savvy user: Typically
        // occurs before a huge array-loading, to keep array re-allocations to a minimum.
        if (___xMqFY(nelem)) {
            // "expandActive to zombie"
            // (active count not changed)
            numZombie = nelem;
        } else {
        }
        hyper_safe = 0;
    }

    template <class T, class SZ>
    Array<T> & Array<T, SZ>::operator=(const Array<T> & rhs) {
        if (this != &rhs) {
            if (!rhs.numActive)
                Empty(); // Space (Free()) or speed (Empty())?
            else {
                delete [] ObjAry;
                numActive = rhs.numActive;
                numZombie = 0L;
                ObjAry = new T[numActive];
                _cpary(rhs);
            }
        }
        return *this;
    }

    template <class T, class SZ>
    Array<T> & Array<T, SZ>::operator+(const Array<T> & rhs) {
        T ref;
        AddZombies(rhs.Nelem());
        for (size_t ss = 0L; ss < rhs.Nelem(); ss++) {
            rhs.At(ss, ref);
            Append(ref);
        }
        return *this;
    }

    template <class T, class SZ>
    Array<T> & Array<T, SZ>::operator+=(const Array<T> & rhs) {
        T ref;
        AddZombies(rhs.Nelem());
        for (size_t ss = 0L; ss < rhs.Nelem(); ss++) {
            rhs.At(ss, ref);
            Append(ref);
        }
        return *this;
    }

    template <class T, class SZ>
    bool Array<T, SZ>::At(SZ ss, T& rCopy) const // THIS, OF COURSE, IS ZERO BASED!
    {
        if (ss >= numActive) {
            // BLANK-IT-OUT;
            T rResult;
            rCopy = rResult;
            return false;
        }
        // COPY IT OUT;
        rCopy = ObjAry[ss];
        return true;
    }

    template <class T, class SZ>
    const T& Array<T, SZ>::At(SZ ss) const // THIS, OF COURSE, IS ZERO BASED!
    {
        static T refNull;
        if (ss >= numActive) {
            // RETURN OBJECT DEFAULT;
            T ref;
            refNull = ref;
            return refNull;
        }
        // REFERENCE IT OUT;
        return ObjAry[ss];
    }

    template <class T, class SZ>
    T & Array<T, SZ>::operator [] (SZ ss) // THIS, OF COURSE, IS ZERO BASED!
    {
        if (ss >= numActive) {
            if (!numActive)
                expandActive(ss); // 5/26/2002 R. Nagy (moved the +1 to expandActive)
            else
                expandActive(ss - numActive); // 5/26/2002 R. Nagy (moved the +1 to expandActive)
        }
        return ObjAry[ss];
    }

    template <class T, class SZ>
    unsigned Array<T, SZ>::operator !=(const Array<T> & rval) const {
        return !(*this == rval);
    }

    template <class T, class SZ>
    unsigned Array<T, SZ>::operator ==(const Array<T> & rval) const {
        if (numActive != rval.numActive)
            return 0;

        for (SZ i = 0; i < numActive; i++)
            if (!(ObjAry[i] == rval.ObjAry[i]))
                return 0;

        return 1;
    }

    template <class T, class SZ>
    SZ Array<T, SZ>::Contains(const T& rVal) const {
        for (SZ i = 0; i < numActive; i++)
            if ((ObjAry[i] == rVal))
                return i;

        return SZ_ERROR;
    }

    template <class T, class SZ>
    void Array<T, SZ>::Dump(ostream& os) {
        for (size_t ss = 0L; ss < Nelem(); ss++) {
            T& rObj = operator[](ss);
            rObj.write(os);
        }
    }

    template <class T, class SZ>
    ostream & Array<T, SZ>::write(ostream & os) const {
        SZ nelem = Nelem();
        os << nelem << " ";
        for (SZ ss = 0; ss < nelem; ss++)
            ObjAry[ss].write(os);
        return os;
    }

    template <class T, class SZ>
    istream & Array<T, SZ>::read(istream &is) {
        Empty();
        SZ nelem = NULL;
        if (!is)
            return is;
        is >> nelem;
        if (!is)
            return is;
        char ch;
        is.read(&ch, 1);
        if (!is)
            return is;
        if (nelem)
            AddZombies(nelem + 2); // 11-22-2002: One more time (sigh)       
        for (SZ ss = 0; ss < nelem; ss++) {
            T rObj;
            rObj.read(is);
            if (!is)
                break; // 11-19-2001: Eliminated the dangling zombie object.
            (*this)[ss] = rObj;
        }
        return is;
    }

    template <class T>
    ostream & operator <<(ostream & os, Array<T>& a) {
        return a.write(os);
    }

    template <class T>
    istream & operator >>(istream & is, Array<T>& a) {
        return a.read(is);
    }
    //template <class T, class SZ = size_t> class QuickSort
#ifdef MSDOS
#define _cdecl
#endif
#ifdef UNIX
#define _cdecl
#endif

    template <class T, class SZ> class QuickSort {
    private:

        static int _cdecl cmp(const void *pl, const void *pr) {
            T l = *((T *) pl);
            T r = *((T *) pr);
            if (l > r)
                return 1;
            if (l < r)
                return -1;
            return 0;
        }

    protected:

        virtual void Swap(T& l, T& r) {
            T hold = l;
            l = r;
            r = hold;
        }

    public:

        void Sort(T *parray, SZ nelem) {
            qsort(parray, nelem, sizeof (T), QuickSort::cmp);
        }

        virtual void Sort(Array<T, SZ>& array) {
            Sort(array.ObjAry, array.Nelem());
        }
    };

} // namespace stdnoj

#endif


