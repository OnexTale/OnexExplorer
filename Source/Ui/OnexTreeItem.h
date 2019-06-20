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
    virtual QByteArray getContent();
    bool hasParent();
    short fromLittleEndianToShort(QByteArray array);
    int fromLittleEndianToInt(QByteArray array);
    QByteArray fromShortToLittleEndian(short number);
    QByteArray fromIntToLittleEndian(int number);
    int getContentSize();

    virtual QString getName();
    virtual QWidget *onClicked() = 0;
    virtual ~OnexTreeItem();

    QString getSaveDirectory(QString name, QString filter);
    QMessageBox getMsgBox(QString title, QString message, QMessageBox::Icon icon);

    public slots:
    virtual int onExportAll(QString directory);
    virtual int onExportSingle(QString directory);
    virtual int onExporSingleRaw(QString directory);
    virtual int onExporAsOriginal();
    virtual int onReplace(QString directory);

    virtual void onDelete();
    virtual void actionClose();
};

#endif // ONEXTREEITEM_H
