#include "f_file.h"
#include "utils.h"

FileArchive Archives;

bool FileArchive::open(String filename, String directory)
{
    std::ifstream str;
    str.open(filename.c_str(), std::ios::in | std::ios::binary);
    if(!str.is_open())
        return false;

    str.seekg(0);

    uint32_t magic;
    str.read((char*)&magic, 4);
    if(magic != 0x31415926)
        return false;

    uint32_t fat_root, fat_offset, fat_size;
    str.read((char*)&fat_root, 4);
    str.seekg(16);
    str.read((char*)&fat_offset, 4);
    str.read((char*)&fat_size, 4);

    str.seekg(fat_offset);
    if(str.tellg() != fat_offset)
        return false;

    uint32_t root_size = fat_size-fat_root;

    Record rec;
    rec.ResourceName = filename;
    rec.Name = directory;
    rec.Flags = 1;
    rec.Length = root_size;
    rec.Offset = 0;

    std::vector<uint32_t> stack_sizes;
    std::vector<uint32_t> stack_positions;
    std::vector<Record*> stack_records;

    stack_sizes.push_back(fat_root+root_size);
    stack_positions.push_back(fat_root);
    stack_records.push_back(&rec);

    // replaced monstrous recursion with this
    while(stack_sizes.size())
    {
        bool hreturn = false;

        for(uint32_t i = stack_positions.back(); i < stack_sizes.back(); i++)
        {
            str.seekg(fat_offset+i*32);

            uint32_t r_junk, r_offset, r_size, r_type;
            char r_name[17];
            r_name[16] = 0;

            str.read((char*)&r_junk, 4);
            str.read((char*)&r_offset, 4);
            str.read((char*)&r_size, 4);
            str.read((char*)&r_type, 4);
            str.read(r_name, 16);

            Record r;
            r.Offset = r_offset;
            r.Length = r_size;
            r.Flags = r_type;
            r.Name = r_name;
            r.ResourceName = filename;
            stack_records.back()->Children.push_back(r);

            if(r.Flags & 1) // directory
            {
                stack_positions.back() = i+1;
                stack_sizes.push_back(r.Offset+r.Length);
                stack_positions.push_back(r.Offset);
                stack_records.push_back(&stack_records.back()->Children.back());
                hreturn = true;
                break;
            }
        }

        if(hreturn) continue;
        stack_sizes.pop_back();
        stack_positions.pop_back();
        stack_records.pop_back();
    }

    myRoot.push_back(rec);
    return true;
}

const FileArchive::Record& FileArchive::getRecord(String filename)
{
    static Record rec;
    filename = TruncateSlashes(FixSlashes(filename));

    std::vector<String> path = filename.split("/");
    std::vector<Record>* where_search = &myRoot;
    for(uint32_t i = 0; i < path.size(); i++)
    {
        bool found = true;
        for(uint32_t j = 0; j < where_search->size(); j++)
        {
            Record& record = where_search->at(j);
            if(record.Name.toLowerCase() == path[i])
            {
                if(i < path.size()-1)
                {
                    where_search = &record.Children;
                    found = true;
                    break;
                }
                else return record;
            }
        }

        if(!found) break;
    }

    return rec;
}

File::File()
{
    myOpen = false;
    myName = "";
    myOffset = 0;
    myLength = 0;
    myPosition = 0;
}

bool File::open(String filename)
{
    filename = TruncateSlashes(FixSlashes(filename));
    if(filename.find("patch/") != 0 && open("patch/"+filename))
        return true;

    myStream.clear();
    myStream.open(filename.c_str(), std::ios::in | std::ios::binary);

    if(myStream.is_open())
    {
        myName = filename;
        myOffset = 0;
        myStream.seekg(0, std::ios::end);
        myLength = myStream.tellg();
        myStream.seekg(0);
        myPosition = 0;
        myOpen = true;
        return true;
    }
    else
    {
        myStream.clear();

        const FileArchive::Record& record = Archives.getRecord(filename);
        if(record.Flags & 2) return false;
        if(record.Flags & 1) return false;
        myStream.open(record.ResourceName.c_str(), std::ios::in | std::ios::binary);
        if(!myStream.is_open()) return false;

        myName = filename;
        myOffset = record.Offset;
        myLength = record.Length;
        myPosition = 0;
        myStream.seekg(myOffset);
        myOpen = true;
        return true;
    }
}

void File::close()
{
    if(myStream.is_open()) myStream.close();
    myOpen = false;
}

uint32_t File::read(void* where, uint32_t size)
{
    if(!myOpen) return 0;
    if(size > myLength-myPosition)
        size = myLength-myPosition;
    if(!size) return 0;

    myStream.seekg(myOffset+myPosition);
    myStream.read((char*)where, size);
    uint32_t cnt = myStream.gcount();
    myPosition += cnt;
    return cnt;
}

void File::seek(uint32_t position)
{
    if(!myOpen) return;
    if(position > myLength)
        position = myLength;
    myPosition = position;
    myStream.seekg(myOffset+myPosition);
}

uint32_t File::getPosition()
{
    if(!myOpen) return 0;
    return myPosition;
}

uint32_t File::length()
{
    if(!myOpen) return 0;
    return myLength;
}

bool File::isEof()
{
    if(!myOpen) return true;
    return (myPosition >= myLength);
}

String File::readLn()
{
    String str;
    while(!isEof())
    {
        char ch;
        read(&ch, 1);
        if(ch == '\n')
            break;
        else if(ch != '\r') str += ch;
    }

    return str;
}
