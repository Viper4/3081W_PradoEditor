#pragma once

#include <QtWidgets/QMainWindow>
#include <ui_PradoEditor.h>
#include <image_scroll_gallery.h>
#include <managers.h>

class PradoEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit PradoEditor(QWidget *parent = nullptr);
    ~PradoEditor();

private slots:
    void on_btnHome_clicked();
    void on_btnExpand_clicked();
    void on_btnSearch_clicked();
    void on_btnFavorites_clicked();
    void on_btnEditImage_clicked();
    void on_filterDropdown_currentIndexChanged(int index);
    void on_btnApplyFilter_clicked();
    void on_btnReset_clicked();

private:
    Ui::PradoEditor ui;
    ImageScrollGallery* gallery;
    std::string selectedFilter;
    std::unordered_map<std::string, ArtworkManager::FilterType> stringToFilterType;

    void setHomeBtnVisible(bool visible);
};
