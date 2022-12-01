#include <vector>
#include <iostream>
#include <thread>

struct ThreadContext {
    std::vector<void*> pending_reclaims;
    std::atomic<uint64_t> counter;
    ThreadContext *next;

  ThreadContext(MemManager *m) {
  while (true) {
    next = m->head;
    if (m->head.compare_exchange_strong(next, this))
         break;
     }
 }
};

struct MemManager {
    int wait_time = 100;
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