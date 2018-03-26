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
// 11/24/2008: Created to better support compiling under G++, R. Nagy
//

#ifndef _Common_Object_hpx
#define _Common_Object_hpx

#include <core/StdString.hpp>

namespace stdnoj {
    class StdString;

    // DOC: CommonObjects can be both created and stored using strings.
    // When empty, the object should return NULL.

    class CommonObject {
    public:

        CommonObject(void) {
        }

        virtual ~CommonObject(void) {
        }

        virtual bool FromString(const StdString&) = 0;
        virtual const char *ToString(StdString&) const = 0;

        StdString ToString(void) const {
            StdString str;
            ToString(str);
            return str;
        }

        virtual bool IsNull(void) const;

        virtual bool Transfer(ostream& os);
        virtual bool Transfer(istream& os);
    };


    // DOC: References to sharable object can be exchanged between both platform
    // and platform independant applications. For this reason, they should
    // share some common operations.

    class ShareableObject : public CommonObject {
    public:

        ShareableObject(void) {
        }

        ~ShareableObject(void) {
        }

        // DOC: "Liberates" a node name by removing any platform specific 
        // file naming conventions. Great for device and platform
        // independant string comparisons.
        virtual StdString LiberatedName(char chSep = '|');
    };

} // stdnoj

#endif
