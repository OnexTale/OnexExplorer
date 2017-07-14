#ifndef ONEXTREETEXT_H
#define ONEXTREETEXT_H
#include "OnexTreeItem.h"
#include "SingleTextFilePreview.h"

class OnexTreeText : public OnexTreeItem
{
public:
    OnexTreeText(QString name, QByteArray content = QByteArray());
    virtual QWidget* onClicked() override;
};

#endif // ONEXTREETEXT_H
