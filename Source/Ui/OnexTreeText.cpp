#include "OnexTreeText.h"
#include "SingleTextFilePreview.h"

OnexTreeText::OnexTreeText(QString name, int fileNumber, int isDat, QByteArray content) : OnexTreeItem(name, content)
{
    this->fileNmber = fileNumber;
    this->isDat = isDat;
}

QWidget *OnexTreeText::onClicked()
{
    SingleTextFilePreview* window = new SingleTextFilePreview(content);
    window->setWindowTitle(this->getName());

    return window;
}

OnexTreeText::~OnexTreeText()
{

}
