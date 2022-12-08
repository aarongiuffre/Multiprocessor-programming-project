#include "LFskiplist.cpp"
#include <thread>
#include <ctime> 
#include <chrono> 

constexpr int THREAD_COUNT = 4;
constexpr int ROUNDS = 10;
constexpr int ITERATIONS = 100;
std::atomic<int> id(0);
std::atomic<int> deletion_count(0);

int create_id()
{
    return id++;
};

void runInsert(LFskiplist<int> &l,MemManager &mm)
{
    for (auto j = 0; j != ITERATIONS; ++j)
    {
        l.insert(j,&mm);
        std::cout<<"suc insert"<<std::endl;
    }
};

void runRemove(LFskiplist<int> &l,MemManager &mm)
{
    for (auto j = 0; j != ITERATIONS * ITERATIONS; ++j)
    {
        if (l.remove(j, &mm))
        {
            deletion_count++;
            std::cout<<"suc delete"<<std::endl;
    
        }
    }
}


void run_threaded_test()
{
    for (auto x = 0; x != ROUNDS; ++x)
    {
        LFskiplist<int> l ;
        MemManager mm;
        mm.register_thread(2);
        std::thread totaladd[THREAD_COUNT];
        std::thread totalremove[THREAD_COUNT];

        //id.store(0);

        for (auto i = 0; i != THREAD_COUNT; ++i)
        {
            totaladd[i] = std::thread(runInsert, std::ref(l),std::ref(mm));
           
        }
        for (auto i = 0; i != THREAD_COUNT; ++i)
        {
         totalremove[i] = std::thread(runRemove, std::ref(l),std::ref(mm));
        }
        for (auto i = 0; i != THREAD_COUNT; ++i)
        {
            totaladd[i].join();
            totalremove[i].join();
        }


        int size = l.length;
        std::cout << "Size: " << size << std::endl;
        std::cout << "Insertions: " << THREAD_COUNT * ITERATIONS << std::endl;
        std::cout << "Deletions: " << deletion_count << std::endl;
        //assert(size == ((THREAD_COUNT * ITERATIONS) - deletion_count));
        
    }
}

int main(){
    /*
    auto start = std::chrono::high_resolution_clock::now();
    run_threaded_test();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << std::endl;
    */
    
    run_threaded_test();

    return 0;
}