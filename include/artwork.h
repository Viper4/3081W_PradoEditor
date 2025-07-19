#ifndef ARTWORK_H
#define ARTWORK_H
#include <string>
#include <opencv2/core.hpp>

struct Artwork
{
    std::unordered_map<std::string, std::string> metadata;
};

enum SortCriteria {
    Title,
    Newest,
    Oldest,
    Artist
};

class PradoEditorMobileInterface
{
private:

public:
    std::vector<Artwork> artworkMapToVector(const std::unordered_map<std::string, Artwork>& map);
    void sortArtworks(const SortCriteria &criteria, std::vector<Artwork> &artworks);
};

#endif
