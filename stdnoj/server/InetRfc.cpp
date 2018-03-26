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
#include <Server/InetRfc.hpp>
#include <cstdio>             // sscanf()

const char *POP3_RECOVERY_FILE = "zPop3Batch.Recovery";

const char *POP3_BATCH_FN = "zPop3Batch.ARY";
const char *POP3_BATCH_ERROR_FN = "zDownloadErrors.ARY";

// This is used to check article packets being hung-up on an "MORE/MORE".
// --Very usefull for general article query/resonse error tracking;
//#define MOO_MOO_CHECK 1
// This is used to "show" the logon process;


/*
RFC 977
=======
The first digit of the response broadly indicates the success, failure, or progress of the previous 
command.

1xx   Informative message 
2xx   Command ok
3xx   Command ok so far, send the rest of it. 
4xx   Command was correct, but couldn't be performed for some reason.
5xx   Command unimplemented, or incorrect, or a serious program error occurred.

The next digit in the code indicates the function response category. 

x0x   Connection, setup, and miscellaneous messages
x1x   Newsgroup selection
x2x   Article selection
x3x   Distribution functions
x4x   Posting
x8x   Nonstandard (private implementation) extensions
x9x   Debugging output

The exact response codes that should be expected from each command are detailed in the description of
that command.
 */

#define BEGIN  {
#define END    }

const char *MORE_STRING = "\r\n";

Rfc822Server::Rfc822Server(void) : pStat(new RfcObjectStatus), l_timeout(SOC_TIMEOUT), iShowLogon(NULL) {
}

Rfc822Server::~Rfc822Server(void) {
    delete pStat;
}

/*
void  Rfc822Server::PacketTimeout(long l)
   {
   l_timeout = l;
   }
long  Rfc822Server::PacketTimeout(void)
   {
   return l_timeout;
   }
 */
bool Rfc822Server::isdotmagic(StdString& strL, char replace) {
    // If the string is empty, then we have a problem;
    if (!strL.length())
        return false;
    /*
 Text is sent only after a numeric status response line has been sent that indicates that text will
 follow. Text is sent as a series of successive lines of textual matter, each terminated with CR-LF
 pair. A single line containing only a period (.) is sent to indicate the end of the text (i.e., the
 server will send a CR-LF pair at the end of the last line of text, a period, and another CR-LF pair).

 If the text contained a period as the first character of the text line in the original, that first
 period is doubled. Therefore, the client must examine the first character of each line received, and
 for those beginning with a period, determine either that this is the end of the text or whether to
 collapse the doubled period to a single one.

 The intention is that text messages will usually be displayed on the user's terminal whereas
 command/status responses will be interpreted by the client program before any possible display
 is done.
     */
    if (strL.length() < 2)
        return false;

    if ((strL[0] == '.') && (strL[1] == '\r')) {
        // Remove the darn thing;
        strL[0] = replace;
        return true; // What else could it be?
    }

    // *******
    // WARNING
    // *******
    // From here on out, there is a five (5) byte terminaiton sequence, no matter 
    // where it is found. It would fine to simply;
    /*
    long sp = strL.find("\r\n.\r\n");
    if(sp != npos)
       {
       // Remove the darn thing;
       strL[sp+2] = replace;
       return true;
       }
     */
    // but, the problem IS that when we have an article wint an ILLEGAL 
    // (non-doubled) period-newline sequence, a routine would detect it as a 
    // "terminal" (a common problem with email content, but not with NNTP 
    // servers (etc)). 
    //
    // On an interesting vector, I have discovered that the only *ABSOLUTELY* 
    // affermitave way to detect a terminal sequence (due to transmission errors, 
    // transmission padding, etc) is to permit a subsequent time-out after this 
    // routine returns true, but that's neither RFC nor implemented. Indeed, as
    // our news-reader and list server base worked this way for a season, this
    // procedure is tried-and-true... But it wastes significant time at the
    // moment.
    //
    // OUR COMPROMISE is, as it was, to only check the trailing five bytes.
    // We'll assume that modern modem technologies are handling the transmission 
    // errors enough to allow for this: Note that this approach "rareifies" the 
    // ILLEGAL imbedding of a terminal, as well as honors the letter of the RFC's;
    //
    if (strL.length() < 5)
        return false;

    StdString sck = &strL[strL.length() - 5];
    if (sck.find("\r\n.\r\n") != npos) {
        // Remove the darn thing;
        strL[strL.length() - 3] = replace;
        return true;
    }

    return false;
}

void Rfc822Server::SocketTraceOn(StdLog *pTrace) {
    delete soc.Trace(pTrace);
}

void Rfc822Server::SocketTraceOn(void) {
    if (pStat)
        delete soc.Trace(pStat->GetProbe());
}

void Rfc822Server::SocketTraceOff(void) {
    delete soc.Trace(NULL);
}

bool Rfc822Server::SocketLogFileName(const char *pszLogFile) {
    StdLog *sl = new StdLog;
    if (sl->Name(pszLogFile) == false)
        return false;

    delete soc.Trace(sl);
    return true;
}

void Rfc822Server::SocketLogFileName(void) {
    delete soc.Trace(NULL);
}

bool Rfc822Server::Use(RfcObjectStatus *pLog) {
    if (pLog) {
        bool bv = QueryVerbose();
        delete pStat;
        pStat = pLog;
        SetVerbose(bv);
        return true;
    }
    return false;
}

bool Rfc822Server::Use(const StdLog& rLog) {
    if (pStat) {
        pStat->Use(rLog);
        return true;
    }
    return false;
}

bool Rfc822Server::Put(const StdString& std) {
    return soc.Send(std, l_timeout);
}

bool Rfc822Server::Get(StdString& std) {
    // If it's a "MORE", then adjust the time-out to keep us from waiting
    // till we freak-out;
    if (std[0] == '\r')
        return soc.Receive(std, SOC_BUFFER_SZ, 15L);
    // Else. doittoit
    return soc.Receive(std, SOC_BUFFER_SZ, l_timeout);
}

bool Rfc822Server::More(void) {
    return soc.Send(MORE_STRING);
}

bool Rfc822Server::TransactCodeLevel(const StdString& std, StdString& sResult, int RFC_code) {
    if (Transact(std, sResult) == false)
        return false;
    RfcStatusCode RfcCode;
    if (RfcCode.Assign(sResult) == false)
        return false;
    if ((RfcCode.Code() / 100) != RFC_code)
        return false;
    return true;
}

