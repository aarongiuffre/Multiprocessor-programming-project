#include "natarajanTree.hpp"

template <typename T>
class FlagTagRef {
    private:
        uintptr_t reg;
        static const uintptr_t flagMask = 2;
        static const uintptr_t tagMask = 1;
    public:
        FlagTagRef(T* ptr = nullptr, bool flag = false, bool tag = false)
        {
            reg = ((uintptr_t)ptr & ~flagMask & ~tagMask) | (flag ? 2 : 0) | (tag ? 1 : 0);
        }
        T* getPtr(void)
        {
            return (T*)(reg & ~flagMask & ~tagMask);
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

template <typename T>
void seek(T key, SeekRecord* seekRecord)
{

}

template <typename T>
bool search(T key, SeekRecord* seekRecord)
{
    seek(key, seekRecord);
    if(seekRecord->leaf->key = key)
        return true;
    else
        return false;
}

template <typename T>
bool insert(T key)
{
    
}

template <typename T>
bool deleteKey(T key)
{

}

template <typename T>
bool cleanup(T key, SeekRecord* seekRecord)
{

}

void initTree(void)
{
    int inf2 = INT_MAX;
    int inf1 = INT_MAX - 1;
    int inf0 = INT_MAX - 2;

    struct Node* inf2Leaf = {inf2, NULL, NULL};
    struct Node* sentinalSuccessor = {inf1, NULL, NULL};
    sentinalRoot = ;
}