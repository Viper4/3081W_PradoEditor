#include "artwork.h"
#include <opencv2/imgcodecs.hpp>  


std::vector<Artwork> PradoEditorMobileInterface::getArtworkGallery() {
    std::vector<Artwork> result;
    for (const auto& [id, artwork] : GlobalGallery) {
        result.push_back(artwork);
    }
    return result;
}


    //Contributors: Taro Welches and Lucas Giebler
    //Input: const std::string& criteria
    //Purpose: To sort the displayed artwork by a certain criteria input by user
    //return: none
void PradoEditorMobileInterface::sortArtworks(const std::string& criteria) {
    if (criteria != "Newest" && criteria != "Oldest" && criteria != "Artist") {
        std::cerr << "Unknown sorting criteria: " << criteria << std::endl;
        return;
    }

    for (size_t i = 0; i < GlobalGallery.size(); ++i) {
        for (size_t j = i + 1; j < GlobalGallery.size(); ++j) {
            bool shouldSwap = false;

            if (criteria == "Newest") {
                shouldSwap = GlobalGallery[j].year > GlobalGallery[i].year;
            } else if (criteria == "Oldest") {
                shouldSwap = GlobalGallery[j].year < GlobalGallery[i].year;
            } else if (criteria == "Artist") {
                shouldSwap = GlobalGallery[j].author < GlobalGallery[i].author;
            }

            if (shouldSwap) {
                std::swap(GlobalGallery[i], GlobalGallery[j]);
            }
        }
    }
}
std::vector<Artwork> PradoEditorMobileInterface::searchArtworks(const std::string& query) {
    std::vector<Artwork> SearchGallery;
    //Contributors: Taro 
    //Input: const std::string& criteria
    //Purpose: To search the gallery by the user's input and return a new gallery full of new artworks
    //return: SearchGallery, a vector of artwork objects
    for (size_t i = 0; i < GlobalGallery.size(); ++i) {
        const Artwork& art = GlobalGallery[i];
        if (art.work_title.find(query) != std::string::npos ||
            art.author.find(query) != std::string::npos ||
            art.year.find(query) != std::string::npos ||
            art.work_description.find(query) != std::string::npos) {
            SearchGallery.push_back(art);
        }
    }
    return SearchGallery;
}



std::string PradoEditorMobileInterface::getArtworkDescription(const std::string& artworkId) {
    Artwork* art = findArtworkById(artworkId);
    if (!art) {
        std::cerr << "Error: artwork not found\n";
        return "Description not available";
    }
    return art->work_description;
}


SubtitleData PradoEditorMobileInterface::splitSubtitle(const std::string &work_subtitle) {
    // Contributors: Sarah, Taro, and Lucas
    // Purpose: Split the subtitle into year, medium, and dimensions
    // Input: Full subtitle string (e.g., "1854. Óleo sobre lienzo, 60 x 50 cm")
    // Output: SubtitleData with parsed year, medium, dimensions

    SubtitleData subtitle;
    size_t dotPos = work_subtitle.find('.');

    // Find first 4-digit year before the first '.'
    size_t yrPos = std::string::npos;
    for (size_t i = 0; i + 3 < dotPos; i++) {
        if (isdigit(work_subtitle[i]) && isdigit(work_subtitle[i + 1]) &&
            isdigit(work_subtitle[i + 2]) && isdigit(work_subtitle[i + 3])) {
            subtitle.year = work_subtitle.substr(i, 4);
            yrPos = i;
            break;
        }
    }

    if (yrPos == std::string::npos) {
        subtitle.year = "Unknown";
        subtitle.medium = "Unknown";
        subtitle.dimensions = "Unknown";
        return subtitle;
    }

    size_t medPos = work_subtitle.find(',', dotPos);  // Find comma *after* the period
    if (medPos != std::string::npos) {
        std::string medium = work_subtitle.substr(dotPos + 1, medPos - dotPos - 1);
        subtitle.medium = trim(medium);
        subtitle.dimensions = trim(work_subtitle.substr(medPos + 1));
    } else {
        subtitle.medium = trim(work_subtitle.substr(dotPos + 1));
        subtitle.dimensions = "Unknown";
    }

    return subtitle;
}




