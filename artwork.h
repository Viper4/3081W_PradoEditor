#ifndef PRADO_EDITOR_INTERFACE_H
#define PRADO_EDITOR_INTERFACE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <opencv2/core.hpp>  
#include <image_cache.h>

// -------------------
// Artwork Data Struct
// Contributors: Taro Welches
// Purpose: provide structure for attributes for each piece of artwork 
// Input/Output: None
// -------------------
struct Artwork {
    std::string id = "";
    std::string workTitle = "";
    std::string workImageUrl = "";
    std::string originalWorkImageUrl = "";
    std::string author = "";
    std::string authorBio = "";
    std::string authorId = "";
    std::string authorUrl = "";
    std::string workSubtitle = "";
    std::string year = "";
    std::string workDescription = "";
    std::string workExposed = "";
    int x = 0;
    int y = 0;
    int originalX = 0;
    int originalY = 0;
};
struct SubtitleData
{
    std::string year;
    std::string medium;
    std::string dimensions;
};

// -------------------
// Prado Interface
// Contributors: Taro Welches
// Purpose: Provide a layout for each artwork.cpp function to be implemented in artwork.cpp
// Input/Output: None
// -------------------
class PradoEditorMobileInterface {
private:
    std::string trim(const std::string &str)
    {
        // Contributors : Sarah
        // Purpose : inline function to trim the substrs for the subtitle struct
        // Parameters: str: a string of the the subtitle field to be split
        // Return Value : a substr of the original input (string)
        size_t start = str.find_first_not_of(' ');
        size_t end = str.find_last_not_of(' ');

        if (start == std::string::npos || end == std::string::npos)
            return "";

        return str.substr(start, end - start + 1);
    }
public:
    PradoEditorMobileInterface() = default;

    cv::Mat getArtworkGallery();
    void sortArtworks(const std::string& criteria){
        {
        if (criteria == "Newest")
        {
            std::sort(gallery.begin(), gallery.end(), [](const Artwork &a, const Artwork &b)
                      { return a.year > b.year; });
        }
        else if (criteria == "Oldest")
        {
            std::sort(gallery.begin(), gallery.end(), [](const Artwork &a, const Artwork &b)
                      { return a.year < b.year; });
        }
        else if (criteria == "Artist")
        {
            std::sort(gallery.begin(), gallery.end(), [](const Artwork &a, const Artwork &b)
                      { return a.author < b.author; });
        }
        else
        {
            std::cerr << "Unknown sorting criteria: " << criteria << std::endl;
        }
    }
    //void resetImage(const std::string& artworkId); Moved to EditorManager.cpp
    Artwork getArtworkDescription(const std::string& artworkId){
        {
        // Contributors : Sarah
        // Purpose : retrieve an artwork description
        // Parameters : artworkID: a string that identifies a unique artpiece (str)
        // Return Value: either the artwork description or its availability status (str)
        Artwork *art = findArtworkById(artworkId);
        if (!art || art->work_description.empty())
        {
            return "Description not available.";
        }
        return art->work_description;
    };
    //Artwork applyFilterToImage(const std::string& artworkId); Moved to EditorManager.cpp
    SubtitleData splitSubtitle(const std::string& workSubtitle);
    //void editImage(const std::string& artworkId); Moved to EditorManager.cpp
    //void cropImage(const std::string& artworkId, int x, int y, int width, int height); Moved to EditorManager.cpp
    //void rotateImage(const std::string& artworkId, double angle); Moved to EditorManager.cpp
    cv::Mat getImage(const std::string& artworkId);
};

#endif // PRADO_EDITOR_INTERFACE_H
