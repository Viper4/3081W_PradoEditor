#include <include/image_scroll_gallery.h>
#include <iostream>
#include <QScrollBar>
#include <QObject>
#include <QListView>
#include <QStandardItemModel>
#include <QStringListModel>
#include <include/image_cache.h>

ImageScrollGallery::ImageScrollGallery(QWidget* parent, float x, float y, float width, float height, float bufferX, float bufferY, int numTotalImages, int imagesPerRow, int iconSize, int itemSize) {
	// Contributors: Lucas Giebler
	// Purpose: Constructor for ImageScrollGallery
	// Parameters: float x
	//             float y
	//             float width
	//			   float height
	//             float bufferX - the buffer between the edge of the gallery and the edge of the window
	//             float bufferY - the buffer between the edge of the gallery and the edge of the window
	//             int imagesPerRow - the number of images per row
	// Return Value: ImageScrollGallery
	// Limitations: 
	// -------------------
	this->posX = x;
	this->posY = y;
	this->width = width;
	this->height = height;
	this->bufferX = bufferX;
	this->bufferY = bufferY;
	this->numTotalImages = numTotalImages;
	this->imagesPerRow = imagesPerRow;
	this->numRows = numTotalImages / imagesPerRow;

	// Initializing list view
	this->listView = new QListView(parent);
	this->listView->setGeometry(x + bufferX, y + bufferY, width, height);

	// Set up the list view for icon grid
	this->listView->setViewMode(QListView::IconMode);
	this->listView->setResizeMode(QListView::Adjust); // items reposition on resize
	this->listView->setMovement(QListView::Static);   // items stay in position
	this->listView->setSpacing(10);                   // Spacing between items
	this->listView->setIconSize(QSize(iconSize, iconSize)); // control icon size
	this->listView->setGridSize(QSize(itemSize, itemSize)); // grid cell size
	this->iconSize = iconSize;
	this->itemSize = itemSize;

	// Add onScroll function as listener to scroll bar
	connect(listView->verticalScrollBar(), &QScrollBar::valueChanged, this, &ImageScrollGallery::onScroll);

	// Load placeholder images into all cells in the scroll view
	QStandardItemModel* model = new QStandardItemModel(this);
	this->listView->setModel(model);
	QPixmap pixmap("C:\\Users\\vpr16\\Documents\\Random\\robloxDefault.png");
	QIcon placeholderIcon(pixmap);
	for (int i = 0; i < numTotalImages; i++) {
		QStandardItem* item = new QStandardItem(placeholderIcon, "Placeholder " + QString::number(i));
		item->setTextAlignment(Qt::AlignCenter);
		model->appendRow(item);
	}
}

void ImageScrollGallery::loadImagesInView(int firstIndex, int lastIndex) {
	// Contributors: Lucas Giebler
	// Purpose: Loads images into the scroll view between firstIndex and lastIndex
	// Parameters: int firstIndex - The index of the first image to load
	//             int lastIndex - The index of the last image to load
	// Return Value: void
	// Limitations: 
	// -------------------
	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(listView->model());
	for (int i = firstIndex; i < lastIndex + 1; i++) {
		std::string artworkId = std::to_string(i);
		cv::Mat image = ImageCache::getCachedImage(artworkId);
		if (image.empty()) {
			continue; // Skip empty images
		}
		// Create new item using loaded image
		QPixmap pixmap = fitPixmapToSize(ImageCache::matToQPixmap(image), iconSize, iconSize, true);
		QStandardItem* item = new QStandardItem(QIcon(pixmap), "Photo " + QString::number(i));
		item->setTextAlignment(Qt::AlignCenter);
		model->setItem(i, 0, item); // Replace item with loaded item
	}
}

QPixmap ImageScrollGallery::fitPixmapToSize(const QPixmap& pixmap, int width, int height, bool keepAspectRatio) {
	// Contributors: Lucas Giebler
	// Purpose: Scales a pixmap to fit a given width and height
	// Parameters: QPixmap pixmap - The pixmap to scale
	//             int width - The width to scale the pixmap to
	//             int height - The height to scale the pixmap to
	//             bool keepAspectRatio - Whether to keep the aspect ratio
	// Return Value: QPixmap
	// Limitations: 
	// -------------------
	// Scale to icon size
	QPixmap scaled;
	if (keepAspectRatio) {
		// Crop to square (centered)
		int side = std::min(pixmap.width(), pixmap.height());
		QPixmap cropped = pixmap.copy(
			(pixmap.width() - side) / 2,
			(pixmap.height() - side) / 2,
			side,
			side
		);

		scaled = cropped.scaled(width, height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
	}
	else {
		// Ignore aspect ratio (stretched to fit)
		scaled = pixmap.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	return scaled;
}

void ImageScrollGallery::onScroll(int value) {
	// Contributors: Lucas Giebler
	// Purpose: Runs every time the scroll view is scrolled and updates the images in the view
	// Parameters: int value - The value of the scroll bar (unused)
	// Return Value: void
	// Limitations: 
	// -------------------
	// Determine which image is at the top left of the scroll view by going down on the y axis step by step
	int firstIndex = -1;
	for (int i = 0; i < itemSize; i++) {
		firstIndex = listView->indexAt(QPoint(posX + bufferX, posY + bufferY + i)).row()-1;
		if (firstIndex > -1) {
			break;
		}
	}
	int visibleRows = height / itemSize; // Calculate visible rows based on how tall each item is and how tall the view is
	int lastIndex = firstIndex + (visibleRows * imagesPerRow) - 1; // Each row has imagesPerRow images

	std::cout << "Detected scroll starting at " << firstIndex << " to " << lastIndex << std::endl;

	loadImagesInView(firstIndex, lastIndex); // Load all iamges from firstIndex to lastIndex into the scroll view
}
