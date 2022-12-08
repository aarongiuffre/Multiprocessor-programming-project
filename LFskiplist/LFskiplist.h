#include <iostream>
#include <random>
#include <cstdlib>
#include "ebr.hpp"

const static int max_level = 16;

template <typename T>
struct SkipListNode {
		T value;
		int level;
		SkipListNode *next_list[max_level];
		SkipListNode(): level(1) { }
		SkipListNode(T value, int level):value(value), level(level) { } 
	}; 


template <typename T>

class LFskiplist
{
    public:
		std::random_device gen_random;
		SkipListNode<T> *header;
		int skiplist_level;
		int length;

		LFskiplist(): skiplist_level(1) {init();}

		~LFskiplist() {}

		void print() {_print();}

		void init() {
			header = new SkipListNode<T>();
			header->level = 1;
			for(auto &p: header->next_list) {
				p = NULL;
			}
		}

        bool insert(T key, MemManager *mm);
        bool remove(T key,MemManager *mm);
        bool find(double key);

    private:    

        SkipListNode<T> *_find(T key);
        SkipListNode<T> *_find(T key, SkipListNode<T> **update);
        int random_level();
        void _print();
};
