
//Structure for the commands.
typedef struct{
    char* name; //The name the user must type to use the command.
    void (*func)(int,char**);  //A pointer to the command funciton that will be run. Takes the standard argc,argv parameters.
    char * helpString; //The initial help text shown. Put more detailed help in the command function itself.
}cmd_t;

//Commands table. Add new commands here, make sure to update the NUM_COMMANDS.
#define NUM_COMMANDS    41
cmd_t commandTable[NUM_COMMANDS]= {
    {"help", printHelp, "Prints the help message."},
    {"send_image",sendImage,"Sends an image to the payload subsystem"},
    {"ping",ping,"Pings the chosen subsystem, sending a packet of data and timing the response"},
    {"listProcess",listProcess,"Lists the tasks and their status of the chosen subsystem"},
    {"uptime",uptime,"Prints how long a subsystem has been turned on for."},
    {"memfree",memFree,"Prints the free memory of a subsystem."},
    {"buffFree",buffFree,"Prints the number of csp buffers available on a subsystem."},
    {"getPldTelem",getPayloadTelemetry,"Gets the latest payload telemetry data from CDH."},
    {"checkPldTelem",checkPayloadTelemetry,"Checks what payload telemetry is collected on CDH."},
    {"requestPldTelem",requestPayloadTelemetry,"Tells CDH to request new telemtry from payload."},
    {"getCdhTelem",getCdhTelemetry,"Gets the latest CDH telemetry data from CDH."},
    {"checkCdhTelem",checkCdhTelemetry,"Checks what CDH telemetry is collected on CDH."},
    {"requestCdhTelem",requestCdhTelemetry,"Tells CDH to generate new CDH telemetry."},
    {"setCdhTime",setCdhTime,"Updates the time on CDH."},
    {"getCdhTime",getCdhTime,"Gets the time on CDH."},
    {"downloadImage",downloadImage,"Gets an image from payload."},
    {"deleteImage",deleteImage,"Deletes a specified payload image."},
    {"scheduleTTT",scheduleTTT,"Schedules a new time tagged task."},
    {"cancelTTT",cancelTTT,"Cancels a time tagged task."},
    {"quit",quitTerminal,"Exit the terminal."},
    {"turnOnCamera1",turnoncamera1,"Turn on camera 1."},
    {"turnOffCamera1",turnoffcamera1,"Turn off camera 1."},
    {"turnOnCamera2",turnoncamera2,"Turn on camera 2."},
    {"turnOffCamera2",turnoffcamera2,"Turn off camera 2."},
    {"resetCamera1",resetcamera1,"Reset camera 1."},
    {"resetCamera2",resetcamera2,"Reset camera 2."},
    {"listofImages",listofimages,"List of images."},
    {"icc1",takeimagecamera1,"Take image with camera 1."},
    {"icc2",takeimagecamera2,"Take image with camera 2."},
    {"pldFileList",pldFileList,"Gets a list of the files stored in Payload flash."},
    {"pldMountFS",pldMountFS,"Mount the payload filesystem."},
    {"pldUnmountFS",pldUnmountFS,"Unmount the payload filesystem."},
    {"powReadTemp",powReadTempChannel,"Power - Read a temperature channel."},
    {"powReadSC",powReadSolarCurrent,"Power - Read a solar current value."},
    {"powReadLC",powReadLoadCurrent,"Power - Read a load current value."},
    {"powReadMsbVoltage",powReadMsbVoltage,"Power - Read MSB voltage value."},
    {"powSetLoadSwitch",powSetLoadSwitch,"Power - Set load switch on/off."},
    {"powSetSolar",powSetSolar,"Power - Set solar array switch on/off."},
    {"powSetMode",powSetMode,"Power - Set power mode."},
    {"sendCommsMessage",commsSendCSPMessage,"Send COMMS a message."},
    {"runCommandFile",runCommandFile,"Run a list of commands in a file (argument 1), and save the output in another file (argument 2)."},
};

void runCommandFile(int argc, char ** argv);
void printHelp(int argc, char ** argv);
void quitTerminal(int argc, char ** argv);
int makeargs(char *args, int *argc, char ***aa);



