

#include "utilities.h"
#include "settingsUtilities.h"

char * settingsFilePathName = "settings.txt";
char * comPort;
int baudRate;
char * logFilePathName;
FILE * logFile;
char * inputFilePathName;
FILE * inputFile;

int InitializeSettings(void)
{
    printf("Initializing settings...\n");
    // Oepn settings file
    FILE *settingsFile;
    settingsFile = fopen(settingsFilePathName, "r");
    if(settingsFile == NULL){
        printf("Could not settings file");
        return 0;
    }
    // Read settings
    char buf[255], temp[255];
    int equalsPosition, lineLength;
    // COM Port
    fscanf(settingsFile, "%s", buf);
    printf("Input line: %s\n",buf);
    //equalsPosition = strchr(buf,'=');
    equalsPosition = getCharLocation(buf,'=');
    printf("Equals position: %d\n",equalsPosition);
    lineLength = strlen(buf);
    printf("Input line length: %d\n",lineLength);
    //memcpy(comPort,&buf[equalsPosition],lineLength-equalsPosition);
    substring(buf,comPort,equalsPosition+1,lineLength);
    printf("Com Port: %s\n",comPort);
    // Baud Rate
    fscanf(settingsFile, "%s", buf);
    equalsPosition = getCharLocation(buf,'=');
    lineLength = strlen(buf);
    memcpy(buf,&buf[equalsPosition],lineLength-equalsPosition);
    memcpy(temp,&buf[equalsPosition],lineLength-equalsPosition);
    // Log file
    sprintf(buf,"");
    fscanf(settingsFile, "%s", buf);
    equalsPosition = getCharLocation(buf,'=');
    lineLength = strlen(buf);
    memcpy(logFilePathName,&buf[equalsPosition],lineLength-equalsPosition);
    if(strcmp(logFilePathName,"") == 0)
    {
        logFile = fopen(logFilePathName, "w");
        if(logFile == NULL)
            printf("Could not open log file: %s\n",logFilePathName);
    }
    // Input
    sprintf(buf,"");
    fscanf(settingsFile, "%s", buf);
    equalsPosition = getCharLocation(buf,'=');
    lineLength = strlen(buf);
    memcpy(inputFilePathName,&buf[equalsPosition],lineLength-equalsPosition);
    if(strcmp(inputFilePathName,"") == 0)
    {
        inputFile = fopen(inputFilePathName, "r");
        if(inputFile == NULL)
            printf("Could not open input file: %s\n",inputFilePathName);
    }
    // Print settings
    printf("Com Port: %s\n",comPort);
    printf("Baud Rate: %s, %s\n",buf,temp);
    printf("Log File: %s\n",logFilePathName);
    printf("Input file: %s\n",inputFilePathName);

    // Cleanup
    fclose(settingsFile);
    fclose(logFile);
    fclose(inputFile);

    return 1;
}
