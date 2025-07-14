#include <image_scroll_gallery.h>

class ImageScrollGallery {
private:
	Qt::QListView* list_view = nullptr;
	float pos_x = 0;
	float pos_y = 0;
	float width = 0;
	float height = 0;
	float buffer_x = 0;
	float buffer_y = 0;
	int num_total_images = 0;
	int images_per_row = 0;
	int num_rows = 0;

	void loadImagesInView(int first_index, int last_index) {
		// Contributors: Lucas Giebler
		// Purpose: Loads images into the scroll view
		// Parameters: int first_index
		//             int last_index
		// Return Value: void

		for (int i = first_index; i < last_index; i++) {
			string artworkId = std::to_string(i);

		}
	}

public:
	ImageScrollGallery(float x, float y, float width, float height, float buffer_x, float buffer_y, int images_per_row) {
		// Contributors: Lucas Giebler
		// Purpose: Constructor for ImageScrollGallery
		// Parameters: float x
		//             float y
		//             float width
		//			   float height
		//             float buffer_x
		//             float buffer_y
		//             int images_per_row
		// Return Value: ImageScrollGallery

		this->list_view = new Qt::QListView(); // Figure this out
		this->pos_x = x;
		this->pos_y = y;
		this->width = width;
		this->height = height;
		this->buffer_x = buffer_x;
		this->buffer_y = buffer_y;
		this->images_per_row = images_per_row;
	}

	void onScroll() {
		// Contributors: Lucas Giebler
		// Purpose: Runs every time the scroll view is scrolled and updates the images in the view
		// Parameters: 
		// Return Value: void

		int first_index = list_view.indexAt(pos_x+buffer_x, pos_y+buffer_y);
		int last_index = list_view.indexAt(pos_x + width - buffer_x, pos_y + height - buffer_y);
	}
};