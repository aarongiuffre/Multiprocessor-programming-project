#include <iostream> 
#include <stdlib.h>
#include <bits/stdc++.h>
#include <atomic>

struct Node {
    int key;
    std::atomic<FlagTagRef>* left;
    std::atomic<FlagTagRef>* right;
};

struct SeekRecord {
    struct Node* ancestor;
    struct Node* successor;
    struct Node* parent;
    struct Node* leaf;
};

struct SeekRecord* seekRecord = (SeekRecord*)malloc(sizeof(SeekRecord));

void seek(int key, SeekRecord* seekRecord);

bool search(int key, SeekRecord* seekRecord);

bool insert(int key);

bool deleteKey(int key);

bool cleanup(int key, SeekRecord* seekRecord);