#include "FileInfo.h"

FileInfo::FileInfo(QWidget *parent) : QWidget(parent) {
    grid = new QGridLayout();
    setLayout(grid);
    setMinimumWidth(225);
    setMaximumWidth(225);
    rows = 0;
}

QLineEdit *FileInfo::addStringLineEdit(QString title, QString value) {
    QLabel *label = new QLabel(title);
    grid->addWidget(label, rows, 0);
    QLineEdit *input = new QLineEdit(value);
    grid->addWidget(input, rows, 1);
    rows++;
    return input;
}

QLineEdit *FileInfo::addIntLineEdit(QString title, int value) {
    QLabel *label = new QLabel(title);
    grid->addWidget(label, rows, 0);
    QLineEdit *input = new QLineEdit(QString::number(value));
    grid->addWidget(input, rows, 1);
    rows++;
    return input;
}

QLineEdit *FileInfo::addFloatLineEdit(QString title, float value) {
    QLabel *label = new QLabel(title);
    grid->addWidget(label, rows, 0);
    QLineEdit *input = new QLineEdit(QString::number(value));
    grid->addWidget(input, rows, 1);
    rows++;
    return input;
}

QCheckBox *FileInfo::addCheckBox(QString title, bool value) {
    QCheckBox *cb = new QCheckBox(title);
    cb->setChecked(value);
    grid->addWidget(cb, rows, 1);
    rows++;
    return cb;
}

void FileInfo::update(QString title, QString value) {
    for (int i = 0; i < grid->rowCount(); i++) {
        if (grid->itemAtPosition(i, 0) == nullptr)
            continue;
        if (static_cast<QLabel *>(grid->itemAtPosition(i, 0)->widget())->text() == title) {
            if (grid->itemAtPosition(i, 1) != nullptr) {
                QLineEdit *in = static_cast<QLineEdit *>(grid->itemAtPosition(i, 1)->widget());
                in->setText(value);
                break;
            }
        }
    }
}

void FileInfo::update(QString title, int value) {
    update(title, QString::number(value));
}

void FileInfo::update(QString title, float value) {
    update(title, QString::number(value));
}

void FileInfo::update(QString title, bool value) {
    for (int i = 0; i < grid->rowCount(); i++) {
        if (grid->itemAtPosition(i, 1) == nullptr)
            continue;
        if (static_cast<QCheckBox *>(grid->itemAtPosition(i, 1)->widget())->text() == title) {
            QCheckBox *cb = static_cast<QCheckBox *>(grid->itemAtPosition(i, 1)->widget());
            cb->setChecked(value);
            break;
        }
    }
}

void FileInfo::replace(FileInfo *info) {
    emit updateInfo(info);
}
