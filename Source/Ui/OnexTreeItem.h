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
protected:
    QByteArray content;
    QString name;
    INosFileOpener *opener;

public:
    OnexTreeItem(QString name, INosFileOpener *opener, QByteArray content = QByteArray());
    virtual QString getName();
    virtual QByteArray getContent();
    bool hasParent();
    short fromLittleEndianToShort(QByteArray array);
    int fromLittleEndianToInt(QByteArray array);
    float fromLittleEndianToFloat(QByteArray array);
    QByteArray fromShortToLittleEndian(short number);
    QByteArray fromIntToLittleEndian(int number);
    QByteArray fromFloatToLittleEndian(float number);
    int getContentSize();

    virtual QWidget *getPreview() = 0;
    virtual FileInfo *getInfos() = 0;
    virtual ~OnexTreeItem();

    QString getSaveDirectory(QString name, QString filter);
    QMessageBox getMsgBox(QString title, QString message, QMessageBox::Icon icon);

public slots:
    virtual void setName(QString name);
    virtual void setContent(QByteArray content);
    virtual int onExport(QString directory);
    virtual int onExportRaw(QString directory);
    virtual int onExportAsOriginal();
    virtual int onReplace(QString directory);
    virtual int onReplaceRaw(QString directory);
    virtual void onDelete();
    virtual void actionClose();
    
signals:
    void changeSignal(QString title, QString value);
    void changeSignal(QString title, int value);
    void changeSignal(QString title, float value);
    void changeSignal(QString title, bool value);
    void replaceInfo(FileInfo *info);
};

#endif // ONEXTREEITEM_H
