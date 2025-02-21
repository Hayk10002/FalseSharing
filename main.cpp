#include <iostream>
#include <thread>
#include <format>
#include <chrono>

// In this project we use volatile to prevent compiler to optimize some variables, so that we can see the effect of false sharing.

// x and y are on the same cache line
// This is bad because when one thread writes to x, the cache line is invalidated and the other thread can't read y until the cache line is reloaded
// Aligning the struct to 64 bytes to ensure that x and y are on the same cache line
struct alignas(64) SharedBetweenThreadsBad
{
    volatile int x{0};
    volatile int y{1};
};

// x and y are on different cache lines
// This is good because when one thread writes to x, the cache line is invalidated but the other thread can still read y
// Aligning y to 64 bytes to ensure that it will be at the start of the next cache line, and x will be on the previous cache line
struct SharedBetweenThreadsGood
{
    volatile int x{0};
    alignas(64) volatile int y{1};
};

template<class SharedBetweenThreads>
void WriteX(SharedBetweenThreads &shared)
{
    for(int i = 0; i < 100000000; i++) shared.x++;
}

template<class SharedBetweenThreads>
int ReadY(SharedBetweenThreads &shared)
{
    volatile int sum{};
    for(int i = 0; i < 100000000; i++) sum += shared.y;
    return sum;
}

// Times the function f by running it N times and returns the time in milliseconds
template<class F>
int time_ms(F f, int N)
{
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < N; i++) f();
    return (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
}


int main() {
    SharedBetweenThreadsBad shared_bad;
    SharedBetweenThreadsGood shared_good;

    int time_threaded_bad = time_ms([&shared_bad](){
        std::thread t1(WriteX<SharedBetweenThreadsBad>, std::ref(shared_bad));
        std::thread t2(ReadY<SharedBetweenThreadsBad>, std::ref(shared_bad));
        t1.join();
        t2.join();
    }, 10);

    int time_threaded_good = time_ms([&shared_good](){
        std::thread t1(WriteX<SharedBetweenThreadsGood>, std::ref(shared_good));
        std::thread t2(ReadY<SharedBetweenThreadsGood>, std::ref(shared_good));
        t1.join();
        t2.join();
    }, 10);

    int time_non_threaded = time_ms([&shared_bad](){
        WriteX(shared_bad);
        ReadY(shared_bad);
    }, 10);

    std::cout << std::format("Time when false sharing:     {}ms\nTime when not false sharing: {}ms\nTime non-threaded:           {}ms", time_threaded_bad, time_threaded_good, time_non_threaded) << std::endl;
   
    return 0;
}
