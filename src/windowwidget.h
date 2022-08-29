#ifndef WINDOWWIDGET_H
#define WINDOWWIDGET_H

#include <QFrame>

enum class ThumbnailType
{
    None,
    WindowIcon,
    WidgetContents
};

class QMdiSubWindow;

class WindowWidget : public QFrame
{
    Q_OBJECT
public:
    explicit WindowWidget(QMdiSubWindow *window, ThumbnailType thumbnailType = ThumbnailType::None, bool scaleThumbnail = false, QWidget *parent = nullptr);

    QMdiSubWindow *window() const;

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QMdiSubWindow *m_window;

signals:
    void clicked(QMdiSubWindow *);
};

#endif // WINDOWWIDGET_H
