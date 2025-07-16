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
#include <QVBoxLayout>

std::vector<Artwork> GlobalGallery;

#ifdef _WIN32
#include <windows.h>
#endif

static void initializeConsole() {
    // Contributors: Lucas Giebler
    // Purpose: Initialize a custom console UI for debugging
    // Parameters: 
    // Return Value: void
    // Limitations: 
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

static void printArtwork(const Artwork& art) {
    // Contributors: Lucas Giebler
    // Purpose: Print a few properties of the given Artwork
    // Parameters: Artwork art - artwork to print
    // Return Value: void
    // Limitations: 
    // -------------------
    std::cout << "\nArtwork " << art.metadata.at("id") << std::endl;
    std::cout << " Title: " << art.metadata.at("work_title") << std::endl;
    std::cout << " Author: " << art.metadata.at("author") << std::endl;
    std::cout << " Subtitle: " << art.metadata.at("subtitle") << std::endl;
    std::cout << " Description: " << art.metadata.at("description") << std::endl;
    std::cout << " Year: " << art.metadata.at("sheet_date") << std::endl;
    std::cout << " Image URL: " << art.metadata.at("image_url") << std::endl;
}

static void printArtworkVector(const std::vector<Artwork>& vec, const std::string& label, int count) {
    // Contributors: Lucas Giebler
	// Purpose: Print the contents of the given Artwork vector
	// Parameters: const std::vector<Artwork>& vec - vector to print
    //             const std::string& label - label to print before printing artworks
    //             int count - maximum number of artworks to print
	// Return Value: void
	// Limitations: 
    std::cout << label << std::endl;
    for (int i = 0; i < min(count, vec.size()); i++) {
        Artwork art = vec.at(i);
        printArtwork(art);
    }
	std::cout << "-------------------" << std::endl;
}

int main(int argc, char* argv[])
{
    // Contributors: Lucas, Taro, Sarah, and Huiwen
    // Purpose: Run the application
    // Parameters: int argc - number of command line arguments
    //             char* argv[] - array of command line arguments
    // Return Value: void
    // Limitations: 
    // -------------------
    initializeConsole();

    // ------------------- LOAD CSV FILE -------------------
    std::ifstream file("images/clean_prado.csv");
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

        // Headers are:
        // work_url,work_image_url,author,author_bio,author_url,author_id,work_title,work_subtitle,work_exposed,work_description,
        // work_tags,technical_sheet_numero_de_catalogo,technical_sheet_autor,technical_sheet_titulo,technical_sheet_fecha,technical_sheet_tecnica,
        // technical_sheet_soporte,technical_sheet_dimension,technical_sheet_serie,technical_sheet_procedencia,bibliography,inventory,expositions,
        // ubication,technical_sheet_autores,technical_sheet_edicion_/_estado,technical_sheet_materia,technical_sheet_ceca,technical_sheet_autora,
        // technical_sheet_lugar_de_produccion

        std::getline(stream, art.metadata["work_url"], ',');
        std::getline(stream, art.metadata["image_url"], ',');
        std::getline(stream, art.metadata["author"], ',');
        std::getline(stream, art.metadata["author_bio"], ',');
        std::getline(stream, art.metadata["author_url"], ',');
        std::getline(stream, art.metadata["author_id"], ',');
        std::getline(stream, art.metadata["work_title"], ',');
        std::getline(stream, art.metadata["subtitle"], ',');
        std::getline(stream, art.metadata["work_exposed"], ','); // Where in the museum the artwork is
        std::getline(stream, art.metadata["description"], ',');
        std::getline(stream, art.metadata["work_tags"], ',');
        std::getline(stream, art.metadata["id"], ','); // Catalog number is our ID
        std::getline(stream, art.metadata["sheet_author"], ','); // Redundant, same as author
		std::getline(stream, art.metadata["sheet_title"], ','); // Redundant, same as work_title
		std::getline(stream, art.metadata["sheet_date"], ','); // Date when the artwork was created
		std::getline(stream, art.metadata["sheet_technique"], ','); // Technique used to make the artwork
		std::getline(stream, art.metadata["sheet_support"], ','); // What the artwork was made on for paintings (paper, canvas, etc.)
		std::getline(stream, art.metadata["sheet_dimensions"], ',');
		std::getline(stream, art.metadata["sheet_series"], ',');
		std::getline(stream, art.metadata["sheet_origin"], ',');
		std::getline(stream, art.metadata["bibliography"], ',');
		std::getline(stream, art.metadata["inventory"], ',');
		std::getline(stream, art.metadata["expositions"], ','); // Description of the signature for the artwork I think?
		std::getline(stream, art.metadata["publication"], ',');
		std::getline(stream, art.metadata["sheet_authors"], ',');
		std::getline(stream, art.metadata["sheet_edition"], ',');
        std::getline(stream, art.metadata["sheet_material"], ','); // Material of the artwork for non-paintings (bronze, stone, etc.)
        std::getline(stream, art.metadata["sheet_ceca"], ',');
		std::getline(stream, art.metadata["sheet_autora"], ','); // Redundant, same as author
		std::getline(stream, art.metadata["sheet_production_place"], ',');

        // Replacing weird characters because of our python CSV cleaner
        for (auto& [key, value] : art.metadata) {
            // Replace | with ,
			std::replace(value.begin(), value.end(), '|', ',');

			// Replace ~ with "
			std::replace(value.begin(), value.end(), '~', '"');
        }

        GlobalGallery.push_back(art);
        ++idcount;
    }
    printArtworkVector(GlobalGallery, "GlobalGallery", 999999);

    std::cout << "Loaded " << GlobalGallery.size() << " artworks.\n";

    // ------------------- APPLY FILTER -------------------
    cv::Mat test_image = cv::imread("C:\\Users\\vpr16\\Documents\\Random\\robloxDefault.png");

    std::cout << "\n[ApplyFilter] Test: grayscale\n";
    std::map<std::string, int> grayscaleParams = { {"type", 1} };
    cv::Mat grayImg = artworkManager.applyFilter(test_image, grayscaleParams);
    ImageCache::addImage("0", test_image);
    ImageCache::addImage("1", grayImg);

    std::cout << "\n[ApplyFilter] Test: invert\n";
    std::map<std::string, int> invertParams = { {"type", 2} };
    cv::Mat invertedImg = artworkManager.applyFilter(test_image, invertParams);
    ImageCache::addImage("2", invertedImg);

    std::cout << "\n[ApplyFilter] Test: blur\n";
    std::map<std::string, int> blurParams = { {"type", 3} };
    cv::Mat blurredImg = artworkManager.applyFilter(test_image, blurParams);
    ImageCache::addImage("3", blurredImg);

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
    std::string testId = "P002073"; // must exist in your CSV
    Artwork found = artworkManager.getArtworkByID(testId);
    if (!found.metadata.empty())
    {
        std::cout << "Found artwork: " << std::endl;
        printArtwork(found);
    }
    else
    {
        std::cout << "Artwork ID '" << testId << "' not found." << std::endl;
    }

    // ------------------- SPLIT SUBTITLE -------------------
    std::cout << "\n[SplitSubtitle] Test:\n";
    if (!GlobalGallery.empty())
    {
        std::string subtitleText = GlobalGallery[0].metadata.at("subtitle");
        SubtitleData parsed = pradoInterface.splitSubtitle(subtitleText);
        std::cout << "Subtitle: " << subtitleText << "\n";
        std::cout << "Parsed year: " << parsed.year << "\n";
        std::cout << "Parsed medium: " << parsed.medium << "\n";
        std::cout << "Parsed dimensions: " << parsed.dimensions << "\n";
    }

    // ------------------- SORT GLOBAL GALLERY -------------------
	std::cout << "\n[SortGlobalGallery] Test:\n";

	pradoInterface.sortArtworks(SortCriteria::Title);
    printArtworkVector(GlobalGallery, "----Artwork Gallery After Title Sort----", 5);

    pradoInterface.sortArtworks(SortCriteria::Newest);
    printArtworkVector(GlobalGallery, "----Artwork Gallery After Newest Sort----", 5);

    pradoInterface.sortArtworks(SortCriteria::Oldest);
    printArtworkVector(GlobalGallery, "----Artwork Gallery After Oldest Sort----", 5);

    pradoInterface.sortArtworks(SortCriteria::Artist);
    printArtworkVector(GlobalGallery, "----Artwork Gallery After Artist Sort----", 5);

    // ------------------- IMAGE CACHE AND IMAGE SCROLL GALLERY UI -------------------
    // Initialize image cache with some images from 10 to 75
    cv::Mat temp_image = cv::imread("C:\\Users\\vpr16\\Documents\\Random\\Absolute Cinema.jpg");
    for (int i = 10; i < 50; i++) {
        ImageCache::addImage(std::to_string(i), temp_image);
    }

    QApplication app(argc, argv);
    PradoEditor window;
    window.resize(1000, 600);
    ImageScrollGallery gallery = ImageScrollGallery(&window, window.width() / 2 - 250, window.height() / 2 - 250, 500, 500, 10, 10, 1000, 3, 125, 150);
    window.show();

    return app.exec();
}

// Prepare dummy GlobalGallery for tests
void setupDummyGallery()
{
    // Contributors: Lucas Giebler
    // Purpose: Initialize a custom console UI for debugging
    // Parameters: 
    // Return Value: void
    // Limitations: 
    // -------------------
    GlobalGallery.clear();

    Artwork a1, a2, a3;
    a1.metadata.at("id") = "001";
    a1.metadata.at("work_title") = "Title A";
    a1.metadata.at("author") = "Artist Z";
    a1.metadata.at("sheet_date") = "2000";

    a2.metadata.at("id") = "002";
    a2.metadata.at("work_title") = "Title B";
    a2.metadata.at("author") = "Artist Y";
    a2.metadata.at("sheet_date") = "1995";

    a3.metadata.at("id") = "003";
    a3.metadata.at("work_title") = "Title C";
    a3.metadata.at("author") = "Artist X";
    a3.metadata.at("sheet_date") = "2020";

    GlobalGallery = {a1, a2, a3};
}
