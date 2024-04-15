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
// 12/17/2003 Bugfix: Closed file in array after parsing. Stopped on 8 (file handle limit), R. Nagy

#ifndef _Http_Log_Db
#define _Http_Log_Db

#include <Server/HttpLogParser.hpp>

enum QueryType
   {
   // Lits of others here, but we are woring on the "what is good for me"
   // principle as the aid to incremental development.
   qtCommandPatternMatch = 'a',
   qtUnknown             = 'z'
   };

struct DatabaseQuery
   {
   public:
      DatabaseQuery(void)
         {
         in.Reset();
         out.Reset();
         }
      struct dqInput
         {
         private:
            StdDateTime    sdtFilter;
            enum DateTimeFilter
               {
               dtBefore = '-',
               dtAfter  = '+',
               dtOn     = '=',
               dtNone   = '0'
               } dtFilter;
         public:
            bool bExclude1;
            bool bExclude2;
            bool bExclude3;
            bool bExclude4;
            bool bExclude5;
            QueryType qtRequest;
            StdString sParameter;
            int       iLimit;    // 0 == "unlimited", other is a pre-allocation amount for the result array

            // Use to omit problem patterns (e.g. cmd.exe site hacking attempts)
            Array<StdString> aExcludePatterns;

            // Limit results to data within a certain range (inclusive)
            void IncludeOn(StdDateTime& dt)     
               {
               sdtFilter = dt; 
               dtFilter = dtOn;
               }
            void IncludeBefore(StdDateTime& dt) 
               {
               sdtFilter = dt; 
               dtFilter = dtBefore;
               }
            void IncludeAfter(StdDateTime& dt)  
               {
               sdtFilter = dt; 
               dtFilter = dtAfter;
               }
            void Reset(void)
               {
               qtRequest   = qtUnknown;
               dtFilter    = dtNone;
               sParameter  = "";
               iLimit      = 100;
               bExclude1 = bExclude2 = bExclude3 = bExclude4 = bExclude5 = false;
               }
            bool ShouldInclude(const HttpLogEntry& log);

            ostream& write(ostream& os);
            istream& read(istream& is);

         } in;

      struct dqOutput
         {
         StdString           sError;
         Array<HttpLogEntry> aResults;
         void Reset(void)
            {
            sError = "";
            aResults.Empty();
            }
         } out;
   };

class HttpLogDatabase
{
private:
   Array<HttpLog> aLogCache;  // If cache is populated, then it is used (exclusively)

protected:
   Directory dirData;
   StdString mkPath(const StdString& sNodeName);

public:
   HttpLogDatabase(void)  {}

   bool Use(Directory& dir);

   void Cache(void);       // Attempt to load all log files to reduce search time
   void DeCache(void);     // Unload all cached log files

   bool Query(DatabaseQuery&, bool bNormalize = true);
   bool AddQuery(HttpLog&, DatabaseQuery&);
   bool Query(HttpLogEntry& log, DatabaseQuery& dq, bool bShouldAddToResult = true);
   bool Query(DatabaseQuery& dq, Array<File>& aFiles, bool bNormalize);

   void Dir(Array<StdString>& aLogs);

   bool Create(HttpLog&, const StdString& sNodeName);
   bool Read(HttpLog&, const StdString& sNodeName);
   bool Update(HttpLog&, const StdString& sNodeName);
   void Delete(const StdString& sNodeName);
};
#endif
