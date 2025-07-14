#ifndef PRADO_EDITOR_INTERFACE_H
#define PRADO_EDITOR_INTERFACE_H

#include <string.h>
#include <unordered_map>
#include <vector>
#include <opencv2/core.hpp>  

// -------------------
// Artwork Data Struct
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
// Global Data Stores
// -------------------
inline std::unordered_map<std::string, Artwork> GlobalGallery;

struct ImageCache {
    static inline std::unordered_map<std::string, cv::Mat> image_map;
};

// -------------------
// Interface Class
// -------------------
class PradoEditorMobileInterface {
private:
    cv::Mat getCachedImage(const std::string& artworkId);
    Artwork* findArtworkById(const std::string& artworkId);

public:
    PradoEditorMobileInterface() = default;

    std::vector<Artwork> getArtworkGallery();
    void sortArtworks(const std::string& criteria);
    void resetImage(const std::string& artworkId);
    Artwork getArtworkDescription(const std::string& artworkId);
    Artwork applyFilterToImage(const std::string& artworkId);
    void splitSubtitle(const std::string& work_subtitle);
    void editImage(const std::string& artworkId);
    void cropImage(const std::string& artworkId, int x, int y, int width, int height);
    void rotateImage(const std::string& artworkId, double angle);
    void getImage(const std::string& artworkId);
};

#endif // PRADO_EDITOR_INTERFACE_H
