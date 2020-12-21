/*#define _GNU_SOURCE
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char** argv)
{
    FILE* f=fopen("holamundo","r");
    fseek(f,0,SEEK_END);
    size_t size=ftell(f);
    fseek(f,0,SEEK_SET);
    char* elfData=(char*)malloc(size);
    fread(elfData,1,size,f);
    fclose(f);
    int memfd=memfd_create("patchedApp",MFD_CLOEXEC);

    write(memfd,elfData,size );
    fsync(memfd);

    char** nargv=(char**)malloc(sizeof(char*));
    argv[0]=0x00;
    char** env=(char**)malloc(sizeof(char*));
    env [0]=0x00;
    int error=execve("./holamundo",nargv,env);
    printf("ERROR: %d\n",error);
    return 0;

}*/
#define _GNU_SOURCE
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                              } while (0)




void loadPatch(char* pfile,char* elfFile)
{
    FILE* f=fopen(pfile,"r");
    //patch_t patch;
    int pos=0;
    int val=0;
    while (!feof(f))
    {
        fscanf(f,"%d,%x",&pos,&val);
        printf("patching: pos %d 0x%x\n",pos,val);
        memcpy(&(elfFile[pos]),&val,4);
    }
    fclose(f);
}

 int main(int argc, char** argv)
  {

     char* cmd=(char*)malloc(1024);
     char *const parmList[] = {NULL};
     char *const envParms[2] = {NULL};



     FILE* f=fopen(argv[1],"r");
     fseek(f,0,SEEK_END);
     size_t size=ftell(f);
     fseek(f,0,SEEK_SET);
     char* elfData=(char*)malloc(size);
     fread(elfData,1,size,f);
     fclose(f);

     loadPatch(argv[2],elfData);

     FILE* memfd=fopen("patched_app","w");

     if (memfd ==0x00)
         errExit("memfd_create");


     fwrite(elfData,size,1,memfd );
     fclose(memfd);

     //sprintf(cmd,"/proc/%ld/fd/%d",(long) getpid(), memfd);

    // printf("%s\n",cmd);
     //execve(cmd, parmList, envParms);
     //printf("Return not expected. Must be an execve error.n");
     return 0;
  }
