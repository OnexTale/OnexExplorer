#ifndef SINGLEIMAGEPREVIEW_H
#define SINGLEIMAGEPREVIEW_H

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
    class SingleImagePreview;
}
class SingleImagePreview : public QWidget {
Q_OBJECT
public:
    explicit SingleImagePreview(const QImage &image, QWidget *parent = nullptr);
    ~SingleImagePreview() override;
private slots:
    void onReplaced(const QImage &newImage);
private:
    Ui::SingleImagePreview *ui;
    ImageConverter converter;
};

#endif // SINGLEIMAGEPREVIEW_H
