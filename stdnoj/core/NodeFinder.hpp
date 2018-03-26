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
// Folders: Set of classes to update the SubDirs() way of thinking.
// 10/02/1997: GetFiles tool created, R. Nagy
// 10/03/1997: Warning, Folder, FolderFinder, WarnNoFolder, WarnFindStopped classes, R. Nagy.
// 08/16/1999: Fixed bug that would result in dot-prefixed (WIN32) directories being ignored, rn.
// 10/07/1999: Dropped use of AFX header: Not that important to THIS file. Others can just use it
//    as-needed, with the required "nafxcwd.lib" in their link, as well.
// 01/14/2000: Renamed Folders.hpp FolderFinder.hpp. Moved inline functions off into cpp file.
//    Added support for QueryFolderReport, R. Nagy
//
// Portability note: Though the ability to find a node is system dependant, the Reporting
// classes could easily become a NodeReport, as the WIN32 data-structure being offered up to
// the File() and Report() functions could just as easily be a "Node".
//
// 06/01/2001: Added ShouldContinue(), R. Nagy
//
// 11/13/2008: Gutted class. Unifying implementation, R. Nagy 
//    Goal is to make it work on ALL platforms. Never completed ports.
//    Need to implement recursion with standard File / Directory logic.
//
#ifndef NodeFS_HPP
#define NodeFS_HPP

#include <stdnoj.hpp>
#include <core/NodeWalker.hpp>

// These are the seven (7) classes in this file;
// --------------------------------------------
class Warning;
class NodeReport;
class NodeFinder;
//class QueryNodeReport;

class WarnNoNode;
class WarnFindStopped;
class WarnNoFile; // NOT used by engine, buy provided for implementor's use

namespace stdnoj {

    class Warning {
    public:
        StdString sWarning;

        Warning(void) {
        }

        virtual ~Warning(void) {
        }

        Warning & operator =(const Warning& op) {
            sWarning = op.sWarning;
            return *this;
        }
    };

    class WarnNoNode : public Warning {
    public:

        WarnNoNode(const char *pwd) {
            sWarning = "WARNING: Node ";
            sWarning.append(pwd);
            sWarning.append(" not found!");
        }
    };

    class WarnNoFile : public Warning {
    public:

        WarnNoFile(const char *pwd) {
            sWarning = "WARNING: File ";
            sWarning.append(pwd);
            sWarning.append(" not found!");
        }
    };

    class WarnFindStopped : public Warning {
    public:

        WarnFindStopped(void) {
            sWarning = "WARNING: Node recusrion stopped.";
        }
    };

    class NodeReport {
    protected:
        StdString sStartDir; // pushed by default StartRun, popped by StopRun.

    public:
        virtual bool OnStart(void);
        virtual void OnStop(long unsigned nfiles);

        virtual void OnWarn(const Warning&);
        virtual bool OnNode(const Node& node);
    };


    const long unsigned FF_ERROR = (long unsigned) - 1L;

    class NodeFinder {
    public:
        NodeFinder(void);
        ~NodeFinder(void);

        long unsigned Run(NodeReport& fobj);
        long unsigned Run(Directory& sStartDir, NodeReport& fobj);
    };

}

#endif
