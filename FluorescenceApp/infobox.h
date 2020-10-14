#ifndef INFOBOX_H
#define INFOBOX_H

#include <QDialog>

namespace Ui {
class infobox;
}

class infobox : public QDialog
{
    Q_OBJECT

public:
    explicit infobox(QWidget *parent = nullptr);
    ~infobox();

private slots:
    void on_info_1_clicked();

    void on_info_2_clicked();

    void on_info_3_clicked();

    void on_info_screen_clicked();

private:
    Ui::infobox *ui;
};

#endif // INFOBOX_H
