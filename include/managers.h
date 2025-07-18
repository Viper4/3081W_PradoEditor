
#ifndef ARTWORK_MANAGER_H
#define ARTWORK_MANAGER_H

#include <string>
#include <map>
#include <opencv2/core.hpp>
#include "artwork.h" // Assumes you have an Artwork class with id, image, etc.

class ArtworkManager {
private:
public:
    enum FilterType {
        GRAYSCALE,
        INVERT,
        BLUR,
        HSV
    };

    static Artwork getArtworkByID(const std::string& artworkId);
    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static cv::Mat getImage(const std::string& artworkId);
    static cv::Mat applyFilter(const cv::Mat& image, const FilterType filterType);
    static cv::Mat cropImage(const cv::Mat& image, const int x, const int y, const int width, const int height);
    static cv::Mat cropImageCentered(const cv::Mat& image, const int width, const int height);
    static Artwork editImage(const std::string& artworkId, const std::map<std::string, int>& params);
    static cv::Mat rotateImage(const cv::Mat& image, double angle);
    static void resetImage(const std::string& artworkId);
};

#endif // ARTWORK_MANAGER_H
