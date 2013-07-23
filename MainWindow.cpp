#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    thread=new QThread();
    thread->start();
    oggSimplePlayer=NULL;
    oggSimplePlayer2=NULL;
}

MainWindow::~MainWindow()
{
    thread->quit();
    thread->wait();
    delete thread;
    if(oggSimplePlayer==NULL)
        delete oggSimplePlayer;
    if(oggSimplePlayer2==NULL)
        delete oggSimplePlayer2;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->thread->setEnabled(false);
    QThread *tempThread=NULL;
    if(ui->thread->isChecked())
        tempThread=this->thread;
    if(oggSimplePlayer==NULL)
        oggSimplePlayer=new QOggSimplePlayer(QCoreApplication::applicationDirPath()+"/file.ogg",tempThread);
    if(oggSimplePlayer2==NULL)
        oggSimplePlayer2=new QOggSimplePlayer(QCoreApplication::applicationDirPath()+"/file2.ogg",tempThread);
    oggSimplePlayer->start();
    oggSimplePlayer2->start();
}
