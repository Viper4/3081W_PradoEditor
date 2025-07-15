#include "artwork.h"
#include "EditorManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Artwork> GlobalGallery; // Declare global gallery

int main() {
    std::ifstream file("smallerpaintings.csv");
    std::string line;
    std::getline(file, line); // Skip header

    PradoEditorMobileInterface interface;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;

        Artwork art;
        SubtitleData subtitle;

        std::getline(ss, art.work_image_url, ',');
        std::getline(ss, art.author, ',');
        std::getline(ss, art.work_title, ',');
        std::getline(ss, art.work_subtitle, ',');
        std::getline(ss, art.work_description, ',');
        std::getline(ss, art.id, ','); // Use catalog ID as unique artwork ID

        subtitle = interface.splitSubtitle(art.work_subtitle);
        art.year = subtitle.year;

        GlobalGallery.push_back(art);
    }

    std::cout << "Loaded " << GlobalGallery.size() << " artworks.\n";

    return 0;
}
