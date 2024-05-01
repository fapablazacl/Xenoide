
#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    xenoide::MainWindow mainWindow;
    mainWindow.setWindowTitle("Xenoide");
    mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}
