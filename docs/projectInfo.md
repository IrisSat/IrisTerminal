# Iris Terminal Project Info

This is an application built in C.

The application currently only runs on Windows (10), but with some modification could run on linux.

## Project Structure

Currently the application is structured as follows:

- main.c -> This holds the _command table_, and parses user input, calling the _command function_ associated with a command.
- csp_client.c -> This contains the CSP startup code and command fucntions for CSP functions(ping,uptime,ps etc.).
- payloadCommands.c -> This holds the command functions related to payload.
- cdhCommands.c -> This holds the command functions related to CDH.
- networkConfig.h -> This contains the CSP address of each subsystem.

The program depends on 2 libraries:

### CSP:

This provides networking capability. Serveral features like ping, uptime etc are provided, as well routing and addressing.

https://github.com/IrisSat/libcsp


### libserialport:
This libray provides serial port communication for windows (and other platforms). This is needed because the CSP windows uart driver didn't work very well.

https://sigrok.org/wiki/Libserialport


## Extending the Application

In general the process to add more commands:
1. Add a command to the command table in main.c
2. Implement the command function, using CSP functions to send and receive data from the satellite.
3. See existing functions for an idea of how to parse inputs and print detailed help etc.
4. Edit the makefile if you add any extra source files.
5. Update the docs if needed.

To add support for other subsystems:
- Add the address to the networkConfig.h file.
- Update the routing table on CDH.
- Add commands as needed.