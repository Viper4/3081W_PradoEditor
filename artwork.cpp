#include "artwork.h"
#include <opencv2/imgcodecs.hpp>  


std::vector<Artwork> PradoEditorMobileInterface::getArtworkGallery() {
    std::vector<Artwork> result;
    for (const auto& [id, artwork] : GlobalGallery) {
        result.push_back(artwork);
    }
    return result;
}

void PradoEditorMobileInterface::sortArtworks(const std::string& criteria) {
    //Contributors: Taro Welches
    //Input: const std::string& criteria
    //Purpose: To sort the displayed artwork by a certain criteria input by user
    //return: none
    std::vector<std::pair<std::string, Artwork>> entries(GlobalGallery.begin(), GlobalGallery.end());

    if (criteria == "Newest") {
        std::sort(entries.begin(), entries.end(),
                  [](const auto& a, const auto& b) { return a.second.year > b.second.year; });
    } else if (criteria == "Oldest") {
        std::sort(entries.begin(), entries.end(),
                  [](const auto& a, const auto& b) { return a.second.year < b.second.year; });
    } else if (criteria == "Artist") {
        std::sort(entries.begin(), entries.end(),
                  [](const auto& a, const auto& b) { return a.second.author < b.second.author; });
    } else if (criteria == "Style") {
        std::sort(entries.begin(), entries.end(),
                  [](const auto& a, const auto& b) { return a.second.work_subtitle < b.second.work_subtitle; });
    } else {
        std::cerr << "Unknown sorting criteria: " << criteria << std::endl;
        return;
    }

    GlobalGallery.clear();
    for (const auto& [id, artwork] : entries) {
        GlobalGallery[id] = artwork;
    }
}

void PradoEditorMobileInterface::resetImage(const std::string& artworkId) {
    //Contributors: Taro Welches
    //Input: const std::string&artworkId
    //Purpose: To restore an edited image's values back to the original
    //Return: None
    Artwork* art = findArtworkById(artworkId);
    if (!art) {
        std::cerr << "Error: artwork not found\n";
        return;
    }

    cv::Mat original = getCachedImage(artworkId);
    if (original.empty()) {
        std::cerr << "Error: original image not found in cache\n";
        return;
    }

    art->work_image_url = "restored_" + artworkId + ".jpg";
    cv::imwrite(art->work_image_url, original);
    art->x = art->original_x;
    art->y = art->original_y;
}

Artwork PradoEditorMobileInterface::getArtworkDescription(const std::string& artworkId) {
    // Contributors : Sarah
    // Purpose : retrieve an artwork description
    // Parameters : artworkID: a string that identifies a unique artpiece (str)
    // Return Value: either the artwork description or its availability status (str)
    Artwork art = findArtworkById(artworkId);
    if (!art || art->work_description.empty())
    {
        return "Description not available.";
    }
    return art->work_description;
}

Artwork PradoEditorMobileInterface::applyFilterToImage(const std::string& artworkId) {
    //Contributors : Taro & Huiwen (I think huiwen did it but I did it on accident feel free to change this)
    //Purpose : apply a filter to a selected image by the user
    //Parameters: artworkID: a string that identifies a unique artpiece (str)
    //Return Value: An edited artwork object
    Artwork* art = findArtworkById(artworkId);
    if (!art) {
        std::cerr << "Error: artwork not found\n";
        return {};
    }

    cv::Mat original = ImageCache::getCachedImage(artworkId);
    if (original.empty()) {
        std::cerr << "Error: image not found in cache\n";
        return {};
    }

    // Apply grayscale + blur
    cv::Mat gray, blurred;
    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(gray, gray, cv::COLOR_GRAY2BGR); // convert back so dimensions match
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

    // Create a new filename
    std::string new_filename = "filtered_" + artworkId + ".jpg";
    cv::imwrite(new_filename, blurred);

    // Update artwork
    art->work_image_url = new_filename;
    ImageCache::addImage(artworkId, blurred);  // Optionally update cache

    return *art;
}


void PradoEditorMobileInterface::splitSubtitle(const std::string &work_subtitle){
    // Contributors : Sarah
    // Purpose : split the given string subtitle description into a struct containing separate year, medium, and
    // Parameters : work_subtitle: the original subtitle field in paragraph form
    // Return Value: subtitle: a struct of the original subtitle field
    SubtitleData subtitle;

    size_t yrPos = work_subtitle.find('.');
    std::string year = work_subtitle.substr(0, yrPos);
    subtitle.year = std::string(trim(year));

    size_t medPos = work_subtitle.find(',');
    std::string medium = work_subtitle.substr(yrPos + 1, medPos - yrPos - 1);
    subtitle.medium = std::string(trim(medium));

    subtitle.dimensions = trim(work_subtitle.substr(medPos + 1));

    return subtitle;
}

void PradoEditorMobileInterface::editImage(const std::string& artworkId) {
    std::cout << "Editing image for artwork ID: " << artworkId << std::endl;
    // Placeholder
}

void PradoEditorMobileInterface::cropImage(const std::string& artworkId, int x, int y, int width, int height) {
    std::cout << "Cropping image for artwork ID: " << artworkId
              << " to region x=" << x << " y=" << y
              << " width=" << width << " height=" << height << std::endl;
    // Placeholder
}

void PradoEditorMobileInterface::rotateImage(const std::string &artworkId, double angle){
    // Contributors : Sarah
    // Purpose : rotate the image clockwise by the requested angle
    // Parameters :
    // artworkID: a string that identifies a unique artpiece (str)
    // angle: the angle in degrees to rotate the image by (double)
    // Return Value: new_image: the newly rotated version of the image (Matrix)
    Mat src = getImage(artworkId);
    Point2f center(src.cols / 2.0F, src.rows / 2.0F);
    // conver the angle to radians
    angle = angle * CV_PI / 180.0;

    // get the rotation matrix
    Mat rot = getRotationMatrix2D(center, angle, 1.0);

    // bounding box to prevent cropping
    Rect2f bound_box = RotatedRect(Point2f(), src.size(), angle).boundingRect2f();

    // adjust the transformation matrix based on bounds
    rot.at<double>(0, 2) += bound_box.width / 2.0 - src.cols / 2.0;
    rot.at<double>(1, 2) += bound_box.height / 2.0 - src.rows / 2.0;

    // rotate and save the new image
    Mat new_image;
    warpAffine(src, new_image, rot, bound_box.size());

    // FINISH: UPDATE IMAGE;
    return new_image;
}

void cv::Mat getImage(const std::string& artworkId){
    std::cout << "make some sort of call to getimagecache and then error handle\n";
}
