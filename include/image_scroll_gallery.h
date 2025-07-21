#ifndef IMAGE_SCROLL_GALLERY_H
#define IMAGE_SCROLL_GALLERY_H

#include <list>
#include <string>
#include <unordered_map>
#include <opencv2/core.hpp>
#include <QListView>
#include <artwork.h>

class ImageScrollGallery : public QWidget {
private:
	QListView* listView;
	int iconSize;
	int itemSize;
	int numTotalImages;
	QIcon placeholderIcon;
	int previousFirstIndex;

	void loadImagesInView(int firstIndex, int lastIndex);
	void onScroll(int value);
	void onItemClicked(const QModelIndex& index);

public:
	static std::unordered_map<std::string, Artwork> GlobalGallery;

	ImageScrollGallery(QListView* listView, int iconSize, int itemSize);
};

#endif // IMAGE_SCROLL_GALLERY_H
