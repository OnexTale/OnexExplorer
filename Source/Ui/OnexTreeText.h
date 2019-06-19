#ifndef ONEXTREETEXT_H
#define ONEXTREETEXT_H
#include "OnexTreeItem.h"

class NosTextOpener;
class OnexTreeText : public OnexTreeItem {
    private:
    int fileNmber;
    int isDat;
    NosTextOpener *opener;

    public:
    OnexTreeText(QString name, NosTextOpener *opener, int fileNumber = 0, int isDat = 0,
                 QByteArray content = QByteArray());
    virtual QWidget *onClicked() override;
    virtual int onExporAsOriginal() override;
    virtual ~OnexTreeText();
    int getFileNmber() const;
    int getIsDat() const;
    public slots:
    virtual int onExportSingle(QString directory);
    virtual int onExportAll(QString directory);
};

#endif // ONEXTREETEXT_H
