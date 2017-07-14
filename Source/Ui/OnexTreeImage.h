#ifndef ONEXTREEIMAGE_H
#define ONEXTREEIMAGE_H
#include "OnexTreeItem.h"

class OnexTreeImage : public OnexTreeItem
{
private:

public:
    OnexTreeImage(QString name, QByteArray content = QByteArray());

    virtual QWidget *onClicked() override;
};

#endif // ONEXTREEIMAGE_H
