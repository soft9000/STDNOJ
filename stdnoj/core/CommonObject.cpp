#include <core/CommonObject.hpp>
#include <core/FilePath.hpp>

namespace stdnoj {

    bool CommonObject::IsNull(void) const {
        StdString str;
        ToString(str);
        return str.is_null();
    }

    bool CommonObject::Transfer(ostream& os) {
        StdString str;
        ToString(str);
        str.write_stream(os);
        return true;
    }

    bool CommonObject::Transfer(istream& is) {
        if (!is)
            return false;
        StdString str;
        str.read_stream(is);
        if (!is)
            return false;
        return FromString(str);
    }

    StdString ShareableObject::LiberatedName(char chSep) {
        StdString str;
        ToString(str);
        return FilePath::LiberatedName(str, chSep);
    }
} // stdnoj
