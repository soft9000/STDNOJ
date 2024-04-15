/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2024 R.A Nagy 

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
#include "HttpLogDatabase.hpp"


bool DatabaseQuery::dqInput::ShouldInclude(const HttpLogEntry& log)
   { 
    switch(qtRequest)
      {
      case qtCommandPatternMatch:
         break;
      case qtUnknown:
      default:
         return false;
      } 
   if(log.sRequest.find(sParameter, 0, 1) != NPOS)
      {
      bool bInclude = true;
      if(dtFilter != dtNone)
         {
         StdDateTime sdt;
         log.QueryDate(sdt);
         switch(dtFilter)
            {
            case dtOn:
               if(sdtFilter.IsSameDate(sdt) == false)
                  bInclude = false;
            break;
            case dtBefore:
               if(sdt >= sdtFilter)
                  bInclude = false;
            break;
            case dtAfter:
               if(sdt <= sdtFilter)
                  bInclude = false;
            break;
            }
         }
      if(bInclude == false)
         return bInclude;  // single breakpoint
      for(size_t ss = 0L; ss < aExcludePatterns.Nelem(); ss++)
         {
         if(log.sRequest.find(aExcludePatterns[ss]) != NPOS)
            return false;
         }
      char ch = log.sResultCode[0];
      if(ch == '1' && bExclude1)
         return false;
      if(ch == '2' && bExclude2)
         return false;
      if(ch == '3' && bExclude3)
         return false;
      if(ch == '4' && bExclude4)
         return false;
      if(ch == '5' && bExclude5)
         return false;

      // If you got this far, then all tests have passed
      return true;
      }
   return false;
   }
ostream& DatabaseQuery::dqInput::write(ostream& os)
   {
   sParameter.write(os);
   sdtFilter.write(os);

   StdString str;
   str.assign(iLimit);
   str.write_stream(os);

   str = "------------------------------";
   str[0] = (char)dtFilter;
   str[1] = (char)qtRequest;
   str.write_stream(os);
   Array<StreamString> arySpace; // The final frontier...
   arySpace.write(os);
   return os;
   }
istream& DatabaseQuery::dqInput::read(istream& is)
   {
   sParameter.read(is);
   sdtFilter.read(is);

   StdString str;
   str.read_stream(is);
   iLimit = str.as_int();

   str.read_stream(is);
   dtFilter = (DateTimeFilter)   str[0];
   qtRequest= (QueryType)        str[1];
   Array<StreamString> arySpace; // The final frontier...
   arySpace.read(is);
   return is;
   }



bool HttpLogDatabase::Use(Directory& dir)
   {
   dirData = dir;
   dirData.Create();
   return dir.Exists();
   }
void HttpLogDatabase::Cache(void)
   {
   // <<<WIP>>> Still need to implement an internal cache load & use.
   }
void HttpLogDatabase::DeCache(void)
   {
   }
bool HttpLogDatabase::Query(DatabaseQuery& dq, bool bNormalize)
   {
   StdString str;
   Array<File> aFiles;
   dirData.Query(aFiles);
   return Query(dq, aFiles, bNormalize);
   }

bool HttpLogDatabase::Query(DatabaseQuery& dq, Array<File>& aFiles, bool bNormalize)
   {
   dq.out.Reset();
   // Open each physical file & load the data.
   // Would b a good place to cache, but these can be huge.
   // Leave that decision to the user...
   File pwFile;
   for(size_t ss = 0L; ss < aFiles.Nelem(); ss++)
      {
      HttpLog log;
      pwFile = aFiles[ss];
      istream& is = pwFile.OpenRead(File::AT_BINARY);
      log.read(is);
      if(!is)
         {
         dq.out.sError = "Error: Unable to open log file: ";
         dq.out.sError.append(aFiles[ss].Name());
         return false;
         }
      if(bNormalize)
         log.Normalize();
      if(AddQuery(log, dq) == false)
         {
         // Error is posted by call
         return false;
         }
      pwFile.Close();     // 12/17/2003 Bugfix: Stopped on 8 (file handle limit), Randall Nagy
      }
   return true;
   }
bool HttpLogDatabase::AddQuery(HttpLog& log, DatabaseQuery& dq)
   {
   // Scan the array for result size, first (massive performance improvement)
   size_t max = 0L;
   size_t ss = 0L; // borland 6 -v- ms 6 compilers (sigh)
   for(ss = 0L; ss < log.aLines.Nelem(); ss++)
      {
      if(dq.in.iLimit && (max >= dq.in.iLimit))
         break;
      if(Query(log.aLines[ss], dq, false) == true)
         max++;
      }
   if(!max)
      return true;
   dq.out.aResults.AddZombies(max + 1);

   // Perform the match operation again, this time appending the results
   for(ss = 0L; ss < log.aLines.Nelem(); ss++)
      {
      if(dq.in.iLimit && (dq.out.aResults.Nelem() >= max))
         break;
      Query(log.aLines[ss], dq);
      }
   return true;
   }
bool HttpLogDatabase::Query(HttpLogEntry& log, DatabaseQuery& dq, bool bShould)
   {
   bool bInclude = false;
   bInclude = dq.in.ShouldInclude(log);            
   if(bShould && bInclude)
      dq.out.aResults[dq.out.aResults.Nelem()] = log;
   return bInclude;
   }
void HttpLogDatabase::Dir(Array<StdString>& aLogs)
   {
   StdString str;
   Array<File> aFiles;
   dirData.Query(aFiles);

   for(size_t ss = 0L; ss < aFiles.Nelem(); ss++)
      aLogs[aLogs.Nelem()] = aFiles[ss].QueryNode(str);
   }
StdString HttpLogDatabase::mkPath(const StdString& sNodeName)
   {
   StdString sPath;
   sPath = dirData.Name();
   sPath.append("\\");
   sPath.append(sNodeName);
   return sPath;
   }
bool HttpLogDatabase::Create(HttpLog& log, const StdString& sNodeName)
   {
   StdString sPath = mkPath(sNodeName);
   File file;
   if(file.Name(sPath) == false)
      return false;
   if(file.Exists() == true)
      return false;
   ostream& os = file.OpenWrite(File::AT_BINARY);
   log.write(os);         
   return true;
   }
bool HttpLogDatabase::Read(HttpLog& log, const StdString& sNodeName)
   {
   StdString sPath = mkPath(sNodeName);
   File file;
   if(file.Name(sPath) == false)
      return false;
   if(file.Exists() == false)
      return false;
   istream& is = file.OpenRead(File::AT_BINARY);
   log.read(is);         
   return true;
   }
bool HttpLogDatabase::Update(HttpLog& log, const StdString& sNodeName)
   {
   StdString sPath = mkPath(sNodeName);
   File file;
   if(file.Name(sPath) == false)
      return false;
   ostream& os = file.OpenWrite(File::AT_BINARY);
   log.write(os);         
   return true;
   }
void HttpLogDatabase::Delete(const StdString& sNodeName)
   {
   StdString sPath = mkPath(sNodeName);
   File file;
   if(file.Name(sPath) == false)
      return;
   file.Remove();
   }