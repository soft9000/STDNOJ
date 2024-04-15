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
// Classes to encapsulate the status and data for RFC object transferrals.
//
// 02/12/1998: Classes removed from InetRfc, R. Nagy
// 01/08/1999: Added OnVerbose() to track all of those (potentially interesting) messages, R. Nagy
// 03/10/2001: Added mkwhite() after UserData.User() assignment. Have discoverd email systems permit all types of nasties therein, R. Nagy
// 08/17/2002: Added support for LocalHost on Host class to support SMTP HELO protocol, R. Nagy
// 09/23/2003: Ported to stdnoj, R. Nagy
//
#ifndef zRfcObjects_Hpp
#define zRfcObjects_Hpp

#include <stdnoj.hpp>
#include <Sockets/StdSocket.hpp>

#include <Server/OutboundEmail.hpp>
#include <Server/DayLog.hpp>

// This function translates a buffer (any buffer) over to RFC newlines
extern const char *MkRFC(StdString& sResult);

class HostName;
class UserData;
class ArticleThunk;
class NewsGroup;
class RfcStatusCode;
class RfcObjectStatus;
class Rfc822Server;

// Extern
class EMailFilter;

typedef unsigned long mail_t;

struct ServerType {
    int server_type_code;

    int none(void) {
        return 0;
    }

    int smtp(void) {
        return 1;
    }

    int ftp(void) {
        return 2;
    }

    int pop3(void) {
        return 3;
    }

    int nntp(void) {
        return 4;
    }

    ServerType(void) {
        server_type_code = none();
    }

    StdString Query(void);
};

class HostName {
protected:
    StdString sError;
    u_short port;
    StdString sHost;

public:

    HostName(void) : port(NULL) {
    }

    HostName(const StdString& std, u_short p) : port(p) {
        sHost = std;
    }

    HostName(const HostName& h) {
        (*this) = h;
    }

    ~HostName(void) {
    }

    void Name(const StdString& std) {
        sHost = std;
    }

    void Port(u_short p) {
        port = p;
    }

    const char *Name(void) const {
        return sHost.c_str();
    }

    u_short Port(void) const {
        return port;
    }

    // Now featured in dialogs;

    const char *QueryPort(StdString& str) {
        str.assign((int) port);
        return str.c_str();
    }

    void SetPort(const StdString& str) {
        port = (int) str.as_int();
    }

    void SetError(const StdString& s) {
        sError = s;
    }

    void GetError(StdString s) {
        s = sError;
    }

    int operator==(const HostName& n) {
        return (sHost == n.sHost);
    }

    HostName & operator=(const HostName& n) {
        if (this == &n) return *this;
        sHost = n.sHost;
        port = n.port;
        sError = n.sError;
        return *this;
    }

    ostream& write(ostream& os) {
        os << port << " ";
        sHost.write_stream(os);
        return os;
    }

    istream& read(istream& os) {
        os >> port;
        sHost.read_stream(os);
        return os;
    }
};

class UserData {
protected:
    StdString sError;
    StdString sUserId;
    StdString sPassword;
    StdString sLocalHost;

    StdString AuthNNTP(Rfc822Server& server);
    StdString AuthESMTP(Rfc822Server& server);
    StdString AuthPOP3(Rfc822Server& server);

    StdString sAuthCodes; // run-time only, please...
    ServerType stAuth; // (ibid)

public:

    UserData(void) {
    }

    UserData(const UserData& n) {
        (*this) = n;
    }

    UserData(const StdString& U, const StdString& P) {
        User(U);
        Pass(P);
    }

    UserData(const StdString& U, const StdString& P, const StdString& D) {
        User(U);
        Pass(P);
        Domain(D);
    }

    void SetError(const StdString& s) {
        sError = s;
    }

    void GetError(StdString s) {
        s = sError;
    }

    // Added support for ESMTP AUTH requirements on server

    void SetUserAuthorization(const StdString& str, ServerType& st) {
        sAuthCodes = str;
        stAuth = st;
    }
    StdString Authorize(Rfc822Server& server);

    // Added support for LocalHost to suport SMTP HELO protocol:

