#ifndef SINGLEIMAGEPREVIEW_H
#define SINGLEIMAGEPREVIEW_H

#include "../ImageConverter.h"
#include "../NosEnumTypes.h"
#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QWidget>
#include <QtEndian>
#include <iostream>

namespace Ui {
    class SingleImagePreview;
}

class SingleImagePreview : public QWidget {
    Q_OBJECT

    public:
    explicit SingleImagePreview(QImage image, QWidget *parent = 0);
    ~SingleImagePreview();

    private:
    Ui::SingleImagePreview *ui;
    ImageConverter converter;
    qint16 byteArrayToShort(QByteArray array);
    private slots:
    void onReplaced(QImage newImage);
};

#endif // SINGLEIMAGEPREVIEW_H
