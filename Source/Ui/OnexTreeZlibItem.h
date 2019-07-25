#ifndef ONEXTREEZLIBITEM_H
#define ONEXTREEZLIBITEM_H
#include "../Openers/NosZlibOpener.h"
#include "OnexTreeItem.h"
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
    NosZlibOpener *opener;

public:
    OnexTreeZlibItem(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                     bool compressed);
    virtual QWidget *getPreview() override;
    virtual QWidget *getInfos() override;
    virtual int onExporAsOriginal() override;
    int getId();
    int getCreationDate();
    bool isCompressed();
    QByteArray getHeader();
    virtual ~OnexTreeZlibItem();

public slots:
    void setId(int id);
    void setId(QString id);
    void setCreationDate(QString date);
    void setCompressed(bool compressed);
    void setHeader(QString header);
};

#endif // ONEXTREEZLIBITEM_H
