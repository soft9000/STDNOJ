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
#include <cstdio>  // sscanf();
#include <Server/RfcObjects.hpp>
#include <Server/InetRfc.hpp>
#include <Server/EmailFilter.hpp>

#include <Server/Base64.hpp>

#define BEGIN {
#define END   }

#include <Server/DayLog.hpp>

const char *MkRFC(StdString& sResult) {
    sResult.replace("\r\n", "@.n$.");
    sResult.replace("\r", "@.n$.");
    sResult.replace("@.n$.", "\r\n");
    return sResult.c_str();
}

bool RfcObjectStatus::StatusMessage(const char *psz, CommentType ct) {
    DayLog std(sLogFileName);
    return std.Write(psz);
}

RfcStatusCode::RfcStatusCode(void) : iCode(NULL) {
}

RfcStatusCode::RfcStatusCode(const RfcStatusCode& rC) {
    *this = rC;
}

RfcStatusCode::~RfcStatusCode(void) {
}

bool RfcStatusCode::Assign(const StdString& stdIn) {
    if (stdIn.length() < 5)
        return false;

    StdString std = stdIn;
    switch (std[0]) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        {
            if ((std[3] != ' ') && std[3] != '-') // 8/17/2002: Big blocks of status and documentation are also possible now (e.g: EHLO), R. Nagy
                break;
            std[3] = NULL;
            iCode = ::atoi(std.c_str());
            sMessage = &std[4];
            return true;
        }
        default:
            break;
    }
    return false;
}

int RfcStatusCode::Code(void) const {
    return iCode;
}

const char *RfcStatusCode::Message(void) const {
    return sMessage.c_str();
}

RfcStatusCode& RfcStatusCode::operator=(const RfcStatusCode& rC) {
    if (this == &rC)
        return *this;
    iCode = rC.Code();
    sMessage = rC.sMessage;
    return *this;
}

ArticleThunk& ArticleThunk::operator=(const ArticleThunk& at) {
    if (this == &at)
        return *this;
    downloaded = at.downloaded;
    sArticleName = at.sArticleName;
    number = at.number;
    return *this;
}

bool ArticleThunk::Assign(StdString& std) {
    /*
    423 Bad article number
    223 282406 <34AD2228.ADFB60DB@its.cl> status
    223 282407 <68kqnb$69c$1@brie.direct.ca> status
    223 282408 <68kqri$6c1$1@brie.direct.ca> status
    223 282408 <68kqri$6c1$1@brie.direct.ca> status
     */
    if (std[0] != '2')
        return false;

    // "snip" off the 977 response code;
    std.snip();

    // && stream it in!
    stringstream srm;
    srm << std.c_str();
    srm.seekg(0, ios::beg);
    read(srm);
    return true;
}

ostream& ArticleThunk::write(ostream& os) {
    os << number << " ";
    os << downloaded << " ";
    sArticleName.write_stream(os);
    return os;
}

istream& ArticleThunk::read(istream& is) {
    is >> number;
    char ch;
    is.read(&ch, 1);
    is >> downloaded;
    is.read(&ch, 1);
    sArticleName.read_stream(is);
    return is;
}

void NewsGroup::SearchNewSetup(const EMailFilter& rFilter) {
    switch (rFilter.Range()) {
        case EMailFilter::GET_NEW :
        case EMailFilter::SEARCH_NEW :
                    m_LowestRead = m_HighestRead;
            break;
        default:
            break;
    }
}

NewsGroup& NewsGroup::operator=(const NewsGroup& n) {
    if (this == &n)
        return *this;
    spNEXT = n.spNEXT;
    m_HighestRead = n.m_HighestRead;
    m_LowestRead = n.m_LowestRead;
    m_ttotal_count = n.m_ttotal_count;
    m_session_count = n.m_session_count;
    okay = n.okay;
    m_count = n.m_count;
    m_greater_than = n.m_greater_than;
    m_less_than = n.m_less_than;
    sGroup = n.sGroup;
    return *this;
}

