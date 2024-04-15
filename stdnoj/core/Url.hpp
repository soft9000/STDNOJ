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
//
// Note: Url is a partial implementation of rfc1738 - Just what I needed, 
// as I needed it. When fully implemented, it will become stdnet::URL.
//
// Targeted syntax is "protocol://<user>:<password>@<host>:<port>/<url-path>"

//
// 08/21/2008: Started, R. Nagy
//
#ifndef xzUrl_Hpp
#define xzUrl_Hpp

#include <core/File.hpp>
#include <core/Directory.hpp>


namespace stdnoj {

    enum UrlType {
        url_none,
        url_http,
        url_https,
        url_ftp,
        url_file
    };

    class Url : public ShareableObject {
    private:
        UrlType uType;
        StdString sUrl;

    public:
        Url(void);

        UrlType Type(void) const {
            return uType;
        }

        StdString Site(void) const;
        StdString Page(void) const;

        bool Name(const StdString& str);
        bool Name(const File& file);
        bool Name(const Directory& dir);

        StdString Name(void) const {
            StdString s = sUrl;
            return s;
        }

        bool FromString(const StdString& str);
        const char *ToString(StdString& str) const;

        bool Launch(void) const;
    };

    inline
    bool OpenURL(const StdString& str) {
        Url url;
        if (url.Name(str) == false)
            return false;
        return url.Launch();
    }

} // namespace stdnoj


#endif
