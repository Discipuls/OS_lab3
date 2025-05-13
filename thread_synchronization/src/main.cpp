#include "array_manager.h"
#include "thread_manager.h"
#include "utils.h"
#include <iostream>
#include <memory>
#include <stdexcept>

int main() {
    try {
        std::cout << "Thread Synchronization Program" << std::endl;
        std::cout << "------------------------------" << std::endl;
        
        std::cout << "Enter array size: ";
        const int arraySize = getValidInput(1, 1000);
        
        auto arrayManager = std::make_shared<ArrayManager>(static_cast<size_t>(arraySize));
        arrayManager->printArray();
        
        std::cout << "Enter number of marker threads: ";
        const int threadCount = getValidInput(1, 100);
        
        auto threadManager = std::make_shared<ThreadManager>(arrayManager);
        threadManager->createThreads(threadCount);
        
        std::cout << "Starting all marker threads..." << std::endl;
        threadManager->startAllThreads();
        
        while (!threadManager->areAllThreadsFinished()) {
            threadManager->waitForAllThreadsBlocked();
            
            std::cout << "All threads are now blocked." << std::endl;
            arrayManager->printArray();
            
            const auto activeThreadIds = threadManager->getActiveThreadIds();
            if (activeThreadIds.empty()) {
                break;
            }
            
            std::cout << "Active threads: ";
            for (size_t i = 0; i < activeThreadIds.size(); ++i) {
                std::cout << activeThreadIds[i];
                if (i < activeThreadIds.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
            
            std::cout << "Enter thread ID to terminate: ";
            const int threadIdToTerminate = getValidInput(1, threadCount);
            
            auto thread = threadManager->findThreadById(threadIdToTerminate);
            if (!thread) {
                std::cout << "Thread " << threadIdToTerminate << " is not active." << std::endl;
                continue;
            }
            
            std::cout << "Terminating thread " << threadIdToTerminate << "..." << std::endl;
            threadManager->terminateThread(threadIdToTerminate);
            
            std::cout << "Thread " << threadIdToTerminate << " terminated." << std::endl;
            arrayManager->printArray();
            
            if (threadManager->areAllThreadsFinished()) {
                std::cout << "All threads have finished." << std::endl;
                break;
            }
            
            std::cout << "Continuing other threads..." << std::endl;
            threadManager->continueOtherThreads();
        }
        
        std::cout << "Program finished successfully." << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred." << std::endl;
        return 1;
    }
}