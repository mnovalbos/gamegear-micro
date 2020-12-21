#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <map>

typedef struct patch_t{
  int file;
  int pos;
  int val;
}patch_t;

std::map<int, patch_t> vpatch;

int nfiles=37;
int fileSizes[37*3]={
    0,0x00,2097,
1,0x831,34123,
2,0x8d7c,5593,
3,0xa355,123840,
4,0x28715,27645,
5,0x2f312,93,
6,0x2f36f,66931,
7,0x3f8e2,386488,
8,0x9de9a,267906,
9,0xdf51c,54,
11,0xdf552,158311,
12,0x105fb9,67,
14,0x105ffc,67,
16,0x10603f,204,
17,0x10610b,10841,
18,0x108b64,1262,
19,0x109052,11732,
20,0x10be26,2156,
21,0x10c692,9990,
22,0x10ed98,18220,
23,0x1134c4,756,
24,0x1137b8,5518,
25,0x114d46,27882,
26,0x11ba30,2348,
27,0x11c35c,31,
28,0x11c37b,107,
29,0x11c3e6,204,
30,0x11c4b2,676,
31,0x11c756,234,
32,0x11c840,50547,
33,0x128db3,39583,
34,0x132852,30614,
35,0x139fe8,441,
36,0x13a1a1,49871,
37,0x146470,58536,
38,0x154918,45780,
39,0x15fbec,346};
        /*{
1,221108,0x0000854b,
4,221120,0x00006bfd,
2,221124,0x000015d9,
3,221128,0x0001e3c0,
17,215112,0x00002a59,
18,194476,0x000004ee,
19,188468,0x00002dd4,
20,215120,0x0000086c,
21,221152,0x00002706,
24,221136,0x0000158e,
26,221144,0x0000092c,
28,440360,0x0000006b,
34,27848,0x00007796,
33,27856,0x00009a9f,
37,27860,0x0000e4a8,
32,27876,0x0000c573,
38,27880,0x0000b2d4,
36,27884,0x0000c2cf,
35,167048,0x000001b9,
39,167056,0x0000015a,
0,215132,0x00000831,
5,446444,0x0000005d,
6,175056,0x00010573,
7,175044,0x0005e5b8,
8,175068,0x00041682,
11,175080,0x00026a67,
22,194208,0x0000472c
};
*/
/*={
    0,2097,
    1,34123,
    2,5593,
    3,123840,
    4,27645,
    5,93,
    6,66931,
    7,386488,
    8,267906,
    9,28,
    10,26,
    11,158311,
    12,28,
    13,39,
    14,28,
    15,39,
    16,204,
    17,10841,
    18,1262,
    19,11732,
    20,2156,
    21,9990,
    22,18220,
    23,756,
    24,5518,
    25,27882,
    26,2348,
    27,31,
    28,107,
    29,204,
    30,676,
    31,234,
    32,50547,
    33,39583,
    34,30614,
    35,441,
    36,49871,
    37,58536,
    38,45780,
    39,346
};*/

bool search(char* data,int dataSize, int val)
{
    int offset=0;
    bool salir=false;
    for(long int i=offset;(i<(dataSize-4))&&(!salir);i++)
    {

        if(memcmp(&val,&(data[i]),4)==0)
        {
            //salir=true;
            printf("Found at %ld HEX: 0x%08x\n", i, val);
            offset=i;
        }
    }

    return salir;
}

void load(char* pfile)
{
    FILE* f=fopen(pfile,"r");
    patch_t patch;
    int fileNum=0;
    while (!feof(f))
    {
        fscanf(f,"%d,%d,%x",&fileNum,&patch.pos,&patch.val);
        printf("patch: %d 0x%x\n",patch.pos,patch.val);
        vpatch[fileNum]=patch;
    }
    fclose(f);
}
int main(int argc,char** argv)
{
    // load("patch.txt");
    char* file=0x00;
    FILE* f=fopen(argv[1],"r");
    size_t size=0;
    fseek(f,0,SEEK_END);
    size=ftell(f);
    fseek(f,0,SEEK_SET);
    file=(char*)malloc(size);

    fread(file,1,size,f);
    fclose(f);


    for(int i=0;i<nfiles;i++)
    {
        printf("FILE: %d ",fileSizes[i*3]);
        printf("\n addr \n");
        search(file,size,fileSizes[i*3+1]);
        printf("\n size \n");
        search(file,size,fileSizes[i*3+2]);

        printf("\n");
    }

return 0;
}
