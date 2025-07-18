/*#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include <map>
#include "artwork.h"
#include "managers.h"
// Contributors: Sarah
// Purpose: test the functionality and correctness of EditorManager & artwork functions

// trim() test cases
TEST(UtilsTest, TrimRemovesSpaces)
{
    PradoEditorMobileInterface editor;
    EXPECT_EQ(editor.trim("  hello  "), "hello");
    EXPECT_EQ(editor.trim("noSpaces"), "noSpaces");
    EXPECT_EQ(editor.trim("   "), "");
}
// splitSubtitle test cases
TEST(SubtitleTest, SplitSubtitleExtractsFields)
{
    PradoEditorMobileInterface editor;
    std::string subtitleStr = "1764. Lápiz negro sobre papel verdoso, 572 x 348 mm";

    SubtitleData result = editor.splitSubtitle(subtitleStr);

    EXPECT_EQ(result.year, "1764");
    EXPECT_EQ(result.medium, "Lápiz negro sobre papel verdoso");
    EXPECT_EQ(result.dimensions, "572 x 348 mm");
}

// getDescription test cases
TEST(ArtworkTest, GetDescriptionWorks)
{
    PradoEditorMobileInterface editor;

    Artwork art;
    art.id = "123";
    art.work_description = "Pretty artwork yay!!";

    EXPECT_EQ(editor.getArtworkDescription("123"), "Pretty artwork yay!!");
}

// getArtworkByID test cases
TEST(EditorManagerTest, GetArtworkByIDLoadsImage)
{
    EditorManager manager;

    // create a dummy image
    cv::Mat dummy = cv::Mat::ones(100, 100, CV_8UC3);
    cv::imwrite("images/test_art.jpg", dummy);

    Artwork art = manager.getArtworkByID("test_art");
    EXPECT_EQ(art.id, "test_art");
    EXPECT_FALSE(art.getImage().empty());
}

// applyFilter test cases
TEST(EditorManagerTest, ApplyFilterGrayscale)
{
    EditorManager manager;
    cv::Mat img = cv::Mat::ones(50, 50, CV_8UC3);

    std::map<std::string, int> params = {{"type", 1}};
    cv::Mat filtered = manager.applyFilter(img, params);

    EXPECT_EQ(filtered.channels(), 1);
}

// cropImage test cases
TEST(EditorManagerTest, CropImageValidRegion)
{
    EditorManager manager;
    cv::Mat img = cv::Mat::ones(100, 100, CV_8UC3);

    std::map<std::string, int> params = {{"x", 10}, {"y", 10}, {"width", 50}, {"height", 50}};
    cv::Mat cropped = manager.cropImage(img, params);

    EXPECT_EQ(cropped.cols, 50);
    EXPECT_EQ(cropped.rows, 50);
}

TEST(EditorManagerTest, CropImageThrowsOutOfBounds)
{
    EditorManager manager;
    cv::Mat img = cv::Mat::ones(100, 100, CV_8UC3);

    std::map<std::string, int> params = {{"x", 90}, {"y", 90}, {"width", 20}, {"height", 20}};
    EXPECT_THROW(manager.cropImage(img, params), std::out_of_range);
}

// editImage test case, testing multiple edits
TEST(EditorManagerTest, EditImageAppliesCropAndFilter)
{
    EditorManager manager;

    cv::Mat img = cv::Mat::ones(100, 100, CV_8UC3);
    cv::imwrite("images/edit_test.jpg", img);

    std::map<std::string, int> params = {
        {"x", 10}, {"y", 10}, {"width", 50}, {"height", 50}, {"type", 2} // invert
    };

    Artwork art = manager.editImage("edit_test", params);
    cv::Mat result = art.getImage();

    EXPECT_EQ(result.cols, 50);
    EXPECT_EQ(result.rows, 50);
}

// rotateImage test case
TEST(EditorManagerTest, RotateImageDoesNotThrow)
{
    EditorManager manager;

    // create and save dummy image
    cv::Mat img = cv::Mat::ones(100, 200, CV_8UC3);
    cv::imwrite("images/rotate_test.jpg", img);

    cv::Mat rotated = manager.rotateImage("rotate_test", 45);
    EXPECT_FALSE(rotated.empty());
}

TEST(ArtworkInterfaceTests, GetArtworkGalleryReturnsAll)
{
    setupDummyGallery();
    PradoEditorMobileInterface iface;
    auto list = iface.getArtworkGallery();
    // Expect gallery size matches mock
    EXPECT_EQ(list.size(), 3);
}

TEST(ArtworkInterfaceTests, SortArtworksByNewest)
{
    setupDummyGallery();
    PradoEditorMobileInterface iface;
    iface.sortArtworks("Newest");
    auto list = iface.getArtworkGallery();
    // First artwork should be year 2020
    EXPECT_EQ(list.front().year, "2020");
}

TEST(ArtworkInterfaceTests, SortArtworksByArtistAlphabetically)
{
    setupDummyGallery();
    PradoEditorMobileInterface iface;
    iface.sortArtworks("Artist");
    auto list = iface.getArtworkGallery();
    // First author should be Artist X
    EXPECT_EQ(list.front().author, "Artist X");
}

TEST(ArtworkInterfaceTests, SortArtworksInvalidCriteriaDoesNotThrow)
{
    setupDummyGallery();
    PradoEditorMobileInterface iface;
    // Just test it runs without crashing (no exception)
    iface.sortArtworks("UnknownCriterion");
    SUCCEED();
}

TEST(ArtworkInterfaceTests, GetArtworkDescriptionValidID)
{
    setupDummyGallery();
    PradoEditorMobileInterface iface;
    auto art = iface.getArtworkDescription("002");
    // Should find Title B
    EXPECT_EQ(art.work_title, "Title B");
}

TEST(ArtworkInterfaceTests, GetArtworkDescriptionInvalidIDReturnsEmpty)
{
    setupDummyGallery();
    PradoEditorMobileInterface iface;
    auto art = iface.getArtworkDescription("999");
    // Should return empty title
    EXPECT_TRUE(art.work_title.empty());
}

TEST(ArtworkInterfaceTests, SplitSubtitleNormalCase)
{
    PradoEditorMobileInterface iface;
    std::string input = "1999. Oil on canvas, 120 x 80 cm";
    SubtitleData sub = iface.splitSubtitle(input);
    // Year and medium and dimensions should be extracted
    EXPECT_EQ(sub.year, "1999");
    EXPECT_EQ(sub.medium.find("Oil"), 0);
    EXPECT_TRUE(sub.dimensions.find("cm") != std::string::npos);
}
*/