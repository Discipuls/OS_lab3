markdown
# Thread Synchronization with Critical Sections and Events

## Overview
This project demonstrates thread synchronization using critical sections and events in a C++ application. The main process consists of a `main` thread and multiple `marker` threads. The application handles deadlocks and ensures proper synchronization between threads.

## Features
- **Thread Synchronization**: Uses critical sections and events to synchronize threads.
- **Deadlock Handling**: Implements mechanisms to detect and handle deadlocks.
- **Dynamic Array Management**: Allocates and manages an array of integers dynamically.
- **User Interaction**: Allows user input for array size and thread management.

## Project Structure
```
thread_synchronization/
├── CMakeLists.txt          # Main CMake file
├── src/
│   ├── main.cpp            # Entry point
│   ├── thread_manager.h    # Thread management interface
│   ├── thread_manager.cpp  # Thread management implementation
│   ├── marker_thread.h     # Marker thread definition
│   ├── marker_thread.cpp   # Marker thread implementation
│   ├── array_manager.h     # Array management interface
│   ├── array_manager.cpp   # Array management implementation
│   ├── sync_primitives.h   # Synchronization primitives (Events, etc.)
│   ├── sync_primitives.cpp # Synchronization implementation
│   └── utils.h             # Utility functions and error handling
├── test/
│   ├── CMakeLists.txt      # Test CMake file
│   ├── unit_tests.cpp      # Unit tests
│   ├── mock_array.h        # Mock array for testing
│   └── mock_thread.h       # Mock thread for testing
└── Makefile                # Main Makefile for building the project
```
## Requirements
- C++17 or later
- CMake 3.10 or later
- A C++ compiler that supports C++17 (e.g., GCC, Clang, MSVC)

## Building the Project
1. **Clone the Repository**:
    ```sh
    git clone <repository-url>
    cd thread_synchronization
    ```

2. **Create a Build Directory**:
    ```sh
    mkdir build
    cd build
    ```

3. **Run CMake**:
    ```sh
    cmake ..
    ```

4. **Build the Project**:
    ```sh
    cmake --build .
    ```

## Running the Application
After building the project, you can run the application from the build directory:
```sh
./thread_synchronization
```

## Usage
1. **Input Array Size**: Enter the size of the array when prompted.
2. **Input Number of Threads**: Enter the number of `marker` threads to launch.
3. **Interact with Threads**: Follow the prompts to manage the threads and view the array contents.

## Testing
To run the unit tests, navigate to the build directory and run:
```sh
ctest
```

## Code Structure
- **main.cpp**: The entry point of the application. Handles user input and manages the main thread.
- **thread_manager.h/cpp**: Manages the creation and synchronization of `marker` threads.
- **marker_thread.h/cpp**: Defines the behavior of the `marker` threads.
- **array_manager.h/cpp**: Manages the dynamic array and its operations.
- **sync_primitives.h/cpp**: Implements synchronization primitives like critical sections and events.
- **utils.h**: Contains utility functions and error handling routines.

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request with your changes.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
