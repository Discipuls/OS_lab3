#include "sync_primitives.h"

Event::Event() : signaled(false) {}

void Event::signal() {
    {
        std::lock_guard<std::mutex> lock(eventMutex);
        signaled = true;
    }
    eventCV.notify_all();
}

void Event::reset() {
    std::lock_guard<std::mutex> lock(eventMutex);
    signaled = false;
}

void Event::wait() {
    std::unique_lock<std::mutex> lock(eventMutex);
    eventCV.wait(lock, [this] { return signaled; });
}

bool Event::waitFor(std::chrono::milliseconds timeout) {
    std::unique_lock<std::mutex> lock(eventMutex);
    return eventCV.wait_for(lock, timeout, [this] { return signaled; });
}

bool Event::isSignaled() const {
    std::lock_guard<std::mutex> lock(eventMutex);
    return signaled;
}

CountdownEvent::CountdownEvent(int initialCount) : remainingCount(initialCount) {
    if (initialCount < 0) {
        throw std::invalid_argument("Initial count cannot be negative");
    }
}

void CountdownEvent::addCount(int count) {
    {
        std::lock_guard<std::mutex> lock(countMutex);
        if (remainingCount == 0) {
            throw std::logic_error("Cannot add to a countdown event that has already been signaled");
        }
        remainingCount += count;
    }
}

void CountdownEvent::signal() {
    bool shouldSignal = false;
    {
        std::lock_guard<std::mutex> lock(countMutex);
        if (remainingCount > 0) {
            --remainingCount;
        }
        shouldSignal = (remainingCount == 0);
    }
    
    if (shouldSignal) {
        countCV.notify_all();
    }
}

void CountdownEvent::wait() {
    std::unique_lock<std::mutex> lock(countMutex);
    countCV.wait(lock, [this] { return remainingCount == 0; });
}

bool CountdownEvent::waitFor(std::chrono::milliseconds timeout) {
    std::unique_lock<std::mutex> lock(countMutex);
    return countCV.wait_for(lock, timeout, [this] { return remainingCount == 0; });
}

bool CountdownEvent::isSet() const {
    std::lock_guard<std::mutex> lock(countMutex);
    return remainingCount == 0;
}

void CountdownEvent::reset(int count) {
    std::lock_guard<std::mutex> lock(countMutex);
    remainingCount = count;
}

ThreadBarrier::ThreadBarrier(int count) 
    : threadCount(count), awaitingThreads(0), generation(0) {
    if (count <= 0) {
        throw std::invalid_argument("Thread count must be positive");
    }
}

bool ThreadBarrier::await() {
    std::unique_lock<std::mutex> lock(barrierMutex);
    int currentGeneration = generation;
    
    if (++awaitingThreads == threadCount) {
        awaitingThreads = 0;
        ++generation;
        barrierCV.notify_all();
        return true;
    } else {
        barrierCV.wait(lock, [this, currentGeneration] { 
            return currentGeneration != generation; 
        });
        return false;
    }
}

void ThreadBarrier::reset(int count) {
    std::lock_guard<std::mutex> lock(barrierMutex);
    if (awaitingThreads != 0) {
        throw std::logic_error("Cannot reset barrier while threads are waiting");
    }
    threadCount = count;
}