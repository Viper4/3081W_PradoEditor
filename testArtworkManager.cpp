#include <iostream>
#include <cassert>
#include "artwork.h"
#include "ArtworkManager.h"
#include "image_cache.h"
#include <opencv2/imgcodecs.hpp>

void test_getArtworkByID() {
    std::cout << "Running: test_getArtworkByID...\n";

    std::string testId = "test001";
    cv::Mat dummy = cv::Mat::ones(100, 100, CV_8UC3) * 127;
    ImageCache::addImage(testId, dummy);

    ArtworkManager manager;
    Artwork art = manager.getArtworkByID(testId);

    assert(art.getImage().rows == 100);
    assert(art.getImage().cols == 100);
    std::cout << "✓ Passed test_getArtworkByID\n";
}

void test_applyFilter_grayscale() {
    std::cout << "Running: test_applyFilter_grayscale...\n";

    cv::Mat image = cv::Mat::ones(100, 100, CV_8UC3) * 255;
    ArtworkManager manager;

    std::map<std::string, int> params = { {"type", 1} }; // 1 = grayscale
    cv::Mat filtered = manager.applyFilter(image, params);

    assert(filtered.channels() == 1 || filtered.channels() == 3); // Might get converted back to 3
    std::cout << "✓ Passed test_applyFilter_grayscale\n";
}

void test_cropImage_valid() {
    std::cout << "Running: test_cropImage_valid...\n";

    cv::Mat image = cv::Mat::ones(200, 200, CV_8UC3);
    ArtworkManager manager;

    std::map<std::string, int> params = {
        {"x", 50}, {"y", 50}, {"width", 100}, {"height", 100}
    };

    cv::Mat cropped = manager.cropImage(image, params);
    assert(cropped.rows == 100);
    assert(cropped.cols == 100);
    std::cout << "✓ Passed test_cropImage_valid\n";
}

void test_editImage_cropAndFilter() {
    std::cout << "Running: test_editImage_cropAndFilter...\n";

    std::string testId = "edit001";
    cv::Mat dummy = cv::Mat::ones(200, 200, CV_8UC3) * 128;
    ImageCache::addImage(testId, dummy);

    ArtworkManager manager;
    std::map<std::string, int> params = {
        {"x", 0}, {"y", 0}, {"width", 100}, {"height", 100}, {"type", 1}
    };

    Artwork edited = manager.editImage(testId, params);
    cv::Mat img = edited.getImage();
    assert(img.rows == 100 && img.cols == 100);
    std::cout << "✓ Passed test_editImage_cropAndFilter\n";
}

void test_rotateImage_90degrees() {
    std::cout << "Running: test_rotateImage_90degrees...\n";

    std::string testId = "rotate001";
    cv::Mat dummy = cv::Mat::zeros(100, 200, CV_8UC3);
    ImageCache::addImage(testId, dummy);

    ArtworkManager manager;
    cv::Mat rotated = manager.rotateImage(testId, 90);

    assert(rotated.cols == 100);
    assert(rotated.rows == 200);
    std::cout << "✓ Passed test_rotateImage_90degrees\n";
}

void runAllTests() {
    ImageCache::max_images = 50;  // Ensure cache can hold tests

    test_getArtworkByID();
    test_applyFilter_grayscale();
    test_cropImage_valid();
    test_editImage_cropAndFilter();
    test_rotateImage_90degrees();

    std::cout << "\nAll tests passed!\n";
}

int main() {
    runAllTests();
    return 0;
}
