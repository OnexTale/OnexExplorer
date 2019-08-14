#include "FileInfo.h"

FileInfo::FileInfo(QWidget *parent) : QWidget(parent) {
    grid = new QGridLayout();
    setLayout(grid);
    setMinimumWidth(225);
    setMaximumWidth(225);
    rows = 0;
}

QLineEdit *FileInfo::addStringLineEdit(const QString &title, const QString &value) {
    auto *label = new QLabel(title);
    grid->addWidget(label, rows, 0);
    auto *input = new QLineEdit(value);
    grid->addWidget(input, rows, 1);
    rows++;
    return input;
}

QLineEdit *FileInfo::addIntLineEdit(const QString &title, int value) {
    auto *label = new QLabel(title);
    grid->addWidget(label, rows, 0);
    auto *input = new QLineEdit(QString::number(value));
    grid->addWidget(input, rows, 1);
    rows++;
    return input;
}

QLineEdit *FileInfo::addFloatLineEdit(const QString &title, float value) {
    auto *label = new QLabel(title);
    grid->addWidget(label, rows, 0);
    auto *input = new QLineEdit(QString::number(value));
    grid->addWidget(input, rows, 1);
    rows++;
    return input;
}

QCheckBox *FileInfo::addCheckBox(const QString &title, bool value) {
    auto *cb = new QCheckBox(title);
    cb->setChecked(value);
    grid->addWidget(cb, rows, 1);
    rows++;
    return cb;
}

void FileInfo::update(const QString &title, const QString &value) {
    for (int i = 0; i < grid->rowCount(); i++) {
        if (grid->itemAtPosition(i, 0) == nullptr)
            continue;
        if (dynamic_cast<QLabel *>(grid->itemAtPosition(i, 0)->widget())->text() == title) {
            if (grid->itemAtPosition(i, 1) != nullptr) {
                auto *in = dynamic_cast<QLineEdit *>(grid->itemAtPosition(i, 1)->widget());
                in->setText(value);
                break;
            }
        }
    }
}

void FileInfo::update(const QString &title, int value) {
    update(title, QString::number(value));
}

void FileInfo::update(const QString &title, float value) {
    update(title, QString::number(value));
}

void FileInfo::update(const QString &title, bool value) {
    for (int i = 0; i < grid->rowCount(); i++) {
        if (grid->itemAtPosition(i, 1) == nullptr)
            continue;
        if (dynamic_cast<QCheckBox *>(grid->itemAtPosition(i, 1)->widget())->text() == title) {
            auto *cb = dynamic_cast<QCheckBox *>(grid->itemAtPosition(i, 1)->widget());
            cb->setChecked(value);
            break;
        }
    }
}

void FileInfo::replace(FileInfo *info) {
    emit updateInfo(info);
}