    void Domain(const StdString& std) {
        sLocalHost = std;
    }

    const char *Domain(void) const {
        return sLocalHost.c_str();
    }

    const char *User(void) {
        return sUserId.c_str();
    }

    const char *Pass(void) {
        return sPassword.c_str();
    }

    void User(const StdString& std) {
        sUserId = std;
        sUserId.mkwhite();
    }

    void Pass(const StdString& std) {
        sPassword = std;
    }

    int operator==(const UserData& n) {
        return ((sUserId == n.sUserId) && (sPassword == n.sPassword));
    }

    UserData & operator=(const UserData& h) {
        if (this == &h) return *this;
        sLocalHost = h.sLocalHost;
        sUserId = h.sUserId;
        sPassword = h.sPassword;
        sError = h.sError;
        return *this;
    }

    ostream& write(ostream& os) {
        sUserId.write_stream(os);
        sPassword.write_stream(os);
        sLocalHost.write_stream(os);
        return os;
    }

    istream& read(istream& os) {
        sUserId.read_stream(os);
        sPassword.read_stream(os);
        sLocalHost.read_stream(os);
        return os;
    }

};

class RfcStatusCode {
protected:
    int iCode;
    StdString sMessage;
public:
    RfcStatusCode(void);
    RfcStatusCode(const RfcStatusCode&);
    virtual ~RfcStatusCode(void);

    bool Assign(const StdString& std);

    int Code(void) const;
    const char *Message(void) const;

    RfcStatusCode & operator=(const RfcStatusCode&);
};

// The article thunk is a strange beastie: It can hold either the NEWNEWS "<message-id>" result
// or the numeric 'STAT/NEXT' id: Of course, both can be used with an ARTICLE-type command.
// -All we need to know is if the Number() is non-zero, to know what type is stored.

class ArticleThunk {
private:
    mail_t StatStart; // used to "walk" a list of articles in a news-group server

protected:
    StdString sError;
    mail_t number;
    StdString sArticleName;
    bool downloaded; // true if the article was downloaded

public:

    ArticleThunk(void) : number(NULL), downloaded(false) {
    }

    ArticleThunk(const ArticleThunk& at) : number(NULL), downloaded(false) {
        *this = at;
    }

    virtual ~ArticleThunk(void) {
    }

    void Error(const StdString& err) {
        sError = err;
    }

    const char* Error(void) const {
        return sError.c_str();
    }

    bool Assign(StdString& std);

    // THIS IS FOR "CALLING OUT" AN ARTICLE THAT IS GIVING US A UNIQUE DEBUG OPPORTUNITY;

    void DebugAssign(mail_t mt) {
        number = mt;
    }

    bool ArticleDownloaded(void) const {
        return downloaded;
    }

    void ArticleDownloaded(bool b) {
        downloaded = b;
    }

    const char *MessageId(void) const {
        return sArticleName.c_str();
    }

    mail_t Number(void) const {
        return number;
    }

    ArticleThunk &
            operator=(const ArticleThunk& at);

    int
    operator==(const ArticleThunk& at) {
        return (sArticleName == at.sArticleName);
    }

    ostream& write(ostream& os);
    istream& read(istream& is);

    friend class Rfc977NewsGroupServer;
};

class NewsGroup {
protected:
    char canpost;
    int okay;
    mail_t m_count;
    mail_t m_greater_than;
    mail_t m_less_than;

    mail_t m_session_count;
    mail_t m_ttotal_count;
    mail_t m_HighestRead;
    mail_t m_LowestRead;

    StdString sGroup;

    void Reset(void);

public:

    NewsGroup(void) {
        Reset();
    }

    NewsGroup(const NewsGroup& n) {
        Reset();
        (*this) = n;
    }

    NewsGroup(const StdString& which) {
        Reset();
        Name(which);
    }

    long spNEXT; // used for find first / next.

    void SearchNewSetup(const EMailFilter& rFilter);

    bool AssignFromLIST(StdString& std);
    bool AssignFromEntry(StdString& std);

    // NOTE: Start and End represent a range. Nelem represents an actual count.
    // Due to article withdrawls and pruning, the deltas are almost certainly not
    // the same.

