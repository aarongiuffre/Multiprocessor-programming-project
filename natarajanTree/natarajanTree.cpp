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
        void setTag(void)
        {
            reg = ((uintptr_t)ptr & ~flagMask & ~tagMask) | (flag ? 2 : 0) | (1);
        }
};

class NatarajanTree {
    private:
        Node sentinalRoot;
    public:
        NatarajanTree()
        {
            int inf2 = INT_MAX;
            int inf1 = INT_MAX - 1;
            int inf0 = INT_MAX - 2;
            Node inf2Leaf = {inf2, NULL, NULL};
            Node inf1Leaf = {inf1, NULL, NULL};
            Node inf0Leaf = {inf0, NULL, NULL};
            Node sentinalSuccessor = {inf1, inf0Leaf, inf1Leaf};
            sentinalRoot = {inf2, sentinalSuccessor, inf2Leaf};
        }
        Node getRoot(void)
        {
            return sentinalRoot;
        }
}

struct SeekRecord* seekRecord = (SeekRecord*)malloc(sizeof(SeekRecord)); //do in main?
natTree = NatarajanTree();

void seek(int key, SeekRecord* seekRecord)
{
    seekRecord->ancestor = natTree.getRoot();
    seekRecord->successor = seekRecord->ancestor->left;
    seekRecord->parent = seekRecord->successor;
    seekRecord->leaf = seekRecord->parent->left.getPtr();
    Node parentField = seekRecord->parent->left;
    Node currentField = seekRecord->leaf->left;
    Node* currentField.getPtr();

    while(current != NULL)
    {
        if(!parentField.getTag())
        {
            seekRecord->ancestor = seekRecord->parent;
            seekRecord->successor = seekRecord->leaf;
        }
        seekRecord->parent = seekRecord->leaf;
        seekRecord->leaf = current;
        parentField = currentField;
        if(key < current->key)
        {
            currentField = current->left;
        }
        else
        {
            currentField = current->right;
        }
        current = currentField.getPtr();
    }
    return;
}

bool search(int key, SeekRecord* seekRecord)
{
    seek(key, seekRecord);
    if(seekRecord->leaf->key = key)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool insert(int key)
{
    while(1)
    {
        seek(key, seekRecord);
        if(seekRecord->leaf->key != key)
        {
            Node parent = seekRecord->parent;
            Node leaf = seekRecord->leaf;
            Node newLeaf = {key, NULL, NULL};
            if(key < parent->key)
            {
                Node* childAddr = parent->left.getPtr();
                Node newInternal = {key, newLeaf, childAddr}
            }
            else
            {
                Node* childAddr = parent->right.getPtr();
                Node newInternal = {childAddr->key, childAddr, newleaf};
            }
            bool result = atomic::compare_exchange_weak(childAddr, newLeaf, newInternal);
            if(result)
            {
                return true;
            }
            else
            {
                bool flag = childAddr.getFlag();
                bool tag = childAddr.getTag();
                Node* address = childAddr.getPtr();
                if(address == &leaf && (flag || tag))
                {
                    cleanup(key, seekRecord);
                }
            }
        }
        else
        {
            return false;
        }
    }
}

bool deleteKey(int key)
{
    bool mode = false; //0 for injection, 1 for cleanup
    while(1)
    {
        seek(key, seekRecord);
        Node parent = seekRecord->parent;
        if(key < parent->key)
        {
            Node* childAddr = parent->left.getPtr();
        }
        else
        {
            Node* childAddr = parent->right.getPtr();
        }
        Node leaf = seekRecord->leaf;
        if(mode == false)
        {
            if(leaf->key != key)
            {
                return false;
            }
            bool result = compare_exchange_weak(childAddr, FlagTagRef(leaf, false, false), FlagTagRef(leaf, true, false));
            if(result)
            {
                mode = true;
                bool done = cleanup(key, seekRecord);
                if(done)
                {
                    return true;
                }
            }
            else
            {
                bool flag = childAddr.getFlag();
                bool tag = childAddr.getTag();
                Node* address = childAddr.getPtr();
                if(address = leaf && (flag || tag))
                {
                    cleanup(key, seekRecord);
                }
            }
        }
        else
        {
            if(seekRecord->leaf != leaf)
            {
                return true;
            }
            else
            {
                bool done = cleanup(key, seekRecord);
                if(done)
                {
                    return true;
                }
            }
        }
    }
}

bool cleanup(int key, SeekRecord* seekRecord)
{
    Node ancestor = seekRecord->ancestor;
    Node successor = seekRecord->successor;
    Node parent = seekRecord->parent;
    Node leaf = seekRecord->leaf;
    if(key < ancestor->key)
    {
        Node* successorAddr = ancestor->left.getPtr();
    }
    else
    {
        Node* successorAddr = ancestor->right.getPtr();
    }
    if(key < parent->key)
    {
        Node* childAddr = parent->left.getPtr();
        Node* siblingAddr = parent->right.getPtr();
    }
    else
    {
        Node* siblingAddr = parent->left.getPtr();
        Node* childAddr = parent->right.getPtr();
    }
    bool flag = childAddr.getFlag();
    if(!flag)
    {
        siblingAddr = childAddr;
    }
    siblingAddr.setTag();
    bool flag = siblingAddr.getFlag();
    Node* address = siblingAddr.getPtr();
    bool result = compare_exchange_weak(successorAddr, FlagTagRef(successor, false, false), FlagTagRef(address, flag, false));
}