#ifndef ONEXNSMNDATA_H
#define ONEXNSMNDATA_H

#include "OnexJsonTreeItem.h"

class OnexNSmnData : public OnexJsonTreeItem {
Q_OBJECT

public:
    OnexNSmnData(const QString &name, int creationDate, INosFileOpener *opener, QByteArray content);
    int getCreationDate();
public slots:
    void setCreationDate(const QString &date, bool update = false);
protected:
    int creationDate;
    void loadJson(QJsonArray array) override;
    FileInfo *generateInfos() override;
    QString getDateAsString();
};


#endif // ONEXNSMNDATA_H