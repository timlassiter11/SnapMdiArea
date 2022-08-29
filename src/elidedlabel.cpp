#include "elidedlabel.h"

#include <QPainter>
#include <QStyleOptionFrame>

ElidedLabel::ElidedLabel(QWidget *parent)
    : QLabel(parent)
{}

ElidedLabel::ElidedLabel(const QString &text, QWidget *parent)
    : QLabel(text, parent)
{}

void ElidedLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter = QPainter(this);
    QStyleOptionFrame opt = QStyleOptionFrame();
    this->initStyleOption(&opt);
    this->style()->drawControl(QStyle::CE_ShapedFrame, &opt, &painter, this);
    int margin = this->margin();
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    int m = this->fontMetrics().horizontalAdvance('x') / 2 - margin;
#else
    int m = this->fontMetrics().width('x') / 2 - margin;
#endif
    QRect rect = this->contentsRect().adjusted(margin + m, margin, -(margin + m), -margin);
    QString elidedText = this->fontMetrics().elidedText(this->text(), Qt::ElideRight, rect.width());
    painter.drawText(rect, this->alignment(), elidedText);
}
