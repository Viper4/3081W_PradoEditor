#ifndef IMAGE_CACHE_H
#define IMAGE_CACHE_H

#include <list>
#include <string>
#include <unordered_map>
#include <opencv2/core.hpp>
#include <QPixmap>

class ImageCache {
private:
    static std::list<std::string> usageList;
    static std::unordered_map<std::string, std::list<std::string>::iterator> usageMap;
    static std::unordered_map<std::string, cv::Mat> imageMap;

	static void printUsageList(const std::string& label);
public:
    static int maxImages;

    static void updateUsage(const std::string& artworkId);
    static void addImage(const std::string& artworkId, const cv::Mat& image);
    static cv::Mat getCachedImage(const std::string& artworkId);
    static QPixmap matToQPixmap(const cv::Mat& mat);
};

#endif // IMAGE_CACHE_H
