#include "SingleTextFilePreview.h"
#include "ui_SingleTextFilePreview.h"

SingleTextFilePreview::SingleTextFilePreview(std::shared_ptr<OnexTreeItem> item, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleTextFilePreview)
{
    ui->setupUi(this);
    ui->plainTextEdit->appendPlainText(QString::fromLocal8Bit(item->getContent()));
}

SingleTextFilePreview::~SingleTextFilePreview()
{
    delete ui;
}
