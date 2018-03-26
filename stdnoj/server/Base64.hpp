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
// 09/23/2003: Ported to stdnoj, R. Nagy
#ifndef base64_ed_h
#define base64_ed_h

#include <stdnoj.hpp>

using namespace stdnoj;

class Base64 {
protected:
    TROOL _encode(const char *_in, unsigned inlen, char *_out, unsigned outmax, unsigned *outlen);
    TROOL _decode(const char *in, unsigned inlen, char *out, unsigned *outlen);
public:
    // Destructive usage:
    bool Encode(StdString& strIO);
    bool Decode(StdString& strIO);
    // Non-destructive:
    bool Encode(const StdString& sIn, StdString& sOut);
    bool Decode(const StdString& sIn, StdString& sOut);
};

#endif



