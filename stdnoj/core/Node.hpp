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
// **************************************************************************
// The NODES Project.  Started on 08/25/95, by and exclusively for Randall Nagy
// This is private and confidential stuff.
// If you are reading or using this code without my expressed permission,
//          -I'm a gonna slappa yo face....  :-o
//
// Project begun 95/08/25, Randall Nagy
//    o  Initial set-up of the Node hirearchy and browser, Randall Nagy.
// 10/02/1996: Promoted a majority of the DOS implemnetation to Node && wrote -n- tested
//    the stream I/O logic.  Also removed Type(), Randall Nagy
// 06/23/1997: f_attrib fixed to copy-over on operator=(), Randall Nagy
// 01/07/1998: Moved it over to StdString, Randall Nagy
// 01/08/1998: Added Extract(PortTime), Randall Nagy
// 01/29/1998: Fixed FullName(const StdString&): sDrive got a path 'cuz of an rfind(), Randall Nagy
// 07/15/1998: Moved it over to string from StdString.
// 07/16/1998: Added "NormalizePath" && made everything into UNIX path-seperators, Randall Nagy.
//             Node I/O LOGIC: Use .read_stream(): white_space is the norm in file-names now, Randall Nagy.
// 10/15/1999: Ran through NOTAB and added copy-constructor, Randall Nagy
// 11/03/1999: Added PathName(), Randall Nagy
// 12/07/1999: Added Open(), Randall Nagy
// 11/13/2008: Lightly updated to support commmon usage, Randall Nagy
// 11/24/2008: Re-arranging headers to better support G++, Randall Nagy
//
// **************************************************************************
// NOTE: Unlike Files and Directories, nodes MUST exist on any file system.
// **************************************************************************
//

#ifndef _Node_hpp
#define _Node_hpp

#include <core/StdString.hpp>

namespace stdnoj {
    class StdString;

    // Under DOS, the type of the file is in WIN.INI and is associated with a file entension.
    // Under UNIX, the type for the file is part of the file system, and in something like `/etc/magic`.
    // -We'll worry about that, if the time comes...  Right now, we'll assume the former will be the
    // case for the commercially-available future: We won't break-out a Type().

    // Unlike Files and Directories, nodes MUST exist on the file system.

    class Node : public ShareableObject {
    private:
        bool scroll;
        bool online;

        bool FA_NORMAL;
        bool FA_RDONLY;
        bool FA_HIDDEN;
        bool FA_SYSTEM;
        bool FA_LABEL;
        bool FA_DIREC;
        bool FA_ARCH;

    protected:
        StdString sReserved;
        bool bIsExec;
        BigFileSize file_size;
        StdString sName;
        StdString sPath;
        StdString sDrive;

        time_t ctime, mtime, atime;

        bool Scrolled(void) const {
            return scroll;
        }

        void Scrolled(bool s) {
            scroll = s;
        }

        void IsOnline(bool);

        void Path(const StdString&);
        void Drive(const StdString&);
        void FullName(const StdString&);

        void SetTime(char h, char m, char s);
        void SetDate(char m, char d, unsigned y);

        void IsDir(bool);
        void IsFile(bool);
        void IsExec(bool);
        void IsLabel(bool);

        void IsRead(bool);
        void IsWrite(bool);
        void IsChanged(bool); // FA_ARCH
        void IsSystem(bool);
        void IsHidden(bool);

        void SetCreateDateTime(time_t st_ctime);
        void SetModDateTime(time_t st_mtime);
        void SetAccessDateTime(time_t st_atime);

    public:

        Node(void) : scroll(false), online(false), ctime(0L), mtime(0L), atime(0L), bIsExec(false),
        FA_NORMAL(false), FA_RDONLY(false), FA_HIDDEN(false), FA_SYSTEM(false), FA_LABEL(false), FA_DIREC(false), FA_ARCH(false) {
        }

        Node(const Node& rNode) {
            *this = rNode;
        }

        ~Node(void) {
        }

        // CommonObject
        bool FromString(const StdString&);
        const char *ToString(StdString&) const;

        //     virtual bool  Open(const StdString& sFileName)  {return false;}

        long unsigned Extract(BigFileSize& fz) const {
            fz = file_size;
            return fz.GetSize();
        }

        const char *Name(StdString &sResult) const;

        StdString QueryNode(void) const;
        const char *QueryNode(StdString& sResult) const;

        StdString GetDrive(void) const;
        StdString GetNodeName(void) const;
        StdString GetNodePath(void) const;
        StdString GetFullName(char sep) const;
        StdString GetPathName(char sep) const;
        StdString GetNormalizePath(StdString& str) const;
        time_t GetCreateTime(void) const;
        time_t GetModTime(void) const;
        time_t GetAccessTime(void) const;

        bool IsOnline(void) const;

        bool IsDir(void) const;
        bool IsFile(void) const;
        bool IsExec(void) const;

        bool IsRead(void) const;
        bool IsWrite(void) const;
        bool IsHidden(void) const;

        bool IsChanged(void) const; // FA_ARCH
        bool IsSystem(void) const;
        bool IsLabel(void) const;

        virtual istream& read(istream&);
        virtual ostream& write(ostream&) const;

        Node & operator =(const Node&);

        bool operator ==(const Node&) const;

        bool operator ==(Node* pDn) const {
            return (*this == *pDn);
        }

        bool operator>(const Node&) const;

        bool operator>(Node* pDn) const {
            return (*this > *pDn);
        }
        bool operator<(const Node& rDn) const;

        bool operator<(Node* pDn) const {
            return (*this < *pDn);
        }

        bool operator <=(const Node& rDN) const {
            if (*this == rDN) return true;
            if (*this < rDN) return true;
            return false;
        }

        bool operator <=(Node* pDN) const {
            return (*this <= *pDN);
        }

        bool operator >=(const Node& rDN) const {
            if (*this == rDN) return true;
            if (*this > rDN) return true;
            return false;
        }

        bool operator >=(Node* pDN) const {
            return (*this >= *pDN);
        }

        friend class FilePath; // Platform way to populate a Node
    };


} // stdnoj

#endif


