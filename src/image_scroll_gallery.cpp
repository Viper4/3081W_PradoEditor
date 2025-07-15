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
	//             float bufferX
	//             float bufferY
	//             int imagesPerRow
	// Return Value: ImageScrollGallery
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

	// Calculate and fix the width of the list view to simulate fixed columns
	//this->listView->setFixedWidth(imagesPerRow * itemSize + (imagesPerRow - 1) * this->listView->spacing());

	// Add onScroll function as listener to scroll bar
	connect(listView->verticalScrollBar(), &QScrollBar::valueChanged, this, &ImageScrollGallery::onScroll);

	// Load placeholder images into all cells in the scroll view
	QStandardItemModel* model = new QStandardItemModel(this);
	this->listView->setModel(model);
	QPixmap pixmap("C:\\Users\\vpr16\\Documents\\Random\\robloxDefault.png");
	for (int i = 0; i < numTotalImages; i++) {
		QStandardItem* item = new QStandardItem(QIcon(pixmap), "Placeholder " + QString::number(i));
		item->setTextAlignment(Qt::AlignCenter);
		model->appendRow(item);
	}
}



void ImageScrollGallery::loadImagesInView(int firstIndex, int lastIndex) {
	// Contributors: Lucas Giebler
	// Purpose: Loads images into the scroll view
	// Parameters: int firstIndex
	//             int lastIndex
	// Return Value: void
	// -------------------
	for (int i = firstIndex; i < lastIndex + 1; i++) {
		std::string artworkId = std::to_string(i);
		cv::Mat image = ImageCache::getCachedImage(artworkId);
		if (image.empty()) {
			continue;
		}
		QPixmap pixmap = fitPixmapToSize(ImageCache::matToQPixmap(image), iconSize, iconSize, true);
		QStandardItem* item = new QStandardItem(QIcon(pixmap), "Photo " + QString::number(i));
		item->setTextAlignment(Qt::AlignCenter);
		QModelIndex index = listView->model()->index(i, 0);
		listView->model()->setData(index, pixmap, Qt::DecorationRole);
	}
}

QPixmap ImageScrollGallery::fitPixmapToSize(const QPixmap& pixmap, int width, int height, bool keepAspectRatio) {
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
		scaled = pixmap.scaled(width, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	}
	return scaled;
}

void ImageScrollGallery::onScroll(int value) {
	// Contributors: Lucas Giebler
	// Purpose: Runs every time the scroll view is scrolled and updates the images in the view
	// Parameters: 
	// Return Value: void
	// -------------------
	//QPoint topLeft = listView->viewport()->rect().topLeft();
	//QPoint bottomRight = listView->viewport()->rect().bottomRight();

	//int firstIndex = listView->indexAt(topLeft).row();
	//int lastIndex = listView->indexAt(bottomRight).row();
	int firstIndex = -1;
	for (int i = 0; i < itemSize; i++) {
		firstIndex = listView->indexAt(QPoint(posX + bufferX, posY + bufferY + i)).row();
		if (firstIndex != -1) {
			break;
		}
	}
	int visibleRows = height / itemSize;
	int lastIndex = firstIndex + (visibleRows * imagesPerRow) - 1;

	std::cout << "Detected scroll starting at " << firstIndex << " to " << lastIndex << std::endl;

	loadImagesInView(firstIndex, lastIndex);
}
