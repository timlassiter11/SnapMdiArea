#include "snapmdiarea.h"
#include "windowselector.h"

#include <QApplication>
#include <QChildEvent>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QEvent>
#include <QMdiSubWindow>
#include <QRubberBand>
#include <QHBoxLayout>

#include <QDebug>

SnapMdiArea::SnapMdiArea(QWidget *parent) :
    QMdiArea(parent),
    m_autoSnap(true),
    m_secondSnap(true),
    m_snapMargins(QMargins(30, 30, 30, 30)),
    m_snapRect(QRect()),
    m_rubberBand(new QRubberBand(QRubberBand::Rectangle, this)),
    m_selectorOverlay(new QWidget(this)),
    m_windowSelector(new WindowSelector(m_selectorOverlay))
{
    m_rubberBand->hide();
    m_selectorOverlay->hide();
    m_selectorOverlay->setObjectName("overlayWidget");
    m_selectorOverlay->installEventFilter(this);

    this->setStyleSheet(
                "#overlayWidget {\n"
                "   background-color: rgba(0, 0, 0, 127);\n"
                "}\n"
                "WindowSelector {\n"
                "   border: 2px solid grey;\n"
                "   border-radius: 8px;\n"
                "   background-color: rgba(0, 0, 0, 127);\n"
                "}\n"
                "WindowSelector WindowWidget QLabel {\n"
                "   color: white;\n"
                "   font-size: 12px;\n"
                "   font-weight: bold;\n"
                "}\n"
                "WindowSelector WindowWidget {\n"
                "   border-radius: 5px;\n"
                "}\n"
                "WindowSelector WindowWidget::hover {\n"
                "   background-color: rgba(127, 127, 127, 127);\n"
                "}\n"
    );
    m_windowSelector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    layout->addWidget(this->m_windowSelector);
    layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));
    this->m_selectorOverlay->setLayout(layout);
    connect(this->m_windowSelector, &WindowSelector::windowSelected, this, &SnapMdiArea::windowSelected);
}

ThumbnailType SnapMdiArea::thumbnailType() const
{
    return this->m_windowSelector->thumbnailType();
}

void SnapMdiArea::setThumbnailType(ThumbnailType thumbnailType)
{
    this->m_windowSelector->setThumbnailType(thumbnailType);
}

QMargins SnapMdiArea::snapMargins() const
{
    return this->m_snapMargins;
}

void SnapMdiArea::setSnapMargins(QMargins margins)
{
    this->m_snapMargins = margins;
}

bool SnapMdiArea::autoSnap() const
{
    return this->m_autoSnap;
}

void SnapMdiArea::setAutoSnap(bool enable)
{
    this->m_autoSnap = enable;
}

bool SnapMdiArea::secondSnap() const
{
    return this->m_secondSnap;
}

void SnapMdiArea::setSecondSnap(bool enable)
{
    this->m_secondSnap = enable;
}

bool SnapMdiArea::scaleThumbnail() const
{
    return this->m_windowSelector->scaleThumbnail();
}

void SnapMdiArea::setScaleThumbnail(bool enable)
{
    this->m_windowSelector->setScaleThumbnail(enable);
}

void SnapMdiArea::cancelSecondSnap()
{
    this->m_rubberBand->hide();
    this->m_selectorOverlay->hide();
}

void SnapMdiArea::windowSelected(QMdiSubWindow *window = nullptr)
{
    if (window)
    {
        window->showNormal();
        window->setGeometry(this->m_snapRect);
        this->setActiveSubWindow(window);
    }
    this->cancelSecondSnap();
}

void SnapMdiArea::childEvent(QChildEvent *childEvent)
{
    QObject *child = childEvent->child();
    if (QMdiSubWindow *sw = qobject_cast<QMdiSubWindow *>(child))
    {
        if (childEvent->added())
        {
            child->installEventFilter(this);
        }
        else if (childEvent->removed())
        {
            child->removeEventFilter(this);
        }
    }
    return QMdiArea::childEvent(childEvent);
}

void SnapMdiArea::resizeEvent(QResizeEvent *resizeEvent)
{
    this->cancelSecondSnap();
    QSize oldSize = resizeEvent->oldSize();
    QSize newSize = resizeEvent->size();

    int halfWidth = newSize.width() / 2;
    int halfHeight = newSize.height() / 2;
    QHash<QMdiSubWindow *, QRect> geometrys;
    // Iterates through all of the windows and resizes
    // any windows that appear to be snapped to a side.
    const QList<QMdiSubWindow *> swList = this->subWindowList();
    for (QMdiSubWindow *sw : swList)
    {
        // Ignore any maximized or minimized windows
        if (sw->isMaximized() || sw->isShaded())
        {
            continue;
        }

        QRect rect = sw->geometry();
        // If the window starts at 0 and is the same height as
        // the viewport we can assume that it's snapped
        // either to the left or right side.
        if (rect.top() == 0 && rect.height() == oldSize.height())
        {
            // Snapped to the left side.
            if (rect.left() == 0)
            {
                rect = QRect(0, 0, halfWidth, newSize.height());
            }
            // Snapped to the right side
            else
            {
                rect = QRect(halfWidth, 0, halfWidth, newSize.height());
            }
        }
        // Left starts at 0 and the width is equal to the viewport
        // means the window is likely snapped to the top or bottom.
        else if (rect.left() == 0 && rect.width() == oldSize.width())
        {
            if (rect.top() == 0)
            {
                rect = QRect(0, 0, newSize.width(), halfHeight);
            }
            else
            {
                rect = QRect(0, halfHeight, newSize.width(), halfHeight);
            }
        }

        // Store the geometry if it was changed so we
        // can resize them after we process the event.
        if (rect != sw->geometry())
        {
            geometrys[sw] = rect;
        }
    }

    // Process the event first so our sizes take precedence.
    // This needs to be called if we want tiling to work still.
    // If windows are tiled this will handle the resizing.
    QMdiArea::resizeEvent(resizeEvent);

    // Now update the geometry of our windows.
    QHash<QMdiSubWindow *, QRect>::iterator i;
    for (i = geometrys.begin(); i != geometrys.end(); ++i)
    {
        i.key()->setGeometry(i.value());
    }
}