void NewsGroup::Reset(void) {
    // Persistent values;
    m_HighestRead = NULL;
    m_LowestRead = NULL;
    m_ttotal_count = NULL;

    // Assigned at entry;
    m_session_count = NULL;
    canpost = NULL;
    okay = NULL;
    m_count = NULL;
    m_greater_than = NULL;
    m_less_than = NULL;
    sGroup = "";
    spNEXT = NULL;
}

void NewsGroup::Name(const StdString& std) {
    StdString std2 = std;
    size_t sp = std2.rfind('\n');
    if (sp != NPOS)
        std2.remove_pos(sp, std2.length());
    sp = std2.rfind('\r');
    if (sp != NPOS)
        std2.remove_pos(sp, std2.length());
    std2.strip();
    sGroup = std2;
}

ostream& NewsGroup::write(ostream& os) {
    os.write((char *) & m_HighestRead, sizeof (mail_t));
    os.write((char *) & m_LowestRead, sizeof (mail_t));
    os.write((char *) & m_ttotal_count, sizeof (mail_t));
    sGroup.write_stream(os);
    return os;
}

istream& NewsGroup::read(istream& is) {
    Reset();
    is.read((char *) & m_HighestRead, sizeof (mail_t));
    is.read((char *) & m_LowestRead, sizeof (mail_t));
    is.read((char *) & m_ttotal_count, sizeof (mail_t));
    sGroup.read_stream(is);
    return is;
}

void NewsGroup::LastArticleRead(ArticleThunk& at) {
    m_ttotal_count++;
    m_session_count++;
    if (at.Number() > m_HighestRead)
        m_HighestRead = at.Number();
    if (at.Number() < m_LowestRead)
        m_LowestRead = at.Number();
    if (!m_LowestRead)
        m_LowestRead = at.Number();
}

bool NewsGroup::AssignFromLIST(StdString& std) {
    /*
    LIST
    Returns a list of valid newsgroups and associated information.  Each
    newsgroup is sent as a line of text in the following format:

       group last first p

    where <group> is the name of the newsgroup, <last> is the number of
    the last known article currently in that newsgroup, <first> is the
    number of the first article currently in the newsgroup, and <p> is
    either 'y' or 'n' indicating whether posting to this newsgroup is
    allowed ('y') or prohibited ('n').

    The <first> and <last> fields will always be numeric.  They may have
    leading zeros.  If the <last> field evaluates to less than the
    <first> field, there are no articles currently on file in the
    newsgroup.
     */

    // 12-08-1998, rn (resolving MSN-format pecularities)
    std.mkwhite();
    std.strip();
    // 12-08-1998 ends

    size_t sp = std.find(' ');
    if (sp == NPOS)
        return false;

    BEGIN
    StdString std2;
    std.copy_pos(std2, 0, sp);
    Name(std2.c_str());
    END

    // group last first p
    m_count = m_less_than = m_greater_than = NULL;
    int nele = ::sscanf(&std[sp + 1], "%ld %ld %c", &m_greater_than, &m_less_than, &canpost);
    if (nele == 3) {
        // The LIST numbers are not the final word on the * of the group.
        // The estimate that you get on entry is also usually off, too.
        // In general, when ever you get a tally of the articles in a newsgroup,
        // they are only guidelines: Actual mileage *WILL* vary;
        m_count = m_greater_than - m_less_than; // "A miss is as good as a smile..." (Dr. Who)
        if (m_count < 0L) {
            /*
            If the <last> field evaluates to less than the
            <first> field, there are no articles currently on file in the
            newsgroup.
             */
            m_count = NULL;
        }
        return true;
    }
    return false;
}

bool NewsGroup::AssignFromEntry(StdString& std) {
    // Response is okay: Parse the response code into the structure;
    // 211 138 35579 35733 kc.jobs
    /*
    GROUP
    The successful selection response will return the article numbers of
    the first and last articles in the group, and an estimate of the
    number of articles on file in the group.  It is not necessary that
    the estimate be correct, although that is helpful; it must only be
    equal to or larger than the actual number of articles on file.  (Some
    implementations will actually count the number of articles on file.
    Others will just subtract first article number from last to get an
    estimate.)
     */
    int nele = ::sscanf(std.c_str(), "%d %ld %ld %ld ",
            &okay, &m_count, &m_less_than, &m_greater_than);
    if (nele != 4) {
        m_count = m_less_than = m_greater_than = NULL;
        return false;
    }

    size_t sp = std.rfind(" ");
    if (sp == NPOS)
        return false;

    BEGIN
    StdString std2;
    std.substr(std2, sp + 1);
    Name(std2.c_str());
    END

    return true;
}

