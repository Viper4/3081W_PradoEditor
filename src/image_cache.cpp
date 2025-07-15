#include <include/image_cache.h>
#include <iostream>
#include <opencv2/opencv.hpp>

int ImageCache::maxImages = 50;
std::list<std::string> ImageCache::usageList; // Most recently used id will be at the front of the list
std::unordered_map<std::string, cv::Mat> ImageCache::imageMap; // Mat is OpenCV's matrix object to represent an image

void ImageCache::printUsageList(const std::string& label) {
    // Contributors: Lucas Giebler
	// Purpose: Prints the usage list
	// Parameters: string label
	// Return Value: void
    // -------------------
    std::cout << label;
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
    // -------------------
    
    //std::cout << "Updating usage of " << artworkId << std::endl;
    //ImageCache::printUsageList("Before update: ");

    ImageCache::usageList.remove(artworkId);
    ImageCache::usageList.push_front(artworkId);

    //ImageCache::printUsageList("After update: ");
}

void ImageCache::addImage(const std::string& artworkId, const cv::Mat& image) {
    // Contributors: Lucas Giebler
    // Purpose: Adds a new image to the cache
    // Parameters: string artworkId
    //             cv::Mat image
    // Return Value: void
    // -------------------
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
    // -------------------
    if (ImageCache::imageMap.find(artworkId) == ImageCache::imageMap.end()) {
        return cv::Mat();
    }
    ImageCache::updateUsage(artworkId);
    return ImageCache::imageMap[artworkId];
}

QPixmap ImageCache::matToQPixmap(const cv::Mat& image) {
    // Contributors: https://forum.qt.io/topic/160407/convert-cv-mat-into-qpixmap
    // Purpose: Converts a cv::Mat to a QPixmap
    // Parameters: cv::Mat image
    // Return Value: QPixmap
    // -------------------
    // Check if image is empty
    if (image.empty()) {
        std::cerr << "Empty image" << std::endl;
        return QPixmap();
    }

    // Convert the cv::Mat to QImage
    QImage qimage;
    QPixmap pixmap = QPixmap::fromImage(QImage((unsigned char*)image.data, image.cols, image.rows, QImage::Format_RGB888));

    return pixmap;
}
