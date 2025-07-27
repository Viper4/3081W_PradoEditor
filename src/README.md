# Prado Editor – Implementation Layer (CPP Files)

This directory contains the C++ source files for the Prado Editor application. These source files define the business logic, UI behavior, and image processing functionality declared in the header files from the `\include` folder.

## Source Files

1. **`main.cpp`**
- **Purpose:** Acts as application entry point.
- **Features:**
    - Loads artwork metadata from a CSV
    - Populates the global gallery (`GlobalGallery`) with dataset paintings
    - Tests filtering and sorting functionality
    - Launches the main UI window via `PradoEditor`
- **NOTE:** main_old.cpp is an outdated version retained for reference; it serves no functionality in the latest program.

2. **`managers.cpp`**
- **Purpose:** Implements the `ArtworkManager` and `EditorManager` methods.
- **Features:**
  - Loading and caching images
  - Image handling with OpenCV:
      - Applying filters (grayscale, invert, blur)
      - Cropping images
      - Rotating images
  - Resetting images to their original state

3. **`artwork.cpp`**
- **Purpose:** Implements support functions for parsing and initializing artwork metadata.
- **Features:**
  - Parsing logic for CSV fields
  - Initializes `Artwork` structs and their associated attributes

4. **`image_cache.cpp`**
- **Purpose:** Provides an image caching mechanism using a static map for OpenCV `cv::Mat` objects.
- **Features:**
  - Enables fast image retrieval
  - Prevents duplicate storage: ensures only one copy of each image is held in memory

5. **`image_scroll_gallery.cpp`**
- **Purpose:** Implements a Qt widget (`ImageScrollGallery`) to display artwork images in grid view.
- **Features:**
  - Scroll-based UI updates
  - Dynamic image loading

6. **`prado_editor.cpp`**
- **Purpose:** Defines the main application window and connects the UI to the backend.
- **Features:**
  - Uses Qt Designer-generated UI (`ui_PradoEditor`).
  - Handles signal-slot connections for user interactions.

## Purpose

The application separates concerns clearly:
- `main.cpp`: orchestration and runtime logic.
- `managers.cpp`: image editing logic.
- `artwork.cpp`: metadata parsing and initialization
- `image_scroll_gallery.cpp` and `prado_editor.cpp`: UI logic

## Dependencies
The .cpp files rely on the following libraries and headers:

### [OpenCV](https://opencv.org/)
- Image processing and manipulation (`cv::Mat` for storing images).
- Used extensively in managers.cpp for filtering, cropping, and transformations.

### [Qt](https://www.qt.io/)
- GUI framework for creating the application window and widgets via `QtWidgets/QApplication`.
- `QMainWindow` in prado_editor.cpp and `QPixmap` for image rendering.

### [cURL](https://curl.se/)
-Handles HTTP(S) requests for downloading edited images from URLs.
-Integrated in `managers.cpp` for fetching artwork images referenced in the dataset.

### C++ Standard Library
- `<map>` and `<unordered_map>`: Used for caching in `image_cache.cpp`.
- `<list>`: Supports dynamic collections for caching and scroll galleries.
- `<vector>` and `<string>`: Core data structures for artwork metadata.
- `<fstream>` and `<sstream>`: CSV parsing in `artwork.cpp`.
- `<iostream>`: Console output for debugging.
- `<regex>`: Regular expression parsing (used for text or subtitle data processing).

## Authors
Huiwen Jia, Lucas Gieber, Sarah Wood, Taro Welches
  
For: MADR 3081W
