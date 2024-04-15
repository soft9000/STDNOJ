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
// 11/12/1998, Created OutboundEMail to support PostJOB1, R. Nagy
// 11/16/1998, Added FileVersion support, R. Nagy
// 05/17/1999, Now MUST use "Load()" and "Save()": Moved read() and write() into "protected", R. Nagy
// 09/23/2003: Ported to stdnoj, R. Nagy
// 06/17/2008: Added Load and Save for file objects, R. Nagy
//
#ifndef zOutBoundEmail_Hpp
#define zOutBoundEmail_Hpp
#include <Server/Email.hpp>
#include <Extra/FileVersion.hpp>

using namespace stdnoj;

class EmailHeader : public FileVersion {
public:

    EmailHeader(void) {
        ProductName("OBEM");
        VersionName("OutBoundEmail");
        VersionNumber(1.1);
    }
};

class OutBoundEmail {
protected:
    EmailHeader header;
    EMail email;

    ostream& write(ostream& os) {
        header.write(os);
        return email.write(os);
    }

    istream& read(istream& is) {
        header.read(is);
        return email.read(is);
    }

public:

    OutBoundEmail(void) {
    }

    OutBoundEmail(const OutBoundEmail& em) {
        *this = em;
    }

    virtual ~OutBoundEmail(void) {
    }

    bool Load(const StdString& sArticle);
    bool Save(const StdString& sArticle);
    bool Load(File& file);
    bool Save(File& file);

    operator EMail&(void) {
        return email;
    }

    operator const EMail&(void) const {
        return email;
    }

    bool IsOutBoundEmailFile(const StdString& pszfn);

    const char *From(void) const {
        return email._from.c_str();
    }

    const char *To(void) const {
        return email._to.c_str();
    }

    const char *Date(void) const {
        return email._date.c_str();
    }

    const char *Subject(void) const {
        return email._subject.c_str();
    }

    const char *Newsgroups(void) const {
        return email._newsg.c_str();
    }

    bool From(const StdString& psz) {
        email._from = psz;
        return true;
    }

    bool To(const StdString& psz) {
        email._to = psz;
        return true;
    }

    bool Date(const StdString& psz) {
        email._date = psz;
        return true;
    }

    bool Subject(const StdString& psz) {
        email._subject = psz;
        return true;
    }

    bool Newsgroups(const StdString& psz) {
        email._newsg = psz;
        return true;
    }

    bool Message(File& file);

    void Message(const StdString& psz) {
        email.Message(psz.c_str());
    }

    const char *Message(void) const {
        return email.Message();
    }

    OutBoundEmail & operator=(const OutBoundEmail& em);

    friend class SmtpServer;
};

inline bool OutBoundEmail::Message(File& file) {
    bool br = false;
    if (IsTextFile(file) == false)
        return br;
    br = email.MessagePayload(file);
    file.Close();
    return br;
}

inline bool OutBoundEmail::Load(const StdString& sArticle) {
    File file(sArticle);
    return Load(file);
}

inline bool OutBoundEmail::Load(File& file) {
    istream& is = file.OpenRead(File::AT_BINARY);
    if (!is)
        return false;
    bool br = false;
    if (read(is))
        br = true;
    file.Close();
    return br;
}

inline bool OutBoundEmail::Save(const StdString& sArticle) {
    File file(sArticle);
    return Save(file);
}

inline bool OutBoundEmail::Save(File& file) {
    file.Remove();
    write(file.OpenWrite(File::AT_BINARY));
    file.Close();
    return file.Exists();
}

#endif
