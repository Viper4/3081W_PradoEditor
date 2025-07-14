#ifndef PRADO_EDITOR_INTERFACE_H
#define PRADO_EDITOR_INTERFACE_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <optional>

struct Artwork {
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


class PradoEditorMobileInterface {
private:
    std::vector<Artwork> gallery;

    // Internal helper to find artwork by ID (returns pointer to artwork or nullptr)
    Artwork* findArtworkById(const std::string& artworkId) {
        for (auto& art : gallery) {
            if (art.id == artworkId) {
                return &art;
            }
        }
        return nullptr;
    }

public:
    PradoEditorMobileInterface() = default;

    std::vector<Artwork> getArtworkGallery() {
        return gallery;
    }

    void sortArtworks(const std::string& criteria) {
        if (criteria == "Newest") {
            std::sort(gallery.begin(), gallery.end(), [](const Artwork& a, const Artwork& b) {
                return a.year > b.year;
            });
        } else if (criteria == "Oldest") {
            std::sort(gallery.begin(), gallery.end(), [](const Artwork& a, const Artwork& b) {
                return a.year < b.year;
            });
        } else if (criteria == "Artist") {
            std::sort(gallery.begin(), gallery.end(), [](const Artwork& a, const Artwork& b) {
                return a.author < b.author;
            });
        } else if (criteria == "Style") {
            std::sort(gallery.begin(), gallery.end(), [](const Artwork& a, const Artwork& b) {
                return a.work_subtitle < b.work_subtitle;
            });
        } else {
            std::cerr << "Unknown sorting criteria: " << criteria << std::endl;
        }
    }

    void resetImage(const std::string& artworkId) {
        Artwork* art = findArtworkById(artworkId);
        if (!art) {
            std::cerr << "Error: artwork not found\n";
            return;
        }

        art->work_image_url = art->original_work_image_url;
        art->x = art->original_x;
        art->y = art->original_y;
    }

    Artwork getArtworkDescription(const std::string& artworkId);  // You still need to implement this
    Artwork applyFilterToImage(const std::string& artworkId);      // You still need to implement this
    void splitSubtitle(const std::string& work_subtitle);          // You still need to implement this
    void editImage(const std::string& artworkId);                  // You still need to implement this
    void cropImage(const std::string& artworkId, int x, int y, int width, int height); // implement
    void rotateImage(const std::string& artworkId, double angle);                     // implement
};

#endif
