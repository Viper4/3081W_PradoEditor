# 3081W_PradoEditor
The Prado Editor is a C++ mobile application developed for MADR 3081W: Program Design & Development. It allows users to browse, sort, search, and edit artworks from the Museo Nacional del Prado in Madrid. The application offers:
- Access to artwork metadata: title, artist, year, medium, description, etc.
- Image manipulation tools: crop, rotate, and apply filters
- Dynamic exploration of the Prado Museum collection with a modern interactive interface

This project demonstrates object-oriented programming, large-scale system design/documentation, and file I/O techniques in C++.

<!-- Table of Contents -->
## :notebook_with_decorative_cover: Table of Contents
- [:star2: Description](#star2-description)
  * [:art: User Interface & Experience](#art-user-interface--experience)
  * [:dart: Objective](#dart-objective)
- [:toolbox: Getting Started](#toolbox-getting-started)
  * [:bangbang: Prerequisites](#bangbang-prerequisites)
  * [:gear: Installing](#gear-installing)
  * [:running: Executing program](#running-executing-program)
  * [:test_tube: Running Tests](#test-running-tests)
- [:eyes: Usage](#eyes-usage)
  * [:grey_question: FAQ](#faq)
- [:wave: Authors](#wave-authors)
- [:scroll: Version History](#scroll-version-history)
- [:handshake: Help & Documentation](#help)
- [:warning: License](#warning-license)
- [:gem: Acknowledgements](#gem-acknowledgements)

## :star2: Description
The application reads artwork metadata from a structured CSV file (sourced from an open-access database). Users can:
- **Sort and search** artworks dynamically by various attributes
- **View** detailed artwork information
- **Edit** the images by cropping, applying filters, & rotating
- **Save** the edited image for future reference

### :art: User Interface & Experience
The application will feature a minimalistic, touch-friendly GUI optimized for mobile users:
- **Home Page:**
  - Scrollable gallery with artwork thumbnails.
- **Action Buttons:**
  - Quick access to filters, crop, rotate, and reset.
- **Detailed View Page:**
  - Displays the selected artwork in full with metadata (title, artist, year, description)
 
### :dart: Objective
Allowing users to customize the Prado’s famous paintings will provide a modern twist on classical artistry through providing an interactive engagement platform. The goal of this project is to increase students’ knowledge and enjoyment of historic Spanish art.

## :toolbox: Getting Started
### :bangbang: Prerequisites
Ensure the following libraries and tools are installed:
- **Operating System:** macOS, Linux, or Windows 10+
- **C++ Compiler:** GCC or Clang with C++17 support
- [CMake](https://cmake.org/) ≥ 3.10
- [Qt](https://www.qt.io/) ≥ 5.15 (for GUI components)
- [OpenCV](https://opencv.org/) ≥ 4.0 (for image processing)
- [Google Test](https://github.com/google/googletest) (for unit testing)

**Standard Library headers used include:**
`<iostream>`, `<vector>`, `<map>`, `<unordered_map>`, `<list>`, `<regex>`

**Qt modules:**
`QMainWindow`, `QPixmap`, `QScrollBar`, `QListView`, `QStandardItemModel`, `QStringListModel`, `QVBoxLayout`, `QtWidgets/QApplication`

### :gear: Installing
To get a local copy of this application up and running follow these simple example steps:

1. **Clone the repository:**
```bash
git clone https://github.com/username/3081W_PradoEditor.git

cd 3081W_PradoEditor
```
    
2. **Install dependencies via your OS package manager:**

   LINUX (Ubuntu/Debian):
   
   ```bash
   sudo apt-get install libopencv-dev qtbase5-dev
   ```

   MacOS (Homebrew):

   ```bash
   brew install opencv qt
   ```

   Windows:
   
   1. Follow the installation guide here: https://docs.opencv.org/4.x/d3/d52/tutorial_windows_install.html  
   2. Download the `.exe` file, run the file, & place the generated opencv folder anywhere on your computer  
   3. Add `...\opencv\build\x64\vc16\bin` to your system environment path variable  
   
### :running: Executing program

3. **Build the project:**
Create a `build` directory, configure with CMake, and compile:
   
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

   Windows (Microsoft Visual Studio):  
   1. Navigate to Project > Properties > VC++ Directories  
   2. Add the following path to your 'Include Directories': `...\opencv\build\include`  
   3. Add the following path to your 'Library Directories': `...\opencv\build\x64\vc16\lib ` 
   4. Navigate to Project > Properties > Linker > Input  
   5. Add the following file to your 'Additional Dependencies': `opencv_world4120d.lib`  
      - Replace 4120 with whatever OpenCV version you are using  
      - 'd' for debug version, remove it for release version  

5. **Run the program:**  
   Linux/macOS:  
   ```bash
   ./PradoEditor
   ```

   Windows:  
   - Build & run the program via MS Visual Studio
   
### :test_tube: Running Tests
Unit tests are implemented with **Google Test:**

```bash
ctest --verbose
```   
   
## :eyes: Usage

### Searching the Gallery

- Type either the artist name, artwork title, or ID into the artwork search bar
- Browse and select the artwork you want to edit. Supported format: `.jpg`.

### **Opening an Image**

- Click on the selected artwork, it will open in a new page as a `.jpg` image.
- View additional information about the artwork, such as its description, year created, and dimensions.

### **Editing Workflow**

- Use the toolbar options to apply filters, crop, or rotate the image.
- Applying Filters
    - Select between grayscale, invert, and blur options using the toolbar.
- Cropping the Image
    - Adjust the cropping border on the image and drag to adjust the image size.
- Rotating the Image
    - Enter the desired rotation angle in degrees and click “Rotate”.

### Saving & Exporting an Image

- Click “Save” to save an edited copy of the original artwork to your device.
  
### :grey_question: FAQ 
**Q: The application can't find Qt or OpenCV during build. What should I do?**  
A: Make sure you have installed the required dependencies. For Windows, set environment variables:
```bash
setx OpenCV_DIR "C:\path\to\opencv\build"
setx Qt5_DIR "C:\path\to\Qt\5.x\msvc2019_64\lib\cmake\Qt5"
```

**Q: Images are not loading in the gallery. Why?**
A: Check that:

The `images/` directory exists in the project root.

Filenames in the CSV match the actual image names.

## :wave: Authors
- [Huiwen Jia](https://github.com/jAww404)
- [Lucas Gieber](https://github.com/Viper4)
- [Sarah Wood](https://github.com/SarahJWood)
- [Taro Welches](https://github.com/tarowelches)

## :scroll: Version History
- 1.1
  - Implemented basic UI using Qt.
  - Full access to the Prado's painting collection using cURL.
  - Added artwork sorting options.
- 1.0
  - Initial release

## :handshake: Help & Documentation

Project Link: https://github.com/Viper4/3081W_PradoEditor

Full Documentation: 
- [Users Manual](https://www.notion.so/Prado-Editor-User-s-Manual-2374a6836ecd80fb9fe6dd5145d7040a?source=copy_link)
- [Developer's Guide](https://www.notion.so/Prado-Editor-Developer-s-Guide-2374a6836ecd8063aee8e5920f76e917?source=copy_link)

## :warning: License 

This project is licensed under the MIT License - see the `LICENSE.md` file for details

<!-- Acknowledgments -->
## :gem: Acknowledgements
 - [Awesome README](https://github.com/matiassingers/awesome-readme)
 - [Emoji Cheat Sheet](https://github.com/ikatyang/emoji-cheat-sheet/blob/master/README.md#travel--places)
 - [Readme Template](https://github.com/othneildrew/Best-README-Template)
 - [Prado Images Kaggle Dataset](https://www.kaggle.com/datasets/maparla/prado-museum-pictures)