void SnapMdiArea::keyReleaseEvent(QKeyEvent *event)
{
    // Cancel the second snap selector
    // if the user presses escape.
    if (event->key() == Qt::Key_Escape)
    {
        this->cancelSecondSnap();
    }

    return QMdiArea::keyReleaseEvent(event);
}

bool SnapMdiArea::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->m_selectorOverlay)
    {
        if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
                this->cancelSecondSnap();
        }
    }
    if (QMdiSubWindow *sw = qobject_cast<QMdiSubWindow *>(watched))
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->cancelSecondSnap();
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->buttons() == Qt::LeftButton)
            {
                // Height of the window decorator used for dragging.
                int handleHeight = sw->height() - sw->widget()->height();
                // Height of the resize handle.
                int topMargin = handleHeight - sw->contentsMargins().top();
                // If the user grabs the window for drag but not for resize.
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                QPointF pos = mouseEvent->position();
#else
                QPointF pos = mouseEvent->pos();
#endif
                if (pos.y() < handleHeight && pos.y() >= topMargin)
                {
                    sw->setProperty("dragStart", sw->mapToParent(mouseEvent->pos()));
                }
            }
        }
        else if (event->type() == QEvent::MouseMove)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->buttons() & Qt::LeftButton)
            {
                // TODO: Its possible for this to return true if the drag started at (0, 0).
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                if (sw->property("dragStart").canConvert(QMetaType(QMetaType::QPoint)))
#else
                if (sw->property("dragStart").canConvert(QVariant::Point))
#endif
                {
                    QPoint dragStart = sw->property("dragStart").toPoint();
                    QPoint dPos = sw->mapToParent(mouseEvent->pos()) - dragStart;
                    if (dPos.manhattanLength() >= QApplication::startDragDistance())
                    {
                        sw->setProperty("dragging", true);
                    }
                }

                QPoint mPos = sw->mapToParent(mouseEvent->pos());
                if (sw->property("dragging").toBool())
                {
                    this->m_snapRect = QRect();
                    if (mPos.y() < this->m_snapMargins.top())
                    {
                        this->m_snapRect.setTopLeft(QPoint(0, 0));
                        this->m_snapRect.setSize(this->size());
                    }
                    else if (mPos.x() < this->m_snapMargins.left())
                    {
                        this->m_snapRect.setTopLeft(QPoint(0, 0));
                        this->m_snapRect.setWidth(this->width() / 2);
                        this->m_snapRect.setHeight(this->height());
                    }
                    else if (mPos.x() > this->width() - this->m_snapMargins.right())
                    {
                        int w = this->width() / 2;
                        this->m_snapRect.setTopLeft(QPoint(w, 0));
                        this->m_snapRect.setWidth(w);
                        this->m_snapRect.setHeight(this->height());
                    }
                    else if (mPos.y() > this->height() - this->m_snapMargins.bottom())
                    {
                        int h = this->height() / 2;
                        this->m_snapRect.setTopLeft(QPoint(0, h));
                        this->m_snapRect.setWidth(this->width());
                        this->m_snapRect.setHeight(h);
                    }

                    if (!this->m_snapRect.isEmpty())
                    {
                        this->m_rubberBand->setGeometry(this->m_snapRect.adjusted(5,5,-5,-5));
                        this->m_rubberBand->show();
                    }
                    else
                    {
                        this->m_rubberBand->hide();
                    }
                }
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                if (sw->property("dragging").toBool())
                {
                    if (!this->m_snapRect.isEmpty())
                    {
                        if (this->m_snapRect.size() == this->size())
                        {
                            sw->showMaximized();
                        }
                        else
                        {
                            sw->setGeometry(this->m_snapRect);
                            QList<QMdiSubWindow *> swList = this->subWindowList();
                            swList.removeAll(sw);

                            // A window can either be snapped left, right or bottom. Top maximizes.
                            // Snapped left is the only case where the second window won't start a 0, 0.
                            if (this->m_snapRect.left() == 0 && this->m_snapRect.height() == this->height())
                            {
                                this->m_snapRect.moveLeft(this->m_snapRect.width());
                            }
                            else
                            {
                                this->m_snapRect.moveTopLeft(QPoint(0, 0));
                            }

                            // If there is only one other window
                            // there's no need to show the selector.
                            // Just snap the only other window.
                            if (swList.length() == 1)
                            {
                                if (this->m_autoSnap)
                                {
                                    swList[0]->setGeometry(this->m_snapRect);
                                }
                            }
                            else if (swList.length() > 1)
                            {
                                if (this->m_secondSnap)
                                {
                                    this->m_windowSelector->setWindows(swList);
                                    this->m_selectorOverlay->show();
                                    this->m_selectorOverlay->setGeometry(this->m_snapRect);
                                }
                            }
                        }
                    }
                }
                this->m_rubberBand->hide();
                sw->setProperty("dragging", false);
                sw->setProperty("dragStart", QVariant());
            }
        }
    }
    return false;
}
