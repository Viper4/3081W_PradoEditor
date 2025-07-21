#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <opencv2/opencv.hpp>
#include "artwork.h"
#include "managers.h"
#include <prado_editor.h>
#include <QtWidgets/QApplication>
#include <image_cache.h>
#include <image_scroll_gallery.h>
#include <QVBoxLayout>
#include <debug.h>
#include <QFile>
#include <QTextStream>

static std::string parseLine(std::stringstream& stream, std::string& line, char delimiter) {
	// Contributors: Lucas Giebler
	// Purpose: Parse a line of a CSV file and remove enclosing double quotes from CSV cell format
    // Parameters: const std::stringstream& stream - stream to parse
    //             const std::string& line - line to parse
	//             char delimiter - delimiter to use
	// Return Value: string
	// Limitations: Expects the line to be in CSV format and to run this function one after another for each line
	// -------------------
    std::getline(stream, line, delimiter); // Read up to the delimiter
    int i = 0;
    while (i < line.length() && line[i] == '"' && line[line.length() - 1 - i] == '"') {
        i++;
    }
    return line.substr(i, line.length() - 2 * i);
}

static int randomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
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
    Debug::initializeConsole();

    PradoEditorMobileInterface pradoInterface;
    ArtworkManager artworkManager;

    // ------------------- LOAD CSV FILE -------------------
    std::ifstream file("images/clean_prado.csv");
    std::string line;
    std::getline(file, line); // Skip header

    int idcount = 0;

    while (std::getline(file, line))
    {
        std::stringstream stream(line);
        Artwork art;

        // Headers are:
        // work_url,work_image_url,author,author_bio,author_url,author_id,work_title,work_subtitle,work_exposed,work_description,
        // work_tags,technical_sheet_numero_de_catalogo,technical_sheet_autor,technical_sheet_titulo,technical_sheet_fecha,technical_sheet_tecnica,
        // technical_sheet_soporte,technical_sheet_dimension,technical_sheet_serie,technical_sheet_procedencia,bibliography,inventory,expositions,
        // ubication,technical_sheet_autores,technical_sheet_edicion_/_estado,technical_sheet_materia,technical_sheet_ceca,technical_sheet_autora,
        // technical_sheet_lugar_de_produccion

        art.metadata["work_url"] = parseLine(stream, line, ',');
        art.metadata["image_url"] = parseLine(stream, line, ',');
		art.metadata["author"] = parseLine(stream, line, ',');
        art.metadata["author_bio"] = parseLine(stream, line, ',');
		art.metadata["author_url"] = parseLine(stream, line, ',');
		art.metadata["author_id"] = parseLine(stream, line, ',');
		art.metadata["work_title"] = parseLine(stream, line, ',');
		art.metadata["subtitle"] = parseLine(stream, line, ',');
		art.metadata["work_exposed"] = parseLine(stream, line, ','); // Where in the museum the artwork is
		art.metadata["description"] = parseLine(stream, line, ',');
		art.metadata["work_tags"] = parseLine(stream, line, ',');
		art.metadata["id"] = parseLine(stream, line, ','); // Catalog number is our ID
		art.metadata["sheet_author"] = parseLine(stream, line, ','); // Redundant, same as author
		art.metadata["sheet_title"] = parseLine(stream, line, ','); // Redundant, same as work_title
        art.metadata["sheet_date"] = parseLine(stream, line, ','); // Date when the artwork was created
		art.metadata["sheet_technique"] = parseLine(stream, line, ','); // Technique used to make the artwork
		art.metadata["sheet_support"] = parseLine(stream, line, ','); // What the artwork was made on for paintings (paper, canvas, etc.)
		art.metadata["sheet_dimensions"] = parseLine(stream, line, ',');
		art.metadata["sheet_series"] = parseLine(stream, line, ',');
		art.metadata["sheet_origin"] = parseLine(stream, line, ',');
        art.metadata["bibliography"] = parseLine(stream, line, ',');
		art.metadata["inventory"] = parseLine(stream, line, ',');
		art.metadata["expositions"] = parseLine(stream, line, ','); // Description of the signature for the artwork I think?
		art.metadata["publication"] = parseLine(stream, line, ',');
		art.metadata["sheet_authors"] = parseLine(stream, line, ','); // Multiple authors
		art.metadata["sheet_edition"] = parseLine(stream, line, ',');
		art.metadata["sheet_material"] = parseLine(stream, line, ','); // Material of the artwork for non-paintings (bronze, stone, etc.)
		art.metadata["sheet_ceca"] = parseLine(stream, line, ',');
		art.metadata["sheet_autora"] = parseLine(stream, line, ','); // Redundant, same as author
		art.metadata["sheet_production_place"] = parseLine(stream, line, ',');

        // Replacing weird characters because of our python CSV cleaner
        for (auto& [key, value] : art.metadata) {
            // Replace | with ,
			std::replace(value.begin(), value.end(), '|', ',');

			// Replace ~ with "
			std::replace(value.begin(), value.end(), '~', '"');
        }

        if (art.metadata["id"].size() < 10) {
            ImageScrollGallery::GlobalGallery[art.metadata["id"]] = art;
            ++idcount;
        }
    }
    Debug::printArtworkMap(ImageScrollGallery::GlobalGallery, "GlobalGallery", 50);

    std::cout << "Loaded " << ImageScrollGallery::GlobalGallery.size() << " artworks.\n";
    /*
    // ------------------- APPLY FILTER -------------------
    cv::Mat testImage = cv::imread("C:\\Users\\vpr16\\Documents\\Random\\robloxDefault.png");
    cv::namedWindow("Original", cv::WINDOW_NORMAL);
    cv::resizeWindow("Original", 400, 400);
    cv::imshow("Original", testImage);

    std::cout << "\n[ApplyFilter] Test: grayscale\n";
    cv::Mat grayImg = artworkManager.applyFilter(testImage, ArtworkManager::FilterType::GRAYSCALE);
    cv::namedWindow("Grayscale", cv::WINDOW_NORMAL);
    cv::resizeWindow("Grayscale", 400, 400);
    cv::imshow("Grayscale", grayImg);

    std::cout << "\n[ApplyFilter] Test: invert\n";
    cv::Mat invertedImg = artworkManager.applyFilter(testImage, ArtworkManager::FilterType::INVERT);
    cv::namedWindow("Inverted", cv::WINDOW_NORMAL);
    cv::resizeWindow("Inverted", 400, 400);
    cv::imshow("Inverted", invertedImg);

    std::cout << "\n[ApplyFilter] Test: blur\n";
    cv::Mat blurredImg = artworkManager.applyFilter(testImage, ArtworkManager::FilterType::BLUR);
    cv::namedWindow("Blurred", cv::WINDOW_NORMAL);
    cv::resizeWindow("Blurred", 400, 400);
    cv::imshow("Blurred", blurredImg);

    std::cout << "\n[ApplyFilter] Test: hsv\n";
    cv::Mat hsvImg = artworkManager.applyFilter(testImage, ArtworkManager::FilterType::HSV);
    cv::namedWindow("HSV", cv::WINDOW_NORMAL);
    cv::resizeWindow("HSV", 400, 400);
    cv::imshow("HSV", hsvImg);

    std::cout << "\n[ApplyFilter] Test: edge case\n";
    cv::Mat edgeCaseFilter = artworkManager.applyFilter(cv::Mat(), ArtworkManager::FilterType::INVERT);

    // ------------------- CROP IMAGE -------------------
    std::cout << "\n[CropImage] Test: crop uncentered\n";
    cv::Mat cropped = artworkManager.cropImage(testImage, testImage.cols / 2, testImage.rows / 2, testImage.cols / 2, testImage.rows / 2);

    cv::namedWindow("Cropped (uncentered)", cv::WINDOW_NORMAL);
    cv::resizeWindow("Cropped (uncentered)", 400, 400);
    cv::imshow("Cropped (uncentered)", cropped);

    std::cout << "\n[CropImage] Test: crop centered\n";
    cv::Mat croppedCentered = artworkManager.cropImageCentered(testImage, testImage.cols / 2, testImage.rows / 2);

    cv::namedWindow("Cropped (centered)", cv::WINDOW_NORMAL);
    cv::resizeWindow("Cropped (centered)", 400, 400);
    cv::imshow("Cropped (centered)", croppedCentered);

    std::cout << "\n[CropImage] Test: crop edge case\n";
    cv::Mat edgeCaseCrop = artworkManager.cropImage(cv::Mat(), -100, -10, -10, -10);

    // ------------------- ROTATE IMAGE -------------------
    std::cout << "\n[RotateImage] Test: rotate 90 degrees\n";
	cv::Mat rotated = artworkManager.rotateImage(testImage, 90);

    cv::namedWindow("Rotated 90", cv::WINDOW_NORMAL);
    cv::resizeWindow("Rotated 90", 400, 400);
    cv::imshow("Rotated 90", rotated);

    std::cout << "\n[RotateImage] Test: rotate edge case\n";
    cv::Mat edgeCaseRotate = artworkManager.rotateImage(cv::Mat(), -1000);

    // ------------------- GET ARTWORK BY ID -------------------
    std::cout << "\n[GetArtworkByID] Test: lookup by ID\n";
    std::vector<std::string> testIds = { "P002073", "P002074", "Hello :)" };
    for (const std::string& testId : testIds) {
        Artwork found = artworkManager.getArtworkByID(testId);
        if (!found.metadata.empty())
        {
            std::cout << "Found artwork: " << std::endl;
            Debug::printArtwork(found, false);
        }
    }

    // ------------------- SORT GLOBAL GALLERY -------------------
	std::cout << "\n[SortGlobalGallery] Test:\n";

    std::vector<Artwork> artworks = pradoInterface.artworkMapToVector(ImageScrollGallery::GlobalGallery);

	pradoInterface.sortArtworks(SortCriteria::Title, artworks);
    Debug::printArtworkVector(artworks, "\n----Artwork Gallery After Title Sort----", 5);

    pradoInterface.sortArtworks(SortCriteria::Newest, artworks);
    Debug::printArtworkVector(artworks, "\n----Artwork Gallery After Newest Sort----", 5);

    pradoInterface.sortArtworks(SortCriteria::Oldest, artworks);
    Debug::printArtworkVector(artworks, "\n----Artwork Gallery After Oldest Sort----", 5);

    pradoInterface.sortArtworks(SortCriteria::Artist, artworks);
    Debug::printArtworkVector(artworks, "\n----Artwork Gallery After Artist Sort----", 5);
    */
    // ------------------- IMAGE CACHE AND IMAGE SCROLL GALLERY UI -------------------
    QApplication app(argc, argv);

    // Load and apply the QSS file
    QFile stylesheet1("C:/Users/vpr16/QtProjects/PradoEditor/prado_theme.qss");
    //QFile stylesheet2("C:/Users/vpr16/QtProjects/PradoEditor/prado_theme_updated.qss");
    //QFile stylesheet3("C:/Users/vpr16/QtProjects/PradoEditor/prado_theme_enhanced.qss");

    if (stylesheet1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        app.setStyleSheet(stylesheet1.readAll());
        stylesheet1.close();
    }

    PradoEditor window;
    window.resize(1000, 600);
    window.show();

    return app.exec();
}

void setupDummyGallery(int numArtworks)
{
    // Contributors: Taro Welches
    // Purpose: Prepare a dummy GlobalGallery for tests
    // Parameters: numArtworks - number of artworks to generate
    // Return Value: void
    // Limitations: Need to manually change number of artworks
    // -------------------
    ImageScrollGallery::GlobalGallery.clear();

    for (int i = 0; i < numArtworks; i++) {
        Artwork art;
		art.metadata.at("id") = std::to_string(i + 1);
        char work_char = 'A' + i;
        art.metadata.at("work_title") = "Title " + work_char;
        art.metadata.at("author") = "Artist " + work_char;
        art.metadata.at("sheet_date") = std::to_string(randomInt(10, 2000));

        ImageScrollGallery::GlobalGallery.insert({art.metadata["id"], art});
    }
}
