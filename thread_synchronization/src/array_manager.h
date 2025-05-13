#ifndef ARRAY_MANAGER_H
#define ARRAY_MANAGER_H

#include <memory>
#include <mutex>
#include <vector>

class ArrayManager {
public:
    explicit ArrayManager(size_t size);
    ~ArrayManager();

    ArrayManager(const ArrayManager&) = delete;
    ArrayManager& operator=(const ArrayManager&) = delete;
    ArrayManager(ArrayManager&&) = delete;
    ArrayManager& operator=(ArrayManager&&) = delete;

    bool markElement(size_t index, int markerValue);
    bool resetElement(size_t index, int markerValue);
    size_t countMarkedElements(int markerValue) const;
    void printArray() const;
    size_t getSize() const;
    int getElementAt(size_t index) const;

private:
    std::vector<int> array;
    mutable std::mutex arrayMutex;
};

#endif