#ifndef PAYLOAD_CMD_H_
#define PAYLOAD_CMD_H_

void pldTestCameraInit(int argc, char **argv);
void pldCameraRegListWrite(int argc, char **argv);
void pldCameraSetI2cWriteAddress(int argc, char **argv);
void pldCameraSetI2cReadAddress(int argc, char **argv);
void pldCameraI2cTransmit(int argc, char **argv);
void pldCameraHandshake(int argc, char **argv);
void pldCameraSensorInit(int argc, char **argv);
void pldBootCount(int argc, char **argv);
void pldSendImage(int argc, char **argv);
void pldGetTelemetry(int argc,char **argv);
void pldCheckTelemetry(int argc,char **argv);
void pldRequestTelemetry(int argc,char **argv);
void downloadImage(int argc, char **argv);
void turnoncamera1(int argc, char **argv);
void turnoffcamera1(int argc, char **argv);
void turnoncamera2(int argc, char **argv);
void turnoffcamera2(int argc, char **argv);
void resetcamera1(int argc, char **argv);
void resetcamera2(int argc, char **argv);
void listofimages(int argc, char **argv);
void takeimagecamera1(int argc, char **argv);
void takeimagecamera2(int argc, char **argv);
void pldFileList(int argc, char **argv);
void deleteImage(int argc, char **argv);
void pldMountFS(int argc, char **argv);
void pldUnmountFS(int argc, char **argv);
void pldRestartFS(int argc, char **argv);

#endif