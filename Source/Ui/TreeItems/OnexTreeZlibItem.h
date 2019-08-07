#ifndef ONEXTREEZLIBITEM_H
#define ONEXTREEZLIBITEM_H
#include "../../Openers/NosZlibOpener.h"
#include "../OnexTreeItem.h"
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

class OnexTreeZlibItem : public OnexTreeItem {
    Q_OBJECT
 protected:
    int id;
    int creationDate;
    bool compressed;
    QByteArray header;

public:
    OnexTreeZlibItem(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                     bool compressed);
    virtual QWidget *getPreview() override;
    virtual FileInfo *getInfos() override;
    int getId();
    int getCreationDate();
    QString getDateAsString();
    bool isCompressed();
    QByteArray getHeader();
    virtual ~OnexTreeZlibItem();

public slots:
    void setId(int id);
    void setCreationDate(QString date);
    void setCompressed(bool compressed);
    void setHeader(QString header);
};

#endif // ONEXTREEZLIBITEM_H
