#ifndef IMAGE_CACHE_H
#define IMAGE_CACHE_H

#include <list>
#include <string>
#include <unordered_map>
#include <opencv2/core.hpp>  

class ImageCache {
private:
	static void printUsageList();
public:
    static int maxImages;
    static std::list<std::string> usageList; // Most recently used id will be at the front of the list
    static std::unordered_map<std::string, cv::Mat> imageMap; // Mat is OpenCV's matrix object to represent an image

    static void updateUsage(const std::string& artworkId);
    static void addImage(const std::string& artworkId, const cv::Mat& image);
    static cv::Mat getCachedImage(const std::string& artworkId);
};

#endif // IMAGE_CACHE_H
