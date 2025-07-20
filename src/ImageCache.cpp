#include "ImageCache.h"

void ImageCache::updateUsage(const std::string& artworkId) {
    if (!image_map.count(artworkId)) return;

    // Move the artworkId to the front of the list (most recently used)
    usage_list.remove(artworkId);
    usage_list.push_front(artworkId);
}

void ImageCache::addImage(const std::string& artworkId, const cv::Mat& image) {
    // If the cache is full, remove the least recently used image
    if (image_map.size() >= max_images) {
        std::string old_id = usage_list.back();
        usage_list.pop_back();
        image_map.erase(old_id);
    }

    // Insert the new image into the cache
    image_map[artworkId] = image.clone();  // Clone to avoid external modifications
    usage_list.remove(artworkId);
    usage_list.push_front(artworkId);
}

cv::Mat ImageCache::getCachedImage(const std::string& artworkId) const {
    auto it = image_map.find(artworkId);
    if (it != image_map.end()) {
        return it->second;  // Return the cached image
    }
    return cv::Mat();  // Return an empty Mat if not found
}
