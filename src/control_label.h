#ifndef CONTROL_LABEL_H
#define CONTROL_LABEL_H
#include "common.h"

#include <QLabel>
class QMouseEvent;


KUPLAYER_NAMESPACE_BEGIN //namespace begin

class ControlLabel final : public QLabel
{
    Q_OBJECT
signals:
    void clicked();
public:
    ControlLabel(const QString &name, QWidget *parent = 0);
protected:
    void mousePressEvent(QMouseEvent *);
};

KUPLAYER_NAMESPACE_END //namespace end

#endif // CONTROL_LABEL_H
