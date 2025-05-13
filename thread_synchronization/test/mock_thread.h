#ifndef MOCK_THREAD_H
#define MOCK_THREAD_H

#include <gmock/gmock.h>
#include "marker_thread.h"

class MockMarkerThread : public MarkerThread {
public:
    MockMarkerThread(int id, std::shared_ptr<ArrayManager> arrayManager)
        : MarkerThread(id, arrayManager) {}
        
    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, signalStart, (), (override));
    MOCK_METHOD(void, waitForBlocking, (), (override));
    MOCK_METHOD(void, sendCommand, (MarkerCommand command), (override));
    MOCK_METHOD(void, join, (), (override));
    MOCK_METHOD(bool, isRunning, (), (const, override));
    MOCK_METHOD(bool, isBlocked, (), (const, override));
    MOCK_METHOD(int, getId, (), (const, override));
    MOCK_METHOD(size_t, getMarkedCount, (), (const, override));
    MOCK_METHOD(size_t, getBlockedIndex, (), (const, override));
};

#endif