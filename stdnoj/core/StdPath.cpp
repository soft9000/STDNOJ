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
#include <stdnoj.hpp>

namespace stdnoj {

    StdPath::Path::Path(const Path& ref) {
        sPath = ref.sPath;
    }

    StdPath::Path::Path(const StdString& ref) {
        *this = ref;
    }

    StdPath::Path& StdPath::Path::operator=(const StdString& s) {
        StdString str = s;
        if (str.is_null()) {
            sPath = str;
            return *this;
        }
        // Paths
        //   -All use a POSIX (or Internet) path naming convention
        str.replace('\\', '/');
        //   -Do not have mount points ("d:", "ftp:". etc.)
        str.remove_pos(0, str.find(':'));
        //   -Do not have a leading "//"
        str.replace("//", '/');
        size_t whence = str.length() - 1;
        //   -Do not end with '/'
        if (str[whence] == '/')
            str.remove(whence);
        //   -Begin with either '/', './', or '../'
        sPath = str;
        return *this;
    }

    StdPath::Path::operator StdString(void) {
        StdString str = sPath;
        return str;
    }

    ostream& StdPath::Path::write(ostream& os) {
        sPath.write_stream(os);
        return os;
    }

    istream& StdPath::Path::read(istream& is) {
        sPath.read_stream(is);
        return is;
    }

    bool StdPath::_relate(const StdString& sFull, StdString& sResult) {
        sResult = "";
        StdString str = sFull;

        str.replace("/./", '/'); // it happens...

        // Is there anything to expand?
        if (str.find("../") == npos) {
            sResult = str;
            return true;
        }

        // Check if a node requires the relative elimination of any previous nodes
        Array<StdString> array;
        PipeArray::Parse(array, str, '/');
        if (!array.Nelem()) {
            sResult = str;
            return true;
        }

        // By now, we should have a mount point. Relative prefixes are now illegal...
        if (array[0] == "..")
            return false;
        if (array[0] == ".")
            return false;

        // Resolve relative
        size_t ss = 0L;
        for (ss = 1; ss < array.Nelem(); ss++) {
            if (array[ss] == "..") {
                // Remove the prior, non-null, non-relative node
                size_t prev = ss;
                while (array[prev].is_null() && array[prev] != "..") {
                    if (prev == 0) {
                        prev = (size_t) - 1L;
                        break;
                    }
                    prev--;
                }
                if (prev != -1L)
                    array[prev] = "";
                array[ss] = ""; // remove the relative markup indicator
            }
        }
        // Finally, we build the final result;
        for (ss = 0; ss < array.Nelem(); ss++) {
            sResult.append(array[ss]);
            sResult.append('/');
        }
        return true;
    }

    bool StdPath::_resolve(const StdString& sMount, const StdString& sNode, StdString& sResult) {
        StdString str = sMount;
        str.replace('\\', '/'); // normalize it
        if (str[str.length() - 1] != '/')
            str.append('/');
        str += sNode;
        return _relate(sNode, sResult);
    }

    bool StdPath::Mount(const Path& path, const Url& url, Url& uResult) {
        /* TODO 1 -oR. Nagy -c<<<WIP>>> : Need to finish this, as required. */
        return false;
    }

    bool StdPath::Mount(const Path& path, const Url& url, File& uResult) {
        /* TODO 1 -oR. Nagy -c<<<WIP>>> : Need to finish this, as required. */
        return false;
    }

    bool StdPath::Mount(const Path&, const Url& url, Directory& uResult) {
        /* TODO 1 -oR. Nagy -c<<<WIP>>> : Need to finish this, as required. */
        return false;
    }

    bool StdPath::Mount(const Path&, const File& file, File& uResult) {
        /* TODO 1 -oR. Nagy -c<<<WIP>>> : Need to finish this, as required. */
        return false;
    }

    bool StdPath::Mount(const Path& path, const File& file, Url& uResult) {
        /* TODO 1 -oR. Nagy -c<<<WIP>>> : Need to finish this, as required. */
        return false;
    }

    bool StdPath::Mount(const Path& path, const Directory& dir, Directory& uResult) {
        /* TODO 1 -oR. Nagy -c<<<WIP>>> : Need to finish this, as required. */
        return false;
    }

    bool StdPath::Mount(const Path&, const Directory& dir, Url& uResult) {
        /* TODO 1 -oR. Nagy -c<<<WIP>>> : Need to finish this, as required. */
        return false;
    }
} // stdnoj

