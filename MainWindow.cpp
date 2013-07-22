#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    oggSimplePlayer(QCoreApplication::applicationDirPath()+"/file.ogg")
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    oggSimplePlayer.start();
}
