#include <iostream> 
#include <stdlib.h>
#include <bits/stdc++.h>

template <typename T>
struct Node {
    T key;
    atomic<FlagTagRef<T>> left;
    atomic<FlagTagRef<T>> right;
};

struct SeekRecord {
    struct Node* ancestor;
    struct Node* successor;
    struct Node* parent;
    struct Node* leaf;
};

template <typename T>
void seek(T key, SeekRecord* seekRecord);

template <typename T>
bool search(T key, SeekRecord* seekRecord);

template <typename T>
bool insert(T key);

template <typename T>
bool deleteKey(T key);

template <typename T>
bool cleanup(T key, SeekRecord* seekRecord);

void initTree(void);