#include "PradoEditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PradoEditor window;
    window.show();
    return app.exec();
}
