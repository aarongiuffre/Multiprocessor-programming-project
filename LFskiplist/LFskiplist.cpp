#include "LFskiplist.hpp"


int main(){

    LFskiplist<int> skiplist;
    skiplist.insert(100);
    skiplist.insert(1000);
    bool x = skiplist.find(100);
    bool y = skiplist.find(50);
    std::cout<<x<<std::endl;
    std::cout<<y<<std::endl;
    return 0;
}