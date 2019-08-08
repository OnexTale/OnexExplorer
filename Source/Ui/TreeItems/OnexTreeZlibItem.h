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
    void setId(int id, bool update = false);
    void setCreationDate(QString date, bool update = false);
    void setCompressed(bool compressed, bool update = false);
    void setHeader(QString header, bool update = false);
};

#endif // ONEXTREEZLIBITEM_H
