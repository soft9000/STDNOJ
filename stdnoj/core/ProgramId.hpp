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
#ifndef nojProgramId_Hpp
#define nojProgramId_Hpp

// ProgramId is an encapsulation that every operating system can support.
// Its purpose is to allow you to know what, and where, your program is running.
// It also allow you to manipulate files and folders relative to both a system
// wide, and a program relative, location. Very important for IPC and persistance.
//
// 04/28/1998: Class extracted from EzInstall base, Randall Nagy
// 09/07/1998: Replaced string with StdString to use in Job*NET, Randall Nagy.
// 11/23/1998: Added SetProgramDirectory(), Randall Nagy.
// 06/12/1999: Added PlaceInDirectory(), Randall Nagy.
// 06/14/1999: Added TempFileName(), Randall Nagy.
// 06/15/1999: Added PlaceInTempDirectory(), Randall Nagy.
// 11/25/1999: Added TempFileName(with suffix), Randall Nagy
// 12/22/1999: Added GetRepository(), Randall Nagy
// 08/24/2000: Added PlaceInSubDirectory() concept, Randall Nagy
// 01/01/2003: Added QuerySystemDirectory(), Randall Nagy
// 08/30/2003: Re-written as part of port to the noj framework, Randall Nagy
// 09/19/2003: Added PutInDirectory(Directory&, File&), Randall Nagy
// 12/03/2008: Added SetProgram(File&) to support DOS / UNIX, as well as to over-ride WIN32 API, Randall Nagy
//             Re-wrtten to support MSDOS, UNIX, and WIN32.
// Note: DOS and UNIX programs MUST call SetProgram() before using this class.
//       WIN32 programs do not need to, but should now do so none the less (portability).
// 01/11/2006: Added Home(File), Randall Nagy
// 02/24/2006: Added Home(Directory), Randall Nagy
// 05/06/2006: Renamed PlaceInSubDirectory() to Concat(), Randall Nagy
//             Added Home(sDir, sNode, chPath), Randall Nagy

#include <stdnoj.hpp>

namespace stdnoj {
    // RESULT CONVENTION
    // -----------------
    // Unless otherwise noted, file and folder objects do not exist until you create them.
    // Except for pre-exisitng system and executable paths or files, results returned
    // are fully qualified, but not necessarily existing, file system elements.
    //

    class ProgramId {
    private:
        File fProgramOverride;

    public:

        ProgramId(void) {
        }

        ~ProgramId(void) {
        }

        bool SetProgram(const StdString &sFQFileName); // argv[0] = Required under DOS and UNIX - Overides platform API on WIN32.

        bool GetSystem(Directory& dir) const; // This is the WINDOWS (system) directory. On unix, /etc might be a good place. Must be an pre-exisitng operating system defined location, and non-transient.
        bool PutInDirectory(const Directory& dir, File& file) const; // Place in the directory

        bool GetProgram(Directory& dir) const; // File where the executable resides
        bool GetProgram(File& file) const; // Fully qualified executable
        bool PutInDirectory(File& file) const; // Place the file in the default (program) directory
        bool PutInDirectory(Directory& dir) const; // Place the leaf in the default (program) directory
        bool Home(File& file) const; // Place the file in the program's home (start-up) location
        bool Home(Directory& file) const; // Place the file in the program's home (start-up) location
        StdString
        Home(const StdString sSubDir, const StdString& sNode, char sep = 0) const; // Place the file in the program's home (start-up) + sub dir location

        // Checks the TEMP environment variable first, then defaults to creating
        // a TEMP dir on the program root. On WIN32, the last chance will use the
        // WIN32 API if SetProgram() has not been called (legacy support).
        bool GetTemp(Directory& dir) const; // A place for temporary files
        bool GetTemp(File& file) const; // Create a unique temporary file
        bool PutInTemp(File& file) const; // Put the file into the temp folder
        bool PutInTemp(Directory& dir) const; // Put the directory tree into the temp dir

        // Call "place in directory" for the sub-directory reference:
        bool PutInSubDirectory(const Directory& dirSub, File& file) const;

        // Brute force: Just concatnate them together, and return the result;
        StdString Concat(const StdString& sSubdir, const StdString& sItem, char sep) const;
        StdString Concat(const StdString& sSubdir, const StdString& sItem) const;
        char PathSep(void) const;
    };

    inline
    char ProgramId::PathSep(void) const {
        char ch = '\\';
#ifdef UNIX
        ch = '/';
#endif
        return ch;
    }

}
#endif


