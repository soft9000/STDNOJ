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

//! EzLog demonstrates how to easily create a stdnoj tool. this tool allows
//! simple messages to be time-stamped and added / listed to either the `pwd`, or
//! to an .ezlog file in the parent's tree or in the default program (".EXE") folder.
//!
//! This 'noj is very useful when you are generating comments on an entire TREE of 
//! files, folder and / or directories.
//! 
//! NOTE THAT LOG FILE USAGE IS OPTOMISTIC - NO LOG FILE CONTENTION LOGIC IS USED AT THIS TIME!
//!
//! See the ReadMe.txt, or EzLog::Usage() for more information.
//!

// 2010/03/03: Added the ability to export the TAB delimited rendition of the log file report.
//

#ifndef EzLog_Hpp
#define EzLog_Hpp

#include <server/StdLog.hpp>

class EzLog
{
protected:
   File pwFile;
   
public:

   EzLog(void);

   void setDefault(void);                    // Set the log to ".ezlog" in the main program folder
   bool setDefaultLog(Directory& dir);       // Sets the log to a file called ".ezlog" in the folder
   bool setDerivedLog(File& file);           // Derive a fle name by appending ".ezlog" to the file+path
   void locateDefaultLog(void);              // Searches up folder parentage looking for a log file, else uses the default -

   bool showLog(ostream& os);                // Word-wrap and display the log file
   bool exportLog(ostream& os);              // Export TAB delimited format to stdout

   bool writeLog(const StdString& message);  // Obvio -
   bool writeLog(const StdString& message, StdString& sResult);// ibid -
   
   const char *logName(void)  {return pwFile.Name();}          // Get the pw log file name
   
   static bool showLog(const File& file, ostream& os);         // First try the HTML version, else the testual -
   static bool exportLog(const File& file, ostream& os);       // Export TAB delimited format to stdout
   static bool showHtmlLog(const File& file);                  // Display the log file in HTML format - TRUE if LAUNCHED
   static bool showTextLog(const File& file, ostream& os);     // Display the log file in word-wrapoed text format -
   static const char *logDefaultName(StdString& str);          // Get the default name
   
   static bool hasDefaultLog(Directory& dir);

   static void Usage(ostream& os);                             // Command-line usage
   static bool Main(Array<StdString>& array, ostream& os);     // Command-line operations
   static bool Main(int argc, char *argv[], ostream& os);      // Command-line 'stuffing
};

#endif
