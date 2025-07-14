#include <"artwork.h">
#include <opencv2/imgcodecs.hpp>  

Artwork* PradoEditorMobileInterface::findArtworkById(const std::string& artworkId) {
    auto it = GlobalGallery.find(artworkId);
    if (it == GlobalGallery.end()) {
        return nullptr;
    }
    return &it->second;
}


std::vector<Artwork> PradoEditorMobileInterface::getArtworkGallery() {
    std::vector<Artwork> result;
    for (const auto& [id, artwork] : GlobalGallery) {
        result.push_back(artwork);
    }
    return result;
}

void PradoEditorMobileInterface::sortArtworks(const std::string& criteria) {
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
    //Contributors: Taro Welchse
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
    Artwork* art = findArtworkById(artworkId);
    if (!art) {
        std::cerr << "Error: artwork not found\n";
        return {};
    }
    return *art;
}

Artwork PradoEditorMobileInterface::applyFilterToImage(const std::string& artworkId) {
    Artwork* art = findArtworkById(artworkId);
    if (!art) {
        std::cerr << "Error: artwork not found\n";
        return {};
    }

    // Placeholder: you would apply a filter using OpenCV here
    std::cout << "Applying filter to " << art->id << std::endl;
    return *art;
}

void PradoEditorMobileInterface::splitSubtitle(const std::string& work_subtitle) {
    std::cout << "Splitting subtitle: " << work_subtitle << std::endl;
    // Placeholder
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

void PradoEditorMobileInterface::rotateImage(const std::string& artworkId, double angle) {
    std::cout << "Rotating image for artwork ID: " << artworkId << " by " << angle << " degrees\n";
    // Placeholder
}
