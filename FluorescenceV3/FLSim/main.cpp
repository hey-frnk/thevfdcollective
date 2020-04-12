#include "fluorescencesimulator.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FluorescenceSimulator w;
    w.show();
    return a.exec();
}
