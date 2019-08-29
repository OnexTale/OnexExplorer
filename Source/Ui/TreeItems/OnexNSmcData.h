#ifndef ONEXNSMCDATA_H
#define ONEXNSMCDATA_H


#include "OnexTreeZlibItem.h"

class OnexNSmcData : public OnexTreeZlibItem {
Q_OBJECT
public:
    OnexNSmcData(const QString &name, QByteArray content, NosZlibOpener *opener, int id, int creationDate,
                 bool compressed);
    ~OnexNSmcData() override;
    QWidget *getPreview() override;
    QString getExportExtension() override;
public slots:
    int afterReplace(QByteArray content) override;
signals:
    void replaceSignal(QByteArray text);
protected:
    int saveAsFile(const QString &path, QByteArray content) override;
    QJsonObject toJson();
    QByteArray fromJson(const QByteArray &data);
};


#endif //ONEXNSMCDATA_H
