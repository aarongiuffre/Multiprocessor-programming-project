#include "natarajanTree.hpp"

class FlagTagRef {
    private:
        uintptr_t reg;
        static const uintptr_t flagMask = 2;
        static const uintptr_t tagMask = 1;
    public:
        FlagTagRef(int* ptr = nullptr, bool flag = false, bool tag = false)
        {
            reg = ((uintptr_t)ptr & ~flagMask & ~tagMask) | (flag ? 2 : 0) | (tag ? 1 : 0);
        }
        int* getPtr(void)
        {
            return (int*)(reg & ~flagMask & ~tagMask);
        }
        bool getFlag(void)
        {
            return (reg & flagMask);
        }
        bool getTag(void)
        {
            return (reg & tagMask);
        }
};

//struct SeekRecord* seekRecord = (SeekRecord*)malloc(sizeof(SeekRecord)); //do in main?
//create tree with sentinal nodes in main ig

void seek(int key, SeekRecord* seekRecord)
{
    seekRecord->ancestor
}

bool search(int key, SeekRecord* seekRecord)
{
    seek(key, seekRecord);
    if(seekRecord->leaf->key = key)
        return true;
    else
        return false;
}

bool insert(int key)
{
    
}

bool deleteKey(int key)
{

}

bool cleanup(int key, SeekRecord* seekRecord)
{

}