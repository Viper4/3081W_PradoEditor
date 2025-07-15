#ifndef ARTWORK_H
#define ARTWORK_H
#include <string>
#include <opencv2/core.hpp>
#include <vector>

struct Artwork
{
    std::string id;
    cv::Mat image;
    std::string image_url;
    std::string description;
    std::string author;
	std::string title;
	std::string subtitle;
	std::string year;


    Artwork() = default;
    Artwork(const std::string &id, const cv::Mat &img)
        : id(id), image(img) {}
};

extern std::vector<Artwork> GlobalGallery;

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

enum SortCriteria {
    Title,
    Newest,
    Oldest,
    Artist
};

class PradoEditorMobileInterface
{
public:
    std::vector<Artwork> getArtworkGallery();
    void sortArtworks(const SortCriteria &criteria);
    SubtitleData splitSubtitle(const std::string &work_subtitle);
};

#endif