LowLevelSocketStatusProbe::LowLevelSocketStatusProbe(RfcObjectStatus *pl) {
    pStatusLog = pl;
}

LowLevelSocketStatusProbe::~LowLevelSocketStatusProbe(void) {
    pStatusLog = NULL;
}

StdString LowLevelSocketStatusProbe::Print(ostream& os, const StdString& psz, int LogCapture) {
    if (pStatusLog) {
        StdString std;
        std.assign("SOCKET: ");
        std.append(psz);
        pStatusLog->StatusMessage(std.c_str(), RfcObjectStatus::ctDEBUG);
    }
    return StdLog::Print(psz);
}

StdString LowLevelSocketStatusProbe::PrintLine(ostream& os, const StdString& psz, int LogCapture) {
    if (pStatusLog) {
        StdString std;
        std.assign("SOCKET: ");
        std.append(psz);
        pStatusLog->StatusMessage(std.c_str(), RfcObjectStatus::ctDEBUG);
    }
    return StdLog::PrintLine(psz);
}

void RfcObjectStatus::OnActivation(void) {
    StatusMessage("News Robot Activated.");
}

void RfcObjectStatus::OnDeActivation(void) {
    StatusMessage("News Robot Deactivated.");
}

void RfcObjectStatus::OnTermination(void) {
    StatusMessage("News Robot Terminated.");
}

void RfcObjectStatus::OnGetNewListStart(void) {
    StatusMessage("Downloading new newsgroup list.");
}

void RfcObjectStatus::OnGetNewListPacket(size_t block, StdString& sData) {
    StdString str;
    str << "Listing package # " << block << " contains # " << sData.tally('\n');
    StatusMessage(str, RfcObjectStatus::ctDEBUG);
}

void RfcObjectStatus::OnGetNewListPacketAbort(void) {
    StatusMessage("! ERROR: Too many timeouts. Newsgroup list download aborted!", RfcObjectStatus::ctERROR);
}

void RfcObjectStatus::OnGetNewListCompleted(void) {
    StatusMessage("New newsgroup list retrieved.");
}

