#ifndef IMAGE_SCROLL_GALLERY_H
#define IMAGE_SCROLL_GALLERY_H

#include <list>
#include <string>
#include <unordered_map>

using namespace cv;

class ImageScrollGallery {
private:
	Qt::QListView* listView;
	float pos_x;
	float pos_y;
	float width;
	float height;
	float buffer_x;
	float buffer_y;
	int num_total_images;
	int images_per_row;
	int num_rows;

	void loadImagesInView(int first_index, int last_index);

public:
	ImageScrollGallery(float x, float y, float width, float height, float buffer_x, float buffer_y, int images_per_row);
	void onScroll();
};

#endif // IMAGE_SCROLL_GALLERY_H
