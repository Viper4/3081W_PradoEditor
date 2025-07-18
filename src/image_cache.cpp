#include <image_cache.h>
#include <iostream>
#include <opencv2/opencv.hpp>

int ImageCache::maxImages = 50; // Maximum number of images to cache
std::list<std::string> ImageCache::usageList; // Most recently used will be at the front
std::unordered_map<std::string, std::list<std::string>::iterator> ImageCache::usageMap; // Maps artworkId to iterator in usageList for O(1) usage updates
std::unordered_map<std::string, cv::Mat> ImageCache::imageMap; // Mat is OpenCV's matrix object to represent an image

void ImageCache::printUsageList(const std::string& label) {
    // Contributors: Lucas Giebler
	// Purpose: Prints the usage list
	// Parameters: string label - The label to print before the usage list
	// Return Value: void
	// Limitations: 
    // -------------------
    std::cout << label;
    for (const auto& id : usageList) {
        std::cout << id << " ";
    }
    std::cout << std::endl;
}

void ImageCache::updateUsage(const std::string& artworkId) {
    // Contributors: Lucas Giebler
	// Purpose: Updates the usage of an image in the cache
	// Parameters: string artworkId - The id of the image to update
	// Return Value: void
	// Limitations: Fatal with asynchronous access since usageList and usageMap get out of sync
	// -------------------
    auto it = usageMap.find(artworkId);
    if (it != usageMap.end()) {
        // Move the existing entry to the front
        usageList.erase(it->second);
    }

    usageList.push_front(artworkId);
    usageMap[artworkId] = usageList.begin();
}

void ImageCache::addImage(const std::string& artworkId, const cv::Mat& image) {
    // Contributors: Lucas Giebler
    // Purpose: Adds a new image to the cache
    // Parameters: string artworkId - The id of the artwork
    //             cv::Mat image - The image to associate with the id
    // Return Value: void
    // Limitations: Does no checking of artworkId or image, so invalid ids or images will be cached
    // -------------------
    // If the id is already in the map, do nothing
    if (imageMap.count(artworkId) != 0) {
        return;
    }
    while (imageMap.size() >= maxImages) {
        std::string leastUsed = usageList.back();
        usageList.pop_back();
        usageMap.erase(leastUsed);
        imageMap.erase(leastUsed);
    }
    imageMap[artworkId] = image;
    usageList.push_front(artworkId);
    usageMap[artworkId] = usageList.begin();
}

cv::Mat ImageCache::getCachedImage(const std::string& artworkId) {
    // Contributors: Lucas Giebler
    // Purpose: Tries to retrieve an image from the cache using the artworkId
    // Parameters: string artworkId - The id of the artwork to retrieve its image
    // Return Value: cv::Mat
    // Limitations: Returns empty cv::Mat if the image is not found
    // -------------------
    if (imageMap.count(artworkId) == 0) {
        return cv::Mat();
    }
    //updateUsage(artworkId);
    return imageMap[artworkId];
}

QPixmap ImageCache::matToQPixmap(const cv::Mat& image) {
    // Contributors: https://forum.qt.io/topic/160407/convert-cv-mat-into-qpixmap
    // Purpose: Converts a cv::Mat to a QPixmap
    // Parameters: cv::Mat image - The image to convert
    // Return Value: QPixmap
    // Limitations: Only supports CV_8UC1 and CV_8UC3
    // -------------------
    // Check if image is empty
    if (image.empty()) {
        std::cout << "WARNING: Converting empty image to QPixmap" << std::endl;
        return QPixmap();
    }

    // Convert the cv::Mat to QImage and then to QPixmap
    QImage qimage;
    cv::Mat rgb;
    switch (image.type()) {
    case CV_8UC1: // Grayscale
        qimage = QImage(image.data, image.cols, image.rows, image.step, QImage::Format_Grayscale8);
        break;
    case CV_8UC3: // Color BGR
        // OpenCV uses BGR, but Qt expects RGB
        cv::cvtColor(image, rgb, cv::COLOR_BGR2RGB);
        qimage = QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
        break;
    default:
        std::cerr << "Unsupported image type for conversion: " << image.type() << std::endl;
        return QPixmap();
    }
    return QPixmap::fromImage(qimage.copy()); // copy to detach from OpenCV data
}
