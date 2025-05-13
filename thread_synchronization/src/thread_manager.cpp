#include "marker_thread.h"
#include <chrono>
#include <iostream>
#include <stdexcept>

using namespace std::chrono_literals;

MarkerThread::MarkerThread(int id, std::shared_ptr<ArrayManager> arrayManager)
    : id(id), 
      arrayManager(arrayManager),
      running(false),
      command(MarkerCommand::Continue),
      blocked(false),
      markedCount(0),
      blockedIndex(0) {
    
    if (!arrayManager) {
        throw std::invalid_argument("Array manager cannot be null");
    }
}

MarkerThread::~MarkerThread() {
    if (running.load()) {
        try {
            sendCommand(MarkerCommand::Terminate);
            join();
        } catch (const std::exception& e) {
            std::cerr << "Error during marker thread shutdown: " << e.what() << std::endl;
        }
    }
}

void MarkerThread::start() {
    if (running.load()) {
        throw std::logic_error("Thread already running");
    }
    
    running.store(true);
    thread = std::thread(&MarkerThread::threadFunction, this);
}

void MarkerThread::signalStart() {
    startEvent.signal();
}

void MarkerThread::waitForBlocking() {
    blockedEvent.wait();
}

void MarkerThread::sendCommand(MarkerCommand cmd) {
    command.store(cmd);
    commandEvent.signal();
}

void MarkerThread::join() {
    if (thread.joinable()) {
        thread.join();
    }
}

bool MarkerThread::isRunning() const {
    return running.load();
}

bool MarkerThread::isBlocked() const {
    return blocked.load();
}

int MarkerThread::getId() const {
    return id;
}

size_t MarkerThread::getMarkedCount() const {
    return markedCount.load();
}

size_t MarkerThread::getBlockedIndex() const {
    return blockedIndex.load();
}

void MarkerThread::threadFunction() {
    try {
        startEvent.wait();
        
        srand(static_cast<unsigned int>(id));
        
        while (running.load()) {
            size_t index = static_cast<size_t>(rand()) % arrayManager->getSize();
            
            try {
                if (arrayManager->markElement(index, id)) {
                    std::this_thread::sleep_for(5ms);
                    markedCount.store(arrayManager->countMarkedElements(id));
                    std::this_thread::sleep_for(5ms);
                } else {
                    std::cout << "Marker " << id 
                              << " blocked. Marked elements: " << arrayManager->countMarkedElements(id) 
                              << ", blocked at index: " << index << std::endl;
                    
                    markedCount.store(arrayManager->countMarkedElements(id));
                    blockedIndex.store(index);
                    blocked.store(true);
                    blockedEvent.signal();
                    
                    commandEvent.wait();
                    blocked.store(false);
                    blockedEvent.reset();
                    
                    if (command.load() == MarkerCommand::Terminate) {
                        resetMarkedElements();
                        break;
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "Error in marker thread " << id << ": " << e.what() << std::endl;
                break;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal error in marker thread " << id << ": " << e.what() << std::endl;
    }
    
    running.store(false);
}

void MarkerThread::resetMarkedElements() {
    try {
        const size_t arraySize = arrayManager->getSize();
        
        for (size_t i = 0; i < arraySize; ++i) {
            arrayManager->resetElement(i, id);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error while resetting marked elements: " << e.what() << std::endl;
    }
}