    mail_t Nelem(void) const {
        return m_count;
    }

    mail_t HighestNumber(void) const {
        return m_greater_than;
    }

    mail_t LowestNumber(void) const {
        return m_less_than;
    }

    // The following group of counters is maintained by this event;
    void LastArticleRead(ArticleThunk& at);

    mail_t SessionYield(void) const {
        return m_session_count;
    }

    mail_t GroupYield(void) const {
        return m_ttotal_count;
    }

    mail_t HighestRead(void) const {
        return m_HighestRead;
    }

    mail_t LowestRead(void) const {
        return m_LowestRead;
    }

    void Name(const StdString& std);

    const char *Name(void) const {
        return sGroup.c_str();
    }

    // DATA will always be different, but the NAMES must always match, => we only cmp the later;

    int operator==(const NewsGroup& n) {
        return (sGroup == n.sGroup);
    }

    // Complete data assignment, of course;
    NewsGroup & operator=(const NewsGroup& n);

    ostream& write(ostream& os);
    istream& read(istream& os);
};

class LowLevelSocketStatusProbe : public StdLog {
protected:
    RfcObjectStatus *pStatusLog;

public:
    LowLevelSocketStatusProbe(RfcObjectStatus *pl);
    virtual ~LowLevelSocketStatusProbe(void);
    StdString Print(ostream& os, const StdString& psz, int LogCapture = 1);
    StdString PrintLine(ostream& os, const StdString& psz, int LogCapture = 1);
};

class RfcObjectStatus {
protected:
    StdString sLogFileName;
    bool verbose;

public:

    enum CommentType {
        ctDEFAULT,
        ctDEBUG,
        ctDRIVER,
        ctABORT,
        ctERROR
    };

    RfcObjectStatus(void) {
        sLogFileName = "StatusObject.Log";
        verbose = false;
    }

    virtual ~RfcObjectStatus(void) {
    }

    void Use(const StdLog& rcLog) {
        sLogFileName = rcLog.Name();
    }

    void Query(StdLog& rcLog) {
        rcLog.Name(sLogFileName.c_str());
    }

    StdLog *GetProbe(void) {
        return (new DayLog);
    } // LowLevelSocketStatusProbe(this));}

    virtual bool StatusMessage(const char *psz, CommentType = ctDEFAULT);

    bool StatusMessage(const StdString std, CommentType = ctDEFAULT) {
        return StatusMessage(std.c_str());
    }

    // Activation / deactivation of the search process;
    virtual void OnActivation(void);
    virtual void OnDeActivation(void);
    virtual void OnTermination(void);

    // Local resource errors;
    virtual void OnDirectoryNotFound(const StdString& sDirectoryName);

    // What to say when the user aborts the process;
    virtual void OnUserAbortProcessed(void);

    // Monitor connections;
    virtual void OnConnect(const HostName& host);
    virtual void OnConnect(const NewsGroup& group);
    virtual void OnConnectFailed(const HostName& host);
    virtual void OnConnectFailed(const NewsGroup& group);

    // Monitor retrevial of the list of available news-groups;
    virtual void OnGetNewListStart(void);
    virtual void OnGetNewListPacket(size_t block, StdString& sPacket);
    virtual void OnGetNewListPacketAbort(void);
    virtual void OnGetNewListCompleted(void);

    // Monitor the loading of the selected news-groups;
    virtual void OnNewsGroupLoadStart(void);
    virtual void OnNewsGroupLoadItem(const StdString& std);
    virtual void OnNewsGroupLoadCompleted(Array<NewsGroup>& rResult);

    // Monitor the actual downloading of articles to your machine;
    virtual void OnNewsGroupDownloadStart(Array<NewsGroup>& ngA);
    virtual void OnNewsGroupDownloadStart(NewsGroup& ng);
    virtual void OnNewsGroupDownloadStartError(NewsGroup& ng);
    virtual void OnNewsGroupDownloadCompleted(Array<NewsGroup>& ngA);
    virtual void OnNewsGroupDownloadCompleted(NewsGroup& ng);

