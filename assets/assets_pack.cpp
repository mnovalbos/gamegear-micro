#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <cstring>

typedef struct patch_t{
  int pos;
  int val;
}patch_t;

std::map<int, patch_t> indexes;
std::map<int, patch_t> sizes;

typedef struct args_t
{
    char* overlayPack;
    char* patchFile;
    char* assetsOut;
    char* patchOut;
}args_t;

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                            } while (0)


#define NUMFILES 40

void loadOrigPatch(char* pfile)
{
    FILE* f=fopen(pfile,"r");
    patch_t index;
    patch_t size;
    int fileNum=0;
    while (!feof(f))
    {
        fscanf(f,"%d addr %d %x size %d %x",&fileNum,&index.pos,&index.val,&size.pos,&size.val);
        printf("original patch:file %d %d 0x%x %d 0x%x\n",fileNum, index.pos,index.val,size.pos,size.val);
        indexes[fileNum]=index;
        sizes[fileNum]=size;
    }
    fclose(f);
}


void printUsage()
{
    printf("/**\n"
      "USAGE:\n"
      "  assetspack -patchIn <offsetFile.txt> -patchOut <newoffsets.txt> -assetsOut <assets.bin> [-overlayPack <path_to_files>]\n");

}

args_t* readArgs(int argc,char** argv)
{
    if(argc<=3)
    {
        printf("Error: Incorrect number of arguments \n");
        printUsage();
        return NULL;
    }
    args_t* arg=(args_t*)malloc(sizeof(args_t));
    //default args
    arg->overlayPack=0x00;
    arg->patchFile=0x00;
    arg->assetsOut=0x00;
    arg->patchOut=0x00;
    //memcpy(arg->outFileName,"out.bin\0",8);

    int i=1;
    while(i<argc)
    {
        if(strcmp(argv[i],"-patchIn")==0)
        {
            i++;
             arg->patchFile=argv[i];
        }else
        if(strcmp(argv[i],"-patchOut")==0)
        {
            i++;
            arg->patchOut=argv[i];
        }else
        if(strcmp(argv[i],"-assetsOut")==0)
        {
            i++;
            arg->assetsOut=argv[i];
        }else
        if(strcmp(argv[i],"-overlayPack")==0)
        {
            i++;
            arg->overlayPack=argv[i];
        }
        i++;
    }

    return arg;
}

void updateFiles(int file,int newDispl)
{
    for(int i=file;i<NUMFILES;i++)
    {
        if(indexes.find(i)!=indexes.end())//there is info
        {
            if(indexes[i].pos!=-1)//valid info
            {
                indexes[i].val+=newDispl;
            }
        }
    }
}


void readFile(int i,char** fileData,int *fileSize,char* pack)
{
    char fileIn[256];
    FILE* f=0x00;
    if(pack!=0x00)//check pack first
    {
        sprintf(fileIn,"%s/_file%02d",pack,i);
        f=fopen(fileIn,"r");
    }
    if(f==0x00)
    {
        sprintf(fileIn,"_file%02d",i);
        f=fopen(fileIn,"r");
    }

    if(f==0x00)
    {
        printf("ERROR: Missing file %d\n",i);
    }
    fseek(f,0,SEEK_END);
    *fileSize=ftell(f);
    fseek(f,0,SEEK_SET);
    *fileData=(char*)malloc(*fileSize);

    fread(*fileData,1,*fileSize,f);
    fclose(f);

}
int main(int argc, char** argv)
{

    args_t* arg=readArgs(argc,argv);

    if(arg==NULL)
    {
        exit(0);
    }
    FILE* assets=fopen(arg->assetsOut,"w");
    FILE* patch=fopen(arg->patchOut,"w");


    char* fileData;
    int fileSize;

    loadOrigPatch(arg->patchFile);

    int displAccum=0;
    for (int i=0;i<NUMFILES;i++)
    {

      //  printf("i %d\n",i);
        readFile(i,&fileData,&fileSize,arg->overlayPack);
        if(sizes.find(i)!=sizes.end())
        {
            if((sizes[i].pos!=-1)&&(sizes[i].val!=fileSize))
            {
                displAccum=fileSize-sizes[i].val;
                fprintf(patch,"%d,0x%x\n",sizes[i].pos,fileSize);
                updateFiles(i+1,displAccum);
            }else
            {
               // printf("ERROR: trying to patch an undocumented file\n");
               // exit(0);
            }
        }
        fwrite(fileData,1,fileSize,assets);
    }

    for(int i=0;i<NUMFILES;i++)
    {
        if(indexes.find(i)!=indexes.end())//there is info
        {
            if(indexes[i].pos!=-1)//valid info
            {
                //indexes[i].val+=newDispl;
                fprintf(patch,"%d,0x%x\n",indexes[i].pos,indexes[i].val);
            }
        }
    }
    //patches for rom filesizes, every rom is 512kb
    fprintf(patch,"173748,0xe3a08702\n");
    fprintf(patch,"174156,0xe3a08702\n");
    //fprintf(patch,"2894,0x6c6c616e\n");
    fclose(patch);
    fclose(assets);
    return 0;
}
