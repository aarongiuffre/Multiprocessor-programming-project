#include "LFskiplist.h"

template <typename T>
bool LFskiplist<T>::insert(T key, MemManager *mm) {
		SkipListNode<T> *node;
		SkipListNode<T> *update[max_level];
		mm->op_begin();
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
			mm->op_end();
			return true;
};

template <typename T>
bool LFskiplist<T>::remove(T key,MemManager *mm) {
			SkipListNode<T> *node;
			SkipListNode<T> *update[max_level];
			mm->op_begin();
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

			return false;
};

template <typename T>
bool LFskiplist<T>::find(double key) {
		SkipListNode<T> *node = _find(key);

			if (node == NULL || node->next_list[0] == NULL) {
				return false;
			}

			if (node->next_list[0]->value == key) {
				return true;
			}
			return false;
};

template <typename T>
SkipListNode<T>* LFskiplist<T>::_find(T key) {
			SkipListNode<T> *node = LFskiplist<T>::header;

			for (int i = LFskiplist<T>::skiplist_level - 1; i >= 0; i -- ) {
				while (node->next_list[i] && node->next_list[i]->value < key) {
					node = node->next_list[i];
				}
			}

			return node;
};

template <typename T>	
SkipListNode<T>* LFskiplist<T>::_find(T key, SkipListNode<T> **update) {
			SkipListNode<T> *node = LFskiplist<T>::header;

			for (int i = LFskiplist<T>::skiplist_level - 1; i >= 0; i -- ) {
				while (node->next_list[i] && node->next_list[i]->value < key) {
					node = node->next_list[i];
				}
				update[i] = node;
			}

			return node;
};
	
template <typename T>	
int LFskiplist<T>::random_level() {
			int num = 1;
			while (gen_random() % 128 < 64) {
				num ++;
			}
			return num < max_level ? num : max_level;
};

template <typename T>	
void LFskiplist<T>::_print() {
			for (int i = 0; i < skiplist_level; i ++ ) {
				SkipListNode<T> *tmp = header->next_list[i];
				while(tmp) {
					std::cout << tmp->value << " ";
					tmp = tmp->next_list[i];
				}
				printf("\n");
			}
	}
