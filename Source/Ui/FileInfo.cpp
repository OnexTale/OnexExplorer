#include "FileInfo.h"
#include "ui_FileInfo.h"

FileInfo::FileInfo(OnexTreeItem *item, QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::FileInfo)
{    
    ui->setupUi(this);

    ui->name->setText(item->getName());
    ui->header->setText(QString::number(item->getHeaderValue()));
    ui->sizeb->setText(QString::number(item->getContent().size()));
    int timestamp = item->getTimestamp();
    int year = (timestamp & 0xFFFF0000) >> 0x10;
    int month = (timestamp & 0xFF00) >> 0x08;
    int day = timestamp & 0xFF;

    QString date = QString("%1/%2/%3").arg(day, 2, 16, QChar('0')).arg(month, 2, 16, QChar('0')).arg(year, 4, 16, QChar('0'));
    ui->date->setText(date);
}

FileInfo::~FileInfo()
{
    delete ui;
}
