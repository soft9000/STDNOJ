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
#ifndef _SmallChange_Hppp
#define _SmallChange_Hppp

#include "HttpLogDatabase.hpp"
#include <iuo/SetComma.hpp>

struct SmallChangeCommand
   {
   StdString sOpcode;
   StdString sOperand;

   const char *Compose(StdString& str)  {str = sOpcode; str += " "; str += sOperand; return str.c_str();}

   ostream& write(ostream& os)         {sOpcode.write_stream(os); sOperand.write_stream(os); return os;}
   istream& read(istream& is)          {sOpcode.read_stream(is); sOperand.read_stream(is); return is;}
   };

class SmallChangeOptions
{
protected:
   StdString sOptions;
   File fileOptions;
public:
   Directory dirRepository;
   Array<SmallChangeCommand> aCommands;

   SmallChangeOptions(void);

   bool IsMacroMode(void)        {return (sOptions[0] == '-') ? false : true;}
   void IsMacroMode(bool b)      {sOptions[0] = (b == true) ? '+' : '-';}

   bool Load(void);
   bool Save(void);

   ostream& write(ostream& os)   {dirRepository.write(os); aCommands.write(os); sOptions.write_stream(os); return os;}
   istream& read(istream& is)    {dirRepository.read(is);  aCommands.read(is);  sOptions.read_stream(is);  return is;}
};

class SmallChange
   {
   protected:
      class FileStats
         {
         public:
            StdString      sName;
            long unsigned  luBytes;
            long unsigned  luHits;

            FileStats() : luBytes(0L), luHits(0L) {}
         };

      void     SortByHits(Array<FileStats>& aStats);
      void     QueryStats(Array<HttpLogEntry>&, Array<FileStats>&);
      size_t   FnLookup(Array<FileStats>& aStats, const StdString& sCommand);
      void     GetDefaultInit(DatabaseQuery& dbqResult);    // restore query defaults
      bool     SetDefaultInit(DatabaseQuery& dbqResult);    // save defaults

      void     FileHistogram(Array<HttpLogEntry>& aData, ostream& os);

      // Opcodes
      bool opAdd(HttpLogDatabase& dbLog, const StdString& sFile);
      bool opRemove(HttpLogDatabase& dbLog, const StdString& sFile);

      bool opChangeRepository(HttpLogDatabase& dbLog, const StdString& sParameter);
      bool opList(HttpLogDatabase& dbLog, const StdString& sParameter);

      bool opFilesReport(HttpLogDatabase& dbLog, const StdString& sParameter);
      bool opFileReport(HttpLogDatabase& dbLog, const StdString& sParameter);

      bool opGlobalFilter(HttpLogDatabase& dbLog, const StdString& sParameter);

   public:
      SmallChangeOptions options;            // is automatically loaded

      void Usage(ostream&);

      bool Exec(const StdString& sOperand);  // execute stored program
      bool Exec(SmallChangeCommand& cmd);
      bool Exec(const StdString& sOpcode, const StdString& sOperand);
   };


#endif