    // Monitor as a QUALIFIED search is taking place;
    virtual void OnQualificationStart(const NewsGroup& ng, const EMailFilter& emf);
    virtual void OnQualifiedNone(const NewsGroup& ng, const EMailFilter& flt);
    virtual void OnArticleMatched(const NewsGroup& ng, const EMail& Em, const EMailFilter& emf);
    virtual void OnArticleMatched(const NewsGroup& ng, const EMail& Em);
    virtual void OnArticleRejected(const NewsGroup& ng, const EMail& Em, const EMailFilter& emf);
    virtual void OnArticleRejected(const NewsGroup& ng, const EMail& Em);

    // Monitor what happens when QUALIFIED articles are being saved to local resources;
    virtual void OnQualifiedSaveStart(const NewsGroup& ng, Array<EMail> rOut, const EMailFilter& flt);
    virtual void OnQualifiedSaveCompleted(const NewsGroup& ng, Array<EMail> rOut, const EMailFilter& flt);
    virtual void OnBadFileName(const EMail& em);
    virtual void OnRenamedFileName(const EMail& em);
    virtual void OnDuplicateFileName(const EMail& em);
    virtual void OnArticleSaveFailure(const EMail& em);
    virtual void OnArticleSaved(const EMail& em);

    // Monitor object / object collections as-they-aggregate;
    virtual void OnNewsGroups(const Array<NewsGroup>& rGroups);
    virtual void OnNewsGroupObject(const NewsGroup& ng, const ArticleThunk& aT, mail_t StatStart, int retries);
    virtual void OnLocalObject(const NewsGroup& ng, const EMail& Em); // (local search)
    virtual void OnNewsGroupObject(const NewsGroup& ng, const EMail& Em, const ArticleThunk aT);
    virtual void OnNewsGroupObjects(const NewsGroup& rGroup, Array<EMail>& rOut, const EMailFilter& flt);
    virtual void OnNewsGroupObjects(const NewsGroup& group, Array<ArticleThunk>& thunks);
    virtual void OnNewsGroupObjectsMissing(const NewsGroup& group, Array<ArticleThunk>& thunks);

    // Monitor low-level article STAT-status;
    virtual void OnGroupStatStart(const NewsGroup& ng, mail_t StatStart);
    //    virtual void OnGroupStatError(const NewsGroup& ng, const char *psz);
    virtual void OnArticleStat(const NewsGroup& ng, size_t block, int retries);
    virtual void OnError(const NewsGroup& group, ArticleThunk& thunks);
    virtual void OnError(const NewsGroup& group, RfcStatusCode& RfcCode);
    virtual void OnTimeoutDeath(const NewsGroup& group, ArticleThunk& thunk, int packet_retry_times);
    //    virtual void OnWithdrawn(const NewsGroup& group, ArticleThunk& thunk, int RfcCode, const StdString& sBuffer);

    // Monitor low-level buffer retries and bail-outs;
    virtual void OnBadArticleBuffer(const NewsGroup& ng, ArticleThunk& aThunk, EMail& em);
    virtual void OnBufferError(const NewsGroup& ng, ArticleThunk& aT, StdString& stdOut);
    virtual void OnBufferError(const NewsGroup& ng, StdString& stdOut);
    virtual void OnSocketError(StdSocket& soc);

    virtual bool ShouldAbort(void) {
        return false;
    }

    // There are legacy. Used mostly by the servers in InetRfc. Maybe we'll phase them out,
    // or phase out the redundant ones from (above);

    virtual void Got(HostName&) {
    }

    virtual void Got(UserData&) {
    }

    virtual void Got(NewsGroup&) {
    }

    virtual void Got(ArticleThunk&) {
    }

    virtual void Got(EMail&) {
    }

    virtual void OnError(const EMail& group, RfcStatusCode& RfcCode);
    virtual void OnError(const EMail& group, const StdString& RfcCode);

    // Added 01/08/1999

    void SetVerbose(bool b = true) {
        verbose = b;
    }

    bool QueryVerbose(void) {
        return verbose;
    }
    virtual void OnVerbose(const StdString& VerboseMessage);
};

#endif
