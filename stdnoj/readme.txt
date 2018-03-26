-------------------------------------------------------------------------
***** OVERVIEW *****
-------------------------------------------------------------------------
This is the main include point for the stdnoj namespace. 

Projects in this directory have been built using various 
compilers and tool sets.  The list includes:

   Microsoft Visual Studio.NET
   Microsoft Visual C++ 6
   Borland C++Builder 6
   Borland Kylix
   Eclipse 3.0 / G++ (CDT)
   Eclipse 3.0 / DJGPP (CDT + a few hacks of my own)
   NetBeans 6.* & 7.*
   cygwin G++

If you have one of these tool sets, then you should be able
to easily re-create any of these projects to use the code.
Let me know if you have any trouble.

Because platform files are referenced by many of the
files in the noj folder (e.g. <unix/unix_FileInformation.cpp>),
the default folder should be included in your path. 

You will need to #define a manifest constant for your platform 
(UNIX, MSDOS, WIN32), as well as to add the "using namespace stdnoj"
whenever including <stdnoj.hpp>

Enjoy!


R.A. Nagy
Soft9000.com
r.a.nagy@gmail.com


-------------------------------------------------------------------------
***** LOCATIONS *****
-------------------------------------------------------------------------
Here is a briefing on what is in this folder:

o stdnoj (this folder) is where the "Neat Odd Job" library 
lives. Needs to be on your main include path.

o noj is where the official "core files" reside,
as included by <stdnoj.hpp>

o dos, win32, unix are where the platform files
reside. (Also included by other files.)

o server, socket, stdnet are where some handy 
"super-namespace" frameworks live. Managed seperately, 
yet their models are dependant upon stdnoj.

o extra is for things that are likely to be 
re-modeled / re-factored as time goes on.

o projects is where we share our neat odd jobs. (I have
hundreds. They will surface here as time allows.)
Feel free to submit yours!

o _doc and _support contain more informaiton on building
and maintaining the project on your machine.


-------------------------------------------------------------------------
***** NOTES *****
-------------------------------------------------------------------------
01/07/2007 09:00:40 - This project is fine as-is, but an updated namespace
is in the works. The new namespace includes better email (mime attachments, 
etc), cleaned-up inheritance, HTML/XML classes, delimited database support, 
improved INI support, more noj projects, and a whole lot more.

NOTE: GCC users will find some handy shell scripts to build 
the library under the .\_support directory. Libraries are
kept (as expected) under .\lib

Note that this zip contains the core stdnoj library only. 
The examples and demonstration tools described in the 
introduction are maintained spereately.


-------------------------------------------------------------------------
***** GUILT AVOIDANCE :-) *****
-------------------------------------------------------------------------
We all have bills to pay. We all have the same number 
of hours in the day. Your donations will allow us to 
divert time away from our family, friends, and full-time 
jobs to add exciting new features to this project.

Of course, this means that the future of the Neat Odd 
Job Project is totally in your hands... So be it a 
single dollar or fifty, consider contributing today 
- Become a reason for your success, as well as the 
success of the Open Source Movement. 

After all, if you spent countless volunteer hours 
working on a free project, wouldn't donations be 
one of the things that you would like to see? It 
is the capitalistic way of saying "thank you"!
... It is a ''Golden Rule'' thing ...

May the source be with you... Always.

--R.A Nagy
