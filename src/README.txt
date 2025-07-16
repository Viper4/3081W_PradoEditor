# Prado Editor – Implementation Layer (CPP Files)

This directory contains the implementation files for the Prado Editor application. These source files define the core business logic, user interface behavior, and image processing functionality.

.cpp Files

1. `main.cpp`
Handles application entry point. Loads artwork metadata from a CSV, populates the global gallery, tests filter and sort functionality, and launches the UI via `PradoEditor`.

(Note: main_old.cpp is simply an old version of main to look back for reference and serves no functionality in the latest program)

2. `managers.cpp`
Implements the `ArtworkManager` and `EditorManager` methods. Handles:

- Loading and caching images
- Applying filters (grayscale, invert, blur)
- Cropping and rotating images
- Resetting image to original

3. `artwork.cpp`
Implements support functions for parsing artwork metadata and initializing the `Artwork` struct. Contains parsing logic for CSV fields.

4. `image_cache.cpp`
Manages caching of OpenCV `cv::Mat` images using a static map. Provides fast retrieval and ensures only one copy of each image is held in memory.

5. `image_scroll_gallery.cpp`
Implements a Qt widget (`ImageScrollGallery`) to show artwork images in a grid view. Supports scrolling and updating visible images dynamically.

6. `prado_editor.cpp`
Implements the main application window setup. Connects the UI defined in Qt Designer to the rest of the application logic.

---


The application separates concerns clearly:

- `main.cpp` handles orchestration and runtime logic.
- `managers.cpp` deals with image editing logic.
- `artwork.cpp` parses structured data from CSV.
- `image_scroll_gallery.cpp` and `prado_editor.cpp` implement the user interface.

