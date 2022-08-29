#include "windowselector.h"

#include <QMdiSubWindow>
#include <QHBoxLayout>
#include <QResizeEvent>

WindowSelector::WindowSelector(QWidget *parent)
    : QFrame(parent),
      m_thumbnailType(ThumbnailType::None),
      m_scaleThumbnail(true)
{
    this->setLayout(new QHBoxLayout());
}

ThumbnailType WindowSelector::thumbnailType() const
{
    return this->m_thumbnailType;
}

void WindowSelector::setThumbnailType(ThumbnailType thumbnailType)
{
    if (thumbnailType != this->m_thumbnailType)
    {
        this->m_thumbnailType = thumbnailType;
        this->setWindows(this->m_windowList);
    }
}

bool WindowSelector::scaleThumbnail() const
{
    return this->m_scaleThumbnail;
}

void WindowSelector::setScaleThumbnail(bool enable)
{
    this->m_scaleThumbnail = enable;
    this->setWindows(this->m_windowList);
}

void WindowSelector::setWindows(const QList<QMdiSubWindow *> &windows)
{
    this->clearWindows();
    QHBoxLayout *layout = static_cast<QHBoxLayout *>(this->layout());
    for (QMdiSubWindow *window : windows)
    {
        WindowWidget *w = new WindowWidget(window, this->m_thumbnailType, this->m_scaleThumbnail);
        connect(w, &WindowWidget::clicked, this, &WindowSelector::windowSelected);
        w->setMaximumSize(200, 200);
        layout->addWidget(w);
    }
    this->m_windowList = QList<QMdiSubWindow *>(windows);
}

void WindowSelector::clearWindows()
{
    QLayoutItem *item;
    while ( (item = this->layout()->takeAt(0)))
    {
        item->widget()->deleteLater();
        delete item;
    }
}

void WindowSelector::resizeEvent(QResizeEvent *event)
{
    event->accept();


}
