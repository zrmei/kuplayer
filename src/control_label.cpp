#include "control_label.h"

USR_NAMESPACE_KUPLAYER

ControlLabel::ControlLabel(const QString &name, QWidget *parent)
    : QLabel(parent)
{
    setObjectName(name.split("/").last());
    setPixmap(QPixmap(name).scaled(26,26));
    setFixedSize(30,30);
}
void ControlLabel::mousePressEvent(QMouseEvent *ev)
{
    QLabel::mousePressEvent(ev);
    emit clicked();
}
