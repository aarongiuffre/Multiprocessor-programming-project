#include <thread>
#include <ctime> 
#include <chrono> 
#include "natarajanTree/natarajanTree.hpp"

int main(){
    //initializing base natarajan tree
    int inf2 = INT_MAX;
    int inf1 = INT_MAX - 1;
    int inf0 = INT_MAX - 2;
    Node inf2Leaf = {inf2, NULL, NULL};
    Node inf1Leaf = {inf1, NULL, NULL};
    Node inf0Leaf = {inf0, NULL, NULL};
    Node sentinalSuccessor = {inf1, &inf0Leaf, inf1Leaf};
    Node sentinalRoot = {inf2, sentinalSuccessor, inf2Leaf};
    //struct SeekRecord* seekRecord = (SeekRecord*)malloc(sizeof(SeekRecord)); //each thread should have one?

    return 0;
}