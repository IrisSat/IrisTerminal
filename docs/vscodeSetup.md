# Setting Up VSCode

The project includes some files for easy use with VS code.
Following these instructions, you should be able to use autocomplete, and debug the program in VS code.

### Check the search paths

In \.vscode\c_cpp_properties.json you should see a field called "includePath". Update this to fix any issues with autocomplete not working.

It's currently setup where the source code for the libraries are in folders two levels up from the project folder:

libcsp/
libserialProt/
/Utilities/IrisTerminal/

This only really matters if you need to debug the library functions.

### Setup debugging

In \.vscode\launch.json are the settings for debugging.
Change the COM port in the "args" field to match whatever it is on your computer.
Change "cwd", and "miDebuggerPath" to be the paths containing the mingw binaries, and the gdb debugger. 

You might need to install gdb in msys2, or mess with those paths a bit, but it is possible to debug through VS code.

