# Common Serialization Library

This project presenting serialization protocol which is named **Common Serialization Protocol (CSP)**
and its implementation in C++.

CSP provide fast serialization process, with low memory consumption.
It supports versioning of structures, work in different compatibility modes (flexibility),
and versioning of protocol itself with backward compatibility (extensibility).
Also CSP provide client-server data exchange mechanism.

Definition of the CSP can be found in file ***CSP.txt*** in the repository root directory.
The main parts of the protocol are already defined there, but some paragraphs not finished yet.

### A few words about implementation.

To achieve maximum speed most interfaces using CRTP pattern and other tools of templates 
instead of dynamic polymorphic classes. The only part where dynamic polymorphism is used is CSP Messaging, 
because, first of all, it would be very inconvenient to deal with static polymorphism there,
and, secondly, in case of CSP Messaging process it does not have any significant impact on execution speed.

It was assumed that this library will be used in different environments including
those where no C++ standard library is present, such as Windows or Linux kernels.
So, one of the milestones was to add fully functional classes of allocators, 
containers, synchronization primitives etc. to operate with.
Currently only allocators, some containers and std meta functions equivalents are defined.
It is so due to the fact that my interest is primarily focused on the implementation of CSP.
When it would be finished (including documentation) and when I will add scripts to generate 
some header files that I have to manually enter for now, I'll take care of remaining classes.

At this time all main modules of CSP are ready. For most of its parts unit-tests are defined
(but more must be done there). And I polish individual components.

It is being built with MSVC version 19.41.34123.
At the moment, work with other compilers has not yet been fully tested and is not guaranteed.

Under development.

## License

Copyright (c) 2023-2024 Andrey Grabov-Smetankin <ukbpyh@gmail.com>

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
