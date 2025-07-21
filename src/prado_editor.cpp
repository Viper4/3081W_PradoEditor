#include "prado_editor.h"
#include <iostream>
#include <image_scroll_gallery.h>

PradoEditor::PradoEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->gallery = new ImageScrollGallery(ui.galleryListView, 125, 150);
    
    ui.retranslateUi(this);
}

PradoEditor::~PradoEditor()
{

}

void PradoEditor::on_pushButton_clicked() {
    std::cout << "Button Clicked" << std::endl;
}
