#include "fluorescenceapp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FluorescenceApp w;

    // Fix size and disable maximizing
    w.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    w.setFixedSize(760, 500);

    w.show();
    return a.exec();
}
