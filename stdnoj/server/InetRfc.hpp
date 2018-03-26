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
// Internet classes. Designed specifically to work with observed and documented RFC's.
// (c) 1998 by R. A. Nagy.
//
// 02/10/1998: Added the bShouldAbort members to support on-demand abortions.
// 02/12/1998: Created and implemented the StatusObject for better status reporting, R. Nagy
// 12/03/1998: Added PacketTimeout() and 'l_timeout` to reduce the 5 .. 8 minutes wait for BAILOUT, R. Nagy
// 01/05/1999: Added t_GetAtMost(,, NELEM) to get <= AN EXACT COUNT from a group, R. Nagy
// 04/30/1999: Added Post(), R. Nagy
// 01/08/2000: Added Set/QueryVerbose(), R. Nagy
// 03/06/2000: Fixed isdotmagic() (re-wrote around a context bug in .rfind(pos)).
//             Also added the 'replace' parameter to same, R. Nagy
// 08/17/2002: Added support for RFC821 HELO protocol, R. Nagy
//             Added support for SMTP Login (i.e: promoted && re-used NNTP logic), R. Nagy
// 09/23/2003: Ported to stdnoj, R. Nagy
//
#ifndef InetRfc_Hpp
#define InetRfc_Hpp

#include <Server/RfcObjects.hpp>
#include <Server/EmailFilter.hpp>
#include <Server/EmailFactory.hpp>

#define USE_COMMENT

using namespace stdnoj;

// This defines a way to get articles from a stream. It insures and parses
// CR/LF commands.

class Rfc822Server {
private:
    long l_timeout;
    int iShowLogon;
    StdString sMagicBuffer; // Exclusively used by isdotmagic()

protected:
    StdSocket soc;

    RfcObjectStatus *pStat;
    /*
          void  PacketTimeout(long l);     // Packet
          long  PacketTimeout(void);
     */
    bool Put(const StdString& std);
    bool Get(StdString& str);
    bool More(void);

    bool isdotmagic(StdString& strL, char terminal_dot_replacement = ' ');

    // Commands are assumed to produce a contiguous response (i.e: NOTE: a 'MORE' is never sent);
    virtual bool TransactRfcOrDottedResponse(const StdString& sCmd, StdString& sResult);

public:
    Rfc822Server(void);
    virtual ~Rfc822Server(void);

    bool Transact(const StdString& stdIn, StdString& stdOut);

    // CodeLevel returns true is RFC result is same LEVEL (typically 1, 2, 3, 4, or 5)...
    bool TransactCodeLevel(const StdString& std, StdString& sResult, int RFC_code_expected = 200);

    // CodeSpecific returns true is RFC result is exact same code...
    bool TransactCodeSpecific(const StdString& std, StdString& sResult, int RFC_code_expected);

    // Tell the default RefObject to show all of those protocol state messages sent to it's
    // RfcObject::OnVerbose() member;

    void SetVerbose(bool b = true) {
        pStat->SetVerbose(b);
    }

    bool QueryVerbose(void) {
        return pStat->QueryVerbose();
    }

    // Mostly for end-user debugging of their authentication process.

    void ShowLogon(void) {
        iShowLogon = 1;
    }

    void DontShowLogon(void) {
        iShowLogon = NULL;
    }

    int ShouldShowLogon(void) {
        return iShowLogon;
    }

    virtual bool Connect(HostName&);
    virtual void Disconnect(HostName&);

    bool Use(RfcObjectStatus *pLog); // OWNERSHIP TRANSFERS IN: (do not delete pointer)
    bool Use(const StdLog& rLog);

    bool SocketLogFileName(const char *pszLogFile);
    void SocketLogFileName(void);

    void SocketTraceOn(void);
    void SocketTraceOn(StdLog *pTrace); // OWNERSHIP TRANSFERS IN: (do not delete pointer)
    void SocketTraceOff(void);

    void LastError(StdString& std);
    virtual void MonitorConnection(ostream& os, istream& is);

#ifdef USE_COMMENT
    bool Comment(const char *psz);
    bool Comment(const StdString& std);
#endif
};

class Rfc977NewsGroupServer : public Rfc822Server {
protected:
    bool br;

private:
    StdString std;
    NewsGroup pwNewsGroup;

    bool GetMagicBuffer(NewsGroup& ng, ArticleThunk& at, stringstream& sResult);

protected:
    StdString sServer;
    StdString sGroup;

    // Sometimes you need them from the most resent to the oldest. Other times,
    // the other way 'round makes the most sense. These functions work both ways;      
    bool GetFirstThunk(NewsGroup&, ArticleThunk&, bool bForward = false);
    bool GetNextThunk(NewsGroup&, ArticleThunk&, bool bForward = false);

public:

    Rfc977NewsGroupServer(void) {
    }

    virtual ~Rfc977NewsGroupServer(void) {
    }

    virtual bool Connect(HostName& hn) {
        return Rfc822Server::Connect(hn);
    }

    virtual void Disconnect(HostName& hn) {
        Rfc822Server::Disconnect(hn);
    }

    virtual bool Connect(UserData&);
    virtual void Disconnect(UserData&);

    virtual bool Connect(NewsGroup&);
    virtual void Disconnect(NewsGroup&);

