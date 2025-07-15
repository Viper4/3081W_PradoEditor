#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_PradoEditor.h"

class PradoEditor : public QMainWindow
{
    Q_OBJECT

public:
    PradoEditor(QWidget *parent = nullptr);
    ~PradoEditor();

private:
    Ui::PradoEditorClass ui;
};

