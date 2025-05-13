#ifndef SYNC_PRIMITIVES_H
#define SYNC_PRIMITIVES_H

#include <condition_variable>
#include <mutex>
#include <vector>
#include <atomic>

class Event {
public:
    Event();
    
    void signal();
    void reset();
    void wait();
    bool waitFor(std::chrono::milliseconds timeout);
    bool isSignaled() const;

private:
    std::mutex eventMutex;
    std::condition_variable eventCV;
    bool signaled;
};

class CountdownEvent {
public:
    explicit CountdownEvent(int initialCount);
    
    void addCount(int count = 1);
    void signal();
    void wait();
    bool waitFor(std::chrono::milliseconds timeout);
    bool isSet() const;
    void reset(int count);

private:
    mutable std::mutex countMutex;
    std::condition_variable countCV;
    int remainingCount;
};

class ThreadBarrier {
public:
    explicit ThreadBarrier(int count);
    
    bool await();
    void reset(int count);

private:
    mutable std::mutex barrierMutex;
    std::condition_variable barrierCV;
    int threadCount;
    int awaitingThreads;
    int generation;
};

#endif