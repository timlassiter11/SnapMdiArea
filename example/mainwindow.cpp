#include "mainwindow.h"
#include "snapmdiarea.h"
#include "./ui_mainwindow.h"

#include <QMdiSubWindow>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ThumbnailType thumbnailType = ui->snapMdiArea->thumbnailType();
    switch (thumbnailType)
    {
    case ThumbnailType::None:
        ui->thumbnailNone_radio->setChecked(true);
        break;
    case ThumbnailType::WindowIcon:
        ui->thumbnailIcon_radio->setChecked(true);
        break;
    case ThumbnailType::WidgetContents:
        ui->thumbnailContents_radio->setChecked(true);
        break;
    }

    QMargins margins = ui->snapMdiArea->snapMargins();
    ui->marginLeft_spin->setValue(margins.left());
    ui->marginTop_spin->setValue(margins.top());
    ui->marginRight_spin->setValue(margins.right());
    ui->marginBottom_spin->setValue(margins.bottom());

    ui->autoSnap_check->setChecked(ui->snapMdiArea->autoSnap());
    ui->secondSnap_check->setChecked(ui->snapMdiArea->secondSnap());
    ui->scaleThumbnail_check->setChecked(ui->snapMdiArea->scaleThumbnail());

    connect(ui->actionAdd_Window, &QAction::triggered, this, &MainWindow::addWindow);
    connect(ui->actionTile_Windows, &QAction::triggered, this->ui->snapMdiArea, &SnapMdiArea::tileSubWindows);

    for (int i = 0; i < 4; ++i)
        this->addWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addWindow()
{
    int windows = ui->snapMdiArea->subWindowList().length() + 1;
    QString text = QString("Window %1").arg(windows);
    QMdiSubWindow *sw = ui->snapMdiArea->addSubWindow(new QLabel(text));
    sw->setWindowTitle(text);
    sw->setWindowIcon(QIcon(":/icons/gear.png"));
    sw->showNormal();
    sw->resize(200, 200);
}

void MainWindow::on_thumbnailNone_radio_clicked()
{
    ui->snapMdiArea->setThumbnailType(ThumbnailType::None);
}

void MainWindow::on_thumbnailIcon_radio_clicked()
{
    ui->snapMdiArea->setThumbnailType(ThumbnailType::WindowIcon);
}

void MainWindow::on_thumbnailContents_radio_clicked()
{
    ui->snapMdiArea->setThumbnailType(ThumbnailType::WidgetContents);
}

void MainWindow::on_marginLeft_spin_valueChanged(int margin)
{
    QMargins margins = this->ui->snapMdiArea->snapMargins();
    margins.setLeft(margin);
    this->ui->snapMdiArea->setSnapMargins(margins);
}

void MainWindow::on_marginTop_spin_valueChanged(int margin)
{
    QMargins margins = this->ui->snapMdiArea->snapMargins();
    margins.setTop(margin);
    this->ui->snapMdiArea->setSnapMargins(margins);
}

void MainWindow::on_marginRight_spin_valueChanged(int margin)
{
    QMargins margins = this->ui->snapMdiArea->snapMargins();
    margins.setRight(margin);
    this->ui->snapMdiArea->setSnapMargins(margins);
}

void MainWindow::on_marginBottom_spin_valueChanged(int margin)
{
    QMargins margins = this->ui->snapMdiArea->snapMargins();
    margins.setBottom(margin);
    this->ui->snapMdiArea->setSnapMargins(margins);
}

void MainWindow::on_autoSnap_check_stateChanged(int arg1)
{
    ui->snapMdiArea->setAutoSnap((bool)arg1);
}

void MainWindow::on_secondSnap_check_stateChanged(int arg1)
{
    ui->snapMdiArea->setSecondSnap((bool)arg1);
}

void MainWindow::on_scaleThumbnail_check_stateChanged(int arg1)
{
    ui->snapMdiArea->setScaleThumbnail((bool)arg1);
}

void MainWindow::on_actionEdit_Stylesheet_triggered()
{
    bool ok;
    QString stylesheet = QInputDialog::getMultiLineText(this, "Edit Stylesheet", "", this->ui->snapMdiArea->styleSheet(), &ok);
    if (ok)
        ui->snapMdiArea->setStyleSheet(stylesheet);
}

