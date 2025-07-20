#pragma once
#include <unordered_map>
#include <list>
#include <string>
#include <opencv2/opencv.hpp>

class ImageCache {
public:
    explicit ImageCache(size_t max_images = 50)
        : max_images(max_images) {}

    // Moves artworkId to the front of the usage list (most recently used)
    void updateUsage(const std::string& artworkId);

    // Adds an image to the cache, evicting the least recently used if full
    void addImage(const std::string& artworkId, const cv::Mat& image);

    // Retrieves the cached image (or an empty cv::Mat if not found)
    cv::Mat getCachedImage(const std::string& artworkId) const;

private:
    size_t max_images;
    mutable std::unordered_map<std::string, cv::Mat> image_map;
    mutable std::list<std::string> usage_list;  // LRU order (front = most recent)
};
