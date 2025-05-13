#include <gtest/gtest.h>
#include <memory>
#include <thread>
#include <chrono>

#include "array_manager.h"
#include "marker_thread.h"
#include "thread_manager.h"
#include "sync_primitives.h"

using namespace std::chrono_literals;

class ArrayManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        arrayManager = std::make_shared<ArrayManager>(10);
    }
    
    std::shared_ptr<ArrayManager> arrayManager;
};

TEST_F(ArrayManagerTest, InitialArrayIsEmpty) {
    for (size_t i = 0; i < arrayManager->getSize(); ++i) {
        EXPECT_EQ(arrayManager->getElementAt(i), 0);
    }
}

TEST_F(ArrayManagerTest, MarkElementWorks) {
    EXPECT_TRUE(arrayManager->markElement(5, 1));
    EXPECT_EQ(arrayManager->getElementAt(5), 1);
}

TEST_F(ArrayManagerTest, MarkElementFailsWhenAlreadyMarked) {
    EXPECT_TRUE(arrayManager->markElement(5, 1));
    EXPECT_FALSE(arrayManager->markElement(5, 2));
    EXPECT_EQ(arrayManager->getElementAt(5), 1);
}

TEST_F(ArrayManagerTest, ResetElementWorks) {
    EXPECT_TRUE(arrayManager->markElement(5, 1));
    EXPECT_TRUE(arrayManager->resetElement(5, 1));
    EXPECT_EQ(arrayManager->getElementAt(5), 0);
}

TEST_F(ArrayManagerTest, ResetElementFailsWithWrongMarker) {
    EXPECT_TRUE(arrayManager->markElement(5, 1));
    EXPECT_FALSE(arrayManager->resetElement(5, 2));
    EXPECT_EQ(arrayManager->getElementAt(5), 1);
}

TEST_F(ArrayManagerTest, CountMarkedElementsWorks) {
    EXPECT_TRUE(arrayManager->markElement(1, 1));
    EXPECT_TRUE(arrayManager->markElement(3, 1));
    EXPECT_TRUE(arrayManager->markElement(5, 1));
    EXPECT_TRUE(arrayManager->markElement(7, 2));
    
    EXPECT_EQ(arrayManager->countMarkedElements(1), 3);
    EXPECT_EQ(arrayManager->countMarkedElements(2), 1);
    EXPECT_EQ(arrayManager->countMarkedElements(3), 0);
}

TEST_F(ArrayManagerTest, OutOfBoundsAccess) {
    EXPECT_THROW(arrayManager->markElement(20, 1), std::out_of_range);
    EXPECT_THROW(arrayManager->resetElement(20, 1), std::out_of_range);
    EXPECT_THROW(arrayManager->getElementAt(20), std::out_of_range);
}

class EventTest : public ::testing::Test {
protected:
    Event event;
};

TEST_F(EventTest, NewEventIsNotSignaled) {
    EXPECT_FALSE(event.isSignaled());
}

TEST_F(EventTest, SignalSetsEvent) {
    event.signal();
    EXPECT_TRUE(event.isSignaled());
}

TEST_F(EventTest, ResetClearsEvent) {
    event.signal();
    event.reset();
    EXPECT_FALSE(event.isSignaled());
}

TEST_F(EventTest, WaitReturnsImmediatelyIfSignaled) {
    event.signal();
    auto start = std::chrono::steady_clock::now();
    event.wait();
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    EXPECT_LT(duration.count(), 10);
}

TEST_F(EventTest, WaitForReturnsTrue) {
    event.signal();
    EXPECT_TRUE(event.waitFor(100ms));
}

TEST_F(EventTest, WaitForReturnsFalseOnTimeout) {
    EXPECT_FALSE(event.waitFor(50ms));
}

class MarkerThreadTest : public ::testing::Test {
protected:
    void SetUp() override {
        arrayManager = std::make_shared<ArrayManager>(10);
        markerThread = std::make_shared<MarkerThread>(1, arrayManager);
    }
    
    std::shared_ptr<ArrayManager> arrayManager;
    std::shared_ptr<MarkerThread> markerThread;
};

TEST_F(MarkerThreadTest, InitialIdIsCorrect) {
    EXPECT_EQ(markerThread->getId(), 1);
}

TEST_F(MarkerThreadTest, ThreadRunsAndBlocks) {
    
    markerThread->start();
    markerThread->signalStart();
    markerThread->waitForBlocking();
    
    
    EXPECT_TRUE(markerThread->isBlocked());
    EXPECT_GT(markerThread->getMarkedCount(), 0);
    
    
    markerThread->sendCommand(MarkerCommand::Terminate);
    markerThread->join();
    
    size_t zeroCount = 0;
    for (size_t i = 0; i < arrayManager->getSize(); ++i) {
        if (arrayManager->getElementAt(i) == 0) {
            zeroCount++;
        }
    }
    
    EXPECT_EQ(zeroCount, arrayManager->getSize());
    EXPECT_FALSE(markerThread->isRunning());
}

class ThreadManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        arrayManager = std::make_shared<ArrayManager>(20);
        threadManager = std::make_shared<ThreadManager>(arrayManager);
    }
    
    std::shared_ptr<ArrayManager> arrayManager;
    std::shared_ptr<ThreadManager> threadManager;
};

TEST_F(ThreadManagerTest, CreateThreadsIncreasesActiveThreadCount) {
    threadManager->createThreads(3);
    EXPECT_EQ(threadManager->getActiveThreadCount(), 3);
}

TEST_F(ThreadManagerTest, FindThreadByIdWorks) {
    threadManager->createThreads(3);
    auto thread = threadManager->findThreadById(2);
    ASSERT_NE(thread, nullptr);
    EXPECT_EQ(thread->getId(), 2);
    
    auto nonExistentThread = threadManager->findThreadById(10);
    EXPECT_EQ(nonExistentThread, nullptr);
}

TEST_F(ThreadManagerTest, ThreadTerminationReducesActiveCount) {
    threadManager->createThreads(3);
    threadManager->startAllThreads();
    
    
    threadManager->waitForAllThreadsBlocked();
    
    
    threadManager->terminateThread(2);
    
    
    EXPECT_EQ(threadManager->getActiveThreadCount(), 2);
    EXPECT_EQ(threadManager->findThreadById(2), nullptr);
    
    
    threadManager->continueOtherThreads();
    threadManager->waitForAllThreadsBlocked();
    
    
    auto activeIds = threadManager->getActiveThreadIds();
    EXPECT_EQ(activeIds.size(), 2);
    EXPECT_NE(std::find(activeIds.begin(), activeIds.end(), 1), activeIds.end());
    EXPECT_NE(std::find(activeIds.begin(), activeIds.end(), 3), activeIds.end());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}