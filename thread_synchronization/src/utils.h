#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <limits>
#include <string>

int getValidInput(int min, int max) {
    int value;
    while (true) {
        try {
            std::string input;
            std::getline(std::cin, input);
            
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Invalid input");
            }
            
            size_t pos;
            value = std::stoi(input, &pos);
            
            if (pos != input.size()) {
                throw std::invalid_argument("Input contains non-numeric characters");
            }
            
            if (value < min || value > max) {
                throw std::out_of_range("Input out of valid range");
            }
            
            return value;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << ". Valid range is [" << min << ", " << max << "]. Try again: ";
        }
    }
}

bool getYesNo(const std::string& prompt) {
    std::cout << prompt << " (y/n): ";
    std::string response;
    
    while (true) {
        std::getline(std::cin, response);
        
        if (response == "y" || response == "Y") {
            return true;
        } else if (response == "n" || response == "N") {
            return false;
        } else {
            std::cout << "Please enter 'y' or 'n': ";
        }
    }
}

template<typename T>
class ScopedResource {
public:
    explicit ScopedResource(T* resource) : resource(resource) {}
    
    ~ScopedResource() {
        delete resource;
        resource = nullptr;
    }
    
    ScopedResource(const ScopedResource&) = delete;
    ScopedResource& operator=(const ScopedResource&) = delete;
    
    ScopedResource(ScopedResource&& other) noexcept : resource(other.resource) {
        other.resource = nullptr;
    }
    
    ScopedResource& operator=(ScopedResource&& other) noexcept {
        if (this != &other) {
            delete resource;
            resource = other.resource;
            other.resource = nullptr;
        }
        return *this;
    }
    
    T* get() const {
        return resource;
    }
    
    T* operator->() const {
        return resource;
    }
    
    T& operator*() const {
        return *resource;
    }
    
    void reset(T* newResource = nullptr) {
        delete resource;
        resource = newResource;
    }

private:
    T* resource;
};

template<typename T, typename... Args>
ScopedResource<T> makeScopedResource(Args&&... args) {
    return ScopedResource<T>(new T(std::forward<Args>(args)...));
}

#endif