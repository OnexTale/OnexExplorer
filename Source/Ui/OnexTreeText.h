#ifndef ONEXTREETEXT_H
#define ONEXTREETEXT_H
#include "OnexTreeItem.h"

class NosTextOpener;
class OnexTreeText : public OnexTreeItem {
    Q_OBJECT
private:
    int fileNumber;
    int isDat;
    NosTextOpener *opener;

public:
    OnexTreeText(QString name, NosTextOpener *opener, int fileNumber = 0, int isDat = 0,
                 QByteArray content = QByteArray());
    virtual QWidget *getPreview() override;
    virtual QWidget *getInfos() override;
    virtual int onExporAsOriginal() override;
    virtual ~OnexTreeText();
    int getFileNumber() const;
    int getIsDat() const;

public slots:
    virtual int onExport(QString directory);
    virtual int onReplace(QString directory);
    void setFileNumber(int number);
    void setFileNumber(QString fileNumber);
    void setIsDat(bool isDat);

signals:
    void replaceSignal(QByteArray text);
};

#endif // ONEXTREETEXT_H
