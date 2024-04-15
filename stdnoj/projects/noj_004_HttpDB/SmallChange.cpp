/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2024 R.A. Nagy 

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
// SmallChange: Project to demonstrate stdnoj capabilities by creating a 
// programable, system-wide HTTP log repository and query capability.
//
// 10/11/2003: Added command and option storage ability, R. Nagy
//
#include "SmallChange.hpp"

SmallChangeOptions::SmallChangeOptions(void)
   {
   sOptions = "------------------";
   dirRepository.Name("HttpRepository");
   ProgramId pid;
   pid.PutInDirectory(dirRepository);
   dirRepository.Create();
   fileOptions.Name("HttpRepository.1");
   pid.PutInDirectory(fileOptions);
   // Do this last to enforce any user changes to above:
   Load();
   }
bool SmallChangeOptions::Load(void)
   {
   bool br = false;
   if(fileOptions.Exists())
      {
      istream& is = fileOptions.OpenRead(File::AT_BINARY);
      read(is);
      if(is)
         br = true;
      fileOptions.Close();
      }
   return br;
   }
bool SmallChangeOptions::Save(void)
   {
   fileOptions.Remove();
   ostream& os = fileOptions.OpenWrite(File::AT_BINARY);
   write(os);
   fileOptions.Close();
   return fileOptions.Exists();
   }

void SmallChange::Usage(ostream& ccc)
   {
   ccc << endl;
   ccc << " HttpDb: PROGRAM to cache and query standard HTTP Server log files." << endl;
   ccc << "         Version \"" << __DATE__ << "\", by R.A. A. Nagy (rnagy@soft9000.com)" << endl;
   ccc << "         Distribution is unlimited. Educational use only. Use at your own risk!" << endl;
   ccc << endl;
   ccc << " Usage:   " << "HttpDb option [\"parameter\"]" << endl;
   ccc << " Example: " << "HttpDb + \"d:\\tmp\\soft9000.log\"" << endl;
   ccc << endl;
   ccc << " -------------- The Cache Set:"  << endl;
   ccc << "+ file_name      ADD    Cache server log (add to repostiory.)"  << endl;
   ccc << "- cache_name     DEL    Remove cache file from repostiory."     << endl;
   ccc << "/ folder_name    CD     Change repository location."            << endl;
   ccc << "=                DIR    Repostiory contents." << endl;
   ccc << " -------------- The Query Set:"   << endl;
   ccc << "? pattern        TALLY  Single pattern match (http target)."    << endl;
   ccc << "* pattern        CHART  Multi-pattern match (http target)."     << endl;
   ccc << "~ pattern        LIMIT  Limit subsequent queries (SEE README)." << endl;
   ccc << " -------------- The Macro Set:" << endl;
   ccc << "{                NEW    Begin replacement of default macro."    << endl;
   ccc << ";                UPDATE Begin adding to the default macro."     << endl;
   ccc << "}                END    End macro command storage mode."        << endl;
   ccc << "#                LIST   Enumerate macro content."               << endl;
   ccc << "! step_number    REMOVE Remove macro step from list."           << endl;
   ccc << "$ [report_title] EXEC   Run macro."                             << endl;
   return;
   }
bool SmallChange::Exec(const StdString& sOperand)
   {
   StdString str;
   cout << endl;
   if(!sOperand.is_null())
      {
      // Entitle, if you got it...
      cout << "::::: " << sOperand << " :::::" << endl;
      }
   cout << endl;
   for(size_t ss = 0L; ss < options.aCommands.Nelem(); ss++)
      {
      cout << endl;
      options.aCommands[ss].Compose(str);
      cout << ss << ".) " << str << endl;
      if(Exec(options.aCommands[ss]) == false)
         {
         cerr << "\t*** Error ***." << endl;
         cerr << endl;
         return false;
         }
      }
   return true;
   }
bool SmallChange::Exec(const StdString& sOpcode, const StdString& sOperand)
   {
   SmallChangeCommand cmd;
   cmd.sOpcode = sOpcode;
   cmd.sOperand = sOperand;
   return Exec(cmd);
   }
