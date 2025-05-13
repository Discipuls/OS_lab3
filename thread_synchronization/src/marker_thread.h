#ifndef MARKER_THREAD_H
#define MARKER_THREAD_H

#include "array_manager.h"
#include "sync_primitives.h"
#include <atomic>
#include <memory>
#include <thread>

enum class MarkerCommand {
    Continue,
    Terminate
};

class MarkerThread {
public:
    MarkerThread(int id, std::shared_ptr<ArrayManager> arrayManager);
    ~MarkerThread();
    
    MarkerThread(const MarkerThread&) = delete;
    MarkerThread& operator=(const MarkerThread&) = delete;
    MarkerThread(MarkerThread&&) = delete;
    MarkerThread& operator=(MarkerThread&&) = delete;
    
    void start();
    void signalStart();
    void waitForBlocking();
    void sendCommand(MarkerCommand command);
    void join();
    bool isRunning() const;
    bool isBlocked() const;
    int getId() const;
    size_t getMarkedCount() const;
    size_t getBlockedIndex() const;

private:
    void threadFunction();
    void resetMarkedElements();
    
    int id;
    std::shared_ptr<ArrayManager> arrayManager;
    std::thread thread;
    std::atomic<bool> running;
    
    Event startEvent;
    Event blockedEvent;
    Event commandEvent;
    
    std::atomic<MarkerCommand> command;
    std::atomic<bool> blocked;
    std::atomic<size_t> markedCount;
    std::atomic<size_t> blockedIndex;
};

#endif