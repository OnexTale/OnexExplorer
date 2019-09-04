#include "SingleTextFilePreview.h"
#include "ui_SingleTextFilePreview.h"

SingleTextFilePreview::SingleTextFilePreview(QByteArray &item, const QString &encoding, QWidget *parent)
        : QWidget(parent), ui(new Ui::SingleTextFilePreview) {
    ui->setupUi(this);
    codec = QTextCodec::codecForName(encoding.toLocal8Bit());
    QString encodeContent = codec->toUnicode(item);
    ui->label_encoding->setText(encoding);
    ui->plainTextEdit->appendPlainText(encodeContent);
    ui->plainTextEdit->moveCursor(QTextCursor::Start);
}

SingleTextFilePreview::~SingleTextFilePreview() {
    delete ui;
}

void SingleTextFilePreview::onReplaced(const QByteArray &text) {
    ui->plainTextEdit->clear();
    QString encodeContent = codec->toUnicode(text);
    ui->plainTextEdit->appendPlainText(encodeContent);
    ui->plainTextEdit->moveCursor(QTextCursor::Start);
}