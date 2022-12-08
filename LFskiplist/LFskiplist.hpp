#include <iostream>
#include <random>
#include <cstdlib>
#include "Epoch_based_reclamation.hpp"

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
class LFskiplist {

	public:
		std::random_device gen_random;
		SkipListNode<T> *header;
		int skiplist_level;
		int length;

		LFskiplist(): skiplist_level(1) {
			init();
		}
		~LFskiplist() {

		}

		void print() {
			_print();
		}

		void init() {
			header = new SkipListNode<T>();
			header->level = 1;
			for(auto &p: header->next_list) {
				p = NULL;
			}
		}

		bool insert(T key, MemManager *mm) {
			SkipListNode<T> *node;
			SkipListNode<T> *update[max_level];

			node = _find(key, update);
			int level = random_level();
			SkipListNode<T> *new_node = new SkipListNode<T>(key, level);
			if (level > skiplist_level) {
				for (int i = skiplist_level; i < level; i ++ ) {
					update[i] = header;
				}
			}
			for (int i = 0; i < level; i ++ ) {
				new_node->next_list[i] = update[i]->next_list[i];
				update[i]->next_list[i] = new_node;
			}
			length ++;
			return true;
		}

		bool remove(T key,MemManager *mm) {
			mm->op_begin();
			SkipListNode<T> *node;
			SkipListNode<T> *update[max_level];
			node = _find(key, update);
			if (node == NULL || node->next_list[0] == NULL) {
				mm->op_end();
				return false;
			}
			if (node->next_list[0]->value == key) {
				
				SkipListNode<T> *update[max_level];
				for (int i = 0; i < node->level; i ++ ) {
					update[i] = node->next_list[i];
				}
				
				while (skiplist_level > 1 && 
					header->next_list[skiplist_level - 1]) {
					skiplist_level --;
				}
				mm->sched_for_reclaim(node);
				length --;
				mm->op_end();
				return true;
			}
			mm->op_end();
			return false;
		}

		bool find(double key) {
			SkipListNode<T> *node = _find(key);

			if (node == NULL || node->next_list[0] == NULL) {
				return false;
			}

			if (node->next_list[0]->value == key) {
				return true;
			}
			return false;
		}		

	private:

		SkipListNode<T> *_find(T key) {
			SkipListNode<T> *node = header;

			for (int i = skiplist_level - 1; i >= 0; i -- ) {
				while (node->next_list[i] && node->next_list[i]->value < key) {
					node = node->next_list[i];
				}
			}

			return node;
		}
		
		SkipListNode<T> *_find(T key, SkipListNode<T> **update) {
			SkipListNode<T> *node = header;

			for (int i = skiplist_level - 1; i >= 0; i -- ) {
				while (node->next_list[i] && node->next_list[i]->value < key) {
					node = node->next_list[i];
				}
				update[i] = node;
			}

			return node;
		}
	
		int random_level() {
			int num = 1;
			while (gen_random() % 128 < 64) {
				num ++;
			}
			return num < max_level ? num : max_level;
		}

		void _print() {
			for (int i = 0; i < skiplist_level; i ++ ) {
				SkipListNode<T> *tmp = header->next_list[i];
				while(tmp) {
					std::cout << tmp->value << " ";
					tmp = tmp->next_list[i];
				}
				printf("\n");
			}
		}

	};
