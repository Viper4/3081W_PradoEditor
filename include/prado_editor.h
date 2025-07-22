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
    void on_btnBack_clicked();
    void on_btnForward_clicked();
    void on_btnSave_clicked();
    void on_btnDoSearch_clicked();

private:
    Ui::PradoEditor ui;
    ImageScrollGallery* gallery;
    std::string selectedFilter;
    std::unordered_map<std::string, ArtworkManager::FilterType> stringToFilterType;
    std::vector<int> backPageStack;
    std::vector<int> forwardPageStack;

    void setHomeBtnVisible(bool visible);
    void changePageIndex(int index, bool addToStack);
    void changePage(QWidget* page, bool addToStack);
};
