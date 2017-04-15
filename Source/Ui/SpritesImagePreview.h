#ifndef SPRITESIMAGEPREVIEW_H
#define SPRITESIMAGEPREVIEW_H

#include <QWidget>
#include <QtEndian>
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include "../ImageConverter.h"
#include "../NosEnumTypes.h"

namespace Ui {
class SpritesImagePreview;
}

class SpritesImagePreview : public QWidget
{
    Q_OBJECT

public:
    explicit SpritesImagePreview(int spriteAmount, QByteArray header, QVector<QByteArray> contents, int headerValue, QWidget *parent = 0);
    ~SpritesImagePreview();
private:
    Ui::SpritesImagePreview *ui;
    ImageConverter converter;
    qint16 byteArrayToShort(QByteArray array);
    QImage createQImage(int w, int h, qint8 formatType, QByteArray &content);
    void showWarningMessage();
};

#endif // SPRITESIMAGEPREVIEW_H
