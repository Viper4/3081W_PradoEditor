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

    std::cout << "getArtworkByID : Artwork ID '" << artworkId << "' not found." << std::endl;
    return Artwork();
}

size_t ArtworkManager::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    // Contributors: Lucas Giebler
	// Purpose: To write the image data to a buffer (used for cURL downloading images)
    // Parameters: void* contents - contents of image
    //             size_t size - size of contents
    //             size_t nmemb - number of elements
    //             void* userp - user pointer
	// Return: size_t totalSize
	// Limitations: 
	// -------------------
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
    if (ImageScrollGallery::GlobalGallery.count(artworkId) != 0) {
        CURL* curl = curl_easy_init();
        std::vector<uchar> image_data;
        cv::Mat image = cv::Mat();

        if (curl) {
            // Set up the cURL request
            curl_easy_setopt(curl, CURLOPT_URL, ImageScrollGallery::GlobalGallery.at(artworkId).metadata.at("image_url").c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &image_data);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  // Follow redirects

            CURLcode result = curl_easy_perform(curl); // Perform the request
            if (result != CURLE_OK) {
                std::cout << "getImage : curl_easy_perform() failed " << ImageScrollGallery::GlobalGallery.at(artworkId).metadata.at("image_url") << ": " << curl_easy_strerror(result) << std::endl;
            }
            else {
                image = cv::imdecode(image_data, cv::IMREAD_COLOR);
                if (image.empty()) {
                    std::cout << "getImage : Failed to decode image " << ImageScrollGallery::GlobalGallery.at(artworkId).metadata.at("image_url") << "." << std::endl;
                }
            }

            curl_easy_cleanup(curl); // Cleanup
        }

		// Add the image to the cache
        ImageCache::addImage(artworkId, image);
        return image;
    }

	// If the image is not found in the gallery, return an empty cv::Mat
    std::cout << "getImage : Artwork ID not found: " << artworkId << std::endl;
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
        std::cout << "applyFilter : Cannot apply filter to empty image." << std::endl;
		return cv::Mat();
    }

    cv::Mat result;
    std::vector<cv::Mat> hsvChannels;
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
    case SATURATE:
        // Convert to HSV
        cv::cvtColor(image, result, cv::COLOR_BGR2HSV);

        // Split channels
        cv::split(result, hsvChannels);

        // Increase saturation (S channel)
        // 1.0 = no change, 1.5 = saturate by 50%
        hsvChannels[1].convertTo(hsvChannels[1], -1, 1.5, 0);

        // Clamp values to [0, 255]
        cv::threshold(hsvChannels[1], hsvChannels[1], 255, 255, cv::THRESH_TRUNC);

        // Merge and convert back to BGR
        cv::merge(hsvChannels, result);
        cv::cvtColor(result, result, cv::COLOR_HSV2BGR);
        break;
    default:
        std::cout << "applyFilter : Unknown filter type." << std::endl;
        return image;
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
		std::cout << "cropImage : Cannot crop empty image." << std::endl;
        return cv::Mat();
    }

    if (x < 0 || y < 0 || width <= 0 || height <= 0 ||
        x + width > image.cols || y + height > image.rows)
    {
        std::cout << "cropImage : Crop area is out of bounds." << std::endl;
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

    return image(cv::Rect(x, y, cropWidth, cropHeight)).clone();
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
		std::cout << "editImage : Failed to load image for: " << artworkId << std::endl;
        return original;
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
        std::cout << "rotateImage : Cannot rotate empty image." << std::endl;
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
    // Purpose: resets the image to its original state without any changes/manipulations
    // Parameters: artworkID: a string that identifies a unique artpiece (str)
    // Return Value: void
    // Limitations: 
    // -------------------
    try {
        // Get the artwork metadata
        Artwork art = getArtworkByID(artworkId);

        // Load original image from disk (e.g., art001_original.jpg)
        cv::Mat original = cv::imread("images/" + artworkId + "_original.jpg");
        if (original.empty()) {
            std::cout << "resetImage : original image file not found for artwork ID: " << artworkId << "\n";
            return;
        }

        // Save restored image to a new file
        std::string outputPath = "images/restored_" + artworkId + ".jpg";
        if (!cv::imwrite(outputPath, original)) {
            std::cout << "resetImage : failed to write restored image to disk.\n";
        }
        else {
            std::cout << "resetImage : Restored image saved to: " << outputPath << "\n";
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Exception in resetImage: " << e.what() << "\n";
    }
}

QPixmap ArtworkManager::matToPixmap(const cv::Mat& image) {
    if (image.empty()) {
        std::cerr << "matToPixmap: Converting empty image to QPixmap" << std::endl;
        return QPixmap();
    }

    QImage qimage;
    cv::Mat converted;

    switch (image.type()) {
    case CV_8UC1: // Grayscale
        qimage = QImage(image.data, image.cols, image.rows, image.step, QImage::Format_Grayscale8);
        break;
    case CV_8UC3: // BGR color
        cv::cvtColor(image, converted, cv::COLOR_BGR2RGB);
        qimage = QImage(converted.data, converted.cols, converted.rows, converted.step, QImage::Format_RGB888);
        break;
    case CV_8UC4: // BGRA color (with alpha)
        cv::cvtColor(image, converted, cv::COLOR_BGRA2RGBA);
        qimage = QImage(converted.data, converted.cols, converted.rows, converted.step, QImage::Format_RGBA8888);
        break;
    default:
        std::cerr << "matToPixmap: Unsupported cv::Mat type for conversion: " << image.type() << std::endl;
        return QPixmap();
    }

    return QPixmap::fromImage(qimage.copy()); // .copy() to detach from OpenCV memory
}

cv::Mat ArtworkManager::pixmapToMat(const QPixmap& pixmap) {
    // Contributors: Lucas Giebler
    // Purpose: Converts a QPixmap to a cv::Mat
    // Parameters: QPixmap pixmap - The pixmap to convert
    // Return Value: cv::Mat
    // Limitations: Only supports Format_Grayscale8 and Format_RGB888
    // -------------------
	// Convert the QPixmap to QImage and then to cv::Mat
    QImage qimage = pixmap.toImage();

	cv::Mat image;
    switch (qimage.format()) {
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied: {
        // 4-channel image: BGRA
        cv::Mat temp(qimage.height(), qimage.width(), CV_8UC4, const_cast<uchar*>(qimage.bits()), qimage.bytesPerLine());
        cv::cvtColor(temp, image, cv::COLOR_BGRA2BGR); // Convert to BGR
        break;
    }
    case QImage::Format_RGB888: {
        // 3-channel image: RGB
        QImage swapped = qimage.rgbSwapped(); // Convert to BGR
        image = cv::Mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
        break;
    }
    case QImage::Format_Grayscale8: {
        image = cv::Mat(qimage.height(), qimage.width(), CV_8UC1, const_cast<uchar*>(qimage.bits()), qimage.bytesPerLine()).clone();
        break;
    }
    case QImage::Format_Indexed8: {
        // Sometimes used for grayscale
        image = cv::Mat(qimage.height(), qimage.width(), CV_8UC1, const_cast<uchar*>(qimage.bits()), qimage.bytesPerLine()).clone();
        break;
    }
    default:
        std::cerr << "qimageToMat: Unsupported QImage format: " << qimage.format() << std::endl;
        return cv::Mat(); // Return empty image
    }

    return image;
}
