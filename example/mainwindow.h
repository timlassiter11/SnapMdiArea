#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addWindow();

    void on_thumbnailNone_radio_clicked();
    void on_thumbnailIcon_radio_clicked();
    void on_thumbnailContents_radio_clicked();
    void on_marginLeft_spin_valueChanged(int margin);
    void on_marginTop_spin_valueChanged(int margin);
    void on_marginRight_spin_valueChanged(int margin);
    void on_marginBottom_spin_valueChanged(int margin);
    void on_autoSnap_check_stateChanged(int arg1);
    void on_secondSnap_check_stateChanged(int arg1);
    void on_scaleThumbnail_check_stateChanged(int arg1);
    void on_actionEdit_Stylesheet_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
