#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //Tasks for Main
    // The data will be cleaned, with only paintings that have the attribute of year
    //1. With this data, we have to first initialize the image cache with 50 or however many image structs
    //2. Obtain the jpg or png file from these structs and display them
    //3. Update the image cache based on the index of scroll and continue to display new paintings as the user scrolls
    //4. On event of a click of an image display the options such as edit, reset, or display description
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