bool SmallChange::Exec(SmallChangeCommand& cmd)
   {
   bool br = false;
   HttpLogDatabase dbLog;
   if(dbLog.Use(options.dirRepository) == false)
      {
      cerr << "Unable to stat local repository" << endl;
      return false;
      }

   // If the macro mode is in pester, then save the commands for latter use:
   if(options.IsMacroMode())
      {
      switch(cmd.sOpcode[0])
         {
         // No mode changes
         case '#':   // ignore the macro list command
         case '!':   // ignore the macro delete command
            {
            options.IsMacroMode(false);
            br = Exec(cmd);
            options.IsMacroMode(true); // This just saved the "macro off" mode...
            br = options.Save();       // so we need to turn it back on, again
            return br;
            }
         break;
         // Implicit mode changes
         case '}':   // ignore the macro end command
         case ';':   // ignore the macro update command
         case '{':   // ignore the macro reset command
         case '$':   // ignore the macro execute command
            {
            options.IsMacroMode(false);
            br = options.Save();
            return Exec(cmd);
            }
         break;
         default:
            {
            cout << "MACRO MODE" << endl;
            cout << "==========" << endl;
            StdString str;
            options.aCommands[options.aCommands.Nelem()] = cmd;
            br = options.Save();

            // Use ourself to automatically do the list thang:
            SmallChangeCommand cmd2;
            cmd2.sOpcode = '#';
            options.IsMacroMode(false);
            Exec(cmd2);
            options.IsMacroMode(true);
            return br;
            }
         break;
         }
      }

   switch(cmd.sOpcode[0])
      {
      case '+':
         {
         br = opAdd(dbLog, cmd.sOperand);
         if(br == true)
            opList(dbLog, cmd.sOperand);
         }
      break;
      case '-':
         {
         br = opRemove(dbLog, cmd.sOperand);
         if(br == true)
            opList(dbLog, cmd.sOperand);
         }
      break;
      case '=':
         {
         opList(dbLog, cmd.sOperand);
         }
      break;
      case '?':
         {
         br = opFileReport(dbLog, cmd.sOperand);
         }
      break;
      case '*':
         {
         br = opFilesReport(dbLog, cmd.sOperand);
         }
      break;
      case '~':
         {
         br = opGlobalFilter(dbLog, cmd.sOperand);
         }
      break;
      case '/':
         {
         br = opChangeRepository(dbLog, cmd.sOperand);
         }
      break;
      case '{':
         {
         options.aCommands.Empty();
         options.IsMacroMode(true);
         br = options.Save();
         }
      break;
      case ';':
         {
         options.IsMacroMode(true);
         br = options.Save();
         }
      break;
      case '}':
         {
         options.IsMacroMode(false);
         br = options.Save();
         }
      break;
      case '!':
         {
         if(!cmd.sOperand.is_null())
            {
            options.aCommands.Remove(cmd.sOperand.as_int() - 1);
            br = options.Save();
            }
         }
      case '#':
         {
         StdString str;
         cout << endl;
         cout << "Default Macro" << endl;
         cout << "=============" << endl;
         for(size_t ss = 0L; ss < options.aCommands.Nelem(); ss++)
            cout << ss + 1 << ".) " << options.aCommands[ss].Compose(str) << endl;
         }
      break;
      case '$':
         {
         dbLog.Cache();
         br = Exec(cmd.sOperand);
         }
      break;
      }
   return br;
   }
size_t SmallChange::FnLookup(Array<FileStats>& aStats, const StdString& sCommand)
   {
   for(size_t ss = 0L; ss < aStats.Nelem(); ss++)
      {
      if(aStats[ss].sName == sCommand)
         return ss;
      }
   return NPOS;
   }
void SmallChange::SortByHits(Array<FileStats>& aStats)
   {
   for(size_t ss = 0L; ss < aStats.Nelem(); ss++)
      {
      for(size_t ss2 = ss+1; ss2 < aStats.Nelem(); ss2++)
         {
         if(aStats[ss].luHits > aStats[ss2].luHits)
            {
            // Swaperoo
            FileStats fsHold = aStats[ss];
            aStats[ss]       =  aStats[ss2];
            aStats[ss2]      = fsHold;
            }
         }
      }
   }
void SmallChange::QueryStats(Array<HttpLogEntry>& aData, Array<FileStats>& aStats)
   {
   // <<<WIP>>> We are assuming a name-sorted order... When more speed is needed, then make it so...
   aStats.Empty();
   StdString sName;
   // Scan the entire array
   for(size_t ss = 0L; ss < aData.Nelem(); ss++)
      {
      sName = aData[ss].FileName();
      size_t whence = FnLookup(aStats, sName);
      if(whence == NPOS)
         {
         whence = aStats.Nelem();
         aStats[whence].sName = sName;
         }
      aStats[whence].luHits++;
      aStats[whence].luBytes += aData[ss].sBytesSent.as_int();
      }
   }
bool SmallChange::opGlobalFilter(HttpLogDatabase& dbLog, const StdString& sParameter)
   {
   // <<WIP>> Need to implement the filtering capability (all else is ready - last step!)
   // The yet to be coined is encoding strategy for the parameter. Tells us either to 
   //    omit a pattern (e.d. `cmd.exe`), 
   //    use a date range (before, after, at), 
   //    set a numerical limit (results),
   //    dump a report on the present settings,
   //    remove all of the above (RESET ALL)
   //
   // Then we apply any deltas to ALL subsequent queries (save) using
   //    SetDefaultInit(DatabaseQuery& dbqResult);    // save defaults
   //
   // (note that the restore capability has already bee implemented for 
   // the two query types - just need to decode param and save - rest is ready to test.)
   return false;
   }
