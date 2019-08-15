#ifndef ONEXNSMNDATA_H
#define ONEXNSMNDATA_H

#include "OnexJsonTreeItem.h"

class OnexNSmnData : public OnexJsonTreeItem {
Q_OBJECT

public:
    OnexNSmnData(QByteArray header, QString name, int creationDate, INosFileOpener *opener, QByteArray content = QByteArray());
    QByteArray getHeader();
    int getCreationDate();
public slots:
    void setHeader(QString QString, bool update = false);
    void setCreationDate(const QString &date, bool update = false);
protected:
    QByteArray header;
    int creationDate;
    FileInfo *generateInfos() override;
    QString getDateAsString();
};


#endif // ONEXNSMNDATA_H