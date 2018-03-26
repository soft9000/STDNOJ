#include "MailBot01.hpp"
//#include <WinCon/WinCon01.hpp>
#include "../IniSet/IniSet.hpp"

struct Thug
{
/*
static bool Get(Pop3Server& server, Directory& dirSave, const StdString& sType, bool bRemoveFromServer)
   {
   StdString str;

   // STEP
   // ****
   // Set up our download location
   if(dirSave.Exists() == false && dirSave.Create() == false)
      {
      stringstream srm;
      srm << "Error: Unable to create " << dirSave.Name();
      // zStat.OnVerbose(PRESTO(srm));
      return false;
      }

   // STEP
   // ****
   // Prime the loop with the number of articles avaialble NOW
   size_t szStat;
   if(server.StatArticles(szStat) == false)
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
         if(fileTEMP.Rename(fileLocal) == false)
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
*/
static bool Download(HostName& host, UserData& user, Directory& dir, StdLog& sLog, bool bDelete, bool bVerbose)
   {
   bool br = false;
   Pop3Server server;
   server.Use(sLog);
   server.SetVerbose(bVerbose);
   br = server.Connect(host);
   if(br == false)
      {
      stringstream srm;
      srm << "Unable to connect to host " << host.Name();
      sLog.PrintLine(PRESTO(srm));
      return false;
      }
   br = server.Connect(user);
   if(br == false)
      {
      stringstream srm;
      srm << "Unable to connect to host " << host.Name() << " as " << user.User();
      sLog.PrintLine(PRESTO(srm));
      return false;
      }

   br = server.Get(dir, ".EML", bDelete);
   if(br == false)
      {
      // TODO: Setup automatic recovery (etc?)
      }

   server.Disconnect(user);
   server.Disconnect(host);

   return br;
   }
};


File MailBot01::FindIni(const StdString& sfb, StdLog& log)
   {
   StdString str = sfb;

   str.to_lower();
   if(str.ends_with(".ini") == false)
      str.append(".ini");  // ini file type is required -

   File file;
   if(file.Name(str) == false)
      return File(); // bad file name - return null -

   if(sfb.find(PathChar()) != npos)
      return file;   // got a FQN - might be creating it - let is go!

   if(file.Exists())
      return file;   // fully qualified file name - ae

   log.PrintLine(StdString("Warning: Unable to locate `") + file.Name() + "` - Activating 'deep search' mode.");

   Directory dir;
   dir.Get();
   dir.Home(file);
   if(file.Exists() == true)
      {
      log.PrintLine(StdString("Warning: Deep Search found `") + file.Name() + "` - Works for me, eh?");
      return file;   // pwd - undocumented, but very nice -
      }
      
   ProgramId pid;
   if(pid.Home(file) == false)
      {
      log.PrintLine("Error: Deep Search giving up. RTM!");
      return File(); // bad home folder - return null -
      }

   if(file.Exists() == true)
      {
      log.PrintLine(StdString("Warning: Deep Search found `") + file.Name() + "` - Works for me, eh?");
      return file;
      }

   log.PrintLine(StdString("Warning: Deep Search found `") + file.Name() + "` - going for it ...!");
   return file;      // default is next to the program -
   }

StdString MailBot01::ProgramName(void)    
   {
   return StdString("EPOP");
   }

StdString MailBot01::ProgramVersion(void) 
   {
   return "1.3.1";  // Version since anyone ELSE has received it -
   }

StdString MailBot01::GetPopDebug(IniFile& ini)
   {
   return ini.Get(ProgramName(), "POP_Debug");
   }

StdString MailBot01::GetPopServer(IniFile& ini)
   {
   return ini.Get(ProgramName(), "POP_Server");
   }


StdString MailBot01::GetPopPort(IniFile& ini)
   {
   return ini.Get(ProgramName(), "POP_Port");
   }


StdString MailBot01::GetPopUser(IniFile& ini)
   {
   return ini.Get(ProgramName(), "POP_User");
   }


StdString MailBot01::GetPopPassword(IniFile& ini)
   {
   return ini.Get(ProgramName(), "POP_Password");
   }


StdString MailBot01::GetPopFolder(IniFile& ini)
   {
   return ini.Get(ProgramName(), "POP_Folder");
   }

bool MailBot01::QueryIni(File& file)
   {
   ProgramId pid;
   file = File(ProgramName() + ".INI");
   return pid.Home(file);
   }

StdString MailBot01::ProgramByline(void)      
   {
   StdString str = ProgramName();
   str += " (aka MailBot01), Ver. ";
   str += ProgramVersion();
   str += ": ";
   str += __DATE__;
   str += ", via R.A. Nagy";
   return str;
   }

