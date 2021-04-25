# How to Build Iris Terminal

In order to build the Iris Terminal, you need to install MSYS2.
This is needed to provide the compiler and tools for buildign the project.

You can get MSYS2 here: https://www.msys2.org/

Follow all the instructions to install mingw.
To check that you have the required tools, try the following:
1. Open MSYS2 MinGW 64-bit
2. Type "gcc". You should see an error about no input files.
3. Type "make". You should see an error about no targets or makefile found.

Now navigate to the project directory.

Type "make"

The project should build and you should have a new exacutable.


## Building the libraries

In most cases you don't need to build the libraries. The static library files are included in the github repository, so you should only need to rebuild if you have modified the libraries or want to compile them with different options.
It's important when building the libraries to use the same build environment (MSYS2 MinGW 64-bit) for everything, otherwise you'l get linker errors!

### LibCSP
To build the CSP library:
1. Use the MSYS2 MinGW 64-bit terminal and navigate to the CSP project.
2. Make sure you've got python installed in msys.
3. "python waf2 configure --enable-if-kiss --enable-crc32 --with-os=windows"
4. If that runs without errors, then "python waf2 build"
5. Copy the .a file to this project.

This will configure with the options currently needed. Also you can double check the wscript file, to change the gcc flags, if you want to enabled/disable optimization or debug symbols etc.

### Libserialport

To build the serialport library:
1. Use the MSYS2 MinGW 64-bit terminal and navigate to the serialport project.
2. Run "./autogen.sh"
3. Run "./configure"
4. Run "make"
5. Copy the .a file (in .libs) to this project.


