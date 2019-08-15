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
    explicit MultiImagePreview(QList<QImage> *image, QWidget *parent = nullptr);
    ~MultiImagePreview() override;
private slots:
    void onReplaced(QList<QImage> *newImages);
private:
    Ui::MultiImagePreview *ui;
    QList<QImage> *images;
    QImage generateImage();
    QPair<int, int> getResolution();
};

#endif // MULTIIMAGEPREVIEW_H