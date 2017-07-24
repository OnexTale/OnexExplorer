#include "OnexTreeZlibItem.h"

OnexTreeZlibItem::OnexTreeZlibItem(QString name, QByteArray content, NosZlibOpener *opener, int id, int creationDate, bool compressed) :
    OnexTreeItem(name, content),
    id(id),
    creationDate(creationDate),
    compressed(compressed),
    opener(opener)
{

}

QWidget *OnexTreeZlibItem::onClicked()
{
    return nullptr;
}

void OnexTreeZlibItem::onExporAsOriginal()
{
    QString fileName = getSaveDirectory("NOS Archive (*.NOS)");

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".NOS"))
        fileName += ".NOS";

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(NULL, "Woops", "Couldn't open this file for writing");
        return;
    }

    if (file.write(opener->encrypt(this)) == -1)
    {
        QMessageBox::critical(NULL, "Woops", "Couldn't write to this file");
        return;
    }

    QMessageBox::information(NULL, "Yeah", "File exported into .NOS");

}

int OnexTreeZlibItem::getId()
{
    return id;
}

int OnexTreeZlibItem::getCreationDate()
{
    return creationDate;
}

bool OnexTreeZlibItem::isCompressed()
{
    return compressed;
}

OnexTreeZlibItem::~OnexTreeZlibItem()
{

}
