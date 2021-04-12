import serial
import argparse
import os.path
import math

CHUNK_SIZE = 2048   #The chunk size to split the image

parser = argparse.ArgumentParser(description='Send an image to Payload')
parser.add_argument('port', type=str,help='com port')
parser.add_argument('baud',  type=int, help='baud rate')
parser.add_argument('imgFile',  type=str, help='image file.')
args = parser.parse_args()

print("Connecting to CDH on {} at {} bps. Uploading image {}.".format(args.port,args.baud,args.imgFile))

f = open(args.imgFile,'rb')
fileSize = os.path.getsize(args.imgFile)
print("Image size in bytes: {}".format(fileSize))

numChunks = math.ceil(fileSize/CHUNK_SIZE)
print("Splitting the image into {} {} kiB chunks...".format(numChunks,CHUNK_SIZE/1024))

S = serial.Serial(args.port,args.baud,timeout=None)

if(not S.is_open):
    print("Could not open serial port...")
    exit()


for chunkNum in range(numChunks):

    chunkData = f.read(CHUNK_SIZE)



f.close()
S.close()