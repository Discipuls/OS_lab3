#ifndef MOCK_ARRAY_H
#define MOCK_ARRAY_H

#include <gmock/gmock.h>
#include "array_manager.h"

class MockArrayManager : public ArrayManager {
public:
    explicit MockArrayManager(size_t size) : ArrayManager(size) {}
    
    MOCK_METHOD(bool, markElement, (size_t index, int markerValue), (override));
    MOCK_METHOD(bool, resetElement, (size_t index, int markerValue), (override));
    MOCK_METHOD(size_t, countMarkedElements, (int markerValue), (const, override));
    MOCK_METHOD(void, printArray, (), (const, override));
    MOCK_METHOD(size_t, getSize, (), (const, override));
    MOCK_METHOD(int, getElementAt, (size_t index), (const, override));
};

#endif