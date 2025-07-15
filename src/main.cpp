#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <opencv2/opencv.hpp>
#include "artwork.h"
#include "EditorManager.h"
#include "applyFilter.h"
#include "cropImage.h"
#include "getArtworkByID.h"
#include <gtest/gtest.h>
#include "artwork.h"

std::vector<Artwork> GlobalGallery;

int main() {
    std::ifstream file("smallerpaintings.csv");
    std::string line;
    std::getline(file, line); // Skip header

    PradoEditorMobileInterface interface;
    int idcount = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Artwork art;
        SubtitleData subtitle;

        std::getline(ss, art.work_image_url, ',');
        std::getline(ss, art.author, ',');
        std::getline(ss, art.work_title, ',');
        std::getline(ss, art.work_subtitle, ',');
        std::getline(ss, art.work_description, ',');
        std::getline(ss, art.id, ','); // Use catalog ID

        subtitle = interface.splitSubtitle(art.work_subtitle);
        art.year = subtitle.year;
        GlobalGallery.push_back(art);
        ++idcount;
    }

    std::cout << "Loaded " << GlobalGallery.size() << " artworks.\n";

    // ------------------- APPLY FILTER -------------------
    std::cout << "\n[ApplyFilter] Test: grayscale\n";
    cv::Mat colorImg = cv::Mat::ones(10, 10, CV_8UC3) * 255;
    std::map<std::string, int> grayscaleParams = { {"type", 1} };
    cv::Mat grayImg = applyFilter(colorImg, grayscaleParams);
    std::cout << "Channels after grayscale: " << grayImg.channels() << " (Expected: 1)\n";

    std::cout << "\n[ApplyFilter] Test: invert\n";
    cv::Mat blackImg = cv::Mat::zeros(10, 10, CV_8UC1);
    std::map<std::string, int> invertParams = { {"type", 2} };
    cv::Mat whiteImg = applyFilter(blackImg, invertParams);
    std::cout << "Pixel[0,0] after invert: " << static_cast<int>(whiteImg.at<uchar>(0, 0)) << " (Expected: 255)\n";

    // ------------------- CROP IMAGE -------------------
    std::cout << "\n[CropImage] Test: crop 5x5\n";
    cv::Mat input = cv::Mat::ones(10, 10, CV_8UC1) * 200;
    std::map<std::string, int> cropParams = { {"x", 2}, {"y", 2}, {"width", 5}, {"height", 5} };
    try {
        cv::Mat cropped = cropImage(input, cropParams);
        std::cout << "Cropped size: " << cropped.rows << "x" << cropped.cols << " (Expected: 5x5)\n";
    } catch (const std::exception& e) {
        std::cerr << "CropImage error: " << e.what() << "\n";
    }

    // ------------------- GET ARTWORK BY ID -------------------
    std::cout << "\n[GetArtworkByID] Test: lookup by ID\n";
    ArtworkManager manager;
    std::string testId = "art001"; // must exist in your CSV
    Artwork found = manager.getArtworkByID(testId);
    if (!found.work_title.empty()) {
        std::cout << "Found artwork: " << found.work_title << " by " << found.author << "\n";
    } else {
        std::cout << "Artwork ID '" << testId << "' not found.\n";
    }

    // ------------------- SPLIT SUBTITLE -------------------
    std::cout << "\n[SplitSubtitle] Test:\n";
    if (!GlobalGallery.empty()) {
        std::string subtitleText = GlobalGallery[0].work_subtitle;
        SubtitleData parsed = interface.splitSubtitle(subtitleText);
        std::cout << "Subtitle: " << subtitleText << "\n";
        std::cout << "Parsed year: " << parsed.year << "\n";
        std::cout << "Parsed medium: " << parsed.medium << "\n";
        std::cout << "Parsed dimensions: " << parsed.dimensions << "\n";
    }

    return 0;
    // Prepare dummy GlobalGallery for tests
void setupDummyGallery() {
    GlobalGallery.clear();

    Artwork a1, a2, a3;
    a1.id = "001";
    a1.work_title = "Title A";
    a1.author = "Artist Z";
    a1.year = "2000";

    a2.id = "002";
    a2.work_title = "Title B";
    a2.author = "Artist Y";
    a2.year = "1995";

    a3.id = "003";
    a3.work_title = "Title C";
    a3.author = "Artist X";
    a3.year = "2020";

    GlobalGallery = { a1, a2, a3 };
}

TEST(ArtworkInterfaceTests, GetArtworkGalleryReturnsAll) {
    setupDummyGallery();
    PradoEditorMobileInterface iface;
    auto list = iface.getArtworkGallery();
    // Expect gallery size matches mock
    EXPECT_EQ(list.size(), 3);
}

TEST(ArtworkInterfaceTests, SortArtworksByNewest) {
    setupDummyGallery();
    PradoEditorMobileInterface iface;
    iface.sortArtworks("Newest");
    auto list = iface.getArtworkGallery();
    // First artwork should be year 2020
    EXPECT_EQ(list.front().year, "2020");
}

TEST(ArtworkInterfaceTests, SortArtworksByArtistAlphabetically) {
    setupDummyGallery();
    PradoEditorMobileInterface iface;
    iface.sortArtworks("Artist");
    auto list = iface.getArtworkGallery();
    // First author should be Artist X
    EXPECT_EQ(list.front().author, "Artist X");
}

TEST(ArtworkInterfaceTests, SortArtworksInvalidCriteriaDoesNotThrow) {
    setupDummyGallery();
    PradoEditorMobileInterface iface;
    // Just test it runs without crashing (no exception)
    iface.sortArtworks("UnknownCriterion");
    SUCCEED();
}

TEST(ArtworkInterfaceTests, GetArtworkDescriptionValidID) {
    setupDummyGallery();
    PradoEditorMobileInterface iface;
    auto art = iface.getArtworkDescription("002");
    // Should find Title B
    EXPECT_EQ(art.work_title, "Title B");
}

TEST(ArtworkInterfaceTests, GetArtworkDescriptionInvalidIDReturnsEmpty) {
    setupDummyGallery();
    PradoEditorMobileInterface iface;
    auto art = iface.getArtworkDescription("999");
    // Should return empty title
    EXPECT_TRUE(art.work_title.empty());
}

TEST(ArtworkInterfaceTests, SplitSubtitleNormalCase) {
    PradoEditorMobileInterface iface;
    std::string input = "1999. Oil on canvas, 120 x 80 cm";
    SubtitleData sub = iface.splitSubtitle(input);
    // Year and medium and dimensions should be extracted
    EXPECT_EQ(sub.year, "1999");
    EXPECT_EQ(sub.medium.find("Oil"), 0);
    EXPECT_TRUE(sub.dimensions.find("cm") != std::string::npos);

}
