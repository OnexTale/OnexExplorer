#ifndef ONEXNSMNDATA_H
#define ONEXNSMNDATA_H

#include "../OnexTreeItem.h"

class OnexNSmnData : public OnexTreeItem {
Q_OBJECT

public:
    OnexNSmnData(const QString &name, int creationDate, INosFileOpener *opener, QByteArray content);
    QWidget *getPreview() override;
    QString getExportExtension() override;
    int getCreationDate();
public slots:
    int onExport(QString directory) override;
    int onReplace(QString directory) override;
    int afterReplace(QByteArray content) override;
    void setCreationDate(const QString &date, bool update = false);
signals:
    void replaceSignal(QByteArray text);
protected:
    int creationDate;
    void loadJson(QJsonArray array);
    FileInfo *generateInfos() override;
    QString getDateAsString();
};


#endif // ONEXNSMNDATA_H