bool Rfc822Server::TransactCodeSpecific(const StdString& std, StdString& sResult, int RFC_code) {
    if (Transact(std, sResult) == false)
        return false;
    RfcStatusCode RfcCode;
    if (RfcCode.Assign(sResult) == false)
        return false;
    if (RfcCode.Code() != RFC_code)
        return false;
    return true;
}

bool Rfc822Server::Transact(const StdString& stdIn, StdString& stdOut) {
    return soc.TransactResponseContaining(stdIn, stdOut, '\n', l_timeout);
}

bool Rfc822Server::Connect(HostName& host) {
    if (ShouldShowLogon())
        SocketTraceOn();

    // connect the nntp host;
    if (soc.Connect(host.Name(), host.Port()) == false) {
        StdString sError;
        sError = soc.GetLastError();
        host.SetError(sError);

        pStat->OnConnectFailed(host);
        return false;
    }
    pStat->OnConnect(host);

    StdString std;
    // get the sign-on banner;
    int i = 0;
    while (soc.Receive(std) == false) {
        if (i++ == 10) {
            pStat->OnSocketError(soc);
            return false;
        }
    }


    if (ShouldShowLogon())
        SocketTraceOff();

    return true;
}

void Rfc822Server::Disconnect(HostName&) {
    soc.Disconnect();
}

void Rfc822Server::LastError(StdString& std) {
    std = soc.GetLastError();
}

void Rfc822Server::MonitorConnection(ostream& os, istream& is) {
    soc.MonitorConnection(os, is);
}

bool Rfc977NewsGroupServer::Connect(UserData& ud) {
    // *****
    // See if "login" is required for NNTP:
    // *****
    // (This works fine with FTP strings like "user rnagy", "pass xxxxxxxx" (literally!));
    ServerType st;
    st.server_type_code = st.nntp();
    ud.SetUserAuthorization("", st);
    StdString str;
    str = ud.Authorize(*this);

    // Inform of slave status;
    bool br = false;
    if (str.is_null() == true) {
        std = "SLAVE\r\n";
        br = soc.TransactResponseContaining(std, std);
        if (br == true)
            pStat->Got(ud);
    }
    return br;
}

void Rfc977NewsGroupServer::Disconnect(UserData&) {
    // Stop the news server;
    std = "QUIT\r\n";
    soc.TransactResponseContaining(std, std);
    // No "logout" required for NNTP.
}

bool Rfc977NewsGroupServer::Connect(NewsGroup& group) {
    br = false;

    std = "GROUP ";
    std.append(group.Name());
    std.append("\r\n");
    if (soc.TransactResponseContaining(std, std) == true) {
        switch (std[0]) {
            case '2':
            case '1':
                br = group.AssignFromEntry(std);
                break;

            default:
                break;
        };
    }
    // Assign what we have assigned;
    if (br == true) {
        pwNewsGroup = group;
        pStat->Got(group);
    } else {
        NewsGroup ng;
        pwNewsGroup = ng;
    }
    return br;
}

void Rfc977NewsGroupServer::Disconnect(NewsGroup& ng) {
}

bool Rfc977NewsGroupServer::Get(NewsGroup& ng, Array<ArticleThunk>& rResult, mail_t StatStart) {
    ArticleThunk aT;
    // Carefull! If GetFirstThunk changes, the we need to change the following to match;
    aT.StatStart = StatStart;
    bool br = GetNextThunk(ng, aT);
    while (br == true) {
        rResult[rResult.Nelem()] = aT;
        br = GetNextThunk(ng, aT);
    }
    if (!rResult.Nelem())
        return false;
    return true;
}

bool Rfc977NewsGroupServer::GetFirstThunk(NewsGroup& ng, ArticleThunk& aT, bool bForward) {
    // WE NEED TO TRAP FOR
    // WSAECONRESET
    // AT THE SOCKET LEVEL!
    // <<<WIP>>>
    BEGIN
    // Zero out the result;
    ArticleThunk at2;
    aT = at2;
    END
    if (bForward == true) {
        aT.StatStart = (ng.LowestRead() > ng.LowestNumber()) ? ng.LowestRead() : ng.LowestNumber();

        // If we've seen 'em all, then we've no more to see (c++?);
        if (aT.StatStart >= ng.HighestNumber())
            return false;
    } else
        aT.StatStart = ng.HighestNumber(); // Searching BACKWARDS from here (inclusive)

    pStat->OnGroupStatStart(ng, aT.StatStart);

    return GetNextThunk(ng, aT, bForward);
}

bool Rfc977NewsGroupServer::GetNextThunk(NewsGroup& ng, ArticleThunk& aT, bool bForward) {
    if (!(pwNewsGroup == ng))
        if (Connect(ng) == false) // "AssignFromEntry"
            return false;

    // AFTER a possible Connect() ("AssignFromEntry()"),
    // if there are none to get, then we've got em all!
    if (!ng.Nelem())
        return false;

    StdString stdIn, stdOut;
    int retries = NULL;

    StdString std2;
    int running = 1;
    // This is a count of how many time we will "ignore" a "garbage filled", or non RFC-compliant, response.
    int too_much = NULL;
    while (running) {
        if (pStat->ShouldAbort() == true) {
            pStat->OnUserAbortProcessed();
            return false;
        }

        // Are we done?
        if (aT.StatStart < ng.LowestNumber())
            return false;

        // Are we done?
        if (aT.StatStart > ng.HighestNumber())
            return false;

        // We are not using the CP here. Instead, we count back/forwards from the last know article
        // number;
        stdIn = "";
        stdIn << "STAT " << aT.StatStart << "\r\n";
#ifdef DEBUG
        pStat->OnArticleStat(ng, aT.StatStart, retries);
#endif

        stdOut = "^";
        size_t times = 6;
        while (times-- && (soc.TransactResponseContaining(stdIn, stdOut) == false))
            ;
        if (!times) {
            // <<WIP>>>: This is where we should try a re-connect;
            return false;
        }

        // Nothing back. Dung happened;
        if (stdOut[0] == '^') {
            // <<WIP>>>: This is where we should try a re-connect;
            return false;
        }

        RfcStatusCode RfcCode;
        if (RfcCode.Assign(stdOut) == true) {
            switch (stdOut[0]) {
                case '2':
                    //    2xx - Command ok
                    // Either got garbage back, or we're legally done;
                    if (aT.Assign(stdOut) == true) {
                        retries = NULL;
                        pStat->Got(aT);
                        pStat->OnNewsGroupObject(ng, aT, aT.StatStart, retries);
                        if (bForward)
                            aT.StatStart++;
                        else
                            aT.StatStart--;
                        return true;
                    }
                case '4':
                    //    4xx - Command was correct, but couldn't be performed for some reason.
                    // An error of this type is either an article withdrawn, the end of the
                    // enumeration, or a concidental positioning of an ASCII character at
                    // the [0] position (I've seen it happen!). Either way, we'll count it as
                    // the same error and re-try;
                    // Observed: "483 NEWNEWS temporarily disabled due to load" in both slave and non-slave modes.
                    // Observed: "423 Bad article number"
                    if (RfcCode.Code() != 423) {
                        pStat->OnError(ng, RfcCode);
                        return false;
                    }

                case '1':
                    //    1xx - Informative message
                case '3':
                    //    3xx - Command ok so far, send the rest of it.
                default:
                    retries = NULL;
                    if (bForward)
                        aT.StatStart++;
                    else
                        aT.StatStart--;
                    continue;
                case '5':
                    //    5xx - Command unimplemented, or incorrect, or a serious program error occurred.
                    // "oficially" give-up on this dog;
                    pStat->OnError(ng, RfcCode);
                    retries = NULL;

                    // Check our too-much count;
                    too_much++;
                    if (too_much > 25)
                        return false;

                    // Go to the next one, if we can;
                    if (bForward)
                        aT.StatStart++;
                    else
                        aT.StatStart--;
                    continue;
            }
        } else {
            retries++;
            if (retries > 10) {
                // "oficially" give-up on this dog;
                pStat->OnBufferError(ng, stdOut);
                retries = NULL;

                // Check our too-much count;
                too_much++;
                if (too_much > 25)
                    return false;

                // Go to the next one, if we can;
                if (bForward)
                    aT.StatStart++;
                else
                    aT.StatStart--;
                continue;
            }
        }
    } // ewhile
    return true;
}

