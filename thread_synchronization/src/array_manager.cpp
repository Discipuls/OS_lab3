#include "array_manager.h"
#include <iostream>
#include <stdexcept>

ArrayManager::ArrayManager(size_t size) {
    try {
        array.resize(size, 0);
    } catch (const std::bad_alloc& e) {
        throw std::runtime_error("Failed to allocate array memory: " + std::string(e.what()));
    }
}

ArrayManager::~ArrayManager() = default;

bool ArrayManager::markElement(size_t index, int markerValue) {
    std::lock_guard<std::mutex> lock(arrayMutex);
    
    if (index >= array.size()) {
        throw std::out_of_range("Array index out of bounds");
    }
    
    if (array[index] == 0) {
        array[index] = markerValue;
        return true;
    }
    
    return false;
}

bool ArrayManager::resetElement(size_t index, int markerValue) {
    std::lock_guard<std::mutex> lock(arrayMutex);
    
    if (index >= array.size()) {
        throw std::out_of_range("Array index out of bounds");
    }
    
    if (array[index] == markerValue) {
        array[index] = 0;
        return true;
    }
    
    return false;
}

size_t ArrayManager::countMarkedElements(int markerValue) const {
    std::lock_guard<std::mutex> lock(arrayMutex);
    
    size_t count = 0;
    for (size_t i = 0; i < array.size(); ++i) {
        if (array[i] == markerValue) {
            ++count;
        }
    }
    
    return count;
}

void ArrayManager::printArray() const {
    std::lock_guard<std::mutex> lock(arrayMutex);
    
    std::cout << "Array contents: [";
    for (size_t i = 0; i < array.size(); ++i) {
        std::cout << array[i];
        if (i < array.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

size_t ArrayManager::getSize() const {
    return array.size();
}

int ArrayManager::getElementAt(size_t index) const {
    std::lock_guard<std::mutex> lock(arrayMutex);
    
    if (index >= array.size()) {
        throw std::out_of_range("Array index out of bounds");
    }
    
    return array[index];
}