#ifndef ARTWORK_MANAGER_H
#define ARTWORK_MANAGER_H

#include <string>
#include <map>
#include <opencv2/core.hpp>
#include "artwork.h" // Assumes you have an Artwork class with id, image, etc.

class ArtworkManager {
public:
    ArtworkManager() = default;

    /**
     * @brief Retrieve an artwork by ID from cache or disk.
     * @param artworkId Unique identifier for the artwork.
     * @return Artwork object.
     */
    Artwork getArtworkByID(const std::string& artworkId);

    cv::Mat applyFilter(const cv::Mat& image, const std::map<std::string, int>& params);

    cv::Mat cropImage(const cv::Mat& image, const std::map<std::string, int>& params);

    Artwork editImage(const std::string& artworkId, const std::map<std::string, int>& params);

    cv::Mat rotateImage(const std::string& artworkId, double angle);
};

class EditorManager {
public:
    EditorManager() = default;
    void resetImage(const std::string& artworkId);
};

#endif // ARTWORK_MANAGER_H
