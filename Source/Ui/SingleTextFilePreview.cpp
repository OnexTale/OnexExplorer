#include "SingleTextFilePreview.h"
#include "ui_SingleTextFilePreview.h"

SingleTextFilePreview::SingleTextFilePreview(QByteArray content, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleTextFilePreview)
{
    ui->setupUi(this);
    ui->plainTextEdit->appendPlainText(QString::fromLocal8Bit(content));
}

SingleTextFilePreview::~SingleTextFilePreview()
{
    delete ui;
}
