#ifndef MOVABLELABEL_H
#define MOVABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class MovableLabel : public QLabel {
    Q_OBJECT

public:
    explicit MovableLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~MovableLabel();

signals:
    void clicked();
    void mouseMoved(const QPoint&);

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent *event);

private:
    // QPoint offset;
};

#endif // MOVABLELABEL_H
