Open source release of the dirtyJOE - editor for the compiled java .class files.

The software was actively maintained between 2010 and 2014, thus C++ style is quite dated at the moment. Until 2017 I still had some hope to develop new features, but I was spending less and less time on that project. The code base includes attempt at moderninzing the disassmebler to be more object oriented to facilitate writing graph view feature and later deobfuscator and decompiler. These were at least some high level goals that I had in mind at that time.

The release contains:
- dirtyJOE C++ source code
- PyJOE C++ source code - Python (2.5, 2.6, 2.7) scripting integration (sorry 3.0 was still not a thing at the time)
- HTML help files that can be built into CHM help file
- Python/C++ translator of original Oracle JVM opcodes HTML help to the internal builtin dirtyJOE opcode help

The project can be build with the Visual Studio Community 2022. For the convenince I've added some .bat build scripts that can be run from the solution directory (you need to run vcvarsall.bat first).

Known issues:
- build process complains about "afxres.h" file - you need to install MFC package with your Visual Studio

Third party dependencies (included in the repository):
- aPLib (https://ibsensoftware.com/products_aPLib.html) - compression library used jvmspec.dat
- WTL (Windows Template Library) - Microsoft's UI alternative to MFC
- Python - headers and libs for Python (2.5, 2.6, 2.7) required to build PyJOE extensions

Project website: http://dirty-joe.com (I'll eventually move it to the github pages at some point).