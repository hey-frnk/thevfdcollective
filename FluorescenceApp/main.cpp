#include "fluorescenceapp.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << QFontDatabase::addApplicationFont(QStringLiteral(":/Resources/RalewayBlack.ttf"));
    qDebug() << QFontDatabase::addApplicationFont(QStringLiteral(":/Resources/RalewayBold.ttf"));
    qDebug() << QFontDatabase::addApplicationFont(QStringLiteral(":/Resources/RalewayMedium.ttf"));

    FluorescenceApp w;

    // Fix size and disable maximizing
    w.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    w.setFixedSize(760, 440);

    w.show();

    return a.exec();
}
