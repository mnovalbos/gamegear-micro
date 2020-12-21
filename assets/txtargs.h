#ifndef TXTARGS_H
#define TXTARGS_H
#include "filesearch.h"
#define TXT_MAGIC 0x6c746974
#define TXT_MAGIC2 0x2E646E73

class txtArgs: public fileSearch
{
public:
    txtArgs(char* dataIn, int size);
    bool search();
    void extract(const char *fileNameDest);
};

#endif // TXTARGS_H
