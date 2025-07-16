# README.md — Prado Editor Testing Layer (`/tests` Folder)

## Overview

This folder contains Google Test (`GTest`) unit tests for the Prado Editor application. These tests ensure the correctness, reliability, and robustness of the core functionalities, including artwork parsing, image manipulation, and gallery operations.

## Test Files

1. **`testing.cpp`**
-**Purpose:** Implements test cases using GTest to validate:
   - Artwork Retrieval
      - `getArtworkDescription()`: Confirms correct description retrieval by ID.
      - `getArtworkByID()`: Tests image loading for a given artwork ID.
   - Image Editing & Filters
      - `applyFilter()`: Ensures grayscale and invert filters apply correctly.
      - `cropImage()`: Tests cropping with valid and out-of-bounds coordinates.
      - `editImage()`: Applies multiple edits (crop + filter) in a single operation.
      - `rotateImage()`: Verifies image rotation works without errors.
    - Gallery Operations
      - Sorting by Newest and Artist.
      - Handling of invalid sorting criteria without exceptions.
      - Retrieving full artwork gallery.
    - Edge Cases
      - Invalid artwork IDs return empty values without crashing.
2. **`CMakeLists.txt`**
-**Purpose:** Configures the Google Test build environment for the Prado Editor tests.
-**Features:**
   - Builds two executables:
      - `runTests`: Compiles and links `testing.cpp` with core application logic for testing.
      - `OpenCVTest`: For OpenCV image operations (optional).
   - Enables CMake testing with `enable_testing()`.
   
## Dependencies

The test layer requires:

### External Libraries
- [Google Test](https://github.com/google/googletest): Unit testing framework.
- [OpenCV](https://opencv.org/): Used for creating and validating test images (cv::Mat).
- [Qt](Qt): Required indirectly for project components under test.

### C++ Standard Library
- `<iostream>`: Debugging output.
- `<map>`: Parameter handling for image operations.

## Executing the Program

Enable testing using the CMake (`CMakeLists.txt` file).

### Build and Run:
<pre> 
   mkdir build && cd build
   
   cmake ..
   
   make
   
   ctest --verbose
</pre>

## Authors
Huiwen Jia, Lucas Gieber, Sarah Wood, Taro Welches

From: MADR 3081W