bool Rfc977NewsGroupServer::Get(NewsGroup& ng, Array<EMail>& rResult, Array<ArticleThunk>& rIn, EMailFilter *pQ) {
    // Keep the thrashing to a minimum;
    if (rResult.NumZombies() < rIn.Nelem())
        rResult.AddZombies(rIn.Nelem());

    // Collect the beasties;
    // ng.get_failed = ng.get_success = NULL;
    for (size_t which = NULL; which < rIn.Nelem(); which++) {
        if (pStat->ShouldAbort() == true) {
            pStat->OnUserAbortProcessed();
            return false;
        }

        EMail Em;

        ArticleThunk& aT = rIn[which];

        if (Get(ng, Em, aT) == true) {
            //ng.get_success++;
            if (pQ) {
                if (pQ->Qualifies(Em) == true) {
                    rResult[rResult.Nelem()] = Em;
                    pStat->OnArticleMatched(ng, Em, *pQ);
                } else
                    pStat->OnArticleRejected(ng, Em, *pQ);
            } else {
                pStat->OnArticleMatched(ng, Em);
                rResult[rResult.Nelem()] = Em;
            }
        } else {
            pStat->OnArticleRejected(ng, Em);
            //ng.get_failed++;
        }
    } // for
    return true;
}

bool Rfc977NewsGroupServer::Get(NewsGroup& ng, EMail& em, ArticleThunk& aThunk) {
    BEGIN
    // Blank out any previous result;
    EMail em2;
    em = em2;
    END

    stringstream is;
    // This sets-up aThunk.ArticleDownloaded() as required;
    br = GetMagicBuffer(ng, aThunk, is);
    if (br == false) {
        pStat->OnError(ng, aThunk);
        return br;
    }

    is.seekg(0, ios::beg);
    br = em.Parse(is);
    if (br == false) {
        pStat->OnBadArticleBuffer(ng, aThunk, em);
        return br;
    }

    ng.LastArticleRead(aThunk);
    pStat->OnNewsGroupObject(ng, em, aThunk);

    return true;
}

bool Rfc977NewsGroupServer::Post(const EMail& email, StdString& sError) {
    EMail em = email;
    bool br = false;
    sError = "POST: ";


    StdString std, stc;

    std = "post\r\n";
    br = soc.TransactResponseContaining(std, stc);
    if (!br) {
        sError.append("No response to POST request.");
        return br;
    }

    RfcStatusCode RfcCode;
    if (RfcCode.Assign(stc) == true)
        if (RfcCode.Code() != 340) {
            sError.append(stc);
            return false;
        }

    stc.assign(em.Newsgroups());
    if (!stc.is_null()) {
        std.assign("Newsgroups: ");
        std.append(stc);
        std.append("\r\n");
        br = soc.Send(std);
        if (!br) {
            sError.append("Send(Newsgroups:) failed.");
            return br;
        }
    }


    stc.assign(em.From());
    if (!stc.is_null()) {
        std.assign("From: ");
        std.append(stc);
        std.append("\r\n");
        br = soc.Send(std);
        if (!br) {
            sError.append("Send(From:) failed.");
            return br;
        }
    }


    stc.assign(em.Subject());
    if (!stc.is_null()) {
        std.assign("Subject: ");
        std.append(stc);
        std.append("\r\n");
        br = soc.Send(std);
        if (!br) {
            sError.append("Send(Subject:) failed.");
            return br;
        }
    }

    if (soc.Send("\r\n") == false) {
        sError.append("Send(MESSAGE_TEXT_START) failed.");
        return false;
    }

    br = em.FirstMessageLine(std);
    while (br) {
        br = soc.Send(std);
        if (!br) {
            sError.append("Send(MESSAGE_TEXT) failed.");
            return br;
        } else
            br = em.NextMessageLine(std);
    }

    std = "\r\n.\r\n";
    br = soc.TransactResponseContaining(std, stc);
    if (!br) {
        sError.append("Article \"dot\" termination failed. Article might not have been posted by server.");
        return br;
    }

    if (RfcCode.Assign(stc) == true)
        if (RfcCode.Code() != 240) {
            sError.append(stc);
            return false;
        }

    sError = "";
    return br;
}

// NOTE: 'MORE' is never used. Commands are assumed to produce a contiguous response;

bool Rfc822Server::TransactRfcOrDottedResponse(const StdString& sCmd, StdString& sResult) {
    bool br = soc.TransactResponseContaining(sCmd, sResult);
    if (br) {
        // If we have an RfcStatusCode, then we are done, too;
        RfcStatusCode RfcCode;
        if (RfcCode.Assign(sResult) == true) {
            switch (sResult[0]) {
                case '1': //    1xx - Informative message
                    break;
                case '2': //    2xx - Command ok (Either got garbage back, or we're legally done);
                case '3': //    3xx - Command ok so far, send the rest of it.
                case '4': //    4xx - Command was correct, but couldn't be performed for some reason.
                case '5': //    5xx - Command unimplemented, or incorrect, or a serious program error occurred.
                default:
                    return true;
            }
        }
    }
    while (br && (isdotmagic(sResult) == false)) {
        StdString sBuf;
        br = soc.Receive(sBuf);
        sResult.append(sBuf);
    }
    return br;
}

