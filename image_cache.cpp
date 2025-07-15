#include <image_cache.h>
#include <opencv2/imgcodecs.hpp>
#include <iostream>

int ImageCache::maxImages = 50;
std::list<std::string> ImageCache::usageList; // Most recently used id will be at the front of the list
std::unordered_map<std::string, cv::Mat> ImageCache::imageMap; // Mat is OpenCV's matrix object to represent an image

void ImageCache::printUsageList() {
    std::cout << "Usage list: ";
    for (const auto& id : ImageCache::usageList) {
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

    ImageCache::usageList.remove(artworkId);
    ImageCache::usageList.push_front(artworkId);

    ImageCache::printUsageList();
}

void ImageCache::addImage(const std::string& artworkId, const cv::Mat& image) {
    // Contributors: Lucas Giebler
    // Purpose: Adds a new image to the cache
    // Parameters: string artworkId
    //             cv::Mat image
    // Return Value: void

    std::cout << "Add image " << artworkId << " with image size " << image.size << std::endl;

    if (ImageCache::imageMap.size() >= ImageCache::maxImages && ImageCache::imageMap.find(artworkId) != ImageCache::imageMap.end()) {
        ImageCache::usageList.pop_back();
    }
    ImageCache::imageMap[artworkId] = image;
    ImageCache::updateUsage(artworkId);
}

cv::Mat ImageCache::getCachedImage(const std::string& artworkId) {
    // Contributors: Lucas Giebler
    // Purpose: Tries to retrieve an image from the cache using the artworkId
    // Parameters: string artworkId
    // Return Value: cv::Mat

	std::cout << "Get image " << artworkId << std::endl;

    if (ImageCache::imageMap.find(artworkId) == ImageCache::imageMap.end()) {
        return cv::Mat();
    }
    return ImageCache::imageMap[artworkId];
}
