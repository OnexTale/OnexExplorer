#include "SingleTextFilePreview.h"
#include "ui_SingleTextFilePreview.h"

SingleTextFilePreview::SingleTextFilePreview(QByteArray &item, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleTextFilePreview)
{
    ui->setupUi(this);
    QTextCodec *codec = QTextCodec::codecForName("Windows-1250");
    QString encodeContent = codec->toUnicode(item);
    ui->plainTextEdit->appendPlainText(encodeContent);
}

SingleTextFilePreview::~SingleTextFilePreview()
{
    delete ui;
}
