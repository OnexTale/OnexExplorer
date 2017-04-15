#ifndef SINGLEMODELPREVIEW_H
#define SINGLEMODELPREVIEW_H

#include <QWidget>
#include <QtEndian>
#include <QDebug>
#include <iostream>
#include <QMessageBox>
#include "../NosEnumTypes.h"

namespace Ui {
class SingleModelPreview;
}

class SingleModelPreview : public QWidget
{
    Q_OBJECT

public:
    explicit SingleModelPreview(QByteArray content, int headerValue, QWidget *parent = 0);
    ~SingleModelPreview();
private:
    Ui::SingleModelPreview *ui;
};

#endif // SINGLEMODELPREVIEW_H
