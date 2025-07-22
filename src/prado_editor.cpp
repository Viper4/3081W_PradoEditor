#include <prado_editor.h>
#include <iostream>
#include <image_scroll_gallery.h>
#include <managers.h>
#include <opencv2/opencv.hpp>

PradoEditor::PradoEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->resize(400, 800);
    this->setFixedSize(400, 800); // or whatever size you want

    // Initialize the stringToFilterType map
    stringToFilterType = {
		{"Grayscale", ArtworkManager::FilterType::GRAYSCALE},
		{"Blur", ArtworkManager::FilterType::BLUR},
		{"Invert", ArtworkManager::FilterType::INVERT},
		{"HSV", ArtworkManager::FilterType::HSV},
		{"Saturate", ArtworkManager::FilterType::SATURATE},
	};

    // Add items to the filter dropdown
    int i = 0;
    for (auto& pair : stringToFilterType) {
		ui.filterDropdown->insertItem(i, pair.first.c_str());
        i++;
    }

    this->gallery = new ImageScrollGallery(ui.galleryListView, 125, 150);

    setHomeBtnVisible(false);
    
    ui.retranslateUi(this);
}

PradoEditor::~PradoEditor()
{

}

void PradoEditor::setHomeBtnVisible(bool visible) {
    ui.btnHome->setEnabled(visible);
	ui.btnHome->setVisible(visible);
}

void PradoEditor::on_btnHome_clicked() {
    // Contributors: Lucas Giebler
	// Purpose: Opens the home page
    // Parameters:
    // Return Value: void
    // Limitations:
    std::cout << "Home Button Clicked" << std::endl;
    ui.stackedWidget->setCurrentIndex(0);
	setHomeBtnVisible(false);
}

void PradoEditor::on_btnExpand_clicked() {
    // Contributors: Lucas Giebler
	// Purpose: Opens the details page and updates the values on the details page
	// Parameters: 
	// Return Value: void
    // Limitations:
    std::cout << "Expand Button Clicked" << std::endl;
    if (gallery->selectedItem == nullptr) {
        return;
    }

    std::string artworkId = gallery->selectedItem->data(Qt::UserRole).toString().toStdString();
    if (ImageScrollGallery::GlobalGallery.count(artworkId) == 0) {
        std::cout << "Artwork ID '" << artworkId << "' not found." << std::endl;
        return;
    }
    ui.stackedWidget->setCurrentIndex(1); // Switch to the details page
    setHomeBtnVisible(true);
    Artwork art = ImageScrollGallery::GlobalGallery.at(artworkId); // Get the Artwork object
    QIcon icon = gallery->selectedItem->data(Qt::DecorationRole).value<QIcon>(); // Get the artwork's icon

    // Update the details page
    ui.labelArtworkImage->setPixmap(icon.pixmap(300, 300));
    ui.labelArtworkTitle->setText(art.metadata.at("work_title").c_str());
    ui.labelArtworkDescription->setText(art.metadata.at("description").c_str());
}

void PradoEditor::on_btnSearch_clicked() {
    // Contributors: Lucas Giebler
	// Purpose: Opens the search page to prompt the user to enter a search term for artworks
	// Parameters:
	// Return Value: void
	// Limitations:
    std::cout << "Search Button Clicked" << std::endl;
}

void PradoEditor::on_btnFavorites_clicked() {
    // Contributors: Lucas Giebler
	// Purpose: Opens the favorites page and loads favorited artworks saved to the device
    // Parameters:
    // Return Value: void
    // Limitations:
    std::cout << "Favorites Button Clicked" << std::endl;
}

void PradoEditor::on_btnEditImage_clicked() {
    // Contributors: Lucas Giebler
	// Purpose: Opens the edit page and updates the values on the edit page
	// Parameters: 
    // Return Value: void
    // Limitations: 
	std::cout << "Edit Image Button Clicked" << std::endl;
    ui.stackedWidget->setCurrentIndex(2); // Switch to the edit page
    setHomeBtnVisible(true);
    QIcon icon = gallery->selectedItem->data(Qt::DecorationRole).value<QIcon>(); // Get the artwork's icon
    ui.labelPreview->setPixmap(icon.pixmap(300, 300));
}

void PradoEditor::on_filterDropdown_currentIndexChanged(int index) {
    // Contributors: Lucas Giebler
    // Purpose: Selects a filter from the dropdown menu
    // Parameters: 
    // Return Value: void
    // Limitations: 
    std::cout << "Filter Option Selected" << std::endl;
    selectedFilter = ui.filterDropdown->itemText(index).toStdString();
}

void PradoEditor::on_btnApplyFilter_clicked() {
    // Contributors: Lucas Giebler
    // Purpose: Applies the selected filter to the preview image
    // Parameters: 
    // Return Value: void
    // Limitations: 
    std::cout << "Apply Filter Button Clicked" << std::endl;
    std::string artworkId = gallery->selectedItem->data(Qt::UserRole).toString().toStdString();
	cv::Mat previewImage = ArtworkManager::pixmapToMat(ui.labelPreview->pixmap());
    std::cout << "previewImage is of type " << previewImage.type() << std::endl;
    if (stringToFilterType.count(selectedFilter) == 0) {
		std::cout << "Selected filter '" << selectedFilter << "' not found." << std::endl;
        return;
    }
    cv::Mat filteredImage = ArtworkManager::applyFilter(previewImage, stringToFilterType.at(selectedFilter));
    std::cout << "filteredImage is of type " << previewImage.type() << std::endl;
    QPixmap pixmap = ArtworkManager::matToPixmap(filteredImage);
    ui.labelPreview->setPixmap(pixmap);
	ui.labelPreview->setMinimumSize(300, 300);
}

void PradoEditor::on_btnReset_clicked() {
    // Contributors: Lucas Giebler
	// Purpose: Resets the preview image to the original image with no manipulations
	// Parameters: 
	// Return Value: void
	// Limitations: 
	std::cout << "Reset Button Clicked" << std::endl;
	std::string artworkId = gallery->selectedItem->data(Qt::UserRole).toString().toStdString();
    ui.labelPreview->setPixmap(gallery->selectedItem->data(Qt::DecorationRole).value<QIcon>().pixmap(300, 300));
}