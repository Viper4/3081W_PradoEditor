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
    //Contributors: Taro Welches
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
                  [](const auto& a, const auto& b) { return a.second.author < b.second.author; });
    } else if (criteria == "Style") {
        std::sort(entries.begin(), entries.end(),
                  [](const auto& a, const auto& b) { return a.second.workSubtitle < b.second.workSubtitle; });
    } else {
        std::cerr << "Unknown sorting criteria: " << criteria << std::endl;
        return;
    }

    GlobalGallery.clear();
    for (const auto& [id, artwork] : entries) {
        GlobalGallery[id] = artwork;
    }
}


Artwork PradoEditorMobileInterface::getArtworkDescription(const std::string& artworkId) {
    Artwork* art = findArtworkById(artworkId);
    if (!art) {
        std::cerr << "Error: artwork not found\n";
        return {};
    }
    return *art;
}


SubtitleData PradoEditorMobileInterface::splitSubtitle(const std::string &work_subtitle){
        // Contributors : Sarah
        // Purpose : split the given string subtitle description into a struct containing separate year, medium, and
        // Parameters : work_subtitle: the original subtitle field in paragraph form
        // Return Value: subtitle: a struct of the original subtitle field
        SubtitleData subtitle;

        size_t yrPos = work_subtitle.find('.');
        std::string year = work_subtitle.substr(0, yrPos);
        subtitle.year = std::string(trim(year));

        size_t medPos = work_subtitle.find(',');
        std::string medium = work_subtitle.substr(yrPos + 1, medPos - yrPos - 1);
        subtitle.medium = std::string(trim(medium));

        subtitle.dimensions = trim(work_subtitle.substr(medPos + 1));

        return subtitle;
}

void PradoEditorMobileInterface::cropImage(const std::string& artworkId, int x, int y, int width, int height) {
    std::cout << "Cropping image for artwork ID: " << artworkId
              << " to region x=" << x << " y=" << y
              << " width=" << width << " height=" << height << std::endl;
    // Placeholder
}

void cv::Mat getImage(const std::string& artworkId){
    std::cout << "make some sort of call to getimagecache and then error handle\n";
}
