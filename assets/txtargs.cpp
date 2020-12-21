#include "txtargs.h"
#include <string.h>
txtArgs::txtArgs(char *dataIn, int size):fileSearch(dataIn,size)
{

    long long int mgc=TXT_MAGIC;
    fileTypeName="TXT";
    magicSize=4;
    magic=new char[magicSize];
    memcpy(magic,&mgc,magicSize);
}

bool txtArgs::search()
{
    //printf("searching magic 0x%llX %ld\n",magic,sizeof (long long int));
    bool salir=false;
    int mgc2=TXT_MAGIC2;
    char* magic2=new char[magicSize];
    memcpy(magic2,&mgc2,magicSize);

    for(long int i=offset;(i<(dataSize-8))&&(!salir);i++)
    {

        if((memcmp(magic,&(data[i]),magicSize)==0)||(memcmp(magic2,&(data[i]),magicSize)==0))
        {
            salir=true;
            //printf("Found %s at %ld HEX: 0x%lx\n",fileTypeName.c_str(), i, i);
            printf("0x%lx,",i);
            offset=i;
        }
    }

    return salir;
}

void txtArgs::extract(const char *fileNameDest)
{
    FILE* fout=fopen(fileNameDest,"w");
    long int offsetIni=offset;
    offset+=magicSize;

    while((data[offset]!=0x0a)&&(data[offset-1]!=0x0d))
    {
        offset++;
    }
    offset++;
    long int size=offset-offsetIni;
   // printf("offset init: %ld offset end: %ld length: %ld 0x%x\n",offsetIni,offset,size,size);
    printf("%ld\n",size);
    fwrite(&(data[offsetIni]),1,size,fout);
    fclose(fout);
}
