#include <image_scroll_gallery.h>
#include <iostream>
#include <QScrollBar>
#include <QObject>
#include <QListView>
#include <QStandardItemModel>
#include <QStringListModel>
#include <image_cache.h>
#include <managers.h>
#include <future>
#include <debug.h>
#include <chrono>

std::unordered_map<std::string, Artwork> ImageScrollGallery::GlobalGallery;

ImageScrollGallery::ImageScrollGallery(QWidget* parent, float x, float y, float width, float height, float bufferX, float bufferY, int imagesPerRow, int iconSize, int itemSize) {
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
	// Limitations: Does no checking for valid parameters so negative values will cause UI to not show
	// -------------------
	this->posX = x;
	this->posY = y;
	this->width = width;
	this->height = height;
	this->bufferX = bufferX;
	this->bufferY = bufferY;
	this->numTotalImages = GlobalGallery.size();
	this->imagesPerRow = imagesPerRow;
	this->numRows = numTotalImages / imagesPerRow;
	this->visibleRows = height / iconSize;

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

	// Add onItemClicked function as listener to list view
	connect(listView, &QListView::clicked, this, &ImageScrollGallery::onItemClicked);

	// Load placeholder images into all cells in the scroll view
	QStandardItemModel* model = new QStandardItemModel(this);
	this->listView->setModel(model);
	QPixmap pixmap("C:\\Users\\vpr16\\Documents\\Random\\Absolute Cinema.jpg");
	this->placeholderIcon = QIcon(pixmap);
	for (auto itr = GlobalGallery.begin(); itr != GlobalGallery.end(); itr++) {
		QStandardItem* item = new QStandardItem(placeholderIcon, QString::fromStdString(itr->first));
		item->setTextAlignment(Qt::AlignCenter);
		item->setEditable(false);
		model->appendRow(item);
	}

	// Load initial images into the scroll view
	previousFirstIndex = -999;
	onScroll(0);
}

void ImageScrollGallery::loadImagesInView(int firstIndex, int lastIndex) {
	// Contributors: Lucas Giebler
	// Purpose: Loads images into the scroll view between firstIndex and lastIndex
	// Parameters: int firstIndex - The index of the first image to load
	//             int lastIndex - The index of the last image to load
	// Return Value: void
	// Limitations: 
	// -------------------
	if (firstIndex < 0 || firstIndex >= numTotalImages || lastIndex < 0) {
		return;
	}
	lastIndex = std::min(lastIndex, numTotalImages - 1);

	QStandardItemModel* model = qobject_cast<QStandardItemModel*>(listView->model());
	int previousLastIndex = previousFirstIndex + (visibleRows * imagesPerRow) - 1; // Each row has imagesPerRow images

	int unloadFirstIndex = previousFirstIndex;
	if (previousFirstIndex >= firstIndex && previousFirstIndex <= lastIndex) {
		unloadFirstIndex = lastIndex + 1;
	}

	int unloadLastIndex = previousLastIndex;
	if (previousLastIndex >= firstIndex && previousLastIndex <= lastIndex) {
		unloadLastIndex = firstIndex - 1;
	}

	int loadFirstIndex = firstIndex;
	if (firstIndex >= previousFirstIndex && firstIndex <= previousLastIndex) {
		loadFirstIndex = previousLastIndex + 1;
	}

	int loadLastIndex = lastIndex;
	if (lastIndex >= previousFirstIndex && lastIndex <= previousLastIndex) {
		loadLastIndex = previousFirstIndex - 1;
	}

	// Replace previously loaded images to save memory
	for (int i = unloadFirstIndex; i <= unloadLastIndex; i++) {
		QStandardItem* item = model->item(i, 0);
		if (item) {
			item->setIcon(placeholderIcon); // Replace the icon
		}
	}
	previousFirstIndex = firstIndex;

	std::vector<std::future<cv::Mat>> futures;
	for (int i = loadFirstIndex; i <= loadLastIndex; i++) {
		std::string artworkId = model->item(i, 0)->text().toStdString();
		futures.push_back(std::async(std::launch::async, ArtworkManager::getImage, artworkId));
	}
	for (int i = loadFirstIndex; i <= loadLastIndex; i++) {
		// TODO: Somehow move this to a separate thread to prevent curl requests from freezing the program
		auto start = std::chrono::high_resolution_clock::now();
		cv::Mat image = futures[i - loadFirstIndex].get();
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::cout << "get took " << duration << "ms" << std::endl;

		if (image.empty()) {
			model->setData(model->index(i, 0), placeholderIcon, Qt::DecorationRole);
			continue; // Skip empty images
		}
		int cropSize = std::min(image.rows, image.cols);
		// Update item with image
		QPixmap pixmap = ImageCache::matToQPixmap(image);
		//QPixmap pixmap = ImageCache::matToQPixmap(ArtworkManager::cropImageCentered(image, cropSize, cropSize));

		model->setData(model->index(i, 0), QIcon(pixmap), Qt::DecorationRole);
	}
}

void ImageScrollGallery::onScroll(int value) {
	// Contributors: Lucas Giebler
	// Purpose: Runs every time the scroll view is scrolled and updates the images in the view
	// Parameters: int value - The value of the scroll bar (unused)
	// Return Value: void
	// Limitations: 
	// -------------------
	// Calculate index of the first image at the top left of the scroll view
	int firstRow = value / itemSize;
	int firstIndex = firstRow * imagesPerRow;
	int lastIndex = firstIndex + (visibleRows * imagesPerRow) - 1; // Each row has imagesPerRow images

	std::cout << "Detected scroll " << value << " starting at " << firstIndex << " to " << lastIndex << std::endl;
	loadImagesInView(firstIndex, lastIndex); // Load all images from firstIndex to lastIndex into the scroll view
}

void ImageScrollGallery::onItemClicked(const QModelIndex& index) {
	// Contributors: Lucas Giebler
	// Purpose: Expands the image when an item is clicked and displays the description
	// Parameters: QModelIndex index - The index of the item that was clicked
	// Return Value: void
	// Limitations: 
	// -------------------
	// Get the index of the item that was clicked
	int itemIndex = index.row();
	if (itemIndex < 0 || itemIndex >= numTotalImages) {
		std::cout << "Clicked item index " << itemIndex << " out of range" << std::endl;
		return;
	}
	std::string artworkId = listView->model()->data(index, Qt::DisplayRole).toString().toStdString();
	std::cout << "Item " << itemIndex << " clicked with id " << artworkId << std::endl;
	Debug::printArtwork(ArtworkManager::getArtworkByID(artworkId), true);
	
	// TODO: Expand the item image and display description
	QImage image = listView->model()->data(index, Qt::DecorationRole).value<QImage>();
}
