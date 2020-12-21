#ifndef FILESEARCH_H
#define FILESEARCH_H
#include <string>

class fileSearch
{
protected:
    std::string fileTypeName;
    int magicSize=0;
    char* magic;
    long int offset=0;
    int dataSize=0;
    char* data=0x00;
    char* dataIdx;

public:
     fileSearch(char* dataIn, int size);
     virtual bool search();
     int getNextMagic();
     std::string getFileType(){return fileTypeName;};
     virtual void extract(const char *fileNameDest)=0;

};

#endif // FILESEARCH_H
