#include "artwork.h"
#include <opencv2/imgcodecs.hpp>  


std::vector<Artwork> PradoEditorMobileInterface::getArtworkGallery() {
    std::vector<Artwork> result;
    for (const auto& [id, artwork] : GlobalGallery) {
        result.push_back(artwork);
    }
    return result;
}

void PradoEditorMobileInterface::sortArtworks(const std::string& criteria) {
    //Contributors: Taro Welches and Lucas Giebler
    //Input: const std::string& criteria
    //Purpose: To sort the displayed artwork by a certain criteria input by user
    //return: none
    std::vector<std::pair<std::string, Artwork>> entries(GlobalGallery.begin(), GlobalGallery.end());

    if (criteria == "Newest") {
        std::sort(entries.begin(), entries.end(),
                  [](const auto& a, const auto& b) { return a.second.year > b.second.year; });
    } else if (criteria == "Oldest") {
        std::sort(entries.begin(), entries.end(),
                  [](const auto& a, const auto& b) { return a.second.year < b.second.year; });
    } else if (criteria == "Artist") {
        std::sort(entries.begin(), entries.end(),
                  [](const auto& a, const auto& b) { return a.second.author < b.second.author; });//alphabetical ig
    } else {
        std::cerr << "Unknown sorting criteria: " << criteria << std::endl;
        return;
    }

    GlobalGallery.clear();
    for (const auto& [id, artwork] : entries) {
        GlobalGallery[id] = artwork;
    }
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




