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


SubtitleData PradoEditorMobileInterface::splitSubtitle(const std::string &work_subtitle){
        // Contributors : Sarah and Taro and Lucas
        // Purpose : split the given string subtitle description into a struct containing separate year, medium, and
        // Parameters : work_subtitle: the original subtitle field in paragraph form
        // Return Value: subtitle: a struct of the original subtitle field
        SubtitleData subtitle;
        size_t dotPos = work_subtitle.find('.');

        for (size_t i = 0; i<dotPos; i++) {
            if (isdigit(prefix[i]) && isdigit(prefix[i+1]) &&
                isdigit(prefix[i+2]) && isdigit(prefix[i+3])) {
                subtitle.year = prefix.substr(i, 4);
                break;
            }
        }

        size_t medPos = work_subtitle.find(',');
        std::string medium = work_subtitle.substr(yrPos + 1, medPos - yrPos - 1);
        subtitle.medium = std::string(trim(medium));

        subtitle.dimensions = trim(work_subtitle.substr(medPos + 1));

        return subtitle;
}

void cv::Mat getImage(const std::string& artworkId){
    std::cout << "make some sort of call to getimagecache and then error handle\n";
}
