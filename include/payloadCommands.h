#ifndef PAYLOAD_CMD_H_
#define PAYLOAD_CMD_H_

void sendImage(int argc, char **argv);
void getPayloadTelemetry(int argc,char **argv);
void checkPayloadTelemetry(int argc,char **argv);
void requestPayloadTelemetry(int argc,char **argv);
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

#endif