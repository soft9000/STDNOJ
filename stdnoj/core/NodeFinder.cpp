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
#include <stdnoj.hpp>

namespace stdnoj {

    bool NodeReport::OnStart(void) {
        return true;
    }
    // "nfiles" is the number of times you said true to a File() report;

    void NodeReport::OnStop(long unsigned nfiles) {
    }

    void NodeReport::OnWarn(const Warning& item) {
        cout << item.sWarning.c_str() << endl;
    }

    bool NodeReport::OnNode(const class Node& FindFileData) {
        // cout << endl << "[" << pwDir.c_str() << "]" << endl;
        cout << FindFileData.GetFullName('/') << endl;
        return true;
    }



    // false from a Starting will abort same;

    NodeFinder::NodeFinder(void) {
    }

    NodeFinder::~NodeFinder(void) {
    }

    long unsigned NodeFinder::Run(Directory& tDir, NodeReport& fobj) {
        static int levels = 0L;

        if (!levels)
            fobj.OnStart();

        Directory dirHold;
        dirHold.Get();
        dirHold.Push();

        levels++;

        long unsigned lResult = 0L;
        Array<Node> nodes;

        if (FileSystem::Query(tDir.Name(), nodes) != 0L) {
            for (size_t ss = 0L; ss < nodes.Nelem(); ss++) {
                if (fobj.OnNode(nodes[ss]) == false)
                    break;
                if (nodes[ss].IsDir() == true) {
                    Directory zDir;
                    StdString str = nodes[ss].GetFullName(PathChar());
                    if (zDir.Name(str) == false)
                        break;
                    lResult += Run(zDir, fobj);
                }
            }
        }

        levels--;

        dirHold.Pop();

        if (!levels)
            fobj.OnStop(lResult);

        return lResult;
    }

    long unsigned NodeFinder::Run(NodeReport& fobj) {
        Directory dir;
        return Run(dir, fobj);
    }




} // stdnoj
