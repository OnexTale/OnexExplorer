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

    return contextMenu;
}

OnexTreeItem::~OnexTreeItem()
{

}

void OnexTreeItem::onExportAll()
{
    QMessageBox msgBox(QMessageBox::Warning, QObject::tr("Not yet"), QObject::tr("This isn't implemented yet"));
    msgBox.exec();
}
