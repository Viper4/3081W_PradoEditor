

#define PRADO_EDITOR_INTERFACE_H
#include <string>
#include <string.h>
#include <vector>

struct Artwork {
    std::string work_title;
    std::string work_image_url;
    std::string author;
    std::string work_subtitle;
    std::string year;
    std::string work_description;
    int x;
    int y;
};

//# work_url: string
//# work_image_url: string
//# author: string
//# author_bio: string
//# author_id: string
// # author_url: string
// # work_title: string
// # work_subtitle: string
// contains year, dimensions, type
// # work_exposed: string
// # work_description: string






class PradoEditorMobileInterface {
public:


    Artwork:Artwork()
    {
        id = "";
        work_title = "";
    }
    // Retrieve list of artworks for the main gallery.
    //for the main page for scrolling through pieces
    std::vector<Artwork> getArtworkGallery();

    std::vector<Artwork> searchArtworks(const std::string& query);

    void sortArtworks(const std::string& criteria){

    }

    Artwork getArtworkDescription(const std::string& artworkId);

    Artwork applyFilterToImage(const std::string& artworkId);

    void resetImage(const std::string& artworkId);

    void splitSubtitle(const std::string &work_subtitle);

    void editImage(const std::string& artworkId);
    void cropImage(const std::string& artworkId, int x, int y, int width, int height);
    void rotateImage(const std::string& artworkId, double angle);
};