bool Pop3Server::RemoveFromServer(Array<PopId>& array)
   {
   PopId def;
   for(size_t ss = 0L; ss < array.Nelem(); ss++)
      {
      if(array[ss].szPos == def.szPos)
         continue;   // gigo
      if(DeleteArticle(array[ss].szPos) == false)
         return false;
      }
   return true;
   }

bool Pop3Server::Get(Directory& dirSave, const StdString& sType, bool bRemoveFromServer)
   {
   StdString str;

   // STEP
   // ****
   // Set up our download location
   if(dirSave.Exists() == false && dirSave.Create() == false)
      {
      stringstream srm;
      srm << "Error: Unable to create " << dirSave.Name();
      zStat.OnVerbose(PRESTO(srm));
      return false;
      }

   // STEP
   // ****
   // Prime the loop with the number of articles avaialble NOW
   size_t szStat;
   if(StatArticles(szStat) == false)
      return false;
   if(!szStat)
      return true;

   // STEP
   // ****
   // DOWNLOAD EVERY ARTICLE IN THE MAILBOX.
   // DELETE EVERYTHING EXTRACTED FROM THE HOST IF REQUESTED.
   Array<PopId> array;
   for(size_t ss = NULL; ss < szStat; ss++)
      {
      BEGIN
      stringstream srm;
      srm << "Attempting buffer capture for article " << ss;
      srm << ends;
      zStat.OnVerbose(PRESTO(srm));
      END

      bool br = GetMagicBuffer(ss+1, str);
      if(br == false)
         {
         stringstream srm;
         srm << "ERROR: Unable to download article #";
         srm << ss+1;
         srm << " from HOST.";
         srm << ends;
         zStat.OnVerbose(PRESTO(srm));
         if(bRemoveFromServer)
            {
            stringstream srm2;
            if(RemoveFromServer(array))
               srm2 << "CLEANUP: Removed " << array.Nelem() << " downloaded articles from host...";
            else
               srm2 << "CLEANUP: UNABLE TO REMOVE downloaded articles from host!";
            zStat.OnVerbose(PRESTO(srm2));
            }
         return false;
         }

      zStat.OnVerbose("Article buffer captured.");

      // STEP:
      // *****
      // Save the message buffer to a TEMP file for RFC822 parse / rename (next)
      File fileTEMP;
      if(dirSave.Home(fileTEMP) == false)
         zStat.OnVerbose("WARNING: Unexpected Home(temp) error!");
      fileTEMP.MkUniqueName(dirSave, sType);
      BEGIN
      stringstream srm;
      srm << "Temporarily saving buffer as `" << fileTEMP.Name() << "`...";
      srm << ends;
      zStat.OnVerbose(PRESTO(srm));
      END
      str.write(fileTEMP.OpenWrite(File::AT_BINARY));
      fileTEMP.Close();
      str = "";

      // STEP:
      // *****
      // Rename the file - from the RFC822 portions of the HEADER
      EMailHeader emh;
      if(EMailFactory::Import(fileTEMP, emh) == true)
         {
         // 02/11/2007 - Server was timing out - Rename is a lot faster than parse!
         // Since we ARE using the RFC as the default i/o format, all we
         // need to do is to RENAME it.
         // 01/10/2008: Changed to MkLocalSubjectName (was MkLocanName())
         File fileLocal = EMailFactory::MkLocalName(dirSave, emh.Subject(), sType);
         if(fileTEMP.Rename(fileLocal.Name()) == false)
            {
            stringstream srm;
            srm << "WARNING: MkLocalName(" << fileTEMP.Name() << ") FAILED.";
            zStat.OnVerbose(PRESTO(srm));
            }
         else
            {
            stringstream srm;
            srm << "Buffer renamed to `" << fileTEMP.Name() << "`.";
            srm << ends;
            zStat.OnVerbose(PRESTO(srm));
            }
         }
      else
         {
         stringstream srm;
         srm << "WARNING: RFC822 EMailHeader.Import(" << fileTEMP.Name() << ") FAILED.";
         zStat.OnVerbose(PRESTO(srm));
         }

      // email._pop3_server_id = ss+1;
      array.Append(PopId(ss+1));
      } // efor

      // STEP
      // ****
      // FILES SECURED ON CLIENT:
      // We are free to delete the items from the server, if that's what
      // the user requested (default);
      if(bRemoveFromServer)
         {
         stringstream srm;
         srm << "CLEANUP: " << array.Nelem() << " articles ";
         if(RemoveFromServer(array))
            srm << " removed from host.";
         else
            srm << " UNABLE TO BE COMPLETELY REMOVED from host!";
         zStat.OnVerbose(PRESTO(srm));
         }
      return true;
   }
