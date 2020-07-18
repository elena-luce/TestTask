#include <QApplication>

#include "mainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindow     client("localhost", 2323);

    client.show();

    return app.exec();
}
