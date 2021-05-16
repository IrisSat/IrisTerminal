#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char **argv){

        FILE* inFile = fopen(argv[1],"rb");

        if(inFile == NULL){
                printf("could not open file %s\n",argv[1]);
                return -1;
        }

        FILE* outFile = fopen(argv[2],"wb");

        if(outFile == NULL){
                printf("could not open file %s\n",argv[2]);
                return -1;
        }


        uint32_t startByte = 0;
        if(argc == 4){
                startByte = atoi(argv[3]);
        }

        printf("Swapping bytes in %s starting at byte %d. Output to %s\n",argv[1],startByte,argv[2]);

        fseek(inFile,0,SEEK_END);

        uint32_t image_size = ftell(inFile);

        fseek(inFile,0,SEEK_SET);
        //If we're not starting at the beginning copy the bytes to the out file.
        if(startByte >0){

                uint8_t* temp = (uint8_t*)malloc(startByte);
                fread(temp,1,startByte,inFile);
                fwrite(temp,1,startByte,outFile);
                                
        }


        for(uint32_t i=startByte;i<image_size-2;i++){
        
                uint8_t oldbyte=0;
                fread(&oldbyte,1,1,inFile);

                uint8_t newbyte = ((oldbyte>>1)&0x11) | ((oldbyte<<1)&0x22) | (oldbyte &0xCC);
               
                fwrite(&newbyte,1,1,outFile);
        }

        uint8_t endMarker[2];
        fread(&endMarker,1,2,inFile);
        fwrite(&endMarker,1,2,outFile);

        fclose(inFile);
        fclose(outFile);

}
