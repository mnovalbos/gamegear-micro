#include "zstdsearch.h"
#include <string.h>
typedef union castings_t
{
    long long int u64b;
    struct u32b{
        int data;
        int pad;
    }u32b;
    struct u16b{
        short int data;
        short int pad2;
        int   pad;
    }u16b;
    struct u8b{
        char data;
        char pad3;
        short int pad2;
        int pad;
    }u8b;

}castings_t;

zStdSearch::zStdSearch(char *dataIn, int size):fileSearch(dataIn,size)
{

    long long int mgc=ZSTD_MAGIC;
    fileTypeName="ZSTD";
    magicSize=4;
    magic=new char[magicSize];
    memcpy(magic,&mgc,magicSize);
}



void zStdSearch::extract(const char *fileNameDest)
{
    FILE* fout=fopen(fileNameDest,"w");
    long int offsetIni=offset;
    offset+=magicSize;
    unsigned char frameHeader=data[offset];
    offset++;
    unsigned char Frame_Content_Size_flag=frameHeader>>6;
    unsigned char Single_Segment_flag=(frameHeader&SINGLE_SEGMENT_FLAG)>>5;
    unsigned char Content_Checksum_flag=(frameHeader&CONTENT_CHECKSUM_FLAG)>>2;
    unsigned char Dictionary_ID_flag=(frameHeader&DICTIONARY_ID_FLAG);

   // printf("zSTD header: FCS %d SS %d CC %d DID %d \n",Frame_Content_Size_flag,Single_Segment_flag,Content_Checksum_flag,Dictionary_ID_flag);

    castings_t frameSize;
    frameSize.u64b=0;
    switch (Frame_Content_Size_flag) {
        case 0:
            if(Single_Segment_flag)
                memcpy(&frameSize.u8b.data,&(data[offset]),1);
                offset++;
            break;
        case 1:
             memcpy(&frameSize.u16b.data,&(data[offset]),2);
             offset+=2;
        break;
        case 2:
            memcpy(&frameSize.u32b.data,&(data[offset]),4);
            offset+=4;
        break;
        case 3:
            memcpy(&frameSize.u64b,&(data[offset]),8);
            offset+=8;
        break;

        default:
            printf("error frame content size\n");
        break;
    }

    //printf("Data length: 0x%llx\n",frameSize.u64b);

    bool lastBlock=0;
    while(!lastBlock){

        unsigned char blockHDR[3];
        memcpy(blockHDR,&(data[offset]),3);
        offset+=3;
        if(blockHDR[0]&0x01)
        {
           // printf("lastBlock\n");
            lastBlock=true;
        }
        unsigned int blocksize=(blockHDR[0]>>3)|(blockHDR[1]<<5)|(blockHDR[2]<<13);

        //printf("blockSize: %u\n",blocksize);

        offset+=blocksize;

    }
    if(Content_Checksum_flag)
        offset+=4;

    long int size=offset-offsetIni;
    //printf("offset init: %ld offset end: %ld length: %ld 0x%x\n",offsetIni,offset,size,size);
    printf("%ld\n",size);
    fwrite(&(data[offsetIni]),1,size,fout);

    fclose(fout);

}
