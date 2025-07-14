#ifndef PRADO_EDITOR_INTERFACE_H
#define PRADO_EDITOR_INTERFACE_H

#include <string.h>
#include <unordered_map>
#include <vector>
#include <opencv2/core.hpp>  

// -------------------
// Artwork Data Struct
// Contributors: Taro Welches
// Purpose: provide attributes for each piece of artwork
// Input/Output: None
// -------------------
struct Artwork {
    std::string id = "";
    std::string work_title = "";
    std::string work_image_url = "";
    std::string original_work_image_url = "";
    std::string author = "";
    std::string author_bio = "";
    std::string author_id = "";
    std::string author_url = "";
    std::string work_subtitle = "";
    std::string year = "";
    std::string work_description = "";
    std::string work_exposed = "";
    int x = 0;
    int y = 0;
    int original_x = 0;
    int original_y = 0;
};


// -------------------
// Interface Class
// Contributors: Taro Welches
// Purpose: Provide a layout for each artwork.cpp function to be implemented in artwork.cpp
// Input/Output: None
// -------------------
class PradoEditorMobileInterface {
public:
    PradoEditorMobileInterface() = default;

    cv::Mat getArtworkGallery();
    void sortArtworks(const std::string& criteria);
    void resetImage(const std::string& artworkId);
    Artwork getArtworkDescription(const std::string& artworkId);
    Artwork applyFilterToImage(const std::string& artworkId);
    void splitSubtitle(const std::string& work_subtitle);
    void editImage(const std::string& artworkId);
    void cropImage(const std::string& artworkId, int x, int y, int width, int height);
    void rotateImage(const std::string& artworkId, double angle);
    cv::Mat getImage(const std::string& artworkId);
};

#endif // PRADO_EDITOR_INTERFACE_H
