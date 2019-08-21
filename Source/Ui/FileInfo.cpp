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
    label->setMinimumWidth(50);
    grid->addWidget(label, rows, 0);
    auto *input = new QLineEdit(value);
    grid->addWidget(input, rows, 1);
    rows++;
    return input;
}

QLineEdit *FileInfo::addIntLineEdit(const QString &title, int value) {
    auto *label = new QLabel(title);
    label->setMinimumWidth(50);
    grid->addWidget(label, rows, 0);
    auto *input = new QLineEdit(QString::number(value));
    grid->addWidget(input, rows, 1);
    rows++;
    return input;
}

QLineEdit *FileInfo::addFloatLineEdit(const QString &title, float value) {
    auto *label = new QLabel(title);
    label->setMinimumWidth(50);
    grid->addWidget(label, rows, 0);
    auto *input = new QLineEdit(QString::number(value));
    grid->addWidget(input, rows, 1);
    rows++;
    return input;
}

QCheckBox *FileInfo::addCheckBox(const QString &title, bool value) {
    auto *cb = new QCheckBox(title);
    cb->setChecked(value);
    grid->addWidget(cb, rows, 0, 1, 2);
    rows++;
    return cb;
}

QComboBox *FileInfo::addSelection(const QString &title, const QStringList &items, int value) {
    auto *label = new QLabel(title);
    label->setMinimumWidth(50);
    grid->addWidget(label, rows, 0);
    auto *box = new QComboBox();
    for (const QString &s : items) {
        box->addItem(s);
    }
    box->setCurrentIndex(value);
    grid->addWidget(box, rows, 1);
    rows++;
    return box;
}

QPushButton *FileInfo::addReplaceButton(const QString &title) {
    auto *button = new QPushButton(title);
    connect(button, &QPushButton::clicked, [=]() { emit replaceFile(); });
    grid->addWidget(button, rows, 0, 1, 2);
    rows++;
    return button;
}

QPushButton *FileInfo::addReplaceRawButton(const QString &title) {
    auto *button = new QPushButton(title);
    connect(button, &QPushButton::clicked, [=]() { emit replaceRawFile(); });
    grid->addWidget(button, rows, 0, 1, 2);
    rows++;
    return button;
}

void FileInfo::update(const QString &title, const QString &value) {
    for (int i = 0; i < grid->rowCount(); i++) {
        if (grid->itemAtPosition(i, 0) == nullptr || dynamic_cast<QLabel *>(grid->itemAtPosition(i, 0)->widget()) == nullptr)
            continue;
        if (dynamic_cast<QLabel *>(grid->itemAtPosition(i, 0)->widget())->text() == title) {
            if (grid->itemAtPosition(i, 1) != nullptr) {
                auto *in = dynamic_cast<QLineEdit *>(grid->itemAtPosition(i, 1)->widget());
                if (in != nullptr) {
                    in->setText(value);
                    break;
                } else {
                    auto *box = dynamic_cast<QComboBox *>(grid->itemAtPosition(i, 1)->widget());
                    if (box != nullptr) {
                        box->setCurrentIndex(value.toInt());
                        break;
                    }
                }
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
        if (grid->itemAtPosition(i, 0) == nullptr)
            continue;
        auto *current = dynamic_cast<QCheckBox *>(grid->itemAtPosition(i, 0)->widget());
        if (current != nullptr && current->text() == title) {
            auto *cb = dynamic_cast<QCheckBox *>(grid->itemAtPosition(i, 0)->widget());
            cb->setChecked(value);
            break;
        }
    }
}

void FileInfo::replace(FileInfo *info) {
    emit updateInfo(info);
}