bool Rfc977NewsGroupServer::GetMagicBuffer(NewsGroup& ng, ArticleThunk& at, stringstream& sResult) {
    // The default;
    at.ArticleDownloaded(false);

    if (!(pwNewsGroup == ng))
        if (Connect(ng) == false)
            return false;

    // Call out the EXACT article that we are looking for (preference to  thunk's number)
    std = "";
    if (at.Number())
        std << "ARTICLE " << at.Number() << "\r\n";
    else
        std << "ARTICLE " << at.MessageId() << "\r\n";

    int first_one = 1;
    int put_packet_retry_times = 0;
    int get_packet_retry_times = 0;
    while (pStat->ShouldAbort() == false) {
        // ********
        // * STEP *
        // EXTRACT THE PACKET FROM THE SOCKET (includes rational timeout-handling)

        // A time-out on SEND is actually very common, since the socket driver might very well
        // TIMEOUT on us under Windows: NNTP activity seems to fail to be noticed as "activity"!
        // Maybe this behavior could be avoided by paying attention to some type of a KEEPALIVE
        // socket-level protocol? Who knows! -As mail_t as we have the ArticleThunk.Id(), and can
        // resume a download from THAT point, this isn't a very large problem...
        if (Put(std) == false) {
#ifdef MOO_MOO_CHECK
            SocketTraceOn();
#endif
            switch (put_packet_retry_times) {
                case 0:
                case 1:
                    put_packet_retry_times++;
                    continue;
                default:
                {
                    pStat->OnTimeoutDeath(ng, at, put_packet_retry_times);
                    return false;
                }
            }
        }

        // ********
        // * STEP *
        // GET the response.
        if (pStat->ShouldAbort() == true)
            continue;

        bool br = Rfc822Server::Get(std);
        if (br == false) {
#ifdef MOO_MOO_CHECK
            SocketTraceOn();
#endif
            // This is an elegant way to make sure we that we are not "hung"
            // on a "more", while waiting to download an existing article:
            // WE JUST ASK FOR THE WHOLE DARN THING OVER AGAIN if a threshold
            // of "get_packet_retry_times" has not yet expired;
            switch (get_packet_retry_times) {
                    // SEND OKAY, THEN RECEIVE PACKET ERROR / TIME OUT, LOGIC;
                    // =======================================================
                case 0: // second time
                case 1: // third time
                case 2: // fourth time
                    // Always assume that time-out is a more serious error. Ask for the
                    // article again, by name. -This is to keep us from getting "hung"
                    // on an unexpected "more";
                    if (!first_one) {
                        // Junk the present article && ask for it over again, by name;
                        std = "";
                        if (at.Number())
                            std << "ARTICLE " << at.Number() << "\r\n";
                        else
                            std << "ARTICLE " << at.MessageId() << "\r\n";
                        sResult.seekp(0, ios::beg);
                    }
                    get_packet_retry_times++;
                    continue;
                default:
                {
                    pStat->OnTimeoutDeath(ng, at, get_packet_retry_times);
                    return false;
                }
            }
        } // ewhile(FLASE)

        // ********
        // * STEP *
        // Packet received okay, check the NNTP codes.
        get_packet_retry_times = 0;

        // Check the return code on the first_one;
        if (first_one) {
            switch (std[0]) {
                case '2': // article found...
                {
                    at.Error("");

                    // First packet will have the NNTP result line in it. We need to remove it;
                    long sp = std.find('\n');
                    if (size_t(sp) != npos)
                        std.remove_pos(0, sp);
                }
                    break;
                case '4':
                    // An error of this type is either an article withdrawn, the end of the
                    // enumeration, or a concidental positioning of an ASCII character at
                    // the [0] position (I've seen it happen!). Either way, we'll count it as
                    // the same error and re-try;
                    // Observed: "483 NEWNEWS temporarily disabled due to load" in both slave and non-slave modes.
                    // Observed: "423 Bad article number"
                case '5':
                default:
                {
                    RfcStatusCode RfcCode;
                    if (RfcCode.Assign(std) == true) {
                        pStat->OnError(ng, RfcCode);
                        return false;
                    }
                    sResult << std.c_str();
                    sResult << ends;
                    at.Error(std);
                    pStat->OnBufferError(ng, at, std);
                    return false;
                }
            }
        } // first_one

        // ********
        // * STEP *
        // Looks official, build the article, block-by-block;
        br = isdotmagic(std);
        first_one = 0;
        sResult << std.c_str();
        if (br == true)
            break; // We're done! (normal exit point)

        std = MORE_STRING; // "MORE"
    }
    sResult << ends;

    if (pStat->ShouldAbort() == true) {
        pStat->OnUserAbortProcessed();
        return false;
    }

    at.ArticleDownloaded(true);
#ifdef MOO_MOO_CHECK
    SocketTraceOff();
#endif
    return true;
}

bool SmtpServer::Connect(HostName& hn, UserData& ud) {
    // Added on 08/17/2002, R. Nagy
    bool bUseHelo = true;
    StdString str;
    ud.SetError(str);
    hn.SetError(str);
    StdString std = ud.Domain();
    if (std.is_null()) {
        bUseHelo = false;
        //str = "LocalError: SMTP HELO protocol requires a domain name [RFC821].";
        //ud.SetError(str);
        //hn.SetError(str);
        //return false;
    }

    if (Rfc822Server::Connect(hn) == false) {
        StdSocket soc;
        std = soc.GetLastError();
        std.prepend("Socket: Cannot connect to HOST: ");
        ud.SetError(str);
        hn.SetError(str);
        return false;
    }

    // 08/17/2002, R. Nagy
    if (bUseHelo == false)
        return true;

    /*****
       StdString sHelp;
       if(Transact("HELP\r\n", sHelp) == false)
          {
          ud.SetError(sHelp);
          hn.SetError(sHelp);
          return false;
          }
     *****/

    // Some servers (like outgoing.verizon.net) support EHLO...
    std = ud.Domain();
    std.prepend("EHLO ");
    std.append("\r\n");
    StdString sResult;
    ServerType st;
    st.server_type_code = st.smtp();
    if (TransactCodeLevel(std, sResult, 2) == false) {
        ud.SetUserAuthorization("", st);
        std = ud.Domain();
        std.prepend("HELO ");
        std.append("\r\n");
        StdString sResult;
        if (TransactCodeLevel(std, sResult, 2) == false) {
            ud.SetError(sResult);
            hn.SetError(sResult);
            return false;
        }
    } else {
        // Store supported authorazition encoding types:
        size_t whence = sResult.find("AUTH ");
        if (whence != NPOS) {
            StdString sResult2 = &sResult[whence];
            whence = sResult.find('\r');
            if (whence != NPOS)
                sResult2.remove(whence);
            ud.SetUserAuthorization(sResult2, st);
        }
    }
    /*
    RFC 1869 prohibits all extensions except (1) extensions defined by the IETF 
    and (2) extensions beginning with the letter X. However, some servers use 
    private extensions that do not begin with the letter X. In any case, clients 
    must be prepared for unrecognized extensions; the IETF may define new extensions 
    at any time. Extensions generally apply to a single connection. If the client 
    reconnects to the same host a minute later, it may find a completely different 
    list of extensions. 

    Extensions are supposed to be interpreted without regard to case. In practice 
    extensions are always sent in uppercase. I don't know if there are clients that 
    have trouble with lowercase.

                Extension name index
                The following list shows all the extensions that I've seen from SMTP servers. 
                ----------------------------------------------------------------------------
                8BITMIME 
                ATRN 
                AUTH 
                AUTH=LOGIN 
                BINARYMIME 
                CHUNKING 
                DSN 
                ENHANCEDSTATUSCODES 
                ENHANGEDSTATUSCODES [sic] 
                ETRN 
                EXPN 
                HELP 
                ONEX 
                PIPELINING 
                RSET 
                SAML 
                SEND 
                SIZE 
                SOML 
                TIME 
                TLS 
                TURN 
                VERB 
                VRFY 
                X-EXPS 
                X-EXPS=LOGIN 
                X-LINK2STATE 
                X-RCPTLIMIT 
                X-TURNME 
                XADR 
                XAUD 
                XDSN 
                XEXCH50 
                XGEN 
                XONE 
                XQUE 
                XREMOTEQUEUE 
                XSTA 
                XTRN 
                XUSR 
                XVRB    
    
     */
    return true;
}

