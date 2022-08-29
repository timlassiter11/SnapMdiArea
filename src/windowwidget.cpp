#include "windowwidget.h"

#include <QMdiSubWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QIcon>

WindowWidget::WindowWidget(QMdiSubWindow *window, ThumbnailType thumbnailType, bool scaleThumbnail, QWidget *parent)
    : QFrame(parent),
      m_window(window)
{
    QLabel *thumbnail = nullptr;
    switch (thumbnailType)
    {
        case ThumbnailType::WidgetContents:
        {
            // In order to render the widget contents we have
            // to force it to be drawn. Set this attribute
            // so we can "draw" the widget without the user seeing it.
            window->setAttribute(Qt::WA_DontShowOnScreen);
            // If the window was minimized we want to
            // set it back to that state when done.
            bool isShaded = window->isShaded();
            // Show the window so we can render it to a pixmap.
            window->showNormal();

            QPixmap pix;
            QWidget *w = window->widget();
            // If the widget is a graphics scene we need to use the grab method.
            if (QGraphicsView *view = qobject_cast<QGraphicsView *>(w))
            {
                pix = view->grab(view->sceneRect().toRect());
            }
            else
            {
                pix = QPixmap(w->size());
                w->render(&pix);
            }

            // Restore the state of the window.
            if (isShaded)
                window->showMinimized();
            window->setAttribute(Qt::WA_DontShowOnScreen, false);

            thumbnail = new QLabel();
            thumbnail->setPixmap(pix);
            break;
        }
        case ThumbnailType::WindowIcon:
        {
            QIcon icon = window->windowIcon();
            QPixmap pix = icon.pixmap(icon.actualSize(QSize(200, 200)));
            thumbnail = new QLabel();
            thumbnail->setPixmap(pix);
            break;
        }
    default:
        break;
    }

    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *title = new QLabel(window->windowTitle(), this);
    title->setAlignment(Qt::AlignHCenter);

    layout->addWidget(title);

    if (thumbnail)
    {
        thumbnail->setScaledContents(scaleThumbnail);
        layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
        layout->addWidget(thumbnail);
        layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
    }
    this->setLayout(layout);
}

void WindowWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        event->accept();
        emit this->clicked(this->m_window);
    }

    return QFrame::mouseReleaseEvent(event);
}
