#include <QMouseEvent>
#include "movablelabel.h"

MovableLabel::MovableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent, f) {
}

MovableLabel::~MovableLabel() {}

void MovableLabel::mouseMoveEvent(QMouseEvent *event)
{
    QPoint relative_pos = mapToParent(event->pos()) - QPoint(this->width() / 2, this->height() / 2);
    emit mouseMoved(relative_pos);
    if(event->buttons() & Qt::LeftButton) {
        this->move(relative_pos);
    }
}

void MovableLabel::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    // offset = event->pos();
}