void SmtpServer::Disconnect(HostName& hn) {
    StdString sResult;
    Transact("QUIT\r\n", sResult);
    Rfc822Server::Disconnect(hn);
}

bool SmtpServer::Connect(UserData& ud) {
    // 08/17/2002, R. Nagy 
    StdString str;
    str = ud.Authorize(*this);
    if (str.is_null()) {
        ConnectedUser = ud;
        return true;
    }
    return false;
}

void SmtpServer::Disconnect(UserData&) {
}

bool SmtpServer::Post(const OutBoundEmail& obm, StdString& sError) {
    EMail emOut;
    emOut = obm;
    emOut._to = obm.To();
    emOut._from = obm.From();
    return Post(emOut, sError);
}

bool SmtpServer::Post(const EMail& email, StdString& sError) {
    StdString std, sResult;
    std = "MAIL FROM:<";
    std.append(email.From()); //ConnectedUser.User());
    std.append(">\r\n");
    if (Transact(std, sResult) == false)
        return false;
    RfcStatusCode RfcCode;
    if (RfcCode.Assign(sResult) == false) {
        pStat->OnError(email, RfcCode);
        return false;
    }
    if (RfcCode.Code() != 250) {
        pStat->OnError(email, RfcCode);
        return false;
    }

    std = email.To();
    std.prepend("RCPT TO:<");
    std.append(">\r\n");
    if (Transact(std, sResult) == false)
        return false;
    if (RfcCode.Assign(sResult) == false) {
        pStat->OnError(email, RfcCode);
        return false;
    }
    if (RfcCode.Code() != 250) {
        pStat->OnError(email, RfcCode);
        return false;
    }

    std = "DATA";
    std.append("\r\n");
    if (Transact(std, sResult) == false)
        return false;

    if (RfcCode.Assign(sResult) == false) {
        pStat->OnError(email, RfcCode);
        Transact("\r\n.\r\n", sResult);
        pStat->OnError(email, sResult);
        return false;
    }
    if (RfcCode.Code() != 354) {
        pStat->OnError(email, RfcCode);
        return false;
    }

    EMailLines lines;
    lines.Assign(email);
    bool br = lines.FirstLine(std);
    while (br == true) {
        br = Put(std);
        if (br == false) {
            pStat->OnError(email, "Send-data failed.");
            // WARNING: Conversation is now in an unknown state!
            // We assume that the connection has been lost, else
            // we need to terminate (as above).
            return false;
        }
        br = lines.NextLine(std);
    };

    if (Transact("\r\n.\r\n", sResult) == false)
        return false;
    if (RfcCode.Assign(sResult) == false) {
        pStat->OnError(email, RfcCode);
        return false;
    }
    if (RfcCode.Code() != 250) {
        pStat->OnError(email, RfcCode);
        return false;
    }
    return true;
}

bool Pop3Server::Connect(UserData& ud) {
    // 08/17/2002: We must keep the Kaled race pure...
    ServerType st;
    st.server_type_code = st.pop3();
    ud.SetUserAuthorization("", st);
    StdString str;
    str = ud.Authorize(*this);
    if (str.is_null())
        return true;
    return false;
}

void Pop3Server::Disconnect(UserData& ud) {
    StdString sError;
    Transact("QUIT\r\n", sError);
}

bool Pop3Server::Get(Pop3Batch& rBatch) {
    EMail email;
    StdString std, sTempFile;

    // STEP
    // ****
    // If we need to recover the batch, then we should do so now;
    if (rBatch.ShouldRecover()) {
        pStat->OnVerbose("Automatic recovery initiated.");
        rBatch.LoadRecoveryFile();
    } else
        pStat->OnVerbose("Automatic recovery not required.");

    BEGIN
    ProgramId pid;
    File file;
    pid.GetTemp(file);
    sTempFile = file.Name();
    sTempFile.append(".EML");
    END

    rBatch.Error(""); // Reset the error flag.

    size_t sz;
    if (StatArticles(sz) == false)
        return false;
    if (!sz)
        return true;

    {
        // STEP
        // ****
        // PARSE EVERY ARTICLE IN THE MAILBOX:
        for (size_t ss = NULL; ss < sz; ss++) {
            BEGIN
            stringstream srm;
            srm << "Attempting buffer capture for article " << ss;
            srm << ends;
            pStat->OnVerbose(PRESTO(srm));
            END

                    bool br = GetMagicBuffer(ss + 1, std);
            if (br == false) {
                stringstream srm;
                srm << "ERROR: Unable to download article #";
                srm << ss + 1;
                srm << " from HOST.";
                srm << ends;
                rBatch.Error(PRESTO(srm).c_str());
                return false;
            }
            pStat->OnVerbose("Article buffer captured.");

            File file;
            file.Name(sTempFile);

            BEGIN
            stringstream srm;
            srm << "Saving buffer as `" << sTempFile << "`...";
            srm << ends;
            pStat->OnVerbose(PRESTO(srm));
            END

            std.write(file.OpenWrite(File::AT_BINARY));
            std = "";
            file.Close();

            BEGIN
            stringstream srm;
            srm << "Parsing `" << sTempFile << "`.";
            srm << ends;
            pStat->OnVerbose(PRESTO(srm));
            END

            if (email.Import(file.Name()) == true) {
                file.Close();
                BEGIN
                stringstream srm;
                srm << "Parse of `" << sTempFile << "` compleded successfully. Removing same.";
                srm << ends;
                pStat->OnVerbose(PRESTO(srm));
                END
                file.Remove();

                email._pop3_server_id = ss + 1;
                if (rBatch.Append(email) == false) {
                    stringstream srm;
                    srm << "ERROR: Pop3Batch is unable to add article #";
                    srm << ss + 1;
                    srm << " to batch.";
                    srm << ends;
                    rBatch.Error(PRESTO(srm).c_str());
                    return false;
                }
            } else {
                file.Close();
                stringstream srm;
                srm << "WARNING: EMail.Parse(";
                srm << sTempFile;
                srm << ") FAILED. File kept for examination. POP3 session's article #";
                srm << ss + 1;
                srm << " remains on host.";
                srm << ends;
                rBatch.Error(PRESTO(srm).c_str());

                // Now we need another temp-file name;
                ProgramId pid;
                File file;
                pid.GetTemp(file);
                sTempFile = file.Name();
                continue;
            }

        } // efor

        // STEP
        // ****
        // MAILBOX SUCCESSFULLY PARSED:
        // IF we are REMOVING these files from the server, then we need to
        // "harden" the articles so we will not loose ANYthing if we go down;
        if (rBatch.AutoRemoveHostArticles() && (rBatch.SaveRecoveryFile() == false)) {
            rBatch.Error("ERROR: Unable to save-out batch. Articles NOT removed from host.");
            return false;
        }

        // STEP
        // ****
        // FILES SECURED ON CLIENT:
        // We are free to delete the items from the server, if that's what
        // the user requested (default);
        if (rBatch.AutoRemoveHostArticles()) {
            EMail *pEmail = NULL;
            pEmail = rBatch.Recall(pEmail);
            while (pEmail) {
                if (RemoveFromServer(*pEmail) == false) {
                    stringstream srm;
                    srm << "WARNING: Unable to DELETE article";
                    srm << " from host. All remaining articles NOT deleted.";
                    srm << ends;
                    rBatch.Error(PRESTO(srm).c_str());
                    pEmail = NULL;
                } else
                    pEmail = rBatch.Recall(pEmail);
            }
        }
    }
    return true;
}

