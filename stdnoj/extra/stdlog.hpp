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
// Class StdLogger: Class to allow for the creation of a fully-qualified log-file. Need to keep the log
// files from floatin' round! Also needed to get access to the Format() for GUI / non-stream based logging.
//
// 01/17/1998: StdLog class created, Randall Nagy.
// 01/21/1998: Broke-out StdLogger to return formatted string (Better for GUI, etc), Randall Nagy
//             Added the Print/PrintLine(os, psz, 1) idea form the StdSocket LOG capability.
// 09/19/2003: Ported StdLogger over to stdnoj/extra/StdLog.hpp, Randall Nagy
//             Added a few functions & changed poor constructor init logic.
// 10/15/2003: Lightly modernized the interface (i.e. use File for assignements of same), Randall Nagy
//             New! Every executable has their own log (default)
// 06/15/2003: Updated for legacy usage, Randall Nagy
// 12/04/2008: Added operator=, operator+=, Disable(), Enable(), _append() and 
//             changed all const char * return values to StdString, Randall Nagy
// 08/04/2005: Added LogFormat, Randall Nagy
// 09/15/2006: Added IsLogging(void) const, Randall Nagy
//
#ifndef zStdLogger_Hpp
#define zStdLogger_Hpp

#include <stdnoj.hpp>

using namespace stdnoj;


class StdLog
   {
   private:
      bool bEnabled;
      virtual void _init(void);

   protected:
      virtual bool _append(const StdString& sLine);

   public:
      File        pwFile;

      StdLog(void);
      StdLog(const File&);
      StdLog(const StdString& str)  {pwFile.Name(str);}
      virtual ~StdLog(void);

      virtual void Enable(void);   // The default: Enable printing to the log file - debug, etc...
      virtual void Disable(void);  // The option:  Disable printing to the log file - format only
      bool         IsLogging(void) const {return bEnabled;}

      bool         Delete(void);   // Remove the log file

      StdString    Name(void)                   const     {return pwFile.Name();}
      bool         Name(const StdString& str)             {return pwFile.Name(str);}
      bool         Name(const File& file)                 {return pwFile.Name(file.Name());}
      bool         Assign(const File& file);
      bool         Query(File& file);
      bool         MoveTo(Directory& dir);


      virtual StdString Format(const StdString& cstr);

      virtual StdString Print(const StdString& cstr);
      virtual StdString PrintLine(const StdString& cstr);

// obsolete
//      virtual StdString Print(ostream& os, const StdString& psz);
//      virtual StdString PrintLine(ostream& os, const StdString& psz);

      virtual ostream& write(ostream& os)
         {
         pwFile.write(os);
         return os;
         }
      virtual istream& read(istream& is)
         {
         pwFile.read(is);
         return is;
         }

      StdLog operator=(const StdString& str)
         {
         Delete();
         PrintLine(str);
         return *this;
         }
      StdLog operator+(const StdString& str)
         {
         Print(str);
         return *this;
         }
      StdLog operator+=(const StdString& str)
         {
         PrintLine(str);
         return *this;
         }
      };


inline StdString LogFormat(const StdString& msg, const StdString& sStrftime = "%m/%d/%Y %H:%M:%S - ")
   {
   size_t   iMax = sStrftime.length() * 5;
   char     *psz = new char[iMax];

   // Concoct the time prefix in the requested format
   time_t   pwTime;
   tm         *ptm;
   time(&pwTime);
   ptm = localtime(&pwTime);
   strftime(psz, iMax-1, sStrftime.c_str(), ptm);

   // Encode Result
   StdString sResult;
   stringstream srm;
   srm << psz << msg;
   srm << ends;
   sResult = PRESTO(srm);

   return sResult;
   }

#endif