    // Collect the article thunks
    bool Get(NewsGroup& ng, Array<ArticleThunk>& rResult, mail_t StatStart = NULL);

    // Get the articles, via a filter. Results are tallied by the objects, themselves;
    // Returns false if process was aborted.
    bool Get(NewsGroup&, Array<EMail>& raOut, Array<ArticleThunk>& raIn, EMailFilter *pQ = NULL);

    // Get a single article
    bool Get(NewsGroup&, EMail& rOut, ArticleThunk& rIn);

    virtual bool Post(const EMail&, StdString& sError);

    virtual void MonitorConnection(ostream& os, istream& is) {
        Rfc822Server::MonitorConnection(os, is);
    }
};

class SmtpServer : public Rfc822Server {
protected:
    UserData ConnectedUser;

public:
    virtual bool Connect(HostName& hn, UserData& ud);
    virtual void Disconnect(HostName& hn);

    virtual bool Connect(UserData& ud);
    virtual void Disconnect(UserData&);

    virtual bool Post(const EMail&, StdString& sError);
    virtual bool Post(const OutBoundEmail&, StdString& sError);
};


// Unlike news-group postings, articles in a POP3 account can be deleted PERMINATELY.
// For this reason, special care is given to these articles to insure that if the
// session terminates abnormally, we can recover WITHOUT DATA LOASS. The Pop3Batch
// is the "wrapper" that we use to ensure that data is either (1) on the server,
// or (2) on the local hard drive. WORST CASE is that items are duplicated during
// disaster recovery, but, hey, that's why God created the "EMail::operator=()" !

class Pop3Batch {
private:
    bool bShouldDelete;
    size_t ssNext;

    bool SaveRecoveryFile(void);
    bool ShouldRecover(void);
    bool LoadRecoveryFile(void);

    bool Append(EMail&);
    EMail * Recall(EMail *pem = NULL);

    void Error(const StdString& std);

    time_t error_file_time;
    time_t email_file_time;

    StdString sOutputDirectory;

public: // 06/16/2008, R. Nagy
    Array<StreamString> rErrorArray;
    Array<EMail> rArray;

public:
    Pop3Batch(void);
    virtual ~Pop3Batch(void);

    // EMail saved via array.
    // Errors "     "    "  .
    // Returns NPOS on error, else number of items saved;
    size_t SaveArrays(void);
    bool LoadArray(Array<EMail>& rArray);
    bool LoadArray(Array<StreamString>& rArray);
    bool ReplaceArray(Array<EMail>& rArray);
    bool ReplaceArray(Array<StreamString>& rArray);
    bool HaveArraysChanged(void);

    // Each EMail saved using "SaveBin" under a temp name.
    // Errors saved in "LoadArray" format (above).
    // Returns NPOS on error, else number of items saved;
    size_t Save(void);

    // Each EMail saved using "Dump" under a temp name.
    // Errors saved in "LoadArray" format (above).
    // Returns NPOS on error, else number of items saved;
    size_t Dump(const StdString& sExtension = ".TXT");

    // Like the above, but overrides and replaces the output directory for
    // the entire instance ...
    size_t Export(Directory& dir, const StdString& sExtension = ".TXT");

    // After files are downloaded and BEFORE they are removed from the server,
    // a "hardened" copy of the EMail items is saved. To stop the automatic
    // recovery of the SAVED file, you *MUST* call the following member AFTER
    // you have successfully copied the files from your BATCH instance into 
    // another storage facility;
    void BatchCompleted(void);

    size_t NumInBatch(void) {
        return rArray.Nelem();
    }

    size_t Nelem(void) {
        return rArray.Nelem();
    }

    bool AutoRemoveHostArticles(void);

    void AutoRemoveHostArticles(bool b) {
        bShouldDelete = b;
    }


    bool HadErrors(void);
    void RecallErrors(StdString& sCrLfErrorList);

    friend class Pop3Server;
};

class Pop3Server : public Rfc822Server {
protected:
   bool DeleteArticle(size_t which);
   bool StatArticles(size_t&);
   bool GetMagicBuffer(size_t which, StdString& sResult);

   // 2009/02/11 - As pulled from the FastJob class update -
   RfcObjectStatus zStat;
   // 2009/02/11 - As pulled from the FastJob class update -
   struct PopId
    {
    size_t szPos;
    PopId(void) : szPos(NPOS) {}
    PopId(size_t sz) : szPos(sz) {}
    };
   // 2009/02/11 - As pulled from the FastJob class update -
   bool RemoveFromServer(Array<PopId>& array);

public:

    virtual bool Connect(HostName& hn) {
        return Rfc822Server::Connect(hn);
    }

    virtual void Disconnect(HostName& hn) {
        Rfc822Server::Disconnect(hn);
    }

    virtual bool Connect(UserData&);
    virtual void Disconnect(UserData&);

    virtual bool Get(Pop3Batch&);
    virtual bool RemoveFromServer(EMail&);

    // 2009/02/11 - As pulled from the FastJob class update -
    bool Get(Directory& dirSave, const StdString& sType, bool bRemoveFromServer);
};






#ifdef USE_COMMENT

inline bool Rfc822Server::Comment(const char *psz) {
    return pStat->StatusMessage(psz);
}

inline bool Rfc822Server::Comment(const StdString& std) {
    return pStat->StatusMessage(std.c_str());
}
#endif

#endif
