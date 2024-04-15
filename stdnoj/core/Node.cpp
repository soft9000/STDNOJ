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

    istream& Node::read(istream& is) {
        StdString sBuf;
        sBuf.read_stream(is);
        FullName(sBuf.c_str());
        sReserved.read_stream(is);
        return is;
    }

    ostream& Node::write(ostream& os) const {
        StdString sBuf = GetFullName('/');
        sBuf.write_stream(os);
        sReserved.write_stream(os);
        return os;
    }

    bool Node::FromString(const StdString& str) {
        // Unlike Files and Directories, nodes MUST exist on the file system.
        return FilePath::Open(str, *this);
    }

    const char *Node::ToString(StdString& str) const {
        Name(str);
        return str.c_str();
    }

    bool Node::IsOnline(void) const {
        return online;
    }

    bool Node::IsDir(void) const {
        return FA_DIREC;
    }

    bool Node::IsFile(void) const {
        return !IsDir();
    }

    bool Node::IsExec(void) const {
        return bIsExec;
    }

    void Node::IsExec(bool br) {
        bIsExec = br;
    }

    bool Node::IsChanged(void) const {
        return FA_ARCH;
    }

    bool Node::IsLabel(void) const {
        return FA_LABEL;
    }

    bool Node::IsRead(void) const {
        return FA_RDONLY;
    }

    bool Node::IsWrite(void) const {
        return FA_RDONLY;
    }

    bool Node::IsSystem(void) const {
        return FA_SYSTEM;
    }

    bool Node::IsHidden(void) const {
        return FA_HIDDEN;
    }

    void Node::IsOnline(bool bf) {
        online = bf;
    }

    void Node::IsDir(bool bf) {
        FA_DIREC = bf;
    }

    void Node::IsFile(bool bf) {
        FA_DIREC = bf;
    }

    void Node::IsChanged(bool bf) {
        FA_ARCH = bf;
    }

    void Node::IsLabel(bool bf) {
        FA_LABEL = bf;
    }

    void Node::IsRead(bool bf) {
        FA_RDONLY = bf;
    }

    void Node::IsWrite(bool bf) {
        FA_RDONLY = bf;
    }

    void Node::IsSystem(bool bf) {
        FA_SYSTEM = bf;
    }

    void Node::IsHidden(bool bf) {
        FA_HIDDEN = bf;
    }

    const char *Node::Name(StdString& str) const {
        str = GetFullName(PathChar());
        return str.c_str();
    }

    StdString Node::QueryNode(void) const {
        StdString sBuf = GetNodeName();
        return sBuf;
    }

    const char *Node::QueryNode(StdString& sBuf) const {
        sBuf = GetNodeName();
        return sBuf.c_str();
    }

    StdString Node::GetNodeName(void) const {
        StdString sBuf = sName;
        return sBuf;
    }

    StdString Node::GetNodePath(void) const {
        StdString sBuf = sPath;
        return sBuf;
    }

    StdString Node::GetDrive(void) const {
        StdString sBuf = sDrive;
        return sBuf;
    }

    StdString Node::GetNormalizePath(StdString& sPath) const {
        size_t ss;
        do {
            ss = sPath.rfind("\\");
            if (ss != NPOS)
                sPath[ss] = '/'; // Enforce UNIX path conventions
        } while (ss != NPOS);
        return sPath;
    }

    void Node::Path(const StdString& psz) {
        sPath.assign(psz);
        GetNormalizePath(sPath);
    }

    void Node::Drive(const StdString& psz) {
        sDrive.assign(psz);
    }

    StdString Node::GetPathName(char sep) const {
        StdString sPath;
        sPath = GetNodePath();
        if (sep == '/')
            return sPath;

        sPath.replace('/', sep);
        return sPath;
    }

    StdString Node::GetFullName(char sep) const {
        StdString sBuf;
        sBuf.assign(GetDrive());

        StdString pPath = GetNodePath();
        sBuf.append(pPath);
        size_t sz = pPath.length();
        if (pPath[sz - 1] != '/')
            sBuf.append("/");

        sBuf.append(GetNodeName());

        if (sep != '/')
            sBuf.replace('/', sep);

        return sBuf;
    }

    void Node::FullName(const StdString& psz) {
        sPath.assign("<NOPATH>");
        sDrive.assign("<NODRIVE>");

        StdString str = psz;
        GetNormalizePath(str);

        size_t ss = str.rfind("/");
        if (ss != NPOS) {
            StdString std;
            sName.assign(str.substr(std, ss + 1));
            str.remove(ss);
        } else {
            // No identifying marks: Assume it's a node name
            // && we're outta here!
            sName.assign(str.c_str());
            return;
        }

        // We've got a name, now the path and drive;
        ss = str.find("/");
        if (ss != NPOS) {
            StdString std;
            Path(str.substr(std, ss));
            str.remove(ss);
        }

        sDrive.assign(str);
    }

    time_t Node::GetCreateTime(void) const {
        return ctime;
    }

    time_t Node::GetModTime(void) const {
        return mtime;
    }

    time_t Node::GetAccessTime(void) const {
        return atime;
    }

    bool Node::operator ==(const Node& rDN) const {
        // Assuming selectors / linear address base;
        if ((this) == (&rDN))
            return true;
        StdString s1, s2;
        s1 = GetFullName('/');
        s2 = rDN.GetFullName('/');
        if (s1 == s2)
            return true;
        return false;
    }

    bool Node::operator<(const Node& rDN) const {
        return (ctime < rDN.ctime);
    }

    bool Node::operator>(const Node& rDN) const {
        return (ctime > rDN.ctime);
    }

    Node& Node::operator=(const Node& nr) {
        if (this == &nr)
            return *this;
        ctime = nr.ctime;
        mtime = nr.mtime;
        atime = nr.atime;
        sName.assign(nr.sName);
        sPath.assign(nr.sPath);
        sDrive.assign(nr.sDrive);
        online = nr.online;
        FA_NORMAL = nr.FA_NORMAL;
        FA_RDONLY = nr.FA_RDONLY;
        FA_HIDDEN = nr.FA_HIDDEN;
        FA_SYSTEM = nr.FA_SYSTEM;
        FA_LABEL = nr.FA_LABEL;
        FA_DIREC = nr.FA_DIREC;
        FA_ARCH = nr.FA_ARCH;
        bIsExec = nr.bIsExec;
        file_size = nr.file_size;
        return *this;
    }

    void Node::SetCreateDateTime(time_t ttt) {
        ctime = ttt;
    }

    void Node::SetModDateTime(time_t ttt) {
        mtime = ttt;
    }

    void Node::SetAccessDateTime(time_t ttt) {
        atime = ttt;
    }

} // stdnoj

