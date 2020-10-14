#include "infobox.h"
#include "ui_infobox.h"
#include <QDesktopServices>
#include <QUrl>

infobox::infobox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::infobox)
{
    ui->setupUi(this);
}

infobox::~infobox()
{
    delete ui;
}

void infobox::on_info_1_clicked()
{
    // Just take you to the support
    QDesktopServices::openUrl(QUrl("https://www.thevfdcollective.com/support"));
}

void infobox::on_info_2_clicked()
{
    on_info_1_clicked();
}

void infobox::on_info_3_clicked()
{
    on_info_1_clicked();
}

void infobox::on_info_screen_clicked()
{
    this->close();
    return;
}
