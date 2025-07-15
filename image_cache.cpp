#include <image_cache.h>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

int ImageCache::max_images = 50;
std::list<std::string> ImageCache::usage_list; // Most recently used id will be at the front of the list
std::unordered_map<std::string, cv::Mat> ImageCache::image_map; // Mat is OpenCV's matrix object to represent an image

void ImageCache::printUsageList() {
    std::cout << "Usage list: ";
    for (const auto& id : ImageCache::usage_list) {
        std::cout << id << " ";
    }
    std::cout << std::endl;
}

void ImageCache::updateUsage(const std::string& artworkId) {
    // Contributors: Lucas Giebler
    // Purpose: Updates the usage of the artworkId in the linked list
    // Parameters: string artworkId
    // Return Value: void

    std::cout << "Updating usage of " << artworkId << std::endl;
    ImageCache::printUsageList();

    ImageCache::usage_list.remove(artworkId);
    if (ImageCache::image_map.find(artworkId) == ImageCache::image_map.end()) {
        
    }
    ImageCache::usage_list.push_front(artworkId);

    ImageCache::printUsageList();
}

void ImageCache::addImage(const std::string& artworkId, const cv::Mat& image) {
    // Contributors: Lucas Giebler
    // Purpose: Adds a new image to the cache
    // Parameters: string artworkId
    //             cv::Mat image
    // Return Value: void

    std::cout << "Add image " << artworkId << " with image size " << image.size << std::endl;

    if (ImageCache::image_map.size() >= ImageCache::max_images && ImageCache::image_map.find(artworkId) != ImageCache::image_map.end()) {
        ImageCache::usage_list.pop_back();
    }
    ImageCache::image_map[artworkId] = image;
    ImageCache::updateUsage(artworkId);
}

cv::Mat ImageCache::getCachedImage(const std::string& artworkId) {
    // Contributors: Lucas Giebler
    // Purpose: Tries to retrieve an image from the cache using the artworkId
    // Parameters: string artworkId
    // Return Value: cv::Mat

	std::cout << "Get image " << artworkId << std::endl;

    if (ImageCache::image_map.find(artworkId) == ImageCache::image_map.end()) {
        return cv::Mat();
    }
    return ImageCache::image_map[artworkId];
}