bool Pop3Server::GetMagicBuffer(size_t which, StdString& sResult) {
    EMail email;
    StdString std;
    BEGIN
    stringstream srm;
    srm << "RETR ";
    srm << which;
    srm << "\r\n";
    srm << ends;
    std = PRESTO(srm).c_str();
    END

    pStat->OnVerbose("RETR start.");
    bool br = Transact(std, std);
    if (br == false) {
        std.prepend("Error: RETR command returned: ");
        pStat->OnError(email, std);
        return br;
    }
    pStat->OnVerbose("RETR completed.");

    size_t sz = std.find("+OK ");
    if (sz == NPOS) {
        std.prepend("Error: Unknown RETR response: ");
        pStat->OnError(email, std);
        return false;
    }
    pStat->OnVerbose("RETR result okay.");

    // OBSERVED: Some times, on Windows98 ONLY (no joke)
    // RETR returns the entire message, while same code
    // and server combination did not do so on Windows2000!
    sz = std.find("\r\n");
    if (sz != NPOS)
        std.remove_pos(0, sz + 2); // Chomp the RETR message
    sResult = std;
    if (isdotmagic(sResult) == true) {
        pStat->OnVerbose("Server returned the ENTIRE message on RETR!");
    } else {
        pStat->OnVerbose("Attempting Get(1).");
        if (Rfc822Server::Get(std) == false) {
            pStat->OnError(email, "Error: Get(1) failed.");
            return false;
        }
        pStat->OnVerbose("Get(1) okay.");
        sResult.append(std);
        while (isdotmagic(sResult) == false) {
            pStat->OnVerbose("No dot magic: Attempting Get(2).");
            std = "\r"; // Be sure to select a shorter time-out (see Get())
            if (Rfc822Server::Get(std) == false) {
                stringstream srm;
                srm << "Error: Get(2) failed. Results follow;" << endl;
                srm << "/RESULTS[" << sResult << "]RESULTS\\" << endl << endl;
                srm << "/ERRORRESULTS[" << std << "]ERRORRESULTS\\" << endl;
                pStat->OnError(email, PRESTO(srm));
                return false;
            }
            pStat->OnVerbose("Get(2) okay.");
            sResult.append(std);
        }
    }


    pStat->OnVerbose("Properly terminated message buffer successfully retrieved from host.");
    return true;
}

bool Pop3Server::DeleteArticle(size_t which) {
    StdString std;
    BEGIN
    stringstream srm;
    srm << "DELE ";
    srm << which;
    srm << "\r\n";
    srm << ends;
    std = PRESTO(srm).c_str();
    END

    pStat->OnVerbose("DELE start.");
    bool br = Transact(std, std);
    if (br == false) {
        EMail email;
        std.prepend("Error: DELE command returned: ");
        pStat->OnError(email, std);
        return br;
    }
    pStat->OnVerbose("DELE command returned.");

    size_t sz = std.find("+OK ");
    if (sz == NPOS) {
        EMail email;
        std.prepend("Error: Unknown DELE response: ");
        pStat->OnError(email, std);
        return false;
    }
    pStat->OnVerbose("DELE command successful.");

    return true;
}

bool Pop3Server::StatArticles(size_t& sz) {
    StdString std;
    pStat->OnVerbose("STAT start.");
    bool br = Transact("STAT\r\n", std);
    if (br == false) {
        EMail email;
        std.prepend("Error: STAT command returned: ");
        pStat->OnError(email, std);
        return false;
    }
    pStat->OnVerbose("STAT command completed.");

    sz = std.find("+OK ");
    if (sz == NPOS) {
        EMail email;
        std.prepend("Error: Unknown STAT response: ");
        pStat->OnError(email, std);
        return false;
    }
    pStat->OnVerbose("STAT command okay.");

    std.remove_pos(0, 4);
    sz = std.find(' ');
    if (sz == NPOS) {
        EMail email;
        std.prepend("Error: Unknown STAT response: ");
        pStat->OnError(email, std);
        return false;
    }

    pStat->OnVerbose("STAT command successful.");
    std = std.substr(0, sz);
    sz = atoi(std.c_str());

    return true;
}

bool Pop3Server::RemoveFromServer(EMail& email) {
    if (!email._pop3_server_id)
        return true;
    if (DeleteArticle(email._pop3_server_id)) {
        email._pop3_server_id = NULL;
        return true;
    }
    return false;
}

Pop3Batch::Pop3Batch(void) : bShouldDelete(true), ssNext(NULL), error_file_time(NULL), email_file_time(NULL) {
    ProgramId pid;
    Directory dir;
    pid.GetProgram(dir);
    sOutputDirectory = dir.Name();
    sOutputDirectory.append("\\Pop3Batch");
    dir.Name(sOutputDirectory);
    dir.Create();
}

Pop3Batch::~Pop3Batch(void) {
}

bool Pop3Batch::Append(EMail& em) {
    rArray[rArray.Nelem()] = em;
    return true;
}

bool Pop3Batch::SaveRecoveryFile(void) {
    ProgramId pid;
    StdString std = POP3_RECOVERY_FILE;
    File file;
    file.Name(std);
    pid.PutInTemp(file);
    ostream& os = file.OpenWrite(File::AT_BINARY);

    Array<StreamString> rReserved;
    rReserved.write(os);

    rArray.write(os);
    rErrorArray.write(os);
    os.flush();
    if (os)
        return true;
    ::unlink(file.Name());
    return false;
}

