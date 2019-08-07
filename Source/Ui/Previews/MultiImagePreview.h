#ifndef MULTIIMAGEPREVIEW_H
#define MULTIIMAGEPREVIEW_H

#include "../../Converters/ImageConverter.h"
#include "../../NosEnumTypes.h"
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QWidget>
#include <QtEndian>
#include <iostream>

namespace Ui {
    class MultiImagePreview;
}

class MultiImagePreview : public QWidget {
    Q_OBJECT

public:
    explicit MultiImagePreview(QList<QImage> *image, QWidget *parent = 0);
    ~MultiImagePreview();

private:
    Ui::MultiImagePreview *ui;
    QList<QImage> *images;
    ImageConverter converter;
    qint16 byteArrayToShort(QByteArray array);
    QImage generateImage();
    QPair<int,int> getResolution();

private slots:
    void onReplaced(QList<QImage> *newImages);
};

#endif // MULTIIMAGEPREVIEW_H