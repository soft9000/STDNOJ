/* The MIT License (Open Source Approved)

Copyright (c) 1993 - 2024 Randall Nagy 

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
#include <Extra/StdLog.hpp>
#include <cstdio>         // unlink()
#include <ctime>


void StdLog::_init(void)
   {
   Enable();
   // Every executable has their own log - this is the default
   ProgramId pid;
   File file;
   pid.GetProgram(file);
   StdString str = file.Name();
   str += ".log";
   pwFile.Name(str);
   }


StdLog::StdLog(void)
   {
   _init();
   }


void StdLog::Disable(void)
   {
   bEnabled = false;
   }


void StdLog::Enable(void)
   {
   bEnabled = true;
   }


StdLog::StdLog(const File& file)
   {
   _init(); // just in case we as more than that...
   pwFile = file;
   }


StdLog::~StdLog(void)                                       
   {
   }


bool StdLog::Assign(const File& file)
   {
   pwFile = file;
   return true;
   }


bool StdLog::Query(File& file)
   {
   file = pwFile;
   return true;
   }


bool StdLog::MoveTo(Directory& dir)
   {
   if(dir.Exists())
      {
      StdString sBuf;
      File file;
      Query(file);
      file.QueryNode(sBuf);
      StdString str = StdString(dir.Name()) + StdString(PathChar()) + sBuf;
      return pwFile.Name(str);
      }
   return false;
   }


StdString StdLog::Format(const StdString& cstr)
   {
	char		*pszBuf = new char[60];
	time_t	Dtimer;
	tm			*pStaticTmBuffer;

	// Time-stamp the log-entry;
	time(&Dtimer);
	pStaticTmBuffer = localtime(&Dtimer);
	::strftime(pszBuf, 59, "%m/%d/%y  %H:%M:%S    ", pStaticTmBuffer);	// Allows other countries use their own time formats...

   stringstream srm;
	srm << pszBuf << cstr;
   srm << ends;

   StdString sLine = PRESTO(srm);
   delete [] pszBuf;

   return sLine;
   }


bool StdLog::_append(const StdString& sLine)
   {
   if(bEnabled)
      {
	   ostream& os = pwFile.OpenAppend();
      if(!os)
         return false;
      os << sLine;
      pwFile.Close();
      }
   return true;
   }


StdString StdLog::Print(const StdString& cstr)
   {
	StdString sLine = Format(cstr);
   if(_append(sLine) == false)
      sLine = "";
   return sLine;
   }


StdString StdLog::PrintLine(const StdString& cstr)
   {
   stringstream srm;
   srm <<  Format(cstr) << endl;
   StdString sLine = PRESTO(srm);
   if(_append(sLine) == false)
      sLine = "";
   return sLine;
   }

/*
StdString StdLog::Print(ostream& cout, const StdString& pszBuf)
   {
   StdString sLine;
   sLine = Print(pszBuf);
   if(sLine.is_null())
      sLine = "";
   else
      if(_append(sLine) == false)
         sLine = "";
   return sLine;
   }


StdString StdLog::PrintLine(ostream& cout, const StdString& pszBuf)
   {
   stringstream srm;
   srm << Print(pszBuf) << endl;
   StdString sLine;
   sLine = PRESTO(srm);
   if(_append(sLine) == false)
      sLine = "";
   return sLine;
   }
*/

bool StdLog::Delete(void)
   {
   return pwFile.Remove();
   }

 
