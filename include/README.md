### README.md — Prado Editor Interfaces (.h Files)

## Overview:

This document describes the header files that define the core architecture of the Prado Editor project. These interfaces collectively describe how artwork data is managed, edited, and displayed in the application. They do not contain implementation logic—only declarations and structural design—for better maintainability and modularity.

## Header Interfaces:

1. **`artwork.h`**
   - **Purpose:** Defines the `Artwork` struct representing a single art piece and its attributes.
   - **Key Features:**
        - Attributes include: `work_title`, `author`, `work_url`, `work_image_url`, `work_subtitle`, and `work_description`, 
        - Also defines `SubtitleData`, a helper struct used to split the `work_subtitle` into:
             - `year`: the year the painting creation began
             - `medium`: the material of the painting (e.g. acrylics, oil, etc.)
             - `dimensions`: the size of the painting
        - Declares a global `std::vector<Artwork> GlobalGallery` that stores all artworks at runtime.

2. **`managers.h`**
   - **Purpose:** Declares `ArtworkManager` and `EditorManager` classes.
   - **Classes:**
        - `ArtworkManager`: Handles image retrieval, cropping, filtering, and rotation.
        - `EditorManager`: Manages UI-level interactions, editing requests, and state resets.

3. **`prado_editor.h`**
   - **Purpose:** Declares the `PradoEditor` class that inherits from `QMainWindow`. Connects to a UI layout file (`ui_PradoEditor`) to create the main application window.
   - **NOTE:** Does not contain business or image processing logic.

4. **`image_scroll_gallery.h`**
   - **Purpose:** Defines the `ImageScrollGallery` class for displaying images in a scrollable Qt list view. Primary usage in development phase of the product before global gallery implementation.
   - **Features:**
      - Dynamic image loading
      - Geometry configuration
      - Scroll detection.

5. **`image_cache.h`**
   - **Purpose:** Declares the `ImageCache` class for memory-efficient image storage. 
   - **Methods:**
      - `getCachedImage`
      - `addImage`
      - `updateUsage` 
   - **NOTE:** scroll+cache approach was deprecated in Version 1.0, this header remains for optional reactivation in later versions.

## Purpose:

These headers separate interfaces from implementation to promote clean modularization by:
- Isolating declarations (interfaces) from logic (in .cpp files),
- Supporting easy unit testing and mocking
- Promoting reusability across GUI and image modules

## Dependencies:

The header files in this folder rely on the following libraries and components:

### [**OpenCV**](https://opencv.org/)
Provides image processing functionality. cv::Mat is used for operations such as cropping, resizing, and filtering artwork images to maintain their resolution and bounds.

### [**Qt**](https://www.qt.io/)
Required for GUI components and image handling. Includes:
- QMainWindow (in prado_editor.h) for the main application window.
- QPixmap for efficient image representation and rendering in the UI.

### **C++ Standard Library**
Common containers and utilities:
- <map>: For ordered key-value storage.
- <unordered_map>: For hash-based key-value storage with faster lookups.
- <list>: For managing dynamic collection of image cache.
- <vector> and <string>: Widely used for storing artwork data and metadata.

## Authors:

Huiwen Jia, Lucas Giebler, Sarah Wood, Taro Welches

For: MADR3081W
