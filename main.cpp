#include "prado_editor.h"
#include <QtWidgets/QApplication>
#include <image_cache.h>

#ifdef _WIN32
#include <windows.h>
#include <iostream>
#endif

static void initializeConsole() {
#ifdef _WIN32
    AllocConsole();                   // Allocate a new console window
    FILE* fpOut;
    freopen_s(&fpOut, "CONOUT$", "w", stdout);  // Redirect stdout to console
    freopen_s(&fpOut, "CONOUT$", "w", stderr);  // Redirect stderr to console
    std::cout << "Custom console initialized" << std::endl;
#endif
}

int main(int argc, char *argv[])
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
    ImageCache::maxImages = 3;
    ImageCache::addImage("1", cv::Mat(100, 100, CV_8UC3));
    ImageCache::addImage("2", cv::Mat(100, 100, CV_8UC3));
    ImageCache::addImage("3", cv::Mat(100, 100, CV_8UC3));

    ImageCache::updateUsage("1");

    ImageCache::addImage("4", cv::Mat(100, 100, CV_8UC3));

    ImageCache::getCachedImage("2");

    window.show();
    return app.exec();
}
