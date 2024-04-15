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
//
// The stdnet namespace contains modernized versions of models designed to
// operate on the Internet. Here you will find Sockets, Protocols, and patterns
// designed for portable operation across unix, linux, windows (WIN32), DOS, 
// and Mac OSX.
//
// 09/15/2006: Namespace created, R. Nagy
// 10/08/2006: Added a more comprehensive URL definition, R. Nagy
//
#ifndef stdnet_hpp
#define stdnet_hpp


#include <extra/stdlog.hpp>
using namespace stdnoj;

namespace stdnet {
    // Seconds for timeout -
    const long SOC_TIMEOUT = 15L;
}

#include <stdnet/URL.hpp>
#include <stdnet/CommBase.hpp>
#include <stdnet/Socket.hpp>
#include <stdnet/SocketSelect.hpp>
#include <stdnet/SocketMonitor.hpp>
#include <stdnet/Http.hpp>



#endif
