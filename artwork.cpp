#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <algorithm>

using namespace cv;

#ifndef PRADO_EDITOR_INTERFACE_H
#define PRADO_EDITOR_INTERFACE_H

struct Artwork
{
    std::string id = "";
    std::string work_title = "";
    std::string work_image_url = "";
    std::string original_work_image_url = "";
    std::string author = "";
    std::string author_bio = "";
    std::string author_id = "";
    std::string author_url = "";
    std::string work_subtitle = "";
    std::string year = "";
    std::string work_description = "";
    std::string work_exposed = "";
    int x = 0;
    int y = 0;
    int original_x = 0;
    int original_y = 0;
};

struct SubtitleData
{
    std::string year;
    std::string medium;
    std::string dimensions;
};

class PradoEditorMobileInterface
{
private:
    std::vector<Artwork> gallery;

    // Internal helper to find artwork by ID (returns pointer to artwork or nullptr)
    Artwork *findArtworkById(const std::string &artworkId)
    {
        for (auto &art : gallery)
        {
            if (art.id == artworkId)
            {
                return &art;
            }
        }
        return nullptr;
    }
    std::string trim(const std::string &str)
    {
        // Contributors : Sarah
        // Purpose : inline function to trim the substrs for the subtitle struct
        // Parameters: str: a string of the the subtitle field to be split
        // Return Value : a substr of the original input (string)
        size_t start = str.find_first_not_of(' ');
        size_t end = str.find_last_not_of(' ');

        if (start == std::string::npos || end == std::string::npos)
            return "";

        return str.substr(start, end - start + 1);
    }

public:
    PradoEditorMobileInterface() = default;

    std::vector<Artwork> getArtworkGallery()
    {
        return gallery;
    }

    void sortArtworks(const std::string &criteria)
    {
        if (criteria == "Newest")
        {
            std::sort(gallery.begin(), gallery.end(), [](const Artwork &a, const Artwork &b)
                      { return a.year > b.year; });
        }
        else if (criteria == "Oldest")
        {
            std::sort(gallery.begin(), gallery.end(), [](const Artwork &a, const Artwork &b)
                      { return a.year < b.year; });
        }
        else if (criteria == "Artist")
        {
            std::sort(gallery.begin(), gallery.end(), [](const Artwork &a, const Artwork &b)
                      { return a.author < b.author; });
        }
        else if (criteria == "Style")
        {
            std::sort(gallery.begin(), gallery.end(), [](const Artwork &a, const Artwork &b)
                      { return a.work_subtitle < b.work_subtitle; });
        }
        else
        {
            std::cerr << "Unknown sorting criteria: " << criteria << std::endl;
        }
    }

    void resetImage(const std::string &artworkId)
    {
        Artwork *art = findArtworkById(artworkId);
        if (!art)
        {
            std::cerr << "Error: artwork not found\n";
            return;
        }

        art->work_image_url = art->original_work_image_url;
        art->x = art->original_x;
        art->y = art->original_y;
    }

    std::string getArtworkDescription(const std::string &artworkId)
    {
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
    };
    Artwork applyFilterToImage(const std::string &artworkId); // You still need to implement this

    SubtitleData splitSubtitle(const std::string &work_subtitle)
    {
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
    };
    void editImage(const std::string &artworkId);                                      // You still need to implement this
    void cropImage(const std::string &artworkId, int x, int y, int width, int height); // implement
    Mat rotateImage(const std::string &artworkId, double angle)
    {
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
    };
};

#endif
