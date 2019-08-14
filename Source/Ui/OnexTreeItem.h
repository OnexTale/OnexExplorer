#ifndef ONEXTREEITEM_H
#define ONEXTREEITEM_H

#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QObject>
#include <QTreeWidgetItem>
#include <QtEndian>
#include "../Openers/INosFileOpener.h"
#include "FileInfo.h"

class OnexTreeItem : public QObject, public QTreeWidgetItem {
Q_OBJECT
public:
    OnexTreeItem(const QString &name, INosFileOpener *opener, QByteArray content = QByteArray());
    ~OnexTreeItem() override;
    virtual QWidget *getPreview() = 0;
    virtual FileInfo *getInfos();
    bool hasParent();
    virtual QString getName();
    virtual QByteArray getContent();
    int getContentSize();
    virtual QString getExportExtension();
    QString getExportExtensionFilter();
    short fromLittleEndianToShort(QByteArray array);
    int fromLittleEndianToInt(QByteArray array);
    float fromLittleEndianToFloat(QByteArray array);
    QByteArray fromShortToLittleEndian(short number);
    QByteArray fromIntToLittleEndian(int number);
    QByteArray fromFloatToLittleEndian(float number);
public slots:
    virtual void setName(QString name);
    virtual int onExportAsOriginal(QString path);
    virtual void setContent(QByteArray content);
    virtual int onExport(QString directory);
    virtual int onExportRaw(QString directory);
    virtual int onReplace(QString directory);
    virtual int onReplaceRaw(QString directory);
    virtual int afterReplace(QByteArray content);
signals:
    void changeSignal(QString title, QString value);
    void changeSignal(QString title, int value);
    void changeSignal(QString title, float value);
    void changeSignal(QString title, bool value);
    void replaceInfo(FileInfo *info);
protected:
    QByteArray content;
    QString name;
    INosFileOpener *opener;
    virtual FileInfo *generateInfos() = 0;
    virtual int saveAsFile(const QString &path, QByteArray content = QByteArray());
    QString getCorrectPath(QString input, QString extension = QString());
};

#endif // ONEXTREEITEM_H
