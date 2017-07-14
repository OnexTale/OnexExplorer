#include "OnexTreeText.h"

OnexTreeText::OnexTreeText(QString name, QByteArray content) : OnexTreeItem(name, content)
{

}

QWidget *OnexTreeText::onClicked()
{
    SingleTextFilePreview* window = new SingleTextFilePreview(shared_from_this());
    window->setWindowTitle(this->getName());

    return window;
}
