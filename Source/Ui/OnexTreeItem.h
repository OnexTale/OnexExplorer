#ifndef ONEXTREEITEM_H
#define ONEXTREEITEM_H
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QObject>
#include <QTreeWidgetItem>
#include <QtEndian>

class OnexTreeItem : public QObject, public QTreeWidgetItem {
    Q_OBJECT
protected:
    QByteArray content;
    QString name;

public:
    OnexTreeItem(QString name, QByteArray content = QByteArray());
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
    virtual QWidget *getInfos() = 0;
    virtual ~OnexTreeItem();

    QString getSaveDirectory(QString name, QString filter);
    QMessageBox getMsgBox(QString title, QString message, QMessageBox::Icon icon);

public slots:
    virtual void setName(QString name);
    virtual void setContent(QByteArray content);
    virtual int onExport(QString directory);
    virtual int onExportRaw(QString directory);
    virtual int onExporAsOriginal();
    virtual int onReplace(QString directory);
    virtual int onReplaceRaw(QString directory);
    virtual void onDelete();
    virtual void actionClose();
};

#endif // ONEXTREEITEM_H
