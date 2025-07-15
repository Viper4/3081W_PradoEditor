#include <include/prado_editor.h>
#include <QtWidgets/QApplication>
#include <include/image_cache.h>
#include <include/image_scroll_gallery.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

static void initializeConsole() {
    // Contributors: Lucas Giebler
	// Purpose: Initialize the console for debugging
    // Parameters: 
    // Return Value: void
	// -------------------
#ifdef _WIN32
    AllocConsole();                   // Allocate a new console window
    FILE* fpOut;
    freopen_s(&fpOut, "CONOUT$", "w", stdout);  // Redirect stdout to console
    freopen_s(&fpOut, "CONOUT$", "w", stderr);  // Redirect stderr to console
    std::cout << "Custom console initialized" << std::endl;
#else
    // Redirect stdout and stderr to a log file on macOS/Linux
    FILE* fpOut = freopen("/tmp/myapp_stdout.log", "w", stdout);
    FILE* fpErr = freopen("/tmp/myapp_stderr.log", "w", stderr);
    if (fpOut && fpErr) {
        std::cout << "Console output redirected to /tmp/*.log (Unix)" << std::endl;
    }
    else {
        std::cerr << "Failed to redirect console output on Unix" << std::endl;
    }
#endif
}

int main_old(int argc, char *argv[])
{
    //Tasks for Main
    // The data will be cleaned, with only paintings that have the attribute of year
    //1. With this data, we have to first initialize the image cache with 50 or however many image structs
    //2. Obtain the jpg or png file from these structs and display them
    //3. Update the image cache based on the index of scroll and continue to display new paintings as the user scrolls
    //4. On event of a click of an image display the options such as edit, reset, or display description
    initializeConsole();

    QApplication app(argc, argv);
    PradoEditor window;
    cv::Mat temp_image = cv::imread("C:\\Users\\vpr16\\Documents\\Random\\Absolute Cinema.jpg");
    for (int i = 10; i < 75; i++) {
		ImageCache::addImage(std::to_string(i), temp_image);
    }

    ImageScrollGallery gallery = ImageScrollGallery(&window, 0, 0, 500, 500, 10, 10, 1000, 3, 125, 150);

    window.show();
    return app.exec();
}
