#ifndef SINGLEIMAGEPREVIEW_H
#define SINGLEIMAGEPREVIEW_H

#include <QWidget>
#include <QtEndian>
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include <QMenu>
#include <QFileDialog>
#include "../ImageConverter.h"
#include "../NosEnumTypes.h"

namespace Ui {
class SingleImagePreview;
}

class SingleImagePreview : public QWidget
{
    Q_OBJECT

public:
    explicit SingleImagePreview(QImage image, QWidget *parent = 0);
    ~SingleImagePreview();
private:
    Ui::SingleImagePreview *ui;
    ImageConverter converter;
    qint16 byteArrayToShort(QByteArray array);
    void showWarningMessage();
private slots:
    void showCustomMenu(const QPoint &pos);
    void exportImage();
    void onReplaced(QImage newImage);
};

#endif // SINGLEIMAGEPREVIEW_H
