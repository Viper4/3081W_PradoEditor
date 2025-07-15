#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <opencv2/opencv.hpp>
#include "include/artwork.h"
#include "include/managers.h"
#include <include/prado_editor.h>
#include <QtWidgets/QApplication>
#include <include/image_cache.h>
#include <include/image_scroll_gallery.h>

std::vector<Artwork> GlobalGallery;

#ifdef _WIN32
#include <windows.h>
#endif

static void initializeConsole() {
    // Contributors: Lucas Giebler
    // Purpose: Initialize a custom console UI for debugging
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

static void printArtworkVector(const std::vector<Artwork>& vec, const std::string& label) {
    // Contributors: Lucas Giebler
	// Purpose: Print the contents of the Artwork vector
	// Parameters: const std::vector<Artwork>& vec
	// Return Value: void
    std::cout << label << std::endl;
    for (const auto& art : vec) {
        std::cout << "Artwork " << art.id << std::endl;
        std::cout << " Title: " << art.title << std::endl;
        std::cout << " Author: " << art.author << std::endl;
        std::cout << " Subtitle: " << art.subtitle << std::endl;
        std::cout << " Description: " << art.description << std::endl;
        std::cout << " Year: " << art.year << std::endl;
		std::cout << " Image URL: " << art.image_url << std::endl;
    }
	std::cout << "-------------------" << std::endl;
}

int main(int argc, char* argv[])
{
    initializeConsole();

    // ------------------- LOAD CSV FILE -------------------
    std::ifstream file("images/clean_smallerpaintings.csv");
    std::string line;
    std::getline(file, line); // Skip header

    PradoEditorMobileInterface pradoInterface;
    ArtworkManager artworkManager;

    int idcount = 0;

    while (std::getline(file, line))
    {
        std::stringstream stream(line);
        Artwork art;
        SubtitleData subtitle;

        std::cout << "LINE: " << line << std::endl;

        // Headers are:
        // work_url,work_image_url,author,author_bio,author_url,author_id,work_title,work_subtitle,work_exposed,work_description,
        // work_tags,technical_sheet_numero_de_catalogo,technical_sheet_autor,technical_sheet_titulo,technical_sheet_fecha,technical_sheet_tecnica,
        // technical_sheet_soporte,technical_sheet_dimension,technical_sheet_serie,technical_sheet_procedencia,bibliography,inventory,expositions,
        // ubication,technical_sheet_autores,technical_sheet_edicion_/_estado,technical_sheet_materia,technical_sheet_ceca,technical_sheet_autora,
        // technical_sheet_lugar_de_produccion

        std::getline(stream, art.image_url, ',');
        std::getline(stream, art.author, ',');
        std::getline(stream, art.title, ',');
        std::getline(stream, art.subtitle, ',');
        std::getline(stream, art.description, ',');
        std::getline(stream, art.id, ','); // Use catalog ID

        // Replacing weird characters because of our python CSV cleaner
        // Replace | with ,
        std::replace(art.image_url.begin(), art.image_url.end(), '|', ',');
        std::replace(art.author.begin(), art.author.end(), '|', ',');
		std::replace(art.title.begin(), art.title.end(), '|', ',');
		std::replace(art.subtitle.begin(), art.subtitle.end(), '|', ',');
		std::replace(art.description.begin(), art.description.end(), '|', ',');
        std::replace(art.id.begin(), art.id.end(), '|', ',');

        // Replace ~ with "
        std::replace(art.image_url.begin(), art.image_url.end(), '~', '"');
        std::replace(art.author.begin(), art.author.end(), '~', '"');
        std::replace(art.title.begin(), art.title.end(), '~', '"');
        std::replace(art.subtitle.begin(), art.subtitle.end(), '~', '"');
        std::replace(art.description.begin(), art.description.end(), '~', '"');
        std::replace(art.id.begin(), art.id.end(), '~', '"');

        subtitle = pradoInterface.splitSubtitle(art.subtitle);
        art.year = subtitle.year;
        GlobalGallery.push_back(art);
        ++idcount;

        printArtworkVector(GlobalGallery, "GlobalGallery");
    }

    std::cout << "Loaded " << GlobalGallery.size() << " artworks.\n";

    // ------------------- APPLY FILTER -------------------
    std::cout << "\n[ApplyFilter] Test: grayscale\n";
    cv::Mat colorImg = cv::Mat::ones(10, 10, CV_8UC3) * 255;
    std::map<std::string, int> grayscaleParams = { {"type", 1} };
    cv::Mat grayImg = artworkManager.applyFilter(colorImg, grayscaleParams);
    std::cout << "Channels after grayscale: " << grayImg.channels() << " (Expected: 1)\n";

    std::cout << "\n[ApplyFilter] Test: invert\n";
    cv::Mat blackImg = cv::Mat::zeros(10, 10, CV_8UC1);
    std::map<std::string, int> invertParams = { {"type", 2} };
    cv::Mat whiteImg = artworkManager.applyFilter(blackImg, invertParams);
    std::cout << "Pixel[0,0] after invert: " << static_cast<int>(whiteImg.at<uchar>(0, 0)) << " (Expected: 255)\n";

    // ------------------- CROP IMAGE -------------------
    std::cout << "\n[CropImage] Test: crop 5x5\n";
    cv::Mat input = cv::Mat::ones(10, 10, CV_8UC1) * 200;
    std::map<std::string, int> cropParams = { {"x", 2}, {"y", 2}, {"width", 5}, {"height", 5} };
    try
    {
        cv::Mat cropped = artworkManager.cropImage(input, cropParams);
        std::cout << "Cropped size: " << cropped.rows << "x" << cropped.cols << " (Expected: 5x5)\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "CropImage error: " << e.what() << "\n";
    }

    // ------------------- GET ARTWORK BY ID -------------------
    std::cout << "\n[GetArtworkByID] Test: lookup by ID\n";
    std::string testId = "art001"; // must exist in your CSV
    Artwork found = artworkManager.getArtworkByID(testId);
    if (!found.title.empty())
    {
        std::cout << "Found artwork: " << found.title << " by " << found.author << "\n";
    }
    else
    {
        std::cout << "Artwork ID '" << testId << "' not found.\n";
    }

    // ------------------- SPLIT SUBTITLE -------------------
    std::cout << "\n[SplitSubtitle] Test:\n";
    if (!GlobalGallery.empty())
    {
        std::string subtitleText = GlobalGallery[0].subtitle;
        SubtitleData parsed = pradoInterface.splitSubtitle(subtitleText);
        std::cout << "Subtitle: " << subtitleText << "\n";
        std::cout << "Parsed year: " << parsed.year << "\n";
        std::cout << "Parsed medium: " << parsed.medium << "\n";
        std::cout << "Parsed dimensions: " << parsed.dimensions << "\n";
    }

    // ------------------- SORT GLOBAL GALLERY -------------------
	std::cout << "\n[SortGlobalGallery] Test:\n";

	pradoInterface.sortArtworks(SortCriteria::Title);
    printArtworkVector(GlobalGallery, "----Artwork Gallery After Title Sort----");

    pradoInterface.sortArtworks(SortCriteria::Newest);
    printArtworkVector(GlobalGallery, "----Artwork Gallery After Newest Sort----");

    pradoInterface.sortArtworks(SortCriteria::Oldest);
    printArtworkVector(GlobalGallery, "----Artwork Gallery After Oldest Sort----");

    pradoInterface.sortArtworks(SortCriteria::Artist);
    printArtworkVector(GlobalGallery, "----Artwork Gallery After Artist Sort----");

    // ------------------- IMAGE CACHE AND IMAGE SCROLL GALLERY UI -------------------
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

// Prepare dummy GlobalGallery for tests
void setupDummyGallery()
{
    GlobalGallery.clear();

    Artwork a1, a2, a3;
    a1.id = "001";
    a1.title = "Title A";
    a1.author = "Artist Z";
    a1.year = "2000";

    a2.id = "002";
    a2.title = "Title B";
    a2.author = "Artist Y";
    a2.year = "1995";

    a3.id = "003";
    a3.title = "Title C";
    a3.author = "Artist X";
    a3.year = "2020";

    GlobalGallery = {a1, a2, a3};
}