void RfcObjectStatus::OnNewsGroupDownloadStart(Array<NewsGroup>& ngA) {
    stringstream srm;
    srm << "= Searching [";
    srm << ngA.Nelem() << "]";
    srm << " news groups.";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnNewsGroupDownloadCompleted(Array<NewsGroup>& ngA) {
    mail_t grand = NULL;
    for (size_t ss = NULL; ss < ngA.Nelem(); ss++)
        grand += ngA[ss].SessionYield();
    stringstream srm;
    srm << "= Examined a total of " << grand << " articles from [" << ngA.Nelem() << "] news groups.";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnNewsGroupDownloadStart(NewsGroup& ng) {
    stringstream srm;
    srm << "===== " << ng.Name() << " =====";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnNewsGroupDownloadStartError(NewsGroup& ng) {
    stringstream srm;
    srm << "! ERROR " << ng.Name() << ": Unable to locate news-group.";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnNewsGroupDownloadCompleted(NewsGroup& ng) {
    stringstream srm;
    srm << "= Examined " << ng.SessionYield() << " articles from " << ng.Name();
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnNewsGroupLoadStart(void) {
    StatusMessage("Extracting desired newsgroups...");
}

void RfcObjectStatus::OnNewsGroupLoadItem(const StdString& std) {
}

void RfcObjectStatus::OnNewsGroupLoadCompleted(Array<NewsGroup>& rResult) {
    stringstream srm;
    srm << "Articles extracted. Found " << rResult.Nelem() << " matching news-groups.";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnQualificationStart(const NewsGroup& ng, const EMailFilter& emf) {
    /*
    if(emf.Type() == st_DEFAULT)
    StatusMessage(" = Matching ALL articles.");
    else
    StatusMessage(" = Matching qualified articles.");
     */
}

void RfcObjectStatus::OnQualifiedNone(const NewsGroup& ng, const EMailFilter& flt) {
    stringstream srm;
    srm << ng.Name();
    srm << ": No articles qualified.";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnQualifiedSaveStart(const NewsGroup& ng, Array<EMail> rOut, const EMailFilter& flt) {
    StdString std;
    stringstream srm;
    srm << "SAVING: ";
    srm << std.c_str() << " GROUP ";
    srm << ng.Name() << " has ";
    srm << rOut.Nelem() << " qualified news-articles.";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnQualifiedSaveCompleted(const NewsGroup& ng, Array<EMail> rOut, const EMailFilter& flt) {
    stringstream srm;
    srm << "List extracted. Found " << rOut.Nelem() << " qualified news-articles.";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnBadFileName(const EMail& em) {
    stringstream srm;
    srm << "! ERROR: Article " << em.GetLocalName() << " cannot be converted to WIN95 filename.";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str(), RfcObjectStatus::ctERROR);
}

void RfcObjectStatus::OnRenamedFileName(const EMail& em) {
    StatusMessage("*** Subject of file exists: Article renamed.", RfcObjectStatus::ctDEFAULT);
    stringstream srm;
    srm << "*** \"" << em.GetLocalName() << "\"";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str(), RfcObjectStatus::ctDEFAULT);
}

void RfcObjectStatus::OnDuplicateFileName(const EMail& em) {
    StatusMessage("*** Article already downloaded (SKIPPED)", RfcObjectStatus::ctDEFAULT);
    stringstream srm;
    srm << "*** \"" << em.GetLocalName() << "\"";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str(), RfcObjectStatus::ctDEFAULT);
}

void RfcObjectStatus::OnArticleSaveFailure(const EMail& em) {
    stringstream srm;
    srm << "! ERROR: Article " << em.GetLocalName() << " was not written to disk!";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str(), RfcObjectStatus::ctERROR);
}

void RfcObjectStatus::OnArticleSaved(const EMail& em) {
    stringstream srm;
    srm << "Article " << em.GetLocalName() << " written to disk.";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str(), RfcObjectStatus::ctDEFAULT);
}

void RfcObjectStatus::OnNewsGroups(const Array<NewsGroup>& rGroups) {
    stringstream srm;
    srm << "SEARCH: " << rGroups.Nelem() << " groups. ";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnNewsGroupObjects(const NewsGroup& rGroup, Array<EMail>& rOut, const EMailFilter& flt) {
}

void RfcObjectStatus::OnConnectFailed(const HostName& host) {
    stringstream srm;
    srm << "! ERROR: Unable to connect to `" << host.Name() << "`";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str(), RfcObjectStatus::ctERROR);
}

void RfcObjectStatus::OnConnect(const HostName& host) {
}

void RfcObjectStatus::OnConnectFailed(const NewsGroup& group) {
    stringstream srm;
    srm << "! ERROR: Unable to connect to `" << group.Name() << "`";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str(), RfcObjectStatus::ctERROR);
}

void RfcObjectStatus::OnConnect(const NewsGroup& group) {
    stringstream srm;
    srm << "GROUP: Connected to " << group.Name() << ". Extracting article headers (\"thunks\").";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnNewsGroupObject(const NewsGroup& ng, const ArticleThunk& aT, mail_t StatStart, int retries) {
    stringstream srm;
    srm << "Located Article #" << StatStart;
    if (retries)
        srm << ", retry = " << retries;
    srm << ends;
    StatusMessage(PRESTO(srm).c_str(), RfcObjectStatus::ctDEBUG);
}

void RfcObjectStatus::OnLocalObject(const NewsGroup& ng, const EMail& Em) {
    StdString std;
    stringstream srm;
    srm << "Opened article ";
    srm << Em.MkLocalName(std);
    srm << ends;
    StatusMessage(PRESTO(srm).c_str(), RfcObjectStatus::ctDEBUG);
}

void RfcObjectStatus::OnNewsGroupObject(const NewsGroup& ng, const EMail& Em, const ArticleThunk aT) {
    stringstream srm;
    srm << "Opened article #";
    srm << aT.Number() << ".";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str(), RfcObjectStatus::ctDEBUG);
}

void RfcObjectStatus::OnArticleMatched(const NewsGroup& ng, const EMail& Em) {
    StdString std;
    std << "* MATCHED  ";
    std << " article ";
    std << Em.MessageID();
    StatusMessage(std);
}

void RfcObjectStatus::OnArticleRejected(const NewsGroup& ng, const EMail& Em) {
    StdString std;
    std << "! ERROR  ";
    std << " article ";
    std << Em.MessageID();
    StatusMessage(std);
}

void RfcObjectStatus::OnArticleMatched(const NewsGroup& ng, const EMail& Em, const EMailFilter& emf) {
    StdString std;
    std << "* MATCHED  ";
    std << " article ";
    std << Em.MessageID();
    StatusMessage(std);
}

void RfcObjectStatus::OnArticleRejected(const NewsGroup& ng, const EMail& Em, const EMailFilter& emf) {
    StdString std;
    std << "> Rejected ";
    std << " article ";
    std << Em.MessageID();
    StatusMessage(std);
}

void RfcObjectStatus::OnNewsGroupObjectsMissing(const NewsGroup& group, Array<ArticleThunk>& thunks) {
    StatusMessage("! ERROR: No directory of articles available: Try again later.", RfcObjectStatus::ctERROR);
}

void RfcObjectStatus::OnNewsGroupObjects(const NewsGroup& group, Array<ArticleThunk>& thunks) {
    stringstream srm;
    srm << "GROUP: " << group.Name() << ", " << thunks.Nelem() << " thunks to articles.";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnGroupStatStart(const NewsGroup& ng, mail_t StatStart) {
    BEGIN
    stringstream srm;
    srm << ng.Name();
    srm << ": " << (ng.HighestNumber() - ng.LowestNumber() + 1) << " postings. From ";
    srm << ng.LowestNumber() << " to " << ng.HighestNumber() << ".";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
    END
    BEGIN
    stringstream srm;
    srm << "Starting at " << StatStart << ".";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
    END
}

/*
void RfcObjectStatus::OnGroupStatError(const NewsGroup& ng, const char *psz)
   {
   StatusMessage("! ERROR: Group Status (buffer follows)", RfcObjectStatus::ctERROR);
   StatusMessage(psz, RfcObjectStatus::ctERROR);
   }
 */
void RfcObjectStatus::OnUserAbortProcessed(void) {
    StatusMessage("News Robot Aborted.", RfcObjectStatus::ctABORT);
}

void RfcObjectStatus::OnArticleStat(const NewsGroup& ng, size_t block, int retries) {
    stringstream srm;
    srm << "Article count " << block << " retry=" << retries;
    srm << ends;
    StatusMessage(PRESTO(srm).c_str(), RfcObjectStatus::ctDEBUG);
}

void RfcObjectStatus::OnError(const NewsGroup& group, RfcStatusCode& RfcCode) {
    stringstream srm;
    srm << "! ERROR: Server reports " << RfcCode.Message();
    srm << " (" << RfcCode.Code() << ").";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnError(const NewsGroup& group, ArticleThunk& thunk) {
    stringstream srm;
    srm << "! ERROR: On thunk " << thunk.Number();
    srm << ends;
    StatusMessage(PRESTO(srm).c_str(), RfcObjectStatus::ctDEBUG);
}

void RfcObjectStatus::OnBufferError(const NewsGroup& ng, ArticleThunk& aT, StdString& stdOut) {
    StdString std;
    std << "! ERROR: Bad-buffer result (ARTICLE " << aT.Number() << ").";
    StatusMessage(std, RfcObjectStatus::ctERROR);
    StatusMessage(stdOut, RfcObjectStatus::ctDEBUG);
}

void RfcObjectStatus::OnBufferError(const NewsGroup& ng, StdString& stdOut) {
    StdString std;
    std << "! ERROR: Bad-buffer result.";
    StatusMessage(std, RfcObjectStatus::ctERROR);
    StatusMessage(stdOut, RfcObjectStatus::ctDEBUG);
}

void RfcObjectStatus::OnBadArticleBuffer(const NewsGroup& ng, ArticleThunk& aThunk, EMail& em) {
    StdString std;
    BEGIN
    stringstream srm;
    srm << "! ERROR (RFC_ERROR): Buffer for article ";
    srm << aThunk.Number() << " is not a valid message (RFC822).";
    srm << ends;
    std = PRESTO(srm).c_str();
    END
    StatusMessage(std, RfcObjectStatus::ctERROR);
}

void RfcObjectStatus::OnSocketError(StdSocket& soc) {
    StatusMessage(soc.GetLastError(), RfcObjectStatus::ctERROR);
}

void RfcObjectStatus::OnTimeoutDeath(const NewsGroup& group, ArticleThunk& thunk, int packet_retry_times) {
    StdString std;
    BEGIN
    stringstream srm;
    srm << "! ERROR (TIMEOUT): " << group.Name() << " ARTICLE " << thunk.Number();
    srm << ends;
    std = PRESTO(srm).c_str();
    END
    StatusMessage(std, RfcObjectStatus::ctERROR);
}

void RfcObjectStatus::OnDirectoryNotFound(const StdString& sDir) {
    StdString std;
    BEGIN
    stringstream srm;
    srm << "! ERROR: Unable to locate directory \"" << sDir.c_str() << "\".";
    srm << ends;
    std = PRESTO(srm).c_str();
    END
    StatusMessage(std, RfcObjectStatus::ctERROR);
}

void RfcObjectStatus::OnError(const EMail& group, RfcStatusCode& RfcCode) {
    stringstream srm;
    srm << "! ERROR: Server reports " << RfcCode.Message();
    srm << " (" << RfcCode.Code() << ").";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnError(const EMail& group, const StdString& sMessage) {
    stringstream srm;
    srm << "! ERROR: Process reports " << sMessage;
    srm << " (service terminated)";
    srm << ends;
    StatusMessage(PRESTO(srm).c_str());
}

void RfcObjectStatus::OnVerbose(const StdString& sMessage) {
    if (QueryVerbose()) {
        stringstream srm;
        srm << "! VERBOSE: Process reports " << sMessage;
        srm << ends;
        StatusMessage(PRESTO(srm).c_str());
    }
}

StdString ServerType::Query(void) {
    StdString sResult;
    switch (server_type_code) {
        case 4:
            sResult = "NNTP";
            break;
        case 3:
            sResult = "POP3";
            break;
        case 2:
            sResult = "FTP";
            break;
        case 1:
            sResult = "SMTP";
            break;
        case 0:
            sResult = "NONE";
            break;
        default:
            sResult = "[UNKNOWN]";
            break;
    }
    return sResult;
}

StdString UserData::AuthPOP3(Rfc822Server& server) {
    bool br = false;
    StdString sResult;

    // *****
    // See if "login" is required for NNTP:
    // *****
    // (This works fine with FTP strings like "user rnagy", "pass xxxxxxxx" (literally!));
    if ((*(User())) && (*(Pass()))) {
        /*
           ... 
           The POP3 session is now in the AUTHORIZATION state.  The client must
           now identify and authenticate itself to the POP3 server.  Two
           possible mechanisms for doing this are described in theis document,
           the USER and PASS command combination and the APOP command.  Both
           mechanisms are described later in this document.  Additional
           authentication mechanisms are described in [RFC1734].  While there is
           no single authentication mechanism that is required of all POP3
           servers, a POP3 server must of course support at least one
           authentication mechanism.
         */
        StdString std = User();
        std.append("\r\n");
        br = server.Transact(std, std);
        if (br == false) {
            sResult = std;
            return sResult;
        }
        if (std.find("+OK") == NPOS) {
            // Try the "APOP" style;
            std = "APOP ";
            std.append(User());
            std.append(" ");
            std.append(Pass());
            std.append("\r\n");
            br = server.Transact(std, std);
            if (std.find("+OK") == NPOS) {
                sResult = std;
                return sResult;
            }
        } else {
            std = Pass();
            std.append("\r\n");
            br = server.Transact(std, std);
            if (br == false) {
                sResult = std;
                return sResult;
            }
            if (std.find("+OK") == NPOS) {
                sResult = std;
                return sResult;
            }
        }

    }
    // Okay! Indicate success.
    sResult = "";
    return sResult;
}

StdString UserData::AuthNNTP(Rfc822Server& server) {
    StdString sResult;
    StdString std;
    if ((*(User())) && (*(Pass()))) {
        // *****
        // POST BANNER-DISPLAY PROCESSING:
        // *****

        // This is for the OLDER "TTY" login;
        // -----
        // First we inject the USERID. If we get an RFC code back, then authorization
        // is at the AUTHINFO level, else we're using a traditional TTY-type of
        // "log(on/in): / Password:"
        std = User();
        std.append("\r\n");
        if (server.Transact(std, std) == false) {
            sResult = std;
            return sResult;
        }

        RfcStatusCode rfc;
        if (rfc.Assign(std) == false) {
            // If we DID NOT get an RFC code back, then we need to send the pasword;
            std = Pass();
            std.append("\r\n");
            if (server.Transact(std, std) == false) {
                sResult = std;
                return sResult;
            }
        } else {
            // std = "LIST EXTENSIONS\r\n";     // (cerca 1996)
            // br = TransactRfcOrDottedResponse(std, std);
            // GOT: "480 Authentication required for command"

            // This is for the NEWER AUTHINFO process (see RFC 1049)
            // -----
            std = "AUTHINFO USER ";
            std.append(User());
            std.append("\r\n");
            server.Transact(std, std);

            if (rfc.Assign(std) == true) // Returns a "381 PASS required"
            {
                std = "AUTHINFO PASS ";
                std.append(Pass());
                std.append("\r\n");
                if (server.Transact(std, std) == false) {
                    sResult = std;
                    return sResult;
                }
                // Q: When to use "AUTHINFO GENERIC" (see RFC 1049)?
            } else {
                // This is here for DOCUMENTATION PURPOSES;
                // -----
                std = "HELP\r\n";
                server.Transact(std, std);
                sResult = std;
                return sResult;
            }
        }
    } else
        sResult = "";

    return sResult;
}

StdString UserData::AuthESMTP(Rfc822Server& server) {
    StdString sResult;
    if (sAuthCodes.find("LOGIN ") != NPOS) {
        if (server.TransactCodeLevel("AUTH LOGIN\r\n", sResult, 3) == true) {
            Base64 b64;
            unsigned final_size = NULL;
            if (b64.Encode(sUserId, sResult) != true) {
                sResult = "Unable to encode user_id";
                return sResult;
            }
            sResult.append("\r\n");
            if (server.TransactCodeLevel(sResult, sResult, 3) == false)
                return sResult;
            if (b64.Encode(sPassword, sResult) != true) {
                sResult = "Unable to encode user_password";
                return sResult;
            }
            sResult.append("\r\n");
            if (server.TransactCodeLevel(sResult, sResult, 2) == false)
                return sResult;

            sResult = "";
            return sResult; // success
        }
    }
    sResult = "Unsupported ESMTP authorization scheme [";
    sResult.append(sAuthCodes);
    sResult.append("]");
    return sResult;
}

StdString UserData::Authorize(Rfc822Server& server) {
    if (server.ShouldShowLogon())
        server.SocketTraceOn();
    StdString sResult; // is null on success...
    if (stAuth.server_type_code == stAuth.smtp()) {
        sResult = AuthESMTP(server);
        if (server.ShouldShowLogon())
            server.SocketTraceOff();
        return sResult;
    }
    if (stAuth.server_type_code == stAuth.nntp()) {
        sResult = AuthNNTP(server);
        if (server.ShouldShowLogon())
            server.SocketTraceOff();
        return sResult;
    }
    if (stAuth.server_type_code == stAuth.pop3()) {
        sResult = AuthPOP3(server);
        if (server.ShouldShowLogon())
            server.SocketTraceOff();
        return sResult;
    }
    if (server.ShouldShowLogon())
        server.SocketTraceOff();
    sResult = "Unable to locate authorization scheme for server type.";
    return sResult;
}



