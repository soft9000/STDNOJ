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
// NodeFact unifies all of the Nodes and their factories into a single
// usefull point of reuse.
//
// 07/15/1998: Added NodeCollection and GenericNodeArray<node_t>, Randall Nagy.
// 11/13/2008: Converted to static usage, Randall Nagy
//
#ifndef _FileSystem_HPP
#define _FileSystem_HPP

#include <core/StdString.hpp>
#include <core/StdDateTime.hpp>
#include <core/PortTime.hpp>
#include <core/BigFileSize.hpp>
#include <core/Node.hpp>
#include <core/FilePath.hpp>
#include <core/File.hpp>
#include <core/Directory.hpp>

#include <core/NodeFinder.hpp>

namespace stdnoj {

    class FileSystem {
    public:
        static size_t Nelem(const StdString&);

        static size_t Query(const StdString&, Array<Node>& aResult);
        static size_t Query(const StdString&, Array<File>& aResult);
        static size_t Query(const StdString&, Array<Directory>& aResult);

        static bool Create(Node&);
        static bool Create(File&);
        static bool Create(Directory&);

        static bool Delete(Node&);
        static bool Delete(File&);
        static bool Delete(Directory&);

        static PortTime Query(const Node&); // Not implemented yet.
        static PortTime Query(const File&); // Not implemented yet.
        static PortTime Query(const Directory&); // Not implemented yet.

        static bool Touch(Node&, PortTime& pt); // Not implemented yet.
        static bool Touch(File&, PortTime& pt); // Not implemented yet.
        static bool Touch(Directory&, PortTime& pt); // Not implemented yet.
    };


} // stdnoj

#endif

