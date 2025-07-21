#pragma once

#include <QtWidgets/QMainWindow>
#include <ui_PradoEditor.h>
#include <image_scroll_gallery.h>

class PradoEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit PradoEditor(QWidget *parent = nullptr);
    ~PradoEditor();

private slots:
    void on_pushButton_clicked();

private:
    Ui::PradoEditor ui;

    ImageScrollGallery* gallery;
};
