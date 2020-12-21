#ifndef PNGSEARCH_H
#define PNGSEARCH_H
#include "filesearch.h"
#define PNG_MAGIC 0x0A1A0A0D474E5089

//0x89504E470D0A1A0A

typedef struct  __attribute__((packed))
{
  long long int magic;
}pngHeader_t ;

typedef struct  __attribute__((packed)) {
    int Length;
    int Type;
    char* chunkData;
    int crc;
}chunkHeader_t;

typedef struct   __attribute__((packed)){
    int  Width;     //              4 bytes
    int  Height;    //              4 bytes
    char BitDepth;  //              1 byte
    char ColorType; //              1 byte
    char CompressionMethod; //      1 byte
    char FilterMethod;      //      1 byte
    char InterlaceMethod;   //      1 byte
}IHDRHeader_t;


class PngSearch: public fileSearch
{

public:
    PngSearch(char* dataIn, int size);
    void extract(const char *fileNameDest);
};

#endif // PNGSEARCH_H
