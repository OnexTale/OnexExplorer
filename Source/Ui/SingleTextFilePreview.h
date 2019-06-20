#ifndef SINGLETEXTFILEPREVIEW_H
#define SINGLETEXTFILEPREVIEW_H

#include "OnexTreeText.h"
#include <QTextCodec>
#include <QWidget>
#include <iostream>

namespace Ui {
    class SingleTextFilePreview;
}

class SingleTextFilePreview : public QWidget {
    Q_OBJECT

public:
    explicit SingleTextFilePreview(QByteArray &item, QWidget *parent = 0);
    ~SingleTextFilePreview();

private:
    Ui::SingleTextFilePreview *ui;

private slots:
    void onReplaced(QByteArray text);
};

#endif // SINGLETEXTFILEPREVIEW_H