void SmallChange::FileHistogram(Array<HttpLogEntry>& aData, ostream& os)
   {
   Array<FileStats> aStats;
   QueryStats(aData, aStats);
   long unsigned ssSigma;
   long unsigned maxLen;
   ssSigma = maxLen = 0L;
   // Get hi / low range
   for(size_t ss = 0L; ss < aStats.Nelem(); ss++)
      {
      ssSigma += aStats[ss].luHits;
      if(aStats[ss].sName.length() > maxLen)
         maxLen = aStats[ss].sName.length();
      }
   SortByHits(aStats);
   // Display the results
   int expansion_factor = 22;
   maxLen++;      // spacer
   os << endl;
   for(ss = 0L; ss < aStats.Nelem(); ss++)
      {
      double ddd = double(aStats[ss].luHits) / double(ssSigma);
      int pct = int(ddd * expansion_factor);
      for(size_t stars = 0; stars < expansion_factor; stars++)
         {
         if(pct >= stars)
            os << "*";
         else
            os << "_";
         }
      os << " | " << aStats[ss].sName;
      for(size_t pad = aStats[ss].sName.length(); pad < maxLen; pad++)
         {
         os << " ";
         }
      os << "(" << ((int)(ddd * 100)) << "%)" << endl;
      }
   os << endl;
   if(aStats.Nelem())
      {
      StdString str;
      os << "\t" << "Target " << aStats[aStats.Nelem()-1].sName << " occurs ";
      os << CommaFormat(aStats[aStats.Nelem()-1].luHits, str) << " times." << endl;
      }
   }
bool SmallChange::opList(HttpLogDatabase& dbLog, const StdString& sParameter)
   {
   Array<StdString> aFiles;
   dbLog.Dir(aFiles);
   cout << options.dirRepository.Name() << endl;
   if(!aFiles.Nelem())
      {
      cerr << "(Repository is empty.)" << endl;
      return false;
      }
   for(size_t ss = 0L; ss < aFiles.Nelem(); ss++)
      {
      cout << ss + 1 << ".)\t" << aFiles[ss] << endl;
      }
   return true;
   }
bool SmallChange::opChangeRepository(HttpLogDatabase& dbLog, const StdString& sParameter)
   {
   Directory dir;
   if(dir.Name(sParameter) == false)
      return false;
   if(dir.Exists() == false)
      {
      if(dir.Create() == false)
         return false;
      }
   options.dirRepository = dir;
   options.Save();
   return true;
   }
void SmallChange::GetDefaultInit(DatabaseQuery& dbqResult)
   {
   ProgramId pid;
   File file;
   file.Name("DatabaseQuery1.sc");
   pid.PutInDirectory(file);
   if(file.Exists())
      dbqResult.in.read(file.OpenRead(File::AT_BINARY));
   }
bool SmallChange::SetDefaultInit(DatabaseQuery& dbqResult)
   {
   ProgramId pid;
   File file;
   file.Name("DatabaseQuery1.sc");
   pid.PutInDirectory(file);
   dbqResult.in.write(file.OpenWrite(File::AT_BINARY));
   return file.Exists();
   }
bool SmallChange::opFilesReport(HttpLogDatabase& dbLog, const StdString& sParameter)
   {
   DatabaseQuery dbq;
   GetDefaultInit(dbq);
   dbq.in.sParameter = sParameter;
   dbq.in.qtRequest = qtCommandPatternMatch;
   if(dbLog.Query(dbq) == false)
      {
      cerr << dbq.out.sError << endl;
      return false;
      }
   FileHistogram(dbq.out.aResults, cout);
   return true;
   }
bool SmallChange::opFileReport(HttpLogDatabase& dbLog, const StdString& sParameter)
   {
   DatabaseQuery dbq;
   GetDefaultInit(dbq);
   dbq.in.sParameter = sParameter;
   dbq.in.qtRequest = qtCommandPatternMatch;
   if(dbLog.Query(dbq) == false)
      {
      cerr << dbq.out.sError << endl;
      return false;
      }
   StdString str;
   cout << sParameter << " occurs " << CommaFormat(dbq.out.aResults.Nelem(), str) << " times." << endl;
   return true;
   }
bool SmallChange::opAdd(HttpLogDatabase& dbLog, const StdString& sFile)
   {
   File file;
   if(file.Name(sFile) == false)
      {
      cerr << "Error: " << sFile << " is not a valid file name." << endl;
      return false;
      }
   HttpLog log;
   HttpLogParser parser;
   if(parser.Import(log, file) == false)
      {
      cerr << "Error: Unable to import file into repository." << endl;
      return false;
      }
   
   StdString sNodeName;
   if(dbLog.Create(log, file.QueryNode(sNodeName)) == false)
      {
      cerr << "Error: Unable to create file in repository." << endl;
      return false;
      }
   return true;
   }
bool SmallChange::opRemove(HttpLogDatabase& dbLog, const StdString& sFile)
   {
   File file;
   if(file.Name(sFile) == false)
      {
      cerr << "Error: " << sFile << " is not a valid file name." << endl;
      return false;
      }

   HttpLog log;
   StdString sNodeName;
   dbLog.Delete(file.QueryNode(sNodeName));
   return true;
   }
