#ifndef ONEXNSMCDATA_H
#define ONEXNSMCDATA_H


#include "OnexTreeZlibItem.h"

class OnexNSmcData : public OnexTreeZlibItem {
Q_OBJECT
public:
    OnexNSmcData(const QString &name, QByteArray content, NosZlibOpener *opener, int id = -1, int creationDate = 0,
                 bool compressed = false);
    OnexNSmcData(QJsonObject jo, NosZlibOpener *opener, const QString &directory);
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
