#include <managers.h>
#include <stdexcept>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <artwork.h>
#include <opencv2/core/types.hpp>      // for cv::Point2f and cv::RotatedRect
#include <opencv2/imgproc.hpp>         // for getRotationMatrix2D, warpAffine
#include <image_cache.h>
#include <curl/curl.h>
#include <image_scroll_gallery.h>
#include <curl/easy.h>

Artwork ArtworkManager::getArtworkByID(const std::string& artworkId)
{
    // Contributors: Huiwen Jia
    // Purpose: Initialize a custom console UI for debugging
    // Parameters: 
    // Return Value: void
    // Limitations: 
    // -------------------
    if (ImageScrollGallery::GlobalGallery.find(artworkId) != ImageScrollGallery::GlobalGallery.end()) {
		return ImageScrollGallery::GlobalGallery.at(artworkId);
    }

    std::cout << "Artwork ID '" << artworkId << "' not found." << std::endl;
    return Artwork();
}

size_t ArtworkManager::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::vector<uchar>* buffer = static_cast<std::vector<uchar>*>(userp);
    size_t totalSize = size * nmemb;
    uchar* data = static_cast<uchar*>(contents);
    buffer->insert(buffer->end(), data, data + totalSize);
    return totalSize;
}

cv::Mat ArtworkManager::getImage(const std::string& artworkId)
{
	// Contributors: Lucas Giebler
	// Input: const std::string& artworkId
	// Purpose: To get the actual cv::Mat image of an artwork by ID
	// Return: cv::Mat image
	// Limitations: 
	// -------------------
    // First, try to retrieve the image from the cache
    cv::Mat cachedResult = ImageCache::getCachedImage(artworkId);
    if (!cachedResult.empty()) {
        return cachedResult;
    }
    // If the image is not in the cache, send curl request to get the image
    if (ImageScrollGallery::GlobalGallery.find(artworkId) != ImageScrollGallery::GlobalGallery.end()) {
        CURL* curl = curl_easy_init();
        std::vector<uchar> image_data;
        cv::Mat image = cv::Mat();

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, ImageScrollGallery::GlobalGallery.at(artworkId).metadata.at("image_url").c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &image_data);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  // follow redirects

            CURLcode result = curl_easy_perform(curl);
            if (result != CURLE_OK) {
                std::cout << "curl_easy_perform() failed " << ImageScrollGallery::GlobalGallery.at(artworkId).metadata.at("image_url") << ": " << curl_easy_strerror(result) << std::endl;
            }
            else {
                image = cv::imdecode(image_data, cv::IMREAD_COLOR);
                if (image.empty()) {
                    std::cout << "Failed to decode image " << ImageScrollGallery::GlobalGallery.at(artworkId).metadata.at("image_url") << "." << std::endl;
                }
            }

            curl_easy_cleanup(curl);
        }

		// Add the image to the cache
        ImageCache::addImage(artworkId, image);
        return image;
    }

	// If the image is not found in the gallery, return an empty cv::Mat
    std::cout << "Artwork ID not found: " << artworkId << std::endl;
    return cv::Mat();
}

cv::Mat ArtworkManager::applyFilter(const cv::Mat& image, const FilterType filterType)
{
    // Contributors: Huiwen Jia
    // Input: const cv::Mat& image, const std::map<std::string, int>& params
    // Purpose: To apply a selected filter (grayscale, invert, blur) to the image
    // Return: Filtered cv::Mat image
    // Limitations: 
    // -------------------	
    if (image.empty()) {
        std::cout << "Cannot apply filter to empty image." << std::endl;
		return cv::Mat();
    }

    cv::Mat result;
    switch (filterType)
    {
    case GRAYSCALE:
        cv::cvtColor(image, result, cv::COLOR_BGR2GRAY);
        break;
    case INVERT:
        cv::bitwise_not(image, result);
        break;
    case BLUR:
        cv::blur(image, result, cv::Size(5, 5));
        break;
    case HSV:
		cv::cvtColor(image, result, cv::COLOR_BGR2HSV);
        break;
    default:
        throw std::invalid_argument("Unknown filter type.");
    }
    return result;
}
cv::Mat ArtworkManager::cropImage(const cv::Mat& image, const int x, const int y, const int width, const int height)
{
    // Contributors: Huiwen Jia
    // Input: const cv::Mat& image, const std::map<std::string, int>& params
    // Purpose: To crop the image at specified position and size
    // Return: Cropped cv::Mat image
    // Limitations: 
    // -------------------	
    if (image.empty()) {
		std::cout << "Cannot crop empty image." << std::endl;
        return cv::Mat();
    }

    if (x < 0 || y < 0 || width <= 0 || height <= 0 ||
        x + width > image.cols || y + height > image.rows)
    {
        std::cout << "WARNING: Crop area is out of bounds." << std::endl;
        return image.clone();
    }
    return image(cv::Rect(x, y, width, height)).clone();
}


