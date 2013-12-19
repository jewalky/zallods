#ifndef F_FILE_H_INCLUDED
#define F_FILE_H_INCLUDED

#include "String.h"
#include <stdint.h>
#include <vector>
#include <fstream>

class FileArchive
{
public:
    struct Record
    {
        uint32_t Flags;
        String Name;

        uint32_t Length;
        uint32_t Offset;

        std::vector<Record> Children;
        String ResourceName;

        Record()
        {
            Flags = 2;
            Name = "";
            Length = 0;
            Offset = 0;
        }
    };

    bool open(String filename, String directory);
    const Record& getRecord(String filename);

private:
    String myFileName;
    String myDirectoryName;

    std::vector<Record> myRoot;
};

extern FileArchive Archives;

class File
{
public:
    File();
    bool open(String filename);
    void close();

    uint32_t read(void* destination, uint32_t size);
    void seek(uint32_t position);
    uint32_t getPosition();
    uint32_t length();

    bool isEof();
    String readLn();

private:
    bool myOpen;
    String myName;
    uint32_t myLength;
    uint32_t myPosition;
    uint32_t myOffset;

    std::ifstream myStream;
};

#endif // F_FILE_H_INCLUDED
