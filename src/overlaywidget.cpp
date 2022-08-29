#include "overlaywidget.h"

#include <QPainter>
#include <QMouseEvent>

OverlayWidget::OverlayWidget(QWidget *parent)
    : QFrame{parent}
{
    setPalette(Qt::transparent);
    setAttribute(Qt::WA_NoSystemBackground);
}

void OverlayWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(QColor(0, 0, 0, 150)));
    painter.setOpacity(20);
    painter.setPen(Qt::NoPen);
    painter.drawRect(this->rect());
}

void OverlayWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
    QFrame::mouseReleaseEvent(event);
}
