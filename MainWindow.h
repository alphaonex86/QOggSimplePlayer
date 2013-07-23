#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QOggSimplePlayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();
private:
    Ui::MainWindow *ui;
    QOggSimplePlayer *oggSimplePlayer;
    QOggSimplePlayer *oggSimplePlayer2;
    QThread * thread;
};

#endif // MAINWINDOW_H
