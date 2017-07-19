#ifndef ONEXTREEITEM_H
#define ONEXTREEITEM_H
#include <QTreeWidgetItem>
#include <QtEndian>
#include <QMenu>
#include <QMessageBox>
#include <QObject>
#include <QFileDialog>

class OnexTreeItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
protected:
    QByteArray content;
    QString name;

    QString getSelectedDirectory();
public:
    OnexTreeItem(QString name, QByteArray content = QByteArray());
    QByteArray getContent();
    bool hasParent();
    short fromLittleEndianToShort(QByteArray array);

    virtual QString getName();
    virtual QMenu* getContextMenu();
    virtual QWidget *onClicked() = 0;
    virtual ~OnexTreeItem();

private slots:
    virtual void onExportAll();
};

#endif // ONEXTREEITEM_H
