#ifndef ONEXTREETEXT_H
#define ONEXTREETEXT_H
#include "OnexTreeItem.h"

class NosTextOpener;
class OnexTreeText : public OnexTreeItem
{
private:
    int fileNmber;
    int isDat;
    NosTextOpener* opener;
public:
    OnexTreeText(QString name, NosTextOpener* opener, int fileNumber = 0, int isDat = 0, QByteArray content = QByteArray());
    virtual QWidget* onClicked() override;
    virtual void onExporAsOriginal() override;
    virtual ~OnexTreeText();
    int getFileNmber() const;
    int getIsDat() const;
};

#endif // ONEXTREETEXT_H