bool Pop3Batch::LoadRecoveryFile(void) {
    rArray.Free();
    rErrorArray.Free();

    ProgramId pid;
    StdString std = POP3_RECOVERY_FILE;
    File file;
    file.Name(std);
    pid.PutInTemp(file);
    std = file.Name();

    istream& is = file.OpenRead(File::AT_BINARY);

    Array<StreamString> rReserved;
    rReserved.read(is);

    rArray.read(is);
    rErrorArray.read(is);

    if (is)
        return true;
    return false;
}

bool Pop3Batch::ShouldRecover(void) {
    File file;
    file.Name(POP3_RECOVERY_FILE);
    return file.Exists();
}

size_t Pop3Batch::Save(void) {
    Directory dir;
    dir.Get();
    dir.Push();
    if (::SetCurrentDirectory(sOutputDirectory.c_str()) == 0) {
        dir.Pop();
        return NPOS;
    }

    for (size_t ss = NULL; ss < rArray.Nelem(); ss++) {
        File file;
        if (file.MkUniqueName() == false)
            return NPOS;
        if (rArray[ss].SaveBin(file) == false) {
            dir.Pop();
            return NPOS;
        }
    }

    File file;
    file.Name("DownloadErrors.ARY");
    rErrorArray.write(file.OpenAppend(File::AT_BINARY));

    dir.Pop();
    return rArray.Nelem();
}

size_t Pop3Batch::Export(Directory& dirOut, const StdString& sExt) {
    Directory dir;
    dir.Get();
    dir.Push();

    if (dirOut.Set() == false) {
        dir.Pop();
        return NPOS;
    }
    EMail eml;
    StdString sSubject;
    File file;
    for (size_t ss = NULL; ss < rArray.Nelem(); ss++) {
        sSubject = ARTNAME(dirOut, rArray[ss], sExt);
        if (file.Name(sSubject) == false)
            return NPOS;
        if (rArray[ss].Export(file.Name()) == false) {
            dir.Pop();
            return NPOS;
        }
    }

    file.Name("DownloadErrors.ARY");
    rErrorArray.write(file.OpenAppend(File::AT_BINARY));

    dir.Pop();
    return rArray.Nelem();
}

size_t Pop3Batch::Dump(const StdString& sExt) {
    Directory dir;
    dir.Get();
    dir.Push();
    if (::SetCurrentDirectory(sOutputDirectory.c_str()) == 0) {
        dir.Pop();
        return NPOS;
    }

    File file;
    for (size_t ss = NULL; ss < rArray.Nelem(); ss++) {
        if (file.MkUniqueName() == false)
            return NPOS;
        if (rArray[ss].Export(file.Name()) == false) {
            dir.Pop();
            return NPOS;
        }
    }

    file.Name("DownloadErrors.ARY");
    rErrorArray.write(file.OpenAppend(File::AT_BINARY));

    dir.Pop();
    return rArray.Nelem();
}

bool Pop3Batch::ReplaceArray(Array<EMail>& rArray) {
    StdString std;

    std = sOutputDirectory;
    std.append('\\');
    std.append(POP3_BATCH_FN);
    File file;
    file.Name(std);
    ostream& os = file.OpenWrite(File::AT_BINARY);
    if (!os)
        return false;
    rArray.write(os);
    if (os) {
        file.Close();
        email_file_time = file.GetTime();
        return true;
    }
    email_file_time = NULL;
    return false;
}

bool Pop3Batch::HaveArraysChanged(void) {
    File file;
    StdString std;

    std = sOutputDirectory;
    std.append('\\');
    std.append(POP3_BATCH_ERROR_FN);
    file.Name(std);
    if (error_file_time && (error_file_time != file.GetTime()))
        return true;

    std = sOutputDirectory;
    std.append('\\');
    std.append(POP3_BATCH_FN);
    file.Name(std);
    if (email_file_time && (email_file_time != file.GetTime()))
        return true;

    return false;
}

bool Pop3Batch::ReplaceArray(Array<StreamString>& rArray) {
    StdString std;

    std = sOutputDirectory;
    std.append('\\');
    std.append(POP3_BATCH_ERROR_FN);
    File file;
    file.Name(std);
    ostream& os = file.OpenWrite(File::AT_BINARY);
    if (!os)
        return false;
    rArray.write(os);
    if (os) {
        file.Close();
        error_file_time = file.GetTime();
        return true;
    }
    error_file_time = NULL;
    return false;
}

bool Pop3Batch::LoadArray(Array<EMail>& rArray) {
    StdString std;
    std = sOutputDirectory;
    std.append('\\');
    std.append(POP3_BATCH_FN);
    rArray.Free();

    File file;
    file.Name(std);
    email_file_time = file.GetTime();
    istream& is = file.OpenRead(File::AT_BINARY);
    if (!is) {
        file.Close();
        if (file.Exists() == false) // The first time through, the file will NOT exist
            return true; // so this in not a problem.
        return false;
    }
    rArray.read(is);
    if (!is)
        return false;
    return true;
}

bool Pop3Batch::LoadArray(Array<StreamString>& rArray) {
    StdString std;
    std = sOutputDirectory;
    std.append('\\');
    std.append(POP3_BATCH_ERROR_FN);

    File file;
    file.Name(std);
    error_file_time = file.GetTime();
    istream& is = file.OpenRead(File::AT_BINARY);
    if (!is)
        return false;
    rArray.read(is);
    if (!is)
        return false;
    return true;
}

size_t Pop3Batch::SaveArrays(void) {
    bool br;
    br = ReplaceArray(rArray);
    if (br == false)
        return NPOS;
    br = ReplaceArray(rErrorArray);
    if (br == false)
        return NPOS;
    return rArray.Nelem();
}

void Pop3Batch::BatchCompleted(void) {
    rArray.Free();
    rErrorArray.Free();
    ProgramId pid;
    StdString std = POP3_RECOVERY_FILE;
    File file;
    file.Name(std);
    pid.PutInTemp(file);
    std = file.Name();
    ::unlink(std.c_str());
}

bool Pop3Batch::AutoRemoveHostArticles(void) {
    return bShouldDelete;
}

void Pop3Batch::Error(const StdString& std) {
    rErrorArray[rErrorArray.Nelem()] = std;
}

EMail *Pop3Batch::Recall(EMail *pem) {
    if (!pem)
        ssNext = NULL;
    if (ssNext < rArray.Nelem()) {
        pem = &rArray[ssNext];
        ssNext++;
    } else
        pem = NULL;
    return pem;
}

void Pop3Batch::RecallErrors(StdString& std) {
    std = "";
    for (size_t ss = NULL; ss < rErrorArray.Nelem(); ss++) {
        std.append(rErrorArray[ss]);
        std.append("\r\n");
    }
}

bool Pop3Batch::HadErrors(void) {
    if (rErrorArray.Nelem())
        return true;
    return false;
}
