#ifndef WINDOWSELECTOR_H
#define WINDOWSELECTOR_H

#include "windowwidget.h"

#include <QFrame>

class QMdiSubWindow;

class WindowSelector : public QFrame
{
    Q_OBJECT
public:
    explicit WindowSelector(QWidget *parent = nullptr);

    ThumbnailType thumbnailType() const;
    bool scaleThumbnail() const;

    void setWindows(const QList<QMdiSubWindow *> &windows);

public slots:
    void setThumbnailType(ThumbnailType thumbnailType);
    void setScaleThumbnail(bool enable);

    void clearWindows();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    QList<QMdiSubWindow *> m_windowList;
    ThumbnailType m_thumbnailType;
    bool m_scaleThumbnail;

signals:
    void windowSelected(QMdiSubWindow *);
};

#endif // WINDOWSELECTOR_H