void MailBot01::Usage(ostream& cout)
{
   cout << endl << ProgramByline() << endl;
   cout << endl;
   cout << "Retrieve electronic mail messages from an RFC822 style POP Server." << endl;
   cout << "One message per file. Support for multiple accounts via multiple" << endl;
   cout << "ini files. Full invocation / error logging supported." << endl;
   cout << endl;
   cout << "Usage: " << ProgramName() << " [-c | -d | -l | -x ss | -m | -o | -h | -p] [-i=\"c:\\pathto\\file.ini\"]" << endl;
   cout << endl;
   cout << "-c: [C]reate a new INI file (if not found)." << endl;
   cout << "-d: [D]ownload server messages to default folder." << endl;
   cout << "-i: [I]NI config file to use. (Default is the INI in .EXE folder.)" << endl;
   cout << "-l: [L]ist the content of the output folder" << endl;
   cout << "-x: e[X]terminate item listed as 'ss' (move to eXterminate subdir)" << endl;
   cout << "-o: View l[O]g folder" << endl;
   cout << "-m: View [M]essage folder" << endl;
   cout << "-h: View [H]elp (this screen)" << endl;
   cout << "-p: View [P]df file, (if available)" << endl;
}


bool MailBot01::CreateDefault(StdLog& log)
{
   File file;
   if(QueryIni(file) == false)
      {
      log.PrintLine(StdString("Error: CreateDefault - Unable to home file ") + file.Name());
      return false;
      }

   return CreateDefault(file, log);
}


bool MailBot01::Move(IniFile& file, StdLog& log, size_t ssWhich, ostream& cout)
{
   if(ssWhich == 0L)
      {
      log.PrintLine("Error: Item index for Move() cannot equal zero.");
      return false;
      }

   IniFile ini(file);
   ini.Load();
   StdString sDir = GetPopFolder(ini);
   Directory dir;
   if(dir.Name(sDir) == false)
      {
      log.PrintLine(StdString("Error: Unable to locate folder \"") + sDir + "\"");
      return false;
      }

   Directory dirTmp;
   dirTmp.Name("eXterminated");
   dir.Home(dirTmp);
   if(dirTmp.Exists() == false)
      dirTmp.Create();

   Array<File> files;
   dir.Query(files);
   size_t iTally = 0L;
   for(size_t ss = 0L; ss < files.Nelem(); ss++)
      {
      EMailHeader emh;
      if(emh.Import(files[ss]) == true)
         {
         iTally++;
         if(ssWhich == iTally)
            {
            ListItem(files[ss], emh, iTally, cout);
            if(files[ss].CopyTo(dirTmp) == true)
               if(files[ss].Remove() == true)
               cout << endl << "eXterminated." << endl;
            return true;
            }
         }
      }
   return false;
}


void MailBot01::ListItem(const File& file, EMailHeader& emh, size_t iTally, ostream& cout)
{
   cout << endl;
   cout << "======= Item " << iTally << " =======" << endl;
   cout << "File:    " << file.Name()<< endl;
   cout << "To:      " << emh.To()        << endl;
   cout << "From:    " << emh.From()      << endl;
   cout << "Date:    " << emh.Date()      << endl;
   cout << "Subject: " << emh.Subject()   << endl;
}

bool MailBot01::StatUserDestination(const File& file, StdLog& log)
{
   IniFile ini(file);
   ini.Load();
   return StatUserDestination(ini, log);
}

bool MailBot01::StatUserDestination(IniFile& file, StdLog& log)
{
   StdString sDir = GetPopFolder(file);
   Directory dir;
   if(dir.Name(sDir) == false)
      {
      log.PrintLine(StdString("Error: Unable to locate folder \"") + sDir + "\"");
      return false;
      }
   return true;
}


void MailBot01::ListConsole(IniFile& file, EMailHeader& emh, size_t iTally)
{
   // TODO: Integrate with WinCon01, then with Main()
}

bool MailBot01::List(IniFile& file, StdLog& log, ostream& cout)
{
   if(StatUserDestination((IniFile&)file, log) == false)
      return false;

   Directory dir;
   dir.Name(GetPopFolder(file));

   log.PrintLine(StdString("Listing the content of \"") + dir.Name() + "\"");
   Array<File> files;
   dir.Query(files);
   size_t iTally = 0L;
   for(size_t ss = 0L; ss < files.Nelem(); ss++)
      {
      EMailHeader emh;
      if(emh.Import(files[ss]) == true)
         {
         iTally++;
         ListItem(files[ss], emh, iTally, cout);
         }
      }
   stringstream srm;
   srm << "Found " << iTally << " valid message headers." << endl;
   StdString str = srm.str().c_str();
   cout << endl << str << endl;
   log.PrintLine(str);
   return true;
}

bool MailBot01::CreateDefault(const File& file, StdLog& log)
{
   if(file.Exists())
      {
      log.PrintLine(StdString("Warning: CreateDefault - File ") + file.Name() + " already exists. (not replaced)");
      return true;
      }

   if(IniSet::AppendEpop(file) == false)
      {
      log.PrintLine(StdString("Error: CreateDefault - Unable to save file ") + file.Name());
      return false;
      }

   log.PrintLine(StdString("Success: Created ") + file.Name() + ".");
   return true;
}

