#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "pngsearch.h"
#include "txtargs.h"
#include "zstdsearch.h"

#include <string>
#define ZSTD 0xfd2fb528
#define PNG  0x474E5089
#define TITLE_ARGS 0x6c746974
#define SND_ARGS 0x2E646E73

void loadAssets(const char* name, char* &data, int &datalen)
{
    int fd=open(name,O_RDONLY);
    datalen=lseek(fd,0,SEEK_END);
    datalen=lseek(fd,0,SEEK_CUR);
    lseek(fd,0,SEEK_SET);
    data=new char[datalen];
    read(fd,data,datalen);
    close(fd);
}



int main(int argc,char** argv)
{
    char* filename=new char[1024];
    fileSearch* files=0x00;
    PngSearch* pngf;
    zStdSearch* zstdf;
    txtArgs* txtf;
    char* data=0x00;
    int dataLen=0;
    bool nextImg=true;

    int count=0;

    loadAssets("assets.bin",data,dataLen);

    pngf=new PngSearch(data,dataLen);
    zstdf=new zStdSearch(data,dataLen);
    txtf=new txtArgs(data,dataLen);
    files=zstdf;
    nextImg=true;
    while(nextImg){

        sprintf(filename,"_file%02d",count);
        printf("%d,",count);
        nextImg=files->search();
        //filename="_file"+std::to_string(count)+"."+files->getFileType();
        files->extract(filename);
        int nextType=files->getNextMagic();
       // printf("Next magic: 0x%x\n", nextType);

        switch(nextType)
        {
            case PNG:
                files=pngf;
            break;
            case ZSTD:
                files=zstdf;
            break;
            case TITLE_ARGS:
            case SND_ARGS:
                files=txtf;
            break;
            default:
                printf("Unknown header: 0x%x\n",nextType);
                nextImg=false;
            break;
        }

        //nextImg=files->search();
        count++;
    }
    delete[] data;
    return 0;

}
