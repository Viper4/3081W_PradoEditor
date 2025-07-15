#ifndef ARTWORK_H
#define ARTWORK_H
#include <string>
#include <opencv2/core.hpp>
#include <vector>

struct Artwork
{
    std::string id;
    cv::Mat image;
    std::string description;
    std::string work_image_url;

    Artwork() = default;
    Artwork(const std::string &id, const cv::Mat &img)
        : id(id), image(img) {}
};

extern std::vector<Artwork> GlobalGallery;
#endif

// # work_url: string
// # work_image_url: string
// # author: string
//  # work_title: string
//  # work_subtitle: string
//  contains year, dimensions, type
//  # work_description: string

struct SubtitleData
{
    std::string year;
    std::string medium;
    std::string dimensions;
};

class PradoEditorMobileInterface
{
public:
    std::vector<Artwork> getArtworkGallery();
    void sortArtworks(const std::string &criteria);
    std::string getArtworkDescription(const std::string &artworkId);
    SubtitleData splitSubtitle(const std::string &work_subtitle);
};

#endif
