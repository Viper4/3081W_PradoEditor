#include <image_scroll_gallery.h>
#include <iostream>
#include <QScrollBar>
#include <QObject>
#include <QListView>
#include <QStandardItemModel>
#include <QStringListModel>

ImageScrollGallery::ImageScrollGallery(QWidget* parent, float x, float y, float width, float height, float bufferX, float bufferY, int imagesPerRow) {
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

	this->listView = new QListView(parent);
	this->listView->setGeometry(x + bufferX, y + bufferY, width, height);
	this->listView->setViewMode(QListView::IconMode);
	this->listView->setIconSize(QSize(150, 150));
	this->listView->setMovement(QListView::Static);
	this->listView->setFlow(QListView::LeftToRight);
	QScrollBar* scrollBar = this->listView->verticalScrollBar();
	connect(listView->verticalScrollBar(), &QScrollBar::valueChanged, this, &ImageScrollGallery::onScroll);

	QStandardItemModel* model = new QStandardItemModel(this);
	this->listView->setModel(model);
	QPixmap pixmap("C:\\Users\\vpr16\\Documents\\Random\\robloxDefault.png");
	for (int i = 0; i < 10; i++) {
		QStandardItem* item = new QStandardItem(QIcon(pixmap), "Photo " + QString::number(i));
		model->appendRow(item);
	}

	/*QStringListModel* model = new QStringListModel(this);
	this->listView->setModel(model);
	QStringList items;
	items << "Image 1" << "Image 2" << "Image 3" << "Image 4" << "Image 5" << "Image 6" << "Image 7" << "Image 8" << "Image 9" << "Image 10";
	model->setStringList(items);*/

	this->posX = x;
	this->posY = y;
	this->width = width;
	this->height = height;
	this->bufferX = bufferX;
	this->bufferY = bufferY;
	this->imagesPerRow = imagesPerRow;
	this->numRows = 0;
	this->numTotalImages = 0;
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

void ImageScrollGallery::onScroll(int value) {
	// Contributors: Lucas Giebler
	// Purpose: Runs every time the scroll view is scrolled and updates the images in the view
	// Parameters: 
	// Return Value: void

	std::cout << "Detected scroll" << std::endl;

	QPoint topLeft = listView->viewport()->mapToGlobal(listView->viewport()->rect().topLeft());
	QPoint bottomRight = listView->viewport()->mapToGlobal(listView->viewport()->rect().bottomRight());
	int firstIndex = listView->indexAt(topLeft).row() + 1;
	int lastIndex = listView->indexAt(bottomRight).row() + 1;
}
