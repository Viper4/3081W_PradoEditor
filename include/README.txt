README.txt — Prado Editor Interfaces

Overview:
---------
This document describes the header/interface files that define the core architecture of the Prado Editor project.
These interfaces collectively describe how artwork data is managed, edited, and displayed in the application.
They do not contain implementation logic — only declarations and structural design.

Header Interfaces:
------------------

1. artwork.h
   - Defines the `Artwork` struct representing a single art piece.
   - Attributes include: title, author, image URL, subtitle, year, description, dimensions, and more.
   - Also defines `SubtitleData`, a helper struct used to split the `subtitle` into:
       • year
       • medium
       • dimensions
   - Declares a global `std::vector<Artwork> GlobalGallery` that stores all artworks at runtime.

2. managers.h
   - Declares `ArtworkManager` and `EditorManager` classes.
   - `ArtworkManager`: Handles fetching, cropping, filtering, and editing images.
   - `EditorManager`: Manages UI-level editing interactions and state resets.
   - Both classes work with OpenCV (`cv::Mat`) to process artwork images.

3. prado_editor.h
   - Declares the `PradoEditor` class that inherits from `QMainWindow`.
   - Connects to a UI layout file (`ui_PradoEditor`) to create the main application window.
   - Does not contain business or image logic.

4. image_scroll_gallery.h
   - Defines the `ImageScrollGallery` class for displaying images in a scrollable Qt list view.
   - Used primarily in earlier versions of the project (prior to moving to global gallery load).
   - Supports dynamic image loading, geometry configuration, and scroll detection.

5. image_cache.h
   - Declares the `ImageCache` class to manage memory-efficient storage of artwork images.
   - Provides `getCachedImage`, `addImage`, and `updateUsage` to implement a basic image cache.
   - Although the scroll+cache approach was deprecated, this header remains for optional reactivation.

Purpose:
--------
These headers separate interface from implementation. They enable clean modularization by:
- Isolating declarations (interfaces) from logic (in .cpp files),
- Supporting easy unit testing and mocking,
- Promoting reusability across GUI and image modules.



Authors:
--------
Huiwen Jia, Lucas Giebler, Sarah Wood, Taro Welches
For: MADR3081W
