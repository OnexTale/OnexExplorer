#include "OnexTreeItem.h"

OnexTreeItem::OnexTreeItem(QString name, QByteArray content)
{
    this->name = name;
    this->content = content;
    this->setText(0, name);
}

QString OnexTreeItem::getSelectedDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(0, tr("Select Directory"));
    if (dir.isEmpty())
        return dir;

    return dir + "/";
}

QString OnexTreeItem::getSaveDirectory(QString name, QString filter)
{
    return QFileDialog::getSaveFileName(0, tr("Save as..."), name, filter);
}

QString OnexTreeItem::getOpenDirectory(QString filter)
{
    return QFileDialog::getOpenFileName(0, tr("Open..."), "", filter);
}

QByteArray OnexTreeItem::getContent()
{
    return content;
}

bool OnexTreeItem::hasParent()
{
    return QTreeWidgetItem::parent();
}

short OnexTreeItem::fromLittleEndianToShort(QByteArray array)
{
    return qFromLittleEndian<qint16>(reinterpret_cast<const uchar *>(array.data()));
}

int OnexTreeItem::getContentSize()
{
    return content.size();
}

QString OnexTreeItem::getName()
{
    return name;
}

QMenu *OnexTreeItem::getContextMenu()
{
    QMenu* contextMenu = new QMenu();
    if (!hasParent())
    {
        QAction* exportAllAction = new QAction(QObject::tr("Export all"), contextMenu);
        contextMenu->addAction(exportAllAction);
        QObject::connect(exportAllAction, SIGNAL(triggered(bool)), this, SLOT(onExportAll()));

        QAction* exportOriginalAction = new QAction(QObject::tr("Export as original"), contextMenu);
        contextMenu->addAction(exportOriginalAction);
        QObject::connect(exportOriginalAction, SIGNAL(triggered(bool)), this, SLOT(onExporAsOriginal()));

        QAction* closeThisItem = new QAction(QObject::tr("Close"), contextMenu);
        contextMenu->addAction(closeThisItem);
        QObject::connect(closeThisItem, SIGNAL(triggered(bool)), this, SLOT(actionClose()));

    }
    else
    {
        QAction* exportSingleAction = new QAction(QObject::tr("Export"), contextMenu);
        contextMenu->addAction(exportSingleAction);
        QObject::connect(exportSingleAction, SIGNAL(triggered(bool)), this, SLOT(onExportSingle()));

        QAction* exportSingleToRawAction = new QAction(QObject::tr("Export to raw"), contextMenu);
        contextMenu->addAction(exportSingleToRawAction);
        QObject::connect(exportSingleToRawAction, SIGNAL(triggered(bool)), this, SLOT(onExporSingleRaw()));

        QAction* replaceAction = new QAction(QObject::tr("Replace"), contextMenu);
        contextMenu->addAction(replaceAction);
        QObject::connect(replaceAction, SIGNAL(triggered(bool)), this, SLOT(onReplace()));
    }

    return contextMenu;
}

OnexTreeItem::~OnexTreeItem()
{

}

void OnexTreeItem::onExportAll()
{
    QMessageBox::warning(NULL, tr("Not yet"), tr("This isn't implemented yet"));
}

void OnexTreeItem::onExportSingle()
{
    QMessageBox::warning(NULL, tr("Not yet"), tr("This isn't implemented yet"));
}

void OnexTreeItem::onExporSingleRaw()
{
    QString fileName = getSaveDirectory(this->getName(), "Raw data (*.rawdata)");
    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(tr(".rawdata")))
        fileName += ".rawdata";

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    if (file.write(this->getContent()) == -1)
        QMessageBox::critical(NULL, tr("Woops"), tr("Couldn't save that file"));
    else
        QMessageBox::information(NULL, tr("Yeah"), tr("File exported"));
    file.close();
}

void OnexTreeItem::onExporAsOriginal()
{
    QMessageBox::warning(NULL, tr("Not yet"), tr("This isn't implemented yet"));
}

void OnexTreeItem::onReplace()
{
    QMessageBox::warning(NULL, tr("Not yet"), tr("This isn't implemented yet"));
}

void OnexTreeItem::actionClose()
{
    QList<QTreeWidgetItem*> selectedItems = this->treeWidget()->selectedItems();

    foreach (auto& item, selectedItems)
        delete item;
}
