
.PHONY: all clean test coverage

all: build
	@echo "Building thread synchronization project..."
	@cd build && cmake --build .

build:
	@echo "Configuring CMake..."
	@mkdir -p build
	@cd build && cmake ..

clean:
	@echo "Cleaning build directory..."
	@rm -rf build

test: all
	@echo "Running tests..."
	@cd build && ctest --verbose

coverage: clean
	@echo "Building with coverage information..."
	@mkdir -p build
	@cd build && cmake -DENABLE_COVERAGE=ON ..
	@cd build && cmake --build .
	@cd build && ctest --verbose
	@cd build && lcov --capture --directory . --output-file coverage.info
	@cd build && lcov --remove coverage.info '/usr/*' --output-file coverage.info
	@cd build && genhtml coverage.info --output-directory coverage_report
	@echo "Coverage report generated in build/coverage_report/index.html"

ifeq ($(OS),Windows_NT)
clean:
	@echo "Cleaning build directory..."
	@if exist build rmdir /s /q build
endif