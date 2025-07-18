#include <debug.h>
#include <iostream>
#include <artwork.h>
#include <opencv2/core/utils/logger.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

void Debug::initializeConsole() {
    // Contributors: Lucas Giebler
    // Purpose: Initialize a custom console UI for debugging
    // Parameters: 
    // Return Value: void
    // Limitations: 
    // -------------------
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
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

void Debug::printArtwork(const Artwork& art, const bool verbose) {
    // Contributors: Lucas Giebler
    // Purpose: Print a few properties of the given Artwork
    // Parameters: Artwork art - artwork to print
    //             bool verbose - whether to print all properties
    // Return Value: void
    // Limitations: Expects Artwork to have at least id, work_title, author, subtitle, description, sheet_date, and image_url properties in metadata
    // -------------------
    std::cout << "\nArtwork " << art.metadata.at("id") << std::endl;
    if (verbose) {
        for (const auto& [key, value] : art.metadata) {
			std::cout << " " << key << ": " << value << std::endl;
		}
    }
    else {
        std::cout << " Title: " << art.metadata.at("work_title") << std::endl;
        std::cout << " Author: " << art.metadata.at("author") << std::endl;
        std::cout << " Subtitle: " << art.metadata.at("subtitle") << std::endl;
        std::cout << " Year: " << art.metadata.at("sheet_date") << std::endl;
        std::cout << " Image URL: " << art.metadata.at("image_url") << std::endl;
    }
}

void Debug::printArtworkMap(const std::unordered_map<std::string, Artwork>& map, const std::string& label, const int count) {
    // Contributors: Lucas Giebler
    // Purpose: Print the contents of the given Artwork vector
    // Parameters: const std::unordered_map<std::string, Artwork>& map - map to print
    //             const std::string& label - label to print before printing artworks
    //             int count - maximum number of artworks to print
    // Return Value: void
    // Limitations: 
    std::cout << label << std::endl;

    std::unordered_map<std::string, Artwork>::iterator itr;
    int i = 0;
    for (auto itr = map.begin(); itr != map.end() && i < count; itr++) {
        Artwork art = itr->second;
        printArtwork(art, false);
        i++;
    }
    std::cout << "-------------------" << std::endl;
}

void Debug::printArtworkVector(const std::vector<Artwork>& vector, const std::string& label, const int count) {
    // Contributors: Lucas Giebler
    // Purpose: Print the contents of the given Artwork vector
    // Parameters: const std::vector<Artwork>& vector - vector to print
    //             const std::string& label - label to print before printing artworks
    //             int count - maximum number of artworks to print
    // Return Value: void
    // Limitations: 
    std::cout << label << std::endl;
    for (int i = 0; i < vector.size() && i < count; i++) {
        printArtwork(vector[i], false);
    }
}
