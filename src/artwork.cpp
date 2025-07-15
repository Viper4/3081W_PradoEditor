#include "include/artwork.h"
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <include/image_cache.h>
#include <include/managers.h>
#include <regex>

std::vector<Artwork> PradoEditorMobileInterface::getArtworkGallery()
{
    // What are we doing here? Just copying the GlobalGallery??

    /*std::vector<Artwork> result;
    result = GlobalGallery;
    for (const auto &[id, artwork] : GlobalGallery)
    {
        result.push_back(artwork);
    }
    return result;*/
}

void PradoEditorMobileInterface::sortArtworks(const SortCriteria &criteria)
{
    // Contributors: Taro Welches and Lucas Giebler
    // Input: const std::string& criteria
    // Purpose: To sort the displayed artwork by a certain criteria input by user
    // return: none
    std::vector<std::pair<std::string, Artwork>> entries(GlobalGallery.begin(), GlobalGallery.end());

    switch (criteria) {
    case Title:
		std::sort(entries.begin(), entries.end(),
			[](const auto& a, const auto& b)
			{ return a.second.title < b.second.title; });
        break;
    case Newest:
        std::sort(entries.begin(), entries.end(),
            [](const auto& a, const auto& b)
            { return a.second.year > b.second.year; });
        break;
    case Oldest:
        std::sort(entries.begin(), entries.end(),
            [](const auto& a, const auto& b)
            { return a.second.year < b.second.year; });
        break;
    case Artist:
        std::sort(entries.begin(), entries.end(),
            [](const auto& a, const auto& b)
            { return a.second.author < b.second.author; }); // alphabetical ig
        break;
	default:
        std::cerr << "Unknown sorting criteria: " << criteria << std::endl;
        break;
    }

    GlobalGallery.clear();
    for (const auto &[id, artwork] : entries)
    {
        GlobalGallery.push_back(artwork);
    }
}

SubtitleData PradoEditorMobileInterface::splitSubtitle(const std::string &work_subtitle)
{
    // Contributors : Sarah and Taro and Lucas
    // Purpose : split the given string subtitle description into a struct containing separate year, medium, and dimensions
    // Parameters : work_subtitle: the original subtitle field in paragraph form
    // Return Value: subtitle: a struct of the original subtitle field
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
        std::cerr << "No match found.\n";
    }

    /*size_t dotPos = work_subtitle.find('.');

    for (size_t i = 0; i < dotPos; i++)
    {
        if (isdigit(prefix[i]) && isdigit(prefix[i + 1]) &&
            isdigit(prefix[i + 2]) && isdigit(prefix[i + 3]))
        {
            subtitle.year = prefix.substr(i, 4);
            break;
        }
    }

    size_t medPos = work_subtitle.find(',');
    std::string medium = work_subtitle.substr(yrPos + 1, medPos - yrPos - 1);
    subtitle.medium = std::string(trim(medium));

    subtitle.dimensions = trim(work_subtitle.substr(medPos + 1));*/

    return subtitle;
}

cv::Mat getImage(const std::string &artworkId)
{

    std::cout << "make some sort of call to getimagecache and then error handle\n";
}
