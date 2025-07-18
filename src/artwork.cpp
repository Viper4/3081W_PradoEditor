#include <artwork.h>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <image_cache.h>
#include <managers.h>
#include <regex>
#include <image_scroll_gallery.h>

std::vector<Artwork> PradoEditorMobileInterface::artworkMapToVector(const std::unordered_map<std::string, Artwork>& artworkMap) {
    std::vector<Artwork> artworks;
    for (const auto& pair : artworkMap) {
        artworks.push_back(pair.second);
    }
	return artworks;
}

void PradoEditorMobileInterface::sortArtworks(const SortCriteria &criteria, std::vector<Artwork> &artworks)
{
    // Contributors: Taro Welches and Lucas Giebler
    // Purpose: To sort the displayed artwork by a certain criteria input by user
    // Parameters: const std::string& criteria
    // Return: void
    // Limitations: 
    switch (criteria) {
    case Title:
        std::sort(artworks.begin(), artworks.end(),
            [](const Artwork& a, const Artwork& b) {
                return a.metadata.at("work_title") < b.metadata.at("work_title");
            });
        break;
    case Newest:
        std::sort(artworks.begin(), artworks.end(),
            [](const Artwork& a, const Artwork& b) {
                return a.metadata.at("sheet_date") > b.metadata.at("sheet_date");
            });
        break;
    case Oldest:
        std::sort(artworks.begin(), artworks.end(),
            [](const Artwork& a, const Artwork& b) {
                return a.metadata.at("sheet_date") < b.metadata.at("sheet_date");
            });
        break;
    case Artist:
        std::sort(artworks.begin(), artworks.end(),
            [](const Artwork& a, const Artwork& b) {
                return a.metadata.at("author") < b.metadata.at("author");
            });
        break;
	default:
        std::cout << "Unknown sorting criteria: " << criteria << std::endl;
        break;
    }
}
