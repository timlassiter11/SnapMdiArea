#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QFrame>

class OverlayWidget : public QFrame
{
    Q_OBJECT
public:
    explicit OverlayWidget(QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void clicked();

};

#endif // OVERLAYWIDGET_H
