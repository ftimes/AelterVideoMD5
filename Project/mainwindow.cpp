#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QDebug>
#include<QCheckBox>
#include<QCryptographicHash>
#include<QDateTime>
#include<QFileDevice>
#include<QFile>
QStringList fileNames;

QString getMD5(QString sourceFilePath)
{
    QFile sourceFile(sourceFilePath);
    qint64 fileSize = sourceFile.size();
    const qint64 bufferSize = 10240;

    if (sourceFile.open(QIODevice::ReadOnly))
    {
        char buffer[bufferSize];
        int bytesRead;
        int readSize = qMin(fileSize, bufferSize);

        QCryptographicHash hash(QCryptographicHash::Md5);

        while (readSize > 0 && (bytesRead = sourceFile.read(buffer, readSize)) > 0)
        {
            fileSize -= bytesRead;
            hash.addData(buffer, bytesRead);
            readSize = qMin(fileSize, bufferSize);
        }

        sourceFile.close();
        return QString(hash.result().toHex());
    }
    return QString();

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0,ui->tableWidget->width()/3);
    ui->tableWidget->setColumnWidth(1,ui->tableWidget->width()/3);
    ui->tableWidget->setColumnWidth(2,ui->tableWidget->width()/3);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    fileNames = QFileDialog::getOpenFileNames(
                this,
                tr("选择文件"),
                tr("D:/"),
                tr("video files(*.avi *.mp4 *.wmv)"));
    if (fileNames.isEmpty())
    {
        QMessageBox::warning(this, "Warning!", "Failed to open the video!");
    }
    else
    {
        ui->tableWidget->setRowCount(fileNames.length());
        for(int i=0;i<fileNames.length();++i)
        {
            ui->tableWidget->setItem(i,0,new QTableWidgetItem( fileNames[i]));
        }
    }

}

void aelter(QString in_path)
{

    int const BUFFER_SIZE=32;
    QFile file(in_path);
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();
    char p[BUFFER_SIZE];
    memset(p,0,BUFFER_SIZE);
    itoa(timeT,p,2);

    if(file.open(QIODevice::ReadWrite|QIODevice::Append))
    {

        int ret=0;
        QDataStream in(&file);
        in<<p;
        if(ret == -1)
        {
            qDebug()<<"Warning: ret == -1"<<endl;
        }
    }
    else
    {
        qDebug()<<"Can't open!"<<endl;
    }
    file.flush();
    file.close();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!ui->checkBox->isChecked())
    {
        //uchar *QFileDevice::map(qint64 offset, qint64 size, QFileDevice::MemoryMapFlags flags = NoOptions)
        for(int i=0;i<fileNames.length();++i)
        {
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(getMD5(fileNames[i])));
        }
        for(int i=0;i<fileNames.length();++i)
        {
            aelter(fileNames[i]);
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(getMD5(fileNames[i])));
        }
    }
    else
    {
        for(int i=0;i<fileNames.length();++i)
        {
            aelter(fileNames[i]);
        }
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    fileNames.clear();
    ui->tableWidget->clearContents();

}
