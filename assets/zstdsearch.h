#ifndef ZSTDSEARCH_H
#define ZSTDSEARCH_H
#include "filesearch.h"
//little endian

#define ZSTD_MAGIC 0xFD2FB528


#define SINGLE_SEGMENT_FLAG 0b00100000
#define CONTENT_CHECKSUM_FLAG 0b00000100
#define DICTIONARY_ID_FLAG 0b00000011

class zStdSearch:public fileSearch
{
public:
    zStdSearch(char* dataIn, int size);

    void extract(const char *fileNameDest);
};

#endif // ZSTDSEARCH_H