cv::Mat ArtworkManager::cropImageCentered(const cv::Mat& image, const int width, const int height) {
    // Contributors: Lucas Giebler
    // Input: const cv::Mat& image
    //        const int width
    //        const int height
    // Purpose: To crop the image around the center to the specified width and height
    // Return: cv::Mat image
    // Limitations: 
    // -------------------
    int cropWidth = std::min({ width, image.cols});
    int cropHeight = std::min({ height, image.rows });
    int x = (image.cols - cropWidth) / 2;
    int y = (image.rows - cropHeight) / 2;

    // Ensure ROI is within bounds
    cv::Rect roi(x, y, cropWidth, cropHeight);

    return image(roi);
}

Artwork ArtworkManager::editImage(const std::string& artworkId, const std::map<std::string, int>& params)
{
    // Contributors: Huiwen Jia and Lucas Giebler
    // Input: const std::string& artworkId, const std::map<std::string, int>& params
    // Purpose: To perform both cropping and filtering operations on the image
    // Return: Artwork object with the edited image
    // Limitations: 
    // -------------------	
    Artwork original = getArtworkByID(artworkId);

    cv::Mat image = cv::imread("images/" + artworkId + ".jpg");
    if (image.empty()) {
        throw std::runtime_error("Failed to load image for: " + artworkId);
    }

    if (params.count("width") && params.count("height"))
    {
        image = cropImage(image, params.at("x"), params.at("y"), params.at("width"), params.at("height"));
    }
    if (params.count("type"))
    {
        image = applyFilter(image, static_cast<FilterType>(params.at("type")));
    }

    Artwork edited = original;
    //edited.image = image;
    return edited;

}

cv::Mat ArtworkManager::rotateImage(const cv::Mat& image, double angle)
{
    // Contributors : Sarah and Taro
    // Purpose: rotate the image clockwise by the requested angle
    // Parameters: artworkID - a string that identifies a unique artpiece (str)
    //             angle - the angle in degrees to rotate the image by (double)
    // Return Value: new_image: the newly rotated version of the image (Matrix)
    // Limitations: 
    // -------------------	
    if (image.empty()) {
        std::cout << "Cannot rotate empty image." << std::endl;
		return cv::Mat();
    }

    cv::Point2f center(image.cols / 2.0F, image.rows / 2.0F);
    // convert the angle to radians
    //angle = angle * CV_PI / 180.0;

    // get the rotation matrix
    cv::Mat rot = getRotationMatrix2D(center, angle, 1.0);

    // bounding box to prevent cropping
    cv::Rect2f bound_box = cv::RotatedRect(cv::Point2f(), image.size(), angle).boundingRect2f();

    // adjust the transformation matrix based on bounds
    rot.at<double>(0, 2) += bound_box.width / 2.0 - image.cols / 2.0;
    rot.at<double>(1, 2) += bound_box.height / 2.0 - image.rows / 2.0;

    // rotate and save the new image
    cv::Mat new_image;
    cv::warpAffine(image, new_image, rot, bound_box.size());

    // FINISH: UPDATE IMAGE;
    return new_image;
}


void ArtworkManager::resetImage(const std::string& artworkId)
{
    // Contributors : Sarah and Taro
    // Purpose : rotate the image clockwise by the requested angle
    // Parameters :
    	// artworkID: a string that identifies a unique artpiece (str)
    	// angle: the angle in degrees to rotate the image by (double)
    // Return Value: new_image: the newly rotated version of the image (Matrix)
    // Limitations: 
    // -------------------
    try {
        // Get the artwork metadata
        Artwork art = getArtworkByID(artworkId);

        // Load original image from disk (e.g., art001_original.jpg)
        cv::Mat original = cv::imread("images/" + artworkId + "_original.jpg");
        if (original.empty()) {
            std::cerr << "Error: original image file not found for artwork ID: " << artworkId << "\n";
            return;
        }

        // Save restored image to a new file
        std::string outputPath = "images/restored_" + artworkId + ".jpg";
        if (!cv::imwrite(outputPath, original)) {
            std::cerr << "Error: failed to write restored image to disk.\n";
        }
        else {
            std::cout << "Restored image saved to: " << outputPath << "\n";
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Exception in resetImage: " << e.what() << "\n";
    }
}
