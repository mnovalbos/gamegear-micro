#include "pngsearch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIG_TO_LITTLE(num)  ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000)

PngSearch::PngSearch(char *dataIn, int size):fileSearch(dataIn,size)
{
  long long int mgc=PNG_MAGIC;
  fileTypeName="PNG";
  magicSize=8;
  magic=new char[magicSize];
  memcpy(magic,&mgc,magicSize);

}


void PngSearch::extract(const char *fileNameDest)
{

    FILE* fout=fopen(fileNameDest,"w");
    long int offsetIni=offset;
    offset+=sizeof(pngHeader_t);//magic;
    chunkHeader_t chH;
    chH.Type=0;
    while(chH.Type!=0x444E4549)//IEND
    {
        memcpy(&chH,&(data[offset]),8);//length and type
        chH.Length=BIG_TO_LITTLE(chH.Length);
        //printf("offset: %ld chunk type: 0x%x length: %d\n",offset,chH.Type,chH.Length);
        offset+=4+4+chH.Length+4;//length+type+data+crc
    }
    long int size=offset-offsetIni;
    //printf("offset init: %ld offset end: %ld length: %ld 0x%x\n",offsetIni,offset,size,size);
    printf("%ld\n",size);
    fwrite(&(data[offsetIni]),1,size,fout);
    fclose(fout);
}
