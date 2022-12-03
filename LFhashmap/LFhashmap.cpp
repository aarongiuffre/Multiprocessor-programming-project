#include "LFhashmap.hpp"
int main()
{
    int capacity = 64;
    LockFreeHashSet test(capacity);
    int x = test.getbucketSize();
    std::cout<<x<<std::endl;;
    test.add(3);
    capacity = capacity >> 1;
    std::cout<<capacity;
    return 0;
}