#include "EditorManager.h"
#include <stdexcept>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "artwork.h"
#include <opencv2/core/types.hpp>      // for cv::Point2f and cv::RotatedRect
#include <opencv2/imgproc.hpp>         // for getRotationMatrix2D, warpAffine


Artwork ArtworkManager::getArtworkByID(const std::string &artworkId)
{
    // Contributors: Huiwen Jia
    // Loop over GlobalGallery to find the artwork by ID
    for (const auto& art : GlobalGallery) {
        if (art.id == artworkId) {
            // Try to load the image from disk
            cv::Mat img = cv::imread("images/" + artworkId + ".jpg");
            if (img.empty()) {
                throw std::runtime_error("Image file not found for artwork ID: " + artworkId);
            }

            // Construct and return artwork object with image
            Artwork result = art;
            result.image = img;  // Assuming you added cv::Mat image; field in Artwork
            return result;
        }
    }

    throw std::runtime_error("Artwork ID not found: " + artworkId);
}

cv::Mat ArtworkManager::applyFilter(const cv::Mat &image, const std::map<std::string, int> &params)
{
    // Contributors: Huiwen Jia
    // Input: const cv::Mat& image, const std::map<std::string, int>& params
    // Purpose: To apply a selected filter (grayscale, invert, blur) to the image
    // Return: Filtered cv::Mat image
    if (!params.count("type"))
        return image.clone();

    int type = params.at("type");
    cv::Mat result;
    switch (type)
    {
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

cv::Mat ArtworkManager::cropImage(const cv::Mat &image, const std::map<std::string, int> &params)
{
    // Contributors: Huiwen Jia
    // Input: const cv::Mat& image, const std::map<std::string, int>& params
    // Purpose: To crop the image at specified position and size
    // Return: Cropped cv::Mat image

    int x = params.at("x");
    int y = params.at("y");
    int width = params.at("width");
    int height = params.at("height");

    if (x < 0 || y < 0 || width <= 0 || height <= 0 ||
        x + width > image.cols || y + height > image.rows)
    {
        throw std::out_of_range("Crop area is out of bounds.");
    }
    return image(cv::Rect(x, y, width, height)).clone();
}

Artwork ArtworkManager::editImage(const std::string &artworkId, const std::map<std::string, int> &params)
{
    // Contributors: Huiwen Jia and Lucas Giebler
    // Input: const std::string& artworkId, const std::map<std::string, int>& params
    // Purpose: To perform both cropping and filtering operations on the image
    // Return: Artwork object with the edited image

    Artwork original = getArtworkByID(artworkId);

    cv::Mat image = cv::imread("images/" + artworkId + ".jpg");
    if (image.empty()) {
    throw std::runtime_error("Failed to load image for: " + artworkId);
    }


    if (params.count("width") && params.count("height"))
    {
        image = cropImage(image, params);
    }
    if (params.count("type"))
    {
        image = applyFilter(image, params);
    }

    Artwork edited = original;
    edited.image = image; 
    return edited;

}

cv::Mat ArtworkManager::rotateImage(const std::string &artworkId, double angle)
{
    // Contributors : Sarah and Taro
    // Purpose : rotate the image clockwise by the requested angle
    // Parameters :
    // artworkID: a string that identifies a unique artpiece (str)
    // angle: the angle in degrees to rotate the image by (double)
    // Return Value: new_image: the newly rotated version of the image (Matrix)
    cv::Mat src = cv::imread("images/" + artworkId + ".jpg");
    if (src.empty()) {
    throw std::runtime_error("Failed to load image for: " + artworkId);
    }

    cv::Point2f center(src.cols / 2.0F, src.rows / 2.0F);
    // conver the angle to radians
    angle = angle * CV_PI / 180.0;

    // get the rotation matrix
    cv::Mat rot = getRotationMatrix2D(center, angle, 1.0);

    // bounding box to prevent cropping
    cv::Rect2f bound_box = RotatedRect(Point2f(), src.size(), angle).boundingRect2f();

    // adjust the transformation matrix based on bounds
    rot.at<double>(0, 2) += bound_box.width / 2.0 - src.cols / 2.0;
    rot.at<double>(1, 2) += bound_box.height / 2.0 - src.rows / 2.0;

    // rotate and save the new image
    cv::Mat new_image;
    cv::warpAffine(src, new_image, rot, bound_box.size());

    // FINISH: UPDATE IMAGE;
    return new_image;
}


void EditorManager::resetImage(const std::string &artworkId)
// Contributors : Sarah, Taro and Huiwen
{
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
        } else {
            std::cout << "Restored image saved to: " << outputPath << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Exception in resetImage: " << e.what() << "\n";
    }
}
