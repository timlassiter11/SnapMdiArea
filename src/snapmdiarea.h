#ifndef SNAPMDIAREA_H
#define SNAPMDIAREA_H

#include "SnapMdiArea_global.h"
#include "windowselector.h"
#include <QMdiArea>
#include <QRect>
#include <QPoint>

class QRubberBand;
class OverlayWidget;

class SNAPMDIAREA_EXPORT SnapMdiArea : public QMdiArea
{
public:
    SnapMdiArea(QWidget *parent = nullptr);

    ThumbnailType thumbnailType() const;
    QMargins snapMargins() const;
    bool autoSnap() const;
    bool secondSnap() const;
    bool scaleThumbnail() const;

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void setThumbnailType(ThumbnailType thumbnailType);
    void setSnapMargins(QMargins margins);
    void setAutoSnap(bool enable);
    void setSecondSnap(bool enable);
    void setScaleThumbnail(bool enable);

protected:
    virtual void childEvent(QChildEvent *childEvent) override;
    virtual void resizeEvent(QResizeEvent *resizeEvent) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void releaseSnap();
    void windowSelected(QMdiSubWindow *window);

private:
    bool m_autoSnap;
    bool m_secondSnap;
    QMargins m_snapMargins;

    bool m_dragging;
    QRect m_snapRect;
    QRubberBand *m_rubberBand;
    OverlayWidget *m_selectorOverlay;
    WindowSelector *m_windowSelector;

};

#endif // SNAPMDIAREA_H
