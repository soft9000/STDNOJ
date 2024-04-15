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
#ifndef ObjectThunk_Hpp
#define ObjectThunk_Hpp

//#ifdef WIN32
//#define EXPORT __declspec(dllexport)
//#else
//#define EXPORT _export
//#endif


// An ObjectThunk is what you use when you want to store a polymorphic hirearchy of objects into
// a single array. By using Array<ObjectThunk<My_Base_Object> > you can store the actual pointers
// to the object in Array<T>.
//
// Note that ownership is provided for, but the default is * NO * deletion upon array destruction.
// Note also that the read()ing of polymorphic objects is an art form that requires an ObjectFactory!
//  -for this reason, an array of ObjectThunk's is best used for in-memory, run-time, un_owned persistance.
//  However, if you can "factory" a read via yet ANOTHER pointer thunk ("presto-changeo" object-pointer
//  morphing upon read -w- dynamic class detection / instanciation), then you're an OO master && this
//  approach will work as expected!

namespace stdnoj {

    template <class T>
    class ObjectThunk {
    protected:
        int should_delete;
        T *pT;

    public:

        ObjectThunk(void) {
            pT = new T;
            should_delete = 0;
        }

        ObjectThunk(T *pe) {
            pT = pe;
            should_delete = 0;
        }

        ObjectThunk(const ObjectThunk& et) {
            *this = et;
        }

        virtual ~ObjectThunk(void) {
            if (should_delete) delete pT;
            pT = NULL;
        }

        void owner(int owns) {
            should_delete = owns;
        }

        int owner(void) {
            return should_delete;
        }

        T * get(void) {
            return pT;
        }

        void put(T *pe) {
            pT = pe;
        }

        operator T(void) {
            return *pT;
        }

        // IMPORTANT: ownership *MUST* transfer upon assignment, as operator=(et) is what Array<T> uses;

        ObjectThunk & operator=(const ObjectThunk& et) {
            pT = et.pT;
            should_delete = et.should_delete;
            return *this;
        }
        // NOTE THAT: Assignment is by pointer. We avoid assignment by reference for safety's (!stack) sake;

        ObjectThunk & operator=(T *pe) {
            pT = pe;
            return *this;
        }

        int operator==(const ObjectThunk& et) {
            if (!pT) {
                if (!et.pT) return 1;
                return 0;
            }
            return (*(et.pT) == (*pT));
        }

        virtual int operator>(const ObjectThunk& et) {
            return (*pT > *(et.pT));
        }

        virtual int operator<(const ObjectThunk& et) {
            return (*pT < *(et.pT));
        }

        virtual int operator<=(const ObjectThunk& et) {
            return (*pT <= *(et.pT));
        }

        virtual int operator>=(const ObjectThunk& et) {
            return (*pT >= *(et.pT));
        }

        ostream& write(ostream& os) {
            os << should_delete << ' ';
            if (!pT) return os;
            return (pT->write(os));
        }
        // WARNING: WITHOUT AN OBJECT FACTORY, THE READING OF POLYMORPHIC TYPES IS OF QUESTIONABLE VALUE!

        istream& read(istream& os) {
            char ch;
            os >> should_delete;
            os >> ch;
            if (!pT) return os;
            return (pT->read(os));
        }
    };

}

#endif
