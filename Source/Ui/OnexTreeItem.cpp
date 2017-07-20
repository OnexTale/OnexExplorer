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

QString OnexTreeItem::getSaveDirectory(QString filter)
{
    return QFileDialog::getSaveFileName(0, tr("Save as..."), "", filter);
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
    }
    else
    {
        QAction* exportSingleAction = new QAction(QObject::tr("Export"), contextMenu);
        contextMenu->addAction(exportSingleAction);
        QObject::connect(exportSingleAction, SIGNAL(triggered(bool)), this, SLOT(onExportSingle()));

        QAction* exportSingleToRawAction = new QAction(QObject::tr("Export to raw"), contextMenu);
        contextMenu->addAction(exportSingleToRawAction);
        QObject::connect(exportSingleToRawAction, SIGNAL(triggered(bool)), this, SLOT(onExporSingleRaw()));
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
    QString fileName = getSaveDirectory("Raw data (*.rawdata)");
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
