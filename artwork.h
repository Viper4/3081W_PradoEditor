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
    std::string trim(const std::string &str);
public:
    PradoEditorMobileInterface() = default;

    cv::Mat getArtworkGallery();
    void sortArtworks(const std::string& criteria);
    //void resetImage(const std::string& artworkId); Moved to EditorManager.cpp
    Artwork getArtworkDescription(const std::string& artworkId);
    //Artwork applyFilterToImage(const std::string& artworkId); Moved to EditorManager.cpp
    SubtitleData splitSubtitle(const std::string& workSubtitle);
    //void editImage(const std::string& artworkId); Moved to EditorManager.cpp
    //void cropImage(const std::string& artworkId, int x, int y, int width, int height); Moved to EditorManager.cpp
    //void rotateImage(const std::string& artworkId, double angle); Moved to EditorManager.cpp
    cv::Mat getImage(const std::string& artworkId);
};

#endif // PRADO_EDITOR_INTERFACE_H
