#include <image_scroll_gallery.h>

ImageScrollGallery::ImageScrollGallery(float x, float y, float width, float height, float bufferX, float bufferY, int imagesPerRow) {
	// Contributors: Lucas Giebler
	// Purpose: Constructor for ImageScrollGallery
	// Parameters: float x
	//             float y
	//             float width
	//			   float height
	//             float bufferX
	//             float bufferY
	//             int imagesPerRow
	// Return Value: ImageScrollGallery

	this->listView = new QListView(); // Figure this out
	this->posX = x;
	this->posY = y;
	this->width = width;
	this->height = height;
	this->bufferX = bufferX;
	this->bufferY = bufferY;
	this->imagesPerRow = imagesPerRow;
}

void ImageScrollGallery::loadImagesInView(int firstIndex, int lastIndex) {
	// Contributors: Lucas Giebler
	// Purpose: Loads images into the scroll view
	// Parameters: int firstIndex
	//             int lastIndex
	// Return Value: void

	for (int i = firstIndex; i < lastIndex; i++) {
		std::string artworkId = std::to_string(i);

	}
}

void ImageScrollGallery::onScroll() {
	// Contributors: Lucas Giebler
	// Purpose: Runs every time the scroll view is scrolled and updates the images in the view
	// Parameters: 
	// Return Value: void

	int firstIndex = listView.indexAt(posX+bufferX, posY+bufferY);
	int lastIndex = listView.indexAt(posX + width - bufferX, posY + height - bufferY);
}
