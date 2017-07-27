#ifndef ONEXTREETEXT_H
#define ONEXTREETEXT_H
#include "OnexTreeItem.h"

class OnexTreeText : public OnexTreeItem
{
private:
    int fileNmber;
    int isDat;
public:
    OnexTreeText(QString name, int fileNumber = 0, int isDat = 0, QByteArray content = QByteArray());
    virtual QWidget* onClicked() override;

    virtual ~OnexTreeText();
};

#endif // ONEXTREETEXT_H
