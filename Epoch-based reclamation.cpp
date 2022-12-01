#include "Epoch-based relcamation.hpp"
void MemManager::register_thread(int num) { self = new ThreadContext(this); }
void MemManager::unregister_thread() { /* no-op */ }
void MemManager::op_begin() { self->counter++; }
void MemManager::sched_for_reclaim(void* ptr) { self->pending_reclaims.push_back(ptr); }
bool MemManager::try_reserve(void* ptr) { return false; }
void MemManager::unreserve(void* ptr) { }

void MemManager::op_end() {
 self->counter++;
 if (self->pending_reclaims.size() == 0)
    return;
 wait_until_unreserved();
 for (auto p : self->pending_reclaims){free(p);}
}
void MemManager::wait_until_unreserved() {
ThreadContext* curr = head;
 while (curr) {
    uint64_t val = curr->counter;
    if (odd(val))
     do {
         wait(&wait_time);
         } while (curr->counter == val);
    curr = curr->next;
    }
}

bool odd(uint64_t val)
{
    if (val%2==1){
        return true;
    }
    else{
        return false;
    }
}