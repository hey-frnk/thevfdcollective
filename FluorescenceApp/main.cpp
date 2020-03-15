#include "fluorescenceapp.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FluorescenceApp w;
    w.show();
    return a.exec();
}
