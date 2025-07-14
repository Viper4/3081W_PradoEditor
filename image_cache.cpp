#include <image_cache.h>

class ImageCache {
public:
    static int max_images;
    static std::list<std::string> usage_list; // Most recently used id will be at the front of the list
    static std::unordered_map<std::string, cv::Mat> image_map; // Mat is OpenCV's matrix object to represent an image

    static void updateUsage(const std::string& artworkId) {
        // Contributors: Lucas Giebler
        // Purpose: Updates the usage of the artworkId in the linked list
        // Parameters: string artworkId
        // Return Value: void

        if (ImageCache::image_map.find(artworkId) == ImageCache::image_map.end()) {
            ImageCache::usage_list.remove(artworkId);
        }
        ImageCache::usage_list.push_front(artworkId);
    }

    static void addImage(const std::string& artworkId, const cv::Mat& image) {
        // Contributors: Lucas Giebler
        // Purpose: Adds a new image to the cache
        // Parameters: string artworkId
        //             cv::Mat image
        // Return Value: void

        if (ImageCache::image_map.size() >= ImageCache::max_images && ImageCache::image_map.find(artworkId) != ImageCache::image_map.end()) {
            ImageCache::usage_list.pop_back();
        }
        ImageCache::image_map[artworkId] = image;
        ImageCache::updateUsage(artworkId);
    }

    static cv::Mat getCachedImage(const std::string& artworkId) {
        // Contributors: Lucas Giebler
        // Purpose: Tries to retrieve an image from the cache using the artworkId
        // Parameters: string artworkId
        // Return Value: cv::Mat

        if (ImageCache::image_map.find(artworkId) == ImageCache::image_map.end()) {
            return cv::Mat();
		}
        return ImageCache::image_map[artworkId];
    }
}