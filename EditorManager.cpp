#include "ArtworkManager.h"
#include "ImageCache.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdexcept>

extern ImageCache imageCache;


Artwork ArtworkManager::getArtworkByID(const std::string& artworkId) {
    // Contributors: Huiwen Jia
    // Input: const std::string& artworkId
    // Purpose: To retrieve the artwork from cache or disk based on ID
    // Return: Artwork object containing the image and ID
    cv::Mat img = imageCache.getCachedImage(artworkId);
    if (img.empty()) {
        img = cv::imread("images/" + artworkId + ".jpg");
        if (img.empty()) {
            throw std::runtime_error("Image not found for artwork ID: " + artworkId);
        }
        imageCache.addImage(artworkId, img);
    }
    imageCache.updateUsage(artworkId);
    return Artwork(artworkId, img);
}

cv::Mat ArtworkManager::applyFilter(const cv::Mat& image, const std::map<std::string, int>& params) {
    // Contributors: Huiwen Jia
    // Input: const cv::Mat& image, const std::map<std::string, int>& params
    // Purpose: To apply a selected filter (grayscale, invert, blur) to the image
    // Return: Filtered cv::Mat image  
    if (!params.count("type")) return image.clone();

    int type = params.at("type");
    cv::Mat result;
    switch (type) {
    case 1:
        cv::cvtColor(image, result, cv::COLOR_BGR2GRAY);
        break;
    case 2:
        cv::bitwise_not(image, result);
        break;
    case 3:
        cv::blur(image, result, cv::Size(5, 5));
        break;
    default:
        throw std::invalid_argument("Unknown filter type.");
    }
    return result;
}

cv::Mat ArtworkManager::cropImage(const cv::Mat& image, const std::map<std::string, int>& params) {
    // Contributors: Huiwen Jia
    // Input: const cv::Mat& image, const std::map<std::string, int>& params
    // Purpose: To crop the image at specified position and size
    // Return: Cropped cv::Mat image

    int x = params.at("x");
    int y = params.at("y");
    int width = params.at("width");
    int height = params.at("height");

    if (x < 0 || y < 0 || width <= 0 || height <= 0 ||
        x + width > image.cols || y + height > image.rows) {
        throw std::out_of_range("Crop area is out of bounds.");
    }
    return image(cv::Rect(x, y, width, height)).clone();
}

Artwork ArtworkManager::editImage(const std::string& artworkId, const std::map<std::string, int>& params) {
    // Contributors: Huiwen Jia
    // Input: const std::string& artworkId, const std::map<std::string, int>& params
    // Purpose: To perform both cropping and filtering operations on the image
    // Return: Artwork object with the edited image

    Artwork original = getArtworkByID(artworkId);
    cv::Mat image = original.getImage();

    if (params.count("width") && params.count("height")) {
        image = cropImage(image, params);
    }
    if (params.count("type")) {
        image = applyFilter(image, params);
    }

    return Artwork(artworkId, image);
}
