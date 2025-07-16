README.txt for include folder

Directory Overview:
--------------------
1. `artwork.h` / `artwork.cpp`
   - Defines the `Artwork` struct to hold all image and metadata fields.
   - Provides a global `GlobalGallery` for storing artwork data at runtime.
   - Includes a helper for splitting subtitles into structured components.

2. `managers.h` / `EditorManager.cpp`
   - Handles all image editing functionality:
     * `applyFilter`, `cropImage`, `rotateImage`, and `resetImage`.
   - Fetches images based on ID and integrates with OpenCV for transformations.

3. `image_scroll_gallery.h` / `.cpp`
   - Manages the image viewing interface using Qt's `QListView`.
   - (Deprecated in current setup) Initially supported infinite scroll + caching, but now disabled in favor of direct global loading.

4. `image_cache.h` / `.cpp`
   - A shared utility for managing image memory through LRU caching.
   - Can be re-enabled for future memory efficiency needs.

5. `prado_editor.h` / `PradoEditor.cpp`
   - Initializes the main window for the application and hooks up UI.
   - Based on the `ui_PradoEditor` layout file.

6. `main.cpp`
   - The application's entry point.
   - Loads the smaller paintings dataset (50 entries).
   - Initializes and prints the artwork gallery.
   - Executes various test cases (image filters, sort logic, etc.).
   - Launches the GUI.

Important Notes:
-----------------
- The current data loader now properly handles quoted CSV fields, allowing commas inside text attributes like descriptions.
- The entire scroll-based lazy loading system has been disabled in favor of a single global vector (`GlobalGallery`) that loads and displays all 50 artworks immediately.
- Future work may reintroduce cache mechanisms or link this system to a PostgreSQL backend.

Dependencies:
--------------
- OpenCV (image manipulation)
- Qt (GUI interface)
- C++17+

Build System:
--------------
This project assumes use of qmake or CMake depending on platform. Ensure OpenCV and Qt paths are configured correctly.

Credits:
---------
Contributors: Huiwen Jia, Lucas Giebler, Sarah Wood, Taro Welches
Course: 3081W – Software Engineering Interfaces
Institution: University of Minnesota
