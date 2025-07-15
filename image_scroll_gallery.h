#ifndef IMAGE_SCROLL_GALLERY_H
#define IMAGE_SCROLL_GALLERY_H

#include <list>
#include <string>
#include <unordered_map>
#include <opencv2/core.hpp>
#include <QListView>

class ImageScrollGallery : public QObject {
private:
	QListView* listView;
	float posX;
	float posY;
	float width;
	float height;
	float bufferX;
	float bufferY;
	int iconSize;
	int itemSize;
	int numTotalImages;
	int imagesPerRow;
	int numRows;

	void loadImagesInView(int firstIndex, int lastIndex);

	QPixmap fitPixmapToSize(const QPixmap& pixmap, int width, int height, bool keepAspectRatio);

public:
	ImageScrollGallery(QWidget* parent, float x, float y, float width, float height, float bufferX, float bufferY, int numTotalImages, int imagesPerRow, int iconSize, int itemSize);
	void onScroll(int value);
};

#endif // IMAGE_SCROLL_GALLERY_H
