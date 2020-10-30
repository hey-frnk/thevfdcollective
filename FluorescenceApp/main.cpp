#include "fluorescenceapp.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    qDebug() << QFontDatabase::addApplicationFont(QStringLiteral(":/Resources/RalewayBlack.ttf"));
    qDebug() << QFontDatabase::addApplicationFont(QStringLiteral(":/Resources/RalewayBold.ttf"));
    qDebug() << QFontDatabase::addApplicationFont(QStringLiteral(":/Resources/RalewayMedium.ttf"));

    FluorescenceApp w;

    // Fix size and disable maximizing (update: freely resizable now!)
    // w.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    #ifndef Q_OS_IOS
    w.setFixedSize(600, 600);
    #endif

    w.show();

    return a.exec();
}
