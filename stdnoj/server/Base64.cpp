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
#include <cstdio>
#include <cstdlib>
#include <Server/Base64.hpp>


char *encode_table;
char *decode_table;

#define CHAR64(c)  (((c) < 0 || (c) > 127) ? -1 : index_64[(c)])


static char basis_64[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/???????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????";

static char index_64[128] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
};

bool Base64::Encode(const StdString& sIn, StdString& sOut) {
    sOut = sIn;
    return Encode(sOut);
}

bool Base64::Decode(const StdString& sIn, StdString& sOut) {
    sOut = sIn;
    return Decode(sOut);
}

bool Base64::Encode(StdString& strIO) {
    bool br = true;
    size_t sz = (strIO.length() * 2) + 2;
    char *pBuf = new char[sz];
    unsigned final_size = NULL;
    if (_encode(strIO.c_str(), strIO.length(), pBuf, sz, &final_size) != tTRUE)
        br = false;
    strIO = pBuf;
    delete [] pBuf;
    return br;
}

bool Base64::Decode(StdString& strIO) {
    bool br = true;
    size_t sz = strIO.length() + 2;
    char *pBuf = new char[sz];
    unsigned final_size = NULL;
    if (_decode(strIO.c_str(), strIO.length(), pBuf, &final_size) != tTRUE)
        br = false;
    strIO = pBuf;
    delete [] pBuf;
    return br;
}

TROOL Base64::_encode(const char *_in, unsigned inlen,
        char *_out, unsigned outmax, unsigned *outlen) {
    const unsigned char *in = (const unsigned char *) _in;
    unsigned char *out = (unsigned char *) _out;
    unsigned char oval;
    char *blah;
    unsigned olen;

    /* Will it fit? */
    olen = (inlen + 2) / 3 * 4;
    if (outlen)
        *outlen = olen;
    if (outmax < olen)
        return tERROR;

    /* Do the work... */
    blah = (char *) out;
    while (inlen >= 3) {
        /* user provided max buffer size; make sure we don't go over it */
        *out++ = basis_64[in[0] >> 2];
        *out++ = basis_64[((in[0] << 4) & 0x30) | (in[1] >> 4)];
        *out++ = basis_64[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
        *out++ = basis_64[in[2] & 0x3f];
        in += 3;
        inlen -= 3;
    }
    if (inlen > 0) {
        /* user provided max buffer size; make sure we don't go over it */
        *out++ = basis_64[in[0] >> 2];
        oval = (in[0] << 4) & 0x30;
        if (inlen > 1) oval |= in[1] >> 4;
        *out++ = basis_64[oval];
        *out++ = (inlen < 2) ? '=' : basis_64[(in[1] << 2) & 0x3c];
        *out++ = '=';
    }

    if (olen < outmax)
        *out = '\0';

    return tTRUE;
}

TROOL Base64::_decode(const char *in, unsigned inlen,
        char *out, unsigned *outlen) {
    unsigned len = 0, lup;
    int c1, c2, c3, c4;

    /* check parameters */
    if (out == NULL) return tFALSE;

    /* xxx these necessary? */
    if (in[0] == '+' && in[1] == ' ') in += 2;
    if (*in == '\r') return tFALSE;

    for (lup = 0; lup < inlen / 4; lup++) {
        c1 = in[0];
        if (CHAR64(c1) == -1) return tFALSE;
        c2 = in[1];
        if (CHAR64(c2) == -1) return tFALSE;
        c3 = in[2];
        if (c3 != '=' && CHAR64(c3) == -1) return tFALSE;
        c4 = in[3];
        if (c4 != '=' && CHAR64(c4) == -1) return tFALSE;
        in += 4;
        *out++ = (CHAR64(c1) << 2) | (CHAR64(c2) >> 4);
        ++len;
        if (c3 != '=') {
            *out++ = ((CHAR64(c2) << 4) & 0xf0) | (CHAR64(c3) >> 2);
            ++len;
            if (c4 != '=') {
                *out++ = ((CHAR64(c3) << 6) & 0xc0) | CHAR64(c4);
                ++len;
            }
        }
    }

    *out = 0; /* terminate string */
    *outlen = len;
    return tTRUE;
}
