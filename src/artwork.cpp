#include "include/artwork.h"
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <include/image_cache.h>
#include <include/managers.h>
#include <regex>

std::vector<Artwork> PradoEditorMobileInterface::getArtworkGallery()
{
    // RESOLVE: What are we doing here? Just copying the GlobalGallery??
    // Contributors: 
    // Purpose: 
    // Limitations: 
	
    /*std::vector<Artwork> result;
    result = GlobalGallery;
    for (const auto &[id, artwork] : GlobalGallery)
    {
        result.push_back(artwork);
    }
    return result;*/
    return GlobalGallery;
}

void PradoEditorMobileInterface::sortArtworks(const SortCriteria &criteria)
{
    // Contributors: Taro Welches and Lucas Giebler
    // Purpose: To sort the displayed artwork by a certain criteria input by user
    // Parameters: const std::string& criteria
    // Return: void
    // Limitations: 

    switch (criteria) {
    case Title:
        std::sort(GlobalGallery.begin(), GlobalGallery.end(), 
            [](const Artwork& a, const Artwork& b) {
                return a.metadata.at("work_title") < b.metadata.at("work_title");
            });
        break;
    case Newest:
        std::sort(GlobalGallery.begin(), GlobalGallery.end(), 
            [](const Artwork& a, const Artwork& b) {
                return a.metadata.at("sheet_date") > b.metadata.at("sheet_date");
            });
        break;
    case Oldest:
        std::sort(GlobalGallery.begin(), GlobalGallery.end(), 
            [](const Artwork& a, const Artwork& b) {
                return a.metadata.at("sheet_date") < b.metadata.at("sheet_date");
            });
        break;
    case Artist:
        std::sort(GlobalGallery.begin(), GlobalGallery.end(), 
            [](const Artwork& a, const Artwork& b) {
                return a.metadata.at("author") < b.metadata.at("author");
            });
        break;
	default:
        std::cout << "Unknown sorting criteria: " << criteria << std::endl;
        break;
    }
}

SubtitleData PradoEditorMobileInterface::splitSubtitle(const std::string &work_subtitle)
{
    // Contributors : Sarah and Taro and Lucas
    // Purpose : split the given string subtitle description into a struct containing separate year, medium, and dimensions
    // Parameters : work_subtitle: the original subtitle field in paragraph form
    // Return Value: subtitle: a struct of the original subtitle field
    // Limitations: 
	
    // NOTE: We no longer need this function since there are already columns in the dataset for year, technique, and dimensions
    SubtitleData subtitle;
    std::regex pattern(R"(([^.]*)\. ([^,]*), (.*))");
    std::smatch matches;

    if (std::regex_match(work_subtitle, matches, pattern)) {
        std::cout << "Full match: " << matches[0] << "\n";
        std::cout << "Group 1 (before period): " << matches[1] << "\n";
        std::cout << "Group 2 (before comma): " << matches[2] << "\n";
        std::cout << "Group 3 (rest): " << matches[3] << "\n";

		subtitle.year = matches[1];
		subtitle.medium = matches[2];
		subtitle.dimensions = matches[3];
    }
    else {
        std::cerr << "No match found for subtitle: " << work_subtitle << std::endl;
    }

    return subtitle;
}

cv::Mat getImage(const std::string &artworkId)
    // Contributors : 
    // Purpose : 
    // Parameters : 
    // Return Value: 
	// ::param artworkID: 
    // Limitations: 
{
    std::cout << "make some sort of call to getimagecache and then error handle\n";
    return cv::Mat();
}
