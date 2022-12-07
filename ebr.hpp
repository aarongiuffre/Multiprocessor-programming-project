#include <vector>
#include <iostream>

struct MemManager;

struct ThreadContext {
    std::vector<void*> pending_reclaims;
    std::atomic<uint64_t> counter;
    ThreadContext *next;
    ThreadContext(MemManager *m);
};

struct MemManager {
    int wait_time = 1000000;
    static thread_local ThreadContext *self;
    std::atomic<ThreadContext*> head;
    void register_thread(int num);
    void unregister_thread();
    void op_begin();
    void sched_for_reclaim(void* ptr);
    bool try_reserve(void* ptr);
    void unreserve(void* ptr);
    void op_end();
    void wait_until_unreserved();
};

ThreadContext::ThreadContext(MemManager *m) {
  while (true) {
    next = m->head;
    if (m->head.compare_exchange_strong(next, this))
         break;
     }
 };

bool odd(uint64_t val)
{
    if (val%2==1){
        return true;
    }
    else{
        return false;
    }
};

void MemManager::register_thread(int num) { self = new ThreadContext(this); }
void MemManager::unregister_thread() {  }
void MemManager::op_begin() { self->counter++; }
void MemManager::sched_for_reclaim(void* ptr) { self->pending_reclaims.push_back(ptr); }
bool MemManager::try_reserve(void* ptr) { return false; }
void MemManager::unreserve(void* ptr) { }

void MemManager::op_end() {
 self->counter++;
 if (self->pending_reclaims.size() == 0)
    return;
 wait_until_unreserved();
 for (auto p : self->pending_reclaims) free(p);
};

void MemManager::wait_until_unreserved() {
ThreadContext* curr = head;
 while (curr) {
    uint64_t val = curr->counter;
    if (odd(val))
     do {
         wait(&wait_time);
         } while (curr->counter.load() == val);
    curr = curr->next;
    }
};
