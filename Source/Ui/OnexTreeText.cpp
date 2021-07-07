#include "OnexTreeText.h"
#include "SingleTextFilePreview.h"
#include "../Openers/NosTextOpener.h"

int OnexTreeText::getFileNmber() const
{
    return fileNmber;
}

int OnexTreeText::getIsDat() const
{
    return isDat;
}

OnexTreeText::OnexTreeText(QString name, NosTextOpener *opener, int fileNumber, int isDat, QByteArray content) : OnexTreeItem(name, content)
{
    this->opener = opener;
    this->fileNmber = fileNumber;
    this->isDat = isDat;
}

QWidget *OnexTreeText::onClicked()
{
    SingleTextFilePreview* window = new SingleTextFilePreview(content);
    window->setWindowTitle(this->getName());

    return window;
}

void OnexTreeText::onExporAsOriginal()
{
    QString fileName = getSaveDirectory(this->getName(), "NOS Archive (*.NOS)");

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

void OnexTreeText::onExportSingle()
{
    QString fileExtension;
    QString fileName = this->getName();

    if (fileName.endsWith(".txt"))
        fileExtension = "Text documents (*.txt)";
    if (fileName.endsWith(".dat"))
        fileExtension = "Text documents (*.dat)";
    if (fileName.endsWith(".lst"))
        fileExtension = "Text documents (*.lst)";

    fileName = getSaveDirectory(fileName, fileExtension);

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(NULL, "Yeah", "Text file exported");
        file.write(content);
        file.close();
    }
    else
        QMessageBox::critical(NULL, "Woops", "Couldn't export that text file");
}

void OnexTreeText::onExportAll()
{
    QString directory = this->getSelectedDirectory();
    if (directory.isEmpty())
        return;

    int count = 0;
    for (int i = 0; i != this->childCount(); ++i)
    {
        OnexTreeText* item = static_cast<OnexTreeText*>(this->child(i));
        QFile file(directory + item->getName());
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(item->content);
            file.close();
            count++;
        }
    }
    QString text = "Saved " + QString::number(count) + " of " + QString::number(this->childCount()) + " files.";
    QMessageBox msgBox(QMessageBox::Information, tr("End of operation"), text);
    msgBox.exec();
}

OnexTreeText::~OnexTreeText()
{

}
