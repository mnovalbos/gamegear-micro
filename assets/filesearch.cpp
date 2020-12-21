#include "filesearch.h"
#include <string.h>


fileSearch::fileSearch(char *dataIn, int size)
{
    printf("loading size: %d\n", size);
    offset=0;
    data=dataIn;
    dataIdx=dataIn;
    dataSize=size;
}


bool fileSearch::search()
{
    //printf("searching magic 0x%llX %ld\n",magic,sizeof (long long int));
    bool salir=false;
    for(long int i=offset;(i<(dataSize-8))&&(!salir);i++)
    {

        if(memcmp(magic,&(data[i]),magicSize)==0)
        {
            salir=true;
            //printf("Found %s at %ld HEX: 0x%lx\n",fileTypeName.c_str(), i, i);
            printf("0x%lx,",i);
            offset=i;
        }
    }

    return salir;
}

int fileSearch::getNextMagic()
{
    int magic=0;
    if(offset<dataSize)
        memcpy(&magic,&(data[offset]),4);
    return magic;
}
