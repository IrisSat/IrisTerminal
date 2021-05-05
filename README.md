# Iris Terminal

This program is a command line utility for communicating with the Iris subsystems over a serial connection.
The program can be used to download images from payload, or to schedule time tagged tasks, or to ping subsytems etc.

The program uses the Cubesat Space Protocol to communicate with the subsystems. 

Below are the minimal steps to setup and use the program.
See the docs folder for more details on the project, such as extended usage instructions, how to build the project, add new commands or subssytems etc.


## Hardware Setup

A serial connection must be used to communicate between the computer running the Iris Terminal and the CDH subsystem.

If the CDH dev kit (Smartfusion2 Maker Kit) is being used, simply plug the USB into your computer.
Check the device manager, under "Ports (COM & LPT)", for the COM port you are connected to. It should show ups as "FlashPro5 Port". 

If the CDH EM board is being used, then a USB-Serial converter is needed. Connect the RX and TX pins of the converter , to the TX and RX pins of the CDH EM board. Connect the ground pin of the converter to ground on the CDH board. Again check the device manager to find the COM port.

The CDH board should be connected to the other satellite subsystems over CAN bus. Follow the recommended wiring from the wiki...

## Software Setup 

This program has only been tested on Windows 10.

Each satellite subsystem will require a certain version of it's software in order to be fully compatible with all the ocmmands available. At the minimum, a subsystem must have CAN bus communications and CSP working, to communicate with  CDH.
With CSP support, a subsystem will automatically have some basic commands implemented.

The following subsystems currently have extended functionallity enabled, with the softweare versions listed below:

- CDH -> https://github.com/IrisSat/IrisSat-Flight-Software/tree/operations
- Payload -> https://github.com/IrisSat/IrisSat-Payload-Software ??? 

This list should be updated as progress is made.

## How to Use Iris Terminal

Download the executable from the releases page. Alternatively you can build the applciation yourself, but this requires more work.

Open powershell in the folder with the executable. 
(Hint: You can Shift-Right click in the folder, then "Open Powershell here").

Run the terminal with "./IrisTerminal.exe COM21 115200".
Change COM21 to COM(X) for whatever COM port your connected on. 
115200 is the current baudrate, and must be the same as on CDH.

You should see some basic info printed out.

![The Iris Terminal](/docs/resources/terminalScreenshot.PNG "The Iris Terminal Screenshot")

Now you can run different commands and functions.
Type "help" to see a list of the commands.

![The Iris Terminal Help Output](/docs/resources/terminalHelp.PNG "The Iris Terminal Help Screenshot")

You can type help "command" to get more detailed help, or checkout the docs page for more info.

To exit you can type "quit" or control-c, if the terminal freezes up for some reason.
