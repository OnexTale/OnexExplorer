#include "SingleImagePreview.h"
#include "ui_SingleImagePreview.h"

SingleImagePreview::SingleImagePreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleImagePreview)
{
    ui->setupUi(this);
}

SingleImagePreview::~SingleImagePreview()
{
    delete ui;
}
