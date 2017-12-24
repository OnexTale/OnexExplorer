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

OnexTreeText::~OnexTreeText()
{

}