bool MailBot01::Main(Array<StdString>& aArgs, StdLog& log)
{
	log.PrintLine(ProgramByline());

   char chOption = 'h';
   File iniFile;
   if(QueryIni(iniFile) == false)
      {
      log.PrintLine("Error: Unable to stat default INI file!");
      return false;
      }

   size_t ssWhich = 0L;

   for(int ss = 1; ss < aArgs.Nelem(); ss++)
      {
      StdString str = aArgs[ss];
      if(str.find("-c") != npos)
         {
         chOption = 'c';
         continue;
         }
      if(str.find("-l") != npos)
         {
         chOption = 'l';
         continue;
         }
      if(str.find("-p") != npos)
         {
         chOption = 'p';
         continue;
         }
      if(str.find("-m") != npos)
         {
         chOption = 'm';
         continue;
         }
      if(str.find("-o") != npos)
         {
         chOption = 'o';
         continue;
         }
      if(str.find("-x") != npos)
         {
         chOption = 'x';
         if(ss +1 != aArgs.Nelem())
            {
            ss++;
            ssWhich = aArgs[ss].as_long();
            }
         continue;
         }
      if(str.find("-d") != npos)
         {
         chOption = 'd';
         continue;
         }
      if(str.find("-i") != npos)
         {
         int iPos = str.rfind('=');
         if(iPos != npos)
            {
            str = str.subpos(iPos+1, str.length());
            str.strip(); // jic
            File file = FindIni(str, log);
            if(file.IsNull())
               {
               log.PrintLine(StdString("Error: Unable to locate \"") + str + "\"?");
               continue;
               }
            iniFile = file;
            }
         continue;
         }

      }

	return Main(iniFile, chOption, ssWhich, log);
}


bool MailBot01::Main(const File& ifn, char chOption, int ssWhich, StdLog& log)
{
   // The INI is not found -
   if(chOption == 'c')
      return CreateDefault(ifn, log);

   // The INI is not needed -
   if(chOption == 'h')
      {
      Usage(cout);
      return true;
      }

   // The INI is not needed -
   if(chOption == 'p')
      {
      // View the pdf help file -
      ProgramId pid;
      File file;
      file.Name("EPOP.PDF");
      if(pid.Home(file) == false)
         {
         log.PrintLine("Error: Unable to stat program folder?!");
         return false;
         }
      if(file.Launch() == false)
         {
         log.PrintLine(StdString("Error: '") + file.Name() + "' not opened.");
         return false;
         }
      return true;
      }

   // Now we can assume the INI exists -
   if(ifn.Exists() == false)
      {
      log.PrintLine("Error: Unable to locate the .INI file.");
      return false;
      }

   // Now we can cache it  -
   IniFile iniFile(ifn);
   if(iniFile.Load() == false)
      {
      log.PrintLine("Error: Unable to load the .INI file.");
      return false;
      }

   switch(chOption)
      {
      case 'm':
         {
         // View the message folder -
         StdString str = GetPopFolder(iniFile);
         Directory dir;
         if(dir.Name(str) == false)
            {
            log.PrintLine("Error: '" + dir.QueryNode() + "' is an invalid output folder");
            return false;
            }
         if(dir.Exists() == false)
            {
            log.PrintLine("Error: '" + dir.QueryNode() + "' not found.");
            return false;
            }
         if(dir.Launch() == false)
            {
            log.PrintLine("Error: '" + dir.QueryNode() + "' not opened.");
            return false;
            }
         return true;
         }
      case 'o':
         {
         // View the log folder -
         ProgramId pid;
         Directory dir;
         if(pid.GetProgram(dir) == false)
            {
            log.PrintLine("Error: Unable to stat program folder?!");
            return false;
            }
         if(dir.Launch() == false)
            {
            log.PrintLine("Error: '" + dir.QueryNode() + "' not opened.");
            return false;
            }
         return true;
         }
      case 'x':
         return Move(iniFile, log, ssWhich, cout);

      case 'l':
         return List(iniFile, log, cout);

      default:

      case 'd':   // go for it!
         break;
      }

   // Execute POP retreval operations
   HostName host;
   host.Name(GetPopServer(iniFile));
   host.Port(GetPopPort(iniFile).as_int());

   UserData user;
   user.User(GetPopUser(iniFile));
   user.Pass(GetPopPassword(iniFile));

   StdString str = GetPopDebug(iniFile);
   str.to_lower();
   str.strip();
   bool bDebug = true;
   if(str.find("false") != npos)
      bDebug = false;

   str = GetPopFolder(iniFile);
   Directory dir;
   if(dir.Name(str) == false)
      {
      log.PrintLine(StdString("Error: Invalid directory name - ") + str);
      return false;
      }


	bool br = Thug::Download(host, user, dir, log, !(bDebug), bDebug); // DEBUG(true) == REMOVE(false)
   if(br == false)
      {
      stringstream srm;
      srm << "Error: - ";
      host.GetError(str);
      srm << str;
      srm << " / ";
      user.GetError(str);
      srm  << str;
      srm << ends;
      log.PrintLine(srm.str().c_str());
      }
   else
      log.PrintLine(StdString("Success: ") + ProgramName());
   return br;
}
