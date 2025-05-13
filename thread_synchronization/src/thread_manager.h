#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include "array_manager.h"
#include "marker_thread.h"
#include "sync_primitives.h"
#include <memory>
#include <vector>

class ThreadManager {
public:
    explicit ThreadManager(std::shared_ptr<ArrayManager> arrayManager);
    ~ThreadManager();
    
    ThreadManager(const ThreadManager&) = delete;
    ThreadManager& operator=(const ThreadManager&) = delete;
    ThreadManager(ThreadManager&&) = delete;
    ThreadManager& operator=(ThreadManager&&) = delete;
    
    void createThreads(int count);
    void startAllThreads();
    void waitForAllThreadsBlocked();
    void terminateThread(int id);
    void continueOtherThreads();
    bool areAllThreadsFinished() const;
    size_t getActiveThreadCount() const;
    std::vector<int> getActiveThreadIds() const;
    std::shared_ptr<MarkerThread> findThreadById(int id);

private:
    std::shared_ptr<ArrayManager> arrayManager;
    std::vector<std::shared_ptr<MarkerThread>> threads;
    CountdownEvent threadsBlockedEvent;
    size_t activeThreadCount;
};

#endif